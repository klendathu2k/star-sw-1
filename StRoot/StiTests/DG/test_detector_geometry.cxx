// StRoot/StiTests/DG/test_detector_geometry.cxx
// Unit tests for the STI detector geometry model.
//
// Covers:
//   StiPlanarShape        — planar (rectangular) shape
//   StiCylindricalShape   — annular-wedge cylindrical shape
//   StiMaterial           — material physical properties
//   StiElossCalculator    — Bethe-Bloch energy-loss calculator
//   StiPlacement          — position/orientation representation
//   StiIsActiveFunctor    — active-area predicate
//   StiDetector           — full detector assembly
//   StiDetectorBuilder    — shape/material/detector registry (via MockDetectorBuilder)

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include <cmath>

#include "Sti/StiPlanarShape.h"
#include "Sti/StiCylindricalShape.h"
#include "Sti/StiMaterial.h"
#include "Sti/StiElossCalculator.h"
#include "Sti/StiPlacement.h"
#include "Sti/StiIsActiveFunctor.h"
#include "Sti/StiDetector.h"

#include "StiTests/Detector/MockDetectorBuilder.h"
#include "StiTests/Detector/MockMaterials.h"
#include "StiTests/Detector/MockShapes.h"

static const double kPi = 3.14159265358979323846;

// ---------------------------------------------------------------------------
// StiPlanarShape
// ---------------------------------------------------------------------------

TEST_CASE("StiPlanarShape — default construction", "[StiPlanarShape][DG]") {
    StiPlanarShape s;
    CHECK(s.getHalfDepth()   == Approx(0.f));
    CHECK(s.getThickness()   == Approx(0.f));
    CHECK(s.getHalfWidth()   == Approx(0.f));
    CHECK(s.getShapeCode()   == kPlanar);
}

TEST_CASE("StiPlanarShape — parameterised construction", "[StiPlanarShape][DG]") {
    StiPlanarShape s("svt/ladder_0", /*halfDepth*/5.f, /*thickness*/0.03f, /*halfWidth*/2.5f);
    CHECK(s.getName()      == "svt/ladder_0");
    CHECK(s.getHalfDepth() == Approx(5.f));
    CHECK(s.getThickness() == Approx(0.03f));
    CHECK(s.getHalfWidth() == Approx(2.5f));
    CHECK(s.getShapeCode() == kPlanar);
}

TEST_CASE("StiPlanarShape — setHalfDepth / setThickness / setHalfWidth", "[StiPlanarShape][DG]") {
    StiPlanarShape s;
    s.setHalfDepth(10.f);
    s.setThickness(0.05f);
    s.setHalfWidth(4.f);
    CHECK(s.getHalfDepth() == Approx(10.f));
    CHECK(s.getThickness() == Approx(0.05f));
    CHECK(s.getHalfWidth() == Approx(4.f));
}

TEST_CASE("StiPlanarShape — negative values are rejected", "[StiPlanarShape][DG]") {
    StiPlanarShape s;
    s.setHalfDepth(-1.f);
    s.setThickness(-1.f);
    CHECK(s.getHalfDepth() == Approx(0.f));
    CHECK(s.getThickness() == Approx(0.f));
}

TEST_CASE("StiPlanarShape — getVolume is positive for non-zero dimensions", "[StiPlanarShape][DG]") {
    StiPlanarShape s("test", 5.f, 0.1f, 3.f);
    CHECK(s.getVolume() > 0.0);
}

TEST_CASE("StiPlanarShape — getOpeningAngle returns sentinel", "[StiPlanarShape][DG]") {
    StiPlanarShape s;
    CHECK(s.getOpeningAngle() == Approx(999.f));
}

// ---------------------------------------------------------------------------
// StiCylindricalShape
// ---------------------------------------------------------------------------

TEST_CASE("StiCylindricalShape — default construction", "[StiCylindricalShape][DG]") {
    StiCylindricalShape s;
    CHECK(s.getOuterRadius()   == Approx(0.f));
    CHECK(s.getOpeningAngle()  == Approx(0.f));
    CHECK(s.getShapeCode()     == kCylindrical);
}

