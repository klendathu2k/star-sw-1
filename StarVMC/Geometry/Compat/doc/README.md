# Compat — Legacy AGE Geometry Compatibility Layer

## Overview

The `Compat/` directory is a **compatibility shim** that makes legacy AGE
(`.g`) geometry source files available to the AGML-based geometry framework
without rewriting them as native AGML XML.  Each XML file in this directory
wraps one or more original AGE source files from `pams/geometry/` using the
verbatim import mechanism:

```xml
<Document file="...">
  <Export language="Mortran">
    <Import file="pams/geometry/<subdir>/<name>.g" verbatim="true" />
  </Export>
</Document>
```

When the AGML build system processes a file with `<Export language="Mortran">`
and `verbatim="true"`, it passes the referenced `.g` file **directly through
the Mortran preprocessor without XML parsing**.  The `.g` file is treated as
raw AGE/Mortran source, exactly as it would have been in the legacy `starsim`
environment.  This allows detectors whose geometry has not yet been ported to
native AGML XML to continue participating in the unified simulation framework.

---

## How Verbatim Imports Work

```
  MutdGeo.xml                        tpcegeo.g
  (AGML XML)                         (AGE/Mortran source)
       │                                    │
       │  <Export language="Mortran">       │
       │    <Import file="..." verbatim>────┘
       │  </Export>
       │
       ▼
  AGML XML parser
  (parses Document/Export/Import tags)
       │
       ▼ passes file content unchanged
  Mortran preprocessor
  (expands Mortran macros, CDE blocks)
       │
       ▼
  AGE / GEANT3 geometry calls
```

The `<Export language="Mortran">` element signals that its content (or the
content of its `<Import>` children) should be emitted verbatim as Mortran code
rather than translated from AGML XML.  This is the only mechanism in the AGML
system that bypasses XML geometry description entirely.

**Consequence for maintenance**: changes to the geometry of a verbatim-imported
detector must be made in the corresponding `pams/geometry/<subdir>/*.g` files,
not here.  The XML files in `Compat/` are essentially frozen wrappers.

---

## Special Cases

Two files deviate from the pure verbatim pattern:

- **`dummgeo.xml`** — Contains a genuine minimal AGML module (`dummgeo`)
  with no geometry at all (just CDE imports).  It exists as a placeholder
  module that can be activated in a geometry configuration without producing
  any volumes.

- **`tpcegeo3.xml`** — Contains a minimal AGML module (`TPCEGEO3`) that uses
  `<Export language="Mortran">` to emit a `Call TpceGeo3a` Fortran statement,
  delegating to a separately compiled routine rather than importing a `.g` file.

---

## File Table

| XML file        | Imported `.g` file(s) | Detector subsystem | Notes |
|-----------------|----------------------|--------------------|-------|
| `calbpar.xml`   | `pams/geometry/calbgeo/calbpar.g` | BEMC / CALB — Barrel Electromagnetic Calorimeter parameter definitions | Common parameter block, not a full geometry module |
| `dummgeo.xml`   | *(none)*              | Placeholder / dummy | Native minimal AGML module; no volumes |
| `etsphit.xml`   | `pams/geometry/calbgeo/etsphit.g` | BEMC / CALB — end-cap scintillator hit structure | Hit structure definitions for ETSM end-cap |
| `fgtdgeo.xml`   | `pams/geometry/fgtdgeo/fgtdgeo.g` <br> `pams/geometry/fgtdgeo/fgtdgeo1.g` <br> `pams/geometry/fgtdgeo/fgtdgeo2.g` | FGT — Forward GEM Tracker | Three `.g` files imported in sequence covering base geometry and version variants |
| `ffpdstep.xml`  | `pams/geometry/fpdmgeo/ffpdstep.g` | FPD — Forward Pion Detector step definitions | Ancillary step/parameter file for FPD geometry |
| `fhcmgeo.xml`   | `pams/geometry/fhcmgeo/fhcmgeo.g` | FHCM — Forward Hadronic Calorimeter (FHC) | Forward hadronic calorimeter geometry |
| `fpdmgeo.xml`   | `pams/geometry/fpdmgeo/fpdmgeo.g` | FPD — Forward Pion Detector | Forward π⁰ detector geometry |
| `fstdgeo.xml`   | `pams/geometry/fstdgeo/fstdgeo.g` | FST — Forward Silicon Tracker (disk) | Forward silicon tracker disk geometry |
| `gembgeo.xml`   | `pams/geometry/gembgeo/gembgeo.g` | GEM Barrel — inner GEM detector | Barrel GEM detector geometry |
| `hpdtgeo.xml`   | `pams/geometry/hpdtgeo/hpdtgeo.g` | HPDT — High-Pt Detector Tracker | High-pT tracking detector geometry |
| `igtdgeo.xml`   | `pams/geometry/igtdgeo/igtdgeo.g` | IGTD — Inner GEM Tracker Disk | Inner forward GEM tracker disk geometry |
| `istbego.xml`   | `pams/geometry/istbgeo/istbgeo.g` <br> `pams/geometry/istbgeo/istbgeo1.g` <br> `pams/geometry/istbgeo/istbgeo2.g` <br> `pams/geometry/istbgeo/istbgeo3.g` <br> `pams/geometry/istbgeo/istbgeo4.g` <br> `pams/geometry/istbgeo/istbgeo5.g` <br> `pams/geometry/istbgeo/istbgeo6.g` <br> `pams/geometry/istbgeo/istbgeo00.g` | IST — Intermediate Silicon Tracker (barrel) | Eight `.g` files covering base geometry and multiple version variants (istbgeo00–6) |
| `itspgeo.xml`   | `pams/geometry/itspgeo/itspgeo.g` | ITSP — Inner Tracker Support Structure | Support cone / inner tracker support structure geometry |
| `pixlgeo.xml`   | `pams/geometry/pixlgeo/pixlgeo00.g` <br> `pams/geometry/pixlgeo/pixlgeo.g` <br> `pams/geometry/pixlgeo/pixlgeo1.g` <br> `pams/geometry/pixlgeo/pixlgeo2.g` | PIXL — Pixel Detector (PXL / HFT) | Four `.g` files; a fifth (`pixlgeo3.g`) is commented out |
| `richgeo.xml`   | `pams/geometry/richgeo/richgeo.g` | RICH — Ring Imaging Cherenkov Detector | STAR RICH detector geometry |
| `tpcegeo.xml`   | `pams/geometry/tpcegeo/tpcegeo.g` | TPC — Time Projection Chamber | Main TPC geometry (full detector); verbatim import for starsim integration |
| `tpcegeo3.xml`  | *(none — calls `TpceGeo3a`)* | TPC — alternative TPC geometry v3 | Thin wrapper module; delegates to separately compiled `TpceGeo3a` routine |
| `wallgeo.xml`   | `pams/geometry/wallgeo/wallgeo.g` | WALL — beam pipe / inner wall | Beam-pipe and inner wall geometry |

---

## Maintenance Notes

- Files in `Compat/` should **not** be modified to change geometry.  All
  geometry changes must be made in the corresponding `pams/geometry/` source.
- To port a subsystem from verbatim import to native AGML XML, replace the
  `<Export language="Mortran"><Import verbatim>` wrapper with a proper
  `<Module>` block and move the file to the appropriate named geometry
  directory under `StarVMC/Geometry/`.
- The `Document file=` attribute in several files still references legacy paths
  (e.g. `StarVMC/Compat/` instead of `StarVMC/Geometry/Compat/`); this is a
  known inconsistency and has no effect on processing.
