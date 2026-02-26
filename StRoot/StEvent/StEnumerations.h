/*!
 * \file StEnumerations.h
 */
/***************************************************************************
 *
 * $Id: StEnumerations.h,v 2.70 2021/01/11 20:27:40 ullrich Exp $
 *
 * Author: Thomas Ullrich, Jan 1999
 ***************************************************************************
 *
 * Description: Enumerations for StEvent.
 *              Note that lots of code depend on this file and
 *              any change will force a recompilation of almost
 *              all of StEvent.
 *
 ***************************************************************************
 *
 * $Log: StEnumerations.h,v $
 * Revision 2.70  2021/01/11 20:27:40  ullrich
 * Updated to add FCS elements (Akio).
 *
 * Revision 2.69  2020/01/27 21:28:31  genevb
 * Add short tracks toward ETOF
 *
 * Revision 2.68  2019/02/11 18:51:31  ullrich
 * Added EToF.
 *
 * Revision 2.67  2018/12/11 19:53:10  ullrich
 * Added RICHf.
 *
 * Revision 2.66  2018/04/30 23:18:10  smirnovd
 * [Cosmetic] Minor changes in various files
 *
 * - Renamed data member s/m/mMass/ in StikalmanTrack
 * - Changes in white space
 * - Return STAR code
 *
 * Revision 2.65  2018/03/27 02:40:12  genevb
 * Introduce kiTpcId
 *
 * Revision 2.64  2017/05/04 00:54:13  perev
 * Fts added
 *
 * Revision 2.63  2016/12/19 16:11:14  ullrich
 * Added FPost detector ID and StFpostConstants enums.
 *
 * Revision 2.62  2016/02/25 17:10:20  ullrich
 * Implemented detector() which is now a pure abstract method in StHit.
 *
 * Revision 2.61  2015/12/24 00:14:44  fisyak
 * Add GMT and SST Id and new dE/dx method
 *
 * Revision 2.60  2015/09/14 16:58:13  ullrich
 * Added items to StFmsDetectorId enumeration
 *
 * Revision 2.59  2015/09/01 18:29:01  ullrich
 * Changes due to adding StFpsSlat and interconnection between slats and points.
 *
 * Revision 2.58  2015/05/13 17:06:13  ullrich
 * Added hooks and interfaces to Sst detector (part of HFT).
 *
 * Revision 2.57  2015/02/14 18:57:24  ullrich
 * Big upgrade after adding StFmPoint and StFmsCluster.
 *
 * Revision 2.56  2014/04/10 14:35:26  ullrich
 * Add Ist constants.
 *
 * Revision 1.1.1.1  2013/07/23 14:13:30  fisyak
 *
 *
 * Revision 2.54  2013/07/16 14:29:03  fisyak
 * Restore mass fit tracks
 *
 * Revision 2.53  2013/05/07 19:30:21  jeromel
 * No change but add markers
 *
 * Revision 2.52  2013/04/10 19:15:52  jeromel
 * Step back from StEvent changes - previous change recoverable [Thomas OK-ed]
 *
 * Revision 2.50  2013/01/15 23:18:12  fisyak
 * Add more bits for track quality matching to EMC
 *
 * Revision 2.49  2013/01/08 18:56:03  ullrich
 * Changed value of kFgtNumTimeBins
 *
 * Revision 2.48  2012/11/05 18:18:45  ullrich
 * Add 3 new members to StFgtClusterSeedType.
 *
 * Revision 2.47  2012/09/16 21:34:48  fisyak
 * Add Tpc West Only and East only bits
 *
 * Revision 2.46  2012/07/30 14:41:52  ullrich
 * Added 2 new variables to enum StFgtClusterSeedType
 *
 * Revision 2.45  2012/07/21 03:31:37  perev
 *  Increase Track Range 300=>500
 *
 * Revision 2.44  2012/05/07 14:42:57  fisyak
 * Add handilings for Track to Fast Detectors Matching
 *
 * Revision 2.43  2012/04/27 01:32:13  perev
 * Tracking now is not TPC only
 *
 * Revision 2.42  2012/04/16 20:22:16  ullrich
 * Changes necessary to add Fgt package.
 *
 * Revision 2.41  2012/02/21 18:46:21  perev
 * max numeber of detid ==40
 *
 * Revision 2.40  2012/02/21 16:25:47  fisyak
 * add KFVertexFinder
 *
 * Revision 2.39  2012/01/24 03:00:42  perev
 * Etr detector added
 *
 * Revision 2.38  2011/04/25 21:25:09  ullrich
 * Modifications to hold MTD data.
 *
 * Revision 2.37  2009/11/23 22:22:25  ullrich
 * Minor cleanup performed and hooks for RPS added.
 *
 * Revision 2.36  2009/04/28 13:36:32  ullrich
 * Updated PWG enumarations.
 *
 * Revision 2.35  2009/04/06 19:23:53  ullrich
 * Add detector Ids for FPD East/West and FMS.
 *
 * Revision 2.34  2007/10/11 21:50:19  ullrich
 * Added new enums for PXL and IST detectors.
 *
 * Revision 2.33  2007/02/22 20:44:13  ullrich
 * Added l2DisplacedVertex to StL2AlgorithmId.
 *
 * Revision 2.32  2006/08/15 14:32:58  ullrich
 * Added kHpdId.
 *
 * Revision 2.31  2006/05/10 15:13:17  jeromel
 * Additional enum
 *
 * Revision 2.30  2006/05/04 19:05:05  ullrich
 * Added StL2TriggerResultType.
 *
 * Revision 2.29  2006/04/26 14:41:22  jeromel
 * doxygenized
 *
 * Revision 2.28  2006/04/25 23:20:31  ullrich
 * Added StPrimaryVertexOrder.
 *
 * Revision 2.27  2006/03/22 20:57:19  ullrich
 * Added StL2AlgorithmId enumeration.
 *
 * Revision 2.26  2006/03/12 17:00:15  jeromel
 * Added ppvNoCtbVertexFinder to hopefully clarify analysis
 *
 * Revision 2.25  2006/01/19 22:30:05  jeromel
 * kMaxId -> kMaxDetectorId
 *
 * Revision 2.24  2006/01/19 21:51:26  ullrich
 * Added new RnD detectors.
 *
 * Revision 2.23  2005/08/17 21:57:29  perev
 * kMaxId == max number of detectors+1
 *
 * Revision 2.22  2005/06/15 21:46:21  ullrich
 * Added StVertexFinderId enum.
 *
 * Revision 2.21  2004/11/03 18:30:07  ullrich
 * Added definitions for FTPC calibration vertices.
 *
 * Revision 2.20  2004/10/20 18:51:56  ullrich
 * Add enumerations from StDedxMethod.h StDetectorId.h StTrackMethod.h StVertexId.h.
 *
 * Revision 2.19  2004/10/14 19:59:42  ullrich
 * Added enum for EMC crate status.
 *
 * Revision 2.18  2004/04/29 14:05:00  fisyak
 * Add track finding method Id defines (for StTrackMethod.inc and sync. with StEnumerations)
 *
 * Revision 2.17  2002/04/18 23:22:41  jeromel
 * Changes for SVT 2 tables scheme.
 *
 * Revision 2.16  2002/02/27 18:56:27  ullrich
 * Added enum for l3 track finder.
 *
 * Revision 2.15  2002/02/25 19:32:03  ullrich
 * Added enums for RHIC ring to StBeamDirection.
 *
 * Revision 2.14  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.13  2001/04/05 04:00:35  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.12  2001/03/14 02:33:37  ullrich
 * Fixed typo.
 *
 * Revision 2.11  2001/03/14 02:30:20  ullrich
 * Added StPwg enum.
 *
 * Revision 2.10  2000/11/26 15:07:55  lasiuk
 * replace multiplyassigned bit for RICH
 *
 * Revision 2.9  2000/11/25 11:48:40  lasiuk
 * Modify the StRichHitFlags to account for ambiguities
 *
 * Revision 2.8  2000/11/01 16:42:19  lasiuk
 * add StRichHitFlag for PID info
 *
 * Revision 2.7  2000/09/28 10:57:13  ullrich
 * Added enums related to RICH PID.
 *
 * Revision 2.6  2000/08/28 16:28:45  ullrich
 * Added enum tpt and removed CINT pragmas.
 *
 * Revision 2.5  2000/08/28 17:08:58  didenko
 * get back revision 2.2
 *
 * Revision 2.2  1999/12/01 15:58:05  ullrich
 * New decoding for dst_track::method. New enum added.
 *
 * Revision 2.1  1999/11/15 18:48:16  ullrich
 * Adapted new enums for dedx and track reco methods.
 *
 * Revision 2.0  1999/10/12 18:41:45  ullrich
 * Completely Revised for New Version
 *
 **************************************************************************/
