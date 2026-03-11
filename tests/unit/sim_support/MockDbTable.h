// tests/unit/sim_support/MockDbTable.h
//
// Mock database table injection for STAR simulator unit tests.
//
// Provides MockDbDataset: a TDataSet tree that mimics the STAR database
// directory hierarchy so that makers can call GetDataBase("Calibrations/tof")
// (or similar) and receive a populated TDataSet containing the requested
// table — without connecting to any external database server.
//
// Usage (in an integration-test stub):
//
//   MockDbDataset db;
//
//   // Populate a DB struct (zero-initialised by default):
//   tofTotCorr_st corr{};
//   corr.nBinMax[0] = 60;
//   corr.t0[0]      = 0.5;
//   db.inject("Calibrations/tof", "tofTotCorr", &corr, sizeof(tofTotCorr_st));
//
//   // Inject the DB root into the maker chain before InitRun:
//   chain.AddData(db.root(), "db");
//   // -- OR -- override the maker's GetDataBase() via a MockDbMaker (see SimTestFixture.h)
//
// Structural unit tests (construction, setters, pure-math functions) do NOT
// need this header.
//
// DB struct types (e.g. tofTotCorr_st) are generated at build time from IDL
// definitions under StDb/.  They are only available in a full STAR build
// environment; integration stubs that include this header require STAR DB
// headers on the include path.

#ifndef MOCK_DB_TABLE_H
#define MOCK_DB_TABLE_H

#include "TDataSet.h"
#include "TTable.h"
#include <cassert>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

// ---------------------------------------------------------------------------
// MockDbDataset
//
// Builds a TDataSet subtree that mirrors the STAR DB path hierarchy.
// A path "A/B" results in the tree:
//   root
//   └── A
//       └── B
//           └── <tableName>  (TTable with injected data)
//
// Makers that call GetDataBase("A/B") will receive the "B" node; they then
// call Find("tableName") to locate the TTable and cast its GetTable() pointer.
// ---------------------------------------------------------------------------
class MockDbDataset {
public:
    MockDbDataset() : mRoot(new TDataSet("MockDb")) {}
    ~MockDbDataset() { delete mRoot; }

    // Inject a single DB table row into the given path.
    //
    // @param path      DB directory path, e.g. "Calibrations/tof"
    // @param tableName Table name, e.g. "tofTotCorr"
    // @param data      Pointer to a populated DB struct (POD)
    // @param rowSize   sizeof(the DB struct)
    //
    // This creates a minimal TTable-like dataset node containing a copy of
    // the data.  In a real STAR environment the result of GetDataBase(path)
    // is a TDataSet; makers call ds->Find(tableName) and then cast
    // ((TTable*)result)->GetTable() to the typed struct pointer.
    void inject(const char* path, const char* tableName,
                const void* data, size_t rowSize)
    {
        TDataSet* dir = getOrCreatePath(path);
        // Wrap the raw data in a minimal TTable substitute.
        // MockRawTable stores a copy of the byte array and presents
        // it via an overriding GetTable() accessor.
        MockRawTable* t = new MockRawTable(tableName, data, rowSize);
        dir->Add(t);
    }

    // Returns the root TDataSet*.
    TDataSet* root() const { return mRoot; }

    // Retrieve a dataset node by path for direct inspection in tests.
    TDataSet* find(const char* path) const {
        return mRoot->FindByPath(path);
    }

    // -----------------------------------------------------------------------
    // MockRawTable — minimal TTable substitute that stores an arbitrary
    // byte array and returns it from GetTable().
    // Not a real TTable subclass (avoids needing full St_Tables build) but
    // satisfies TDataSet::Find() by being a named child node.
    // -----------------------------------------------------------------------
    class MockRawTable : public TDataSet {
    public:
        MockRawTable(const char* name, const void* data, size_t size)
            : TDataSet(name), mData(size) {
            std::memcpy(mData.data(), data, size);
        }
        // Cast-safe accessor used by test assertions:
        const void* GetTable() const { return mData.data(); }
        void*       GetTable()       { return mData.data(); }
        size_t      GetRowSize() const { return mData.size(); }
    private:
        std::vector<char> mData;
    };

private:
    TDataSet* mRoot;

    // Navigate or create the directory hierarchy under mRoot for the given
    // slash-separated path.  Returns the deepest node.
    TDataSet* getOrCreatePath(const char* path) {
        TDataSet* cur = mRoot;
        std::istringstream ss(path);
        std::string token;
        while (std::getline(ss, token, '/')) {
            if (token.empty()) continue;
            TDataSet* child = cur->Find(token.c_str());
            if (!child) {
                child = new TDataSet(token.c_str());
                cur->Add(child);
            }
            cur = child;
        }
        return cur;
    }

    // Non-copyable
    MockDbDataset(const MockDbDataset&) = delete;
    MockDbDataset& operator=(const MockDbDataset&) = delete;
};

// ---------------------------------------------------------------------------
// Convenience typed accessor.
// Cast the MockRawTable's byte buffer to the expected struct type.
// Returns nullptr if the named child is not found or is not a MockRawTable.
//
// Usage:
//   auto* tbl = dbGet<tofTotCorr_st>(db, "Calibrations/tof/tofTotCorr");
//   REQUIRE(tbl != nullptr);
//   CHECK(tbl->nBinMax[0] == 60);
// ---------------------------------------------------------------------------
template<typename StructType>
const StructType* dbGet(const MockDbDataset& db, const char* fullPath) {
    TDataSet* node = db.find(fullPath);
    if (!node) return nullptr;
    auto* raw = dynamic_cast<MockDbDataset::MockRawTable*>(node);
    if (!raw) return nullptr;
    return static_cast<const StructType*>(raw->GetTable());
}

#endif // MOCK_DB_TABLE_H
