// tests/unit/StSsdSimulationMaker/test_StSsdSimulationMaker.cxx
//
// Catch2 v3 unit tests for the three makers in StRoot/StSsdSimulationMaker:
//
//   St_sls_Maker        — Geant hits → strip signals ("SSD Lazy Simulator")
//   St_spa_Maker        — DAQ / pedestal simulation ("SSD Pedestal Annihilator")
//   StSsdEmbeddingMaker — merging simulated strips with real SSD raw data
//
// Tests are organised into three groups:
//   [St_sls_Maker]          construction, naming, GetCVS, geometry helpers,
//                           IsOnWafer, idWaferToWafer
//   [St_spa_Maker]          construction, naming, GetCVS
//   [StSsdEmbeddingMaker]   construction, naming, GetCVS, setters,
//                           idWaferToWafer
//
// Tests tagged [integration] require a full STAR / ROOT runtime
// (StSsdDbMaker, geometry database, StMcEvent, DAQ reader …) and are
// provided as stubs only.  They document the expected lifecycle contract
// without executing it.
//
// NOTE: IsOnWafer and setSsdParameters tests include
//       "tables/St_ssdDimensions_Table.h", which is generated during the
//       STAR build.  The tests are compiled only when that header is
//       available (i.e. inside a standard STAR build tree).

#include "../catch_amalgamated.hpp"

#include "St_sls_Maker.h"
#include "St_spa_Maker.h"
#include "StSsdEmbeddingMaker.h"

// Generated table header — available after a standard STAR cmake/cons build.
#include "tables/St_ssdDimensions_Table.h"

#include <string>
#include <cstring>

// ============================================================================
// Helpers
// ============================================================================

/// Return a ssdDimensions_st populated with the nominal SSD geometry values
/// taken from StDb/idl/ssdDimensions.idl.
static ssdDimensions_st makeNominalGeometry()
{
    ssdDimensions_st g{};
    g.wafersPerLadder    = 16;
    g.a128PerSide        = 6;
    g.stripPerSide       = 768;
    g.stripPitch         = 0.0095;   // cm
    g.stereoAngle        = 0.0175;   // rad
    g.waferHalfLength    = 3.75;     // cm
    g.waferHalfWidth     = 2.1;      // cm
    g.waferHalfThickness = 0.015;    // cm
    g.waferHalfActLength = 3.65;     // cm  → mDetectorLargeEdge = 7.30 cm
    g.waferHalfActWidth  = 2.0;      // cm  → mDetectorSmallEdge = 4.00 cm
    return g;
}

// ============================================================================
// St_sls_Maker — construction
// ============================================================================

TEST_CASE("St_sls_Maker construction with default name", "[St_sls_Maker]")
{
    St_sls_Maker maker;

    SECTION("GetName returns the default 'sls_strip'") {
        REQUIRE(std::string(maker.GetName()) == "sls_strip");
    }

    SECTION("GetName is non-null") {
        REQUIRE(maker.GetName() != nullptr);
    }
}

TEST_CASE("St_sls_Maker construction with explicit name", "[St_sls_Maker]")
{
    St_sls_Maker maker("my_sls");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(maker.GetName()) == "my_sls");
    }
}

TEST_CASE("St_sls_Maker default name is 'sls_strip'", "[St_sls_Maker]")
{
    // The default argument in the declaration is "sls_strip".
    St_sls_Maker a;
    St_sls_Maker b("sls_strip");
    SECTION("explicit 'sls_strip' and default produce the same name") {
        REQUIRE(std::string(a.GetName()) == std::string(b.GetName()));
    }
}

// ============================================================================
// St_sls_Maker — GetCVS
// ============================================================================

TEST_CASE("St_sls_Maker::GetCVS", "[St_sls_Maker]")
{
    St_sls_Maker maker("cvs_sls");
    const char *cvs = maker.GetCVS();

    SECTION("GetCVS returns a non-null pointer") {
        REQUIRE(cvs != nullptr);
    }

    SECTION("GetCVS string is non-empty") {
        REQUIRE(std::string(cvs).size() > 0);
    }

    SECTION("GetCVS string references the source file") {
        // The CVS tag embeds the filename "St_sls_Maker.h".
        REQUIRE(std::string(cvs).find("St_sls_Maker") != std::string::npos);
    }
}

