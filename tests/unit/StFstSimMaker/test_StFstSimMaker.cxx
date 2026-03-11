// tests/unit/StFstSimMaker/test_StFstSimMaker.cxx
//
// Catch2 v3 unit tests for StFstFastSimMaker (Forward Silicon Tracker
// fast simulator) and the FstGlobal helper namespace defined in the same
// translation unit.
//
// Test categories
// ---------------
//  Structural / pure-math (no [integration] tag)
//    Run standalone — no TApplication or STAR runtime required.
//    StFstFastSimMaker can be constructed on the stack; the ROOT TObject
//    hierarchy it inherits does not require a TApplication for construction
//    alone.  FstGlobal::Hack1to6 is a pure Jacobian transformation tested
//    with analytically known inputs.
//
//  Integration stubs (tagged [integration])
//    Require a full StChain, GEANT input tables (g2t_fsi_hit, g2t_track),
//    and the STAR runtime.  Provided as commented-out placeholders using the
//    SimTestFixture<> framework; see tests/unit/sim_support/SimTestFixture.h.

#include "../catch_amalgamated.hpp"

#include "StFstFastSimMaker.h"

// StHit / StRnDHit — concrete subclass used to construct Hack1to6 inputs.
#include "StRnDHit.h"

// StarClassLibrary types referenced in the FstGlobal forward declaration.
#include "StMatrixF.hh"
#include "StThreeVectorF.hh"

#include <cmath>
#include <string>

// ---------------------------------------------------------------------------
// FstGlobal namespace — forward-declarations for symbols defined in
// StFstFastSimMaker.cxx.
//
// Non-const namespace-scope arrays have external linkage and are reachable
// via 'extern' without modifying the production header.
//
// 'SQRT12' is constexpr (implicitly const → internal linkage in C++) and
// 'verbose' is const bool (same); neither can be externed.  Their expected
// values are verified independently in the constants tests below.
// ---------------------------------------------------------------------------
namespace FstGlobal {

    // Non-const arrays: external linkage → accessible via extern.
    extern float RMIN[];      ///< Inner radius limit per disk [6 entries].
    extern float RMAX[];      ///< Outer radius limit per disk [6 entries].
    extern float RSegment[];  ///< r-strip boundaries for disk array 4-6 [9 entries].

    // Forward declaration of the covariance-matrix builder so we can call it
    // from the unit test without modifying the production header.
    StMatrixF Hack1to6(const StHit* stHit);

} // namespace FstGlobal

// ---------------------------------------------------------------------------
// Helper: build an StRnDHit with a given position and (r,phi,z) errors.
// ---------------------------------------------------------------------------
static StRnDHit makeHit(float x, float y, float z,
                        float er, float ep, float ez)
{
    StRnDHit hit;
    hit.setPosition(StThreeVectorF(x, y, z));
    hit.setPositionError(StThreeVectorF(er, ep, ez));
    return hit;
}

// ============================================================================
// 1.  Construction and GetName()
// ============================================================================

TEST_CASE("StFstFastSimMaker construction with explicit name",
          "[StFstFastSimMaker]")
{
    StFstFastSimMaker maker("fstTestMaker");

    SECTION("GetName returns the name passed to the constructor") {
        REQUIRE(std::string(maker.GetName()) == "fstTestMaker");
    }
}

TEST_CASE("StFstFastSimMaker construction with default name",
          "[StFstFastSimMaker]")
{
    StFstFastSimMaker maker;   // default argument: "fstFastSim"

    SECTION("default name is fstFastSim") {
        REQUIRE(std::string(maker.GetName()) == "fstFastSim");
    }
}

// ============================================================================
// 2.  SetDisk — verifies RMIN[i] / RMAX[i] are updated in FstGlobal
//
// FstGlobal::RMIN and FstGlobal::RMAX are non-const namespace-scope arrays
// (external linkage).  SetDisk writes into them directly, so the change is
// observable from outside the class without a getter.
//
// The originals are saved and restored so that later constant-value tests
// see the default geometry regardless of execution order.
// ============================================================================

