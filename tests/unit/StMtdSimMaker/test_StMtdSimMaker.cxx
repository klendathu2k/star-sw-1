// tests/unit/StMtdSimMaker/test_StMtdSimMaker.cxx
//
// Catch2 v3 unit tests for StMtdSimMaker (Muon Telescope Detector simulator).
//
// Structural tests (construction, CalcCellId arithmetic, default flags) run
// standalone without TApplication.  Integration stubs tagged [integration]
// document what a full-chain test would exercise but SKIP at runtime so the
// binary can run on CI without a live STAR environment.
//
// Build: link against -lStMtdSimMaker -lStEvent -lStMcEvent -lStChain
//        plus Catch2 (catch_amalgamated.cpp).

#include "../catch_amalgamated.hpp"

#include "StMtdSimMaker.h"

// The file-scope array defined in StMtdSimMaker.cxx that maps GEANT backleg
// indices (1-based) to corrected backleg IDs.  It is zero-initialised at
// program start (C++ guarantees zero-init for namespace-scope POD arrays).
// CalcCellId uses geant2backlegIDMap[backlegTemp - 1] where
// backlegTemp = (volume_id/100)/10.  Tests that need a specific mapping inject
// a value through MapGuard, which restores the original on scope exit.
extern Int_t geant2backlegIDMap[30];

// ---------------------------------------------------------------------------
// Physical constants mirrored from StMtdSimMaker.cxx for expected-value
// calculations.  Kept here so readers do not need to open the implementation.
//
//   kMtdPadWidth = 3.8 cm (pad) + 0.6 cm (inner gap) = 4.4 cm
//   kNCell       = 12 cells per module
//
// Cell formula (from CalcCellId):
//   icell = Int_t((ylocal + kMtdPadWidth * kNCell / 2) / kMtdPadWidth)
//         = Int_t((ylocal + 26.4f) / 4.4f)
//   if imodule > 3:  icell = 11 - icell   (strip numbering reversed)
//
// Return value (all ranges valid):
//   icell + 100 * (imodule + 100 * ibackleg)
// ---------------------------------------------------------------------------
static constexpr float kTestPadWidth = 4.4f;
static constexpr int   kTestNCell    = 12;
// Half-span used in the cell formula: kMtdPadWidth * kNCell / 2 = 26.4 cm
// Note: operator precedence means (kMtdPadWidth * kNCell) / 2, i.e. float
// arithmetic throughout.
static constexpr float kTestHalfSpan = kTestPadWidth * kTestNCell / 2.0f;  // 26.4 f

// ---------------------------------------------------------------------------
// TestMtdSimMaker
//
// Thin public subclass that surfaces the protected Boolean flags for
// white-box testing.  No ClassDef: this class is never streamed.
// ---------------------------------------------------------------------------
class TestMtdSimMaker : public StMtdSimMaker {
public:
    explicit TestMtdSimMaker(const char* name = "MtdSim")
        : StMtdSimMaker(name) {}

    bool getBookHisto()    const { return static_cast<bool>(mBookHisto);    }
    bool getWriteHisto()   const { return static_cast<bool>(mWriteHisto);   }
    bool getWriteStEvent() const { return static_cast<bool>(mWriteStEvent); }
};

// ---------------------------------------------------------------------------
// MapGuard
//
// RAII helper that sets one entry of the global geant2backlegIDMap to a
// known value and restores the original on destruction.  This keeps
// CalcCellId tests independent of each other.
// ---------------------------------------------------------------------------
struct MapGuard {
    int   index;
    Int_t saved;

    MapGuard(int i, Int_t newVal) : index(i), saved(geant2backlegIDMap[i]) {
        geant2backlegIDMap[i] = newVal;
    }
    ~MapGuard() { geant2backlegIDMap[index] = saved; }
};

// ===========================================================================
// 1.  Construction and GetName
// ===========================================================================

TEST_CASE("StMtdSimMaker construction — default name", "[StMtdSimMaker]") {
    StMtdSimMaker maker;

    SECTION("GetName returns the hard-coded default 'MtdSim'") {
        // Default argument in StMtdSimMaker(const char* name = "MtdSim").
        REQUIRE(std::string(maker.GetName()) == "MtdSim");
    }
}

