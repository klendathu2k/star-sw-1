/***************************************************************************
 *
 * $Id: StFgtPointCollection.h,v 2.1 2012/04/16 20:20:49 ullrich Exp $
 * Author: S. Gliske, Oct 2011
 *
 ***************************************************************************
 *
 * Description: A collection of StFgtPoint classes for StEvent.
 * Basically a wrapper for an StSPtrVecFgtPoint
 *
 ***************************************************************************
 *
 * $Log: StFgtPointCollection.h,v $
 * Revision 2.1  2012/04/16 20:20:49  ullrich
 * Initial Revision
 *
 *
 **************************************************************************/

#ifndef _ST_FGT_POINT_COLLECTION_H_
#define _ST_FGT_POINT_COLLECTION_H_

/// @file StFgtPointCollection.h
/// @brief Collection of FGT reconstructed 2D points (StFgtPoint) for StEvent.

#include "StObject.h"
#include "StContainers.h"

class StFgtPoint;

/// @brief Container of StFgtPoint objects (reconstructed 2D space points) for the entire FGT.
class StFgtPointCollection : public StObject {
public:
    // constructors
    StFgtPointCollection();
    // StFgtPointCollection( const StFgtPointCollection& other );            ---> use default
    // StFgtPointCollection& operator=( const StFgtPointCollection& other ); ---> use default 
    
    // deconstructor
    ~StFgtPointCollection();
    
    // accessors/modifiers for the underlying vector
    StSPtrVecFgtPoint& getPointVec();
    const StSPtrVecFgtPoint& getPointVec() const;
    
    size_t getNumPoints() const;
    
    // Clear
    void Clear( Option_t *opt = "" );
    
protected:
    // the data member
    StSPtrVecFgtPoint mPointVec; ///< Persistent vector of reconstructed 2D space points.
    
private:   
    ClassDef(StFgtPointCollection,1);
}; 


// inline functions

inline StFgtPointCollection::StFgtPointCollection() : StObject() {
   // nothing else to do
};

inline StSPtrVecFgtPoint& StFgtPointCollection::getPointVec() {
   return mPointVec;
};

inline const StSPtrVecFgtPoint& StFgtPointCollection::getPointVec() const{
   return mPointVec;
};

inline size_t StFgtPointCollection::getNumPoints() const {
   return mPointVec.size();
};

#endif