TEST_CASE("StiCylindricalShape — parameterised construction", "[StiCylindricalShape][DG]") {
    float oa = static_cast<float>(kPi / 6.0); // 30 deg
    StiCylindricalShape s("tpc/row_0", 200.f, 0.4f, 60.2f, oa);
    CHECK(s.getName()          == "tpc/row_0");
    CHECK(s.getHalfDepth()     == Approx(200.f));
    CHECK(s.getThickness()     == Approx(0.4f));
    CHECK(s.getOuterRadius()   == Approx(60.2f));
    CHECK(s.getOpeningAngle()  == Approx(oa));
    CHECK(s.getShapeCode()     == kCylindrical);
}

TEST_CASE("StiCylindricalShape — getHalfWidth = outerRadius * sin(openingAngle/2)", "[StiCylindricalShape][DG]") {
    float r  = 60.0f;
    float oa = static_cast<float>(kPi / 6.0);
    StiCylindricalShape s("test", 200.f, 0.4f, r, oa);
    float expected = r * static_cast<float>(sin(oa / 2.0));
    CHECK(s.getHalfWidth() == Approx(expected).epsilon(1e-5));
}

TEST_CASE("StiCylindricalShape — getVolume is positive", "[StiCylindricalShape][DG]") {
    StiCylindricalShape s("test", 200.f, 0.4f, 60.2f, static_cast<float>(kPi/6.));
    CHECK(s.getVolume() > 0.0);
}

TEST_CASE("StiCylindricalShape — setters", "[StiCylindricalShape][DG]") {
    StiCylindricalShape s;
    s.setOuterRadius(50.f);
    s.setOpeningAngle(static_cast<float>(kPi));
    CHECK(s.getOuterRadius()  == Approx(50.f));
    CHECK(s.getOpeningAngle() == Approx(kPi).epsilon(1e-5));
}

// ---------------------------------------------------------------------------
// StiMaterial
// ---------------------------------------------------------------------------

TEST_CASE("StiMaterial — default construction", "[StiMaterial][DG]") {
    StiMaterial m;
    CHECK(m.getDensity() == Approx(0.0));
    CHECK(m.getX0()      == Approx(0.0));
}

TEST_CASE("StiMaterial — 4-parameter construction", "[StiMaterial][DG]") {
    StiMaterial m("Silicon", 14., 28.09, 2.33, 9.37);
    CHECK(m.getName()    == "Silicon");
    CHECK(m.getZ()       == Approx(14.0));
    CHECK(m.getA()       == Approx(28.09));
    CHECK(m.getDensity() == Approx(2.33));
    CHECK(m.getX0()      == Approx(9.37));
}

TEST_CASE("StiMaterial — getZOverA is consistent", "[StiMaterial][DG]") {
    StiMaterial m("Silicon", 14., 28.09, 2.33, 9.37);
    CHECK(m.getZOverA() == Approx(14.0 / 28.09).epsilon(1e-4));
}

TEST_CASE("StiMaterial — set() updates all fields", "[StiMaterial][DG]") {
    StiMaterial m;
    m.set("Argon", 18., 40., 1.784e-3, 14., 0.);
    CHECK(m.getName()    == "Argon");
    CHECK(m.getZ()       == Approx(18.0));
    CHECK(m.getA()       == Approx(40.0));
    CHECK(m.getDensity() == Approx(1.784e-3).epsilon(1e-8));
}

TEST_CASE("StiMaterial — pre-built MockMaterials are well-formed", "[StiMaterial][DG]") {
    StiMaterial* gas = MockMaterials::p10Gas();
    REQUIRE(gas != nullptr);
    CHECK(gas->getDensity() > 0.0);
    CHECK(gas->getX0()      > 0.0);
    CHECK(gas->getZ()       > 0.0);

    StiMaterial* si = MockMaterials::silicon();
    REQUIRE(si != nullptr);
    CHECK(si->getDensity() == Approx(2.33));
}

// ---------------------------------------------------------------------------
// StiElossCalculator
// ---------------------------------------------------------------------------

TEST_CASE("StiElossCalculator — default construction", "[StiElossCalculator][DG]") {
    StiElossCalculator calc;
    // Default construction: all fields zero; calculate() should not crash.
    double de = calc.calculate(1.0, 0.139, 0.9);
    (void)de;
    CHECK(true);
}

