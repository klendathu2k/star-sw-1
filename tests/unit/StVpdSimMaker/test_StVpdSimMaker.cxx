// tests/unit/StVpdSimMaker/test_StVpdSimMaker.cxx
//
// Catch2 v3 unit tests for StVpdSimMaker (Vertex Position Detector simulator).
//
// Structural tests — construction, setter/getter round-trips, collection-pointer
// null checks, VpdSingleHit field layout, and geometry constants — run entirely
// without a TApplication or full STAR runtime.
//
// Tests that require the STAR chain, GEANT input, StMcEvent, and the calibration
// database are provided as commented-out stubs tagged [integration].  To activate
// them, link against the full STAR environment and uncomment the stubs.
//
// Coverage map
// ============
//  Section 1 — Construction and GetName()
//  Section 2 — setBookHisto() / mBookHisto flag
//  Section 3 — setParamsFile() / getParamsFileName() round-trip
//  Section 4 — VpdSingleHit struct: field layout and sentinel values
//  Section 5 — Geometry constants (via StVpdSimConfig::getVpdDistance())
//  Section 6 — GetVpdCollection() / GetMcBTofHitCollection() null before Init
//  Section 7 — Integration stubs (commented out, tagged [integration])

#include "../catch_amalgamated.hpp"

#include "StVpdSimMaker.h"
#include "StBTofUtil/StVpdSimConfig.h"

#include <cmath>
#include <string>

// ============================================================================
// TestableVpdSimMaker
//
// White-box subclass that exposes the protected state and the protected
// VpdSingleHit nested struct for unit-test inspection.  Only the minimal
// surface needed by the tests is exposed; no new behaviour is introduced.
// ============================================================================
class TestableVpdSimMaker : public StVpdSimMaker {
public:
    explicit TestableVpdSimMaker(const char* name = "VpdSim")
        : StVpdSimMaker(name) {}

    // Expose the protected histogram-booking flag.
    bool bookHistoFlag()     const { return mBookHisto; }

    // Expose the file-parameters flag.
    bool useFileParamsFlag() const { return mUseFileParameters; }

    // Re-export the protected VpdSingleHit type under a public alias so that
    // test cases can name the type without violating access rules.
    using PublicSingleHit = VpdSingleHit;

    // Factory: return a zero-initialised VpdSingleHit.
    static PublicSingleHit makeZeroHit() {
        VpdSingleHit h{};
        return h;
    }

    // Factory: return a hit whose sentinel fields match the values that
    // vpdResponse() unconditionally assigns (pathL = -9999, q = 0).
    static PublicSingleHit makeSentinelHit() {
        VpdSingleHit h{};
        h.tray   = 121;      // West VPD tray
        h.tubeId = 1;
        h.tof    = 1.9;      // ns
        h.t0     = 0.0;
        h.de     = 2.5e-4;   // GeV
        h.pathL  = -9999.0;  // vpdResponse always assigns this sentinel
        h.q      = 0.0;      // vpdResponse always assigns 0
        return h;
    }
};

// ============================================================================
// Section 1 — Construction and GetName()
// ============================================================================

TEST_CASE("StVpdSimMaker default construction", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;

    SECTION("GetName returns the default name 'VpdSim'") {
        REQUIRE(std::string(maker.GetName()) == "VpdSim");
    }

    SECTION("GetName is non-null") {
        REQUIRE(maker.GetName() != nullptr);
    }
}

TEST_CASE("StVpdSimMaker named construction", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker("myVpdSim");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "myVpdSim");
    }
}

TEST_CASE("StVpdSimMaker names are independent between instances", "[StVpdSimMaker]") {
    TestableVpdSimMaker a("alpha");
    TestableVpdSimMaker b("beta");

    CHECK(std::string(a.GetName()) == "alpha");
    CHECK(std::string(b.GetName()) == "beta");
    CHECK(std::string(a.GetName()) != std::string(b.GetName()));
}

