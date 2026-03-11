// tests/unit/sim_support/SimTestFixture.h
//
// Minimal StChain fixture for STAR simulator integration-test stubs.
//
// Usage (in an [integration]-tagged TEST_CASE):
//
//   SimTestFixture<StBTofSimMaker> f("btofSim");
//
//   // Optional: inject GEANT input
//   g2t_ctf_hit_st hit{};  hit.volume_id = 100201;  hit.tof = 12.5e-9;
//   f.geant().addTable("g2t_ctf_hit", ...);
//
//   // Optional: inject DB tables
//   tofTotCorr_st corr{};  corr.nBinMax[0] = 60;
//   f.db().inject("Calibrations/tof", "tofTotCorr", &corr, sizeof(corr));
//
//   // Run the chain
//   REQUIRE(f.init()  == kStOK);
//   REQUIRE(f.make()  == kStOK);
//   REQUIRE(f.finish()== kStOK);
//
//   // Inspect maker output
//   StBTofCollection* coll = f.maker().GetBTofCollection();
//   REQUIRE(coll != nullptr);
//
// This fixture is used ONLY in integration stubs (TEST_CASEs tagged
// [integration]).  Structural unit tests construct makers directly on the
// stack without this fixture.
//
// Requires: TApplication instance, full STAR runtime, StChain library.

#ifndef SIM_TEST_FIXTURE_H
#define SIM_TEST_FIXTURE_H

#include "StChain.h"
#include "MockGeantInput.h"
#include "MockDbTable.h"

// ---------------------------------------------------------------------------
// MockDbMaker — minimal StMaker that serves the MockDbDataset as the "db"
// branch so that GetDataBase() calls inside the maker under test resolve to
// injected structs rather than a live MySQL connection.
// ---------------------------------------------------------------------------
#include "StMaker.h"

class MockDbMaker : public StMaker {
public:
    explicit MockDbMaker(MockDbDataset& db)
        : StMaker("MockDb"), mDb(db) {}

    Int_t Make() override { return kStOK; }

    // Override GetDataBase to intercept DB requests from child makers.
    // If the path is found in our mock dataset we return it; otherwise
    // we fall back to the base-class (which will fail gracefully).
    TDataSet* GetDataBase(const char* path, const TDatime* = nullptr) override {
        TDataSet* ds = mDb.find(path);
        return ds ? ds : StMaker::GetDataBase(path);
    }

    ClassDef(MockDbMaker, 0)

private:
    MockDbDataset& mDb;
};

// ---------------------------------------------------------------------------
// SimTestFixture<MakerType>
//
// Template fixture that owns:
//   - StChain
//   - MockDbMaker (child of chain)
//   - MakerType instance (child of chain)
//   - MockGeantDataset (injected as "geant" input)
//   - MockDbDataset    (served by MockDbMaker)
// ---------------------------------------------------------------------------
template<typename MakerType>
class SimTestFixture {
public:
    explicit SimTestFixture(const char* makerName = "SimUnderTest")
        : mChain(new StChain("testChain"))
        , mDbMaker(new MockDbMaker(mDb))
        , mMaker(new MakerType(makerName))
    {
        mChain->AddMaker(mDbMaker);
        mChain->AddMaker(mMaker);
    }

    ~SimTestFixture() {
        // StChain owns its makers; they are deleted with the chain.
        delete mChain;
    }

    // Accessors for the mock data containers — populate these before init().
    MockGeantDataset& geant() { return mGeant; }
    MockDbDataset&    db()    { return mDb;    }

    // The maker under test.
    MakerType& maker() { return *mMaker; }

    // Chain lifecycle wrappers.
    Int_t init(int runNumber = 1) {
        // Inject the mock GEANT dataset into the chain's input tree
        // so GetInputDS("geant") resolves correctly.
        mChain->AddInputData(mGeant.dataset(), "geant");
        return mChain->Init();
    }
    Int_t initRun(int runNumber = 1) { return mChain->InitRun(runNumber); }
    Int_t make()   { return mChain->Make(); }
    Int_t finish() { return mChain->Finish(); }

private:
    StChain*         mChain;
    MockDbMaker*     mDbMaker;
    MakerType*       mMaker;
    MockGeantDataset mGeant;
    MockDbDataset    mDb;

    // Non-copyable
    SimTestFixture(const SimTestFixture&) = delete;
    SimTestFixture& operator=(const SimTestFixture&) = delete;
};

#endif // SIM_TEST_FIXTURE_H
