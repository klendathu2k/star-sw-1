"""tests/unit/StBFChain/test_bigfullchain.py

Structural unit tests for StRoot/StBFChain/BigFullChain.h.

These tests parse the C header directly with Python; no ROOT or STAR framework
is required.  They validate:

  - The option table can be parsed and yields a non-trivial number of entries.
  - No two entries share the same Key (case-insensitive).
  - Each field fits within its declared Bfc_st buffer.
  - Every token in an Opts dependency string (after stripping a leading '-')
    refers either to a Key defined in the same table, or to a known dynamic
    pattern (sdt/dbv timestamps, empty string).
  - Entries marked OBSOLETE carry no non-trivial Opts or Maker.
  - The table contains the expected categories (sections).
"""

import re

import pytest

# ---------------------------------------------------------------------------
# Bfc_st field size limits (from Bfc.h)
# ---------------------------------------------------------------------------
FIELD_LIMITS = {
    'key':     63,
    'name':    63,
    'chain':   63,
    'opts':   255,
    'maker':   63,
    'libs':   255,
    'comment':255,
}

# Known dynamic timestamp prefixes that may appear in Opts but are not static
# table entries (handled by kOpt's pattern-matching branch or added at runtime
# from StMaker::GetDbAliases()).
_TIMESTAMP_RE = re.compile(
    r'^r?(sdt|dbv|y)\d+[a-z_]*(\.\d+)?(_\w+_\w+)?$', re.IGNORECASE
)
# Year-alias tags added at runtime from GetDbAliases() — e.g. ry1h, y2010a,
# ry2013_1x, y2013_1c (year + optional underscore-delimited revision suffix).
_YEAR_ALIAS_RE = re.compile(r'^r?y\d{1,4}([a-z]|_\d+[a-z]?)?$', re.IGNORECASE)


def _is_dynamic_opt(token: str) -> bool:
    """Return True if *token* is a dynamic option not in the static table."""
    t = token.lower().lstrip('-')
    return bool(_TIMESTAMP_RE.match(t)) or bool(_YEAR_ALIAS_RE.match(t))


# Keys that are intentionally duplicated in BigFullChain.h.
# agml: two entries for the #ifdef __AgMLonFly__ / #else branches.
# Others are pre-existing copy-paste artefacts; the first match wins in kOpt().
_KNOWN_DUPLICATE_KEYS = {
    'agml',                    # #ifdef __AgMLonFly__ conditional variants
    'test.default.stivmc',     # copy-paste artefact
    'rc.pp.y2012b.notofmin',   # copy-paste artefact
    'b2013',
    'mudstdeps', 'mudst',
    'gmt', 'gmtdat', 'gmtclu',
    'l3onl',
    'l3count', 'fpd', 'fgtatoc', 'stipulls',
    'beamback', 'kink2', 'v02', 'xi2',
    'strngmudst', 'cmudst', 'display',
}

# Opts tokens that are known defects in BigFullChain.h (wrong key referenced).
# Listed here so the test catches any *new* defects while documenting the old.
_KNOWN_MISSING_OPTS = {
    ('ccomplete', 'Complete'),   # 'Complete' is a section title, not a Key
    ('pp2008c',   'Minuit'),     # 'Minuit' is a ROOT library, not a BFC option
}

# Entries whose Comment is STAR_CHAIN_OBSOLETE but still carry a non-trivial
# Opts value — pre-existing defects in BigFullChain.h.
_KNOWN_OBSOLETE_WITH_OPTS = {
    'beamback',  # OBSOLETE entry still expands to StEvent (copy-paste defect)
}


# ---------------------------------------------------------------------------
# Basic parsing tests
# ---------------------------------------------------------------------------