#ifndef StEnumerations_hh
#define StEnumerations_hh

// These include files contain definitions that are also used in
// Fortran code. The orginal definitions are implemented via pre-
// processor #define statements. For compatibility reasons we have
// to leave it as is for now.
#include "Rtypes.h"
#include "StDetectorDefinitions.h"
#include "StDedxDefinitions.h"
#include "StVertexDefinitions.h"
#include "StTrackDefinitions.h"

/*!
 * \enum StBeamDirection
 * \brief Labels the two RHIC beam directions / beam colours.
 */
enum StBeamDirection {east   = 0,    ///< East direction (yellow beam travels west→east)
                      yellow = 0,    ///< Yellow beam travels west→east (alias for east)
                      west   = 1,    ///< West direction (blue beam travels east→west)
                      blue   = 1};   ///< Blue beam travels east→west (alias for west)

/*!
 * \enum StBeamPolarizationAxis
 * \brief Orientation of the beam spin-polarization axis.
 */
enum StBeamPolarizationAxis {transverse,   ///< Polarization axis transverse to the beam direction
                              longitudinal};///< Polarization axis along the beam direction

/*!
 * \enum StChargeSign
 * \brief Sign of a particle's electric charge.
 */
enum StChargeSign {negative, ///< Negatively charged particle
                   positive}; ///< Positively charged particle

/*!
 * \enum StDetectorId
 */
