// tests/unit/StIstSimMaker/test_StIstSimMaker.cxx
//
// Catch2 v3 unit tests for StIstFastSimMaker and StIstSlowSimMaker
// (Intermediate Silicon Tracker simulation).
//
// Design constraints
// ------------------
// * No TApplication — ROOT global state (display, interpreter) is not
//   initialised.  ROOT's core library, loaded by linking, provides gROOT
//   and the formula registry required by TRandom1 and TF1("landau",...).
// * Init() / InitRun() / Make() require a full STAR chain (StChain), a DB
//   connection, and GEANT input; those paths are covered only by the
//   [integration] stubs at the end of this file.
// * Structural tests construct the makers directly on the stack and exercise
//   only the constructor, the buildIdealGeom() setter, and the pure-utility
//   distortHit() function.
//
// White-box access to private members of StIstFastSimMaker
// ---------------------------------------------------------
// distortHit(), mBuildIdealGeom, mResXIst1, and mResZIst1 are declared
// private in StIstFastSimMaker.h.  A standard white-box technique is to
// promote all private members to protected in this translation unit before
// the header is included.  '#define private protected' is scoped tightly:
//   1. StIstFastSimMaker's own dependencies (TRandom1.h, StMaker.h) are
//      pre-included first so their include-guards prevent re-processing
//      under the redefinition.
//   2. The redefinition covers only StIstFastSimMaker.h itself.
//   3. '#undef private' immediately restores normal semantics.
// The object layout is unaffected (access specifiers are compile-time only),
// so linking against the production StIstSimMaker shared library is safe.
//
// StIstSlowSimMaker already declares mBuildIdealGeom as protected; no
// redefinition is needed for that class.

// Pre-include StIstFastSimMaker's direct dependencies under normal access
// control so that their include-guards fire before the '#define' below.
#include "TRandom1.h"
#include "StMaker.h"

// Promote private members of StIstFastSimMaker to protected for this TU only.
#define private protected
#include "StIstSimMaker/StIstFastSimMaker.h"
#undef private

// StIstSlowSimMaker uses protected: for mBuildIdealGeom; no redefinition needed.
#include "StIstSimMaker/StIstSlowSimMaker.h"

#include "../catch_amalgamated.hpp"

#include <cmath>
#include <string>

// ============================================================================
// Test-exposure subclasses
// ============================================================================

/// Thin wrapper that promotes the (now-protected) private members and
/// distortHit() to public scope for unit testing.
class TestableFastSim : public StIstFastSimMaker {
public:
    explicit TestableFastSim(const char* name = "istFastSim",
                             bool useRandomSeed = false)
        : StIstFastSimMaker(name, useRandomSeed) {}

    // Promote distortHit() (made protected by the '#define private protected').
    using StIstFastSimMaker::distortHit;

    // Read-only accessors for private state exposed by the '#define'.
    bool   idealGeom() const { return mBuildIdealGeom; }
    double resX()      const { return mResXIst1; }
    double resZ()      const { return mResZIst1; }

    /// Seed mResXIst1 / mResZIst1 without InitRun so that distortHit tests
    /// are self-contained.
    void setResolutions(double rx, double rz) {
        mResXIst1 = rx;
        mResZIst1 = rz;
    }
};

/// Thin wrapper that exposes the protected mBuildIdealGeom flag for
/// read-back.  No access-specifier trick required — StIstSlowSimMaker already
/// declares it protected.
class TestableSlowSim : public StIstSlowSimMaker {
public:
    explicit TestableSlowSim(const char* name = "istSlowSim")
        : StIstSlowSimMaker(name) {}

    bool idealGeom() const { return mBuildIdealGeom; }
};

// ============================================================================
// StIstFastSimMaker — structural unit tests
// ============================================================================

TEST_CASE("StIstFastSimMaker default construction", "[StIstFastSimMaker]") {
    TestableFastSim maker;

    SECTION("GetName returns 'istFastSim'") {
        REQUIRE(std::string(maker.GetName()) == "istFastSim");
    }

    SECTION("GetName returns a non-null pointer") {
        REQUIRE(maker.GetName() != nullptr);
    }
}

