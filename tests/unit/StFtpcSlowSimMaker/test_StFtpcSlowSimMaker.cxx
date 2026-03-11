// tests/unit/StFtpcSlowSimMaker/test_StFtpcSlowSimMaker.cxx
//
// Catch2 v3 unit tests for StFtpcSlowSimMaker (Forward TPC Slow Simulator).
//
// Public interface under test (from StFtpcSlowSimMaker.h):
//
//   StFtpcSlowSimMaker(const char *name = "ftpc_raw")
//   virtual ~StFtpcSlowSimMaker()
//   virtual Int_t Init()            -- integration test
//   virtual Int_t InitRun(int)      -- integration test
//   virtual Int_t Make()            -- integration test
//   virtual const char *GetCVS() const
//
// Free utility under test (declared extern in StFtpcSlowSimField.hh,
// implemented in StFtpcSlowSimLibs.cc):
//
//   int Locate(int npt, const float* x, float xx)
//
//   Performs a binary search on a monotone (ascending or descending) array
//   x[0..npt-1].  Returns the index jlow of the interval [x[jlow], x[jlow+1]]
//   that brackets xx.  For an exact interior node value x[i], the algorithm
//   returns the index of the interval immediately below it (jlow = i-1 when
//   x[i] is the upper bound of that interval).  Returns 0 and logs LOG_WARN
//   when xx is outside the array range.
//
// Protected member (not directly accessible from a unit test):
//
//   Float_t microsecondsPerTimebin
//     Set in InitRun() from the RHIC clock frequency or the
//     Calibrations_ftpc/ftpcElectronics database table.
//     Documented here; verified only via integration tests.
//
// FTPC geometry (loaded from database at runtime — not compile-time constants):
//
//   numberOfPadrowsPerSide =  10   (padrows 1-10 per FTPC half)
//   numberOfPadrows        =  20   (East + West combined)
//   numberOfSectors        =   6   (sectors per padrow, 60° each)
//   numberOfPads           = 182   (pads per sector)
//   numberOfTimebins       = 256 or 512 (configurable via ftpcElectronics)
//   geantPlane range       = [1, 20] (GEANT volume → padrow number)
//   geantSector range      = [1,  6] (GEANT volume → sector number)
//
// These tests run without a TApplication or STAR framework context.
// Integration stubs are tagged [integration] and can be excluded with:
//   ./test_StFtpcSlowSimMaker ~[integration]
//
// To add this test executable to the build, append to tests/unit/Conscript:
//
//   my $envFtpcSlowSim = $envTest->clone(
//       'CPPPATH' => $envTest->{CPPPATH}
//                 . $main::PATH_SEPARATOR . "#StRoot/StFtpcSlowSimMaker"
//                 . $main::PATH_SEPARATOR . "#StRoot/StFtpcClusterMaker",
//       'LIBS'    => "-lStFtpcSlowSimMaker -lStFtpcClusterMaker "
//                 . $envTest->{LIBS},
//   );
//   sub build_ftpcslowsim_test {
//       my ($name, @srcs) = @_;
//       my @objs = Objects $envFtpcSlowSim @srcs;
//       Program $envFtpcSlowSim $name, (@catch_obj, @objs);
//       Install $envFtpcSlowSim $BIN, $name;
//   }
//   build_ftpcslowsim_test( "test_StFtpcSlowSimMaker",
//       "StFtpcSlowSimMaker/test_StFtpcSlowSimMaker.cxx" );

#include "../catch_amalgamated.hpp"

#include "StFtpcSlowSimMaker.h"

// Locate() is defined in StFtpcSlowSimLibs.cc and declared extern in
// StFtpcSlowSimField.hh.  Forward-declare it here to avoid pulling in the
// full field header and its heavy dependencies.
extern int Locate(const int npt, const float* x, const float xx);

// ===========================================================================
// Construction and naming
// ===========================================================================

TEST_CASE("StFtpcSlowSimMaker construction with explicit name",
          "[StFtpcSlowSimMaker]") {
    StFtpcSlowSimMaker maker("testFtpc");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(maker.GetName()) == "testFtpc");
    }
}

TEST_CASE("StFtpcSlowSimMaker default construction", "[StFtpcSlowSimMaker]") {
    StFtpcSlowSimMaker maker;

    SECTION("GetName is non-null") {
        REQUIRE(maker.GetName() != nullptr);
    }

    SECTION("default name is 'ftpc_raw'") {
        // The header declares: StFtpcSlowSimMaker(const char *name="ftpc_raw")
        CHECK(std::string(maker.GetName()) == "ftpc_raw");
    }
}

