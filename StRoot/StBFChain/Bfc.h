#ifndef __Bfc_st__
#define __Bfc_st__
#include "TDataSet.h"
#include "TTable.h"
#include "Ttypes.h"
#define STR_OBSOLETE "WARNING *** Option is OBSOLETE ***"
#define USE_BFCTIMESTAMP

/// One row in the Big Full Chain (BFC) option table.
///
/// The full table is defined in BigFullChain.h as a static \c Bfc_st \c BFC[]
/// array and is loaded at runtime by StBFChain::Setup() via the ROOT macro
/// BFC.C.  Each entry describes one named option: the libraries it requires,
/// the C++ maker class it instantiates, the sub-chain it belongs to, and the
/// other options it depends on.
struct Bfc_st {
  Char_t  Key[64];      ///< User-facing option nick name; matched case-insensitively by StBFChain::kOpt()
  Char_t  Name[64];     ///< Maker instance name passed to the maker constructor
  Char_t  Chain[64];    ///< Parent sub-chain name; empty string means the top chain
  Char_t  Opts[256];    ///< Comma-separated dependency option keys; prefix \c - disables an option
  Char_t  Maker[64];    ///< C++ class name to instantiate; empty means this is a pure alias/config token
  Char_t  Libs[256];    ///< Comma-separated shared libraries to load before instantiation
  Char_t  Comment[256]; ///< Human-readable description; also stores the value for \c opt=value syntax
  Char_t  Flag;         ///< Runtime on/off; \c kTRUE when this option is active
};

#ifdef USE_BFCTIMESTAMP
/// Detector-specific database or simulation timestamp.
///
/// Populated when the user specifies options of the form
/// \c dbvYYYYMMDD_detector_realm or \c sdtYYYYMMDD_detector_realm.
struct BFCTimeStamp {
  Int_t     Type;     ///< 1 for DBV (max DB entry-time), 2 for SDT (simulation geometry timestamp)
  Int_t     Date;     ///< Date component in YYYYMMDD format
  Int_t     Time;     ///< Time component in HHmmss format (0 when not specified)
  TString   Detector; ///< Detector tag matching the DB branch name
  TString   Realm;    ///< DB realm, e.g. "Calibrations" or "Geometry"
};
typedef std::vector<BFCTimeStamp> StVecBFCTS;
#endif /*  USE_BFCTIMESTAMP */

/// ROOT TTable container for the Bfc_st BFC option array.
class St_Bfc : public TTable {
 public:
  ClassDefTable(St_Bfc,Bfc_st)
  ClassDef(St_Bfc,1) //C++ container for chain/makers status 
};

#endif /* __Bfc_st__ */
