# StSsdDaqMaker

## Overview

`StSsdDaqMaker` reads raw Silicon Strip Detector (SSD) data from DAQ files and
decodes it into calibrated strip-level tables consumed by the downstream cluster
and point makers. It reads the raw ADC values reported by the SSD front-end
electronics (A128C chips), applies the hardware channel–to–strip mapping
encoded in `ssdLadderMap.h`, subtracts pedestals, and writes the resulting
fired-strip records into the `spa_strip` table. The maker also fills a suite of
diagnostic histograms (occupancy, noise, and pedestal maps per chip, wafer and
ladder) and, optionally, an NTuple of per-strip pedestal/noise values.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| DAQ raw data | `StDAQReader` / `StSSDReader` | Raw RDO/fiber words from the SSD DAQ system |
| `Geometry/ssd/ssdConfiguration` | DB table (`St_ssdConfiguration`) | Active ladder mask and channel counts per run |
| `Calibrations/ssd/ssdPedStrip` | DB table (`St_ssdPedStrip`) | Per-strip pedestal and noise values |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `.data/spa_strip` | `St_spa_strip` | Pedestal-subtracted fired-strip ADC values for each active SSD strip |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| DAQ `.daq` event file | Read | Accessed via `StDAQReader`; contains raw SSD RDO data |
| `Pedestal.root` (optional) | Write | ROOT file containing the pedestal NTuple (`TNtuple`) when `mPedOut` is enabled |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StDAQMaker` (`StDAQReader`, `StSSDReader`) | Raw DAQ data access |
| `StSsdUtil` (`StSsdConfig`) | SSD geometry configuration helper |
| `tables/St_spa_strip_Table.h` | Output strip-data table definition |
| `tables/St_ssdConfiguration_Table.h` | Active-ladder configuration table |
| `tables/St_ssdPedStrip_Table.h` | Pedestal/noise calibration table |
| `ssdLadderMap.h` (local) | Hardware RDO-channel → ladder/strip mapping |
| ROOT (`TH1F`, `TH2S`, `TFile`, `TNtuple`) | Diagnostic histograms and NTuples |

## Typical Workflow Integration

`StSsdDaqMaker` sits immediately after the DAQ reader in the Big Full Chain
(BFC) and before `StSsdPointMaker`:

```
StDAQMaker  →  StSsdDaqMaker (SpaStrip)  →  StSsdDbMaker (SsdDb)  →  StSsdPointMaker (SsdPoint)
```

It must be instantiated before any maker that consumes `spa_strip`. The maker
name defaults to `"SpaStrip"` which matches the dataset path that
`StSsdPointMaker` requests.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StSsdDaqMaker` | Top-level STAR maker; orchestrates DAQ decoding and table filling |
| `StSsdDaqMaker::Init()` | Declares histograms and (optionally) the pedestal NTuple |
| `StSsdDaqMaker::InitRun()` | Loads the `ssdConfiguration` table and prints active-ladder summary |
| `StSsdDaqMaker::Make()` | Decodes one event's raw SSD data via `StSSDReader` and fills `spa_strip` |
| `StSsdDaqMaker::Finish()` | Writes diagnostic histograms/NTuple to file |
| `PrintConfiguration()` | Prints active-ladder map at run start |
| `DeclareNTuple()` | Sets up the pedestal diagnostic `TNtuple` |
| `ssdLadderMap.h` | Static look-up tables mapping RDO channel numbers to physical ladder/side/strip indices |