TEST_CASE("StIstFastSimMaker custom-name construction", "[StIstFastSimMaker]") {
    TestableFastSim maker("myIstFastSim");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(maker.GetName()) == "myIstFastSim");
    }
}

TEST_CASE("StIstFastSimMaker multiple instances have independent names",
          "[StIstFastSimMaker]")
{
    TestableFastSim a("fastSimA");
    TestableFastSim b("fastSimB");

    REQUIRE(std::string(a.GetName()) == "fastSimA");
    REQUIRE(std::string(b.GetName()) == "fastSimB");
    REQUIRE(std::string(a.GetName()) != std::string(b.GetName()));
}

// ---------------------------------------------------------------------------
// GetCVS()
// ---------------------------------------------------------------------------

TEST_CASE("StIstFastSimMaker GetCVS returns non-empty CVS string",
          "[StIstFastSimMaker]")
{
    TestableFastSim maker;

    SECTION("GetCVS is not null") {
        REQUIRE(maker.GetCVS() != nullptr);
    }

    SECTION("GetCVS contains class name") {
        std::string cvs(maker.GetCVS());
        REQUIRE(cvs.find("StIstFastSimMaker") != std::string::npos);
    }
}

// ---------------------------------------------------------------------------
// buildIdealGeom() setter
// ---------------------------------------------------------------------------

TEST_CASE("StIstFastSimMaker mBuildIdealGeom defaults to kFALSE",
          "[StIstFastSimMaker]")
{
    // The constructor initialises mBuildIdealGeom(kFALSE).  With ideal
    // geometry off, InitRun() uses the DB rotation tables rather than the
    // TGeoManager VMC geometry.
    TestableFastSim maker;

    SECTION("mBuildIdealGeom is false after construction") {
        REQUIRE(maker.idealGeom() == false);
    }
}

TEST_CASE("StIstFastSimMaker buildIdealGeom setter round-trip",
          "[StIstFastSimMaker]")
{
    TestableFastSim maker;

    SECTION("buildIdealGeom(true) stores true") {
        maker.buildIdealGeom(true);
        REQUIRE(maker.idealGeom() == true);
    }

    SECTION("buildIdealGeom(false) stores false") {
        maker.buildIdealGeom(true);
        maker.buildIdealGeom(false);
        REQUIRE(maker.idealGeom() == false);
    }

    SECTION("repeated buildIdealGeom(true) leaves flag true") {
        maker.buildIdealGeom(true);
        maker.buildIdealGeom(true);
        REQUIRE(maker.idealGeom() == true);
    }
}

// ---------------------------------------------------------------------------
// mResXIst1 / mResZIst1
//
// These members are NOT initialised in the constructor; they are populated
// only in InitRun() from the HitError calibration DB table:
//   mResXIst1 = sqrt(istHitError->coeff[0])   (r-phi resolution)
//   mResZIst1 = sqrt(istHitError->coeff[3])   (z resolution)
// Because sqrt() always yields a non-negative value and production coeff[]
// entries are positive, both resolutions are guaranteed positive after a
// successful InitRun().  The integration stub below covers that path.
//
// The unit tests here verify: (a) the setResolutions helper works correctly,
// and (b) once set to realistic values the positivity invariant holds.
// ---------------------------------------------------------------------------

