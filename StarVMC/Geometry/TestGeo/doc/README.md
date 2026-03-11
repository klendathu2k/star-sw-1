# TestGeo — Test / Tutorial Geometry Module

## Overview

`TestGeo/` contains a single AGML XML module (`TestGeo1.xml`) used for
**development and tutorial purposes**.  It demonstrates advanced AGML placement
features — specifically parameterized shape definition at placement time and
volume division — using simple, artificial geometries that carry no
correspondence to any real STAR detector subsystem.

Author: JCW.

---

## XML Files

| Filename       | Module name | Description |
|----------------|-------------|-------------|
| `TestGeo1.xml` | `TestGeo1`  | Demonstrates parameterized tube placement and Y-axis volume division. |

---

## Volumes Defined in TestGeo1

### `MAIN`

A 200 × 200 × 200 cm Air box placed directly in `Cave`.  It is the master
volume that owns all sub-volumes.

### `TUBD` — Division volume

A volume of shape type `Division` that divides `MAIN` along the Y-axis
(`iaxis=2`) into **5 equal slices**.  Divisions are a ROOT/Geant4 construct for
creating replicated daughter volumes without explicit coordinate arithmetic.

### `TUBP` — Parent volume for tubes

An Air box (dz = 50 cm, dy = 6 cm; dx inherits from division) placed once in
each `TUBD` slice.  It acts as the immediate parent of all tube volumes.  Its
dx dimension is not specified in the XML and is therefore inherited from the
containing division.

### `TUBE` — Parameterized tube

An Iron `Tube` shape with `parameterized="true"`.  The shape parameters `rmin`,
`rmax`, and `dz` are left as zero in the volume definition and are instead
supplied at each `<Placement>` call.  Eleven instances are placed in `TUBP`
spanning x = −100 to +100 cm in steps of 20 cm, with rmax varying as
1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1 cm (a symmetric bell profile), all with
rmin = 0 and dz = 0 (point-like along z).  The tubes are coloured 44
(dark blue in GEANT3 colour table).

---

## Purpose and Intended Use Cases

1. **Testing the AGML parameterized-placement mechanism**: verifies that shape
   dimensions can be overridden at `<Placement>` time rather than fixed in the
   `<Volume>` definition.

2. **Testing volume division**: demonstrates `<Shape type="Division">` with
   `iaxis` and `ndiv` attributes, which maps onto `GEANT3 GSDVN` / ROOT
   `TGeoManager::Division`.

3. **Tutorial / minimal example**: provides the simplest possible self-contained
   AGML module that exercises non-trivial geometry features, suitable as a
   starting point for learning the AGML XML language.

4. **Regression testing**: can be loaded standalone to confirm that the AGML
   parser and geometry builder correctly handle parameterized shapes and
   divisions without requiring any real detector infrastructure.

---

## Notes

- `TestGeo1` has no sensitive volumes and no hit instrumentation; it produces
  no simulation output.
- It uses the common preamble `<CDE>AGECOM,GCUNIT,GCONST</CDE>` which imports
  standard GEANT3 common blocks.
- The `<Volume>` element (as opposed to `<Block>`) is used throughout,
  reflecting AGML usage that maps volumes directly without additional AGE
  wrapping.
- No `TestGeoConfig.xml` or setup configuration file exists for this directory;
  the module is typically loaded directly by name during debugging sessions.
