// tests/unit/StBTofSimMaker/test_StBTofSimMaker.cxx
// Unit tests for StBTofSimMaker — the Barrel TOF simulation maker.
//
// StBTofSimMaker inherits from StMaker (TDataSet/TObject).  The constructor
// only sets member defaults and calls Reset() (which zeroes the hit-flag
// array); no TApplication, database, or g2t tables are required for the
// structural tests below.
//
// CalcCellId() is protected; a thin test subclass exposes it together with
// the protected flags and the TrackHit struct so they can be verified without
// modifying production code.
//
// Tests that require StChain, database back-ends, or g2t tables are tagged
// [integration] and kept as commented-out stubs at the bottom of this file.

#include "../catch_amalgamated.hpp"

#include "StBTofSimMaker/StBTofSimMaker.h"

// ─────────────────────────────────────────────────────────────────────────────
// TestableBTofSimMaker
// Thin subclass that opens access to the protected members exercised by the
// unit tests.  No production behaviour is changed.
// ─────────────────────────────────────────────────────────────────────────────
class TestableBTofSimMaker : public StBTofSimMaker {
public:
    explicit TestableBTofSimMaker(const char* name = "TofSim")
        : StBTofSimMaker(name) {}

    // Expose CalcCellId as a public wrapper returning std::vector<int>.
    std::vector<int> calcCellId(int volume_id, float ylocal) {
        return CalcCellId(volume_id, ylocal);
    }

    // Expose protected default-flag accessors.
    bool bookHisto()    const { return mBookHisto;    }
    bool writeStEvent() const { return mWriteStEvent; }
    bool isEmbedding()  const { return mIsEmbedding;  }
    bool cellXtalk()    const { return mCellXtalk;    }
    bool slowSim()      const { return mSlow;          }
    bool useVpdStart()  const { return mUseVpdStart;  }
    bool vpdSim()       const { return mVpdSim;        }

    // Bring the protected TrackHit struct into the public interface so tests
    // can value-initialise and inspect it.
    using TrackHitType = TrackHit;

    // Expose the geometry enum constants used inside CalcCellId.
    static constexpr int kNTray   = mNTray;    // 120
    static constexpr int kNModule = mNModule;  // 32
    static constexpr int kNCell   = mNCell;    //  6

    // Expose the pad-width constant (static const float, defined in .cxx).
    static float padWidth() { return mBTofPadWidth; }  // 3.45 cm
};

// ─────────────────────────────────────────────────────────────────────────────
// CalcCellId helper — volume_id encoding
//
// The GEANT volume_id encodes three fields:
//
//   rileft   = volume_id / 100000          (1 = west half, 2 = east half)
//   itray_h  = (volume_id % 100000) / 1000 (tray index within the half)
//   imodule  = (volume_id % 1000)   / 10   (module index, 1-32)
//
// Full tray id: itray = itray_h + (rileft - 1) * 60   (1-120)
// Cell id from ylocal (cm): int((ylocal + PadWidth*NCell/2) / PadWidth) + 1
//
// The helper below constructs volume_ids from human-readable components.
// ─────────────────────────────────────────────────────────────────────────────
static int makeVolumeId(int rileft, int itray_half, int imodule) {
    return rileft * 100000 + itray_half * 1000 + imodule * 10;
}

// ─────────────────────────────────────────────────────────────────────────────
// Construction and naming
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StBTofSimMaker construction with explicit name",
          "[StBTofSimMaker]") {
    TestableBTofSimMaker maker("myBTofSim");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "myBTofSim");
    }
}

TEST_CASE("StBTofSimMaker default construction",
          "[StBTofSimMaker]") {
    TestableBTofSimMaker maker;

    SECTION("GetName is non-null") {
        REQUIRE(maker.GetName() != nullptr);
    }

    SECTION("default name is 'TofSim'") {
        CHECK(std::string(maker.GetName()) == "TofSim");
    }
}

