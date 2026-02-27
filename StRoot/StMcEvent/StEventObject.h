#ifndef STAR_STEVENTOBJECT
#define STAR_STEVENTOBJECT

/// @file StEventObject.h
/// @brief Generic template wrapper that associates a typed data pointer with an StObject base.

//! base class for all StMcEvent objects
#include "StObject.h"

/// @brief Template base class wrapping a typed data pointer (usually a g2t table row pointer)
/// as an StObject so it can participate in ROOT I/O and browser hierarchies.
template <class T>
class StEventObject : public StObject {
  protected:
    T fData; ///< Pointer to the underlying GEANT table row struct
  
  public:
    int operator==(const StEventObject&h) const
    {  return *h.fData == *fData;       }

    int operator!=(const StEventObject&h) const
    {  return !(*this == h);            }
    
    /// @brief Default constructor; initialises fData to its default value.
    StEventObject() : fData() {};
    /// @brief Construct with an explicit data pointer.
    StEventObject(T data) : fData(data) {};
    virtual ~StEventObject() {}
    /// @brief Return the stored data pointer (mutable).
    T  Data()             { return fData;}
    /// @brief Return the stored data pointer (const).
    const T Data() const { return fData;}
    /// @brief Replace the stored data pointer.
    void SetData( T data) { fData = data; }
};

#endif