// ============================================================================
// St_sls_Maker — idWaferToWafer (inline, no initialisation required)
//
// Formula: (idWafer - 7000) / 100 - 1   (integer arithmetic)
// Encodes: layer*1000 + waf*100 + ladder  →  0-based wafer index within ladder
// ============================================================================

TEST_CASE("St_sls_Maker::idWaferToWafer formula", "[St_sls_Maker]")
{
    St_sls_Maker maker;

    SECTION("wafer 1 (waf field = 1) maps to index 0") {
        // idWafer = 7*1000 + 1*100 + 1 = 7101
        REQUIRE(maker.idWaferToWafer(7101) == 0);
    }

    SECTION("wafer 2 (waf field = 2) maps to index 1") {
        // idWafer = 7*1000 + 2*100 + 1 = 7201
        REQUIRE(maker.idWaferToWafer(7201) == 1);
    }

    SECTION("wafer 3 (waf field = 3) maps to index 2") {
        // idWafer = 7*1000 + 3*100 + 1 = 7301
        REQUIRE(maker.idWaferToWafer(7301) == 2);
    }

    SECTION("ladder index does not affect the waf-field component") {
        // Different ladders, same waf-field → same index.
        REQUIRE(maker.idWaferToWafer(7101) == maker.idWaferToWafer(7102));
        REQUIRE(maker.idWaferToWafer(7201) == maker.idWaferToWafer(7220));
    }

    SECTION("formula matches manual integer arithmetic") {
        for (int waf = 1; waf <= 16; ++waf) {
            int idWafer   = 7000 + waf * 100 + 1;   // ladder 1, varying waf
            int expected  = (idWafer - 7000) / 100 - 1;
            REQUIRE(maker.idWaferToWafer(idWafer) == expected);
        }
    }
}

// ============================================================================
// St_sls_Maker — setSsdParameters / IsOnWafer
//
// setSsdParameters sets:
//   mDetectorLargeEdge = 2 * waferHalfActLength   (active X half-width × 2)
//   mDetectorSmallEdge = 2 * waferHalfActWidth     (active Y half-width × 2)
//
// IsOnWafer returns 1 if |x| ≤ LargeEdge/2  AND  |y| ≤ SmallEdge/2,
//                         0 otherwise.
//
// With nominal geometry:
//   LargeEdge = 7.30 cm  →  boundary at ±3.65 cm in X
//   SmallEdge = 4.00 cm  →  boundary at ±2.00 cm in Y
// ============================================================================

TEST_CASE("St_sls_Maker::setSsdParameters does not crash", "[St_sls_Maker]")
{
    St_sls_Maker maker;
    ssdDimensions_st g = makeNominalGeometry();

    // setSsdParameters is public; verify it is callable without crashing.
    REQUIRE_NOTHROW(maker.setSsdParameters(&g));
}

TEST_CASE("St_sls_Maker::IsOnWafer — points inside active area", "[St_sls_Maker]")
{
    St_sls_Maker maker;
    ssdDimensions_st g = makeNominalGeometry();
    maker.setSsdParameters(&g);

    SECTION("origin is inside active area") {
        StThreeVector<double> p(0.0, 0.0, 0.0);
        REQUIRE(maker.IsOnWafer(p) == 1);
    }

    SECTION("point near centre is inside") {
        StThreeVector<double> p(1.0, 0.5, 0.0);
        REQUIRE(maker.IsOnWafer(p) == 1);
    }

    SECTION("point at nominal SSD wafer half-lengths is inside") {
        // waferHalfActLength = 3.65, waferHalfActWidth = 2.0 — boundary values.
        // The check is strict (>) so exactly at the boundary is still inside.
        StThreeVector<double> p(3.65, 2.0, 0.0);
        REQUIRE(maker.IsOnWafer(p) == 1);
    }

    SECTION("negative-quadrant point within bounds is inside") {
        StThreeVector<double> p(-2.5, -1.5, 0.0);
        REQUIRE(maker.IsOnWafer(p) == 1);
    }
}

