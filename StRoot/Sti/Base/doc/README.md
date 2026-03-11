# Sti Base Infrastructure — Reference Manual

## Overview

The `Base/` subdirectory of the STI (STAR Track Infrastructure) package provides a
self-contained set of reusable C++ infrastructure classes that underpin the entire Sti
framework.  It delivers five loosely coupled subsystems: **identity** (object naming and
description via `Named` and `Described`), **object pooling** (zero-fragmentation memory
management via `Factory`/`StiFactory`), **predicate filtering** (`Filter`/`EditableFilter`),
**tunable parameter management** (`Parameter`/`ConstrainedParameter`/`EditableParameter`/
`Parameters`/`EditableParameters`), and **change notification** (the observer pattern via
`Subject`/`Observer`).  These classes do not depend on ROOT or on any other Sti component,
so they can be used and tested in isolation and are freely mixed into higher-level Sti classes
through multiple inheritance.

---

## Class Reference Table

| Class | Template params | Role | Key methods |
|-------|----------------|------|-------------|
| `Named` | — | Provides a string name to any subclass | `setName()`, `getName()`, `isName()` |
| `Described` | — | Adds a free-text description string | `setDescription()`, `getDescription()`, `isDescribed()` |
| `BFactory` | — | Non-template base for all factories; pool accounting | `clear()`, `reset()`, `free(void*)`, `setMaxIncrementCount()`, `getCurrentCount()` |
| `Factory<A>` | `A` = abstract type | Typed abstract factory interface | `getInstance()`, `free(A*)` |
| `StiHolder<O>` | `O` = concrete type | Internal linked-list wrapper for pooled objects | (internal use) |
| `StiBlock<O>` | `O` = concrete type | Fixed-size block (100 slots) of `StiHolder<O>` | `reset()`, `getSize()` |
| `StiFactory<C,A>` | `C` = concrete, `A` = abstract | Concrete growing-pool factory; no heap fragmentation | `getInstance()`, `free()`, `clear()`, `reset()`, `myInstance()` |
| `Filter<F>` | `F` = filtered type | Abstract predicate interface | `accept()`, `filter()`, `reset()`, `getAnalyzedCount()`, `getAcceptedCount()` |
| `EditableFilter<F>` | `F` = filtered type | Filter with runtime-editable parameters | inherits `Filter<F>` + `EditableParameters` |
| `Parameter` | — | Single typed parameter (bool/int/float/double) | `getValue*()`, `setValue()`, `set()`, `getKey()`, `getType()` |
| `ConstrainedParameter` | — | Parameter with clamped min/max/default | `getMinimum()`, `getMaximum()`, `getDefault()`, `setMinimum()`, `setMaximum()`, `reset()` |
| `EditableParameter` | — | `ConstrainedParameter` + step increment for GUI | `getIncrement()`, `setIncrement()`, `reset()` |
| `Parameters` | — | Named/described vector of `Parameter*` with enable/edit flags | `add()`, `getParameter()`, `begin()`, `end()`, `initialize()`, `isEnabled()`, `isEditable()` |
| `EditableParameters` | — | `Parameters` that notifies `Observer`s on change | inherits `Parameters` + `Subject`; `setDefaults()` |
| `Subject` | — | Observable; maintains observer list | `attach()`, `detach()`, `notify()` |
| `Observer` | — | Abstract listener for `Subject` changes | `getNewState()`, `changed()`, `forgetSubject()` |

---

## Subsystem Reference

### 1. Identity — `Named` and `Described`

Nearly every Sti class inherits `Named` (and usually `Described`) via multiple inheritance.
The two classes are independent mixins; `Described` does **not** inherit from `Named`.
Higher-level classes such as `Parameter` combine both.

#### `Named`

```
class Named
```

**Purpose:** Attach a `std::string` name to any object, enabling name-based lookup and
diagnostic printout throughout the framework.

**Constructor:**
```cpp
Named(const string & aName = " ");   // protected; call from derived class
```

**Public methods:**

| Method | Description |
|--------|-------------|
| `void setName(const string & newName)` | Replace the stored name |
| `const string & getName() const` | Read-only access to the name |
| `bool isName(const string & aName) const` | Test for name equality |

**Data member:** `string _name`

---

#### `Described`

```
class Described
```

**Purpose:** Attach a free-text description string to an object, complementing the short
`Named` name with a longer human-readable explanation.