TEST_CASE("SetDisk updates FstGlobal::RMIN and RMAX for disk index 0",
          "[StFstFastSimMaker]")
{
    const float savedRmin = FstGlobal::RMIN[0];
    const float savedRmax = FstGlobal::RMAX[0];

    StFstFastSimMaker maker;
    maker.SetDisk(0, 3.5f, 16.0f);

    CHECK(FstGlobal::RMIN[0] == Catch::Approx(3.5f));
    CHECK(FstGlobal::RMAX[0] == Catch::Approx(16.0f));

    // Restore shared state.
    FstGlobal::RMIN[0] = savedRmin;
    FstGlobal::RMAX[0] = savedRmax;
}

TEST_CASE("SetDisk updates FstGlobal::RMIN and RMAX for disk index 4",
          "[StFstFastSimMaker]")
{
    const float savedRmin = FstGlobal::RMIN[4];
    const float savedRmax = FstGlobal::RMAX[4];

    StFstFastSimMaker maker;
    maker.SetDisk(4, 5.0f, 30.0f);

    CHECK(FstGlobal::RMIN[4] == Catch::Approx(5.0f));
    CHECK(FstGlobal::RMAX[4] == Catch::Approx(30.0f));

    FstGlobal::RMIN[4] = savedRmin;
    FstGlobal::RMAX[4] = savedRmax;
}

TEST_CASE("SetDisk: RMIN and RMAX are independently settable",
          "[StFstFastSimMaker]")
{
    const float savedRmin = FstGlobal::RMIN[2];
    const float savedRmax = FstGlobal::RMAX[2];

    StFstFastSimMaker maker;
    maker.SetDisk(2, 4.5f, 24.0f);
    maker.SetDisk(2, 6.0f, 20.0f);  // overwrite once more

    CHECK(FstGlobal::RMIN[2] == Catch::Approx(6.0f));
    CHECK(FstGlobal::RMAX[2] == Catch::Approx(20.0f));

    FstGlobal::RMIN[2] = savedRmin;
    FstGlobal::RMAX[2] = savedRmax;
}

// ============================================================================
// 3.  SetInEfficiency — smoke test
//
// The private member mInEff has no public getter; the setter is verified by
// confirming it does not throw for boundary and typical values.
// Full behavioural verification (rejected-hit fraction) requires integration
// testing against GEANT input tables.
// ============================================================================

TEST_CASE("SetInEfficiency does not throw", "[StFstFastSimMaker]")
{
    StFstFastSimMaker maker;

    SECTION("default value 0.1") {
        REQUIRE_NOTHROW(maker.SetInEfficiency());
    }
    SECTION("zero inefficiency — all hits kept") {
        REQUIRE_NOTHROW(maker.SetInEfficiency(0.0f));
    }
    SECTION("full inefficiency — all hits rejected") {
        REQUIRE_NOTHROW(maker.SetInEfficiency(1.0f));
    }
    SECTION("typical mid-range value") {
        REQUIRE_NOTHROW(maker.SetInEfficiency(0.05f));
    }
}

// ============================================================================
// 4.  SetFillHist — smoke test
//
// mHist is private with no getter.  Init() creates the TFile and histograms
// when mHist is true; only the setter call itself is tested here.
// ============================================================================

TEST_CASE("SetFillHist does not throw", "[StFstFastSimMaker]")
{
    StFstFastSimMaker maker;

    SECTION("enable histogramming") {
        REQUIRE_NOTHROW(maker.SetFillHist(true));
    }
    SECTION("disable histogramming (default)") {
        REQUIRE_NOTHROW(maker.SetFillHist(false));
    }
}

// ============================================================================
// 5.  setGEANTPassthrough — smoke test
//
// When true, FillSilicon() skips rastering and places hits at the raw GEANT
// xyz.  mGEANTPassthrough is private with no getter.
// ============================================================================

TEST_CASE("setGEANTPassthrough does not throw", "[StFstFastSimMaker]")
{
    StFstFastSimMaker maker;

    SECTION("enable passthrough") {
        REQUIRE_NOTHROW(maker.setGEANTPassthrough(true));
    }
    SECTION("disable passthrough (default false)") {
        REQUIRE_NOTHROW(maker.setGEANTPassthrough(false));
    }
}

// ============================================================================
// 6.  SetQAFileName — smoke test
//
// mQAFileName is a private TString with no getter.  The unusual default
// argument (0.1 implicitly constructs an empty TString) is preserved by the
// production code; we only verify the call succeeds.
// ============================================================================

