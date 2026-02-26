// tests/unit/StBFChain/test_StBFChain.cxx
//
// Structural unit tests for the C++ classes in StRoot/StBFChain.
//
// These tests exercise the parts of StBFChain that do not require a full
// STAR/ROOT event-loop runtime:
//
//   - Bfc_st struct   — field layout and size invariants.
//   - BFCTimeStamp    — construction and field access.
//   - StBFChain       — construction, simple getters, SetTFile/GetTFile,
//                       ParseString (static, Sort=kFALSE).
//   - StBFChainOpt    — construction and delegation to StBFChain.
//
// Methods that require Setup() (gInterpreter, fBFC table, full maker chain)
// are integration-level concerns and are not tested here.  The DEFECTS.md
// file in this directory documents known issues.

#include "../catch_amalgamated.hpp"

#include "Bfc.h"
#include "StBFChain.h"
#include "StBFChainOpt.h"

#include "TObjArray.h"
#include "TObjString.h"

// ===========================================================================
// Helpers
// ===========================================================================

/// Collect the string values of all entries in a TObjArray of TObjStrings.
static std::vector<std::string> toStrings(const TObjArray &arr) {
    std::vector<std::string> out;
    out.reserve(arr.GetEntries());
    for (int i = 0; i < arr.GetEntries(); ++i) {
        auto *s = static_cast<TObjString *>(arr.At(i));
        if (s) out.emplace_back(s->GetName());
    }
    return out;
}

// ===========================================================================
// Bfc_st struct
// ===========================================================================

TEST_CASE("Bfc_st field sizes match Bfc.h declarations", "[Bfc_st]") {
    SECTION("Key field is 64 bytes") {
        REQUIRE(sizeof(Bfc_st::Key) == 64);
    }
    SECTION("Name field is 64 bytes") {
        REQUIRE(sizeof(Bfc_st::Name) == 64);
    }
    SECTION("Chain field is 64 bytes") {
        REQUIRE(sizeof(Bfc_st::Chain) == 64);
    }
    SECTION("Opts field is 256 bytes") {
        REQUIRE(sizeof(Bfc_st::Opts) == 256);
    }
    SECTION("Maker field is 64 bytes") {
        REQUIRE(sizeof(Bfc_st::Maker) == 64);
    }
    SECTION("Libs field is 256 bytes") {
        REQUIRE(sizeof(Bfc_st::Libs) == 256);
    }
    SECTION("Comment field is 256 bytes") {
        REQUIRE(sizeof(Bfc_st::Comment) == 256);
    }
}

TEST_CASE("Bfc_st value-initialisation", "[Bfc_st]") {
    Bfc_st row{};
    SECTION("Key starts as empty string") {
        REQUIRE(row.Key[0] == '\0');
    }
    SECTION("Flag starts as zero") {
        REQUIRE(row.Flag == 0);
    }
    SECTION("Opts starts as empty string") {
        REQUIRE(row.Opts[0] == '\0');
    }
}

TEST_CASE("Bfc_st field assignment round-trip", "[Bfc_st]") {
    Bfc_st row{};
    std::strncpy(row.Key,   "db",        sizeof(row.Key)   - 1);
    std::strncpy(row.Maker, "St_db_Maker", sizeof(row.Maker) - 1);
    row.Flag = 1;

    SECTION("Key round-trips") {
        REQUIRE(std::string(row.Key) == "db");
    }
    SECTION("Maker round-trips") {
        REQUIRE(std::string(row.Maker) == "St_db_Maker");
    }
    SECTION("Flag round-trips") {
        REQUIRE(row.Flag == 1);
    }
}

// ===========================================================================
// BFCTimeStamp struct
// ===========================================================================

TEST_CASE("BFCTimeStamp default construction", "[BFCTimeStamp]") {
    BFCTimeStamp ts{};
    SECTION("Type initialises to zero") {
        REQUIRE(ts.Type == 0);
    }
    SECTION("Date initialises to zero") {
        REQUIRE(ts.Date == 0);
    }
    SECTION("Time initialises to zero") {
        REQUIRE(ts.Time == 0);
    }
    SECTION("Detector is empty TString") {
        REQUIRE(ts.Detector.IsNull());
    }
    SECTION("Realm is empty TString") {
        REQUIRE(ts.Realm.IsNull());
    }
}