**Constructor:**
```cpp
Described(const string & aDescribe = " ");  // protected
```

**Public methods:**

| Method | Description |
|--------|-------------|
| `void setDescription(const string &)` | Set the description |
| `const string getDescription() const` | Get the description |
| `bool isDescribed() const` | Returns true if description is non-empty |
| `bool isDescription(const string &) const` | Test description equality |
| `bool sameDescriptionAs(const Described &) const` | Compare with another `Described` |

**Data member:** `string _description`

---

### 2. Factory — `BFactory`, `Factory<A>`, `StiHolder<O>`, `StiBlock<O>`, `StiFactory<C,A>`

The factory subsystem eliminates per-object heap fragmentation for the high-volume object
types in Sti: `StiHit`, `StiKalmanTrackNode`, `StiKalmanTrack`, `StiDetector`, `StiNodeExt`,
`StiNodeInf`, `EditableParameter`, and `Filter<StiTrack>`.  Objects are allocated in blocks
of 100 (`StiBlock::kSize`) and recycled via a free-list without calling `new`/`delete` per
object.

#### `BFactory` (non-template base)

```
class BFactory : public Named
```

**Constructor:** `BFactory(const string & name)` (protected)

**Key public methods:**

| Method | Description |
|--------|-------------|
| `virtual void clear() = 0` | Delete all allocated blocks and reset counts |
| `virtual void reset() = 0` | Return all live objects to the free-list without deallocating |
| `virtual void free(void *obj) = 0` | Return one object to the free-list |
| `static void Free(void *obj)` | Resolve the owning factory from the stored back-reference and call `free()` |
| `void setFastDelete()` | Enable fast delete mode (heap blocks are individually allocated/freed) |
| `void setMaxIncrementCount(int)` | Cap the total pool size |
| `int getCurrentSize() const` | Number of objects currently allocated |
| `int getCurrentCount() const` | Synonym for `getCurrentSize()` |

**Protected data:** `fMaxCount`, `fCurCount`, `fUseCount`, `fFastDel`, `fInstCount`,
`fFreeCount`, `fgTotal` (static; cumulative MB allocated across all factories).

---

#### `Factory<Abstract>` (typed abstract interface)

```cpp
template<class Abstract>
class Factory : public BFactory
```

**Template parameter:** `Abstract` — the type returned by `getInstance()`.

**Constructor:** `Factory(const string & name)` (protected)

**Key methods:**

| Method | Description |
|--------|-------------|
| `virtual Abstract * getInstance() = 0` | Obtain a recycled (or freshly allocated) instance |
| `virtual void free(Abstract *obj) = 0` | Return an instance to the pool |

---

#### `StiHolder<Object>` (internal)

```cpp
template<class Object>
class StiHolder
```

A union wrapper node that overlays the free-list `fNext` pointer with the first byte of
the stored `fObj`.  The `fLong` member holds a factory back-reference (`(long)factory + 1`)
so that `BFactory::Free()` can identify the owning factory from any live pointer.

---

#### `StiBlock<Object>` (internal)

```cpp
template<class Object>
class StiBlock
```

Fixed-size block of `kSize = 100` `StiHolder<Object>` nodes.  Blocks form a singly-linked
list (`fNext`) and are the unit of allocation.  `reset()` returns all holders in a block to
the free-list.

---

#### `StiFactory<Concrete, Abstract>` (concrete implementation)

```cpp
template<class Concrete, class Abstract>
class StiFactory : public Factory<Abstract>
```

**Template parameters:**
- `Concrete` — the actual instantiated class (e.g. `StiKalmanTrackNode`)
- `Abstract` — the type exposed to callers (e.g. `StiKalmanTrackNode` itself, or a base)

**Key methods:**

| Method | Description |
|--------|-------------|
| `static StiFactory * myInstance()` | Access the per-type Meyers singleton |
| `Abstract * getInstance()` | Pop from free-list; allocate a new block if empty; calls `reset()` on the object |
| `void free(Abstract *obj)` | Push back onto free-list; calls `unset()` on the object |
| `void clear()` | Delete all `StiBlock` objects, print memory accounting |
| `void reset()` | Recycle all live objects back to free-list; does not free heap |