TEST_CASE("St_sls_Maker::IsOnWafer — points outside active area", "[St_sls_Maker]")
{
    St_sls_Maker maker;
    ssdDimensions_st g = makeNominalGeometry();
    maker.setSsdParameters(&g);

    SECTION("x exceeds positive large-edge boundary") {
        StThreeVector<double> p(4.0, 0.0, 0.0);   // 4.0 > 3.65
        REQUIRE(maker.IsOnWafer(p) == 0);
    }

    SECTION("x exceeds negative large-edge boundary") {
        StThreeVector<double> p(-4.0, 0.0, 0.0);  // -4.0 < -3.65
        REQUIRE(maker.IsOnWafer(p) == 0);
    }

    SECTION("y exceeds positive small-edge boundary") {
        StThreeVector<double> p(0.0, 2.5, 0.0);   // 2.5 > 2.0
        REQUIRE(maker.IsOnWafer(p) == 0);
    }

    SECTION("y exceeds negative small-edge boundary") {
        StThreeVector<double> p(0.0, -2.5, 0.0);  // -2.5 < -2.0
        REQUIRE(maker.IsOnWafer(p) == 0);
    }

    SECTION("both x and y outside bounds") {
        StThreeVector<double> p(5.0, 3.0, 0.0);
        REQUIRE(maker.IsOnWafer(p) == 0);
    }
}

TEST_CASE("St_sls_Maker::IsOnWafer respects custom geometry", "[St_sls_Maker]")
{
    St_sls_Maker maker;
    ssdDimensions_st g{};
    g.wafersPerLadder    = 4;
    g.stripPerSide       = 128;
    g.stripPitch         = 0.020;
    g.stereoAngle        = 0.010;
    // Active area: 2 cm × 1 cm  →  boundary ±1.0 cm in X, ±0.5 cm in Y.
    g.waferHalfActLength = 1.0;
    g.waferHalfActWidth  = 0.5;
    maker.setSsdParameters(&g);

    SECTION("point inside custom active area") {
        StThreeVector<double> p(0.5, 0.25, 0.0);
        REQUIRE(maker.IsOnWafer(p) == 1);
    }

    SECTION("point outside custom active area in X") {
        StThreeVector<double> p(1.5, 0.0, 0.0);   // 1.5 > 1.0
        REQUIRE(maker.IsOnWafer(p) == 0);
    }

    SECTION("point outside custom active area in Y") {
        StThreeVector<double> p(0.0, 0.8, 0.0);   // 0.8 > 0.5
        REQUIRE(maker.IsOnWafer(p) == 0);
    }
}

// ============================================================================
// St_sls_Maker — idWaferToLadderNumb
//
// Formula: iW = (idWafer - 7000)/100;  iL = idWafer - 7000 - iW*100;
//          return iL - 1;  (0-based ladder index)
// ============================================================================

TEST_CASE("St_sls_Maker::idWaferToLadderNumb formula", "[St_sls_Maker]")
{
    St_sls_Maker maker;
    ssdDimensions_st g = makeNominalGeometry();
    maker.setSsdParameters(&g);

    SECTION("ladder 1  maps to index 0") {
        // idWafer = 7*1000 + 1*100 + 1 = 7101
        REQUIRE(maker.idWaferToLadderNumb(7101) == 0);
    }

    SECTION("ladder 2  maps to index 1") {
        // idWafer = 7*1000 + 1*100 + 2 = 7102
        REQUIRE(maker.idWaferToLadderNumb(7102) == 1);
    }

    SECTION("ladder 20 maps to index 19") {
        // idWafer = 7*1000 + 1*100 + 20 = 7120
        REQUIRE(maker.idWaferToLadderNumb(7120) == 19);
    }

    SECTION("waf field does not affect the ladder index") {
        // Different waf values, same ladder → same ladder index.
        REQUIRE(maker.idWaferToLadderNumb(7101) == maker.idWaferToLadderNumb(7201));
        REQUIRE(maker.idWaferToLadderNumb(7102) == maker.idWaferToLadderNumb(7302));
    }
}

