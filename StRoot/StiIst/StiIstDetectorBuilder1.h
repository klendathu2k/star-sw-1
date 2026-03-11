/// @file StiIstDetectorBuilder1.h
/// @brief Earlier version of the IST detector builder, retained for backward compatibility.
///
/// @ingroup StiDetectorGeometry
#ifndef StiIstDetectorBuilder1_h
#define StiIstDetectorBuilder1_h

#include "StiIst/StiIstDetectorBuilder.h"

class StiMaterial;
class StiPlacement;


/// @class StiIstDetectorBuilder1
/// @brief Earlier version of the IST detector builder; retained for backward compatibility.
///
/// Superseded by StiIstDetectorBuilder.
/// @deprecated Use StiIstDetectorBuilder instead.
/// @ingroup StiDetectorGeometry
class StiIstDetectorBuilder1 : public StiIstDetectorBuilder
{
public:

   StiIstDetectorBuilder1(bool active, bool buildIdealGeom = true);

protected:

   virtual void buildInactiveVolumes();

private:

   void buildPlanerVolume(StiDetector& detector, std::string detName, float halfDepth, float thickness, float halfWidth,
         float yShift, float rShift, float zShift, StiPlacement *placement, StiMaterial *mat);
   void buildTubeVolume(StiDetector& detector, std::string detName, float halfDepth, float thickness,
         float outerRadius, float openingAngle, float zCenter, StiMaterial *mat);
};

#endif