**Usage example:**
```cpp
// Obtain the singleton factory for StiKalmanTrackNode
Factory<StiKalmanTrackNode> *f =
    StiFactory<StiKalmanTrackNode, StiKalmanTrackNode>::myInstance();
f->setMaxIncrementCount(50000);

// Allocate a node
StiKalmanTrackNode *node = f->getInstance();
// ... use node ...
f->free(node);   // return to pool

// At end of event: recycle without deallocation
f->reset();
// At job end: release all memory
f->clear();
```

---

### 3. Filtering — `Filter<F>` and `EditableFilter<F>`

#### `Filter<Filtered>` (abstract predicate interface)

```cpp
template<class Filtered>
class Filter
```

**Purpose:** Provide a single-method accept/reject interface for any object type, with
built-in counters for monitoring filter efficiency.

**Key methods:**

| Method | Description |
|--------|-------------|
| `virtual bool accept(const Filtered *) const = 0` | Pure virtual: implement the selection criterion |
| `bool filter(const Filtered *)` | Calls `accept()`, increments counters; use this in loops |
| `virtual void reset()` | Zero `_analyzedCount` and `_acceptedCount` |
| `int getAnalyzedCount()` | Number of objects passed to `filter()` since last `reset()` |
| `int getAcceptedCount()` | Number of objects for which `accept()` returned `true` |

**Data members:** `int _analyzedCount`, `int _acceptedCount`

**Usage example:**
```cpp
// Concrete filter accepting all StiTrack objects
class AcceptAll : public Filter<StiTrack> {
public:
    bool accept(const StiTrack *) const override { return true; }
};

AcceptAll f;
for (auto *track : tracks)
    if (f.filter(track)) process(track);
printf("%d/%d accepted\n", f.getAcceptedCount(), f.getAnalyzedCount());
```

---

#### `EditableFilter<Filtered>` (runtime-tunable filter)

```cpp
template<class Filtered>
class EditableFilter : public Filter<Filtered>,
                       public EditableParameters
```

**Purpose:** Combine the predicate interface of `Filter<F>` with the parameter container
and observer mechanism of `EditableParameters`, so that filter thresholds can be adjusted
at runtime or via an external GUI without recompilation.

**Constructors:**
```cpp
EditableFilter();
EditableFilter(const string & name, const string & description);
```

Concrete subclasses implement `accept()` and call `add(new EditableParameter(...))` inside
`initialize()` to expose their tunable thresholds.  `StiDefaultTrackFilter` is the standard
implementation in the Sti package.

---

### 4. Parameters — `Parameter`, `ConstrainedParameter`, `EditableParameter`, `Parameters`, `EditableParameters`

#### `Parameter` (single typed value)

```
class Parameter : public Named, public Described
```

**Purpose:** A single named, described tunable value that can hold a `bool`, `int`, `float`,
or `double`.  The type is recorded via the static integer constants `Boolean`, `Integer`,
`Float`, `Double`.  An optional external pointer `_exValue` allows the parameter to drive an
existing variable directly, so that setting the parameter immediately updates the pointed-to
variable.

**Type constants:** `static const int Boolean`, `Integer`, `Float`, `Double`

**Constructors (value copies):**
```cpp
Parameter();
Parameter(const string & name, const string & description, double value, int type, int key);
```

**Constructors (pointer binding — drives external variable):**
```cpp
Parameter(const string & name, const string & description, bool   * value, int key);
Parameter(const string & name, const string & description, int    * value, int key);
Parameter(const string & name, const string & description, float  * value, int key);
Parameter(const string & name, const string & description, double * value, int key);
```

**Key methods:**

| Method | Description |
|--------|-------------|
| `int getKey() const` | Optional user-defined integer key |
| `int getType() const` | One of `Boolean`, `Integer`, `Float`, `Double` |
| `bool   getBoolValue() const` | Typed accessor; reads external pointer if set |
| `int    getIntValue() const` | Typed accessor |
| `float  getFloatValue() const` | Typed accessor |
| `double getDoubleValue() const` | Typed accessor |
| `void setValue(bool/int/float/double)` | Typed setter; writes external pointer if set |
| `void set(name, description, value, type, key)` | Bulk initializer (value-copy form) |
| `void set(name, description, T * value, key)` | Bulk initializer (pointer-binding form) |

**Data members:** `int _key`, `int _type`, `double _value`, `void * _exValue`

---

#### `ConstrainedParameter` (clamped parameter)

```
class ConstrainedParameter : public Parameter
```