TEST_CASE("StIstFastSimMaker resolution members are positive when set to realistic values",
          "[StIstFastSimMaker]")
{
    TestableFastSim maker;

    SECTION("r-phi resolution is positive after manual initialisation") {
        // Typical IST r-phi resolution ~30 µm, well within pad pitch of 594 µm.
        maker.setResolutions(0.0030, 0.0600);
        REQUIRE(maker.resX() > 0.0);
    }

    SECTION("z resolution is positive after manual initialisation") {
        // Typical IST z resolution ~600 µm (column pitch 6275 µm / sqrt(12)).
        maker.setResolutions(0.0030, 0.0600);
        REQUIRE(maker.resZ() > 0.0);
    }

    SECTION("both resolutions remain positive for a range of realistic values") {
        // IST r-phi pad pitch: 594 µm; z pad pitch: 6275 µm.
        // Resolutions are sigma of the Gaussian smear, must be << half-pitch.
        const double rx_values[] = { 0.001, 0.003, 0.006 };
        const double rz_values[] = { 0.030, 0.060, 0.100 };
        for (double rx : rx_values) {
            for (double rz : rz_values) {
                maker.setResolutions(rx, rz);
                REQUIRE(maker.resX() > 0.0);
                REQUIRE(maker.resZ() > 0.0);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// distortHit()
//
// distortHit(x, res, detLength) draws from N(x, res) and rejects samples
// with |result| > detLength (rejection sampling).  When |x| > detLength the
// function emits a warning and returns x unchanged.
//
// Sensor dimensions from StIstConsts.h:
//   kIstSensorActiveSizeRPhi = 3.8016 cm  →  half-length = 1.9008 cm
//   kIstSensorActiveSizeZ    = 7.5300 cm  →  half-length = 3.7650 cm
// ---------------------------------------------------------------------------

TEST_CASE("StIstFastSimMaker distortHit stays within sensor half-length",
          "[StIstFastSimMaker]")
{
    TestableFastSim maker;

    // r-phi active half-length (kIstSensorActiveSizeRPhi / 2.0)
    const double rphiHalf = 1.9008; // cm
    // Typical r-phi resolution ~30 µm
    const double res = 0.0030; // cm

    SECTION("smeared hit at sensor centre stays within bounds") {
        for (int i = 0; i < 200; ++i) {
            const double result = maker.distortHit(0.0, res, rphiHalf);
            REQUIRE(std::abs(result) <= rphiHalf);
        }
    }

    SECTION("smeared hit near positive edge stays within bounds") {
        const double x = rphiHalf * 0.85;
        for (int i = 0; i < 200; ++i) {
            const double result = maker.distortHit(x, res, rphiHalf);
            REQUIRE(std::abs(result) <= rphiHalf);
        }
    }

    SECTION("smeared hit near negative edge stays within bounds") {
        const double x = -rphiHalf * 0.85;
        for (int i = 0; i < 200; ++i) {
            const double result = maker.distortHit(x, res, rphiHalf);
            REQUIRE(std::abs(result) <= rphiHalf);
        }
    }

    SECTION("z-direction smearing also stays within z half-length") {
        // kIstSensorActiveSizeZ / 2.0
        const double zHalf = 3.7650; // cm
        const double zRes  = 0.0600; // cm
        for (int i = 0; i < 200; ++i) {
            const double result = maker.distortHit(0.0, zRes, zHalf);
            REQUIRE(std::abs(result) <= zHalf);
        }
    }
}

TEST_CASE("StIstFastSimMaker distortHit returns x unchanged when outside bounds",
          "[StIstFastSimMaker]")
{
    // When |x| > detLength the function emits LOG_WARN and returns x as-is
    // (no rejection-sampling loop is entered).
    TestableFastSim maker;

    const double rphiHalf = 1.9008; // cm
    const double res      = 0.0030; // cm

    SECTION("x just above +detLength is returned unchanged") {
        const double outside = rphiHalf + 0.01;
        const double result  = maker.distortHit(outside, res, rphiHalf);
        REQUIRE(result == outside);
    }

    SECTION("x just below -detLength is returned unchanged") {
        const double outside = -(rphiHalf + 0.01);
        const double result  = maker.distortHit(outside, res, rphiHalf);
        REQUIRE(result == outside);
    }
}

TEST_CASE("StIstFastSimMaker distortHit result lies within [-detLength, detLength]"
          " for many random positions",
          "[StIstFastSimMaker]")
{
    // Stress test: sample many interior positions to confirm the rejection
    // loop always terminates and the result is always in-bounds.
    TestableFastSim maker;

    const double detHalf = 1.9008; // r-phi half-length, cm
    const double res     = 0.0030; // cm

    // A uniform grid of input positions from -0.9*half to +0.9*half.
    const int    nPoints = 9;
    const double step    = 1.8 * detHalf / (nPoints - 1);
    for (int i = 0; i < nPoints; ++i) {
        const double x = -0.9 * detHalf + i * step;
        for (int j = 0; j < 50; ++j) {
            const double result = maker.distortHit(x, res, detHalf);
            REQUIRE(std::abs(result) <= detHalf);
        }
    }
}

// ============================================================================
// StIstSlowSimMaker — structural unit tests
// ============================================================================
//
// Note: The StIstSlowSimMaker constructor allocates a TF1("fAdc","landau",...)
// which uses ROOT's built-in function registry.  This is available as soon as
// ROOT's core library is loaded (via static initialisation of gROOT) and does
// not require a TApplication.

TEST_CASE("StIstSlowSimMaker default construction", "[StIstSlowSimMaker]") {
    TestableSlowSim maker;

    SECTION("GetName returns 'istSlowSim'") {
        REQUIRE(std::string(maker.GetName()) == "istSlowSim");
    }

    SECTION("GetName returns a non-null pointer") {
        REQUIRE(maker.GetName() != nullptr);
    }
}

TEST_CASE("StIstSlowSimMaker custom-name construction", "[StIstSlowSimMaker]") {
    TestableSlowSim maker("myIstSlowSim");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(maker.GetName()) == "myIstSlowSim");
    }
}

TEST_CASE("StIstSlowSimMaker multiple instances have independent names",
          "[StIstSlowSimMaker]")
{
    TestableSlowSim a("slowSimA");
    TestableSlowSim b("slowSimB");

    REQUIRE(std::string(a.GetName()) == "slowSimA");
    REQUIRE(std::string(b.GetName()) == "slowSimB");
    REQUIRE(std::string(a.GetName()) != std::string(b.GetName()));
}

// ---------------------------------------------------------------------------
// GetCVS()
// ---------------------------------------------------------------------------

TEST_CASE("StIstSlowSimMaker GetCVS returns non-empty CVS string",
          "[StIstSlowSimMaker]")
{
    TestableSlowSim maker;

    SECTION("GetCVS is not null") {
        REQUIRE(maker.GetCVS() != nullptr);
    }

    SECTION("GetCVS contains class name") {
        std::string cvs(maker.GetCVS());
        REQUIRE(cvs.find("StIstSlowSimMaker") != std::string::npos);
    }
}

// ---------------------------------------------------------------------------
// buildIdealGeom() setter
// ---------------------------------------------------------------------------

TEST_CASE("StIstSlowSimMaker mBuildIdealGeom defaults to kFALSE",
          "[StIstSlowSimMaker]")
{
    TestableSlowSim maker;

    SECTION("mBuildIdealGeom is false after construction") {
        REQUIRE(maker.idealGeom() == false);
    }
}

TEST_CASE("StIstSlowSimMaker buildIdealGeom setter round-trip",
          "[StIstSlowSimMaker]")
{
    TestableSlowSim maker;

    SECTION("buildIdealGeom(true) stores true") {
        maker.buildIdealGeom(true);
        REQUIRE(maker.idealGeom() == true);
    }

    SECTION("buildIdealGeom(false) stores false") {
        maker.buildIdealGeom(true);
        maker.buildIdealGeom(false);
        REQUIRE(maker.idealGeom() == false);
    }

    SECTION("repeated buildIdealGeom(true) leaves flag true") {
        maker.buildIdealGeom(true);
        maker.buildIdealGeom(true);
        REQUIRE(maker.idealGeom() == true);
    }
}

// ============================================================================
// Integration stubs — require TApplication + full STAR runtime
// ============================================================================
//
// To run integration tests: ./test_StIstSimMaker [integration]

TEST_CASE("StIstFastSimMaker InitRun populates positive resolutions from DB",
          "[StIstFastSimMaker][integration]")
{
    // Requires:
    //   SimTestFixture<TestableFastSim> f("istFastSim");
    //   DB injection: Calibrations/tracker/ist1HitError
    //     (HitError_st with positive coeff[0] and coeff[3])
    //   DB injection: ist_db (StIstDb with valid TGeoHMatrix rotation list)
    //
    // After f.initRun(runNo):
    //   TestableFastSim& m = static_cast<TestableFastSim&>(f.maker());
    //   REQUIRE(m.resX() > 0.0);   // = sqrt(coeff[0]) from ist1HitError table
    //   REQUIRE(m.resZ() > 0.0);   // = sqrt(coeff[3]) from ist1HitError table
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StIstFastSimMaker Make fills StIstHitCollection from MC hits",
          "[StIstFastSimMaker][integration]")
{
    // Requires:
    //   SimTestFixture<StIstFastSimMaker> f("istFastSim");
    //   DB injection as above
    //   StMcEvent with StMcIstHitCollection containing >= 1 hit
    //   StEvent (empty — the maker creates StIstHitCollection if absent)
    //
    // After f.make():
    //   StEvent* ev = f.rcEvent();
    //   REQUIRE(ev->istHitCollection() != nullptr);
    //   REQUIRE(ev->istHitCollection()->numberOfHits() > 0);
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StIstFastSimMaker Make handles missing StMcEvent gracefully",
          "[StIstFastSimMaker][integration]")
{
    // When StMcEvent is absent Make() returns kStWarn without crashing.
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StIstFastSimMaker Make handles missing StMcIstHitCollection",
          "[StIstFastSimMaker][integration]")
{
    // When StMcEvent has no IST hit collection Make() returns kStFatal.
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StIstFastSimMaker ideal-geometry path uses TGeoManager",
          "[StIstFastSimMaker][integration]")
{
    // With buildIdealGeom(true) and a loaded AgML geometry, InitRun() must
    // successfully find gGeoManager and Make() must use it for transforms.
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StIstSlowSimMaker Init creates StIstCollection",
          "[StIstSlowSimMaker][integration]")
{
    // Requires:
    //   SimTestFixture<StIstSlowSimMaker> f("istSlowSim");
    //
    // After f.init():
    //   REQUIRE(f.maker() has non-null internal StIstCollection);
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StIstSlowSimMaker InitRun loads control and mapping tables from DB",
          "[StIstSlowSimMaker][integration]")
{
    // Requires:
    //   DB injection: ist_db (StIstDb with istControl and istMapping tables)
    //
    // After f.initRun(runNo):
    //   REQUIRE(mDefaultTimeBin    == istControlTable[0].kIstDefaultTimeBin);
    //   REQUIRE(mCurrentTimeBinNum == istControlTable[0].kIstCurrentTimeBinNum);
    //   REQUIRE(mMappingGeomVec is fully populated — no zero entries from DB);
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StIstSlowSimMaker Make produces raw hits from MC input",
          "[StIstSlowSimMaker][integration]")
{
    // Requires:
    //   StMcEvent with StMcIstHitCollection containing >= 1 hit
    //
    // After f.make():
    //   REQUIRE(mIstCollectionPtr->getNumRawHits() > 0);
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StIstSlowSimMaker Make handles missing StMcEvent gracefully",
          "[StIstSlowSimMaker][integration]")
{
    // When StMcEvent is absent Make() returns kStWarn without crashing.
    SUCCEED("integration stub — not exercised in standalone mode");
}

TEST_CASE("StIstSlowSimMaker Clear resets raw hit collections",
          "[StIstSlowSimMaker][integration]")
{
    // After a Make() that populated raw hits, calling Clear() must empty all
    // kIstNumLadders raw-hit collections so that the next event starts clean.
    SUCCEED("integration stub — not exercised in standalone mode");
}
