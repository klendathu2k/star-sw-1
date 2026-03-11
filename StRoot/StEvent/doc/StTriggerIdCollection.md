# StTriggerIdCollection

`StTriggerIdCollection` stores the complete set of **trigger IDs** for a single event in `StEvent`.  STAR's trigger system evaluates events at three hardware/software trigger levels (L1, L2, L3); this class holds a separate `StTriggerId` object for each level and for the L3-expanded set, plus a single **nominal** `StTriggerId` that represents the consensus trigger decision agreed upon across all levels.  Most analyses should access only `nominal()`.

Each `StTriggerId` stores up to 42 trigger IDs that fired for the event, their individual version numbers (name, threshold, prescale), and a 64-bit bitmask.

## Object Ownership

All five `StTriggerId` pointer members are **owned** by this collection.  They are initialised to `nullptr` in the constructor and **explicitly deleted** in the destructor.

| Pointer | Ownership |
|---|---|
| `StTriggerId* mL1TriggerId` | Owned — deleted in destructor. |
| `StTriggerId* mL2TriggerId` | Owned — deleted in destructor. |
| `StTriggerId* mL3TriggerId` | Owned — deleted in destructor. |
| `StTriggerId* mL3ExpandedTriggerId` | Owned — deleted in destructor. |
| `StTriggerId* mNominalTriggerId` | Owned — deleted in destructor. |
| `StTriggerIdCollection` itself | Owned by the `StEvent` framework. |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StTriggerId*` | `mL1TriggerId` | Set of trigger IDs that passed the Level-1 (hardware) trigger decision. May be `nullptr` if not populated. |
| `StTriggerId*` | `mL2TriggerId` | Set of trigger IDs that passed the Level-2 trigger decision. May be `nullptr`. |
| `StTriggerId*` | `mL3TriggerId` | Set of trigger IDs that passed the Level-3 (software) trigger decision. May be `nullptr`. |
| `StTriggerId*` | `mL3ExpandedTriggerId` | Extended Level-3 trigger ID set (introduced Run 6+). May be `nullptr`. |
| `StTriggerId*` | `mNominalTriggerId` | Consensus trigger ID set agreed upon across all levels.  **This is the primary accessor for most analyses.** |

## Key Methods

| Method | Description |
|---|---|
| `const StTriggerId* nominal() const` | Returns the nominal (consensus) trigger ID set.  **Use this for most analyses.** Returns `nullptr` if not set. |
| `const StTriggerId* l1() const` | Returns the L1 trigger ID set; may be `nullptr`. |
| `const StTriggerId* l2() const` | Returns the L2 trigger ID set; may be `nullptr`. |
| `const StTriggerId* l3() const` | Returns the L3 trigger ID set; may be `nullptr`. |
| `const StTriggerId* l3Expanded() const` | Returns the expanded L3 trigger ID set; may be `nullptr`. |
| `void setNominal(StTriggerId*)` | Assigns the nominal trigger ID object; collection takes ownership. |
| `void setL1(StTriggerId*)` | Assigns the L1 trigger ID object; collection takes ownership. |
| `void setL2(StTriggerId*)` | Assigns the L2 trigger ID object; collection takes ownership. |
| `void setL3(StTriggerId*)` | Assigns the L3 trigger ID object; collection takes ownership. |
| `void setL3Expanded(StTriggerId*)` | Assigns the expanded L3 trigger ID object; collection takes ownership. |

## StTriggerId quick reference

`StTriggerId` supports:
- `bool isTrigger(unsigned int id)` — returns `true` if trigger `id` fired.
- `vector<unsigned int> triggerIds()` — list of all fired trigger IDs.
- `uint64_t mask()` — 64-bit fired-trigger bitmask.
- `unsigned int version(id)`, `nameVersion(id)`, `thresholdVersion(id)`, `prescaleVersion(id)` — versioning information per trigger ID.

## Usage Example

```cpp
StTriggerIdCollection* trigColl = event->triggerIdCollection();
if (!trigColl) return;

// Nominal trigger is the correct one for most analyses
const StTriggerId* nominal = trigColl->nominal();
if (nominal) {
    // Check if a specific trigger fired
    if (nominal->isTrigger(15007)) {
        // ... this event passed trigger 15007
    }

    // Get all fired trigger IDs
    vector<unsigned int> ids = nominal->triggerIds();
    for (size_t i = 0; i < ids.size(); ++i) {
        unsigned int id = ids[i];
        unsigned int prescale = nominal->prescaleVersion(id);
    }
}
```
