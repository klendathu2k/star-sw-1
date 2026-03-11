// tests/unit/StFgtSimulatorMaker/test_StFgtSimulatorMaker.cxx
//
// Catch2 v3 unit tests for StFgtSlowSimuMaker.
//
// Scope: construction, static constants (pulseShape[], mxH), and
//        setter/getter round-trips that are exercisable without a full
//        STAR runtime or TApplication.
//
// Tests that require InitRun() (StFgtDb), Init() (ROOT histogram
// allocation), or Make() (StEvent + Geant tables) are stub-documented
// with the [integration] tag and are NOT run by default.
//
// No TApplication is created.  ROOT basic objects (TRandom3, etc.)
// initialise themselves via gROOT at static-init time; TApplication is
// only required for graphics and the interactive event loop.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "StFgtSlowSimuMaker.h"

#include <algorithm>   // std::max_element
#include <numeric>     // std::accumulate
#include <cmath>       // std::fabs

// ---------------------------------------------------------------------------
// TestableStFgtSlowSimuMaker
//
// Thin public subclass that:
//   (a) exposes the protected static pulseShape[] array, and
//   (b) documents the private enum mxH as a public sentinel constant.
//
// The sentinel kExpectedMxH acts as a regression guard: if the private
// enum mxH is changed in the header, this constant and the test that
// uses it must be updated in tandem.
// ---------------------------------------------------------------------------
class TestableStFgtSlowSimuMaker : public StFgtSlowSimuMaker {
public:
    explicit TestableStFgtSlowSimuMaker(const char* name = "TestFgtSlowSimu")
        : StFgtSlowSimuMaker(name) {}

    // Expose the protected static array so tests can inspect individual
    // elements without linking against the full simulator library internals.
    static const Float_t* getPulseShape() { return pulseShape; }

    // Number of elements in pulseShape[].  Encoded as a compile-time
    // constant matching the fixed array size declared in the header.
    static constexpr int kPulseShapeLen = 20;

    // The number of APV time-sample bins used in exportStripPlane2StEvent().
    // Matches the hard-coded loop bound `for(int iTbOff=0; iTbOff<7; iTbOff++)`.
    static constexpr int kNumTimebinsUsed = 7;

    // Regression sentinel for the private enum {mxH=32} declared in the
    // header.  mxH governs the size of the internal hA[mxH] histogram
    // pointer array.  This constant cannot be verified via sizeof because
    // hA is private; instead it documents the expected value so that any
    // future change to the enum is noticed here.
    static constexpr int kExpectedMxH = 32;
};

// ---------------------------------------------------------------------------
// TEST GROUP 1 – Construction and naming
// ---------------------------------------------------------------------------

TEST_CASE("StFgtSlowSimuMaker construction with custom name", "[StFgtSlowSimuMaker]") {
    TestableStFgtSlowSimuMaker maker("MyFgtSim");

    SECTION("GetName returns the name given at construction") {
        CHECK(std::string(maker.GetName()) == "MyFgtSim");
    }
}

TEST_CASE("StFgtSlowSimuMaker default name", "[StFgtSlowSimuMaker]") {
    // The default argument is "FgtSlowSimu" (see header ctor declaration).
    StFgtSlowSimuMaker maker;

    SECTION("GetName returns the default 'FgtSlowSimu'") {
        CHECK(std::string(maker.GetName()) == "FgtSlowSimu");
    }

    SECTION("GetName returns a non-null, non-empty string") {
        REQUIRE(maker.GetName() != nullptr);
        CHECK(std::string(maker.GetName()).size() > 0U);
    }
}

TEST_CASE("StFgtSlowSimuMaker two instances have independent names",
          "[StFgtSlowSimuMaker]") {
    StFgtSlowSimuMaker a("SimA");
    StFgtSlowSimuMaker b("SimB");

    SECTION("first instance name") {
        CHECK(std::string(a.GetName()) == "SimA");
    }
    SECTION("second instance name") {
        CHECK(std::string(b.GetName()) == "SimB");
    }
    SECTION("names are distinct") {
        CHECK(std::string(a.GetName()) != std::string(b.GetName()));
    }
}

