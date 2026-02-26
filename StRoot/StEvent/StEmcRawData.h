/***************************************************************************
 *
 * $Id: StEmcRawData.h,v 2.2 2004/07/15 16:36:24 ullrich Exp $
 *
 * Author: Alex Suaide, Mar 2004
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEmcRawData.h,v $
 * Revision 2.2  2004/07/15 16:36:24  ullrich
 * Removed all clone() declerations and definitions. Use StObject::clone() only.
 *
 * Revision 2.1  2004/03/26 21:53:45  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StEmcRawData_hh
#define StEmcRawData_hh

/// @file StEmcRawData.h
/// @brief Raw DAQ data banks for the STAR Electromagnetic Calorimeter (BEMC or EEMC).

#include "StObject.h"
#include "TArrayS.h"

/// @brief Container for the raw DAQ data banks (header and payload) of one STAR EMC (BEMC or EEMC).
class StEmcRawData : public StObject {
public:
    enum {MAXEMCDATABANK=60}; ///< Maximum number of data banks (one per DAQ crate).

    StEmcRawData();
    /// @brief Copy constructor; performs a deep copy of all bank arrays.
    StEmcRawData(const StEmcRawData&);
    ~StEmcRawData();
        
    /// @brief Returns a pointer to the header array for the given bank index.
          unsigned short* header(int); 
    const unsigned short* header(int) const;

    /// @brief Returns a pointer to the data array for the given bank index.
          unsigned short* data(int);
    const unsigned short* data(int) const; 
    
    /// @brief Returns a single header word at the given bank and word index.
          unsigned short  header(int,int);
    const unsigned short  header(int,int) const;
    
    /// @brief Returns a single data word at the given bank and word index.
          unsigned short  data(int,int);
    const unsigned short  data(int,int) const;
    
    /// @brief Returns the number of header words in the given bank.
          int     sizeHeader(int);
    const int     sizeHeader(int) const;
    
    /// @brief Returns the number of data words in the given bank.
          int     sizeData(int);
    const int     sizeData(int) const;
    
    /// @brief Returns the total number of data banks (= MAXEMCDATABANK).
          int     getNBlocks() { return MAXEMCDATABANK;}
    
    /// @brief Allocates header and data arrays for the given bank with specified sizes.
    void          createBank(int, int, int);
    /// @brief Releases the arrays for the given bank.
    void          deleteBank(int);
    
    /// @brief Sets the header array for the given bank from an external buffer.
    void          setHeader(int, unsigned short*);
    /// @brief Sets the data array for the given bank from an external buffer.
    void          setData(int, unsigned short*);
    /// @brief Sets a single header word in the given bank at the given index.
    void          setHeader(int, int, unsigned short);
    /// @brief Sets a single data word in the given bank at the given index.
    void          setData(int, int, unsigned short);

protected:
    TArrayS       mHeader[MAXEMCDATABANK]; ///< Header arrays for each DAQ crate bank.
    TArrayS       mData[MAXEMCDATABANK];   ///< Payload data arrays for each DAQ crate bank.
    
    ClassDef(StEmcRawData,1)
};
#endif