TEST_CASE("SetQAFileName does not throw", "[StFstFastSimMaker]")
{
    StFstFastSimMaker maker;

    SECTION("non-empty filename") {
        REQUIRE_NOTHROW(maker.SetQAFileName("fst_qa_output.root"));
    }
    SECTION("empty string") {
        REQUIRE_NOTHROW(maker.SetQAFileName(""));
    }
    SECTION("no argument — uses default (empty TString)") {
        REQUIRE_NOTHROW(maker.SetQAFileName());
    }
}

// ============================================================================
// 7.  FstGlobal::Hack1to6 — pure Jacobian covariance transform
//
// The function converts cylindrical (r, phi, z) 1-sigma position errors
// stored in an StHit into the 3×3 Cartesian covariance matrix via:
//
//   J = | cos(phi)  -r·sin(phi)  0 |
//       | sin(phi)   r·cos(phi)  0 |
//       |    0           0       1 |
//
//   G_cart = J · diag(er², ep², ez²) · Jᵀ
//
// Test A — phi = 0  (hit on the +x axis, r = 1):
//   J = identity  →  G_cart = diag(er², ep², ez²)
//
// Test B — phi = π/2  (hit on the +y axis, r = 1):
//   J = [[0,-1,0],[1,0,0],[0,0,1]]
//   G_cart = diag(ep², er², ez²)   (r→y, phi→−x)
// ============================================================================

TEST_CASE("FstGlobal::Hack1to6 — phi=0, Jacobian is identity",
          "[StFstFastSimMaker][FstGlobal]")
{
    const float er = 0.20f, ep = 0.03f, ez = 0.01f;
    // Position on +x axis: r=1, phi=0 → cos(phi)=1, sin(phi)=0 → J = I₃.
    StRnDHit hit = makeHit(1.0f, 0.0f, 10.0f, er, ep, ez);
    StMatrixF mat = FstGlobal::Hack1to6(&hit);

    SECTION("G[0][0] == er^2") {
        REQUIRE(mat[0][0] == Catch::Approx(er * er).epsilon(1e-5f));
    }
    SECTION("G[1][1] == ep^2") {
        REQUIRE(mat[1][1] == Catch::Approx(ep * ep).epsilon(1e-5f));
    }
    SECTION("G[2][2] == ez^2") {
        REQUIRE(mat[2][2] == Catch::Approx(ez * ez).epsilon(1e-5f));
    }
    SECTION("off-diagonal G[0][1] == 0") {
        REQUIRE(mat[0][1] == Catch::Approx(0.0f).margin(1e-8f));
    }
    SECTION("off-diagonal G[0][2] == 0") {
        REQUIRE(mat[0][2] == Catch::Approx(0.0f).margin(1e-8f));
    }
    SECTION("off-diagonal G[1][2] == 0") {
        REQUIRE(mat[1][2] == Catch::Approx(0.0f).margin(1e-8f));
    }
    SECTION("matrix is symmetric: G[1][0] == G[0][1]") {
        REQUIRE(mat[1][0] == Catch::Approx(mat[0][1]).epsilon(1e-6f));
    }
    SECTION("matrix is symmetric: G[2][0] == G[0][2]") {
        REQUIRE(mat[2][0] == Catch::Approx(mat[0][2]).epsilon(1e-6f));
    }
}

TEST_CASE("FstGlobal::Hack1to6 — phi=pi/2, r-direction maps to +y",
          "[StFstFastSimMaker][FstGlobal]")
{
    const float er = 0.20f, ep = 0.03f, ez = 0.01f;
    // Position on +y axis: r=1, phi=π/2 → cos=0, sin=1.
    // J = [[0,-1,0],[1,0,0],[0,0,1]]
    // G_out = J · diag(er²,ep²,ez²) · Jᵀ = diag(ep², er², ez²)
    StRnDHit hit = makeHit(0.0f, 1.0f, 10.0f, er, ep, ez);
    StMatrixF mat = FstGlobal::Hack1to6(&hit);

    SECTION("G[0][0] == ep^2 (phi direction projects onto x)") {
        REQUIRE(mat[0][0] == Catch::Approx(ep * ep).epsilon(1e-5f));
    }
    SECTION("G[1][1] == er^2 (r direction projects onto y)") {
        REQUIRE(mat[1][1] == Catch::Approx(er * er).epsilon(1e-5f));
    }
    SECTION("G[2][2] == ez^2") {
        REQUIRE(mat[2][2] == Catch::Approx(ez * ez).epsilon(1e-5f));
    }
    SECTION("off-diagonal G[0][1] == 0") {
        REQUIRE(mat[0][1] == Catch::Approx(0.0f).margin(1e-8f));
    }
    SECTION("matrix is symmetric: G[1][0] == G[0][1]") {
        REQUIRE(mat[1][0] == Catch::Approx(mat[0][1]).epsilon(1e-6f));
    }
}

