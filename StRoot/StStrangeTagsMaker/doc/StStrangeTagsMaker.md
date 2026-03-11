# StStrangeTagsMaker

## Overview

`StStrangeTagsMaker` is a STAR BFC maker that computes and records event-level strangeness summary tags into the `tags.root` file. For each reconstructed event it loops over V0, cascade (Xi/Omega), and kink vertex collections stored in `StEvent`, counts candidates whose invariant mass falls within configurable windows around the PDG masses of K⁰_S, Λ, Λ̄, Ξ⁻, Ξ̄⁺, Ω⁻, Ω̄⁺, and records both the candidate counts and the maximum transverse momentum in each species window. The resulting `StrangeTag_st` table can be used in offline event-filtering workflows to rapidly select events enriched in strange-particle decays without re-reading full DST data.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `StEvent` (input DS `"StEvent"`) | Fully reconstructed event object containing `StSPtrVecV0Vertex`, `StSPtrVecXiVertex`, and `StSPtrVecKinkVertex` collections. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `St_StrangeTag` (`"StrangeTag"`) | One-row table (type `StrangeTag_st`) added to the maker's output data set. Contains counts and max-pT tags for all supported strange particle species, plus the sideband windows and kink counts. Written into `tags.root` by the tag framework. |

## I/O Files

| File | Access | Description |
|---|---|---|
| `tags.root` | Write (via framework) | Destination of the `StrangeTag` table; written by the STAR tag infrastructure, not directly by this maker. |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StEvent` / `StEventTypes` | Source of V0, Xi, and kink vertex collections |
| `StV0Vertex`, `StXiVertex` | Vertex objects providing daughter momenta and bachelor charge |
| `tables/St_StrangeTag_Table.h` | ROOT-generated table definition for `StrangeTag_st` |
| `PhysicalConstants.h` / `phys_constants.h` | PDG mass values (K⁰_S, Λ, Λ̄, Ξ, Ω, pion, proton) |
| `TMath` | Absolute-value and square-root utilities |

## Typical Workflow Integration

`StStrangeTagsMaker` is added to the BFC after the global reconstruction chain has populated `StEvent` with vertex candidates (i.e. after `StV0FinderMaker`, `StXiFinderMaker`, and kink finders). It is typically one of the last tag-filling makers before the tag-writer step. It reads no persistent calibration from the database and requires no special initialisation beyond the maker framework.

```
BFC chain order (schematic):
  ... → StV0FinderMaker → StXiFinderMaker → StKinkMaker
      → StStrangeTagsMaker → TagWriter
```

## Key Classes/Functions

| Class / Function | File(s) | Description |
|---|---|---|
| `StStrangeTagsMaker` | `StStrangeTagsMaker.h/.cxx` | Main maker class; inherits `StMaker`. Drives the full tag-computation lifecycle. |
| `StStrangeTagsMaker::Init()` | `StStrangeTagsMaker.cxx` | Initialises PDG mass constants and the fractional window size (`mRange = 0.025`). |
| `StStrangeTagsMaker::Make()` | `StStrangeTagsMaker.cxx` | Per-event entry point: allocates `StrangeTag_st`, retrieves `StEvent`, calls `fillTag()`, and publishes the table. |
| `StStrangeTagsMaker::fillTag()` | `StStrangeTagsMaker.cxx` | Core logic: iterates V0, cascade, and kink collections; computes invariant masses using four-momentum arithmetic; fills all 34 fields of `StrangeTag_st`. |
| `StStrangeTagsMaker::printTag()` | `StStrangeTagsMaker.cxx` | Diagnostic printout of all tag fields to a given `ostream`. |
| `StStrangeTagsMaker::tag()` | `StStrangeTagsMaker.h` | Returns pointer to the current `StrangeTag_st` table (useful for inline inspection). |
