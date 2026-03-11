// tests/unit/sim_support/MockGeantInput.h
//
// Mock GEANT input dataset for STAR simulator unit tests.
//
// Usage (in an integration-test stub):
//
//   MockGeantDataset geant;
//
//   // Populate a hit struct (zero-initialised by default):
//   g2t_ctf_hit_st hit{};
//   hit.volume_id = 12345;
//   hit.tof       = 10.2e-9;   // seconds
//   hit.de        = 2.0e-3;    // GeV
//   geant.addTable("g2t_ctf_hit", new St_g2t_ctf_hit("g2t_ctf_hit", 1));
//   static_cast<St_g2t_ctf_hit*>(geant.table("g2t_ctf_hit"))->AddAt(&hit);
//
//   // The dataset root is passed to the maker via:
//   maker.AddInputData(geant.dataset(), "geant");
//
// Structural unit tests (construction, setters, pure-math functions) do NOT
// need this header — it is used only in integration stubs.
//
// The g2t Table classes (St_g2t_ctf_hit, etc.) are generated at build time
// from IDL definitions in pams/.  They are only available in a full STAR
// build environment; integration stubs that include this header therefore
// require STAR headers on the include path.

#ifndef MOCK_GEANT_INPUT_H
#define MOCK_GEANT_INPUT_H

#include "TDataSet.h"
#include "TTable.h"
#include <map>
#include <string>

// ---------------------------------------------------------------------------
// MockGeantDataset
//
// Wraps a TDataSet tree that mimics the "geant" branch provided by the BFC.
// Makers call GetInputDS("geant") which returns this dataset root; they then
// call Find("g2t_ctf_hit") etc. to locate individual tables.
//
// Ownership: the dataset and all tables added to it are owned by this object.
// ---------------------------------------------------------------------------
class MockGeantDataset {
public:
    MockGeantDataset() : mDataset(new TDataSet("geant")) {}
    ~MockGeantDataset() { delete mDataset; }

    // Add a pre-populated TTable (e.g. St_g2t_ctf_hit).
    // The name must match the string used in maker code (e.g. "g2t_ctf_hit").
    // Ownership of the table is transferred to the internal dataset.
    void addTable(const char* name, TTable* table) {
        mDataset->Add(table);
        mTableMap[name] = table;
    }

    // Retrieve a previously-added table by name, or nullptr.
    TTable* table(const char* name) const {
        auto it = mTableMap.find(name);
        return (it != mTableMap.end()) ? it->second : nullptr;
    }

    // Returns the root TDataSet* — pass this to maker.AddInputData(ds,"geant").
    TDataSet* dataset() const { return mDataset; }

private:
    TDataSet*                   mDataset;
    std::map<std::string,TTable*> mTableMap;

    // Non-copyable
    MockGeantDataset(const MockGeantDataset&) = delete;
    MockGeantDataset& operator=(const MockGeantDataset&) = delete;
};

// ---------------------------------------------------------------------------
// Per-hit-type builder helpers.
// Each function creates a named table with one row whose fields are set from
// the supplied struct, adds it to the dataset, and returns the table pointer.
//
// Example:
//   g2t_ctf_hit_st hit{};
//   hit.volume_id = 100201;
//   hit.tof = 12.5e-9;
//   addCtfHit(geant, hit);
//
// These helpers are defined as template functions so this header can be
// compiled independently of the specific g2t table types (which are only
// available in a full STAR environment).  Instantiate them in the .cxx file
// that includes the appropriate Table headers.
// ---------------------------------------------------------------------------

// Generic: add one row of any TTable-derived type.
// TableType must be a TTable subclass whose RowClass is RowStruct.
template<typename TableType, typename RowStruct>
TableType* addOneRow(MockGeantDataset& geant, const char* name, const RowStruct& row) {
    TableType* t = new TableType(name, 1);
    t->AddAt(&row);
    geant.addTable(name, t);
    return t;
}

// Convenience wrappers — call these in integration stubs after including the
// appropriate Table header (e.g. "tables/St_g2t_ctf_hit_Table.h"):
//
//   g2t_ctf_hit_st h{}; h.volume_id = 12345;
//   addCtfHit(geant, h);
//
// #define addCtfHit(geant, hit)  addOneRow<St_g2t_ctf_hit,g2t_ctf_hit_st>(geant,"g2t_ctf_hit",hit)
// #define addTpcHit(geant, hit)  addOneRow<St_g2t_tpc_hit,g2t_tpc_hit_st>(geant,"g2t_tpc_hit",hit)
// #define addEmcHit(geant, hit)  addOneRow<St_g2t_emc_hit,g2t_emc_hit_st>(geant,"g2t_emc_hit",hit)
// #define addSvtHit(geant, hit)  addOneRow<St_g2t_svt_hit,g2t_svt_hit_st>(geant,"g2t_svt_hit",hit)
// #define addSsdHit(geant, hit)  addOneRow<St_g2t_ssd_hit,g2t_ssd_hit_st>(geant,"g2t_ssd_hit",hit)
// #define addFgtHit(geant, hit)  addOneRow<St_g2t_fgt_hit,g2t_fgt_hit_st>(geant,"g2t_fgt_hit",hit)
// #define addFtsHit(geant, hit)  addOneRow<St_g2t_fts_hit,g2t_fts_hit_st>(geant,"g2t_fts_hit",hit)
// #define addIstHit(geant, hit)  addOneRow<St_g2t_ist_hit,g2t_ist_hit_st>(geant,"g2t_ist_hit",hit)
// #define addPmdHit(geant, hit)  addOneRow<St_g2t_pmd_hit,g2t_pmd_hit_st>(geant,"g2t_pmd_hit",hit)
// #define addMtdHit(geant, hit)  addOneRow<St_g2t_mtd_hit,g2t_mtd_hit_st>(geant,"g2t_mtd_hit",hit)
// #define addFtpHit(geant, hit)  addOneRow<St_g2t_ftp_hit,g2t_ftp_hit_st>(geant,"g2t_ftp_hit",hit)
// #define addVpdHit(geant, hit)  addOneRow<St_g2t_vpd_hit,g2t_vpd_hit_st>(geant,"g2t_vpd_hit",hit)
// #define addTrack(geant, trk)   addOneRow<St_g2t_track,  g2t_track_st  >(geant,"g2t_track",   trk )
// #define addVertex(geant, vtx)  addOneRow<St_g2t_vertex, g2t_vertex_st >(geant,"g2t_vertex",  vtx )

#endif // MOCK_GEANT_INPUT_H
