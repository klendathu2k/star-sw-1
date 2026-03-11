# STI Unit Test Method Coverage Report

Static analysis: counts *distinct public method names* declared in
each tested class header and the subset of those names that appear as
calls in the corresponding test files.

> **Scope note.** Only methods declared in the listed headers are
> counted; inherited ROOT/STL methods are excluded.  Destructors are
> excluded — they are invoked implicitly by every stack-allocated
> object and perform only lifecycle cleanup.  Methods that require a
> full STAR reconstruction chain (Init, Make, Finish, propagate, …)
> are excluded per TestSet — see the README for coverage notes.
> Coverage is a lower-bound estimate.

## Summary

| Category | Declared | Covered | Coverage |
|----------|----------|---------|----------|
| MM — Base Memory Model | 40 | 24 | 60% |
| DG — Detector Geometry | 120 | 62 | 52% |
| HT — Hits & Track Representation | 179 | 46 | 26% |
| TF — Track Finding | 19 | 6 | 32% |
| FT — Track Fitting | 25 | 10 | 40% |
| HL — Hit Loading | 27 | 13 | 48% |
| VF — Vertex Finding | 7 | 7 | 100% |
| WF — Workflow / Steering | 11 | 3 | 27% |
| UT — Utilities & Infrastructure | 38 | 22 | 58% |

---

## MM — Base Memory Model

*Factory, StiFactory, StiTreeNode, StiCompositeTreeNode*

Headers: `StRoot/Sti/Base/Factory.h`, `StRoot/Sti/Base/StiFactory.h`, `StRoot/Sti/StiTreeNode.h`, `StRoot/Sti/StiCompositeTreeNode.h`  
Tests:   `StRoot/StiTests/MM/test_memory_model.cxx`  

**24 / 40 methods covered (60%)**

### Covered ✓

- `add`
- `begin`
- `clear`
- `disconnect`
- `end`
- `getChildAt`
- `getChildCount`
- `getCurrentCount`
- `getCurrentSize`
- `getData`
- `getInstance`
- `getName`
- `getNextNode`
- `getOrderKey`
- `getParent`
- `getPrevNode`
- `myInstance`
- `rbegin`
- `rend`
- `reset`
- `setData`
- `setName`
- `setOrderKey`
- `whereInParent`

### Not covered ✗

- `Factory`
- `Free`
- `StiBlock`
- `StiCompositeTreeNode`
- `StiHolder`
- `StiTreeNode`
- `cutTail`
- `free`
- `getFirstNode`
- `getLastNode`
- `getSize`
- `operator=`
- `remove`
- `setFastDelete`
- `setParent`
- `unset`

---

## DG — Detector Geometry

*Shapes, Material, ElossCalculator, Placement, Functor, Detector, Builder*

Headers: `StRoot/Sti/StiShape.h`, `StRoot/Sti/StiPlanarShape.h`, `StRoot/Sti/StiCylindricalShape.h`, `StRoot/Sti/StiMaterial.h`, `StRoot/Sti/StiElossCalculator.h`, `StRoot/Sti/StiPlacement.h`, `StRoot/Sti/StiIsActiveFunctor.h`, `StRoot/Sti/StiDetector.h`, `StRoot/Sti/StiDetectorBuilder.h`  
Tests:   `StRoot/StiTests/DG/test_detector_geometry.cxx`  

**62 / 120 methods covered (52%)**

### Covered ✓

- `StiCylindricalShape`
- `StiElossCalculator`
- `StiMaterial`
- `StiPlacement`
- `StiPlanarShape`
- `add`
- `calcError`
- `calculate`
- `findMaterial`
- `findShape`
- `getA`
- `getCenterRadius`
- `getDens`
- `getDensity`
- `getDetector`
- `getGas`
- `getGroupId`
- `getHalfDepth`
- `getHalfWidth`
- `getKey`
- `getLayerAngle`
- `getLayerRadius`
- `getMaterial`
- `getNRows`
- `getNSectors`
- `getNormalRadius`
- `getNormalRefAngle`
- `getNormalYoffset`
- `getOpeningAngle`
- `getOuterRadius`
- `getPlacement`
- `getRegion`
- `getShape`
- `getShapeCode`
- `getThickness`
- `getVolume`
- `getX0`
- `getZ`
- `getZOverA`
- `getZcenter`
- `getzOverA`
- `isActive`
- `isEditable`
- `set`
- `setGas`
- `setGroupId`
- `setHalfDepth`
- `setHalfWidth`
- `setIsActive`
- `setIsEditable`
- `setKey`
- `setLayerAngle`
- `setLayerRadius`
- `setMaterial`
- `setNormalRep`
- `setOpeningAngle`
- `setOuterRadius`
- `setPlacement`
- `setRegion`
- `setShape`
- `setThickness`
- `setZcenter`

### Not covered ✗