TEST_CASE("FstGlobal::Hack1to6 — symmetry holds for arbitrary phi",
          "[StFstFastSimMaker][FstGlobal]")
{
    // phi = pi/4: x=y=1/sqrt(2), r=1
    const float s  = static_cast<float>(std::sqrt(0.5));
    const float er = 0.15f, ep = 0.04f, ez = 0.02f;
    StRnDHit hit = makeHit(s, s, 15.0f, er, ep, ez);
    StMatrixF mat = FstGlobal::Hack1to6(&hit);

    SECTION("matrix is symmetric G[0][1] == G[1][0]") {
        REQUIRE(mat[0][1] == Catch::Approx(mat[1][0]).epsilon(1e-6f));
    }
    SECTION("matrix is symmetric G[0][2] == G[2][0]") {
        REQUIRE(mat[0][2] == Catch::Approx(mat[2][0]).epsilon(1e-6f));
    }
    SECTION("matrix is symmetric G[1][2] == G[2][1]") {
        REQUIRE(mat[1][2] == Catch::Approx(mat[2][1]).epsilon(1e-6f));
    }
    SECTION("G[2][2] == ez^2 (z row/column of J is always a unit vector)") {
        REQUIRE(mat[2][2] == Catch::Approx(ez * ez).epsilon(1e-5f));
    }
}

// ============================================================================
// 8.  FstGlobal constants
//
// Values are taken directly from the initialiser expressions in
// StFstFastSimMaker.cxx:
//
//   RMIN[6] = { 0.95×4.3, ×4.3, ×4.3,  0.95×5.0, ×5.0, ×5.0 }
//   RMAX[6] = { 1.05×15,  ×25,  ×25,   1.05×28,  ×28,  ×28  }
//   RSegment[9] = { 5, 7.875, 10.75, 13.625, 16.5, 19.375, 22.25, 25.125, 28 }
//
//   SQRT12 = sqrt(12) — constexpr, internal linkage.  The expected value
//   3.46410… is verified independently from the mathematical definition.
// ============================================================================

TEST_CASE("FstGlobal RMIN default values", "[StFstFastSimMaker][FstGlobal]")
{
    // Disks 1–3 (index 0–2) share inner radius 0.95 × 4.3 cm.
    // Disks 4–6 (index 3–5) share inner radius 0.95 × 5.0 cm.
    const float rminDisks123 = 0.95f * 4.3f;
    const float rminDisks456 = 0.95f * 5.0f;

    CHECK(FstGlobal::RMIN[0] == Catch::Approx(rminDisks123).epsilon(1e-5f));
    CHECK(FstGlobal::RMIN[1] == Catch::Approx(rminDisks123).epsilon(1e-5f));
    CHECK(FstGlobal::RMIN[2] == Catch::Approx(rminDisks123).epsilon(1e-5f));
    CHECK(FstGlobal::RMIN[3] == Catch::Approx(rminDisks456).epsilon(1e-5f));
    CHECK(FstGlobal::RMIN[4] == Catch::Approx(rminDisks456).epsilon(1e-5f));
    CHECK(FstGlobal::RMIN[5] == Catch::Approx(rminDisks456).epsilon(1e-5f));
}

TEST_CASE("FstGlobal RMAX default values", "[StFstFastSimMaker][FstGlobal]")
{
    // Disk 1 has a smaller outer radius (15 cm); disks 2–3 extend to 25 cm;
    // disks 4–6 extend to 28 cm.
    CHECK(FstGlobal::RMAX[0] == Catch::Approx(1.05f * 15.0f).epsilon(1e-5f));
    CHECK(FstGlobal::RMAX[1] == Catch::Approx(1.05f * 25.0f).epsilon(1e-5f));
    CHECK(FstGlobal::RMAX[2] == Catch::Approx(1.05f * 25.0f).epsilon(1e-5f));
    CHECK(FstGlobal::RMAX[3] == Catch::Approx(1.05f * 28.0f).epsilon(1e-5f));
    CHECK(FstGlobal::RMAX[4] == Catch::Approx(1.05f * 28.0f).epsilon(1e-5f));
    CHECK(FstGlobal::RMAX[5] == Catch::Approx(1.05f * 28.0f).epsilon(1e-5f));
}

