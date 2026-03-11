// tests/unit/StSsdFastSimMaker/test_StSsdFastSimMaker.cxx
//
// Unit tests for StSsdFastSimMaker — the Silicon Strip Detector fast simulator.
//
// The maker inherits from StMaker (TDataSet/TObject).  Tests that depend on
// the STAR runtime (database access, g2t tables, StEvent chain) are tagged
// [integration] and kept as commented stubs.  The structural and geometry
// tests below work without a TApplication.
//
// Geometry helpers under test
// ───────────────────────────
//   idWaferToWaferNumb(idWafer)
//     idWafer = layer*1000 + waf*100 + ladder  (layer=7 always)
//     waferNumb = (ladder-1)*mNWaferPerLadder + waf - 1
//
//   idWaferToLadderNumb(idWafer)
//     returns  ladder - 1  (0-based)
//
//   waferNumbToIdWafer(waferNumb)
//     Intended inverse of idWaferToWaferNumb, but uses mNLadder (=20)
//     as the divisor instead of mNWaferPerLadder (=16).  See NOTE below.
//
//   IsOnWafer(x, y)
//     Returns 1 when  |x| < mDetectorLargeEdge/2  AND
//                     |y| < mDetectorSmallEdge/2
//
//   RemoveTriangle(x, y)
//     Cuts the trapezoidal corners of the wafer.  Due to the placement of
//     the else-branch that sets iok=1, the function returns 1 for any
//     x >= -l  (l ≈ mDetectorLargeEdge/2 - X) and 0 for x < -l.
//     The right-corner rejection logic is effectively overridden.
//
// NOTE on waferNumbToIdWafer round-trip
// ──────────────────────────────────────
//   idWaferToWaferNumb encodes  waferNumb = (ladder-1)*mNWaferPerLadder + waf - 1
//   but waferNumbToIdWafer decodes using  mNLadder (=20) as the block size,
//   not mNWaferPerLadder (=16).  The two functions are mutual inverses only
//   for the range waferNumb ∈ [0, mNLadder) where the quotient (waferNumb/20)
//   happens to equal the waf-1 encoded by idWaferToWaferNumb.  For waferNumb
//   ≥ mNLadder the round-trip breaks.  The failing case is documented in the
//   integration stubs below.

// ── ssdDimensions_st ────────────────────────────────────────────────────────
// StSsdFastSimMaker.h only forward-declares 'class ssdDimensions_st'.
// The actual struct is generated from StDb/idl/ssdDimensions.idl by the STAR
// stic IDL compiler (long → int, double → double).  We provide the
// layout-compatible definition here so setSsdParameters() can be called in
// standalone tests without a database connection.
struct ssdDimensions_st {
    int    wafersPerLadder;     // IDL: long  (16)
    int    a128PerSide;         // IDL: long  (6)
    int    stripPerSide;        // IDL: long  (768)
    double stripPitch;          // IDL: double (0.0095 cm)
    double stereoAngle;         // IDL: double (0.0175 rad)
    double waferHalfLength;     // IDL: double (3.75 cm)
    double waferHalfWidth;      // IDL: double (2.1 cm)
    double waferHalfThickness;  // IDL: double (0.015 cm)
    double waferHalfActLength;  // IDL: double (3.65 cm)
    double waferHalfActWidth;   // IDL: double (2.0 cm)
};

#include "../catch_amalgamated.hpp"
#include "StSsdFastSimMaker/StSsdFastSimMaker.h"

#include <string>
#include <cmath>

// ── Nominal geometry constants (from ssdDimensions.idl comments) ─────────────

static constexpr int    kNWaferPerLadder   = 16;
static constexpr int    kNLadder           = 20;
static constexpr int    kSsdLayer          = 7;
static constexpr double kWaferHalfActLen   = 3.65;    // cm
static constexpr double kWaferHalfActWidth = 2.0;     // cm
static constexpr double kStereoAngle       = 0.0175;  // rad