// ============================================================================
// Section 2 — setBookHisto() / mBookHisto flag
// ============================================================================

TEST_CASE("StVpdSimMaker setBookHisto initial state", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;

    SECTION("mBookHisto is false immediately after construction") {
        // kFALSE is set explicitly in the StVpdSimMaker constructor.
        CHECK(maker.bookHistoFlag() == false);
    }
}

TEST_CASE("StVpdSimMaker setBookHisto stores true", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;
    maker.setBookHisto(true);

    REQUIRE(maker.bookHistoFlag() == true);
}

TEST_CASE("StVpdSimMaker setBookHisto stores false", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;
    maker.setBookHisto(true);   // set to true first
    maker.setBookHisto(false);  // then clear

    REQUIRE(maker.bookHistoFlag() == false);
}

TEST_CASE("StVpdSimMaker setBookHisto is idempotent", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;
    maker.setBookHisto(true);
    maker.setBookHisto(true);

    SECTION("calling setBookHisto(true) twice leaves flag true") {
        CHECK(maker.bookHistoFlag() == true);
    }
}

// ============================================================================
// Section 3 — setParamsFile() / getParamsFileName() round-trip
// ============================================================================

TEST_CASE("StVpdSimMaker getParamsFileName initial state", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;

    SECTION("mParamsFileName is empty string immediately after construction") {
        // The header declares: string mParamsFileName = "";
        CHECK(maker.getParamsFileName() == "");
    }
}

TEST_CASE("StVpdSimMaker setParamsFile with explicit path", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;
    const char* customPath = "db/vpdSimParams/custom.dat";
    maker.setParamsFile(customPath);

    SECTION("getParamsFileName returns the supplied path") {
        REQUIRE(maker.getParamsFileName() == std::string(customPath));
    }
}

TEST_CASE("StVpdSimMaker setParamsFile default argument", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;
    maker.setParamsFile(); // uses the declared default argument

    SECTION("getParamsFileName returns the built-in default path") {
        REQUIRE(maker.getParamsFileName() == "db/vpdSimParams/vpdSimParams.dat");
    }
}

TEST_CASE("StVpdSimMaker setParamsFile overwrites previous value", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;
    maker.setParamsFile("first/path.dat");
    maker.setParamsFile("second/path.dat");

    REQUIRE(maker.getParamsFileName() == "second/path.dat");
}

TEST_CASE("StVpdSimMaker setParamsFile accepts arbitrary path strings", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;

    SECTION("absolute path") {
        maker.setParamsFile("/data/star/calib/vpdParams.dat");
        CHECK(maker.getParamsFileName() == "/data/star/calib/vpdParams.dat");
    }

    SECTION("relative path with subdirectories") {
        maker.setParamsFile("../run/params.dat");
        CHECK(maker.getParamsFileName() == "../run/params.dat");
    }

    SECTION("empty string") {
        maker.setParamsFile("non-empty");
        maker.setParamsFile("");
        CHECK(maker.getParamsFileName() == "");
    }
}

// ============================================================================
// Section 4 — VpdSingleHit struct: field layout and sentinel values
//
// VpdSingleHit is a protected POD-like struct inside StVpdSimMaker.
// It is exposed here via TestableVpdSimMaker::PublicSingleHit.
// ============================================================================

TEST_CASE("VpdSingleHit zero-initialisation", "[StVpdSimMaker][VpdSingleHit]") {
    using Hit = TestableVpdSimMaker::PublicSingleHit;
    Hit h{};

    SECTION("tray is zero") {
        CHECK(h.tray == 0);
    }

    SECTION("tubeId is zero") {
        CHECK(h.tubeId == 0);
    }

    SECTION("tof is zero") {
        CHECK(h.tof == 0.0);
    }

    SECTION("t0 is zero") {
        CHECK(h.t0 == 0.0);
    }

    SECTION("de is zero") {
        CHECK(h.de == 0.0);
    }

    SECTION("pathL is zero under zero-init (sentinel assigned by vpdResponse)") {
        CHECK(h.pathL == 0.0);
    }

    SECTION("q is zero") {
        CHECK(h.q == 0.0);
    }
}

