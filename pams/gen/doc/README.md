# GEN — Event Generator Tables

The GEN PAMS subsystem defines the common interface tables used to pass event-generator output into the STAR simulation chain. All generators supported by STAR (VENUS, HIJING, PYTHIA, FRITIOF, RQMD, STARLIGHT, VNI, and others) write their output into these tables, which are then consumed by GEANT/GSTAR for detector simulation.

## IDL Tables

| File | Struct | Key fields | Description |
|------|--------|------------|-------------|
| `eg_event.idl` | `eg_event` | `n_event`, `b_impact`, `phi_impact`, `n_track`, `n_vertex` | Per-event summary: event number, impact parameter, reaction-plane angle, and total track and vertex counts |
| `eg_track.idl` | `eg_track` | `ge_pid`, `p[3]`, `itrack`, `ivertex`, `iz` | Per-track data: GEANT particle ID, three-momentum vector (GeV/c), and integer indices linking to the vertex and parent-track lists |
| `eg_vertex.idl` | `eg_vertex` | `x[3]`, `t`, `ivstor`, `iz`, `npstor` | Per-vertex data: space point (mm) and time coordinate (mm/c), plus internal storage indices carried from the generator |
| `eg_gener.idl` | `eg_gener` | `eg_name[32]`, `eg_version`, `sqrts`, `b_max`, `east_a`, `east_z`, `west_a`, `west_z` | Generator run parameters: generator name and version, centre-of-mass energy √s (GeV), maximum impact parameter, and projectile/target mass numbers and charges |
| `particle.idl` | `particle` | `isthep`, `idhep`, `jmohep[2]`, `jdahep[2]`, `phep[5]`, `vhep[4]` | HEP common-block particle record (HEPEVT format): status code, PDG particle ID, mother and daughter pointers, four-momentum and mass (GeV), and production vertex and time (mm, mm/c) |

### Generator codes in `particle.idl`

The `particle` table's first row encodes run metadata via the `HEADPSS` common block. The generator identity is stored as `PSSHEP(3)`:

| Range | Generator |
|-------|-----------|
| 1–2 | FRITIOF |
| 10–11 | HBT |
| 20–29 | HIJET variants |
| 30–33 | HIJING |
| 40–43 | PYTHIA |
| 50–52 | STARLIGHT |
| 60–62 | VENUS |
| 70–80 | VNI |
| 90–91 | RQMD |
| 100 | BEAMGAS (VENUS) |

## Dependencies

**Filled by:**
- All STAR-supported event generators write `eg_event`, `eg_track`, `eg_vertex`, and `eg_gener` via the common generator interface
- Generators following the HEPEVT standard write the `particle` table directly from their internal particle stack
- The `eg_track` and `eg_vertex` tables are derived from the VENUS internal structures `PPTL`/`XSTRX` and their equivalents in other generators

**Read by:**
- GEANT/GSTAR detector simulation, which iterates over `eg_track` to transport each particle through the detector geometry
- `g2t` (GEANT-to-tables) modules that link GEANT hits back to the originating generator tracks
- Analysis modules performing generator-level (truth) studies or acceptance corrections