TEST_CASE("StBTofSimMaker GetCVS returns non-empty string",
          "[StBTofSimMaker]") {
    TestableBTofSimMaker maker;

    SECTION("GetCVS is non-null") {
        REQUIRE(maker.GetCVS() != nullptr);
    }

    SECTION("GetCVS contains the class name") {
        std::string cvs(maker.GetCVS());
        CHECK_FALSE(cvs.empty());
        CHECK(cvs.find("StBTofSimMaker") != std::string::npos);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Default constructor flags
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StBTofSimMaker default flag values after construction",
          "[StBTofSimMaker][defaults]") {
    TestableBTofSimMaker maker;

    SECTION("mBookHisto defaults to false") {
        CHECK(maker.bookHisto() == false);
    }

    SECTION("mWriteStEvent defaults to true") {
        CHECK(maker.writeStEvent() == true);
    }

    SECTION("mIsEmbedding defaults to false (pure simulation)") {
        // kSimulation == kFALSE
        CHECK(maker.isEmbedding() == false);
    }

    SECTION("mCellXtalk defaults to true") {
        CHECK(maker.cellXtalk() == true);
    }

    SECTION("mSlow defaults to false (fast simulation)") {
        CHECK(maker.slowSim() == false);
    }

    SECTION("mUseVpdStart defaults to true") {
        CHECK(maker.useVpdStart() == true);
    }

    SECTION("mVpdSim defaults to false") {
        CHECK(maker.vpdSim() == false);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Public getter defaults (null pointers before Init/InitRun)
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StBTofSimMaker public getters return null before Init",
          "[StBTofSimMaker][defaults]") {
    TestableBTofSimMaker maker;

    SECTION("GetSimParam returns null before InitRun") {
        CHECK(maker.GetSimParam() == nullptr);
    }

    SECTION("GetBTofCollection returns null before Make") {
        CHECK(maker.GetBTofCollection() == nullptr);
    }

    SECTION("GetMcBTofHitCollection returns null before Make") {
        CHECK(maker.GetMcBTofHitCollection() == nullptr);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Public setter round-trips
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StBTofSimMaker setEmbeddingMode round-trip",
          "[StBTofSimMaker][setters]") {
    TestableBTofSimMaker maker;

    SECTION("setEmbeddingMode(true) is reflected by getEmbeddingMode()") {
        maker.setEmbeddingMode(true);
        CHECK(maker.getEmbeddingMode() == true);
    }

    SECTION("setEmbeddingMode(false) is reflected by getEmbeddingMode()") {
        maker.setEmbeddingMode(false);
        CHECK(maker.getEmbeddingMode() == false);
    }

    SECTION("default embedding mode is false") {
        CHECK(maker.getEmbeddingMode() == false);
    }
}

TEST_CASE("StBTofSimMaker setBookHist setter is callable",
          "[StBTofSimMaker][setters]") {
    TestableBTofSimMaker maker;

    SECTION("setBookHist(true) toggles mBookHisto") {
        maker.setBookHist(true);
        CHECK(maker.bookHisto() == true);
    }

    SECTION("setBookHist(false) restores mBookHisto") {
        maker.setBookHist(true);
        maker.setBookHist(false);
        CHECK(maker.bookHisto() == false);
    }
}

TEST_CASE("StBTofSimMaker setCellXtalk setter is callable",
          "[StBTofSimMaker][setters]") {
    TestableBTofSimMaker maker;

    SECTION("setCellXtalk(false) clears the flag") {
        maker.setCellXtalk(false);
        CHECK(maker.cellXtalk() == false);
    }

    SECTION("setCellXtalk(true) sets the flag") {
        maker.setCellXtalk(false);
        maker.setCellXtalk(true);
        CHECK(maker.cellXtalk() == true);
    }
}

TEST_CASE("StBTofSimMaker writeStEvent setter is callable",
          "[StBTofSimMaker][setters]") {
    TestableBTofSimMaker maker;

    SECTION("writeStEvent(false) clears the write-StEvent flag") {
        maker.writeStEvent(false);
        CHECK(maker.writeStEvent() == false);
    }

    SECTION("writeStEvent() with default argument re-enables the flag") {
        maker.writeStEvent(false);
        maker.writeStEvent();
        CHECK(maker.writeStEvent() == true);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Geometry constants accessible through the test subclass
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StBTofSimMaker geometry constants have expected values",
          "[StBTofSimMaker][geometry]") {
    SECTION("mNTray is 120") {
        CHECK(TestableBTofSimMaker::kNTray == 120);
    }

    SECTION("mNModule is 32") {
        CHECK(TestableBTofSimMaker::kNModule == 32);
    }

    SECTION("mNCell is 6") {
        CHECK(TestableBTofSimMaker::kNCell == 6);
    }

    SECTION("mBTofPadWidth is 3.45 cm") {
        CHECK(TestableBTofSimMaker::padWidth() == Catch::Approx(3.45f));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// TrackHit struct — value-initialisation zeroes all fields
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StBTofSimMaker TrackHit struct value-initialises to zero",
          "[StBTofSimMaker][TrackHit]") {
    // TrackHit has no explicit constructor; value-initialisation (= {})
    // zero-initialises every field including the dQdt[600] array.
    TestableBTofSimMaker::TrackHitType hit{};

    SECTION("tray field defaults to 0") {
        CHECK(hit.tray == 0);
    }

    SECTION("module field defaults to 0") {
        CHECK(hit.module == 0);
    }

    SECTION("cell field defaults to 0") {
        CHECK(hit.cell == 0);
    }

    SECTION("trkId field defaults to 0") {
        CHECK(hit.trkId == 0);
    }

    SECTION("dE field defaults to 0.0") {
        CHECK(hit.dE == Catch::Approx(0.0));
    }

    SECTION("dQ field defaults to 0.0") {
        CHECK(hit.dQ == Catch::Approx(0.0));
    }

    SECTION("tof field defaults to 0.0") {
        CHECK(hit.tof == Catch::Approx(0.0));
    }

    SECTION("s_track field defaults to 0.0") {
        CHECK(hit.s_track == Catch::Approx(0.0));
    }

    SECTION("t0 field defaults to 0.0") {
        CHECK(hit.t0 == Catch::Approx(0.0));
    }

    SECTION("dQdt array first element defaults to 0.0") {
        CHECK(hit.dQdt[0] == Catch::Approx(0.0));
    }

    SECTION("dQdt array last element defaults to 0.0") {
        CHECK(hit.dQdt[599] == Catch::Approx(0.0));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// CalcCellId — volume_id decoding and cell-from-ylocal
//
// volume_id encoding (see implementation):
//   rileft   = volume_id / 100000          (1 = west, 2 = east half)
//   itray_h  = (volume_id % 100000) / 1000 (tray within half barrel, 1-60)
//   imodule  = (volume_id % 1000)   / 10   (module, 1-32)
//   itray    = itray_h + (rileft-1)*60      (absolute tray, 1-120)
//
// Cell from ylocal (cm):
//   icell = int((ylocal + PadWidth*NCell/2) / PadWidth) + 1   (1-6)
//   PadWidth = 3.45 cm, NCell = 6  →  half-width = 10.35 cm
//
// Cell boundaries (ylocal):  cell 1 [-10.35, -6.90)
//                             cell 2 [ -6.90, -3.45)
//                             cell 3 [ -3.45,  0.00)
//                             cell 4 [  0.00,  3.45)
//                             cell 5 [  3.45,  6.90)
//                             cell 6 [  6.90, 10.35)
//
// Out-of-range components are clamped to -1 by the implementation.
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("CalcCellId decodes a west-half volume_id correctly",
          "[StBTofSimMaker][CalcCellId]") {
    TestableBTofSimMaker maker;

    // rileft=1 (west), itray_half=5, imodule=12  →  itray=5
    const int volume_id = makeVolumeId(1, 5, 12);  // 105120

    SECTION("tray index is 5 (west half, no offset)") {
        auto id = maker.calcCellId(volume_id, 0.0f);
        REQUIRE(id.size() == 3u);
        CHECK(id[0] == 5);
    }

    SECTION("module index is 12") {
        auto id = maker.calcCellId(volume_id, 0.0f);
        CHECK(id[1] == 12);
    }

    SECTION("ylocal=0 falls in cell 4 (centre of pad 4)") {
        // int((0.0 + 10.35)/3.45) + 1 = int(3.0) + 1 = 4
        auto id = maker.calcCellId(volume_id, 0.0f);
        CHECK(id[2] == 4);
    }
}

TEST_CASE("CalcCellId decodes an east-half volume_id and adds 60-tray offset",
          "[StBTofSimMaker][CalcCellId]") {
    TestableBTofSimMaker maker;

    // rileft=2 (east), itray_half=3, imodule=7  →  itray = 3 + 60 = 63
    const int volume_id = makeVolumeId(2, 3, 7);   // 203070

    SECTION("tray index is 63 (east half: itray_half + 60)") {
        auto id = maker.calcCellId(volume_id, 0.0f);
        REQUIRE(id.size() == 3u);
        CHECK(id[0] == 63);
    }

    SECTION("module index is 7") {
        auto id = maker.calcCellId(volume_id, 0.0f);
        CHECK(id[1] == 7);
    }
}

TEST_CASE("CalcCellId maps ylocal to the correct cell index",
          "[StBTofSimMaker][CalcCellId]") {
    TestableBTofSimMaker maker;
    const int volume_id = makeVolumeId(1, 5, 12);  // valid tray/module

    SECTION("ylocal=-10.0 → cell 1 (lower pad)") {
        // int((-10.0+10.35)/3.45)+1 = int(0.101)+1 = 1
        CHECK(maker.calcCellId(volume_id, -10.0f)[2] == 1);
    }

    SECTION("ylocal=-5.0 → cell 2") {
        // int(( -5.0+10.35)/3.45)+1 = int(1.55)+1 = 2
        CHECK(maker.calcCellId(volume_id, -5.0f)[2] == 2);
    }

    SECTION("ylocal=-2.0 → cell 3") {
        // int(( -2.0+10.35)/3.45)+1 = int(2.42)+1 = 3
        CHECK(maker.calcCellId(volume_id, -2.0f)[2] == 3);
    }

    SECTION("ylocal=0.0 → cell 4") {
        CHECK(maker.calcCellId(volume_id, 0.0f)[2] == 4);
    }

    SECTION("ylocal=5.0 → cell 5") {
        // int((5.0+10.35)/3.45)+1 = int(4.449)+1 = 5
        CHECK(maker.calcCellId(volume_id, 5.0f)[2] == 5);
    }

    SECTION("ylocal=10.0 → cell 6 (upper pad)") {
        // int((10.0+10.35)/3.45)+1 = int(5.899)+1 = 6
        CHECK(maker.calcCellId(volume_id, 10.0f)[2] == 6);
    }
}

TEST_CASE("CalcCellId clamps out-of-range cell to -1",
          "[StBTofSimMaker][CalcCellId]") {
    TestableBTofSimMaker maker;
    const int volume_id = makeVolumeId(1, 5, 12);

    SECTION("ylocal=+15 exceeds upper boundary → cell -1") {
        // int((15.0+10.35)/3.45)+1 = int(7.35)+1 = 8 > 6
        CHECK(maker.calcCellId(volume_id, 15.0f)[2] == -1);
    }

    SECTION("ylocal=-15 exceeds lower boundary → cell -1") {
        // int((-15.0+10.35)/3.45)+1 = int(-1.35)+1 = -1+1 = 0 → ≤0
        CHECK(maker.calcCellId(volume_id, -15.0f)[2] == -1);
    }
}

TEST_CASE("CalcCellId clamps out-of-range module to -1",
          "[StBTofSimMaker][CalcCellId]") {
    TestableBTofSimMaker maker;

    SECTION("imodule=33 exceeds mNModule (32) → module -1") {
        // volume_id = 1*100000 + 5*1000 + 33*10 = 105330
        const int volume_id = makeVolumeId(1, 5, 33);
        auto id = maker.calcCellId(volume_id, 0.0f);
        CHECK(id[1] == -1);
        // tray and cell should still decode
        CHECK(id[0] == 5);
        CHECK(id[2] == 4);
    }
}

TEST_CASE("CalcCellId clamps out-of-range tray to -1",
          "[StBTofSimMaker][CalcCellId]") {
    TestableBTofSimMaker maker;

    SECTION("east-half itray_half=65 → itray=125 > 120 → tray -1") {
        // volume_id = 2*100000 + 65*1000 + 5*10 = 265050
        const int volume_id = makeVolumeId(2, 65, 5);
        auto id = maker.calcCellId(volume_id, 0.0f);
        CHECK(id[0] == -1);
        CHECK(id[1] == 5);
        CHECK(id[2] == 4);
    }
}

TEST_CASE("CalcCellId result vector has exactly three elements",
          "[StBTofSimMaker][CalcCellId]") {
    TestableBTofSimMaker maker;
    const int volume_id = makeVolumeId(1, 10, 15);
    auto id = maker.calcCellId(volume_id, 1.0f);

    SECTION("returns {tray, module, cell} with size 3") {
        REQUIRE(id.size() == 3u);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Integration-test stubs
// These require StChain, database tables (StBTofDaqMap, StTofSimParam),
// a running StVpdSimMaker / StVpdCalibMaker, and g2t hit tables.
// ─────────────────────────────────────────────────────────────────────────────

// TEST_CASE("StBTofSimMaker Init() returns kStOk with minimal chain",
//           "[StBTofSimMaker][integration]") {
//     // StChain chain;
//     // auto* maker = new StBTofSimMaker("TofSim");
//     // chain.AddMaker(maker);
//     // REQUIRE(chain.Init() == kStOk);
// }

// TEST_CASE("StBTofSimMaker Make() returns kStWarn without geant data",
//           "[StBTofSimMaker][integration]") {
//     // Setup chain without g2t_ctf_hit table; expect kStWarn.
// }

// TEST_CASE("StBTofSimMaker Make() populates McBTofHitCollection from g2t hits",
//           "[StBTofSimMaker][integration]") {
//     // Inject synthetic g2t_ctf_hit_st rows; verify the resulting
//     // McBTofHitCollection size matches the number of valid hits.
// }

// TEST_CASE("StBTofSimMaker embedding mode stores hits in BTofCollection",
//           "[StBTofSimMaker][integration]") {
//     // Enable embedding; inject McBTofHit; verify BTofCollection is filled.
// }

// TEST_CASE("StBTofSimMaker Finish() writes histograms when mBookHisto is set",
//           "[StBTofSimMaker][integration]") {
//     // Enable mBookHisto, run Init+Make+Finish; verify the output ROOT file.
// }