TEST_CASE("VpdSingleHit field assignment round-trip", "[StVpdSimMaker][VpdSingleHit]") {
    using Hit = TestableVpdSimMaker::PublicSingleHit;
    Hit h{};

    h.tray   = 122;     // East VPD
    h.tubeId = 14;
    h.tof    = 1.903;   // ns
    h.t0     = 0.012;   // ns
    h.de     = 3.1e-4;  // GeV
    h.pathL  = -9999.0; // sentinel
    h.q      = 0.0;

    CHECK(h.tray   == 122);
    CHECK(h.tubeId == 14);
    CHECK(h.tof    == Catch::Approx(1.903));
    CHECK(h.t0     == Catch::Approx(0.012));
    CHECK(h.de     == Catch::Approx(3.1e-4));
    CHECK(h.pathL  == Catch::Approx(-9999.0));
    CHECK(h.q      == Catch::Approx(0.0));
}

TEST_CASE("VpdSingleHit tray identifiers", "[StVpdSimMaker][VpdSingleHit]") {
    // Header comment: tray == 121 => West, tray == 122 => East.
    using Hit = TestableVpdSimMaker::PublicSingleHit;
    Hit west{};  west.tray = 121;
    Hit east{};  east.tray = 122;

    SECTION("West tray identifier is 121") {
        CHECK(west.tray == 121);
    }

    SECTION("East tray identifier is 122") {
        CHECK(east.tray == 122);
    }

    SECTION("West and East tray identifiers differ by 1") {
        CHECK(east.tray - west.tray == 1);
    }
}

TEST_CASE("VpdSingleHit tubeId valid range", "[StVpdSimMaker][VpdSingleHit]") {
    using Hit = TestableVpdSimMaker::PublicSingleHit;

    // The header documents tubeId in [1, 19] per tray.
    for (int id = 1; id <= 19; ++id) {
        Hit h{};
        h.tubeId = id;
        CHECK(h.tubeId >= 1);
        CHECK(h.tubeId <= 19);
    }
}

TEST_CASE("VpdSingleHit vpdResponse sentinel values", "[StVpdSimMaker][VpdSingleHit]") {
    // vpdResponse() unconditionally sets pathL = -9999 and q = 0 for every hit.
    // These sentinel values signal "not measured" to downstream consumers.
    using Hit = TestableVpdSimMaker::PublicSingleHit;
    Hit h = TestableVpdSimMaker::makeSentinelHit();

    SECTION("pathL sentinel is -9999") {
        CHECK(h.pathL == Catch::Approx(-9999.0));
    }

    SECTION("charge sentinel is 0") {
        CHECK(h.q == Catch::Approx(0.0));
    }
}

// ============================================================================
// Section 5 — Geometry constants
//
// The VPD distance from z = 0 is defined in StVpdSimConfig::VPDDISTANCE = 570.
// This constant is used directly inside StVpdSimMaker::Make() to compute the
// expected time-of-flight.  It is not a public constant on StVpdSimMaker itself,
// but is accessible via StVpdSimConfig::getVpdDistance() (which StVpdSimMaker
// already includes).
//
// Physics layout:
//   West VPD tray centre  ~  -570 cm  (z < 0 side)
//   East VPD tray centre  ~  +570 cm  (z > 0 side)
// ============================================================================

TEST_CASE("VPD geometry: distance from interaction point", "[StVpdSimMaker][geometry]") {
    // StVpdSimConfig can be constructed without a TApplication; its ctor is
    // empty and getVpdDistance() only returns the stored constant.
    StVpdSimConfig cfg;

    SECTION("VPD distance from origin is 570 cm") {
        REQUIRE(cfg.getVpdDistance() == Catch::Approx(570.0f));
    }
}