TEST_CASE("BFCTimeStamp field assignment", "[BFCTimeStamp]") {
    BFCTimeStamp ts{};
    ts.Type     = 1;
    ts.Date     = 20140101;
    ts.Time     = 120000;
    ts.Detector = "tpc";
    ts.Realm    = "Calibrations";

    SECTION("Type round-trips") {
        REQUIRE(ts.Type == 1);
    }
    SECTION("Date round-trips") {
        REQUIRE(ts.Date == 20140101);
    }
    SECTION("Time round-trips") {
        REQUIRE(ts.Time == 120000);
    }
    SECTION("Detector round-trips") {
        REQUIRE(std::string(ts.Detector.Data()) == "tpc");
    }
    SECTION("Realm round-trips") {
        REQUIRE(std::string(ts.Realm.Data()) == "Calibrations");
    }
}

// ===========================================================================
// StBFChain — construction and simple getters
// ===========================================================================

TEST_CASE("StBFChain construction with explicit name", "[StBFChain]") {
    StBFChain bfc("mychain");
    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(bfc.GetName()) == "mychain");
    }
}

TEST_CASE("StBFChain default construction", "[StBFChain]") {
    StBFChain bfc;
    SECTION("default name is 'bfc'") {
        REQUIRE(std::string(bfc.GetName()) == "bfc");
    }
}

TEST_CASE("StBFChain initial file state", "[StBFChain]") {
    StBFChain bfc("test_initial");

    SECTION("GetFileIn returns empty string before Setup") {
        REQUIRE(bfc.GetFileIn().IsNull());
    }
    SECTION("GetFileOut returns empty string before Setup") {
        REQUIRE(bfc.GetFileOut().IsNull());
    }
    SECTION("GetTFile returns nullptr before Setup") {
        REQUIRE(bfc.GetTFile() == nullptr);
    }
}

TEST_CASE("StBFChain SetTFile / GetTFile round-trip", "[StBFChain]") {
    StBFChain bfc("test_tfile");
    TFile *sentinel = reinterpret_cast<TFile *>(0x1234);
    bfc.SetTFile(sentinel);
    SECTION("GetTFile returns value set by SetTFile") {
        REQUIRE(bfc.GetTFile() == sentinel);
    }
}

TEST_CASE("StBFChain::GetCVS returns non-null and contains class name", "[StBFChain]") {
    StBFChain bfc("test_cvs");
    const char *cvs = bfc.GetCVS();
    SECTION("GetCVS is non-null") {
        REQUIRE(cvs != nullptr);
    }
    SECTION("GetCVS string contains 'StBFChain'") {
        REQUIRE(std::string(cvs).find("StBFChain") != std::string::npos);
    }
}

// ===========================================================================
// StBFChain::ParseString — static tokeniser (Sort=kFALSE, no chain required)
// ===========================================================================

TEST_CASE("ParseString single token", "[StBFChain][ParseString]") {
    TObjArray out;
    int n = StBFChain::ParseString("db", out, kFALSE);
    REQUIRE(n == 1);
    REQUIRE(toStrings(out) == std::vector<std::string>{"db"});
}

TEST_CASE("ParseString comma-separated tokens", "[StBFChain][ParseString]") {
    TObjArray out;
    int n = StBFChain::ParseString("db,tpcDB,Sti", out, kFALSE);
    REQUIRE(n == 3);
    auto v = toStrings(out);
    REQUIRE(v[0] == "db");
    REQUIRE(v[1] == "tpcDB");
    REQUIRE(v[2] == "Sti");
}

TEST_CASE("ParseString space-separated tokens", "[StBFChain][ParseString]") {
    TObjArray out;
    int n = StBFChain::ParseString("db tpcDB Sti", out, kFALSE);
    REQUIRE(n == 3);
    auto v = toStrings(out);
    REQUIRE(v[0] == "db");
    REQUIRE(v[1] == "tpcDB");
    REQUIRE(v[2] == "Sti");
}