**Purpose:** Extends `Parameter` with enforced minimum, maximum, and default values.
`setValue()` silently clamps the incoming value to `[_minimum, _maximum]`.  Base class for
`EditableParameter`.

**Additional constructors (examples):**
```cpp
ConstrainedParameter(const string & name, const string & description,
                     double value, double defaultValue,
                     double min, double max, int type = Double, int key = 0);
ConstrainedParameter(const string & name, const string & description,
                     bool * value, bool defaultValue, int key = 0);
ConstrainedParameter(const string & name, const string & description,
                     int * value, int defaultValue, int min, int max, int key = 0);
```

**Additional methods:**

| Method | Description |
|--------|-------------|
| `double getMinimum() const` | Lower bound |
| `double getMaximum() const` | Upper bound |
| `double getDefault() const` | Default value used by `reset()` |
| `void setMinimum(double)` | Set lower bound; clamps `_value` and `_default` if needed |
| `void setMaximum(double)` | Set upper bound; clamps `_value` and `_default` if needed |
| `void setDefault(double)` | Set default; clamps to `[_minimum, _maximum]` |
| `void setValue(double)` | Override: clamps before storing |
| `void reset()` | Restore `_value` to `_default` |

**Additional data members:** `double _minimum`, `double _maximum`, `double _default`

---

#### `EditableParameter` (GUI-steppable parameter)

```
class EditableParameter : public ConstrainedParameter
```

**Purpose:** Adds a step `_increment` value intended for GUI sliders, spinboxes, or other
interactive controls that increment/decrement a parameter by a fixed amount.  Also provides
`reset()` (restore to default) and `unset()` (no-op; for factory recycling protocol).

**Key additional constructor:**
```cpp
EditableParameter(const string & name, const string & description,
                  double value, double defaultValue, double min, double max,
                  double increment, int type, int key);
```

**Additional methods:**

| Method | Description |
|--------|-------------|
| `double getIncrement() const` | The GUI step size |
| `void setIncrement(double)` | Set the step size |
| `void reset()` | Restore `_value` to `_default` |
| `void unset()` | No-op; satisfies factory recycling protocol |

**Additional data member:** `double _increment`

---

#### `Parameters` (container)

```
class Parameters : public Named, public Described
```

**Purpose:** A named, described container of `Parameter*` pointers together with `_enabled`
and `_editable` boolean flags.  Concrete subclasses must implement `initialize()` to populate
the `parameterVector`.  Iterator access (begin/end) follows STL conventions.

**Key methods:**

| Method | Description |
|--------|-------------|
| `Parameter * add(Parameter *)` | Append a parameter; returns the same pointer |
| `Parameter * getParameter(const string name)` | Find by name; returns `nullptr` if not found |
| `ParameterIterator begin()` | STL begin iterator |
| `ParameterIterator end()` | STL end iterator |
| `virtual void initialize() = 0` | Subclass hook to build the parameter list |
| `void clear()` | Empty the vector (does not delete pointed-to parameters) |
| `bool isEnabled()` | Whether this parameter set is active |
| `bool isEditable()` | Whether this parameter set may be modified at runtime |
| `void setEnabled(bool)` | Toggle enabled state |
| `void setEditable(bool)` | Toggle editable state |

---

#### `EditableParameters` (observable container)

```
class EditableParameters : public Parameters, public Subject
```

**Purpose:** A `Parameters` container that participates in the observer pattern as a
`Subject`.  When values change (typically via a GUI), the owner calls `notify()`, which
dispatches `changed()` to all registered `Observer` objects.  Used as the base for
`EditableFilter<F>` and for all tunable parameter sets in the framework.

**Additional methods:**

| Method | Description |
|--------|-------------|
| `virtual void setDefaults()` | Reset all parameters to their default values |
| `friend ostream & operator<<(ostream &, const EditableParameters &)` | Diagnostic print |

---

### 5. Observer Pattern — `Subject` and `Observer`

#### `Subject`

```
class Subject
```

**Purpose:** Observable base for parameter-owning objects.  Maintains a `vector<Observer*>`
and notifies all registered observers when its state changes.

**Key methods:**

| Method | Description |
|--------|-------------|
| `virtual void attach(Observer *)` | Register an observer |
| `virtual void detach(Observer *)` | Unregister an observer |
| `virtual void notify()` | Call `changed(this)` on all registered observers |

**Protected data:** `ObserverVec mObservers` (`vector<Observer*>`)

---

#### `Observer`

