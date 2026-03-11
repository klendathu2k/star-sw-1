/// @file StiPxlDetectorBuilder.h
/// @brief Builds STI geometry for the STAR HFT Pixel (PXL) detector.
///
/// @ingroup StiDetectorGeometry
#ifndef StiPxlDetectorBuilder_h
#define StiPxlDetectorBuilder_h

#include "Sti/StiDetectorBuilder.h"

class StPxlDb;


/**
 * This detector builder is responsible for constructing sensitive and inactive
 * Sti volumes describing the material of the PXL detector. The corresponding
 * PXL detector geometry is described in AgML files in StarVMC/Geometry/
 *
 * \author Dmitri Smirnov, BNL
 */
/// @class StiPxlDetectorBuilder
/// @brief Builds STI geometry for the STAR HFT Pixel (PXL) detector.
///
/// Constructs two concentric silicon pixel barrel layers for Sti tracking.
/// @ingroup StiDetectorGeometry
class StiPxlDetectorBuilder : public StiDetectorBuilder
{
public:

   StiPxlDetectorBuilder(bool active, bool buildIdealGeom=true);
   virtual void buildDetectors(StMaker &source);
   virtual void useVMCGeometry();
   const StiDetector* getActiveDetector(int sector, int ladder, int sensorHalf) const;

protected:

   bool         mBuildIdealGeom;
   StPxlDb     *mPxlDb;

private:

   void buildInactiveVolumes();
   static void convertSensor2StiId(int sector, int ladder, int sensorHalf, int& stiRow, int& stiSensor);

   /// Returns a TGeo path to the sensor in the given sector and ladder
   static std::string formTGeoPath(int sector, int ladder, int sensor);
};

#endif