TEST_CASE("StiElossCalculator — 5-parameter construction and getters", "[StiElossCalculator][DG]") {
    // Silicon: Z/A = 14/28.09, ionization = 173 eV, A=28.09, Z=14, density=2.33
    StiElossCalculator calc(14./28.09, 173e-6, 28.09, 14., 2.33);
    CHECK(calc.getzOverA()     == Approx(14./28.09).epsilon(1e-4));
    CHECK(calc.getA()          == Approx(28.09));
    CHECK(calc.getZ()          == Approx(14.0));
    CHECK(calc.getDens()       == Approx(2.33));
}

TEST_CASE("StiElossCalculator — calculate returns positive value for pion in silicon", "[StiElossCalculator][DG]") {
    StiElossCalculator calc(14./28.09, 173e-6, 28.09, 14., 2.33);
    // Pion (m = 139.6 MeV/c^2 = 0.1396 GeV/c^2), beta^2 = 0.95, charge^2 = 1
    double dEdx = calc.calculate(1.0, 0.1396, 0.95);
    CHECK(dEdx > 0.0);
}

TEST_CASE("StiElossCalculator — calcError is non-negative", "[StiElossCalculator][DG]") {
    StiElossCalculator calc(14./28.09, 173e-6, 28.09, 14., 2.33);
    double err = calc.calcError(1.0, 0.1396, 0.95);
    CHECK(err >= 0.0);
}

TEST_CASE("StiElossCalculator — set() updates parameters", "[StiElossCalculator][DG]") {
    StiElossCalculator calc;
    calc.set(7./14., 82e-6, 14., 7., 1.205e-3);
    CHECK(calc.getZ() == Approx(7.0));
    CHECK(calc.getA() == Approx(14.0));
}

// ---------------------------------------------------------------------------
// StiPlacement
// ---------------------------------------------------------------------------

TEST_CASE("StiPlacement — default construction", "[StiPlacement][DG]") {
    StiPlacement p;
    CHECK(p.getNormalRefAngle()  == Approx(0.f));
    CHECK(p.getNormalRadius()    == Approx(0.f));
    CHECK(p.getNormalYoffset()   == Approx(0.f));
    CHECK(p.getRegion()          == StiPlacement::kUndefined);
}

TEST_CASE("StiPlacement — 4-parameter construction", "[StiPlacement][DG]") {
    StiPlacement p(0.5f, 60.f, 0.f, 0.f);
    CHECK(p.getNormalRefAngle() == Approx(0.5f));
    CHECK(p.getNormalRadius()   == Approx(60.f));
}

TEST_CASE("StiPlacement — setNormalRep round-trip", "[StiPlacement][DG]") {
    StiPlacement p;
    p.setNormalRep(1.0f, 50.f, 2.f);
    CHECK(p.getNormalRefAngle() == Approx(1.0f));
    CHECK(p.getNormalRadius()   == Approx(50.f));
    CHECK(p.getNormalYoffset()  == Approx(2.f));
}

TEST_CASE("StiPlacement — setLayerRadius / getLayerRadius", "[StiPlacement][DG]") {
    StiPlacement p;
    p.setLayerRadius(75.f);
    CHECK(p.getLayerRadius() == Approx(75.f));
}

TEST_CASE("StiPlacement — setLayerAngle / getLayerAngle", "[StiPlacement][DG]") {
    StiPlacement p;
    p.setLayerAngle(0.7854f); // π/4
    CHECK(p.getLayerAngle() == Approx(0.7854f).epsilon(1e-5));
}

TEST_CASE("StiPlacement — setZcenter / getZcenter", "[StiPlacement][DG]") {
    StiPlacement p;
    p.setZcenter(15.f);
    CHECK(p.getZcenter() == Approx(15.f));
}

TEST_CASE("StiPlacement — setRegion / getRegion", "[StiPlacement][DG]") {
    StiPlacement p;
    p.setRegion(StiPlacement::kMidRapidity);
    CHECK(p.getRegion() == StiPlacement::kMidRapidity);

    p.setRegion(StiPlacement::kForwardRapidity);
    CHECK(p.getRegion() == StiPlacement::kForwardRapidity);

    p.setRegion(StiPlacement::kBackwardRapidity);
    CHECK(p.getRegion() == StiPlacement::kBackwardRapidity);
}

