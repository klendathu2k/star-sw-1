/***************************************************************************
 *
 * $Id: StMcEmcHitCollection.hh,v 2.11 2018/03/15 22:00:35 smirnovd Exp $
 * $Log: StMcEmcHitCollection.hh,v $
 * Revision 2.11  2018/03/15 22:00:35  smirnovd
 * Fix linker error by removing declared but undefined functions
 *
 * Revision 2.10  2012/06/11 14:51:46  fisyak
 * std namespace
 *
 * Revision 2.9  2012/03/22 00:33:58  perev
 * private => protected
 *
 * Revision 2.8  2005/11/22 21:44:51  fisyak
 * Add compress Print for McEvent, add Ssd collections
 *
 * Revision 2.7  2005/09/28 21:30:14  fisyak
 * Persistent StMcEvent
 *
 * Revision 2.6  2005/06/28 18:06:41  fine
 * Remove the redundant data-member StMcEmcModuleHitCollection mModules[mNumberOfModules] causing the crash duw double destruction of one and the same object
 *
 * Revision 2.5  2005/01/27 23:40:47  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.4  2001/05/31 02:45:55  perev
 * const(ing)
 *
 * Revision 2.3  2001/05/13 21:12:10  calderon
 * Modifications by Aleksei to the Emc Hit Collections on indexing of
 * module numbers
 *
 * Revision 2.2  2000/08/30 14:52:03  calderon
 * New changes made by Aleksei.
 *
 *
 **************************************************************************/
/// @file StMcEmcHitCollection.hh
/// @brief Top-level container for Monte Carlo EMC (BEMC and EEMC) hits.
#ifndef StMcEmcHitCollection_hh
#define StMcEmcHitCollection_hh

#include "StMcEmcModuleHitCollection.hh"

class StMcCalorimeterHit;

/// @brief Top-level Monte Carlo hit collection for the EMC (BEMC and EEMC).
///
/// Organises StMcCalorimeterHit objects into a two-level hierarchy:
/// this collection owns one StMcEmcModuleHitCollection per detector module.
class StMcEmcHitCollection : public TDataSet {
 public:
  enum  EAddHit {kNull, kErr, kNew, kAdd};
  enum {mNumberOfModules=120};
  /// @brief Default constructor.
  StMcEmcHitCollection();
  /// @brief Constructor with a non-const name string.
  StMcEmcHitCollection(char*);
  /// @brief Constructor with a name string.
  StMcEmcHitCollection(const char*);
  /// @brief Destructor.
  virtual ~StMcEmcHitCollection();
  
  /// @brief Adds a calorimeter hit to the appropriate module sub-collection.
  StMcEmcHitCollection::EAddHit  addHit(StMcCalorimeterHit*);
  /// @brief Returns the total number of hits across all modules.
  unsigned long numberOfHits() const;
  /// @brief Returns the number of modules in this collection.
  unsigned int numberOfModules() const { return mNumberOfModules; }
  /// @brief Returns the summed energy deposit across all hits (GeV).
  float    sum() const;
  
  /// @brief Returns a pointer to the sub-collection for the given module index.
  StMcEmcModuleHitCollection*       module(unsigned int m);
  /// @brief Returns a const pointer to the sub-collection for the given module index.
  const StMcEmcModuleHitCollection* module(unsigned int m) const;
  
  /// @brief Returns a reference to the sub-collection for the given module index.
  StMcEmcModuleHitCollection&       thisModule(unsigned int m) {
    TObjArray &modules = *GetObjArray();
    return *(StMcEmcModuleHitCollection *)modules[m];
  }
  /// @brief Returns a const reference to the sub-collection for the given module index.
  const StMcEmcModuleHitCollection& thisModule(unsigned int m) const {
    TObjArray &modules = *GetObjArray();
    return *(const StMcEmcModuleHitCollection *)modules[m];
  }
  
  /// @brief Returns true; this collection is a folder in the ROOT browser.
  virtual Bool_t IsFolder() const {return kTRUE;} // It is a directory for modules 
  /// @brief Populates the ROOT browser with module sub-collections.
  virtual void Browse(TBrowser *b);
  /// @brief Prints a summary of hits to standard output.
  void    print();
  /// @brief Prints a summary of hits (ROOT menu entry).
  virtual void Print(Option_t *option="") const; // *MENU* 
  
 protected:
  /// @brief Initialises the module sub-collections within the TDataSet.
  void MakeHitCollection();
 protected:
  ClassDef(StMcEmcHitCollection,1)
};

std::ostream&  operator<<(std::ostream& os, const StMcEmcHitCollection&);
#endif