- `AverageVolume`
- `GetCurrentDetectorBuilder`
- `MakeAverageVolume`
- `Named`
- `Print`
- `SetCurrentDetectorBuilder`
- `StiDetector`
- `StiDetectorBuilder`
- `StiIsActiveFunctor`
- `StiShape`
- `_openingAngle`
- `_outerRadius`
- `build`
- `copy`
- `debug`
- `del`
- `findDetector`
- `getCenterOrientation`
- `getCenterRefAngle`
- `getCos`
- `getDetPlane`
- `getDetectorFactory`
- `getDetectors`
- `getEdgeWidth`
- `getElossCalculator`
- `getGasMat`
- `getHitErrorCalculator`
- `getIonization`
- `getIsActiveFunctor`
- `getSin`
- `getTrackingParameters`
- `getTreeNode`
- `getWeight`
- `getionization2`
- `hasMore`
- `insideG`
- `insideL`
- `isDiscreteScatterer`
- `isOn`
- `next`
- `nice`
- `operator()`
- `reset`
- `setDebug`
- `setDetector`
- `setGasMat`
- `setHitErrorCalculator`
- `setIsContinuousMedium`
- `setIsDiscreteScatterer`
- `setIsOn`
- `setNRows`
- `setNSectors`
- `setProperties`
- `setSplit`
- `setTrackingParameters`
- `setTreeNode`
- `splitIt`
- `unset`

---

## HT — Hits & Track Representation

*StiHit, StiNodePars, StiKalmanTrackNode*

Headers: `StRoot/Sti/StiHit.h`, `StRoot/Sti/StiNodePars.h`, `StRoot/Sti/StiKalmanTrackNode.h`  
Tests:   `StRoot/StiTests/HT/test_hits_tracks.cxx`  

**46 / 179 methods covered (26%)**

### Covered ✓

- `StiHit`
- `curv`
- `errMtx`
- `eta`
- `getAlpha`
- `getCharge`
- `getContigHitCount`
- `getContigNullCount`
- `getCos`
- `getCurvature`
- `getEta`
- `getHitCount`
- `getNullCount`
- `getRxy`
- `getSin`
- `getTanL`
- `getX`
- `getY`
- `getZ`
- `hz`
- `incHitCount`
- `incNullCount`
- `initialize`
- `isDca`
- `isZeroH`
- `makeDca`
- `position`
- `ptin`
- `refangle`
- `reset`
- `rotate`
- `rxy`
- `rxy2`
- `set`
- `setCurvature`
- `sxx`
- `sxy`
- `sxz`
- `syy`
- `syz`
- `szz`
- `tanl`
- `timesUsed`
- `x`
- `y`
- `z`

### Not covered ✗

- `A`
- `Comment`
- `IsLaser`
- `PrintStep`
- `PrintpT`
- `ResetComment`
- `SetLaser`
- `StThreeVectorD`
- `StThreeVectorF`
- `StiKalmanTrackNode`
- `StiNodeStat`
- `addTimesUsed`
- `check`
- `cos`
- `crossAngle`
- `debug`
- `detector`
- `evaluateChi2`
- `evaluateChi2Info`
- `extend`
- `fitErrs`
- `fitPars`
- `get`
- `getCyy`
- `getCzz`
- `getDedx`
- `getDensity`
- `getDetector`
- `getDipAngle`
- `getELoss`
- `getEloss`
- `getField`
- `getFlipFlop`
- `getGas`
- `getGasDensity`
- `getGasX0`
- `getGlobalHitErrs`
- `getGlobalMomentum`
- `getGlobalMomentumF`
- `getGlobalPoint`
- `getGlobalRadial`
- `getGlobalTpt`
- `getHelicity`
- `getHelixCenter`
- `getHit`
- `getHitCand`
- `getHz`
- `getIHitCand`
- `getInfo`
- `getLengths`
- `getMaterial`
- `getMomentum`
- `getMomentumF`
- `getP`
- `getPars`
- `getPhase`
- `getPoint`
- `getPseudoRapidity`
- `getPsi`
- `getPt`
- `getTime`
- `getValue`
- `getWindowY`
- `getWindowZ`
- `getX0`
- `globalPosition`
- `hitErrs`
- `idTruth`
- `incContigHitCount`
- `incContigNullCount`
- `isEnded`
- `isUsed`
- `length`
- `locate`
- `mMtx`
- `mPE`
- `mPP`
- `mcs2`
- `merge`
- `nan`
- `nice`
- `nudge`
- `numeDeriv`
- `operator()`
- `operator=`
- `operator[]`
- `pathLToNode`
- `pathlength`
- `phi`
- `pitchAngle`
- `print`
- `propagateError`
- `propagateMCS`
- `propagateMtx`
- `propagateToBeam`
- `propagateToRadius`
- `qaTruth`
- `ready`
- `reduce`
- `resetError`
- `rotateZ`
- `saveInfo`
- `setContigHitCount`
- `setContigNullCount`
- `setDebug`
- `setDetector`
- `setError`
- `setGlobal`
- `setHitCand`
- `setHitCount`
- `setHitErrors`
- `setIHitCand`
- `setMaxTimes`
- `setNullCount`
- `setStHit`
- `setState`
- `setTimesUsed`
- `setUntouched`
- `setVy`
- `setVz`
- `sinCrossAngle`
- `sqrt`
- `stHit`
- `subTimesUsed`
- `testDeriv`
- `testError`
- `unTouched`
- `unset`
- `vy`
- `vz`
- `x_g`
- `y_g`
- `z_g`

