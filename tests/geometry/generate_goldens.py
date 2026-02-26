#!/usr/bin/env python3
"""Generate golden AGML compiler outputs for approval testing.

Runs mgr/agmlParser.py on all XML files under StarVMC/Geometry/ and
captures the output in tests/geometry/approved/.

Usage:
    cd /path/to/star-sw-1
    python tests/geometry/generate_goldens.py
"""
import os
import sys
import subprocess
import glob
import re
import shutil
import json
import tempfile

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GEOMETRY_DIR = os.path.join(REPO_ROOT, 'StarVMC', 'Geometry')
APPROVED_DIR = os.path.join(REPO_ROOT, 'tests', 'geometry', 'approved')
AGML_PARSER = os.path.join(REPO_ROOT, 'mgr', 'agmlParser.py')
PYTHON = sys.executable


def find_xml_files():
    """Return sorted list of all XML files under StarVMC/Geometry/."""
    return sorted(glob.glob(os.path.join(GEOMETRY_DIR, '**', '*.xml'), recursive=True))


def extract_module_name(xml_path):
    """Return the module name from <Module name="..."> or None if not present."""
    try:
        with open(xml_path, 'r', errors='replace') as f:
            content = f.read()
        m = re.search(r'<Module\b[^>]*\bname=["\']([^"\']+)["\']', content)
        if m:
            return m.group(1)
        # Some files have <Module name=... without quotes
        m = re.search(r'<Module\b[^>]*\bname=([A-Za-z0-9_]+)', content)
        if m:
            return m.group(1)
    except Exception:
        pass
    return None


def detector_subdir(xml_path):
    """Return the detector subdirectory name (e.g. 'CaveGeo')."""
    return os.path.basename(os.path.dirname(xml_path))


def run_agroot(xml_path, module_name, out_dir, timeout=120):
    """Run agmlParser.py --export=AgROOT and copy .h/.cxx to out_dir.

    Returns (success, stderr_text).
    """
    with tempfile.TemporaryDirectory(prefix='agml_agroot_') as tmpdir:
        cmd = [
            PYTHON, AGML_PARSER,
            '--file=%s' % xml_path,
            '--export=AgROOT',
            '--module=%s' % module_name,
            '--path=%s' % tmpdir,
        ]
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=timeout,
                cwd=REPO_ROOT,
            )
        except subprocess.TimeoutExpired:
            return False, 'TIMEOUT after %ds' % timeout

        if result.returncode != 0:
            return False, result.stderr

        # Copy produced files to out_dir
        produced = 0
        for fname in os.listdir(tmpdir):
            src = os.path.join(tmpdir, fname)
            dst = os.path.join(out_dir, fname + '.approved')
            shutil.copy2(src, dst)
            produced += 1

        if produced == 0:
            return False, 'No output files produced\n' + result.stderr

        return True, result.stderr


def run_mortran(xml_path, module_name, out_dir, timeout=120):
    """Run agmlParser.py --export=Mortran and capture stdout to .age.approved.

    Returns (success, stderr_text).
    """
    cmd = [
        PYTHON, AGML_PARSER,
        '--file=%s' % xml_path,
        '--export=Mortran',
        '--module=%s' % module_name,
        '--path=%s' % out_dir,
    ]
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=timeout,
            cwd=REPO_ROOT,
        )
    except subprocess.TimeoutExpired:
        return False, 'TIMEOUT after %ds' % timeout

    if result.returncode != 0:
        return False, result.stderr

    age_path = os.path.join(out_dir, module_name + '.age.approved')
    with open(age_path, 'w') as f:
        f.write(result.stdout)

    return True, result.stderr


def main():
    os.makedirs(APPROVED_DIR, exist_ok=True)

    xml_files = find_xml_files()
    print('Found %d XML files under %s' % (len(xml_files), GEOMETRY_DIR))

    manifest = {}
    total = len(xml_files)
    compiled_ok = 0
    compiled_fail = 0
    skipped = 0
    golden_files = 0

    for i, xml_path in enumerate(xml_files, 1):
        rel_path = os.path.relpath(xml_path, REPO_ROOT)
        module_name = extract_module_name(xml_path)
        det_subdir = detector_subdir(xml_path)
        out_dir = os.path.join(APPROVED_DIR, det_subdir)

        if module_name is None:
            print('[%3d/%3d] SKIP  %s (no <Module>)' % (i, total, rel_path))
            manifest[rel_path] = {
                'status': 'skipped',
                'reason': 'no <Module> element',
                'module': None,
                'goldens': [],
            }
            skipped += 1
            continue

        os.makedirs(out_dir, exist_ok=True)

        # --- AgROOT ---
        agroot_ok, agroot_err = run_agroot(xml_path, module_name, out_dir)

        # --- Mortran ---
        mort_ok, mort_err = run_mortran(xml_path, module_name, out_dir)

        base = os.path.basename(xml_path)
        goldens = []
        errors = []

        if agroot_ok:
            for ext in ('.h.approved', '.cxx.approved'):
                candidate = os.path.join(out_dir, module_name + ext)
                if os.path.exists(candidate):
                    goldens.append(os.path.relpath(candidate, REPO_ROOT))
                    golden_files += 1
        else:
            errors.append('AgROOT: ' + agroot_err.strip()[:200])

        if mort_ok:
            age_candidate = os.path.join(out_dir, module_name + '.age.approved')
            if os.path.exists(age_candidate):
                goldens.append(os.path.relpath(age_candidate, REPO_ROOT))
                golden_files += 1
        else:
            errors.append('Mortran: ' + mort_err.strip()[:200])

        if agroot_ok and mort_ok:
            status = 'compiled'
            compiled_ok += 1
            print('[%3d/%3d] OK    %s  (module=%s)' % (i, total, rel_path, module_name))
        else:
            status = 'failed'
            compiled_fail += 1
            print('[%3d/%3d] FAIL  %s  (module=%s)' % (i, total, rel_path, module_name))
            for e in errors:
                print('         %s' % e)

        manifest[rel_path] = {
            'status': status,
            'module': module_name,
            'detector_dir': det_subdir,
            'goldens': goldens,
            'errors': errors,
        }

    # Write manifest
    manifest_path = os.path.join(APPROVED_DIR, 'MANIFEST.json')
    with open(manifest_path, 'w') as f:
        json.dump({'files': manifest}, f, indent=2, sort_keys=True)

    print()
    print('=' * 60)
    print('Summary:')
    print('  Total XML files   : %d' % total)
    print('  Compiled OK       : %d' % compiled_ok)
    print('  Failed            : %d' % compiled_fail)
    print('  Skipped (no mod.) : %d' % skipped)
    print('  Golden files      : %d' % golden_files)
    print('  Manifest          : %s' % manifest_path)
    print('=' * 60)

    return 0 if compiled_fail == 0 else 1


if __name__ == '__main__':
    sys.exit(main())