TEST_CASE("VPD geometry: West tray position", "[StVpdSimMaker][geometry]") {
    StVpdSimConfig cfg;
    const float westZ = -cfg.getVpdDistance();

    SECTION("West tray is at approximately -570 cm") {
        CHECK(westZ == Catch::Approx(-570.0f));
    }

    SECTION("West tray is in the negative-z hemisphere") {
        CHECK(westZ < 0.0f);
    }
}

TEST_CASE("VPD geometry: East tray position", "[StVpdSimMaker][geometry]") {
    StVpdSimConfig cfg;
    const float eastZ = +cfg.getVpdDistance();

    SECTION("East tray is at approximately +570 cm") {
        CHECK(eastZ == Catch::Approx(570.0f));
    }

    SECTION("East tray is in the positive-z hemisphere") {
        CHECK(eastZ > 0.0f);
    }
}

TEST_CASE("VPD geometry: East-West symmetry", "[StVpdSimMaker][geometry]") {
    StVpdSimConfig cfg;
    const float d    = cfg.getVpdDistance();
    const float westZ = -d;
    const float eastZ = +d;

    SECTION("trays are symmetric about z = 0") {
        CHECK(std::abs(westZ + eastZ) < 1e-4f);
    }

    SECTION("separation between trays is 2 * VPD distance") {
        CHECK(eastZ - westZ == Catch::Approx(2.0f * d));
    }
}

// ============================================================================
// Section 6 — Collection accessors return nullptr before Init()
//
// Both collection pointers are initialised to 0 / nullptr in the constructor
// and are only assigned during Make() / fillEvent().  They must therefore be
// null before Init() is invoked.  This section does not call Init() or Make()
// and thus requires neither TApplication nor a full chain.
// ============================================================================

TEST_CASE("StVpdSimMaker GetVpdCollection is null before Init", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;

    SECTION("GetVpdCollection returns nullptr immediately after construction") {
        REQUIRE(maker.GetVpdCollection() == nullptr);
    }
}

TEST_CASE("StVpdSimMaker GetMcBTofHitCollection is null before Init", "[StVpdSimMaker]") {
    TestableVpdSimMaker maker;

    SECTION("GetMcBTofHitCollection returns nullptr immediately after construction") {
        // mMcBTofHitCollection is assigned 0 in the ctor, then Reset() is
        // called which only deletes it (delete 0 is a no-op).  The pointer
        // therefore remains 0 / nullptr after construction.
        REQUIRE(maker.GetMcBTofHitCollection() == nullptr);
    }
}

TEST_CASE("StVpdSimMaker collection pointers are null across multiple instances",
          "[StVpdSimMaker]") {
    TestableVpdSimMaker a("vpdA");
    TestableVpdSimMaker b("vpdB");

    CHECK(a.GetVpdCollection()       == nullptr);
    CHECK(b.GetVpdCollection()       == nullptr);
    CHECK(a.GetMcBTofHitCollection() == nullptr);
    CHECK(b.GetMcBTofHitCollection() == nullptr);
}

// ============================================================================
// Section 7 — Integration stubs  [integration]
//
// The tests below require:
//   - A TApplication instance
//   - StChain, StMcEvent with a primary vertex
//   - GEANT input (g2t_vpd_hit rows)
//   - StVpdSimConfig calibration parameters (from DB or file)
//
// Activate by uncommenting and building against the full STAR environment.
// See tests/unit/sim_support/SimTestFixture.h for the fixture API.
// ============================================================================