TEST_CASE("StFtpcSlowSimMaker names are independent across instances",
          "[StFtpcSlowSimMaker]") {
    StFtpcSlowSimMaker east("ftpc_east");
    StFtpcSlowSimMaker west("ftpc_west");

    SECTION("east instance retains its name") {
        CHECK(std::string(east.GetName()) == "ftpc_east");
    }

    SECTION("west instance retains its name") {
        CHECK(std::string(west.GetName()) == "ftpc_west");
    }
}

TEST_CASE("StFtpcSlowSimMaker constructed with empty name",
          "[StFtpcSlowSimMaker]") {
    StFtpcSlowSimMaker maker("");

    SECTION("GetName is non-null even for empty name") {
        REQUIRE(maker.GetName() != nullptr);
    }
}

// ===========================================================================
// GetCVS
// ===========================================================================

TEST_CASE("StFtpcSlowSimMaker GetCVS", "[StFtpcSlowSimMaker]") {
    StFtpcSlowSimMaker maker("testFtpc");

    SECTION("returns a non-null pointer") {
        REQUIRE(maker.GetCVS() != nullptr);
    }

    SECTION("returns a non-empty string") {
        REQUIRE(std::string(maker.GetCVS()).size() > 0);
    }

    SECTION("string contains the source file identifier") {
        // The macro in the header expands to a string containing the CVS
        // $Id$ tag, which holds the file name.
        const std::string cvs(maker.GetCVS());
        CHECK(cvs.find("StFtpcSlowSimMaker") != std::string::npos);
    }

    SECTION("string contains the 'built' compilation timestamp marker") {
        // The macro appends: "built " __DATE__ " " __TIME__
        const std::string cvs(maker.GetCVS());
        CHECK(cvs.find("built") != std::string::npos);
    }
}

TEST_CASE("StFtpcSlowSimMaker GetCVS is consistent across calls",
          "[StFtpcSlowSimMaker]") {
    StFtpcSlowSimMaker maker("testFtpc");

    SECTION("two successive calls return identical strings") {
        const char* first  = maker.GetCVS();
        const char* second = maker.GetCVS();
        REQUIRE(first  != nullptr);
        REQUIRE(second != nullptr);
        CHECK(std::string(first) == std::string(second));
    }
}

// ===========================================================================
// Locate() — binary-search utility (StFtpcSlowSimLibs.cc)
//
// Locate(npt, x, xx) finds the bracket index jlow such that:
//   ascending:  x[jlow] <= xx (with x[jlow+1] the upper neighbour)
//   descending: x[jlow] >= xx (with x[jlow+1] the lower neighbour)
//
// The loop exits when jup - jlow == 1, so for a value xx equal to an
// interior node x[k], the algorithm returns the index of the interval
// whose upper bound is x[k] (i.e. jlow = k-1 when xx == x[k]).
//
// Only in-range cases are tested here to avoid triggering the LOG_WARN
// call (which dereferences gMessMgr) in the out-of-range branch.
// Out-of-range behaviour is exercised in the integration stub below.
// ===========================================================================

TEST_CASE("Locate ascending uniform array — interior values", "[Locate]") {
    // x = { 0, 1, 2, 3, 4 }  (5 uniform ascending points)
    const int   npt = 5;
    const float x[] = { 0.f, 1.f, 2.f, 3.f, 4.f };

    SECTION("value at x[0] (lower endpoint) returns 0") {
        // Bisection converges to interval [x[0], x[1]]: jlow = 0.
        CHECK(Locate(npt, x, 0.f) == 0);
    }

    SECTION("value strictly inside first interval [x[0], x[1]) returns 0") {
        CHECK(Locate(npt, x, 0.5f) == 0);
    }

    SECTION("value at interior node x[1] returns 0") {
        // At x[1]=1.0 the bisection converges with jup=1, jlow=0: returns 0.
        // The node belongs to the interval ending at x[1], not starting at it.
        CHECK(Locate(npt, x, 1.f) == 0);
    }

    SECTION("value strictly inside second interval (x[1], x[2]) returns 1") {
        CHECK(Locate(npt, x, 1.5f) == 1);
    }

    SECTION("value at interior node x[2] returns 1") {
        // Analogous to x[1]: bisection places x[2]=2.0 in interval [x[1],x[2]].
        CHECK(Locate(npt, x, 2.f) == 1);
    }

    SECTION("value strictly inside third interval (x[2], x[3]) returns 2") {
        CHECK(Locate(npt, x, 2.5f) == 2);
    }

    SECTION("value at interior node x[3] returns 2") {
        CHECK(Locate(npt, x, 3.f) == 2);
    }

    SECTION("value strictly inside last interval (x[3], x[4]) returns 3") {
        CHECK(Locate(npt, x, 3.5f) == 3);
    }

    SECTION("value at x[npt-1] (upper endpoint) returns npt-2") {
        // The upper endpoint x[4]=4.0 is the top of the range; the bisection
        // converges to jlow = npt-2 = 3.
        CHECK(Locate(npt, x, 4.f) == 3);
    }
}