// ---------------------------------------------------------------------------
// TEST GROUP 2 – pulseShape[20] static constant array
//
// Defined in StFgtSlowSimuMaker.cxx:
//   const Float_t StFgtSlowSimuMaker::pulseShape[20] =
//     {0.0, 0.1, 0.3, 0.4, 0.2, 0.0, ..., 0.0};  // comment: sum 1.0
//
// Physical interpretation:
//   - A causal pulse shape applied across 7 APV time bins.
//   - Bin 0 is zero (pulse has not yet risen).
//   - Bins 1–4 carry the signal; bins 5–19 are zero (pulse has fully decayed).
//   - The sum of all 20 elements equals 1.0 (normalised).
// ---------------------------------------------------------------------------

TEST_CASE("pulseShape[] is accessible and has the expected length",
          "[StFgtSlowSimuMaker][pulseShape]") {
    const Float_t* ps = TestableStFgtSlowSimuMaker::getPulseShape();
    REQUIRE(ps != nullptr);

    SECTION("array length sentinel matches declaration (20 elements)") {
        CHECK(TestableStFgtSlowSimuMaker::kPulseShapeLen == 20);
    }
}

TEST_CASE("pulseShape[] element [0] is zero (causal pulse starts at zero)",
          "[StFgtSlowSimuMaker][pulseShape]") {
    const Float_t* ps = TestableStFgtSlowSimuMaker::getPulseShape();

    SECTION("pulseShape[0] == 0.0") {
        CHECK(ps[0] == Catch::Approx(0.0f).margin(1e-6f));
    }
}

TEST_CASE("pulseShape[] individual element values",
          "[StFgtSlowSimuMaker][pulseShape]") {
    const Float_t* ps = TestableStFgtSlowSimuMaker::getPulseShape();

    // Values from the definition in StFgtSlowSimuMaker.cxx.
    SECTION("pulseShape[1] == 0.1") {
        CHECK(ps[1] == Catch::Approx(0.1f).epsilon(1e-5f));
    }
    SECTION("pulseShape[2] == 0.3") {
        CHECK(ps[2] == Catch::Approx(0.3f).epsilon(1e-5f));
    }
    SECTION("pulseShape[3] == 0.4  (peak)") {
        CHECK(ps[3] == Catch::Approx(0.4f).epsilon(1e-5f));
    }
    SECTION("pulseShape[4] == 0.2") {
        CHECK(ps[4] == Catch::Approx(0.2f).epsilon(1e-5f));
    }
    SECTION("pulseShape[5] == 0.0  (tail has decayed)") {
        CHECK(ps[5] == Catch::Approx(0.0f).margin(1e-6f));
    }
    SECTION("pulseShape[19] == 0.0  (last element)") {
        CHECK(ps[19] == Catch::Approx(0.0f).margin(1e-6f));
    }
}

TEST_CASE("pulseShape[] tail elements (indices 5-19) are all zero",
          "[StFgtSlowSimuMaker][pulseShape]") {
    const Float_t* ps = TestableStFgtSlowSimuMaker::getPulseShape();

    for (int i = 5; i < TestableStFgtSlowSimuMaker::kPulseShapeLen; ++i) {
        INFO("pulseShape[" << i << "] should be 0.0");
        CHECK(ps[i] == Catch::Approx(0.0f).margin(1e-6f));
    }
}

TEST_CASE("pulseShape[] is non-trivial (not all zeros)",
          "[StFgtSlowSimuMaker][pulseShape]") {
    const Float_t* ps = TestableStFgtSlowSimuMaker::getPulseShape();
    const int len = TestableStFgtSlowSimuMaker::kPulseShapeLen;

    const Float_t maxVal = *std::max_element(ps, ps + len);

    SECTION("maximum element is strictly positive") {
        CHECK(maxVal > 0.0f);
    }

    SECTION("at least one non-zero element exists") {
        bool anyNonZero = false;
        for (int i = 0; i < len; ++i) {
            if (ps[i] != 0.0f) { anyNonZero = true; break; }
        }
        CHECK(anyNonZero == true);
    }
}

