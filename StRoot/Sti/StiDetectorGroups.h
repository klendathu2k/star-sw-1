/// @file StiDetectorGroups.h
/// @brief Registry of all active StiDetectorGroup objects used during tracking initialisation.
///
/// @ingroup StiDetectorGeometry
#ifndef StiDetectorGroups_H_INCLUDED
#define StiDetectorGroups_H_INCLUDED
#include <vector>
#include "Sti/Base/Named.h"
#include "Sti/Base/Described.h"
class StiGenericDetectorGroup;

/// @class StiDetectorGroups
/// @brief Registry of all active StiDetectorGroup objects used during StiMaker initialisation.
///
/// @ingroup StiDetectorGeometry
class StiDetectorGroups : public Named, public Described, public vector<StiGenericDetectorGroup * >
{
public:
    StiDetectorGroups(const string &name,const string &description) : Named(name), Described(description) {}
    virtual ~StiDetectorGroups() {} 
};
#endif 
