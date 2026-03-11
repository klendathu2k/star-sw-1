// StRoot/StiTests/Detector/MockMaterials.h
// Pre-built StiMaterial instances for the STI unit-test mock detector.
//
// Usage:
//   #include "StiTests/Detector/MockMaterials.h"
//   StiMaterial* gas = MockMaterials::p10Gas();
//
// The returned pointers are owned by the static storage inside each
// function and must NOT be deleted by callers.

#ifndef STITTESTS_DETECTOR_MOCKMATERIALS_H
#define STITTESTS_DETECTOR_MOCKMATERIALS_H

#include "Sti/StiMaterial.h"

namespace MockMaterials {

/// P10 gas (90% Ar / 10% CH4) — TPC fill gas.
inline StiMaterial* p10Gas()
{
    static StiMaterial mat("P10",
                           /*z*/    17.4,
                           /*a*/    37.8,
                           /*density*/ 1.56e-3,  // g/cm^3
                           /*X0*/      11.7);     // cm
    return &mat;
}

/// Silicon — inner-tracker sensitive layers.
inline StiMaterial* silicon()
{
    static StiMaterial mat("Silicon",
                           /*z*/    14.0,
                           /*a*/    28.09,
                           /*density*/ 2.33,     // g/cm^3
                           /*X0*/       9.37);   // cm
    return &mat;
}

/// Dry air — generic gas filling gaps between inner-tracker layers.
inline StiMaterial* air()
{
    static StiMaterial mat("Air",
                           /*z*/    7.3,
                           /*a*/    14.7,
                           /*density*/ 1.205e-3, // g/cm^3
                           /*X0*/      30420.0); // cm
    return &mat;
}

} // namespace MockMaterials

#endif // STITTESTS_DETECTOR_MOCKMATERIALS_H