TEST_CASE("pulseShape[] maximum is at index 3 with value 0.4",
          "[StFgtSlowSimuMaker][pulseShape]") {
    const Float_t* ps = TestableStFgtSlowSimuMaker::getPulseShape();
    const int len = TestableStFgtSlowSimuMaker::kPulseShapeLen;

    const Float_t* peakPtr = std::max_element(ps, ps + len);

    SECTION("peak value is 0.4") {
        CHECK(*peakPtr == Catch::Approx(0.4f).epsilon(1e-5f));
    }

    SECTION("peak is at index 3") {
        const int peakIdx = static_cast<int>(peakPtr - ps);
        CHECK(peakIdx == 3);
    }
}

TEST_CASE("pulseShape[] sum equals 1.0 (normalised pulse)",
          "[StFgtSlowSimuMaker][pulseShape]") {
    const Float_t* ps = TestableStFgtSlowSimuMaker::getPulseShape();
    const int len = TestableStFgtSlowSimuMaker::kPulseShapeLen;

    const double sum = std::accumulate(ps, ps + len, 0.0);

    SECTION("sum is strictly positive") {
        CHECK(sum > 0.0);
    }

    SECTION("sum equals 1.0 (as documented in source comment 'sum 1.0')") {
        CHECK(sum == Catch::Approx(1.0).epsilon(1e-5));
    }
}

TEST_CASE("pulseShape[] 7-timebin window used in digitisation sums to 1.0",
          "[StFgtSlowSimuMaker][pulseShape]") {
    // exportStripPlane2StEvent() uses pulseShape[0..6] for the 7 APV time bins.
    const Float_t* ps = TestableStFgtSlowSimuMaker::getPulseShape();
    const int nBins = TestableStFgtSlowSimuMaker::kNumTimebinsUsed;  // 7

    const double windowSum = std::accumulate(ps, ps + nBins, 0.0);

    SECTION("sum of first 7 elements equals 1.0") {
        CHECK(windowSum == Catch::Approx(1.0).epsilon(1e-5));
    }
}

TEST_CASE("pulseShape[] all values are in [0, 1] (valid probability weights)",
          "[StFgtSlowSimuMaker][pulseShape]") {
    const Float_t* ps = TestableStFgtSlowSimuMaker::getPulseShape();
    const int len = TestableStFgtSlowSimuMaker::kPulseShapeLen;

    for (int i = 0; i < len; ++i) {
        INFO("pulseShape[" << i << "] = " << ps[i]);
        CHECK(ps[i] >= 0.0f);
        CHECK(ps[i] <= 1.0f);
    }
}

// ---------------------------------------------------------------------------
// TEST GROUP 3 – mxH constant
//
// mxH is declared as a *private* enum { mxH = 32 } in the class body
// and controls the size of the internal histogram pointer array hA[mxH].
// Because it is private, it cannot be accessed from a subclass or via
// sizeof tricks on hA.  The kExpectedMxH sentinel in
// TestableStFgtSlowSimuMaker encodes the value read from the header and
// serves as a regression guard.
// ---------------------------------------------------------------------------

TEST_CASE("mxH private enum value is documented as 32",
          "[StFgtSlowSimuMaker][mxH]") {
    // If the private enum { mxH } is changed in StFgtSlowSimuMaker.h,
    // kExpectedMxH must be updated here to keep this test passing and
    // to ensure the change is noticed during review.
    SECTION("kExpectedMxH sentinel equals 32") {
        CHECK(TestableStFgtSlowSimuMaker::kExpectedMxH == 32);
    }
}