TEST_CASE("StMtdSimMaker construction — custom name", "[StMtdSimMaker]") {
    StMtdSimMaker maker("myMtdSim");

    SECTION("GetName returns the name provided at construction") {
        REQUIRE(std::string(maker.GetName()) == "myMtdSim");
    }
}

// ===========================================================================
// 2.  Default flag values
//
// mBookHisto, mWriteHisto, and mWriteStEvent are set explicitly in the ctor
// body before Reset() is called; Reset() does not alter them.
// ===========================================================================

TEST_CASE("StMtdSimMaker default flags", "[StMtdSimMaker]") {
    TestMtdSimMaker maker;

    // mBookHisto=kTRUE  — histograms are booked by default so that Init()
    // creates them when called in a normal BFC chain.
    SECTION("mBookHisto is true by default") {
        CHECK(maker.getBookHisto() == true);
    }

    // mWriteHisto=kFALSE — histograms are NOT written to a file unless the
    // user explicitly enables it, avoiding spurious output files.
    SECTION("mWriteHisto is false by default") {
        CHECK(maker.getWriteHisto() == false);
    }

    // mWriteStEvent=kTRUE — simulated hits are written into StEvent so
    // downstream makers (e.g. StMtdMatchMaker) can use them.
    SECTION("mWriteStEvent is true by default") {
        CHECK(maker.getWriteStEvent() == true);
    }
}

// ===========================================================================
// 3.  CalcCellId — pure arithmetic, no ROOT/DB required
//
// Decoding:
//   ires       = volume_id / 100
//   backlegTemp = ires / 10         (1-based GEANT backleg index, range 1..30)
//   ibackleg   = geant2backlegIDMap[backlegTemp - 1]
//   imodule    = ires % 10
//   if ibackleg in [12, 20]: imodule += 1
//   icell      = Int_t((ylocal + 26.4f) / 4.4f)
//   if imodule > 3: icell = 11 - icell
//   return icell + 100*(imodule + 100*ibackleg)   on success
//   return -1  if icell  < 0 || icell  >= 12
//   return -2  if imodule < 0 || imodule > 99
//   return -3  if ibackleg < 0 || ibackleg > 99
//
// Test vectors use ylocal values that are comfortably within a single cell
// bin (not on a bin boundary) to avoid sensitivity to floating-point rounding.
//
// All tests in this section use the zero-initialised geant2backlegIDMap
// (ibackleg = 0) unless a MapGuard is in scope.
// ===========================================================================

TEST_CASE("CalcCellId — mid-span ylocal, imodule<=3 (no cell flip)",
          "[StMtdSimMaker][CalcCellId]")
{
    // volume_id=11200: ires=112, backlegTemp=11, imodule=2
    // geant2backlegIDMap[10]=0 (zero-init) → ibackleg=0, not in [12,20]
    // ylocal=2.0: icell = Int_t((2.0+26.4)/4.4) = Int_t(6.454...) = 6
    // imodule=2 <= 3, no flip
    // cellId = 6 + 100*(2 + 100*0) = 206
    StMtdSimMaker maker;
    Int_t ibackleg{-1}, imodule{-1}, icell{-1};
    const int cellId = maker.CalcCellId(11200, 2.0f, ibackleg, imodule, icell);

    REQUIRE(cellId == 206);
    CHECK(ibackleg == 0);
    CHECK(imodule  == 2);
    CHECK(icell    == 6);
}

TEST_CASE("CalcCellId — imodule>3 reverses cell index",
          "[StMtdSimMaker][CalcCellId]")
{
    // volume_id=11400: ires=114, backlegTemp=11, imodule=4
    // geant2backlegIDMap[10]=0 → ibackleg=0, not in [12,20]
    // ylocal=2.0: icell_raw=6; imodule=4>3 → icell = 11-6 = 5
    // cellId = 5 + 100*(4 + 100*0) = 405
    StMtdSimMaker maker;
    Int_t ibackleg{-1}, imodule{-1}, icell{-1};
    const int cellId = maker.CalcCellId(11400, 2.0f, ibackleg, imodule, icell);

    REQUIRE(cellId == 405);
    CHECK(ibackleg == 0);
    CHECK(imodule  == 4);
    CHECK(icell    == 5);
}