```
class Observer
```

**Purpose:** Abstract listener.  Concrete subclasses implement `getNewState()` to react to
subject changes.  The constructor can optionally attach the observer to a given subject.

**Constructors:**
```cpp
Observer();
Observer(Subject * subject);   // attaches automatically
```

**Key methods:**

| Method | Description |
|--------|-------------|
| `virtual void getNewState() = 0` | Pure virtual: implement response to change notification |
| `virtual void changed(Subject *)` | Called by `Subject::notify()`; calls `getNewState()` |
| `virtual void forgetSubject(Subject *)` | Nullify `mSubject` when the subject is destroyed |

**Protected data:** `Subject * mSubject`

---

## Inheritance Diagram

```
Named
├── BFactory
│   └── Factory<A>
│       └── StiFactory<C,A>
├── Parameter
│   └── ConstrainedParameter
│       └── EditableParameter
├── Parameters (also Described)
│   └── EditableParameters (also Subject)
├── StiDetector, StiDetectorBuilder, StiTrackContainer, ...  [in Sti/]
└── Described
    (mixin; also inherited by Parameter, Parameters, ...)

Filter<F>          (template)
└── EditableFilter<F>  (also EditableParameters)

Subject
└── EditableParameters (also Parameters)

Observer   (abstract)
```

---

## Usage Patterns and Code Examples

### Getting a Named object's name

```cpp
#include "Sti/Base/Named.h"

class MyComponent : public Named {
public:
    MyComponent(const string & name) : Named(name) {}
};

MyComponent c("TpcPadrow13");
printf("Name: %s\n", c.getName().c_str());
assert(c.isName("TpcPadrow13"));
```

---

### Using `Factory<T>` to allocate and recycle objects

```cpp
#include "Sti/Base/StiFactory.h"
#include "Sti/StiHit.h"

// Obtain the singleton factory (usually done once in StiDefaultToolkit::ctor)
Factory<StiHit> *hitFactory =
    StiFactory<StiHit, StiHit>::myInstance();
hitFactory->setMaxIncrementCount(200000);

// Per-event: get a hit from the pool
StiHit *hit = hitFactory->getInstance();
hit->setLocalY(1.5);
hit->setLocalZ(-10.2);
// ... fill container ...

// End of event: recycle ALL live objects back to pool in O(blocks) time
hitFactory->reset();

// End of job: free all heap memory
hitFactory->clear();
```

---

### Using `Filter<T>` to select objects

```cpp
#include "Sti/Base/Filter.h"
#include "Sti/StiTrack.h"

class MinPtFilter : public Filter<StiTrack> {
public:
    MinPtFilter(double minPt) : mMinPt(minPt) {}
    bool accept(const StiTrack *t) const override {
        return t->getPt() >= mMinPt;
    }
private:
    double mMinPt;
};

MinPtFilter f(0.2);  // 200 MeV/c cut
for (auto *track : trackContainer) {
    if (f.filter(track)) export(track);
}
printf("Accepted %d of %d tracks\n",
       f.getAcceptedCount(), f.getAnalyzedCount());
```

---

### Adding an `EditableParameter` to a parameter set

```cpp
#include "Sti/Base/EditableParameters.h"
#include "Sti/Base/EditableParameter.h"

class MyFilter : public EditableFilter<StiTrack> {
public:
    MyFilter() : EditableFilter<StiTrack>("MyFilter","My track filter") {}
    void initialize() override {
        add(new EditableParameter("minPt", "Minimum transverse momentum",
                                  0.15, 0.15, 0.0, 10.0, 0.01,
                                  Parameter::Double, 0));
    }
    bool accept(const StiTrack *t) const override {
        Parameter *p = getParameter("minPt");
        return !p || t->getPt() >= p->getDoubleValue();
    }
};
```

---

### Using `SubjectObserver` for change notification

```cpp
#include "Sti/Base/SubjectObserver.h"

class PtThresholdUpdater : public Observer {
public:
    PtThresholdUpdater(Subject *s, double &target)
        : Observer(s), mTarget(target) {}
    void getNewState() override {
        // retrieve new value from Subject and update mTarget
    }
private:
    double & mTarget;
};

EditableParameters params("cuts","Track selection cuts");
PtThresholdUpdater updater(&params, ptMin);
params.attach(&updater);
// Later, when parameters change:
params.notify();   // calls updater.getNewState()
```