enum StDetectorId {kUnknownId   = kUnknownIdentifier,
                   kTpcId       = kTpcIdentifier,
                   kSvtId       = kSvtIdentifier,
                   kRichId      = kRichIdentifier,
                   kFtpcWestId  = kFtpcWestIdentifier,
                   kFtpcEastId  = kFtpcEastIdentifier,
                   kTofId       = kTofIdentifier,
                   kBTofId      = kTofIdentifier,
                   kCtbId       = kCtbIdentifier,
                   kSsdId       = kSsdIdentifier,
                   kBarrelEmcTowerId     = kBarrelEmcTowerIdentifier,
                   kBarrelEmcPreShowerId = kBarrelEmcPreShowerIdentifier,
                   kBarrelSmdEtaStripId  = kBarrelSmdEtaStripIdentifier,
                   kBarrelSmdPhiStripId  = kBarrelSmdPhiStripIdentifier,
                   kEndcapEmcTowerId     = kEndcapEmcTowerIdentifier,
                   kEndcapEmcPreShowerId = kEndcapEmcPreShowerIdentifier,
                   kEndcapSmdUStripId    = kEndcapSmdUStripIdentifier,
                   kEndcapSmdVStripId    = kEndcapSmdVStripIdentifier,
                   kZdcWestId   = kZdcWestIdentifier,
                   kZdcEastId   = kZdcEastIdentifier,
                   kMwpcWestId  = kMwpcWestIdentifier,
                   kMwpcEastId  = kMwpcEastIdentifier,
                   kTpcSsdId    = kTpcSsdIdentifier,
                   kTpcSvtId    = kTpcSvtIdentifier,
                   kTpcSsdSvtId = kTpcSsdSvtIdentifier,
                   kSsdSvtId    = kSsdSvtIdentifier,
                   kPhmdCpvId   = kPhmdCpvIdentifier,
                   kPhmdId      = kPhmdIdentifier,
                   kPxlId       = kPxlIdentifier,
                   kIstId       = kIstIdentifier,
                   kFgtId       = kFgtIdentifier,
                   kEtrId       = kEtrIdentifier,
                   kFpdWestId   = kFpdWestIdentifier,
                   kFpdEastId   = kFpdEastIdentifier, 
                   kFmsId       = kFmsIdentifier,  
                   kRpsId       = kRpsIdentifier,
                   kMtdId       = kMtdIdentifier,
                   kSstId       = kSstIdentifier,
                   kGmtId       = kGmtIdentifier,
                   kFtsId       = kFtsIdentifier,
                   kiTpcId      = kiTpcIdentifier,
                   kETofId      = kETofIdentifier,
		           kFcsWcalId   = kFcsWcalIdentifier,
		           kFcsHcalId   = kFcsHcalIdentifier,
		           kFcsPresId   = kFcsPresIdentifier,
                   kFttId       = kFttIdentifier,
                   kFstId       = kFstIdentifier,
                   kRHICfId     = kRHICfIdentifier,
                   kMaxDetectorId = 47};

/*!
 * \enum StTrackType
 * \brief Classification of a reconstructed track by its reconstruction scope.
 */
enum StTrackType {global,      ///< Global track fitted to all hits, not constrained to any vertex
                  primary,     ///< Primary track re-fitted with primary vertex constraint
                  tpt,         ///< Track produced by the TPT (fast online tracker)
                  secondary,   ///< Secondary (decay-daughter) track
                  estGlobal,   ///< Global track from the EST (fast silicon+TPC) tracker
                  estPrimary,  ///< Primary track from the EST tracker
                  massFit,     ///< Global track re-fitted with a mass hypothesis
                  massFitAtVx  ///< Mass-fit track additionally constrained to the vertex
                 };

/*!
 * \enum StTrackModel
 * \brief Parametric model used to describe the track trajectory.
 */
enum StTrackModel {helixModel,   ///< Simple helix (constant-field approximation)
                   kalmanModel}; ///< Full Kalman-filter track model

/*!
 * \enum StTrackFinderMethod
 * \brief Identifies the track-finding algorithm (finder) used during reconstruction.
 */
enum StTrackFinderMethod {svtGrouper    = ksvtGrouperIdentifier,  ///< SVT Grouper finder
                          svtStk        = ksvtStkIdentifier,       ///< SVT Stk finder
                          svtOther      = ksvtOtherIdentifier,     ///< SVT other finder
                          tpcStandard   = ktpcStandardIdentifier,  ///< TPC standard (tpt/CA) finder
                          tpcOther      = ktpcOtherIdentifier,     ///< TPC alternative finder
                          ftpcConformal = kftpcConformalIdentifier,///< FTPC conformal mapping finder
                          ftpcCurrent   = kftpcCurrentIdentifier,  ///< FTPC current-sheet finder
                          svtTpcSvm     = ksvtTpcSvmIdentifier,    ///< SVT-TPC vector–vector matcher
                          svtTpcEst     = ksvtTpcEstIdentifier,    ///< SVT-TPC TPC-vector/SVT-point matcher
                          svtTpcPattern = ksvtTpcPatternIdentifier,///< SVT-TPC pattern finder
                          l3Standard    = kl3StandardIdentifier    ///< Level-3 online standard finder
};

/*!
 * \enum StDedxMethod
 * \brief Algorithm used to compute ionisation energy loss (dE/dx) from detector signals.
 */
enum StDedxMethod {kUndefinedMethodId        = kUndefinedMethodIdentifier,       ///< Unknown or undefined dE/dx method
                  kTruncatedMeanId           = kTruncatedMeanIdentifier,          ///< Truncated-mean estimator
                  kEnsembleTruncatedMeanId   = kEnsembleTruncatedMeanIdentifier,  ///< Ensemble truncated-mean estimator
                  kLikelihoodFitId           = kLikelihoodFitIdentifier,          ///< Likelihood-fit estimator
                  kWeightedTruncatedMeanId   = kWeightedTruncatedMeanIdentifier,  ///< Weighted truncated-mean estimator
                  kOtherMethodId             = kOtherMethodIdentifier,            ///< Other method (first spare)
                  kOtherMethodId2            = kOtherMethodIdentifier2};          ///< Other method (second spare)

