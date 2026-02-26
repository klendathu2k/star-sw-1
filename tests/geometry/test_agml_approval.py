#!/usr/bin/env python3
"""AGML compiler approval tests.

Recompiles every XML file in StarVMC/Geometry/ and compares the output
against the golden files in tests/geometry/approved/.

Usage:
    # Normal (comparison) mode:
    python -m pytest tests/geometry/test_agml_approval.py -v

    # Update goldens after intentional compiler change:
    python -m pytest tests/geometry/test_agml_approval.py --update-approvals -v

Requirements: pytest
"""
import difflib
import glob
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

import pytest

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GEOMETRY_DIR = os.path.join(REPO_ROOT, 'StarVMC', 'Geometry')
APPROVED_DIR = os.path.join(REPO_ROOT, 'tests', 'geometry', 'approved')
AGML_PARSER = os.path.join(REPO_ROOT, 'mgr', 'agmlParser.py')
MANIFEST_PATH = os.path.join(APPROVED_DIR, 'MANIFEST.json')
PYTHON = sys.executable


def _load_manifest():
    if not os.path.exists(MANIFEST_PATH):
        return {}
    with open(MANIFEST_PATH) as f:
        data = json.load(f)
    return data.get('files', {})


def _extract_module_name(xml_path):
    try:
        with open(xml_path, 'r', errors='replace') as f:
            content = f.read()
        m = re.search(r'<Module\b[^>]*\bname=["\']([^"\']+)["\']', content)
        if m:
            return m.group(1)
        m = re.search(r'<Module\b[^>]*\bname=([A-Za-z0-9_]+)', content)
        if m:
            return m.group(1)
    except Exception:
        pass
    return None


def _collect_test_cases():
    """Return a list of (rel_xml_path, module_name, export_type, golden_path) tuples."""
    manifest = _load_manifest()
    cases = []

    for rel_path, info in sorted(manifest.items()):
        if info.get('status') != 'compiled':
            continue
        module_name = info['module']
        for golden_rel in info.get('goldens', []):
            golden_abs = os.path.join(REPO_ROOT, golden_rel)
            fname = os.path.basename(golden_rel)
            if fname.endswith('.h.approved'):
                export_type = 'AgROOT_h'
            elif fname.endswith('.cxx.approved'):
                export_type = 'AgROOT_cxx'
            elif fname.endswith('.age.approved'):
                export_type = 'Mortran'
            else:
                continue
            cases.append((rel_path, module_name, export_type, golden_abs))
    return cases


def _get_compiled_output(xml_path, module_name, export_type):
    """Compile xml_path and return the requested output as a string, or raise."""
    if export_type in ('AgROOT_h', 'AgROOT_cxx'):
        with tempfile.TemporaryDirectory(prefix='agml_approval_') as tmpdir:
            cmd = [
                PYTHON, AGML_PARSER,
                '--file=%s' % xml_path,
                '--export=AgROOT',
                '--module=%s' % module_name,
                '--path=%s' % tmpdir,
            ]
            result = subprocess.run(
                cmd, capture_output=True, text=True, timeout=120, cwd=REPO_ROOT
            )
            if result.returncode != 0:
                raise RuntimeError('AgROOT compilation failed:\n' + result.stderr)
            suffix = '.h' if export_type == 'AgROOT_h' else '.cxx'
            out_file = os.path.join(tmpdir, module_name + suffix)
            if not os.path.exists(out_file):
                raise RuntimeError('Expected output file not found: ' + out_file)
            with open(out_file) as f:
                return f.read()
    else:  # Mortran → stdout
        cmd = [
            PYTHON, AGML_PARSER,
            '--file=%s' % xml_path,
            '--export=Mortran',
            '--module=%s' % module_name,
            '--path=/dev/null',
        ]
        result = subprocess.run(
            cmd, capture_output=True, text=True, timeout=120, cwd=REPO_ROOT
        )
        if result.returncode != 0:
            raise RuntimeError('Mortran compilation failed:\n' + result.stderr)
        return result.stdout


# Build parametrized test cases once at collection time
_TEST_CASES = _collect_test_cases()


def _make_test_id(case):
    rel_path, module_name, export_type, _ = case
    base = os.path.splitext(os.path.basename(rel_path))[0]
    return '%s::%s' % (base, export_type)


@pytest.mark.parametrize('case', _TEST_CASES, ids=[_make_test_id(c) for c in _TEST_CASES])
def test_agml_approval(case, update_approvals):
    rel_path, module_name, export_type, golden_path = case
    xml_abs = os.path.join(REPO_ROOT, rel_path)

    actual = _get_compiled_output(xml_abs, module_name, export_type)

    if update_approvals:
        os.makedirs(os.path.dirname(golden_path), exist_ok=True)
        with open(golden_path, 'w') as f:
            f.write(actual)
        pytest.skip('Updated golden: %s' % golden_path)
        return

    if not os.path.exists(golden_path):
        pytest.fail('Golden file missing: %s\nRun with --update-approvals to create it.' % golden_path)

    with open(golden_path) as f:
        expected = f.read()

    if actual == expected:
        return

    diff = ''.join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile='golden:  ' + os.path.relpath(golden_path, REPO_ROOT),
            tofile='compiled: ' + rel_path,
            n=5,
        )
    )
    pytest.fail(
        'Output differs from golden for %s (%s):\n\n%s' % (rel_path, export_type, diff)
    )