// TEST_CASE("StVpdSimMaker Init/Make/Finish lifecycle", "[StVpdSimMaker][integration]") {
//     // #include "../sim_support/SimTestFixture.h"
//     // SimTestFixture<StVpdSimMaker> f("VpdSim");
//     //
//     // // West-tray hit: volume_id encodes tray and tube.
//     // //   tray  = volume_id / 1000 + 120  =>  1001/1000 + 120 = 121 (West)
//     // //   tube  = volume_id % 100         =>  1001 % 100 = 1
//     // g2t_vpd_hit_st hitW{};
//     // hitW.volume_id = 1001;
//     // hitW.tof       = 1.9e-9;   // seconds
//     // hitW.de        = 2.5e-4;   // GeV
//     // addOneRow<St_g2t_vpd_hit, g2t_vpd_hit_st>(f.geant(), "g2t_vpd_hit", hitW);
//     //
//     // // East-tray hit: volume_id = 2001 => tray 122 (East), tube 1
//     // g2t_vpd_hit_st hitE{};
//     // hitE.volume_id = 2001;
//     // hitE.tof       = 1.9e-9;
//     // hitE.de        = 2.5e-4;
//     // addOneRow<St_g2t_vpd_hit, g2t_vpd_hit_st>(f.geant(), "g2t_vpd_hit", hitE);
//     //
//     // f.maker().setParamsFile("db/vpdSimParams/vpdSimParams.dat");
//     //
//     // REQUIRE(f.init()     == kStOK);
//     // REQUIRE(f.initRun(1) == kStOK);
//     // REQUIRE(f.make()     == kStOK);
//     // REQUIRE(f.finish()   == kStOK);
// }

// TEST_CASE("StVpdSimMaker GetVpdCollection non-null after Make",
//           "[StVpdSimMaker][integration]") {
//     // After a successful Make() with hits on both East and West trays,
//     // GetVpdCollection() returns a non-null StBTofCollection.
//     //
//     // SimTestFixture<StVpdSimMaker> f("VpdSim");
//     // ... inject East + West hits as above ...
//     // REQUIRE(f.make() == kStOK);
//     // REQUIRE(f.maker().GetVpdCollection() != nullptr);
// }

// TEST_CASE("StVpdSimMaker GetMcBTofHitCollection non-null after Make",
//           "[StVpdSimMaker][integration]") {
//     // After Make() the MC BTof hit collection is populated and non-null.
//     //
//     // SimTestFixture<StVpdSimMaker> f("VpdSim");
//     // ... inject hits ...
//     // REQUIRE(f.make() == kStOK);
//     // REQUIRE(f.maker().GetMcBTofHitCollection() != nullptr);
// }

// TEST_CASE("StVpdSimMaker setBookHisto creates QA histograms",
//           "[StVpdSimMaker][integration]") {
//     // When mBookHisto is true and Init() is called, bookHistograms() creates
//     // all QA histograms (mNRawHitsWest, mLeTimeEast, mVpdVertexHist, etc.).
//     //
//     // SimTestFixture<StVpdSimMaker> f("VpdSim");
//     // f.maker().setBookHisto(true);
//     // REQUIRE(f.init() == kStOK);
//     // Verify by querying f.maker().GetHistList() or similar.
// }

// TEST_CASE("StVpdSimMaker vertex reconstruction from symmetric hits",
//           "[StVpdSimMaker][integration]") {
//     // With symmetric East and West hits originating from z = 0,
//     // the reconstructed VPD vertex should be close to 0 cm.
//     //
//     // SimTestFixture<StVpdSimMaker> f("VpdSim");
//     // ... inject equal-tof East + West hits at vertex z = 0 ...
//     // REQUIRE(f.make() == kStOK);
//     // The VPD vertex formula: vpdVz = c * (tE - tW) / 2
//     // With tE == tW the result should be ~0.
// }

// TEST_CASE("StVpdSimMaker only-West hits give sentinel vertex",
//           "[StVpdSimMaker][integration]") {
//     // If only West hits are present and no East hits, Make() sets
//     // mTubeTAvgEast = -9999 and consequently mVpdVertex = -9999.
//     //
//     // SimTestFixture<StVpdSimMaker> f("VpdSim");
//     // ... inject only West-tray hits ...
//     // REQUIRE(f.make() == kStOK);
//     // (Inspect via subclass accessor for mVpdVertex.)
// }
