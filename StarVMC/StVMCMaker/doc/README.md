# StVMCMaker — STAR Chain Maker for VMC Simulation

`StVMCMaker` is the `StMaker` that drives the STAR Virtual Monte Carlo
simulation from within the standard STAR BFC (Big Full Chain).  It creates and
owns the `StarVMCApplication` singleton, the GEANT3 engine (`TGeant3TGeo`), and
all the connected infrastructure, then processes one event per `Make()` call.

---

## Class Reference

### `StVMCMaker`

**Header:** `StVMCMaker.h`

`StMaker` subclass that integrates `StarVMCApplication` into the STAR chain.

#### Initialisation and lifecycle

The initialisation sequence, shown schematically in the source flow comment, is:

```
StVMCMaker::Init()
  new StarVMCApplication(...)
  new TGeant3TGeo(...)           → gMC
  new StarMCHBPrimaryGenerator   (if input file set)
    OR StarMCSimplePrimaryGenerator
  appl->SetPrimaryGenerator(generator)
  StarMCHits *hits = StarMCHits::instance()
  appl->SetStepping(hits)
  hits->SetHitHolder(m_DataSet)
  appl->InitMC()
    gMC->SetStack(fStack)
    gMC->Init()  → ConstructGeometry / InitGeometry / Init hit descriptors
    gMC->BuildPhysics()

StVMCMaker::Make()
  appl->RunMC(1)
    gMC->ProcessRun(1)
      BeginEvent / GeneratePrimaries / Field / PreTrack
      g3track → Stepping → hits->Step → FillG2Table
      PostTrack / FinishEvent → hits->FinishEvent → g2t tables
```

| Method | Description |
|---|---|
| `Init()` | Creates `StarVMCApplication`, `TGeant3TGeo`, a primary generator, `StarMCHits`, and calls `appl->InitMC()`.  If a `TDataSet`-based volume list is available it builds a `TVolume` tree via `TGeoDrawHelper` for the event display. |
| `InitRun(Int_t runNumber)` | Propagates the run number to the event header and calls `SetDateTime()` to configure GEANT3 run conditions. |
| `Make()` | Calls `appl->RunMC(1)` to process one event, retrieves the resulting g2t hit tables from the dataset, and propagates them to downstream makers. |
| `Clear(Option_t*)` | Resets event-level state ready for the next `Make()` call. |
| `Finish()` | Finalises the MC run. |

#### Run configuration

| Method | Description |
|---|---|
| `SetDateTime(Int_t idat, Int_t itim)` | Sets the GEANT3 date and time (in YYYYMMDD and HHMMSS integer form) for database-driven run conditions; propagates to the event header. |
| `SetRunNo(Int_t m)` | Sets the run number, clamped to the range [1, 999999]. |
| `SetInputFile(const Char_t* fileName)` | Registers the path of the HBOOK/ROOT HBT input file.  When set, `Init()` creates a `StarMCHBPrimaryGenerator` rather than a simple generator. |
| `Skip(Int_t nskip)` | Advances the input-file reader by `nskip` events without simulating them. |

#### Static accessors

| Method | Description |
|---|---|
| `GetStarVMCApplication()` | Returns the `StarVMCApplication*` singleton created in `Init()`. |
| `GetGeant3()` | Returns the `TGeant3TGeo*` engine created in `Init()`. |
| `GetVolume()` | Returns the `TDataSet*` TVolume tree created by `TGeoDrawHelper` for the event display, or `nullptr` if not built. |

---

### `TGeoDrawHelper`

**Header:** `TGeoDrawHelper.h`

Converts a `TGeoVolume` tree (ROOT TGeo geometry) to the legacy
`TVolume`/`TShape` tree used by the STAR event display (`TGeometry`-based
rendering).  Written by Valeri Fine (BNL, 2004).

#### Construction

`TGeoDrawHelper(TGeoVolume* geoVolume = nullptr)` — if `geoVolume` is supplied,
immediately constructs the corresponding `TVolume` tree by calling
`MakeVolume(geoVolume)`.

#### Static shape and volume factories

| Method | Description |
|---|---|
| `MakeVolume(TGeoVolume* top, map<TGeoVolume*, TVolume*>* volumeMap)` | Recursively descends the TGeo volume tree, calling `MakeShape()` for each volume's shape and creating a `TVolume` node.  The optional map caches already-converted volumes to avoid duplication. |
| `MakeShape(const TGeoShape*)` | Dispatcher that downcasts to the concrete TGeo shape type and calls the appropriate typed overload. |
| `MakeShape(const TGeoBBox&)` | Creates a `TBRIK` (box). |
| `MakeShape(const TGeoTube&)` | Creates a `TTUBE`. |
| `MakeShape(const TGeoTubeSeg&)` | Creates a `TTUBS`. |
| `MakeShape(const TGeoCone&)` | Creates a `TCONE`. |
| `MakeShape(const TGeoConeSeg&)` | Creates a `TCONS`. |
| `MakeShape(const TGeoPcon&)` | Creates a `TPCON`. |
| `MakeShape(const TGeoPgon&)` | Creates a `TPGON`. |
| `MakeShape(const TGeoEltu&)` | Creates a `TELTU`. |
| `MakeShape(const TGeoTrap&)` | Creates a `TTRAP`. |
| `MakeShape(const TGeoCtub&)` | Creates a `TCTUB`. |
| `MakeShape(const TGeoTrd1&)` | Creates a `TTRD1`. |
| `MakeShape(const TGeoTrd2&)` | Creates a `TTRD2`. |
| `MakeShape(const TGeoGtra&)` | Creates a `TGTRA`. |
| `MakeShape(const TGeoSphere&)` | Creates a `TSPHE`. |
| `MakeShape(const TGeoArb8&)` | Creates a `TARB8`. |
| `MakeShape(const TGeoPara&)` | Creates a `TPARA`. |

#### Rendering

`Draw(Option_t*)` and `Paint(Option_t*)` delegate to the constructed `TVolume`.
`DistancetoPrimitive(px, py)` supports interactive ROOT canvas picking.
`GetVolume()` / `GetGeoVolume()` return the constructed `TVolume*` and the
source `TGeoVolume*` respectively.
