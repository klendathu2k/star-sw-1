/// @file StiMasterDetectorBuilder.h
/// @brief Aggregates all sub-detector StiDetectorBuilder instances and builds geometry in sequence.
///
/// @ingroup StiDetectorGeometry
#ifndef StiMasterDetectorBuilder_H
#define StiMasterDetectorBuilder_H
#include <vector>
#include "StiDetectorBuilder.h"

/// @class StiMasterDetectorBuilder
/// @brief Aggregates all sub-detector StiDetectorBuilder instances; buildDetectors() calls each sub-builder in turn.
///
/// @ingroup StiDetectorGeometry
class StiMasterDetectorBuilder : public StiDetectorBuilder, public vector<StiDetectorBuilder*>
{
public:
    StiMasterDetectorBuilder(bool active);
    virtual ~StiMasterDetectorBuilder(); 
    virtual bool hasMore() const;
    virtual StiDetector * next();
    virtual void reset();
    virtual void unset(){;}
    virtual void build(StMaker&source);
    virtual void add(StiDetectorBuilder *builder);
		virtual StiDetectorBuilder * get(const string & name);
    virtual StiMaterial * add(StiMaterial *material)   {return StiDetectorBuilder::add(material);}
    virtual StiShape    * add(StiShape    *shape)      {return StiDetectorBuilder::add(shape);}
    virtual StiDetector * add(StiDetector *detector)   {return StiDetectorBuilder::add(detector);}
    virtual StiDetector * add(unsigned int row, unsigned int sector, StiDetector *detector)
			{return StiDetectorBuilder::add(row,sector,detector);}
};

#endif 
