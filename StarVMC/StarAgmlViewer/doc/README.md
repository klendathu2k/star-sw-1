# StarAgmlViewer — AGML Interactive Geometry Viewer

`StarAgmlViewer` provides an interactive 3D visualisation of the TGeo geometry
assembled by the AGML framework.  It integrates with the ROOT EVE (Event
Visualisation Environment) system and is implemented as a `StMaker` so that it
can be embedded in the standard STAR maker chain.

---

## Purpose

After the AGML geometry has been constructed (via `StarTGeoStacker` and the
relevant `AgModule::ConstructGeometry()` calls), `StarAgmlViewer` opens an EVE
window and populates it with the TGeo volume tree.  It supports:

- **Selective volume display** — individual named TGeo volumes and their nodes
  can be added to the EVE scene via `addGlobalElement()`.
- **Transparency control** — `applyTransparency()` adjusts the transparency of
  all volumes under a given top node, making it easier to inspect nested
  detector sub-structures.

---

## Class Reference

### `StarAgmlViewer`

Inherits from `StMaker` so that it participates in the standard STAR data
processing chain.

```
StarAgmlViewer(const Char_t *name = "agmlViewer")
```

#### `Init`

```cpp
Int_t Init();
```

Called by the maker framework before the event loop.  Internally calls
`LoadGeometry()` to import the current `TGeoManager` into an EVE
`TGeoTopNode`, and opens the EVE main viewer window.

#### `addGlobalElement`

```cpp
Bool_t addGlobalElement(const Char_t *vol, const Char_t *node);
```

Locates the TGeo volume named `vol` in the current geometry and adds the
daughter node named `node` as a global EVE element, making it visible in all
EVE viewer windows.  Returns `true` on success.

#### `applyTransparency`

```cpp
void applyTransparency(const Char_t *TOP, Int_t mode = -1);
```

Recursively sets the transparency of all TGeo volumes in the sub-tree rooted at
`TOP`.  `mode = -1` applies a default transparency level suitable for
inspecting nested structures.  Useful for making outer detector shells
semi-transparent to reveal inner components.

---

## EVE Integration

`StarAgmlViewer` uses a `TEveManager` instance (`mEve`) created via
`TEveManager::Create()`.  The private `LoadGeometry()` method wraps the current
`gGeoManager` in a `TEveGeoTopNode` and registers it with the EVE geometry
manager, enabling interactive rotation, zooming, and per-volume selection in
the ROOT GUI.

The viewer uses `TGLViewer` for hardware-accelerated OpenGL rendering when
available.

---

## Usage Example

```cpp
// After geometry construction in a ROOT session or StRoot chain:
StarAgmlViewer *viewer = new StarAgmlViewer("agmlViewer");
viewer->Init();

// Display only the TPC end-cap within the CAVE:
viewer->addGlobalElement("CAVE", "TPCE_1");

// Make the magnet yoke semi-transparent:
viewer->applyTransparency("MAGP");
```

---

## Directory Contents

| File | Role |
|---|---|
| `StarAgmlViewer.cxx/.h` | EVE-based interactive TGeo geometry viewer (`StMaker`) |
| `StarAgmlChecker.cxx/.h` | Re-exported here for use in the combined viewer/checker workflow |
| `mkBudgetSet.C` | ROOT macro to compute and display a material-budget set from the viewer |
| `macros/` | Additional helper ROOT macros |
| `doc/` | This documentation |