/*!
 * \enum StTrackFittingMethod
 * \brief Algorithm used to fit the track trajectory to the measured hits.
 */
enum StTrackFittingMethod {kUndefinedFitterId         = kUndefinedFitterIdentifier, ///< Unknown or undefined fitter
                           kHelix2StepId              = kHelix2StepIdentifier,      ///< Helix fit: separate bending/non-bending plane fits
                           kHelix3DId                 = kHelix3DIdentifier,         ///< Full 3-D helix fit
                           kKalmanFitId               = kKalmanFitIdentifier,       ///< Kalman filter fit (local helix)
                           kLine2StepId               = kLine2StepIdentifier,       ///< Straight-line fit: separate 2-D plane fits
                           kLine3DId                  = kLine3DIdentifier,          ///< Full 3-D straight-line fit
                           kL3FitId                   = kL3FitIdentifier,           ///< Level-3 online trigger fitter
                           kITKalmanFitId             = kITKalmanFitIdentifier};    ///< ITTF Kalman fit
/*!
  ETrackStatusBits
 */
enum ETrackStatusBits {
  kCtbMatched      = BIT(14),   // track has CTB hit match
  kToFMatched      = BIT(15),   // track has ToF hit match
  kCtbNotMatched   = BIT(16),   // track has CTB prediction but no hit to match (veto)
  kToFNotMatched   = BIT(17),   // track has ToF prediction but no hit to match (veto)
  kBemcMatched     = BIT(18),   // track has     Bemc          hit match
  kEemcMatched     = BIT(19),   // track has     Eemc          hit match
  kBemcNotMatched  = BIT(20),   // track has     Bemc          prediction but no hit to match (veto)
  kEemcNotMatched  = BIT(21),   // track has             Eemc  prediction but no hit to match (veto)
  kDecayTrack      = BIT(22),   // track fit to decay vertex (V0, ...) 
  kPromptTrack     = BIT(23),   // track has prompt TPC hit: |209.4 - |z|| < 3 cm
  kPostXTrack      = BIT(24),   // Post Crossing Track, track which has >= 2 hit with wrong Z / Sector 
  kXMembrane       = BIT(25),   // track which has >= 2 hits from both sides of Membrane (z = 0)
  kShortTrack2EMC  = BIT(26),   // Short track pointing to EEMC
  kRejectedTrack   = BIT(27),   // track rejected by small no. of fit points (< 10)
  kWestTpcOnlyTrack= BIT(28),   // track has hits only from West Tpc (sector <= 12)
  kEastTpcOnlyTrack= BIT(29),   // track has hits only from East Tpc (sector >  12)
  kShortTrack2ETOF = BIT(30),   // Short track pointing to ETOF
  kEmcEnergyByte   =       0    // bits  0- 2: 1 ==> 0.3 < energy <=  0.6 Mip	 
  //                                           2 ==> 0.6 < energy <=  1.0 Hadron 
  // 	                                       3 ==> 1.0 < energy <=  4.0 Electon
  // 	                                       4 ==> 4.0 < energy <= 10.0 Tower	 
  // 	                                       5 ==>10.0 < energy <= 20.0 W	 
  // 	                                       6 ==>20.0 < energy <=100.0 Z	 
  // 	                                       7 ==>       energy > 100.0 ?       
};			     
/*!
 * \enum StVertexId
 * \brief Classification of a reconstructed vertex by its topological origin.
 */
enum StVertexId {kUndefinedVtxId   = kUndefinedVertexIdentifier,             ///< Unknown or undefined vertex type
                 kEventVtxId       = kEventVertexIdentifier,                  ///< Primary collision vertex
		         kPrimaryVtxId     = kEventVertexIdentifier,                  ///< Alias for kEventVtxId
                 kV0VtxId          = kV0DecayIdentifier,                      ///< V0 decay vertex (e.g. K0s, Lambda)
                 kXiVtxId          = kXiDecayIdentifier,                      ///< Xi (cascade) decay vertex
                 kKinkVtxId        = kKinkDecayIdentifier,                    ///< Kink decay vertex
                 kOtherVtxId       = kOtherTypeIdentifier,                    ///< Other / unclassified vertex
                 kFtpcEastCalVtxId = kFtpcEastCalibrationVertexIdentifier,    ///< FTPC east laser calibration vertex
                 kFtpcWestCalVtxId = kFtpcWestCalibrationVertexIdentifier,    ///< FTPC west laser calibration vertex
		         kBEAMConstrVtxId,   ///< Vertex reconstructed with beam-line constraint
                 kRejectedVtxId,    ///< Vertex candidate rejected during vertex finding
                 kFwdVtxId          ///< Forward (FWD tracker) vertex
                 };

/*!
 * \enum StRichPidFlag
 */
enum StRichPidFlag {eNoMip = 1,                                        /**< enum value eNoMip  */
                    eFastEnough = 2,                                   /**< enum value eFastEnough  */
                    eLightOnPadPlane = 4};

/*!
 * \enum StRichHitFlag
 */