// ============================================================================
// St_sls_Maker — printSsdParameters (smoke test — must not crash)
// ============================================================================

TEST_CASE("St_sls_Maker::printSsdParameters does not crash", "[St_sls_Maker]")
{
    St_sls_Maker maker;
    ssdDimensions_st g = makeNominalGeometry();
    maker.setSsdParameters(&g);
    REQUIRE_NOTHROW(maker.printSsdParameters());
}

// ============================================================================
// St_sls_Maker — integration stubs
// ============================================================================

// [integration] Init / Make / Finish lifecycle.
// These require: StSsdDbMaker in chain, geometry DB, StarMagField instance,
// g2t_ssd_hit / g2t_svt_hit GEANT tables, StMcEvent.
//
// TEST_CASE("St_sls_Maker Init returns kStOk", "[St_sls_Maker][integration]")
// {
//     St_sls_Maker maker("sls_integ");
//     // … set up StSsdDbMaker, chain, histos attribute …
//     REQUIRE(maker.Init()    == kStOK);
//     REQUIRE(maker.Make()    == kStOK);
//     REQUIRE(maker.Finish()  == kStOK);
// }

// ============================================================================
// St_spa_Maker — construction
// ============================================================================

TEST_CASE("St_spa_Maker construction with default name", "[St_spa_Maker]")
{
    St_spa_Maker maker;

    SECTION("GetName returns the default 'spa_strip'") {
        REQUIRE(std::string(maker.GetName()) == "spa_strip");
    }

    SECTION("GetName is non-null") {
        REQUIRE(maker.GetName() != nullptr);
    }
}

TEST_CASE("St_spa_Maker construction with explicit name", "[St_spa_Maker]")
{
    St_spa_Maker maker("my_spa");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(maker.GetName()) == "my_spa");
    }
}

TEST_CASE("St_spa_Maker default name is 'spa_strip'", "[St_spa_Maker]")
{
    St_spa_Maker a;
    St_spa_Maker b("spa_strip");
    SECTION("explicit 'spa_strip' and default produce the same name") {
        REQUIRE(std::string(a.GetName()) == std::string(b.GetName()));
    }
}

// ============================================================================
// St_spa_Maker — GetCVS
// ============================================================================

TEST_CASE("St_spa_Maker::GetCVS", "[St_spa_Maker]")
{
    St_spa_Maker maker("cvs_spa");
    const char *cvs = maker.GetCVS();

    SECTION("GetCVS returns a non-null pointer") {
        REQUIRE(cvs != nullptr);
    }

    SECTION("GetCVS string is non-empty") {
        REQUIRE(std::string(cvs).size() > 0);
    }

    SECTION("GetCVS string references the source file") {
        // The CVS tag embeds the filename "St_spa_Maker.h".
        REQUIRE(std::string(cvs).find("St_spa_Maker") != std::string::npos);
    }
}

// ============================================================================
// St_spa_Maker — integration stubs
// ============================================================================

// [integration] Init / Make / Finish lifecycle.
// These require: StSsdDbMaker in chain, ssdStripCalib database table,
// sdm_condition_db, sls_strip output from St_sls_Maker upstream.
//
// TEST_CASE("St_spa_Maker Init returns kStOk", "[St_spa_Maker][integration]")
// {
//     St_spa_Maker maker("spa_integ");
//     // … build chain with DB maker, run Init …
//     REQUIRE(maker.Init()    == kStOK);
//     REQUIRE(maker.Make()    == kStOK);
//     REQUIRE(maker.Finish()  == kStOK);
// }

// ============================================================================
// StSsdEmbeddingMaker — construction
// ============================================================================