TEST_CASE("CalcCellId — highest valid cell (cell 11), imodule<=3",
          "[StMtdSimMaker][CalcCellId]")
{
    // ylocal=24.0: icell = Int_t((24.0+26.4)/4.4) = Int_t(11.454...) = 11
    // cellId = 11 + 100*(2 + 100*0) = 211
    StMtdSimMaker maker;
    Int_t ibackleg{-1}, imodule{-1}, icell{-1};
    const int cellId = maker.CalcCellId(11200, 24.0f, ibackleg, imodule, icell);

    REQUIRE(cellId == 211);
    CHECK(icell == 11);
}

TEST_CASE("CalcCellId — lowest valid cell (cell 0), imodule<=3",
          "[StMtdSimMaker][CalcCellId]")
{
    // ylocal=-24.0: icell = Int_t((-24.0+26.4)/4.4) = Int_t(0.545...) = 0
    // cellId = 0 + 100*(2 + 100*0) = 200
    StMtdSimMaker maker;
    Int_t ibackleg{-1}, imodule{-1}, icell{-1};
    const int cellId = maker.CalcCellId(11200, -24.0f, ibackleg, imodule, icell);

    REQUIRE(cellId == 200);
    CHECK(icell == 0);
}

TEST_CASE("CalcCellId — returns -1 when ylocal too large (icell >= 12)",
          "[StMtdSimMaker][CalcCellId]")
{
    // ylocal=27.0: icell = Int_t((27.0+26.4)/4.4) = Int_t(12.136...) = 12
    // icell >= kNCell(12) → returns -1
    StMtdSimMaker maker;
    Int_t ibackleg{-1}, imodule{-1}, icell{-1};
    const int result = maker.CalcCellId(11200, 27.0f, ibackleg, imodule, icell);

    REQUIRE(result == -1);
}

TEST_CASE("CalcCellId — returns -1 when ylocal too negative (icell < 0)",
          "[StMtdSimMaker][CalcCellId]")
{
    // ylocal=-31.5: icell = Int_t((-31.5+26.4)/4.4) = Int_t(-1.159...) = -1
    // icell < 0 → returns -1
    StMtdSimMaker maker;
    Int_t ibackleg{-1}, imodule{-1}, icell{-1};
    const int result = maker.CalcCellId(11200, -31.5f, ibackleg, imodule, icell);

    REQUIRE(result == -1);
}

TEST_CASE("CalcCellId — injected backleg mapping is applied correctly",
          "[StMtdSimMaker][CalcCellId]")
{
    // Set geant2backlegIDMap[2] = 5 (backlegTemp=3 → index 2).
    // volume_id=3200: ires=32, backlegTemp=3, imodule=2; ibackleg=5
    // ibackleg=5, not in [12,20], imodule stays 2
    // ylocal=2.0: icell=6; imodule=2<=3, no flip
    // cellId = 6 + 100*(2 + 100*5) = 6 + 50200 = 50206
    MapGuard guard(2, 5);
    StMtdSimMaker maker;
    Int_t ibackleg{-1}, imodule{-1}, icell{-1};
    const int cellId = maker.CalcCellId(3200, 2.0f, ibackleg, imodule, icell);

    REQUIRE(cellId == 50206);
    CHECK(ibackleg == 5);
    CHECK(imodule  == 2);
    CHECK(icell    == 6);
}

TEST_CASE("CalcCellId — ibackleg in [12,20] increments imodule by one",
          "[StMtdSimMaker][CalcCellId]")
{
    // Set geant2backlegIDMap[2] = 15.
    // volume_id=3200: ires=32, backlegTemp=3, imodule=2; ibackleg=15
    // ibackleg=15 in [12,20] → imodule = 2+1 = 3
    // ylocal=2.0: icell=6; imodule=3 not > 3, no flip
    // cellId = 6 + 100*(3 + 100*15) = 6 + 150300 = 150306
    MapGuard guard(2, 15);
    StMtdSimMaker maker;
    Int_t ibackleg{-1}, imodule{-1}, icell{-1};
    const int cellId = maker.CalcCellId(3200, 2.0f, ibackleg, imodule, icell);

    REQUIRE(cellId == 150306);
    CHECK(ibackleg == 15);
    CHECK(imodule  == 3);
    CHECK(icell    == 6);
}