enum StRichHitFlag {eDeconvoluted=1,                                   /**< enum value eDeconvoluted */
                    eMip=2,                                            /**< enum value eMip */
                    eSaturatedPad=4 ,                                  /**< enum value eSaturatedPad */
                    ePhotoElectron=8,                                  /**< enum value ePhotoElectron */
                    eAssociatedMip=16,                                 /**< enum value eAssociatedMip */
                    eMultiplyAssigned=32,                              /**< enum value eMultiplyAssigned */
                    eInAreaPi=64,                                      /**< enum value eInAreaPi */
                    eInAreaK=128,                                      /**< enum value eInAreaK */
                    eInAreap=256,                                      /**< enum value eInAreap */
                    eInConstantAnglePi=512,                            /**< enum value eInConstantAnglePi */
                    eInConstantAngleK=1024,                            /**< enum value eInConstantAngleK */
                    eInConstantAnglep=2048,                            /**< enum value eInConstantAnglep */
                    eInConstantAreaPi=4096,                            /**< enum value eInConstantAreaPi */
                    eInConstantAreaK=8192,                             /**< enum value eInConstantAreaK */
                    eInConstantAreap=16384,                            /**< enum value eInConstantAreap */
                    eInMultipleAreaPi=32768,                           /**< enum value eInMultipleAreaPi */
                    eInMultipleAreaK=65536,                            /**< enum value eInMultipleAreaK */
                    eInMultipleAreap=131072,                           /**< enum value eInMultipleAreap */
                    eInMultipleCAnglePi=262144,                        /**< enum value eInMultipleCAnglePi */
                    eInMultipleCAngleK=524288,                         /**< enum value eInMultipleCAngleK */
                    eInMultipleCAnglep=1048576,                        /**< enum value eInMultipleCAnglep */
                    eInMultipleCAreaPi=2097152,                        /**< enum value eInMultipleCAreaPi */
                    eInMultipleCAreaK=4194304,                         /**< enum value eInMultipleCAreaK */
                    eInMultipleCAreap=8388608,                         /**< enum value eInMultipleCAreap */
                    e1SigmaPi=16777216,                                /**< enum value e1SigmaPi */
                    e1SigmaK=33554432,                                 /**< enum value e1SigmaK */
                    e1Sigmap=67108864,                                 /**< enum value e1Sigmap */
                    e2SigmaPi=134217728,                               /**< enum value e2SigmaPi */
                    e2SigmaK=268435456,                                /**< enum value e2SigmaK */
                    e2Sigmap=536870912};

/*!
 * \enum StPwg
 */
enum StPwg         {generic,                                           /**< enum value generic */
                    ebye,                                              /**< enum value for ebye PWG (obsolete) */
                    hbt,                                               /**< enum value for hbt PWG (obsolete) */
                    highpt,                                            /**< enum value for highpt PWG (obsolete) */
                    pcoll,                                             /**< enum value for peripheral collision PWG */
                    upc = pcoll,                                       /**< alias for pcoll */
                    spectra,                                           /**< enum value for spectra PWG (obsolete) */
                    spin,                                              /**< enum value for spin PWG */
                    strangeness,                                       /**< enum value for strangeness PWG (obsolete) */
                    heavy,                                             /**< enum value for heavy flavor PWG */
                    bulkcorr,                                          /**< enum value for bulk correlation PWG */
                    jetcorr,                                           /**< enum value for jet correlation PWG */
                    lfspectra};                                        /**< enum value for light flavor spectra PWG */

/*!
 * \enum StEmcCrateStatus
 * \brief Operational status of an EMC electronics crate for a given event.
 */
enum StEmcCrateStatus {crateUnknown       = 0,  ///< Crate status unknown
                       crateNotPresent    = 1,  ///< Crate absent or powered off
                       crateOK            = 2,  ///< Crate operational and data good
                       crateHeaderCorrupt = 3}; ///< Crate data header corrupted

/*!
 * \enum StarMaxSize
 */
// maximal sizes of tracking part of STAR in cm (Victor)
enum StarMaxTrackRangeSize {kStarMaxTrackRangeR =  500, ///< Maximum radial extent of STAR tracking volume (cm), includes MTD
                            kStarMaxTrackRangeZ =  500, ///< Maximum longitudinal half-length of STAR tracking volume (cm)
                            kStarMinTrackRangeZ = -kStarMaxTrackRangeZ}; ///< Minimum longitudinal extent (cm)

/*!
 * \enum StVertexFinderId
 * \brief Identifies the primary-vertex finding algorithm used for an event.
 */
enum StVertexFinderId { undefinedVertexFinder = 0,  ///< Unknown or unspecified vertex finder
                        lmvVertexFinder,             ///< Low-Multiplicity Vertex finder (LMV)
                        pplmvVertexFinder,           ///< pp LMV vertex finder
                        egrVertexFinder,             ///< EGR (Minuit-based) vertex finder
                        minuitVertexFinder,          ///< Generic Minuit minimisation vertex finder
                        ppvVertexFinder,             ///< Post-Pass Vertex finder (PPV) with CTB
                        ppvNoCtbVertexFinder,        ///< PPV vertex finder without CTB requirement
		    	        mcEventVertexFFinder,        ///< Monte-Carlo truth vertex finder
			            KFVertexFinder};             ///< Kalman Filter vertex finder (KFParticle)


/*!
 * \enum StL2AlgorithmId
 * \brief Identifies the Level-2 trigger algorithm producing a result.
 */