TEST_CASE("StiPlacement — centerRefAngle / centerRadius are set by setNormalRep", "[StiPlacement][DG]") {
    StiPlacement p;
    p.setNormalRep(0.f, 60.f, 0.f);
    // When normalYoffset = 0, the centre representation coincides with the normal.
    CHECK(p.getCenterRadius()   == Approx(60.f));
}

// ---------------------------------------------------------------------------
// StiIsActiveFunctor
// ---------------------------------------------------------------------------

TEST_CASE("StiIsActiveFunctor — default is active everywhere", "[StiIsActiveFunctor][DG]") {
    StiIsActiveFunctor f;
    CHECK(f.isActive() == kTRUE);
    CHECK(f(0.0, 0.0)  == kTRUE);
    CHECK(f(100.0, 50.0) == kTRUE);
}

TEST_CASE("StiIsActiveFunctor — setIsActive disables", "[StiIsActiveFunctor][DG]") {
    StiIsActiveFunctor f;
    f.setIsActive(kFALSE);
    CHECK(f.isActive() == kFALSE);
    CHECK(f(0.0, 0.0)  == kFALSE);
}

TEST_CASE("StiIsActiveFunctor — isEditable / setIsEditable", "[StiIsActiveFunctor][DG]") {
    StiIsActiveFunctor f;
    CHECK(f.isEditable() == kTRUE);
    f.setIsEditable(kFALSE);
    CHECK(f.isEditable() == kFALSE);
}

// ---------------------------------------------------------------------------
// StiDetector
// ---------------------------------------------------------------------------

TEST_CASE("StiDetector — construction is blank", "[StiDetector][DG]") {
    StiDetector det;
    CHECK(det.getShape()     == nullptr);
    CHECK(det.getPlacement() == nullptr);
    CHECK(det.getGas()       == nullptr);
    CHECK(det.getMaterial()  == nullptr);
}

TEST_CASE("StiDetector — setName / getName", "[StiDetector][DG]") {
    StiDetector det;
    det.setName("TPC/Row_00/Sec_00");
    CHECK(det.getName() == "TPC/Row_00/Sec_00");
}

TEST_CASE("StiDetector — full assembly via setters", "[StiDetector][DG]") {
    StiCylindricalShape shape("s", 200.f, 0.4f, 60.2f,
                              static_cast<float>(kPi / 6.0));
    StiMaterial gas("P10", 17.4, 37.8, 1.56e-3, 11.7);
    StiMaterial mat("P10", 17.4, 37.8, 1.56e-3, 11.7);
    StiPlacement placement(0.f, 60.f, 0.f, 0.f);
    StiIsActiveFunctor active;

    StiDetector det;
    det.setShape(&shape);
    det.setGas(&gas);
    det.setMaterial(&mat);
    det.setPlacement(&placement);
    det.setIsActive(&active);

    CHECK(det.getShape()     == &shape);
    CHECK(det.getGas()       == &gas);
    CHECK(det.getMaterial()  == &mat);
    CHECK(det.getPlacement() == &placement);
}

TEST_CASE("StiDetector — isActive delegates to functor", "[StiDetector][DG]") {
    StiCylindricalShape shape("s", 200.f, 0.4f, 60.f,
                              static_cast<float>(kPi / 6.0));
    StiMaterial gas("P10", 17.4, 37.8, 1.56e-3, 11.7);
    StiMaterial mat("P10", 17.4, 37.8, 1.56e-3, 11.7);
    StiPlacement placement(0.f, 60.f, 0.f, 0.f);
    StiIsActiveFunctor active;

    StiDetector det;
    det.setShape(&shape);
    det.setGas(&gas);
    det.setMaterial(&mat);
    det.setPlacement(&placement);
    det.setIsActive(&active);

    CHECK(det.isActive()          == true);
    CHECK(det.isActive(0.0, 0.0)  == true);

    active.setIsActive(kFALSE);
    CHECK(det.isActive()          == false);
}

TEST_CASE("StiDetector — setKey / getKey round-trip", "[StiDetector][DG]") {
    StiDetector det;
    det.setKey(1, 5);
    det.setKey(2, 99);
    CHECK(det.getKey(1) == 5);
    CHECK(det.getKey(2) == 99);
    CHECK(det.getKey(0) == -1); // invalid index
}

