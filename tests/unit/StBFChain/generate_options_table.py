#!/usr/bin/env python3
"""Generate a Markdown options table from BigFullChain.h.

Usage:
    python generate_options_table.py [--bfc PATH] [--output PATH]

Parses the Bfc_st array in BigFullChain.h and writes a Markdown document with
one collapsible section per category.  The output is suitable for embedding
directly into doc/README.md or for standalone review.
"""

import argparse
import os
import re
import sys

# ---------------------------------------------------------------------------
# Parser
# ---------------------------------------------------------------------------

# Regex to capture a single Bfc_st initialiser row.
# Matches:   {"Key","Name","Chain","Opts","Maker","Libs","Comment",kFALSE}
# Fields may span lines (we pre-join continuation lines) or contain commas
# inside quoted strings.
_ROW_RE = re.compile(
    r'\{' 
    r'\s*"(?P<key>[^"]*)"'
    r'\s*,\s*"(?P<name>[^"]*)"'
    r'\s*,\s*"(?P<chain>[^"]*)"'
    r'\s*,\s*"(?P<opts>[^"]*)"'
    r'\s*,\s*"(?P<maker>[^"]*)"'
    r'\s*,\s*"(?P<libs>[^"]*)"'
    r'\s*,\s*"(?P<comment>[^"]*)"'
    r'[^}]*\}'
)

# Section-header patterns (key starts/ends with spaces or is all dashes)
_SECTION_HEADER_RE = re.compile(r'^-{3,}|.*-{3,}$|\s{2,}')


def _is_separator(key: str) -> bool:
    return bool(re.match(r'^-{4,}$', key.strip()))


def _is_section_title(key: str) -> bool:
    """Section-title rows have keys that contain spaces or trailing spaces."""
    stripped = key.strip()
    return ' ' in stripped or key != stripped


def parse_bigfullchain(path: str):
    """Return (sections, entries) where entries is a list of dicts."""
    with open(path) as fh:
        raw = fh.read()

    # Strip C-style comments
    raw = re.sub(r'/\*.*?\*/', ' ', raw, flags=re.DOTALL)
    raw = re.sub(r'//[^\n]*', ' ', raw)

    # Replace multi-line string concatenations: "..." \n "..."
    raw = re.sub(r'"\s*\n\s*"', '', raw)

    # Replace macro STAR_CHAIN_OBSOLETE
    raw = raw.replace('STAR_CHAIN_OBSOLETE', '"WARNING *** Option is OBSOLETE ***"')

    sections = []          # list of (section_title, [entry_dict])
    current_title = "Miscellaneous"
    current_entries = []

    for m in _ROW_RE.finditer(raw):
        raw_key = m.group('key')          # unstripped — used for section detection
        key     = raw_key.strip()
        name    = m.group('name').strip()
        chain   = m.group('chain').strip()
        opts    = m.group('opts').strip()
        maker   = m.group('maker').strip()
        libs    = m.group('libs').strip()
        comment = m.group('comment').strip()

        # Skip the column-header row
        if key == 'Key':
            continue

        # Separator rows (key is all dashes after strip)
        if _is_separator(key):
            continue

        # Section-title rows: raw key has internal spaces, trailing spaces, or
        # the Opts/Maker/Libs columns are all filled with dashes/hyphens
        opts_is_dashes = bool(re.match(r'^-{4,}$', opts.strip()))
        if _is_section_title(raw_key) or opts_is_dashes:
            if current_entries:
                sections.append((current_title, current_entries))
                current_entries = []
            current_title = key
            continue

        entry = dict(key=key, name=name, chain=chain, opts=opts,
                     maker=maker, libs=libs, comment=comment)
        current_entries.append(entry)

    if current_entries:
        sections.append((current_title, current_entries))

    return sections


# ---------------------------------------------------------------------------
# Markdown writer
# ---------------------------------------------------------------------------

def _escape_md(text: str) -> str:
    """Minimal Markdown escaping for table cells."""
    return text.replace('|', '\\|').replace('\n', ' ')


def render_markdown(sections) -> str:
    lines = []
    lines.append("# BFC Options Reference\n")
    lines.append(
        "This table is auto-generated from `StRoot/StBFChain/BigFullChain.h`.\n"
        "To regenerate after editing that file run:\n"
        "```\n"
        "python tests/unit/StBFChain/generate_options_table.py\n"
        "```\n"
    )
    lines.append(
        "Each row corresponds to one `Bfc_st` entry.  "
        "The **Opts** column lists options that are automatically enabled "
        "(or disabled, prefixed with `-`) when this option is activated.  "
        "The **Maker** column names the C++ class instantiated; an empty "
        "Maker means the option is a pure alias or configuration token.\n"
    )

    for title, entries in sections:
        lines.append(f"<details>\n<summary><b>{_escape_md(title)}</b> ({len(entries)} options)</summary>\n")
        lines.append("")
        lines.append("| Key | Opts (dependencies) | Maker | Comment |")
        lines.append("|-----|---------------------|-------|---------|")
        for e in entries:
            key     = _escape_md(e['key'])
            opts    = _escape_md(e['opts'])
            maker   = _escape_md(e['maker'])
            comment = _escape_md(e['comment'])
            lines.append(f"| `{key}` | {opts} | `{maker}` | {comment} |")
        lines.append("")
        lines.append("</details>\n")

    return "\n".join(lines)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    repo_root = os.path.normpath(
        os.path.join(os.path.dirname(__file__), '..', '..', '..'))
    default_bfc = os.path.join(
        repo_root, 'StRoot', 'StBFChain', 'BigFullChain.h')
    default_out = os.path.join(
        repo_root, 'StRoot', 'StBFChain', 'doc', 'bfc_options_table.md')

    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--bfc',    default=default_bfc, help='Path to BigFullChain.h')
    ap.add_argument('--output', default=default_out,  help='Output Markdown file (- for stdout)')
    args = ap.parse_args()

    sections = parse_bigfullchain(args.bfc)
    md = render_markdown(sections)

    if args.output == '-':
        sys.stdout.write(md)
    else:
        os.makedirs(os.path.dirname(args.output), exist_ok=True)
        with open(args.output, 'w') as fh:
            fh.write(md)
        total = sum(len(e) for _, e in sections)
        print(f"Wrote {total} options in {len(sections)} sections to {args.output}")


if __name__ == '__main__':
    main()