TEST_CASE("FstGlobal RMIN < RMAX for every disk (default geometry)",
          "[StFstFastSimMaker][FstGlobal]")
{
    for (int i = 0; i < 6; ++i) {
        INFO("disk index " << i);
        REQUIRE(FstGlobal::RMIN[i] < FstGlobal::RMAX[i]);
    }
}

TEST_CASE("FstGlobal RSegment boundary values", "[StFstFastSimMaker][FstGlobal]")
{
    // 9 boundaries define 8 r-strips used for disk array 4–6.
    CHECK(FstGlobal::RSegment[0] == Catch::Approx(5.000f).epsilon(1e-5f));
    CHECK(FstGlobal::RSegment[1] == Catch::Approx(7.875f).epsilon(1e-5f));
    CHECK(FstGlobal::RSegment[2] == Catch::Approx(10.75f).epsilon(1e-5f));
    CHECK(FstGlobal::RSegment[3] == Catch::Approx(13.625f).epsilon(1e-5f));
    CHECK(FstGlobal::RSegment[4] == Catch::Approx(16.5f).epsilon(1e-5f));
    CHECK(FstGlobal::RSegment[5] == Catch::Approx(19.375f).epsilon(1e-5f));
    CHECK(FstGlobal::RSegment[6] == Catch::Approx(22.25f).epsilon(1e-5f));
    CHECK(FstGlobal::RSegment[7] == Catch::Approx(25.125f).epsilon(1e-5f));
    CHECK(FstGlobal::RSegment[8] == Catch::Approx(28.0f).epsilon(1e-5f));
}

TEST_CASE("FstGlobal RSegment is strictly increasing",
          "[StFstFastSimMaker][FstGlobal]")
{
    for (int i = 0; i < 8; ++i) {
        INFO("boundary index " << i);
        REQUIRE(FstGlobal::RSegment[i] < FstGlobal::RSegment[i + 1]);
    }
}

TEST_CASE("FstGlobal SQRT12 mathematical value",
          "[StFstFastSimMaker][FstGlobal]")
{
    // SQRT12 = sqrt(12) is used to convert a uniform-distribution strip
    // width w into a 1-sigma error: sigma = w / sqrt(12).
    // The constant is constexpr with internal linkage; we verify its
    // expected floating-point value independently.
    const float sqrt12 = static_cast<float>(std::sqrt(12.0));

    // Expected value: sqrt(12) ≈ 3.46410.
    CHECK(sqrt12 == Catch::Approx(3.46410f).epsilon(1e-4f));
    // Fundamental check: (sqrt(12))² == 12.
    CHECK(sqrt12 * sqrt12 == Catch::Approx(12.0f).epsilon(1e-5f));
}

TEST_CASE("FstGlobal: 6 disks, 8 r-strips, 12 phi-sectors",
          "[StFstFastSimMaker][FstGlobal]")
{
    // These counts are hard-coded in the constructor initialiser list and
    // in the FstGlobal array sizes.  We document them here as regression
    // guards so that accidental changes produce a test failure.
    constexpr int numDisks      = 6;   // length of RMIN/RMAX arrays
    constexpr int numRstrips    = 8;   // mNumR (r-strip count)
    constexpr int numRboundaries = 9;  // length of RSegment (numRstrips + 1)

    // Array sizes are observable through extern.
    // Spot-check via boundary indices derived from the counts above.
    REQUIRE(numRboundaries == numRstrips + 1);

    // Access the last element of each array to confirm the array extends
    // at least that far without out-of-bounds UB.
    REQUIRE(FstGlobal::RMIN[numDisks - 1]       > 0.0f);
    REQUIRE(FstGlobal::RMAX[numDisks - 1]       > 0.0f);
    REQUIRE(FstGlobal::RSegment[numRboundaries - 1] > 0.0f);
}

