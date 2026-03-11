# St_geant_Maker

`St_geant_Maker` is the STAR chain maker that interfaces the STAR software
framework to the GEANT3 Monte Carlo transport engine (via the `TGiant3` ROOT
wrapper).  It initialises the GEANT3 engine, loads the detector geometry,
drives event-by-event simulation, and harvests the resulting Monte Carlo hits
into STAR data tables (`g2t_*`) for downstream reconstruction.

---

## Directory Contents

| File | Lines | Description |
|------|------:|-------------|
| `St_geant_Maker.h` | 312 | Class declaration, GEANT3 common-block pointers, `AddHits<>` template |
| `St_geant_Maker.cxx` | 2 788 | Full implementation: Init, InitRun, Make, Finish, geometry helpers, hit collection |
| `GtHash.h` | 19 | Hash-table interface mapping GEANT3 integer volume IDs → ROOT `TShape*` |
| `GtHash.cxx` | 78 | Hash-table implementation |
| `agstroot.g` | 123 | AGE source: `agstroot` FORTRAN subroutine that reads GEANT3 zebra banks and converts hit containers into ROOT tables |
| `navigate.g` | 155 | AGE source: navigation helpers for traversing the GEANT3 volume tree |
| `PACKAGE` | 6 | SCons build descriptor |
| `README` | 5 | Historical commentary |
| `Embed/StPrepEmbedMaker.h` | 164 | Embedding maker declaration — see `Embed/doc/README.md` |
| `Embed/StPrepEmbedMaker.cxx` | 1 024 | Embedding maker implementation |

---

## Architecture

```
StBFChain / ROOT macro
       │
       ▼
 St_geant_Maker  (StMaker)
       │  Init()    – creates TGiant3, opens input file, sets default kinematics
       │  InitRun() – loads geometry, sets magnetic field, calls agstroot
       │  Make()    – runs "trig", collects g2t_* hit tables
       │  Finish()  – prints QA hit-count summary
       │
       ├── TGiant3  (ROOT C++ wrapper for GEANT3 FORTRAN engine)
       │       └── ZEBRA banks: GCLINK, GCFLAG, GCVOLU, GCNUM, …
       │
       ├── geometry (TDataSet "geom" → TVolume tree or TGeoVolume)
       │       └── FindDataSet("HALL") triggers Work() → Agvolume() traversal
       │
       └── g2t tables (AddData)
             g2t_event, g2t_vertex, g2t_track, g2t_particle,
             g2t_tpc_hit, g2t_svt_hit, g2t_ssd_hit, g2t_pix_hit,
             g2t_ist_hit, g2t_fgt_hit, g2t_fts_hit, g2t_stg_hit,
             g2t_ctb_hit, g2t_tof_hit, g2t_tfr_hit, g2t_eto_hit,
             g2t_emc_hit, g2t_smd_hit, g2t_eem_hit, g2t_esm_hit,
             g2t_fpd_hit, g2t_fsc_hit, g2t_wca_hit, g2t_hca_hit,
             g2t_pre_hit, g2t_vpd_hit, g2t_pmd_hit, g2t_zdc_hit,
             g2t_bbc_hit, g2t_mtd_hit, g2t_etr_hit, g2t_epd_hit, …
```

---

## Key Methods

### `Init()`
Creates a `TGiant3` instance (`geant3`) with the requested ZEBRA memory
(`fNwGeant` × 1 M words, default 20 M words) and obtains pointers to the
GEANT3 common blocks (`GCFLAG`, `GCVOLU`, `GCKINE`, etc.).

Depending on the input-file extension the appropriate KUIP command is issued:

| Extension | KUIP command | Description |
|-----------|-------------|-------------|
| `.fz` | `gfile p <file>` | FORTRAN ZEBRA sequential file |
| `.nt` | `user/input user <file>` | Ntuple (HBOOK/ROOT) |
| `.tx` | `user/input tx <file>` | Plain-text kinematics |
| `.MuDst` | `user/input please MuDst.Dst` | Kinematics from MuDST file |
| *(none)* | `gkine 80 6 ...` or Pythia | Default kinematics (standalone) |

### `InitRun(int run)`
Called once per run (static guard prevents repetition).  Executes the
geometry initialisation string set by `LoadGeometry()` and calls `Agstroot()`
to populate the STAR table structures.  Reads or creates the magnetic field:

- **Live simulation** — uses `mFieldOpt` / `St_MagFactorC` scale factor.
- **Replay of .fz file** — reads scale from `geom_gdat` or `mfld_mflg` ZEBRA
  banks and creates/locks a `StarMagField` instance.

Physics flags (set via `SetAttr` / `IAttr`) applied at this stage:

| Attribute | Effect |
|-----------|--------|
| `phys_off` | All physics off: DCAY, ANNI, BREM, COMP, HADR, MUNU, PAIR, PFIS, PHOT, RAYL, DRAY, MULS, STRA disabled; LOSS=4 (no fluctuations); energy cuts = 1 MeV |
| `hadr_off` | Hadronic interactions off (HADR 0); decay and multiple scattering remain on; LOSS=1 |
| `flux` | Flux monitoring mode: enables `agustep` step hook, GCALOR hadronic model (HADR 6), low energy cuts (γ/e 0.01 MeV, neutron 0.01 meV) |
| `Pythia` | Activates bPythia generator: calls `bpythia`, sets beam/energy/tune, disables Pythia decays for particles handled by GEANT3 (π⁰, K, Λ, Σ, Ξ, Ω) |
| `Wenu` | Restricts Pythia to W→eν: `msel 12`, opens only W→eν decay channel |
| `RunG` | Sets GEANT run number and random seed: `rung N 1`, `rndm N` |
| `Don'tTouchTimeStamp` | Prevents overwriting `StEvtHddr` run/event number and date/time (used in mixer/embedding) |
| `beamLine` | Uses `St_vertexSeedC` beam-line parameters to shift the GEANT vertex with `gvertex` and `gslope` |
| `KeepRunNumber` | Prevents updating run number from the event record |

### `Make()`
Executes one GEANT3 event via the KUIP `trig` command, then reads back all
Monte Carlo hits using the `AddHits<T,F>()` template.

#### Hit Collection — `AddHits<T,F>(container, volumes, tablename, functor)`

The template loops over a list of sensitive GEANT3 volume names within a
named hit container, queries the total hit count with `Gfnhit()`, allocates
a table of type `T`, and calls the functor `F` (a FORTRAN g2t module) to
fill it.  Hit counts and energy sums are accumulated in `mHitCounts` and
`mHitSum` maps reported at the end of the job.

Hit containers and their sensitive volumes:

| Container | Sensitive volumes | Table created | Detector |
|-----------|------------------|---------------|---------|
| `SVTH` | `SVTD` | `g2t_svt_hit` | SVT |
| `SISH` | `SFSD` | `g2t_ssd_hit` | SSD |
| `PIXH` | `PLAC` | `g2t_pix_hit` | Pixel (PXL) |
| `ISTH` | `IBSS` | `g2t_ist_hit` | IST |
| `FGTH` | `FGZC`, `FGZD`, `FZCB` | `g2t_fgt_hit` | FGT |
| `TPCH` | `TPAD` | `g2t_tpc_hit` | TPC pads |
| `TPCH` | `TMSE` | `g2t_mwc_hit` | TPC inner MWC |
| `FTPH` | `FSEC` | `g2t_ftp_hit` | FTPC |
| `BTOH` | `BXSA` | `g2t_ctb_hit` | CTB |
| `BTOH` | `BCSB` | `g2t_tof_hit` | TOFr (old) |
| `BTOH` | `BRSG` | `g2t_tfr_hit` | BTOF |
| `ETOH` | `ECEL` | `g2t_eto_hit` | ETOF |
| `CALH` | `CSUP` | `g2t_emc_hit` | BEMC |
| `CALH` | `CSDA` | `g2t_smd_hit` | BSMD |
| `ECAH` | `ESCI`, `ELGR`, `EPCT` | `g2t_eem_hit` | EEMC |
| `ECAH` | `EXSE`, `EHMS` | `g2t_esm_hit` | ESMD |
| `VPDH` | `VRAD` | `g2t_vpd_hit` | VPD |
| `PHMH` | `PDGS` | `g2t_pmd_hit` | PMD |
| `ZCAH` | `QSCI` | `g2t_zdc_hit` | ZDC |
| `BBCH` | `BPOL` | `g2t_bbc_hit` | BBC |
| `FPDH` | `FLGR`, `FLXF`, `FPSC`, `FOSC` | `g2t_fpd_hit` | FMS/FPD |
| `FSCH` | `FSCT` | `g2t_fsc_hit` | FCS (R&D) |
| `MUTH` | `MIGG`, `MTTT`, `MTTF` | `g2t_mtd_hit` | MTD |
| `EIDH` | `TABD` | `g2t_etr_hit` | ETRD |
| `FTSH` | `FTSA`, `FSIA`, `FSIB`, `FSIC` | `g2t_fts_hit` | FTS |
| `FSTH` | `FTOS`, `FTIS`, `FTUS` | `g2t_fsi_hit` | FST |
| `STGH` | `STGP`, `STGL`, `STGS` | `g2t_stg_hit` | sTGC |
| `WCAH` | `WSCI` | `g2t_wca_hit` | FCS WCAL |
| `HCAH` | `HSCI` | `g2t_hca_hit` | FCS HCAL |
| `PREH` | `PSCI` | `g2t_pre_hit` | FCS preshower |
| `EPDH` | `EPDT` | `g2t_epd_hit` | EPD |