class TestParsing:
    def test_header_parses(self, bfc_entries):
        """BigFullChain.h must yield at least 900 option entries."""
        assert len(bfc_entries) >= 900, (
            f"Expected ≥900 entries, got {len(bfc_entries)}"
        )

    def test_sections_present(self, bfc_sections):
        """Standard section titles must be present."""
        titles = {t.lower() for t, _ in bfc_sections}
        expected = {'makers', 'options', 'geometry', 'valid db', 'time stamps'}
        missing = {e for e in expected if not any(e in t for t in titles)}
        assert not missing, f"Missing expected section(s): {missing}"

    def test_all_entries_have_key(self, bfc_entries):
        """Every entry must have a non-empty Key."""
        empty = [e for e in bfc_entries if not e['key']]
        assert not empty, f"{len(empty)} entries have an empty Key"


# ---------------------------------------------------------------------------
# Duplicate key detection
# ---------------------------------------------------------------------------

class TestDuplicateKeys:
    def test_no_duplicate_keys(self, bfc_entries):
        """Option Keys must be unique (case-insensitive).

        Duplicates cause kOpt() to return the first match, silently ignoring
        later entries with the same key.  Known duplicates that arise from
        conditional compilation (#ifdef) or are pre-existing copy-paste
        artefacts in BigFullChain.h are listed in _KNOWN_DUPLICATE_KEYS and
        are exempt; any *new* duplicate is a genuine defect.
        """
        seen = {}
        unexpected = []
        for e in bfc_entries:
            k = e['key'].lower()
            if k in seen:
                if k not in _KNOWN_DUPLICATE_KEYS:
                    unexpected.append((seen[k], e['key']))
            else:
                seen[k] = e['key']

        assert not unexpected, (
            "Unexpected duplicate keys (not in _KNOWN_DUPLICATE_KEYS):\n"
            + "\n".join(f"  '{a}' vs '{b}'" for a, b in unexpected[:20])
        )


# ---------------------------------------------------------------------------
# Field length validation
# ---------------------------------------------------------------------------

class TestFieldLengths:
    @pytest.mark.parametrize("field,limit", FIELD_LIMITS.items())
    def test_field_fits_in_buffer(self, bfc_entries, field, limit):
        """Each field must fit within its Bfc_st char buffer (including NUL)."""
        violations = [
            (e['key'], field, len(e[field]))
            for e in bfc_entries
            if len(e[field]) > limit
        ]
        assert not violations, (
            f"Field '{field}' (limit {limit}) exceeds buffer in "
            + ", ".join(f"'{k}' ({n} chars)" for k, _, n in violations[:10])
        )


# ---------------------------------------------------------------------------
# Opts dependency validity
# ---------------------------------------------------------------------------

class TestOptsDependencies:
    def test_opts_tokens_reference_valid_keys(self, bfc_entries, bfc_keys):
        """Every dependency token in Opts must refer to a defined Key.

        Tokens that are dynamic (sdt/dbv timestamps, runtime year aliases) or
        empty are exempted.  Entries prefixed with '-' indicate disabling an
        option; the base key still must exist.  Known pre-existing defects in
        BigFullChain.h are listed in _KNOWN_MISSING_OPTS; any *new* violation
        is a genuine defect.
        """
        violations = []
        for entry in bfc_entries:
            if not entry['opts']:
                continue
            tokens = re.split(r'[,\s]+', entry['opts'])
            for tok in tokens:
                tok = tok.strip()
                if not tok:
                    continue
                base = tok.lstrip('-').lower()
                if not base:
                    continue
                if _is_dynamic_opt(base):
                    continue
                if base not in bfc_keys:
                    pair = (entry['key'].lower(), tok)
                    if pair not in {(k.lower(), v) for k, v in _KNOWN_MISSING_OPTS}:
                        violations.append((entry['key'], tok))

        assert not violations, (
            f"{len(violations)} unresolved Opts reference(s) "
            f"(first {min(len(violations),30)} shown):\n"
            + "\n".join(
                f"  entry '{k}' references unknown option '{t}'"
                for k, t in violations[:30]
            )
        )


# ---------------------------------------------------------------------------
# Obsolete entries
# ---------------------------------------------------------------------------