TEST_CASE("Locate descending uniform array — interior values", "[Locate]") {
    // x = { 4, 3, 2, 1, 0 }  (5 uniform descending points)
    const int   npt = 5;
    const float x[] = { 4.f, 3.f, 2.f, 1.f, 0.f };

    SECTION("value at x[0] (top of descending range) returns 0") {
        CHECK(Locate(npt, x, 4.f) == 0);
    }

    SECTION("value strictly inside first descending interval (x[0],x[1]) returns 0") {
        // 3.5 is between x[0]=4 and x[1]=3 — belongs to interval 0.
        CHECK(Locate(npt, x, 3.5f) == 0);
    }

    SECTION("value at interior node x[1] of descending array returns 0") {
        // The bisection places x[1]=3.0 in the interval ending at x[1].
        CHECK(Locate(npt, x, 3.f) == 0);
    }

    SECTION("value strictly inside second descending interval returns 1") {
        CHECK(Locate(npt, x, 2.5f) == 1);
    }

    SECTION("value strictly inside second descending interval (fractional) returns 1") {
        CHECK(Locate(npt, x, 2.2f) == 1);
    }

    SECTION("value at interior node x[3] of descending array returns 2") {
        // x[3]=1.0: bisection converges to jlow=2 (interval [x[2],x[3]]).
        CHECK(Locate(npt, x, 1.f) == 2);
    }

    SECTION("value strictly inside last descending interval (x[3],x[4]) returns 3") {
        CHECK(Locate(npt, x, 0.5f) == 3);
    }
}

TEST_CASE("Locate non-uniform ascending array", "[Locate]") {
    // Non-uniform ascending spacings, mimicking realistic FTPC radial grid
    // points in cm.  x = { 7.5, 9.0, 10.75, 12.75, 15.25 }
    const int   npt = 5;
    const float x[] = { 7.5f, 9.0f, 10.75f, 12.75f, 15.25f };

    SECTION("value in first gap [7.5, 9.0) returns 0") {
        CHECK(Locate(npt, x, 8.0f) == 0);
    }

    SECTION("value in second gap (9.0, 10.75) returns 1") {
        CHECK(Locate(npt, x, 9.5f) == 1);
    }

    SECTION("value at interior node x[2]=10.75 returns 1") {
        // At an exact interior node the bisection returns the lower interval.
        CHECK(Locate(npt, x, 10.75f) == 1);
    }

    SECTION("value in third gap (10.75, 12.75) returns 2") {
        CHECK(Locate(npt, x, 11.5f) == 2);
    }

    SECTION("value in last gap (12.75, 15.25) returns 3") {
        CHECK(Locate(npt, x, 14.0f) == 3);
    }

    SECTION("lower endpoint 7.5 returns 0") {
        CHECK(Locate(npt, x, 7.5f) == 0);
    }

    SECTION("upper endpoint 15.25 returns npt-2 = 3") {
        CHECK(Locate(npt, x, 15.25f) == 3);
    }
}

TEST_CASE("Locate with minimum two-element array", "[Locate]") {
    // npt=2: one interval [x[0], x[1]].  The while loop does not execute
    // (jup - jlow - 1 = 0), so the function returns jlow=0 immediately
    // for any in-range value.
    const int   npt = 2;
    const float x[] = { 1.0f, 5.0f };

    SECTION("lower endpoint returns 0") {
        CHECK(Locate(npt, x, 1.0f) == 0);
    }

    SECTION("midpoint returns 0") {
        CHECK(Locate(npt, x, 3.0f) == 0);
    }

    SECTION("upper endpoint returns 0") {
        CHECK(Locate(npt, x, 5.0f) == 0);
    }
}