TEST_CASE("CalcCellId — ibackleg in [12,20] with imodule>3 after increment",
          "[StMtdSimMaker][CalcCellId]")
{
    // Set geant2backlegIDMap[2] = 13.
    // volume_id=3300: ires=33, backlegTemp=3, imodule=3; ibackleg=13
    // ibackleg=13 in [12,20] → imodule = 3+1 = 4
    // ylocal=2.0: icell_raw=6; imodule=4>3 → icell = 11-6 = 5
    // cellId = 5 + 100*(4 + 100*13) = 5 + 100*1304 = 5 + 130400 = 130405
    MapGuard guard(2, 13);
    StMtdSimMaker maker;
    Int_t ibackleg{-1}, imodule{-1}, icell{-1};
    const int cellId = maker.CalcCellId(3300, 2.0f, ibackleg, imodule, icell);

    REQUIRE(cellId == 130405);
    CHECK(ibackleg == 13);
    CHECK(imodule  == 4);
    CHECK(icell    == 5);
}

// ===========================================================================
// 4.  Integration stubs  (tagged [integration])
//
// These TEST_CASEs are intentionally left as SKIP stubs.  They document what
// a full-chain integration test would exercise and serve as scaffolding for
// future implementation with TApplication + SimTestFixture<StMtdSimMaker>.
// ===========================================================================

TEST_CASE("StMtdSimMaker Init lifecycle [integration]",
          "[StMtdSimMaker][integration]")
{
    SKIP("Requires TApplication and ROOT histogram memory management");
    // SimTestFixture<StMtdSimMaker> f("MtdSim");
    // REQUIRE(f.init() == kStOK);
    //
    // After Init(), bookHistograms() should have created all TH1/TH2 objects.
    // Verify by calling FastCellResponse on an empty collection (no crash).
}

TEST_CASE("StMtdSimMaker InitRun with mock database [integration]",
          "[StMtdSimMaker][integration]")
{
    SKIP("Requires a populated mtdGeant2BacklegIDMap DB table");
    // mtdGeant2BacklegIDMap_st mapTable{};
    // for (int i = 0; i < 30; ++i) mapTable.geant2backlegID[i] = i + 1;
    //
    // MockDbDataset db;
    // db.inject("Geometry/mtd/mtdGeant2BacklegIDMap",
    //           "mtdGeant2BacklegIDMap", &mapTable, sizeof(mapTable));
    //
    // SimTestFixture<StMtdSimMaker> f("MtdSim");
    // // inject db into chain via MockDbMaker
    // REQUIRE(f.init()     == kStOK);
    // REQUIRE(f.initRun(1) == kStOK);
    //
    // After InitRun, geant2backlegIDMap[i] should equal i+1.
    // CHECK(geant2backlegIDMap[0] == 1);
}

TEST_CASE("StMtdSimMaker Make processes a single MTD hit [integration]",
          "[StMtdSimMaker][integration]")
{
    SKIP("Requires GEANT g2t_mtd_hit table and StEvent in the chain");
    // g2t_mtd_hit_st hit{};
    // hit.volume_id = 11200;  // backlegTemp=11, imodule=2
    // hit.x[1]     = 2.0f;   // ylocal → cell 6
    // hit.tof      = 10.e-9; // 10 ns
    // hit.de       = 2.e-3;  // 2 MeV
    // hit.s_track  = 1.0f;   // non-zero → hit is processed
    // hit.track_p  = 1;
    //
    // MockGeantDataset geant;
    // addMtdHit(geant, hit);   // addOneRow<St_g2t_mtd_hit, g2t_mtd_hit_st>
    //
    // SimTestFixture<StMtdSimMaker> f("MtdSim");
    // f.geant() = std::move(geant);
    // REQUIRE(f.init()     == kStOK);
    // REQUIRE(f.initRun(1) == kStOK);
    // REQUIRE(f.make()     == kStOK);
    //
    // StMtdCollection* coll = ...;
    // REQUIRE(coll != nullptr);
    // CHECK(coll->mtdHits().size() == 1u);
    // CHECK(coll->mtdHits()[0]->cell() == 6);
}

TEST_CASE("StMtdSimMaker Finish writes histogram file when mWriteHisto=true [integration]",
          "[StMtdSimMaker][integration]")
{
    SKIP("Requires TFile and histogram objects created during Init");
    // SimTestFixture<StMtdSimMaker> f("MtdSim");
    // f.maker()./* set mWriteHisto = true via a setter or subclass */;
    // REQUIRE(f.init()   == kStOK);
    // REQUIRE(f.finish() == kStOK);
    // CHECK( /* mtdsim.root exists */ );
}
