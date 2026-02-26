/**
 * \class StGmtStripCollection
 * 
 * Collection of GMT strips for StEvent. Basically a wrapper 
 * for an StSPtrVecGmtStrip (based on StFgtStripCollection)
 *
 * \author K.S. Engle, Jan. 2013
 * \author Richard Witt (witt@usna.edu), Jan. 2013
 * \author Grigory Nigmatkulov (nigmatkulov@gmail.com), Dec. 2020
 */

#ifndef StGmtStripCollection_hh
#define StGmtStripCollection_hh
/// @file StGmtStripCollection.h
/// @brief Collection of GMT strips for a single module of the Global Muon Tracker.

// STAR headers
#include "StObject.h"
#include "StContainers.h"
#include "StGmtStrip.h"

//________________
class StGmtStripCollection : public StObject {
 public:
  /// Constructer
  StGmtStripCollection( short module = 0 );
    
  /// Destructor
  ~StGmtStripCollection();
    
  // WARNING: never use getStripVec().push_back() or equivalents. 
  // Instead use StGmtStripCollection::getStrip to add a new strip.

  /// @brief Returns a reference to the internal vector of strips.
  StSPtrVecGmtStrip& getStripVec()             { return mStripVec; }
  /// @brief Returns a const reference to the internal vector of strips.
  const StSPtrVecGmtStrip& getStripVec() const { return mStripVec; }

    /// @brief Sorts the internal strip vector by geoId.
    void sortByGeoId();

    /// @brief Sorts the internal strip vector by coordinate number.
    void sortByCoord();
    /// @brief Partially sorts the internal strip vector by coordinate number.
    void partialSortByCoord();

    /// @brief Sorts the internal strip vector by layer (X strips first, then Y).
    void sortByLayer();

    /// @brief Removes all strips with negative geoIds (flagged as invalid).
    void removeFlagged();

    /// @brief Returns the number of strips in the collection.
    size_t getNumStrips() const;

    /// @brief Returns the module index.
    short getModule() const;
    /// @brief Sets the module index.
    void setModule( short module );

    /// @brief Clears the collection.
    void Clear( Option_t *opt = "" );

    /// @brief Returns a pointer to the strip with the given geoId; creates it if absent.
    /// Ownership is retained by this collection.
    StGmtStrip* getStrip( int Id );
    /// @brief Returns a pointer to a strip located via a sorted search by geoId.
    StGmtStrip* getSortedStrip( int Id );
    
 protected:  
  /// Function used for sorting strips by geoId
  static bool hitGeoIdLessThan( const StGmtStrip* h1, const StGmtStrip* h2 ); 
  /// Function used for sorting strips by coordinate number
  static bool hitCoordLessThan( const StGmtStrip* h1, const StGmtStrip* h2 ); 
  /// Function used for sorting strips by X then Y
  static bool hitLayerLessThan( const StGmtStrip* h1, const StGmtStrip* h2 ); 
     
  /// Module ID
  Short_t mModule;
  /// Vector with strips
  StSPtrVecGmtStrip mStripVec;    
    
  /// Temporary copy of the pointers, indexed by ElecId 
  /// used for the addStripInfo class
  StPtrVecGmtStrip mStripElecIdVec;
  /// Temporary copy of the pointers, indexed by GeoId 
  /// used for the addStripInfo class
  StPtrVecGmtStrip mStripGeoIdVec; 
    
 private:   
  ClassDef(StGmtStripCollection,1)
}; 

inline size_t StGmtStripCollection::getNumStrips() const {
    return mStripVec.size();
};

inline void StGmtStripCollection::setModule( short moduleId ) {
    mModule = moduleId;
};

inline short StGmtStripCollection::getModule() const {
    return mModule;
};

#endif // #define StGmtStripCollection_hh
