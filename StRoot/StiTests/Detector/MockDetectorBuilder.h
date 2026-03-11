// StRoot/StiTests/Detector/MockDetectorBuilder.h
// A self-contained detector builder for STI unit tests.
//
// Builds two sub-systems entirely from the STI geometry API (no TGeo):
//
//   TPC:  45 cylindrical rows, r = 60–236 cm (4 cm spacing)
//         12 azimuthal sectors per row (openingAngle = 2π/12)
//         halfDepth = 200 cm, thickness = 0.4 cm
//         material  = P10 gas  (gas + material both set to P10)
//         isActive  = StiIsActiveFunctor (always active)
//
//   Si inner tracker:  4 full-azimuth cylinders
//         r = 2.5, 5.0, 10.0, 15.0 cm
//         halfDepth = 22 cm, thickness = 0.028 cm
//         material  = Silicon  (gas = Air)
//         isActive  = StiIsActiveFunctor (always active)
//
// Usage:
//   MockDetectorBuilder builder;
//   builder.buildDetectors(maker);   // or just call build() internals
//
// The builder owns all allocated shapes, materials, and placements.
// The StiDetector objects are stored in the base-class detector maps.

#ifndef STITTESTS_DETECTOR_MOCKDETECTORBUILDER_H
#define STITTESTS_DETECTOR_MOCKDETECTORBUILDER_H

#include "Sti/StiDetectorBuilder.h"
#include "Sti/StiDetector.h"
#include "Sti/StiPlacement.h"
#include "Sti/StiIsActiveFunctor.h"
#include "StiTests/Detector/MockMaterials.h"
#include "StiTests/Detector/MockShapes.h"

class MockDetectorBuilder : public StiDetectorBuilder {
public:
    MockDetectorBuilder();
    virtual ~MockDetectorBuilder() {}

    /// Build the mock TPC + Si inner tracker.
    /// This override does NOT require a live StMaker; pass nullptr.
    virtual void buildDetectors(StMaker& /*source*/) override;

private:
    void buildTpc();
    void buildSilicon();
};

#endif // STITTESTS_DETECTOR_MOCKDETECTORBUILDER_H
