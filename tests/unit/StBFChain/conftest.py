"""conftest.py for tests/unit/StBFChain.

Provides a session-scoped fixture that parses BigFullChain.h once and
exposes the raw list of extracted Bfc_st entries for use by all tests.
"""

import os
import sys

import pytest

# Make the test helpers importable
sys.path.insert(0, os.path.dirname(__file__))

from generate_options_table import parse_bigfullchain

# ---------------------------------------------------------------------------
# Resolve path to BigFullChain.h relative to this test file
# ---------------------------------------------------------------------------
_HERE = os.path.dirname(__file__)
_REPO_ROOT = os.path.normpath(os.path.join(_HERE, '..', '..', '..'))
BFC_HEADER = os.path.join(_REPO_ROOT, 'StRoot', 'StBFChain', 'BigFullChain.h')


@pytest.fixture(scope="session")
def bfc_sections():
    """Return the parsed sections list from BigFullChain.h.

    Each element is a (section_title, [entry_dict]) tuple.
    """
    return parse_bigfullchain(BFC_HEADER)


@pytest.fixture(scope="session")
def bfc_entries(bfc_sections):
    """Return a flat list of all Bfc_st entry dicts from BigFullChain.h."""
    entries = []
    for _title, section_entries in bfc_sections:
        entries.extend(section_entries)
    return entries


@pytest.fixture(scope="session")
def bfc_keys(bfc_entries):
    """Return a set of all lower-cased option Keys from the BFC table."""
    return {e['key'].lower() for e in bfc_entries}
