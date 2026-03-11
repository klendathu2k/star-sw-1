/// @file StiPlanarShape.h
/// @brief Rectangular planar shape: half-widths in y and z, thickness.
///
/// @ingroup StiDetectorGeometry
// Class to represent a shape within the STAR geometry
// Ben Norman, Kent State
// 25 July 01

#ifndef STI_PLANAR_SHAPE_H
#define STI_PLANAR_SHAPE_H

#include "StiShape.h"

/// @class StiPlanarShape
/// @brief Flat rectangular pad shape defined by half-widths in y and z and a thickness.
///
/// Used for silicon strip detectors and endcap detectors.
/// @ingroup StiDetectorGeometry
class StiPlanarShape: public StiShape
{
 public:
  
  // constructor
  StiPlanarShape(): StiShape(), _halfWidth(0.){}
  StiPlanarShape(const string &name, float halfDepth, float thickness, float halfWidth);
  
  // accessors
virtual  float getHalfWidth() 	const { return _halfWidth;}	//deltaY
virtual  float getOpeningAngle()const { return 999       ;} 
  StiShapeCode getShapeCode() 	const { return kPlanar   ;}
double getVolume() 	const;


  // mutators
  void setHalfWidth(float val);
  
 protected:
  /// Half extent in local y, always >= 0
  float _halfWidth;  
};

#endif