TEST_CASE("ParseString semicolon-separated tokens", "[StBFChain][ParseString]") {
    TObjArray out;
    int n = StBFChain::ParseString("db;tpcDB;Sti", out, kFALSE);
    REQUIRE(n == 3);
    auto v = toStrings(out);
    REQUIRE(v[0] == "db");
    REQUIRE(v[1] == "tpcDB");
    REQUIRE(v[2] == "Sti");
}

TEST_CASE("ParseString preserves negation-prefix tokens", "[StBFChain][ParseString]") {
    TObjArray out;
    int n = StBFChain::ParseString("P2014a,-tofDat", out, kFALSE);
    REQUIRE(n == 2);
    auto v = toStrings(out);
    REQUIRE(v[0] == "P2014a");
    REQUIRE(v[1] == "-tofDat");
}

TEST_CASE("ParseString empty string yields zero tokens", "[StBFChain][ParseString]") {
    TObjArray out;
    int n = StBFChain::ParseString("", out, kFALSE);
    REQUIRE(n == 0);
    REQUIRE(out.GetEntries() == 0);
}

TEST_CASE("ParseString clears previous content of output array", "[StBFChain][ParseString]") {
    TObjArray out;
    StBFChain::ParseString("a,b,c", out, kFALSE);
    StBFChain::ParseString("x", out, kFALSE);
    SECTION("array has exactly one entry after second call") {
        REQUIRE(out.GetEntries() == 1);
    }
    SECTION("entry is the new token") {
        REQUIRE(toStrings(out)[0] == "x");
    }
}

// ===========================================================================
// StBFChainOpt — construction and delegation
// ===========================================================================

TEST_CASE("StBFChainOpt construction", "[StBFChainOpt]") {
    StBFChain bfc("opt_chain");
    StBFChainOpt opt(&bfc);
    SECTION("GetName returns 'StBFChainOpt'") {
        REQUIRE(std::string(opt.GetName()) == "StBFChainOpt");
    }
}

TEST_CASE("StBFChainOpt::GetFileIn delegates to StBFChain", "[StBFChainOpt]") {
    StBFChain bfc("opt_filein");
    StBFChainOpt opt(&bfc);
    SECTION("GetFileIn matches bfc.GetFileIn()") {
        REQUIRE(opt.GetFileIn() == bfc.GetFileIn());
    }
    SECTION("GetFileIn is empty before Setup") {
        REQUIRE(opt.GetFileIn().IsNull());
    }
}

TEST_CASE("StBFChainOpt::GetFileOut delegates to StBFChain", "[StBFChainOpt]") {
    StBFChain bfc("opt_fileout");
    StBFChainOpt opt(&bfc);
    SECTION("GetFileOut matches bfc.GetFileOut()") {
        REQUIRE(opt.GetFileOut() == bfc.GetFileOut());
    }
    SECTION("GetFileOut is empty before Setup") {
        REQUIRE(opt.GetFileOut().IsNull());
    }
}

TEST_CASE("StBFChainOpt::GetTFile delegates to StBFChain", "[StBFChainOpt]") {
    StBFChain bfc("opt_tfile");
    StBFChainOpt opt(&bfc);
    SECTION("GetTFile returns nullptr before Setup") {
        REQUIRE(opt.GetTFile() == nullptr);
    }
    SECTION("GetTFile matches bfc.GetTFile()") {
        REQUIRE(opt.GetTFile() == bfc.GetTFile());
    }
}

TEST_CASE("StBFChainOpt::GetTFile reflects SetTFile on chain", "[StBFChainOpt]") {
    StBFChain bfc("opt_tfile2");
    StBFChainOpt opt(&bfc);
    TFile *sentinel = reinterpret_cast<TFile *>(0xDEAD);
    bfc.SetTFile(sentinel);
    SECTION("GetTFile sees the value set on the underlying chain") {
        REQUIRE(opt.GetTFile() == sentinel);
    }
}