enum StL2AlgorithmId { l2Diagnostic = 0,     ///< Diagnostic / monitoring algorithm
                       l2EmcCheck,            ///< EMC sanity-check algorithm
                       l2Jpsi,               ///< J/ψ → e+e- trigger algorithm
                       l2Upsilon,            ///< Υ → e+e- trigger algorithm
                       l2Dijet,              ///< Di-jet trigger algorithm
                       l2EmcPedestal,        ///< EMC pedestal-finding algorithm
                       l2Pi0Gamma,           ///< π0/γ trigger algorithm
                       l2DisplacedVertex};   ///< Displaced-vertex (heavy-flavour) algorithm

/*!
 * \enum StPrimaryVertexOrder
 */
enum StPrimaryVertexOrder {
    orderByNumberOfDaughters = 0, /**< enum value for sorting based on NumberOfDaughters (default)  */
    orderByRanking                /**< enum value to switch ordering based on assigned rank         */
};


/*!
 * \enum StL2TriggerResultType
 * \brief Identifies the type of Level-2 trigger result stored in StL2TriggerResult.
 */
enum StL2TriggerResultType {
    l2Trg2006BEMCGammaPi = 0,     ///< 2006 Barrel EMC gamma/pi0 trigger
    l2Trg2006BEMCGammaPiRandom,   ///< 2006 Barrel EMC gamma/pi0 random trigger
    l2Trg2006EEMCGammaPi,         ///< 2006 Endcap EMC gamma/pi0 trigger
    l2Trg2006EEMCGammaPiRandom,   ///< 2006 Endcap EMC gamma/pi0 random trigger
    l2Trg2006MonoJet,             ///< 2006 mono-jet trigger
    l2Trg2006DiJet,               ///< 2006 di-jet trigger
    l2Trg2006RandomJet            ///< 2006 random-jet (reference) trigger
};

/*!
 * \enum StFgtElecConsts
 */
// constants related to electric coordinates
enum StFgtElecConsts { 
    kFgtNumRdos = 2,                    ///< Number of RDOs (RDO indices in {1,2})
    kFgtNumArms = 6,                    ///< Number of ARM boards per RDO (arm indices 0–5; 5 unused in Run12)
    kFgtNumChannels = 128,              ///< Number of channels per APV chip (indices 0–127)
    kFgtApvsPerAssembly = 12,           ///< APV chips per assembly board
    kFgtMaxApvId=kFgtApvsPerAssembly*2, ///< Maximum APV ID value (covers indices 0–23)
    kFgtApvGap = 2,                     ///< Gap in APV numbering (APVs 10 & 11 are unused)
    kFgtApvsPerOct = 5,                 ///< APV chips per octant
    kFgtApvsPerQuad = 10,               ///< APV chips per quadrant
    kFgtApvsPerArm = 20,                ///< APV chips per ARM board
    kFgtNumElecIds = kFgtNumChannels * kFgtApvsPerArm * kFgtNumArms * kFgtNumRdos  ///< Total electronic channel IDs (0 to kFgtNumElecIds-1)
};

/*!
 * \enum StFgtPhysConsts
 */
// constants related to physical coordinates
enum StFgtPhysConsts {
    kFgtNumDiscs = 6,                    ///< Number of FGT discs
    kFgtNumQuads = 4,                    ///< Number of quadrants per disc
    kFgtNumOctantsPerDisc = 8,           ///< Number of octants per disc
    kFgtNumOctants = kFgtNumOctantsPerDisc*kFgtNumDiscs, ///< Total octants across all discs
    kFgtNumLayers = 2,                   ///< Number of strip layers per disc (phi and r)
    kFgtNumStrips = 720,                 ///< Maximum strip count per layer
    kFgtNumGeoIds = kFgtNumQuads * kFgtNumDiscs * kFgtNumLayers * kFgtNumStrips, ///< Total geometric IDs (0 to kFgtNumGeoIds-1)
    kFgtNumPstripsPerOctant = 360,       ///< Phi-strips per octant
    kFgtNumRstripsPerOctant = 280,       ///< R-strips per octant
    kFgtLowerStripOctant = 'L',          ///< Label for the lower-index strip octant
    kFgtHigherStripOctant = 'S',         ///< Label for the higher-index strip octant
    kFgtNumStripsPerDisc = kFgtNumQuads  * kFgtNumLayers * kFgtNumStrips ///< Strips per disc (both planes)
};

/*!
 * \enum StFgtGeneralConsts
 */
// unsorted constants
enum StFgtGeneralConsts {
    kFgtNumTimeBins = 15,   ///< Number of APV time-sample bins per event
    kFgtMaxAdc = 4096,      ///< Maximum ADC value (12-bit)
};

/*!
 * \enum StFgtClusterSeed Type
 * \brief Classification of an FGT strip cluster by its seed quality.
 */
// cluster seed types
enum StFgtClusterSeedType {
    kFgtSeedTypeNo,                  ///< No seed found
    kFgtDeadStrip,                   ///< Strip is dead / masked
    kFgtSeedType1,                   ///< Seed type 1 (highest quality)
    kFgtSeedType2,                   ///< Seed type 2
    kFgtSeedType3,                   ///< Seed type 3
    kFgtSeedType4,                   ///< Seed type 4
    kFgtSeedType5,                   ///< Seed type 5
    kFgtSeedTypeMax,                 ///< Sentinel: number of numbered seed types
    kFgtClusterPart,                 ///< Strip is part of a cluster but not the seed
    kFgtNextToDeadGuy,               ///< Strip is adjacent to a dead strip
    kFgtClusterEndUp,                ///< Strip at the upper end of a cluster
    kFgtClusterEndDown,              ///< Strip at the lower end of a cluster
    kFgtStripShared,                 ///< Strip shared between two clusters
    kFgtClusterTooBig,               ///< Cluster exceeds maximum allowed size
    kFgtClusterSeedInSeaOfNoise,     ///< Seed surrounded by noisy strips
    kFgtNextToCluster,               ///< Strip adjacent to a cluster
    kFgtKeepStrip                    ///< Strip retained despite marginal signal
};