class TestObsoleteEntries:
    # The exact string emitted by the STAR_CHAIN_OBSOLETE macro in BigFullChain.h.
    # Only entries whose Comment field equals this string are fully deprecated.
    # Entries that merely *mention* obsolescence in a longer description string
    # may still carry an active Maker or Opts for backwards compatibility.
    _OBSOLETE_COMMENT = "WARNING *** Option is OBSOLETE ***"

    def test_obsolete_entries_have_no_maker(self, bfc_entries):
        """Entries whose Comment is exactly STAR_CHAIN_OBSOLETE must not have a Maker."""
        bad = [
            e for e in bfc_entries
            if e['comment'] == self._OBSOLETE_COMMENT and e['maker']
        ]
        assert not bad, (
            "STAR_CHAIN_OBSOLETE entries with non-empty Maker: "
            + ", ".join(e['key'] for e in bad)
        )

    def test_obsolete_entries_have_no_opts(self, bfc_entries):
        """Entries whose Comment is exactly STAR_CHAIN_OBSOLETE must not expand dependencies.

        Pre-existing defects in BigFullChain.h are listed in
        _KNOWN_OBSOLETE_WITH_OPTS; any *new* violation is a genuine defect.
        The Opts field is stripped of bare commas/spaces before the check, so
        cosmetic artefacts like Opts=',' (e.g. the Flow entry) are not flagged.
        """
        bad = [
            e for e in bfc_entries
            if e['comment'] == self._OBSOLETE_COMMENT
            and e['opts'].strip(' ,')
            and e['key'].lower() not in _KNOWN_OBSOLETE_WITH_OPTS
        ]
        assert not bad, (
            "STAR_CHAIN_OBSOLETE entries with non-empty Opts: "
            + ", ".join(e['key'] for e in bad)
        )


# ---------------------------------------------------------------------------
# Known sentinel entries
# ---------------------------------------------------------------------------

class TestKnownEntries:
    def test_db_option_present(self, bfc_keys):
        """The fundamental 'db' option must be defined."""
        assert 'db' in bfc_keys

    def test_tpcrs_option_present(self, bfc_keys):
        """The TPC response simulator option 'tpcrs' must be defined."""
        assert 'tpcrs' in bfc_keys

    def test_mudst_option_present(self, bfc_keys):
        """The MuDST output option must be defined."""
        assert any('mudst' in k or 'cmudst' in k for k in bfc_keys), (
            "No MuDST option found in BFC table"
        )

    def test_makeevent_option_present(self, bfc_keys):
        """The StEventMaker option 'makeevent' must be defined."""
        assert 'makeevent' in bfc_keys

    @pytest.mark.parametrize("chain_key", [
        "MC.y2014", "MC.y2013", "RC.y2014",
        "P2014a", "P2012a", "P2011a", "P2010a",
    ])
    def test_production_chains_present(self, bfc_keys, chain_key):
        """Key production chain aliases must be present in the BFC table."""
        assert chain_key.lower() in bfc_keys, (
            f"Expected production chain '{chain_key}' not found in BFC table"
        )


# ---------------------------------------------------------------------------
# ParseString-like tokenisation (pure Python, no ROOT)
# ---------------------------------------------------------------------------

class TestTokenisation:
    """Validate that the comma/space tokenisation logic produces correct results.

    This mirrors the behaviour of StBFChain::ParseString which calls
    TString::Tokenize("[^ ;,]+").
    """

    def _tokenise(self, s: str):
        return [t for t in re.split(r'[,\s;]+', s) if t]

    def test_single_token(self):
        assert self._tokenise("db") == ["db"]

    def test_comma_separated(self):
        assert self._tokenise("db,tpcDB,Sti") == ["db", "tpcDB", "Sti"]

    def test_space_separated(self):
        assert self._tokenise("db tpcDB Sti") == ["db", "tpcDB", "Sti"]

    def test_mixed_separators(self):
        assert self._tokenise("db, tpcDB; Sti") == ["db", "tpcDB", "Sti"]

    def test_negation_token_preserved(self):
        tokens = self._tokenise("P2014a,-tofDat")
        assert "-tofDat" in tokens

    def test_empty_string(self):
        assert self._tokenise("") == []

    def test_whitespace_only(self):
        assert self._tokenise("   ,  ;  ") == []
