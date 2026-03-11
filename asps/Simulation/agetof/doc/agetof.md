# agetof

## Overview

`agetof` is a build-time **AGE→FORTRAN macro preprocessor** for the STAR
simulation software.  It reads source files written in the AGE/MORTRAN
domain-specific language (`.age` / `.g` extension) and translates them into
standard FORTRAN 77 (`.f`) for subsequent compilation.  The binary is also
installed under the alias `geant3`, reflecting its direct role as the
preprocessor front-end for GEANT3-based detector geometry code in STARSIM.

## Purpose

The STAR simulation framework (STARSIM / `asps/Simulation/starsim/`) and the
detector-geometry libraries write all physics code in the **AGE macro language**,
which provides structured programming extensions to FORTRAN 77 (structured
`IF/THEN/ELSE`, `DO` loops, `BLOCK`/`ENDBLOCK`) plus high-level
GEANT3 abstractions (volume, material, and medium macros).  `agetof` must be
executed on every `.age` file before the FORTRAN compiler is invoked; it is
therefore an essential build-time tool and not part of the runtime
reconstruction or simulation chain.

The tool descends from the CERNLIB MORTRAN family of macro processors.  Its
entire implementation is FORTRAN 77 (plus one 25-line C helper, `readlink.c`).
All internal state is held in a single 500,000-element integer array `O`
(`COMMON/NAMO/O`), following the classical CERNLIB packed-memory model.  The
language grammar is defined in an external text file (`agetof.def`); the
bootstrap character-code table is in `agetof.txt`.

## Datasets Consumed (Inputs)

None — `agetof` is a build-time source-code transformer, not an StMaker and
not part of any runtime data chain.

- Input AGE source file (`.age` / `.g`) via `-i` command-line argument.
- `agetof.def` — the grammar rule file (1,143 lines of pattern→template pairs).
- `agetof.txt` — bootstrap character-code table and initial rule set.

## Datasets Produced (Outputs)

None in the runtime sense.

- Output FORTRAN 77 source file (`.f`) written to `-o` destination.
- Optional listing file (`-l`) containing the expanded macro listing.
- Optional error file (`-e`) for diagnostics.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `*.age` / `*.g` | read | AGE macro language source |
| `agetof.def` | read | Grammar rule definitions |
| `agetof.txt` | read | Bootstrap parser state / character table |
| `*.f` | write | Translated FORTRAN 77 output |
| `*.l` (listing) | write (optional) | Expanded macro listing |

## Key Dependencies

- FORTRAN 77 compiler (self-contained; no external libraries at runtime).
- POSIX `readlink(2)` (the single C helper `readlink.c`).
- `agetof.def` and `agetof.txt` data files must be locatable at runtime (searched
  in current directory and `$STAR_SYS/bin/`).

## Typical Workflow Integration

`agetof` is invoked automatically by the STAR build system (SCons/Conscript or
legacy Imakefiles) whenever a `.age` or `.g` source file is compiled:

```sh
# Conceptual build step (run by Conscript/SCons):
agetof -i StTpcGeom.age -o StTpcGeom.f
f77 -c StTpcGeom.f -o StTpcGeom.o
```

It does **not** appear in the BFC chain or in any analysis macro.  Developers
only interact with it directly when debugging macro-expansion issues, using the
`-t` trace flag:

```sh
agetof -t -i MyGeom.age -o MyGeom.f -l MyGeom.list
```

## Key Classes / Functions

All code is FORTRAN 77 subroutines sharing `COMMON/NAMO/O`.

| File | Role |
|------|------|
| `main.F` | Main parser loop: token reading, rule lookup, quantifier matching, macro expansion dispatch |
| `nxtcrd.F` | Next-card reader: continuation lines, quoted strings, include-file switching |
| `define.F` | Rule definition: parses pattern→template pairs from `agetof.def` into the hash table |
| `expand.F` | Macro expansion: substitutes actual arguments into a matched template |
| `ispec.F` | Specifier matching: range quantifiers and bracket-balancing predicates |
| `lexp.F` | Expression evaluator: C-style boolean/arithmetic used in conditional rule predicates |
| `linf.F` | Output writer: formats translated FORTRAN with correct columns, continuation marks |
| `inital.F` | Initialisation: parses command-line arguments, opens I/O files, loads `agetof.txt` |
| `afname.F` | Locates `agetof.def` in standard search paths; uses `readlink.c` for symlink resolution |
| `raw.F` | Reads the character-code bijection table from `agetof.txt` at startup |
| `readlink.c` | 25-line C helper: POSIX `readlink()` wrapper called from FORTRAN |
