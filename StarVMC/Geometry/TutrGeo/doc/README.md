# TutrGeo — Tutorial / Test Geometry

## Overview

TutrGeo is a **tutorial and developer-testing geometry** for the AGML
(AGROOTified Mortran Language) framework.  It is not a physical STAR detector;
instead it provides self-contained example modules that demonstrate AGML
concepts to developers and serves as a functional test that the geometry
framework is operating correctly.

Five XML files are present, covering progressively richer demonstrations:

| File | Module | Purpose |
|---|---|---|
| `TutrGeo1.xml` | `TutrGeo1` | Minimal geometry: one box TUTR mother + one ABOX child |
| `TutrGeo2.xml` | `TutrGeo2` | Multiple shapes: box, tube, tube-segment, cone, cone-segment |
| `TutrGeo3.xml` | `TutrGeo3` | Parametric tubes: structure-driven creation of three tubes from a `<Fill>` loop |
| `TutrGeo4.xml` | `TutrGeo4` | Extended shape library: all shapes from TutrGeo2 plus polycone, polygon, parallelepiped, elliptical tube |
| `ShapGeo.xml` | `ShapGeo` | Exhaustive AGML shape catalogue: every supported TGeo shape |

There is no `TutrConfig.xml`; tutorial modules are not registered as production
detector setups.

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `TutrGeo1.xml` | `TutrGeo1` | Minimal tutorial: TUTR box (500 cm) + ABOX child (10 cm) | None (hardcoded) |
| `TutrGeo2.xml` | `TutrGeo2` | Shapes tutorial: box + tube + tube-segment + cone + cone-segment | None (hardcoded) |
| `TutrGeo3.xml` | `TutrGeo3` | Parametric geometry: three tubes from `TUBG` structure fill loop | `TUBG` (version, rmin, rmax, dz) |
| `TutrGeo4.xml` | `TutrGeo4` | Extended shapes: all of TutrGeo2 + PCON, PGON, PARA, ELTU | None (hardcoded) |
| `ShapGeo.xml` | `ShapGeo` | Shape catalogue: BOX, PARA, TRD1, TRD2, TRAP, GTRA, SPHE, TUBE, TUBS, CTUB, ELTU, HYPE, CONE, CONS, PCON, PGON | None |

---

## Volume Inventories

### TutrGeo1 — minimal geometry
| Volume | Shape | Dimensions | Material | Description |
|---|---|---|---|---|
| TUTR | Box | 500×500×500 cm | Air | Tutorial mother volume |
| ABOX | Box | 10×10×10 cm | default | Example child box |

### TutrGeo2 — multiple shapes
| Volume | Shape | Description |
|---|---|---|
| TUTR | Box 500 cm | Mother |
| ABOX | Box 10 cm | A simple box |
| ATUB | TUBE | Tube with inner and outer radius |
| ATBS | TUBS | Tube segment (phi limits) |
| ACON | CONE | Cone with inner and outer radius |
| ACNS | CONS | Cone segment (phi limits) |

### TutrGeo3 — parametric tubes
| Volume | Shape | Instances | Description |
|---|---|---|---|
| TUTR | Box 500 cm | 1 | Mother |
| ABOX | Box 10 cm | 1 | Box child |
| ATUB | TUBE | 3 | Three tubes from `TUBG` structure versions 1–3; rmin/rmax = {0/5, 2.5/7.5, 5/10} cm; dz = {10, 10, 15} cm |

### TutrGeo4 — extended shapes
| Volume | Shape | Description |
|---|---|---|
| TUTR | Box 500 cm | Mother |
| ABOX | Box 10 cm | Simple box |
| ATUB | TUBE | Tube |
| ATBS | TUBS | Tube segment |
| ACON | CONE | Cone |
| ACNS | CONS | Cone segment |
| APCN | PCON | Polycone |
| APGN | PGON | Polygon |
| APAR | PARA | Parallelepiped (dx=10, dy=8, dz=20, alph=15°, thet=10°, phi=5°) |
| AELT | ELTU | Elliptical tube (p1=10, p2=15, dz=20) |

### ShapGeo — AGML shape catalogue
| Volume | TGeo type | Notes |
|---|---|---|
| BBOX | TGeoBBox | Basic box |
| PARA | TGeoPara | Parallelepiped |
| TRDA | TGeoTrd1 | Trapezoid (type 1) |
| TRDB | TGeoTrd2 | Trapezoid (type 2) |
| TRAP | TGeoTrap | General trapezoid |
| GTRA | TGeoGtra | GTRA shape |
| SPHE | TGeoSphere | Sphere |
| TUBE | TGeoTube | Tube |
| TUBS | TGeoTubeSeg | Tube segment |
| CTUB | TGeoCtub | Cut tube |
| ELTU | TGeoEltu | Elliptical tube |
| HYPE | TGeoHype | Hyperboloid |
| CONE | TGeoCone | Cone |
| CONS | TGeoConeSeg | Cone segment |
| PCON | TGeoPcon | Polycone |
| PGON | TGeoPgon | Polygon |

All ShapGeo volumes are placed inside a 1000 cm BOX mother and demonstrate
each shape with representative parameters.  Author: Jason C. Webb (November 2011).