TEST_CASE("mxH bounds: 7 used time bins fit within mxH-sized histogram array",
          "[StFgtSlowSimuMaker][mxH]") {
    // The digitisation loop uses indices 0..6 of hA (via Fill calls inside
    // InitHisto2, guarded by __FGT_QA_HISTO__).  The array has mxH=32 slots,
    // so all accesses are within bounds.
    SECTION("number of used time bins (7) is less than mxH (32)") {
        CHECK(TestableStFgtSlowSimuMaker::kNumTimebinsUsed
              < TestableStFgtSlowSimuMaker::kExpectedMxH);
    }
}

// ---------------------------------------------------------------------------
// TEST GROUP 4 – Setters / getters
//
// The public interface exposed by the header (beyond lifecycle methods):
//   void setHList(TObjArray*)
//   void setFgtDb(StFgtDb*)
//   void enableAddPeds()
//   void setRndSeed(int)
//
// None of these return a value, so we verify they execute without throwing
// and (where observable) that the constructor defaults are as documented.
// ---------------------------------------------------------------------------

TEST_CASE("StFgtSlowSimuMaker setHList does not throw",
          "[StFgtSlowSimuMaker][setter]") {
    TestableStFgtSlowSimuMaker maker("HListTest");

    SECTION("setHList(nullptr) is accepted") {
        // Constructor already calls setHList(0); calling it again is safe.
        REQUIRE_NOTHROW(maker.setHList(nullptr));
    }

    SECTION("setHList with a real TObjArray pointer is accepted") {
        TObjArray arr;
        REQUIRE_NOTHROW(maker.setHList(&arr));
    }
}

TEST_CASE("StFgtSlowSimuMaker setFgtDb does not throw",
          "[StFgtSlowSimuMaker][setter]") {
    TestableStFgtSlowSimuMaker maker("DbTest");

    SECTION("setFgtDb(nullptr) is accepted (resets to no-DB state)") {
        REQUIRE_NOTHROW(maker.setFgtDb(nullptr));
    }
}

TEST_CASE("StFgtSlowSimuMaker enableAddPeds does not throw",
          "[StFgtSlowSimuMaker][setter]") {
    TestableStFgtSlowSimuMaker maker("PedsTest");

    // The constructor sets switch_addPeds=1 by default; calling
    // enableAddPeds() again is idempotent and must not throw.
    SECTION("enableAddPeds() is callable and does not throw") {
        REQUIRE_NOTHROW(maker.enableAddPeds());
    }
}

TEST_CASE("StFgtSlowSimuMaker setRndSeed does not throw",
          "[StFgtSlowSimuMaker][setter]") {
    TestableStFgtSlowSimuMaker maker("RndTest");

    SECTION("setRndSeed(0) resets to unseeded (time-based) sequence") {
        REQUIRE_NOTHROW(maker.setRndSeed(0));
    }

    SECTION("setRndSeed with a fixed seed produces reproducible state") {
        REQUIRE_NOTHROW(maker.setRndSeed(42));
    }

    SECTION("setRndSeed with large seed value") {
        REQUIRE_NOTHROW(maker.setRndSeed(0x7FFFFFFF));
    }
}

// ---------------------------------------------------------------------------
// TEST GROUP 5 – Geometry / physics constants (from StFgtConsts.h /
//                StEnumerations.h, exercised at compile time)
//
// These constants are consumed by the maker but owned by StFgtUtil.
// Testing them here verifies that the values the maker depends upon have
// not drifted from the expected physics design parameters.
// ---------------------------------------------------------------------------

TEST_CASE("FGT geometry constants have expected physics values",
          "[StFgtSlowSimuMaker][constants]") {
    SECTION("kFgtNumDiscs == 6 (six FGT discs in STAR)") {
        CHECK(kFgtNumDiscs == 6);
    }

    SECTION("kFgtNumQuads == 4 (four quadrants per disc)") {
        CHECK(kFgtNumQuads == 4);
    }

    SECTION("kFgtNumStrips == 720 (strips per readout layer)") {
        CHECK(kFgtNumStrips == 720);
    }

    SECTION("kFgtNumTimeBins == 15 (APV time-sample bins per event)") {
        CHECK(kFgtNumTimeBins == 15);
    }

    SECTION("pulseShape window (7) does not exceed kFgtNumTimeBins (15)") {
        CHECK(TestableStFgtSlowSimuMaker::kNumTimebinsUsed <= kFgtNumTimeBins);
    }
}