TEST_CASE("Locate large ascending array — spot-check indices", "[Locate]") {
    // 11-point array (0, 1, 2, ..., 10) to exercise deeper bisection.
    const int npt = 11;
    const float x[] = { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f,
                        6.f, 7.f, 8.f, 9.f, 10.f };

    SECTION("value 0.5 returns 0") {
        CHECK(Locate(npt, x, 0.5f) == 0);
    }

    SECTION("value 4.5 returns 4") {
        CHECK(Locate(npt, x, 4.5f) == 4);
    }

    SECTION("value 5.0 (exact node) returns 4") {
        // x[5]=5.0 is an exact node; bisection assigns it to interval [4,5].
        CHECK(Locate(npt, x, 5.0f) == 4);
    }

    SECTION("value 5.5 returns 5") {
        CHECK(Locate(npt, x, 5.5f) == 5);
    }

    SECTION("value 9.9 returns 9") {
        CHECK(Locate(npt, x, 9.9f) == 9);
    }

    SECTION("upper endpoint 10.0 returns npt-2 = 9") {
        CHECK(Locate(npt, x, 10.0f) == 9);
    }
}

// ===========================================================================
// FTPC geometry constants — documentation
//
// These values describe the physical FTPC detector.  They are loaded from
// the offline database at runtime and are NOT available as compile-time
// constants.  The standard values are:
//
//   numberOfPadrowsPerSide = 10   (padrows per East/West half)
//   numberOfPadrows        = 20   (total: padrows 1-10 West, 11-20 East)
//   numberOfSectors        =  6   (sectors per padrow, 60° per sector)
//   numberOfPads           = 182  (pads per sector per padrow)
//   numberOfTimebins       = 256 or 512 (set by ftpcElectronics table)
//   geantSector range      = [1, 6]   (from StFtpcGeantReader::geantSector)
//   geantPlane  range      = [1, 20]  (from StFtpcGeantReader::geantPlane)
//
// Verification requires a live StFtpcDbReader; see the integration stub below.
// ===========================================================================

// ===========================================================================
// Integration stubs
//
// These tests require TApplication + a fully-initialised STAR framework
// (StChain, St_db_Maker, offline database, g2t tables).
// Exclude them from standard unit runs with:
//   ./test_StFtpcSlowSimMaker ~[integration]
// ===========================================================================

TEST_CASE("StFtpcSlowSimMaker Init stub",
          "[StFtpcSlowSimMaker][integration]") {
    // Init() calls GetMaker("db") to locate St_db_Maker and then fetches all
    // FTPC calibration and geometry database tables:
    //   ftpcSlowSimGas, ftpcSlowSimPars, ftpcClusterPars, ftpcDimensions,
    //   ftpcAsicMap, ftpcEField, ftpcVDrift, ftpcDeflection, ftpcdVDriftdP,
    //   ftpcdDeflectiondP, ftpcAmpSlope, ftpcAmpOffset, ftpcTimeOffset,
    //   ftpcDriftField, ftpcGas, ftpcElectronics, ftpcInnerCathode, ftpcTemps.
    // Returns kStWarn when the database is unavailable.
    //
    // Integration test outline:
    //   StChain chain;
    //   St_db_Maker *db = new St_db_Maker("db","MySQL:StarDb","$STAR/StarDb");
    //   StFtpcSlowSimMaker *maker = new StFtpcSlowSimMaker("ftpc_raw");
    //   chain.AddMaker(db);
    //   chain.AddMaker(maker);
    //   REQUIRE(chain.Init() == kStOK);
    SUCCEED("Init integration test — not run in unit environment");
}

TEST_CASE("StFtpcSlowSimMaker InitRun stub",
          "[StFtpcSlowSimMaker][integration]") {
    // InitRun(runNumber) reads ftpcElectronics to set microsecondsPerTimebin
    // (either from the RHIC clock frequency or the database default),
    // constructs StFtpcTrackingParams, and initialises StFtpcDbReader.
    // Returns kStOK on success.
    //
    // Integration test outline:
    //   REQUIRE(chain.Init()            == kStOK);
    //   REQUIRE(maker->InitRun(15000001) == kStOK);
    //   // microsecondsPerTimebin is typically ~0.200 µs/bin (200 ns/bin)
    SUCCEED("InitRun integration test — not run in unit environment");
}