/** FPS/FMS/FHC/FPS/FPost detectorId */
enum StFmsDetectorId{
    kFpdNorthDetId=0,       ///< FPD North large-cell calorimeter
    kFpdSouthDetId=1,       ///< FPD South large-cell calorimeter
    kFpdNorthPrsDetId=2,    ///< FPD North pre-shower detector
    kFpdSouthPrsDetId=3,    ///< FPD South pre-shower detector
    kFpdNorthSMDVDetId=4,   ///< FPD North SMD vertical strip
    kFpdSouthSMDVDetId=5,   ///< FPD South SMD vertical strip
    kFpdNorthSMDHDetId=6,   ///< FPD North SMD horizontal strip
    kFpdSouthSMDHDetId=7,   ///< FPD South SMD horizontal strip
    kFmsNorthLargeDetId=8,  ///< FMS North large-cell calorimeter
    kFmsSouthLargeDetId=9,  ///< FMS South large-cell calorimeter
    kFmsNorthSmallDetId=10, ///< FMS North small-cell calorimeter
    kFmsSouthSmallDetId=11, ///< FMS South small-cell calorimeter
    kFhcNorthDetId=12,      ///< FHC (Hadronic Calorimeter) North
    kFhcSouthDetId=13,      ///< FHC South
    kFpsDetId=14,           ///< Forward Preshower (FPS)
    kFpostDetId=15          ///< Forward post-shower (FPost)
};

/** Categorization of a FMS tower cluster as 1- or 2-photon, or "not sure". */
enum StFmsClusterCategory {
    kAmbiguousCluster = 0,  ///< Could be 1- or 2-photon, needs to be fitted
    k1PhotonCluster = 1,  ///< A cluster created by 1 photon
    k2PhotonCluster = 2,  ///< A cluster created by 2 photons
    kInvalidClusterCategory  ///< For default/unknown/error value
};  // enum StFmsClusterCategory

/** FPS basic constants */
enum StFpsConstants {
    kFpsNQuad=4,        ///< Number of FPS quadrants
    kFpsNLayer=3,       ///< Number of scintillator layers per quadrant
    kFpsNSlat=21,       ///< Number of slats per layer
    kFpsMaxSlat=252,    ///< Maximum slat index (= kFpsNQuad * kFpsNLayer * kFpsNSlat)
    kFpsNCandidate=4    ///< Maximum number of FPS photon candidates per event
};

/** FPost basic constants */
enum StFpostConstants {
    kFpostNQuad=2,       ///< Number of FPost quadrants
    kFpostNLayer=5,      ///< Number of scintillator layers per quadrant
    kFpostNSlat=43,      ///< Number of slats per layer
    kFpostMaxSlat=241,   ///< Maximum slat index (= kFpostNQuad * kFpostNLayer * kFpostNSlat / 2 + ...)
    kFpostNCandidate=6   ///< Maximum number of FPost photon candidates per event
};

/** FCS detectorId **/
enum StFcsDetectorId{
    kFcsEcalNorthDetId=0,  ///< FCS ECAL (W-Shashlyk) North
    kFcsEcalSouthDetId=1,  ///< FCS ECAL South
    kFcsHcalNorthDetId=2,  ///< FCS HCAL (hadronic calorimeter) North
    kFcsHcalSouthDetId=3,  ///< FCS HCAL South
    kFcsPresNorthDetId=4,  ///< FCS pre-shower detector North
    kFcsPresSouthDetId=5   ///< FCS pre-shower detector South
};

/** FCS basic constants **/
enum StFcsConstants {
    kFcsNDet=6,           ///< Total number of FCS detector sub-systems
    kFcsEHP=3,            ///< Index dimension: ECAL(0), HCAL(1), Preshower(2)
    kFcsEcalHcal=2,       ///< Index dimension: ECAL(0), HCAL(1)
    kFcsNorthSouth=2,     ///< Index dimension: North(0), South(1)
    kFcsMaxId=748,        ///< Maximum channel ID across all FCS sub-detectors
    kFcsEcalNCol=22,      ///< ECAL number of columns
    kFcsEcalNRow=34,      ///< ECAL number of rows
    kFcsEcalMaxId=748,    ///< ECAL maximum channel ID
    kFcsHcalNCol=13,      ///< HCAL number of columns
    kFcsHcalNRow=20,      ///< HCAL number of rows
    kFcsHcalMaxId=260,    ///< HCAL maximum channel ID
    kFcsPresMaxId=192,    ///< Pre-shower maximum channel ID
    kFcsPresNCol=16,      ///< Pre-shower number of columns
    kFcsPresNRow=12,      ///< Pre-shower number of rows
    kFcsMaxDepCrate=5,    ///< Maximum number of DEP crates
    kFcsMaxDepBd=24,      ///< Maximum number of DEP boards per crate
    kFcsMaxDepCh=32,      ///< Maximum number of channels per DEP board
    kFcsEcal4x4NCol=9,    ///< ECAL 4×4-tower trigger-patch columns
    kFcsEcal4x4NRow=15,   ///< ECAL 4×4-tower trigger-patch rows
    kFcsHcal4x4NCol=5,    ///< HCAL 4×4-tower trigger-patch columns
    kFcsHcal4x4NRow=9     ///< HCAL 4×4-tower trigger-patch rows
};

