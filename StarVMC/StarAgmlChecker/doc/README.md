# StarAgmlChecker — AGML Geometry Material-Budget Checker

`StarAgmlChecker` analyses a fully assembled TGeo geometry produced by
`StarTGeoStacker` and computes the material budget seen by particles as a
function of pseudorapidity (η) and azimuthal angle (φ).  It also provides
deterministic MD5 checksums of geometry sub-trees for regression testing and
change detection.

---

## Purpose

After `ConstructGeometry()` has been called for all detector modules and a
`TGeoManager` has been locked and closed, `StarAgmlChecker` can be used to:

1. **Map the material budget** of the assembled detector as a 2D histogram in
   (η, φ), measured in units of radiation lengths X/X₀, separately for the
   top-level volume and for each of its direct daughter volumes.

2. **Compute geometry checksums** to detect unintended changes to a geometry
   configuration between software releases or between parameter tags.

---

## Class Reference

### `StarAgmlChecker`

Inherits from ROOT's `TGeoChecker`.

```
StarAgmlChecker(TGeoManager *manager, bool scoreAir = true)
```

The `scoreAir` flag controls whether air-equivalent media contribute to the
material budget integrals (default: true).

#### `MaterialPlot`

```cpp
TObjectSet *MaterialPlot(
    const Char_t  *top    = "CAVE",
    const Int_t    nEta   = 100,
    const Double_t mnEta  = -5.0,
    const Double_t mxEta  = +5.0,
    const Int_t    nPhi   = 360,
    const Double_t mnPhi  = -π,
    const Double_t mxPhi  = +π,
    const Double_t rmin   =    0.0,
    const Double_t rmax   =  400.0,   // ~30 cm outside MAGP outer radius
    const Double_t zmin   = -4000.0,
    const Double_t zmax   = +4000.0,
    const Option_t *opts  = "top" );
```

Fires rays from the origin through the TGeo volume tree, integrating the
radiation length of each traversed medium.  Returns a `TObjectSet` containing:
- A 2D histogram `(η, φ) → X/X₀` for the top volume.
- One 2D histogram per direct daughter of `top`, each labelled by the
  daughter volume name.

The `rmin`/`rmax`/`zmin`/`zmax` parameters bound the integration region.
Volumes in the skip list (added via `Skip(volname)`) are excluded from the
traversal.

#### `CheckSum`

```cpp
TMD5 CheckSum(const Char_t *volume);
```

Computes a deterministic MD5 checksum of the named TGeo volume sub-tree by
recursively hashing the shape parameters, material properties, medium
parameters, placement matrices, and node connectivity of every descendant node.
All 18 supported TGeo shape types are handled individually to ensure that every
geometric degree of freedom contributes to the checksum.

#### `CheckSet`

```cpp
TDataSet *CheckSet(const Char_t *volume = "HALL");
```

Returns a `TDataSet` tree that mirrors the TGeo hierarchy, with each node
replaced by its checksum string.  Useful for comparing two geometry
configurations node by node.

#### `Skip`

```cpp
void Skip(const Char_t *vol);
```

Adds a volume name to the exclusion list.  Skipped volumes and all their
descendants are ignored in both `MaterialPlot` and checksum computations.

---

### `CheckSum_t`

A helper accumulator class that wraps a `TMD5` digest and feeds typed values
into it via `operator+=`.  Supports `Int_t`, `Long_t`, `Long64_t`,
`ULong_t`, `ULong64_t`, `Float_t`, `Double_t`, `Bool_t`, `TString`, and
`const char*`.  Implicit conversion operators to `TMD5`, `TString`, and
`const char*` finalise and return the digest.

---

## Usage Example

```cpp
// After geometry construction:
StarAgmlChecker checker( gGeoManager, /*scoreAir=*/true );
checker.Skip("HALL");                                // exclude world volume
TObjectSet *budget = checker.MaterialPlot("CAVE");   // compute material budget
budget->ls();                                        // inspect histograms

TMD5 sum = checker.CheckSum("TPCE");                 // checksum TPC inner field cage subtree
printf("TPC geometry checksum: %s\n", (const char*)sum);
```

---

## Directory Contents

| File | Role |
|---|---|
| `StarAgmlChecker.cxx/.h` | Material-budget analysis and geometry checksum |
| `macros/` | Helper ROOT macros for running the checker interactively |
| `doc/` | This documentation |