TEST_CASE("StiDetector — setGroupId / getGroupId", "[StiDetector][DG]") {
    StiDetector det;
    det.setGroupId(3);
    CHECK(det.getGroupId() == 3);
    // second call is ignored (setGroupId is idempotent)
    det.setGroupId(7);
    CHECK(det.getGroupId() == 3);
}

TEST_CASE("StiDetector — getVolume is positive for valid shape", "[StiDetector][DG]") {
    StiCylindricalShape shape("s", 200.f, 0.4f, 60.2f,
                              static_cast<float>(kPi / 6.0));
    StiMaterial gas("P10", 17.4, 37.8, 1.56e-3, 11.7);
    StiMaterial mat("P10", 17.4, 37.8, 1.56e-3, 11.7);
    StiPlacement placement(0.f, 60.f, 0.f, 0.f);
    StiIsActiveFunctor active;
    StiDetector det;
    det.setShape(&shape);
    det.setGas(&gas);
    det.setMaterial(&mat);
    det.setPlacement(&placement);
    det.setIsActive(&active);

    CHECK(det.getVolume() > 0.0);
}

// ---------------------------------------------------------------------------
// StiDetectorBuilder / MockDetectorBuilder
// ---------------------------------------------------------------------------

TEST_CASE("MockDetectorBuilder — construction", "[StiDetectorBuilder][DG]") {
    MockDetectorBuilder builder;
    CHECK(builder.getNRows() == 0u);
}

TEST_CASE("MockDetectorBuilder — buildDetectors populates rows and sectors", "[StiDetectorBuilder][DG]") {
    MockDetectorBuilder builder;
    // buildDetectors() normally takes a StMaker&; we pass a dummy reference.
    // Since our override ignores the argument, this is safe.
    StMaker* dummy = nullptr;
    builder.buildDetectors(*reinterpret_cast<StMaker*>(dummy));

    using namespace MockShapes;
    const int expectedRows = kNTpcRows + kNSiLayers;
    CHECK(static_cast<int>(builder.getNRows()) == expectedRows);
}

TEST_CASE("MockDetectorBuilder — TPC sectors per row are correct", "[StiDetectorBuilder][DG]") {
    MockDetectorBuilder builder;
    StMaker* dummy = nullptr;
    builder.buildDetectors(*reinterpret_cast<StMaker*>(dummy));

    CHECK(static_cast<int>(builder.getNSectors(0)) == MockShapes::kNTpcSectors);
}

TEST_CASE("MockDetectorBuilder — silicon layers have 1 sector", "[StiDetectorBuilder][DG]") {
    MockDetectorBuilder builder;
    StMaker* dummy = nullptr;
    builder.buildDetectors(*reinterpret_cast<StMaker*>(dummy));

    int siRowOffset = MockShapes::kNTpcRows;
    CHECK(builder.getNSectors(siRowOffset) == 1u);
}

TEST_CASE("MockDetectorBuilder — getDetector returns non-null for valid row/sector", "[StiDetectorBuilder][DG]") {
    MockDetectorBuilder builder;
    StMaker* dummy = nullptr;
    builder.buildDetectors(*reinterpret_cast<StMaker*>(dummy));

    StiDetector* det = builder.getDetector(0, 0);
    REQUIRE(det != nullptr);
    CHECK(det->getShape()     != nullptr);
    CHECK(det->getPlacement() != nullptr);
    CHECK(det->getGas()       != nullptr);
    CHECK(det->getMaterial()  != nullptr);
}

TEST_CASE("MockDetectorBuilder — add/find material", "[StiDetectorBuilder][DG]") {
    MockDetectorBuilder builder;
    StiMaterial* p10 = MockMaterials::p10Gas();
    builder.add(p10);
    StiMaterial* found = builder.findMaterial(p10->getName());
    CHECK(found == p10);
}

TEST_CASE("MockDetectorBuilder — add/find shape", "[StiDetectorBuilder][DG]") {
    MockDetectorBuilder builder;
    StiCylindricalShape* s = MockShapes::makeTpcRowShape(0);
    builder.add(s);
    StiShape* found = builder.findShape(s->getName());
    CHECK(found == s);
    delete s;
}

TEST_CASE("MockDetectorBuilder — setGroupId / getGroupId", "[StiDetectorBuilder][DG]") {
    MockDetectorBuilder builder;
    builder.setGroupId(42);
    CHECK(builder.getGroupId() == 42);
}
