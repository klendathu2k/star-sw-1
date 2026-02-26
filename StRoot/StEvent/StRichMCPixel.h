/*!
 * \class StRichMCPixel 
 * \author Brian Lasiuk, May 2000
 *
 *    MC pixel contains  the raw pixel info but also
 *
 */
/***************************************************************************
 *
 * $Id: StRichMCPixel.h,v 2.3 2002/02/22 22:56:49 jeromel Exp $
 *
 * Author: Brian Lasiuk, May 2000
 ***************************************************************************
 *
 * Description:
 *   MC pixel contains  the raw pixel info but also
 *
 ***************************************************************************
 *
 * $Log: StRichMCPixel.h,v $
 * Revision 2.3  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:40  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  2000/05/22 21:44:44  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRichMCPixel_hh
#define StRichMCPixel_hh

/// @file StRichMCPixel.h
/// @brief Monte Carlo RICH pixel, extending StRichPixel with simulation truth contributions.

#include "StRichPixel.h"
#include "StRichMCInfo.h"
#include "StContainers.h"

/// @brief Monte Carlo RICH pad pixel, augmenting the raw pixel data with per-particle truth records.
class StRichMCPixel : public StRichPixel {
public:
    /// @brief Default constructor.
    StRichMCPixel();
    /// @brief Constructor from a packed 32-bit pixel data word.
    StRichMCPixel(unsigned int packedData);
    /// @brief Constructor from packed data and a vector of MC truth records.
    StRichMCPixel(unsigned int packedData, const StSPtrVecRichMCInfo&);
    // StRichMCPixel(const StRichMCPixel&);            use default
    // StRichMCPixel& operator=(const StRichMCPixel&); use default
    ~StRichMCPixel();

    int operator==(const StRichMCPixel&) const;
    int operator!=(const StRichMCPixel&) const;

    /// @brief Returns the number of Monte Carlo particles contributing to this pixel.
    unsigned short contributions() const;

    /// @brief Appends a Monte Carlo truth record to this pixel.
    void addInfo(const StRichMCInfo*);
    /// @brief Replaces the full vector of Monte Carlo truth records.
    void setInfo(const StSPtrVecRichMCInfo&);

    /// @brief Returns a const reference to the vector of MC truth records.
    const StSPtrVecRichMCInfo& getMCInfo() const;
    /// @brief Returns a mutable reference to the vector of MC truth records.
    StSPtrVecRichMCInfo&       getMCInfo();

protected:
    StSPtrVecRichMCInfo  mInfo;  ///< Monte Carlo truth records for all contributing particles

    ClassDef(StRichMCPixel,1)
};

inline unsigned short StRichMCPixel::contributions() const { return mInfo.size(); }
inline void StRichMCPixel::addInfo(const StRichMCInfo* p) { mInfo.push_back(p);}
inline void StRichMCPixel::setInfo(const StSPtrVecRichMCInfo& p) { mInfo = p;}
inline const StSPtrVecRichMCInfo& StRichMCPixel::getMCInfo() const {return mInfo;}
inline StSPtrVecRichMCInfo& StRichMCPixel::getMCInfo() { return mInfo;}

#endif
