# StarGeometry — AgML Generated TGeo Geometry Output

`StarGeometry/` is a **build output directory**. It contains no source code.

At run time (or during the geometry-build step), the AGML framework assembles the
requested detector geometry tag and writes the resulting ROOT `TGeo` description files
into this directory. The files are consumed by downstream simulation and reconstruction
jobs that need the geometry without re-running the full AGML assembly step.

## Contents at Build Time

After `StVMCMaker::Init()` runs with a given geometry tag (e.g., `y2023b`), this directory
receives:

| File pattern | Description |
|---|---|
| `*.root` | Serialized `TGeoManager` objects (one per geometry tag) |
| `*.C` / `*.xml` | Optional exported TGeo macro or XML representations |

## Relationship to Other Directories

| Directory | Role |
|---|---|
| `StarVMC/Geometry/` | Source XML geometry module definitions (input) |
| `StarVMC/StarAgmlLib/` | AGML C++ library that assembles the geometry |
| `StarVMC/StarGeometry/` | Output: assembled TGeo files (this directory) |

The `README` in this directory contains the note:
> "Do not remove this directory - it will contain the AgML generated TGeo geometry files"