// Active area full widths set by setSsdParameters():
//   mDetectorLargeEdge = 2 * waferHalfActLength = 7.30 cm
//   mDetectorSmallEdge = 2 * waferHalfActWidth  = 4.00 cm
static constexpr double kLargeEdge = 2.0 * kWaferHalfActLen;   // 7.30 cm
static constexpr double kSmallEdge = 2.0 * kWaferHalfActWidth; // 4.00 cm
static constexpr double kHalfLarge = kLargeEdge / 2.0;         // 3.65 cm
static constexpr double kHalfSmall = kSmallEdge / 2.0;         // 2.00 cm

// Trapezoid cut boundary (used by RemoveTriangle):
//   X = (kSmallEdge/2) * tan(kStereoAngle/2) ≈ 0.01750 cm
//   l = kHalfLarge - X                        ≈ 3.6325 cm
//   For x >= -l the function returns 1; for x < -l it returns 0.
static const double kX_cut = kHalfSmall * std::tan(kStereoAngle / 2.0);
static const double kL_cut = kHalfLarge - kX_cut;  // ≈ 3.6325 cm

// ── Test fixture ─────────────────────────────────────────────────────────────
// Constructs a maker and calls setSsdParameters() with the nominal SSD
// geometry so the pure-math helpers can be exercised without a STAR chain.
class SsdFastSimFixture : public StSsdFastSimMaker {
public:
    explicit SsdFastSimFixture(const char* name = "SsdFastSimTest")
        : StSsdFastSimMaker(name)
    {
        mDims.wafersPerLadder    = kNWaferPerLadder;
        mDims.a128PerSide        = 6;
        mDims.stripPerSide       = 768;
        mDims.stripPitch         = 0.0095;
        mDims.stereoAngle        = kStereoAngle;
        mDims.waferHalfLength    = 3.75;
        mDims.waferHalfWidth     = 2.1;
        mDims.waferHalfThickness = 0.015;
        mDims.waferHalfActLength = kWaferHalfActLen;
        mDims.waferHalfActWidth  = kWaferHalfActWidth;
        setSsdParameters(&mDims);
    }
private:
    ssdDimensions_st mDims;
};

// ─────────────────────────────────────────────────────────────────────────────
// Construction and naming
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StSsdFastSimMaker construction with explicit name",
          "[StSsdFastSimMaker]") {
    StSsdFastSimMaker maker("mySsdSim");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "mySsdSim");
    }
}

TEST_CASE("StSsdFastSimMaker default construction",
          "[StSsdFastSimMaker]") {
    StSsdFastSimMaker maker;

    SECTION("GetName is non-null") {
        REQUIRE(maker.GetName() != nullptr);
    }

    SECTION("default name is 'SsdFastSim'") {
        // Default argument in ctor: StSsdFastSimMaker(const char *name="SsdFastSim")
        CHECK(std::string(maker.GetName()) == "SsdFastSim");
    }
}