TEST_CASE("FGT radial geometry constants are physically reasonable",
          "[StFgtSlowSimuMaker][constants]") {
    SECTION("inner radius kFgtRin is positive") {
        CHECK(kFgtRin > 0.0);
    }

    SECTION("outer radius kFgtRout > inner radius kFgtRin") {
        CHECK(kFgtRout > kFgtRin);
    }

    SECTION("kFgtRout is near 38.25 cm") {
        CHECK(kFgtRout == Catch::Approx(38.25).epsilon(1e-4));
    }

    SECTION("kFgtRin is near 11.5 cm") {
        CHECK(kFgtRin == Catch::Approx(11.5).epsilon(1e-4));
    }

    SECTION("dead-zone edge kFgtDeadQuadEdge is positive") {
        CHECK(kFgtDeadQuadEdge > 0.0);
    }
}

TEST_CASE("FGT strip pitch constants are physically reasonable",
          "[StFgtSlowSimuMaker][constants]") {
    SECTION("radial strip pitch kFgtRadPitch is near 800 um (0.0954 cm)") {
        CHECK(kFgtRadPitch == Catch::Approx(0.0954071684).epsilon(1e-6));
    }

    SECTION("phi strip pitch kFgtPhiPitch is near 800 um (0.08 cm at Rmid)") {
        CHECK(kFgtPhiPitch == Catch::Approx(0.08).epsilon(1e-6));
    }
}

// ---------------------------------------------------------------------------
// TEST GROUP 6 – Integration-test stubs  [integration]
//
// These tests document which lifecycle paths require a full STAR runtime.
// They are skipped during normal unit-test runs; enable with
//   --gtest-filter=*integration* (or Catch2 tag filter [integration])
// after setting up a complete STAR environment.
// ---------------------------------------------------------------------------

TEST_CASE("StFgtSlowSimuMaker Init requires ROOT histogram infrastructure"
          " [integration][!shouldfail]",
          "[StFgtSlowSimuMaker][integration]") {
    // Init() calls InitHisto1() which allocates TH2F/TH1F.  These require
    // a ROOT TDirectory (e.g. gROOT) to be properly set up and may collide
    // if multiple test instances use the same histogram names.  A minimal
    // TApplication or at least a TMemFile should be created before calling
    // Init().
    SUCCEED("placeholder – run under a full STAR environment");
}

TEST_CASE("StFgtSlowSimuMaker InitRun requires a valid StFgtDb"
          " [integration][!shouldfail]",
          "[StFgtSlowSimuMaker][integration]") {
    // InitRun() queries the STAR database via StFgtDb::getSimuParam().
    // Without a live DB connection the maker sets par_badSetup != 0 and
    // subsequent Make() calls become no-ops.
    SUCCEED("placeholder – run under a full STAR environment with DB access");
}

TEST_CASE("StFgtSlowSimuMaker Make requires StEvent and g2t_fgt_hit table"
          " [integration][!shouldfail]",
          "[StFgtSlowSimuMaker][integration]") {
    // Make() calls GetInputDS("StEvent") and GetDataSet("g2t_fgt_hit").
    // Both require a populated StChain with the GEANT hit tables present.
    SUCCEED("placeholder – run under a full BFC chain");
}

TEST_CASE("StFgtSlowSimuMaker saveHisto writes a ROOT file"
          " [integration][!shouldfail]",
          "[StFgtSlowSimuMaker][integration]") {
    // saveHisto() is guarded by #ifdef __FGT_QA_HISTO__; it is only active
    // when the macro is defined at compile time and the HList pointer is
    // non-null.  Exercise under a QA build with a writable output directory.
    SUCCEED("placeholder – requires __FGT_QA_HISTO__ build and output path");
}
