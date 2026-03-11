# agetof — AGE-to-FORTRAN Macro Preprocessor

## Overview

`agetof` is a **macro preprocessor** for the **AGE/MORTRAN domain-specific language** used
to write GEANT3-based detector simulation code.  It reads source files written in the AGE
macro language (`.g` extension) and translates them into standard FORTRAN 77 (`.f`) for
subsequent compilation.

The binary is installed under two names — `agetof` and `geant3` (via a build-time symlink) —
reflecting its direct role as the preprocessor that drives GEANT3 detector-geometry compilation
in the STARSIM application.  The name expands to **AGE TO FORTRAN**.  The CVS commit messages
describe it as "ATLAS version of mortran (geant3 => agetof)"; MORTRAN is the classic HEP
macro-FORTRAN preprocessor family from which it descends.

---

## Architecture

The entire implementation is FORTRAN 77 plus one 25-line C helper (`readlink.c`).  All state
is held in a single shared 500,000-element integer array `O`, declared in `COMMON/NAMO/O`.
Named "registers" `B, E, R, S, V, U, W, X` are `EQUIVALENCE`d to fixed cells `O(1)`–`O(8)`.
This is the classical CERNLIB packed-memory model: every data structure — the rule table, the
input buffer, the output buffer, the match stack — is a contiguous region within `O`.

### External data files

| File | Role |
|------|------|
| `agetof.def` (1,143 lines) | The **grammar**: pattern→template rule pairs that define the AGE language constructs — structured `IF/THEN/ELSE`, `DO` loops, `BLOCK`/`ENDBLOCK`, GEANT3 volume/material macros, string operations, and more. |
| `agetof.txt` (1,074 lines) | The **bootstrapped parser state**: the compiled character-code mapping table and initial rule set in packed notation, read by `RAW` at startup. |

---

## File-by-file description

| File | Lines | Role |
|------|------:|------|
| `main.F` | 591 | **Main parser loop.** Reads input tokens, searches the rule table for the longest matching pattern, dispatches expansion.  Handles quantifiers (`@001`, `@010`, `@111`), bracket/parenthesis balancing, and the match stack. |
| `nxtcrd.F` | 353 | **Next-card reader.** Reads a FORTRAN card-image line; handles continuation lines, quoted strings, inline comments, case folding, and include/unit switching. |
| `define.F` | 209 | **Rule definition.** Parses a pattern→template pair from the `.def` file and inserts it into the hash-keyed rule table in `O`. |
| `expand.F` | 264 | **Macro expansion.** Given a matched rule, substitutes actual arguments into the template and emits (or recursively expands) the result.  Handles nested expansion depth. |
| `ispec.F` | 175 | **Specifier matching.** Tests a parameterised specifier (`@NNN`) against the current input position; implements range quantifiers and bracket-balancing predicates. |
| `lexp.F` | 196 | **Expression evaluator.** Evaluates C-style boolean/arithmetic expressions used in conditional rule predicates (IF-THEN-ELSE within the macro grammar). |
| `linf.F` | 168 | **Line formatter / output writer.** Formats translated FORTRAN text to the output unit with correct column layout, continuation marks, and sequence numbers. |
| `rw.F` | 165 | **Read/write primitives.** Reads raw input cards from a FORTRAN unit and translates characters to the internal integer encoding used throughout `O`. |
| `inital.F` | 156 | **Initialisation.** Parses command-line arguments (`-i`, `-o`, `-l`, `-e`, `-v`, `-t`), opens I/O files, calls `RAW` to load the character table, and seeds the rule table. |
| `kat.F` | 154 | **Token/string operations.** String search, copy, and compare utilities operating on runs of cells in `O`. |
| `cccard.F` | 146 | **CERN-style card input.** Handles `++` continuation cards, `C` comment lines, and column-80 wrapping according to the CERN card-image convention. |
| `afname.F` | 166 | **Definition-file finder.** Searches standard locations (current directory, `$STAR_SYS/bin`, etc.) for `agetof.def`; uses `readlink.c` to resolve symlinks. |
| `raw.F` | 94 | **Character-table bootstrap.** Reads the first records of `agetof.txt` to build the internal character-code bijection that all other modules rely on. |
| `llong.F` | 56 | **Long-string utilities.** Handles strings that span multiple cells in `O`. |
| `find.F` | 56 | **Rule-table lookup.** Hash-based search for a macro name in the rule dictionary. |
| `filter.F` | 56 | **Structure-syntax detector.** Scans input for FORTRAN `RECORD/`, `STRUCTURE/`, or `POINTER(` declarations and activates the F77-structure translation mode. |
| `mactrc.F` | 82 | **Trace/debug output.** Prints the current input window and matched rule when the trace level (`-t`) is enabled. |
| `mesage.F` | 77 | **Error messages.** Maps `(E*)` and `(F*)` diagnostic codes (listed in `agetof.txt`) to human-readable messages and optionally aborts. |
| `krunc.F` | 76 | **String truncation.** Compresses or truncates token sequences in `O`. |
| `dumdum.F` | 37 | **Special-character table seeder.** Fills named special-character slots (BLANK, TERM, LPR, RPR, …) in `O` at startup. |
| `user.F` | 45 | **User-hook stub.** Placeholder transformation function; always returns without modification (intended for local customisation). |
| `muser.F` | 30 | **Initialisation user-hook stub.** Called with token `'INIT'` at startup; always returns 1 (success). |
| `nib.F` | 44 | **Nibble decoder.** Converts hexadecimal/octal literal notation into integer cell values. |
| `n5.F` | 33 | **Base-5 encoder.** Packs multi-character tokens into compact base-5 integer keys for the hash table. |
| `afexist.F` | 49 | **File-existence test.** FORTRAN wrapper: opens and immediately closes a file to test whether it exists. |
| `readlink.c` | 25 | **POSIX symlink resolver.** C function called from `afname.F` via FORTRAN/C interop to read the target of a symbolic link. |

---

## Processing pipeline

```
agetof.txt ──► RAW ──► character-code table in O[]
agetof.def ──► INITAL/DEFINE ──► rule table in O[]
                                        │
input .g file ──► NXTCRD/RW/CCCARD ──► token stream in O[]
                                        │
                               MAIN (pattern-match loop)
                                  ├── FIND   (hash lookup)
                                  ├── ISPEC  (quantifier match)
                                  ├── LEXP   (predicate eval)
                                  ├── EXPAND (arg substitution)
                                  └── LINF/DOIT (output emit)
                                        │
                                  FORTRAN 77 .f output
```

---

## Command-line usage

```
agetof [-i] file[.g] [-o outfile] [-l listfile] [-e errfile] [-V version] [-t]
```

| Option | Effect |
|--------|--------|
| `file[.g]` | Input AGE/MORTRAN source (`.g` suffix assumed if omitted) |
| `-o outfile` | Write translated FORTRAN to `outfile` (default: `file.f`) |
| `-l listfile` | Write expanded listing to `listfile` |
| `-e errfile` | Write error messages to `errfile` |
| `-V version` | Select language-version variant |
| `-t` | Enable macro trace output (verbose debugging) |

---

## Role in the STAR software stack

Every `.g` source file under `asps/Simulation/starsim/` and the detector-specific geometry
files that describe the TPC, SVT, BEMC, and other subsystems in the STARSIM GEANT3 application
are written in the AGE macro language.  `agetof` / `geant3` must run on these files before
the FORTRAN compiler is invoked.  It is therefore an **essential build-time tool** — a
compiler front-end for the entire STARSIM detector simulation — and is not part of the
runtime reconstruction chain.