TEST_CASE("StSsdFastSimMaker GetCVS returns non-empty string",
          "[StSsdFastSimMaker]") {
    StSsdFastSimMaker maker;

    SECTION("GetCVS is non-null") {
        REQUIRE(maker.GetCVS() != nullptr);
    }

    SECTION("GetCVS contains the file identifier") {
        std::string cvs(maker.GetCVS());
        CHECK_FALSE(cvs.empty());
        CHECK(cvs.find("StSsdFastSimMaker") != std::string::npos);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// setSsdParameters — callable without a STAR runtime
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StSsdFastSimMaker setSsdParameters does not throw",
          "[StSsdFastSimMaker][geometry]") {
    StSsdFastSimMaker maker;

    SECTION("nominal geometry parameters are accepted") {
        ssdDimensions_st dims{};
        dims.wafersPerLadder    = kNWaferPerLadder;
        dims.a128PerSide        = 6;
        dims.stripPerSide       = 768;
        dims.stripPitch         = 0.0095;
        dims.stereoAngle        = kStereoAngle;
        dims.waferHalfLength    = 3.75;
        dims.waferHalfWidth     = 2.1;
        dims.waferHalfThickness = 0.015;
        dims.waferHalfActLength = kWaferHalfActLen;
        dims.waferHalfActWidth  = kWaferHalfActWidth;
        REQUIRE_NOTHROW(maker.setSsdParameters(&dims));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// idWaferToWaferNumb
// ─────────────────────────────────────────────────────────────────────────────
//
// Encoding: idWafer = layer*1000 + waf*100 + ladder
//   iW (waf)    = (idWafer - 7000) / 100    (1-based, integer division)
//   iL (ladder) = idWafer - 7000 - iW*100   (1-based)
//   waferNumb   = (iL-1)*mNWaferPerLadder + iW - 1
//               = (ladder-1)*16 + waf - 1

TEST_CASE("idWaferToWaferNumb with first wafer of first ladder",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // layer=7, waf=1, ladder=1 → 7101
    REQUIRE(maker.idWaferToWaferNumb(7101) == 0);
}

TEST_CASE("idWaferToWaferNumb with second wafer of first ladder",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // layer=7, waf=2, ladder=1 → 7201;  (1-1)*16 + 2-1 = 1
    REQUIRE(maker.idWaferToWaferNumb(7201) == 1);
}

TEST_CASE("idWaferToWaferNumb with first wafer of second ladder",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // layer=7, waf=1, ladder=2 → 7102;  (2-1)*16 + 1-1 = 16
    REQUIRE(maker.idWaferToWaferNumb(7102) == 16);
}

TEST_CASE("idWaferToWaferNumb with second wafer of second ladder",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // layer=7, waf=2, ladder=2 → 7202;  (2-1)*16 + 2-1 = 17
    REQUIRE(maker.idWaferToWaferNumb(7202) == 17);
}

TEST_CASE("idWaferToWaferNumb with first wafer of tenth ladder",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // layer=7, waf=1, ladder=10 → 7110;  (10-1)*16 + 0 = 144
    REQUIRE(maker.idWaferToWaferNumb(7110) == 144);
}

TEST_CASE("idWaferToWaferNumb with last wafer of last ladder",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // layer=7, waf=16, ladder=20 → 7000+1600+20=8620
    // (20-1)*16 + 16-1 = 304+15 = 319
    REQUIRE(maker.idWaferToWaferNumb(8620) == 319);
}

TEST_CASE("idWaferToWaferNumb: wafer index grows faster than ladder index",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // Incrementing waf by 1 advances waferNumb by 1.
    SECTION("waf=1→2, ladder fixed at 1") {
        CHECK(maker.idWaferToWaferNumb(7201) ==
              maker.idWaferToWaferNumb(7101) + 1);
    }
    // Incrementing ladder by 1 advances waferNumb by mNWaferPerLadder=16.
    SECTION("ladder=1→2, waf fixed at 1") {
        CHECK(maker.idWaferToWaferNumb(7102) ==
              maker.idWaferToWaferNumb(7101) + kNWaferPerLadder);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// idWaferToLadderNumb
// ─────────────────────────────────────────────────────────────────────────────
//
// Returns  ladder - 1  (0-based).

TEST_CASE("idWaferToLadderNumb with first ladder",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // Any wafer on ladder 1 → 0.
    SECTION("waf=1 ladder=1") { CHECK(maker.idWaferToLadderNumb(7101) == 0); }
    SECTION("waf=8 ladder=1") { CHECK(maker.idWaferToLadderNumb(7801) == 0); }
}

TEST_CASE("idWaferToLadderNumb with second ladder",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // Any wafer on ladder 2 → 1.
    SECTION("waf=1 ladder=2") { CHECK(maker.idWaferToLadderNumb(7102) == 1); }
    SECTION("waf=8 ladder=2") { CHECK(maker.idWaferToLadderNumb(7802) == 1); }
}

TEST_CASE("idWaferToLadderNumb with last ladder",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // ladder=20 → 19.
    REQUIRE(maker.idWaferToLadderNumb(8620) == 19);
}

TEST_CASE("idWaferToLadderNumb is independent of waf index",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // Different waf values, same ladder → same result.
    const int ladder5_waf1  = 7105;   // waf=1, ladder=5
    const int ladder5_waf16 = 8605;   // waf=16, ladder=5
    REQUIRE(maker.idWaferToLadderNumb(ladder5_waf1)  == 4);
    REQUIRE(maker.idWaferToLadderNumb(ladder5_waf16) == 4);
}

// ─────────────────────────────────────────────────────────────────────────────
// idWaferToWafer (inline header helper)
// ─────────────────────────────────────────────────────────────────────────────
//
// Returns  (idWafer - 7000)/100 - 1  — the 0-based waf-within-ladder index.
// Used in Make() as the mWafers[] array subscript.

TEST_CASE("idWaferToWafer inline helper",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    SECTION("waf=1 → 0-based index 0") { CHECK(maker.idWaferToWafer(7101) == 0); }
    SECTION("waf=2 → 0-based index 1") { CHECK(maker.idWaferToWafer(7201) == 1); }
    SECTION("waf=16 → 0-based index 15") { CHECK(maker.idWaferToWafer(8620) == 15); }
    SECTION("result is independent of ladder") {
        CHECK(maker.idWaferToWafer(7101) == maker.idWaferToWafer(7102));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// waferNumbToIdWafer — known values
// ─────────────────────────────────────────────────────────────────────────────
//
// Implementation:
//   iL = 1 + (int)(waferNumb / mNLadder)     mNLadder=20
//   iW = waferNumb - (iL-1)*mNLadder + 1
//   return mSsdLayer*1000 + iW*100 + iL
//
// NOTE: this function divides by mNLadder (=20), not mNWaferPerLadder (=16).
// It is the inverse of idWaferToWaferNumb only when waferNumb < mNLadder (=20).

TEST_CASE("waferNumbToIdWafer for waferNumb=0",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // iL=1, iW=1 → 7000+100+1=7101
    REQUIRE(maker.waferNumbToIdWafer(0) == 7101);
}

TEST_CASE("waferNumbToIdWafer for waferNumb=1",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // iL=1, iW=2 → 7000+200+1=7201
    REQUIRE(maker.waferNumbToIdWafer(1) == 7201);
}

TEST_CASE("waferNumbToIdWafer for waferNumb=19",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // iL=1, iW=20 → 7000+2000+1=9001
    REQUIRE(maker.waferNumbToIdWafer(19) == 9001);
}

TEST_CASE("waferNumbToIdWafer for waferNumb=20",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // iL=1+(20/20)=2, iW=20-(1)*20+1=1 → 7000+100+2=7102
    REQUIRE(maker.waferNumbToIdWafer(20) == 7102);
}

// ─────────────────────────────────────────────────────────────────────────────
// Round-trip: waferNumbToIdWafer / idWaferToWaferNumb
// ─────────────────────────────────────────────────────────────────────────────
//
// The round-trip  idWaferToWaferNumb(waferNumbToIdWafer(n)) == n  holds only
// for n ∈ [0, mNLadder) = [0, 20) because that is the range where
//   1 + n/mNLadder == 1  (iL stays 1) and the two divisors (20 vs 16)
// produce the same quotient.
//
// The inverse round-trip waferNumbToIdWafer(idWaferToWaferNumb(id)) == id
// holds only for ladder=1 IDs (waferNumb = waf-1 < 16 < 20).

TEST_CASE("waferNumbToIdWafer round-trip for n in [0, mNLadder)",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;

    // For n < 20 the round-trip idWaferToWaferNumb(waferNumbToIdWafer(n))==n holds.
    SECTION("n=0")  { REQUIRE(maker.idWaferToWaferNumb(maker.waferNumbToIdWafer(0))  == 0);  }
    SECTION("n=1")  { REQUIRE(maker.idWaferToWaferNumb(maker.waferNumbToIdWafer(1))  == 1);  }
    SECTION("n=10") { REQUIRE(maker.idWaferToWaferNumb(maker.waferNumbToIdWafer(10)) == 10); }
    SECTION("n=19") { REQUIRE(maker.idWaferToWaferNumb(maker.waferNumbToIdWafer(19)) == 19); }
}

TEST_CASE("waferNumbToIdWafer inverse round-trip for ladder-1 IDs",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;

    // For IDs with ladder=1 (waferNumb = waf-1 ∈ [0,15] < 20) the inverse
    // round-trip waferNumbToIdWafer(idWaferToWaferNumb(id)) == id holds.
    SECTION("7101 (waf=1, ladder=1)") {
        REQUIRE(maker.waferNumbToIdWafer(maker.idWaferToWaferNumb(7101)) == 7101);
    }
    SECTION("7801 (waf=8, ladder=1)") {
        REQUIRE(maker.waferNumbToIdWafer(maker.idWaferToWaferNumb(7801)) == 7801);
    }
    SECTION("8501 (waf=15, ladder=1)") {
        REQUIRE(maker.waferNumbToIdWafer(maker.idWaferToWaferNumb(8501)) == 8501);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// IsOnWafer
// ─────────────────────────────────────────────────────────────────────────────
//
// Active area: |x| < mDetectorLargeEdge/2 = 3.65 cm  (strict)
//              |y| < mDetectorSmallEdge/2 = 2.00 cm  (strict)
//
// In Make(), called as  IsOnWafer(xlSmear[0], xlSmear[2])  — local x and z.

TEST_CASE("IsOnWafer returns 1 for the wafer centre",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    REQUIRE(maker.IsOnWafer(0.0f, 0.0f) == 1);
}

TEST_CASE("IsOnWafer returns 1 strictly inside all edges",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // Just inside both edges.
    REQUIRE(maker.IsOnWafer( 3.64f,  1.99f) == 1);
    REQUIRE(maker.IsOnWafer(-3.64f, -1.99f) == 1);
}

TEST_CASE("IsOnWafer returns 0 beyond the large (long) edge",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // |x| = 3.66 > kHalfLarge = 3.65
    REQUIRE(maker.IsOnWafer( 3.66f, 0.0f) == 0);
    REQUIRE(maker.IsOnWafer(-3.66f, 0.0f) == 0);
}

TEST_CASE("IsOnWafer returns 0 beyond the small (short) edge",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // |y| = 2.01 > kHalfSmall = 2.00
    REQUIRE(maker.IsOnWafer(0.0f,  2.01f) == 0);
    REQUIRE(maker.IsOnWafer(0.0f, -2.01f) == 0);
}

TEST_CASE("IsOnWafer returns 0 exactly on the large edge (strict inequality)",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // x == ±3.65: condition is strict <, so this is outside.
    REQUIRE(maker.IsOnWafer( static_cast<float>(kHalfLarge), 0.0f) == 0);
    REQUIRE(maker.IsOnWafer(-static_cast<float>(kHalfLarge), 0.0f) == 0);
}

TEST_CASE("IsOnWafer returns 0 exactly on the small edge (strict inequality)",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // y == ±2.00: condition is strict <, so this is outside.
    REQUIRE(maker.IsOnWafer(0.0f,  static_cast<float>(kHalfSmall)) == 0);
    REQUIRE(maker.IsOnWafer(0.0f, -static_cast<float>(kHalfSmall)) == 0);
}

TEST_CASE("IsOnWafer large-edge check is independent of y",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // A point beyond the large edge is outside regardless of y.
    REQUIRE(maker.IsOnWafer(4.0f, 0.5f) == 0);
}

TEST_CASE("IsOnWafer small-edge check is independent of x",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // A point beyond the small edge is outside regardless of x.
    REQUIRE(maker.IsOnWafer(1.0f, 2.5f) == 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// RemoveTriangle
// ─────────────────────────────────────────────────────────────────────────────
//
// Nominal cut parameters (see kX_cut / kL_cut above):
//   mTheta = 0.0175 rad
//   X      = (mDetectorSmallEdge/2) * tan(mTheta/2) ≈ 0.01750 cm
//   l      = mDetectorLargeEdge/2 - X               ≈ 3.6325  cm
//
// Actual behaviour due to code structure:
//   x >= -l  →  iok = 1  (else-branch always sets iok=1)
//   x <  -l  →  iok = 0  (right-corner rejection is dead; left region always 0)
//
// NOTE: the right-corner triangle cuts (x > l) set iok=0 in the first block,
// but are unconditionally overridden to iok=1 by the else of the left-corner
// block when x >= -l.  All hits with x < -l ≈ -3.6325 are rejected.

TEST_CASE("RemoveTriangle returns 1 for the wafer centre",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    REQUIRE(maker.RemoveTriangle(0.0f, 0.0f) == 1);
}

TEST_CASE("RemoveTriangle returns 1 for x well within both sides",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    REQUIRE(maker.RemoveTriangle( 3.0f, 0.0f) == 1);
    REQUIRE(maker.RemoveTriangle(-3.0f, 0.0f) == 1);
}

TEST_CASE("RemoveTriangle returns 1 for x just inside -l from the right",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // x = -3.63 > -l ≈ -3.6325  →  else-branch fires → iok = 1
    REQUIRE(maker.RemoveTriangle(-3.63f, 0.0f) == 1);
}

TEST_CASE("RemoveTriangle returns 1 even deep in the right corner region",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // x = 3.64 > l ≈ 3.6325, but x > -l also → else-branch overrides to 1.
    REQUIRE(maker.RemoveTriangle(3.64f, 0.0f) == 1);
}

TEST_CASE("RemoveTriangle returns 0 for x just beyond -l on the left",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // x = -3.64 < -l ≈ -3.6325  →  left-corner block entered; iok never set to 1.
    REQUIRE(maker.RemoveTriangle(-3.64f, 0.0f) == 0);
}

TEST_CASE("RemoveTriangle returns 0 for any hit in the x < -l strip",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // All y values in the leftmost strip are rejected.
    REQUIRE(maker.RemoveTriangle(-3.64f,  1.5f) == 0);
    REQUIRE(maker.RemoveTriangle(-3.64f, -1.5f) == 0);
    REQUIRE(maker.RemoveTriangle(-3.64f,  0.0f) == 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// Combined IsOnWafer + RemoveTriangle (mirrors the Make() call sequence)
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("IsOnWafer then RemoveTriangle: interior hit passes both cuts",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    const float x = 1.0f, y = 0.5f;
    REQUIRE(maker.IsOnWafer(x, y)      == 1);
    REQUIRE(maker.RemoveTriangle(x, y) == 1);
}

TEST_CASE("IsOnWafer then RemoveTriangle: hit outside active area fails IsOnWafer",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // Outside large edge — fails IsOnWafer before RemoveTriangle is checked.
    REQUIRE(maker.IsOnWafer(4.0f, 0.0f) == 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// SSD geometry constants
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("SSD nominal active-area dimensions match IDL defaults",
          "[StSsdFastSimMaker][geometry]") {
    SECTION("large edge (full length along strips) is 7.30 cm") {
        CHECK(kLargeEdge == Catch::Approx(7.30).epsilon(1e-9));
    }
    SECTION("small edge (full width across strips) is 4.00 cm") {
        CHECK(kSmallEdge == Catch::Approx(4.00).epsilon(1e-9));
    }
    SECTION("stereo angle is 0.0175 rad") {
        CHECK(kStereoAngle == Catch::Approx(0.0175).epsilon(1e-9));
    }
    SECTION("nominal wafers-per-ladder is 16") {
        CHECK(kNWaferPerLadder == 16);
    }
    SECTION("nominal ladder count is 20") {
        CHECK(kNLadder == 20);
    }
    SECTION("total wafer count is 320") {
        CHECK(kNLadder * kNWaferPerLadder == 320);
    }
}

TEST_CASE("SSD layer number is always 7",
          "[StSsdFastSimMaker][geometry]") {
    // setSsdParameters() hard-codes mSsdLayer = 7.
    SECTION("layer constant matches encoding base") {
        CHECK(kSsdLayer == 7);
    }
    // The minimum valid idWafer (waf=1, ladder=1) starts at layer*1000+101.
    SECTION("minimum idWafer for SSD layer") {
        constexpr int kMinId = kSsdLayer * 1000 + 1 * 100 + 1;  // 7101
        CHECK(kMinId == 7101);
    }
    // The maximum idWafer (waf=16, ladder=20) ends at 7000+1600+20=8620.
    SECTION("maximum idWafer for 20 ladders, 16 wafers per ladder") {
        constexpr int kMaxId = kSsdLayer * 1000
                               + kNWaferPerLadder * 100
                               + kNLadder;  // 8620
        CHECK(kMaxId == 8620);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Hardware-position encoding used in Make()
// ─────────────────────────────────────────────────────────────────────────────
//
//   hw = 8 + 16 * idWaferToWaferNumb(currWafId)
//
// The detector ID occupies bits 0-2 (value 8 = SSD detector ID).
// waferNumb occupies bits 3-11 (shifted by 16 = 2^4).

TEST_CASE("hardware position encoding for first SSD wafer",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // waf=1, ladder=1 → waferNumb=0 → hw = 8 + 16*0 = 8
    const int hw = 8 + 16 * maker.idWaferToWaferNumb(7101);
    REQUIRE(hw == 8);
}

TEST_CASE("hardware position encoding for second SSD wafer",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // waf=2, ladder=1 → waferNumb=1 → hw = 8 + 16 = 24
    const int hw = 8 + 16 * maker.idWaferToWaferNumb(7201);
    REQUIRE(hw == 24);
}

TEST_CASE("hardware position encoding for last SSD wafer",
          "[StSsdFastSimMaker][geometry]") {
    SsdFastSimFixture maker;
    // waf=16, ladder=20 → waferNumb=319 → hw = 8 + 16*319 = 5112
    const int hw = 8 + 16 * maker.idWaferToWaferNumb(8620);
    REQUIRE(hw == 8 + 16 * 319);
    REQUIRE(hw == 5112);
}

// ─────────────────────────────────────────────────────────────────────────────
// Integration-test stubs
// These require a TApplication, StChain, database tables, and MC hit tables.
// ─────────────────────────────────────────────────────────────────────────────

// TEST_CASE("StSsdFastSimMaker Init() returns kStOk with database access",
//           "[StSsdFastSimMaker][integration]") {
//     // Requires: StChain, St_db_Maker, Geometry/ssd, Calibrations/tracker.
//     // StChain chain;
//     // StSsdFastSimMaker* maker = new StSsdFastSimMaker("SsdFastSim");
//     // chain.AddMaker(maker);
//     // REQUIRE(chain.Init() == kStOk);
// }

// TEST_CASE("StSsdFastSimMaker Make() returns kStWarn without StEvent",
//           "[StSsdFastSimMaker][integration]") {
//     // Make() returns kStWarn when GetInputDS("StEvent") returns nullptr.
//     // StChain chain;
//     // StSsdFastSimMaker* maker = new StSsdFastSimMaker;
//     // chain.AddMaker(maker);
//     // chain.Init();
//     // REQUIRE(maker->Make() == kStWarn);
// }

// TEST_CASE("StSsdFastSimMaker Make() returns kStWarn without g2t_ssd_hit",
//           "[StSsdFastSimMaker][integration]") {
//     // Make() returns kStWarn when g2t_ssd_hit table is absent.
// }

// TEST_CASE("StSsdFastSimMaker populates StSsdHitCollection from g2t hits",
//           "[StSsdFastSimMaker][integration]") {
//     // Inject synthetic g2t_ssd_hit_st rows; verify collection hit count.
// }

// TEST_CASE("distortHit returns input unchanged when mSmear=0",
//           "[StSsdFastSimMaker][integration]") {
//     // Requires Init() to initialise myRandom and mSmear.
//     // After calling Init(), set mSmear=0 via a dedicated setter (add one),
//     // then verify distortHit(x, res) == x for any x and res.
// }

// TEST_CASE("waferNumbToIdWafer round-trip breaks for waferNumb >= mNLadder",
//           "[StSsdFastSimMaker][integration]") {
//     // This documents the known discrepancy: waferNumbToIdWafer uses mNLadder
//     // (=20) as its block divisor instead of mNWaferPerLadder (=16), so the
//     // round-trip idWaferToWaferNumb(waferNumbToIdWafer(20)) returns 16, not 20.
//     // SsdFastSimFixture maker;
//     // CHECK(maker.idWaferToWaferNumb(maker.waferNumbToIdWafer(20)) != 20); // BUG
//     // Correct implementation would use mNWaferPerLadder as divisor.
// }