TEST_CASE("StSsdEmbeddingMaker construction with default name", "[StSsdEmbeddingMaker]")
{
    StSsdEmbeddingMaker maker;

    SECTION("GetName returns the default 'SsdEmbed'") {
        REQUIRE(std::string(maker.GetName()) == "SsdEmbed");
    }

    SECTION("GetName is non-null") {
        REQUIRE(maker.GetName() != nullptr);
    }
}

TEST_CASE("StSsdEmbeddingMaker construction with explicit name", "[StSsdEmbeddingMaker]")
{
    StSsdEmbeddingMaker maker("myEmbed");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(maker.GetName()) == "myEmbed");
    }
}

TEST_CASE("StSsdEmbeddingMaker default name is 'SsdEmbed'", "[StSsdEmbeddingMaker]")
{
    StSsdEmbeddingMaker a;
    StSsdEmbeddingMaker b("SsdEmbed");
    SECTION("explicit 'SsdEmbed' and default produce the same name") {
        REQUIRE(std::string(a.GetName()) == std::string(b.GetName()));
    }
}

// ============================================================================
// StSsdEmbeddingMaker — GetCVS
// ============================================================================

TEST_CASE("StSsdEmbeddingMaker::GetCVS", "[StSsdEmbeddingMaker]")
{
    StSsdEmbeddingMaker maker("cvs_embed");
    const char *cvs = maker.GetCVS();

    SECTION("GetCVS returns a non-null pointer") {
        REQUIRE(cvs != nullptr);
    }

    SECTION("GetCVS string is non-empty") {
        REQUIRE(std::string(cvs).size() > 0);
    }

    SECTION("GetCVS string references the source file") {
        // The CVS tag embeds the filename "StSsdEmbeddingMaker.h".
        REQUIRE(std::string(cvs).find("StSsdEmbeddingMaker") != std::string::npos);
    }
}

// ============================================================================
// StSsdEmbeddingMaker — setDoEmbedding
//
// setDoEmbedding controls whether the maker attempts to merge simulated
// strips with real SSD DAQ data (mDoEmbedding).  There is no public getter;
// tests verify the setter is callable and does not crash.
// ============================================================================

TEST_CASE("StSsdEmbeddingMaker::setDoEmbedding is callable", "[StSsdEmbeddingMaker]")
{
    StSsdEmbeddingMaker maker;

    SECTION("setDoEmbedding(kTRUE) does not throw") {
        REQUIRE_NOTHROW(maker.setDoEmbedding(kTRUE));
    }

    SECTION("setDoEmbedding(kFALSE) does not throw") {
        REQUIRE_NOTHROW(maker.setDoEmbedding(kFALSE));
    }

    SECTION("toggling setDoEmbedding twice does not throw") {
        REQUIRE_NOTHROW([&]{
            maker.setDoEmbedding(kTRUE);
            maker.setDoEmbedding(kFALSE);
        }());
    }
}

// ============================================================================
// StSsdEmbeddingMaker — setPlainSimEvenIfNoSSD
//
// When kTRUE, the maker falls back to plain simulation instead of skipping
// the event when no SSD data are present in the DAQ stream.
// ============================================================================

TEST_CASE("StSsdEmbeddingMaker::setPlainSimEvenIfNoSSD is callable",
          "[StSsdEmbeddingMaker]")
{
    StSsdEmbeddingMaker maker;

    SECTION("setPlainSimEvenIfNoSSD(kTRUE) does not throw") {
        REQUIRE_NOTHROW(maker.setPlainSimEvenIfNoSSD(kTRUE));
    }

    SECTION("setPlainSimEvenIfNoSSD(kFALSE) does not throw") {
        REQUIRE_NOTHROW(maker.setPlainSimEvenIfNoSSD(kFALSE));
    }

    SECTION("toggling setPlainSimEvenIfNoSSD twice does not throw") {
        REQUIRE_NOTHROW([&]{
            maker.setPlainSimEvenIfNoSSD(kTRUE);
            maker.setPlainSimEvenIfNoSSD(kFALSE);
        }());
    }
}