/** StFtt Enums **/
enum StFttConstants {
    kFttHorizontal = 0,         ///< Horizontal strip orientation
    kFttVertical = 1,           ///< Vertical strip orientation
    kFttDiagonalH = 2,          ///< Diagonal strips on the horizontal chamber
    kFttDiagonalV = 3,          ///< Diagonal strips on the vertical chamber
    kFttUnknownOrientation = 4, ///< Unknown strip orientation
    kFttQuadrantA = 0,          ///< FTT quadrant A
    kFttQuadrantB = 1,          ///< FTT quadrant B
    kFttQuadrantC = 2,          ///< FTT quadrant C
    kFttQuadrantD = 3,          ///< FTT quadrant D
    kFttUnknownQuadrant = 4,    ///< Unknown quadrant
};

/** RHICf basic constants **/
enum StRHICfConstants {
    kRHICfNtower=2,     ///< Number of towers: 0 = small, 1 = large
    kRHICfNplate=16,    ///< Number of longitudinal sampling plates
    kRHICfNrange=2,     ///< Number of gain ranges (wide and narrow Q/charge)
    kRHICfNlayer=4,     ///< Number of bar layers (longitudinal)
    kRHICfNxy=2,        ///< Coordinate index: 0 = x, 1 = y
    kRHICfNbarSmall=20, ///< Number of bars in the small tower (indices 0–19)
    kRHICfNbarLarge=40, ///< Number of bars in the large tower (indices 0–39)
    kRHICfNorder=2,     ///< Number of particle ordering indices
    kRHICfNtdc=256,     ///< TDC channel count
    kRHICfNcad0=5,      ///< Local counter count for DAQ
    kRHICfNgpi0=19,     ///< Number of GPI0 flag bits
    kRHICfNgpi1=17,     ///< Number of GPI1 flag bits
};

/** For more IST related constants see StRoot/StIstUtil/StistConsts.h */
namespace StIstConsts
{
    const int kIstNumLadders = 24;          ///< 24 IST Ladders
    const int kIstNumSensorsPerLadder = 6;  ///< 6 sensor per one IST Ladder
    const int kIstNumSensors = 144;         ///< 144 sensors
    const int kIstNumPadsPerSensor = 768;   ///< 768 pads in each sensor
    const int kIstNumRowsPerSensor = 64;    ///< 64 rows in r-phi direction per each sensor
    const int kIstNumColumnsPerSensor = 12; ///< 12 columns in beam direction per each sensor
}

/*!
 * \enum StGmtElecConsts
 */
// constants related to electric coordinates
enum StGmtElecConsts {
    kGmtNumRdos = 1,                    ///< Number of RDOs (RDO index = {1})
    kGmtNumArms = 2,                    ///< Number of ARM halves (indices 0–1)
    kGmtNumChannels = 128,              ///< Channels per APV chip (indices 0–127)
    kGmtApvsPerAssembly = 2,            ///< APV chips per assembly
    kGmtMaxApvId= 15,                   ///< Maximum APV ID (only indices 0–3 and 12–15 used)
    kGmtApvGap = 8,                     ///< Gap in APV numbering (indices 4–11 unused)
    kGmtApvsPerArm = 24,                ///< APV chips per ARM
    kGmtNumElecIds = kGmtNumChannels * kGmtApvsPerArm * kGmtNumArms * kGmtNumRdos  ///< Total electronic channel IDs (0 to kGmtNumElecIds-1)
};

/*!
 * \enum StGmtPhysConsts
 */
// constants related to physical coordinates
enum StGmtPhysConsts {
    kGmtNumModules = 8,                  ///< Total number of GMT modules
    kGmtNumLayers = 2,                   ///< Layers per module: one X (strip) and one Y (pad)
    kGmtNumStrips = 128,                 ///< Strips per layer per module
    kGmtNumConnectedStripsX = 126,       ///< Connected X-strips per module (2 missing)
    kGmtNumConnectedStripsY = 125,       ///< Connected Y-strips per module (3 missing)
    kGmtNumGeoIds = kGmtNumModules * kGmtNumLayers * kGmtNumStrips, ///< Total geometric IDs (0 to kGmtNumGeoIds-1)
    kGmtNumPstripsPerModule = 128,       ///< Pad (local-Y) strips per module
    kGmtNumSstripsPerModule = 128,       ///< Strip (local-X) channels per module
    kGmtNumStripsPerModule = kGmtNumStrips * kGmtNumLayers ///< Total strips per module (X + Y layers)
};

/*!
 * \enum StGmtGeneralConsts
 */
// unsorted constants
enum StGmtGeneralConsts {
    kGmtNumTimeBins = 15,               ///< Number of APV time-sample bins per event
    kGmtNumTimeBinsForPed = 3,          ///< Time bins used for pedestal determination
    kGmtMaxAdc = 4096,                  ///< Maximum ADC value (12-bit)
    kGmtPedCut = 3,                     ///< Pedestal-subtracted threshold for first-pass hit tagging
    kGmtHitCut = 5                      ///< Pedestal-subtracted threshold for final hit selection
};

const char *detectorNameById(StDetectorId id);
StDetectorId detectorIdByName(const char *name);
#endif
