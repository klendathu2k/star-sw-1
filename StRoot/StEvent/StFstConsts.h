/***************************************************************************
* StFstConsts
* Author: Shenghui Zhang, Sep 2021
* Initial Revision
* Description: Hard-coded constants for the FST detector.
****************************************************************************/

#ifndef StFstConsts_h
#define StFstConsts_h

/// @file StFstConsts.h
/// @brief Hard-coded geometry and electronics constants for the Forward Silicon Tracker (FST).

#include "StEvent/StEnumerations.h"
#include "TMath.h"
const int kFstNumWedges               = 36;  ///< 36 FST wedge
const int kFstNumSensorsPerWedge      = 3;   ///< 3 sensor per one FST Wedge
const int kFstNumInnerSensorsPerWedge = 1;   ///< 1 inner sensor per one FST Wedge
const int kFstNumOuterSensorsPerWedge = 2;   ///< 2 outer sensor per one FST Wedge
const int kFstNumSensors              = 108; ///< 3*36 sensors
const int kFstNumStripsPerInnerSensor = 512; ///< 128*4 strips in each inner sensor
const int kFstNumStripsPerOuterSensor = 256; ///< 64*4 strips in each outer sensor
const int kFstNumWedgePerDisk         = 12;  ///< 12 wedge per one FST Disk
const int kFstNumPhiSegPerWedge       = 128; ///< phi segmentation in r-phi direction per each wedge
const int kFstNumPhiSegPerSensor      = 128; ///< phi segmentation in r-phi direction per each sensor: 128 for sensor 1 | 64 for sensor 2 & 3
const int kFstNumRStripsPerSensor     = 4;   ///< 4 R-strips in r-phi direction per each sensor

/*
 * Hard-coded constants
 */
//electronics constants
const int kFstNumRdos            = 6;     ///< Number of RDO boards (numbered 1–6).
const int kFstNumArmsPerRdo      = 3;     ///< Number of ARM boards per RDO.
const int kFstNumApvsPerArm      = 16;    ///< Number of APV chips per ARM (channels 0–7 and 12–19).
const int kFstNumApvChannels     = 128;   ///< Number of readout channels per APV chip.
const int kFstNumChanPerArm      = 2048;  ///< Total channels per ARM board (16 × 128).
const int kFstNumElecIds         = 36864; ///< Total electronics IDs (8 × 36 × 128); numbered 0–36863.
const int kFstApvsPerWedge       = 8;     ///< Number of APV chips per FST wedge.
const int kFstNumApvs            = 288;   ///< Total APV chips in the FST (36 wedges × 8).
const int kFstNumRStripsPerWedge = 8;     ///< Number of R-strips per FST wedge.
const int kFstNumDisk            = 3;     ///< Number of FST disks.
const int kFstNumWedsPerRdo      = 6;     ///< Number of wedges per RDO.
const int kFstNumWedsPerArm      = 2;     ///< Number of wedges per ARM board.

/// @brief FST strip geometry constants.
const float kFstSensorActiveSizeR             = 5;          ///< Minimum active radius of an FST sensor (cm).
const float kFstStripPitchR                   = 2.875;      ///< Strip pitch in the radial direction, (28-16.5)/4 or (16.5-5)/4 (cm).
const float kFstStripPitchPhi                 = TMath::Pi()*30.0/180.0/kFstNumPhiSegPerSensor; ///< Strip pitch in the azimuthal direction (rad).
const float kFstStripGapPhi                   = TMath::Pi()*1.0/180.0; ///< Azimuthal gap (1°) between the two outer sensors of an FST wedge (rad).
const int   kFstzFilp[kFstNumDisk]            = {1,-1,1};
const int   kFstzDirct[kFstNumWedgePerDisk]   = {1,-1,1,-1,1,-1,1,-1,1,-1,1,-1};
const float kFstphiStart[kFstNumWedgePerDisk] = {2.0, 2.0, 0.0, 12.0, 10.0, 10.0, 8.0, 8.0, 6.0, 6.0, 4.0, 4.0}; // * pi/6
const float kFstphiStop[kFstNumWedgePerDisk]  = {3.0, 1.0, 1.0, 11.0, 11.0,  9.0, 9.0, 7.0, 7.0, 5.0, 5.0, 3.0}; // * pi/6
const float kFstrStart[kFstNumRStripsPerWedge]= {5.000, 7.875, 10.750, 13.625, 16.500, 19.375, 22.250, 25.125}; // in cm
const float kFstrStop[kFstNumRStripsPerWedge] = {7.875, 10.750, 136.25, 16.500, 19.375, 22.250, 25.125, 28.000}; // in cm

//general APV chip constants
const unsigned char kFstNumTimeBins = 9;    ///< Maximum number of ADC time bins sampled per strip (3 or 9 in practice).
const unsigned char kFstDefaultTimeBin = 2; ///< Default time bin index used for FST raw hit charge (2nd bin).
const int kFstMaxAdc                = 4096; ///< Maximum ADC count (12-bit ADC, values 0–4095).

#endif