// ============================================================================
// StSsdEmbeddingMaker — default flag values (construction invariants)
//
// The constructor sets:
//   mDoEmbedding     = kTRUE   (embedding active by default)
//   mPlainSimIfNoSSD = kFALSE  (skip event when no SSD in DAQ by default)
//
// Because the flags are private with no getters, the defaults are
// documented here via observable Make() behaviour in integration mode.
// The construction test below just confirms the object can be created
// without any assertion failures triggered by the defaults.
// ============================================================================

TEST_CASE("StSsdEmbeddingMaker default flags do not cause crash on construction",
          "[StSsdEmbeddingMaker]")
{
    // If the defaults were dangerously wrong the ctor would assert/crash.
    REQUIRE_NOTHROW(StSsdEmbeddingMaker{});
}

// ============================================================================
// StSsdEmbeddingMaker — idWaferToWafer (inline, same formula as St_sls_Maker)
//
// Formula: (idWafer - 7000) / 100 - 1   (integer arithmetic)
// ============================================================================

TEST_CASE("StSsdEmbeddingMaker::idWaferToWafer formula", "[StSsdEmbeddingMaker]")
{
    StSsdEmbeddingMaker maker;

    SECTION("wafer 1 maps to index 0") {
        REQUIRE(maker.idWaferToWafer(7101) == 0);
    }

    SECTION("wafer 2 maps to index 1") {
        REQUIRE(maker.idWaferToWafer(7201) == 1);
    }

    SECTION("wafer 16 maps to index 15") {
        // idWafer = 7*1000 + 16*100 + 1 = 8601
        REQUIRE(maker.idWaferToWafer(8601) == 15);
    }

    SECTION("formula matches St_sls_Maker for all waf fields 1..16") {
        St_sls_Maker sls;
        for (int waf = 1; waf <= 16; ++waf) {
            int id = 7000 + waf * 100 + 1;
            REQUIRE(maker.idWaferToWafer(id) == sls.idWaferToWafer(id));
        }
    }
}

// ============================================================================
// StSsdEmbeddingMaker — setSsdParameters (smoke test)
// ============================================================================

TEST_CASE("StSsdEmbeddingMaker::setSsdParameters does not crash",
          "[StSsdEmbeddingMaker]")
{
    StSsdEmbeddingMaker maker;
    ssdDimensions_st g = makeNominalGeometry();
    // setSsdParameters is public in StSsdEmbeddingMaker.
    REQUIRE_NOTHROW(maker.setSsdParameters(&g));
}

// ============================================================================
// StSsdEmbeddingMaker — integration stubs
// ============================================================================

// [integration] Init / Make / Finish lifecycle.
// These require: StSsdBarrel singleton, Geometry/ssd database,
// ssdDimensions table, StDAQReader with SSD data, spa_strip tables from
// SsdSimuData and SsdRealData datasets.
//
// TEST_CASE("StSsdEmbeddingMaker Init returns kStOk",
//           "[StSsdEmbeddingMaker][integration]")
// {
//     StSsdEmbeddingMaker maker("embed_integ");
//     // … build chain with geometry DB, DAQ reader, spa_strip datasets …
//     REQUIRE(maker.Init()    == kStOk);
//     REQUIRE(maker.Make()    == kStOk);
//     REQUIRE(maker.Finish()  == kStOK);
// }
//
// TEST_CASE("StSsdEmbeddingMaker Make skips event when SSD absent and "
//           "mPlainSimIfNoSSD is false",
//           "[StSsdEmbeddingMaker][integration]")
// {
//     // mDoEmbedding=true, mPlainSimIfNoSSD=false, DAQ reader reports no SSD
//     // → Make() must return kStOk and produce no output strips.
// }
//
// TEST_CASE("StSsdEmbeddingMaker Make runs plain simulation when SSD absent "
//           "and mPlainSimIfNoSSD is true",
//           "[StSsdEmbeddingMaker][integration]")
// {
//     StSsdEmbeddingMaker maker;
//     maker.setDoEmbedding(kTRUE);
//     maker.setPlainSimEvenIfNoSSD(kTRUE);
//     // … run Make() with empty DAQ SSD stream …
//     // mRunningEmbedding should be set to kFALSE internally.
// }