### `Finish()`
Prints QA `LOG_QA` summary of total hit counts and energy depositions per
sensitive container.

### `LoadGeometry(const char* option)`
Stores the geometry initialisation command string (e.g.
`"detp geometry y2023b"`) in `mInitialization` for deferred execution at
`InitRun()` time.  This deferral allows `StMagFMaker` to set the magnetic
field before the geometry is built.

### `Do(const char* option)`
Forwards the string to `TGiant3::Kuexel()` — the GEANT3 KUIP command
interpreter.  All simulation control flows through KUIP commands.

### `Geometry()` / `Agstroot()`
`Geometry()` calls the FORTRAN routine `geometry()` (from `pams/geometry`) to
define all GEANT3 volumes.  `Agstroot()` calls `agstroot()` (from
`agstroot.g`) to set up the g2t hit container structures in the ZEBRA address
space.

### `Work()` / `Agvolume()`
Translates the GEANT3 volume tree into a `TVolume`/`TShape` ROOT geometry,
accessible as the `"HALL"` dataset.  `GtHash` prevents duplicate ROOT shapes.
The modern `TGeoVolume` tree is also built and stored in `fTopGeoVolume`.

---

## Public Configuration API

| Method | Default | Description |
|--------|---------|-------------|
| `SetNwGEANT(n)` | 20 (×10⁶ words) | ZEBRA GCBANK memory allocation |
| `SetNwPAW(n)` | 0 | PAWC memory allocation |
| `SetIwtype(n)` | 0 | HIGZ interface (0 = none) |
| `SetInputFile(file)` | `""` | Input event file (.fz/.nt/.tx/.MuDst) |
| `SetFieldOpt(opt)` | `""` | Magnetic field option string passed to `StBFChain` |
| `SetRemake(flag)` | `kFALSE` | Force geometry rebuild on next `InitRun` |
| `LoadGeometry(option)` | `"detp geometry field_only"` | Defer geometry KUIP command |
| `Do(option)` | — | Execute arbitrary KUIP command |

---

## Usage Example (ROOT macro)

```cpp
// Minimal standalone simulation
St_geant_Maker *geant = new St_geant_Maker("geant");
geant->SetNwGEANT(20);
geant->LoadGeometry("detp geometry y2023b");
chain->Init();
chain->EventLoop(1000);
```

```cpp
// Read back a .fz file
St_geant_Maker *geant = new St_geant_Maker("geant");
geant->SetInputFile("simu.fz");
chain->Init();
chain->EventLoop(0);  // loops until EOF
```

---

## Dependencies

| Header | Purpose |
|--------|---------|
| `TGiant3.h` | GEANT3 VMC wrapper |
| `StMaker.h` | STAR chain base class |
| `StarMagField.h` | STAR mapped magnetic field |
| `StMagF.h` | Field scale wrapper |
| `StDetectorDbMaker/St_MagFactorC.h` | DB magnetic field scale factor |
| `tables/St_g2t_*.h` | Monte Carlo hit table definitions |
| `g2t/St_g2t_*_Module.h` | FORTRAN g2t fill subroutines |
| `StEvtHddr.h` | Run/event header |
| `StarCallf77.h` | FORTRAN name mangling macros |
