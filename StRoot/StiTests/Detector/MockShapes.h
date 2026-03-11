// StRoot/StiTests/Detector/MockShapes.h
// Helper factories for the two shape types used in the mock detector.
//
// TPC rows are cylinders; silicon layers are full-azimuth cylinders.
// Neither requires ROOT TGeo — the shapes are constructed directly via
// the STI shape API.

#ifndef STITTESTS_DETECTOR_MOCKSHAPES_H
#define STITTESTS_DETECTOR_MOCKSHAPES_H

#include "Sti/StiCylindricalShape.h"

namespace MockShapes {

// -------------------------------------------------------------------------
// TPC padrow shapes
// -------------------------------------------------------------------------

/// Number of TPC rows in the mock detector.
static const int kNTpcRows = 45;

/// Innermost TPC row radius (cm).
static const float kTpcRmin = 60.0f;

/// Radial spacing between TPC rows (cm).
static const float kTpcDr   = 4.0f;

/// Half-depth of TPC active volume in z (cm).
static const float kTpcHalfDepth = 200.0f;

/// Radial thickness of a TPC row (sensitive gas layer), cm.
static const float kTpcThickness = 0.4f;

/// Number of azimuthal TPC sectors.
static const int kNTpcSectors = 12;

/// Azimuthal opening angle of one TPC sector (radians).
static const float kTpcOpeningAngle =
    static_cast<float>(2.0 * 3.14159265358979323846 / kNTpcSectors);

/// Build a cylindrical shape for TPC row \a row (0-based).
/// Caller is responsible for lifetime management.
inline StiCylindricalShape* makeTpcRowShape(int row)
{
    char name[64];
    snprintf(name, sizeof(name), "TPC/Row_%02d", row);
    float r = kTpcRmin + row * kTpcDr;
    return new StiCylindricalShape(name,
                                   kTpcHalfDepth,
                                   kTpcThickness,
                                   /*outerRadius*/ r + 0.5f * kTpcThickness,
                                   kTpcOpeningAngle);
}

// -------------------------------------------------------------------------
// Silicon inner-tracker shapes
// -------------------------------------------------------------------------

/// Number of silicon layers in the mock inner tracker.
static const int kNSiLayers = 4;

/// Radii of the four silicon layers (cm).
static const float kSiRadii[kNSiLayers] = {2.5f, 5.0f, 10.0f, 15.0f};

/// Half-depth of each silicon layer in z (cm).
static const float kSiHalfDepth = 22.0f;

/// Radial thickness of a silicon layer (cm).
static const float kSiThickness = 0.028f;

/// Full-azimuth opening angle for silicon cylinders (one sector = 2π).
static const float kSiOpeningAngle =
    static_cast<float>(2.0 * 3.14159265358979323846);

/// Build a full-azimuth cylindrical shape for silicon layer \a layer (0-based).
/// Caller is responsible for lifetime management.
inline StiCylindricalShape* makeSiLayerShape(int layer)
{
    char name[64];
    snprintf(name, sizeof(name), "Si/Layer_%d", layer);
    float r = kSiRadii[layer];
    return new StiCylindricalShape(name,
                                   kSiHalfDepth,
                                   kSiThickness,
                                   /*outerRadius*/ r + 0.5f * kSiThickness,
                                   kSiOpeningAngle);
}

} // namespace MockShapes

#endif // STITTESTS_DETECTOR_MOCKSHAPES_H