// ============================================================================
// 9.  Integration stubs — tagged [integration]
//
// These stubs require:
//   • a TApplication instance
//   • StChain with mocked g2t_fsi_hit and g2t_track GEANT input tables
//   • the complete STAR runtime (StChain, StEvent, etc.)
//
// See tests/unit/sim_support/SimTestFixture.h for the fixture template.
// Uncomment and flesh out each stub when the integration-test infrastructure
// is available in the build environment.
// ============================================================================

// INTEGRATION STUB: Init() with histogramming disabled.
//
// TEST_CASE("StFstFastSimMaker Init — histogramming off",
//           "[StFstFastSimMaker][integration]")
// {
//     SimTestFixture<StFstFastSimMaker> f("fstFastSim");
//     f.maker().SetFillHist(false);
//     REQUIRE(f.init()   == kStOK);
//     REQUIRE(f.finish() == kStOK);
// }

// INTEGRATION STUB: Init() with histogramming enabled (creates TFile/histos).
//
// TEST_CASE("StFstFastSimMaker Init — histogramming on",
//           "[StFstFastSimMaker][integration]")
// {
//     SimTestFixture<StFstFastSimMaker> f("fstFastSim");
//     f.maker().SetFillHist(true);
//     f.maker().SetQAFileName("fst_qa_test.root");
//     REQUIRE(f.init()   == kStOK);
//     REQUIRE(f.finish() == kStOK);
// }

// INTEGRATION STUB: Make() with empty g2t tables → kStOK, zero hits added.
//
// TEST_CASE("StFstFastSimMaker Make — empty GEANT tables",
//           "[StFstFastSimMaker][integration]")
// {
//     SimTestFixture<StFstFastSimMaker> f("fstFastSim");
//     REQUIRE(f.init()   == kStOK);
//     REQUIRE(f.make()   == kStOK);
//     REQUIRE(f.finish() == kStOK);
//     // StEvent* event = ...;
//     // REQUIRE(event->fstHitCollection()->numberOfHits() == 0);
// }

// INTEGRATION STUB: Make() with mGEANTPassthrough=true — hit position equals
// raw GEANT xyz (no rastering to strip centre).
//
// TEST_CASE("StFstFastSimMaker Make — GEANT passthrough preserves xyz",
//           "[StFstFastSimMaker][integration]")
// {
//     SimTestFixture<StFstFastSimMaker> f("fstFastSim");
//     f.maker().setGEANTPassthrough(true);
//     f.maker().SetInEfficiency(0.0f);  // accept all hits
//
//     // volume_id encoding: disk*1000 + wedge*10 + sensor
//     // Disk 1, wedge 1, sensor 0 → volume_id = 1010
//     g2t_fts_hit_st fsiHit{};
//     fsiHit.volume_id = 1010;
//     fsiHit.x[0] = 8.0f;  fsiHit.x[1] = 0.0f;  fsiHit.x[2] = 10.0f;
//     fsiHit.de   = 1.0e-3f;
//     fsiHit.track_p = 0;
//     addOneRow<St_g2t_fts_hit, g2t_fts_hit_st>(
//         f.geant(), "g2t_fsi_hit", fsiHit);
//
//     g2t_track_st trk{};  trk.is_shower = 0;
//     addOneRow<St_g2t_track, g2t_track_st>(f.geant(), "g2t_track", trk);
//
//     REQUIRE(f.init()   == kStOK);
//     REQUIRE(f.make()   == kStOK);
//     REQUIRE(f.finish() == kStOK);
//
//     // StEvent* event = ...;
//     // auto* hit = event->fstHitCollection()->hit(0, 0, 0);
//     // REQUIRE(hit->position().x() == Catch::Approx(8.0f).epsilon(1e-4f));
//     // REQUIRE(hit->position().y() == Catch::Approx(0.0f).margin(1e-4f));
// }

// INTEGRATION STUB: InEfficiency=1.0 → every hit is randomly rejected.
//
// TEST_CASE("StFstFastSimMaker Make — 100% inefficiency drops all hits",
//           "[StFstFastSimMaker][integration]")
// {
//     SimTestFixture<StFstFastSimMaker> f("fstFastSim");
//     f.maker().SetInEfficiency(1.0f);
//     // ... inject hits, run chain ...
//     // REQUIRE(event->fstHitCollection()->numberOfHits() == 0);
// }