---

## TF — Track Finding

*StiKalmanTrackFinder (lightweight construction, reset/clear)*

Headers: `StRoot/Sti/StiKalmanTrackFinder.h`  
Tests:   `StRoot/StiTests/TF/test_track_finding.cxx`  

**6 / 19 methods covered (32%)**

### Covered ✓

- `addSeedFinder`
- `debug`
- `getNPrims`
- `getNTracks`
- `setDebug`
- `unset`

### Not covered ✗

- `CloserAngle`
- `Fit`
- `StiKalmanTrackFinder`
- `extendTrack`
- `extendTracks`
- `extendTracksToVertices`
- `find`
- `findTrack`
- `finish`
- `getTrackFilter`
- `initialize`
- `operator()`
- `setTiming`

---

## FT — Track Fitting

*StiKalmanTrackFitter, QaFit, StiMcsErrs, StiTrackNodeHelper*

Headers: `StRoot/Sti/StiKalmanTrackFitter.h`, `StRoot/Sti/StiTrackNodeHelper.h`  
Tests:   `StRoot/StiTests/FT/test_track_fitting.cxx`  

**10 / 25 methods covered (40%)**

### Covered ✓

- `add`
- `debug`
- `finish`
- `getAccu`
- `getMaxi`
- `getTally`
- `isCutStep`
- `reset`
- `setDebug`
- `setDir`

### Not covered ✗

- `QaFit`
- `StiKalmanTrackFitter`
- `StiTrackNodeHelper`
- `getFlipFlop`
- `getHitErrors`
- `getInfo`
- `getNSgn`
- `getNStd`
- `getUsed`
- `getVertexNode`
- `getWorst`
- `joinTwo`
- `joinVtx`
- `makeFit`
- `set`

---

## HL — Hit Loading

*StiHitLoader<int,int>, StiHitContainer*

Headers: `StRoot/Sti/StiHitLoader.h`, `StRoot/Sti/StiHitContainer.h`  
Tests:   `StRoot/StiTests/HL/test_hit_loading.cxx`  

**13 / 27 methods covered (48%)**

### Covered ✓

- `StiHitContainer`
- `add`
- `clear`
- `getDetector`
- `getHitFactory`
- `hasKey`
- `reset`
- `setDetector`
- `setHitContainer`
- `setHitFactory`
- `setMaxTimes`
- `size`
- `sortHits`

### Not covered ✗

- `StiHitLoader`
- `TestId`
- `VectorAndEnd`
- `begin`
- `getHit`
- `getHits`
- `hasDetector`
- `hits`
- `hitsBegin`
- `hitsEnd`
- `invalidateEnd`
- `push_back`
- `setEnd`
- `unset`

---

## VF — Vertex Finding

*StiVertexFinder (mock concrete subclass)*

Headers: `StRoot/Sti/StiVertexFinder.h`  
Tests:   `StRoot/StiTests/VF/test_vertex_finding.cxx`  

**7 / 7 methods covered (100%)**

### Covered ✓

- `StiVertexFinder`
- `clear`
- `fit`
- `getHitFactory`
- `getVertex`
- `result`
- `size`

---

## WF — Workflow / Steering

*StiMaker (construction, GetName, Clear, getToolkit)*

Headers: `StRoot/StiMaker/StiMaker.h`  
Tests:   `StRoot/StiTests/WF/test_workflow.cxx`  

**3 / 11 methods covered (27%)**

### Covered ✓

- `Clear`
- `StiMaker`
- `getToolkit`

### Not covered ✗

- `CleanGlobalTracks`
- `FillPulls`
- `FinishTracks`
- `GetCVS`
- `InitDetectors`
- `InitPulls`
- `MakeGlobalTracks`
- `MakePrimaryTracks`

---

## UT — Utilities & Infrastructure

*Named, Described, StiDebug, StiElossCalculator*

Headers: `StRoot/Sti/Base/Named.h`, `StRoot/Sti/Base/Described.h`, `StRoot/StiUtilities/StiDebug.h`, `StRoot/Sti/StiElossCalculator.h`  
Tests:   `StRoot/StiTests/UT/test_utilities.cxx`  

**22 / 38 methods covered (58%)**

### Covered ✓

- `Count`
- `Debug`
- `Finish`
- `Init`
- `SetDebug`
- `StiElossCalculator`
- `calcError`
- `calculate`
- `getA`
- `getDens`
- `getDescription`
- `getName`
- `getZ`
- `getionization2`
- `getzOverA`
- `isDescribed`
- `isDescription`
- `isName`
- `sameDescriptionAs`
- `set`
- `setDescription`
- `setName`

### Not covered ✗

- `AddIt`
- `Break`
- `FpeOn`
- `Get`
- `GetN`
- `PrintIt`
- `StiAux`
- `Sumary`
- `dFlag`
- `difY`
- `difZ`
- `iFlag`
- `resY`
- `resZ`
- `show`
- `tally`

---

*Report generated by `StRoot/StiTests/coverage_analysis.py`.*