TEST_CASE("StFtpcSlowSimMaker Make stub — no g2t_ftp_hit table",
          "[StFtpcSlowSimMaker][integration]") {
    // Make() calls GetDataSet("geant") to obtain the g2t_ftp_hit table.
    // When the table is absent it logs a warning and returns kStWarn.
    //
    // Integration test outline:
    //   REQUIRE(chain.Make() == kStWarn); // no geant hit table
    SUCCEED("Make (no g2t_ftp_hit) integration test — not run in unit environment");
}

TEST_CASE("StFtpcSlowSimMaker Make stub — with g2t hits",
          "[StFtpcSlowSimMaker][integration]") {
    // With a populated g2t_ftp_hit table on the chain, Make() should:
    //   1. Construct StFtpcDbReader with all calibration tables.
    //   2. Construct StFtpcParamReader from database and StarDb code tables.
    //   3. Wrap the g2t hit table in StFtpcGeantReader.
    //   4. Create StFtpcRawWriter to receive digitised ADC output.
    //   5. Run StFtpcSlowSimulator::simulate():
    //        - build StFtpcSlowSimField (radial E-field + drift velocity grid),
    //        - loop over hits: drift-diffuse, avalanche, pad/shaper response,
    //          digitise into the ADC array, write via StFtpcRawWriter.
    //   6. Return kStOK.
    //
    // Integration test outline:
    //   REQUIRE(chain.Make() == kStOK);
    SUCCEED("Make (full event) integration test — not run in unit environment");
}

TEST_CASE("StFtpcSlowSimMaker geometry constants via StFtpcDbReader",
          "[StFtpcSlowSimMaker][integration]") {
    // After Init() + InitRun(), the StFtpcDbReader constructed inside Make()
    // exposes the standard FTPC geometry parameters loaded from the database:
    //
    //   CHECK(db.numberOfPadrowsPerSide() == 10);
    //   CHECK(db.numberOfPadrows()        == 20);
    //   CHECK(db.numberOfSectors()        ==  6);
    //   CHECK(db.numberOfPads()           == 182);
    //   // numberOfTimebins is 256 or 512 depending on ftpcElectronics
    //
    // The geant-volume decoders in StFtpcGeantReader satisfy:
    //   geantPlane(volumeId)  in [1, 20]
    //   geantSector(volumeId) in [1,  6]
    SUCCEED("Geometry constants integration test — not run in unit environment");
}

TEST_CASE("StFtpcSlowSimMaker microsecondsPerTimebin stub",
          "[StFtpcSlowSimMaker][integration]") {
    // The protected data member microsecondsPerTimebin (Float_t) is set in
    // InitRun() and used during Make() to convert drift-time bins to µs.
    // Its value is typically ~0.200 µs/bin (RHIC clock: ~9.4 MHz gives
    // 200 ns per 25/4-MHz sample).
    //
    // Because microsecondsPerTimebin is protected with no public accessor,
    // it can only be verified via a subclass or a friend in integration tests.
    //
    // TODO: add a public accessor
    //   Float_t GetMicrosecondsPerTimebin() const {return microsecondsPerTimebin;}
    // then test:
    //   REQUIRE(maker.InitRun(15000001) == kStOK);
    //   CHECK(maker.GetMicrosecondsPerTimebin() == Approx(0.200f).margin(0.01f));
    SUCCEED("microsecondsPerTimebin integration test — not run in unit environment");
}

TEST_CASE("Locate out-of-range behaviour",
          "[Locate][integration]") {
    // When xx lies outside [x[0], x[npt-1]] (ascending) or outside
    // [x[npt-1], x[0]] (descending), Locate() emits LOG_WARN via gMessMgr
    // and returns 0.  Calling it in a bare unit context without a live
    // gMessMgr instance is unsafe (null pointer dereference on the logger).
    //
    // Integration test outline (with StMessMgr initialised by StChain):
    //   const float x[] = { 1.f, 2.f, 3.f };
    //   CHECK(Locate(3, x, 0.f) == 0);  // below ascending range
    //   CHECK(Locate(3, x, 4.f) == 0);  // above ascending range
    SUCCEED("Locate out-of-range integration test — requires live StMessMgr");
}
