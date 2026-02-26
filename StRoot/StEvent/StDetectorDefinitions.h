#ifndef StDetectorDefinitions_hh
#define StDetectorDefinitions_hh

/// @file StDetectorDefinitions.h
/// @brief Integer identifier constants for all STAR sub-detectors.
///
/// These preprocessor constants define the numeric IDs assigned to each
/// sub-detector in the STAR experiment.  The same values back the
/// StDetectorId enumeration in StEnumerations.h and are also used by
/// legacy Fortran code that cannot use C++ enumerations.

/* Numbering scheme for detectors
** TPC             = 1
** SVT             = 2
** RICH            = 3
** FTPC west       = 4
** FTPC east       = 5
** TOF             = 6
** CTB             = 7
** SSD             = 8
** barrel EMC tower= 9
** barrel EMC pre-shower = 10
** barrel SMD eta strip  = 11
** barrel SMD phi strip  = 12
** endcap EMC tower      = 13
** endcap EMC pre-shower = 14
** endcap SMD eta strip  = 15
** endcap SMD phi strip  = 16
** Zero Degree Calo west = 17
** Zero Degree Calo east = 18
** MWPC west       = 19
** MWPC east       = 20
** TPC+SSD         = 21
** TPC+SVT         = 22
** TPC+SSD+SVT     = 23
** SSD+SVT         = 24
** CPV(PMD)        = 25
** PMD             = 26
** Pixel Layer (HFT) PXL = 27
** Strip Layer (HFT) IST = 28
** Forward Tracker   FGT = 29
** FPD West        = 30
** FPD East        = 31
** FMS             = 32
** MTD             = 34
** ETR             = 35
** SST             = 36
** GMT             = 37
** FTS             = 38
** iTPC            = 39
** ETof            = 40
** FCS Wcal        = 41
** FCS Hcal        = 42
** FCS Pres        = 43
*/




#define kUnknownIdentifier             0  ///< Unknown or undefined detector
#define kTpcIdentifier                 1  ///< Time Projection Chamber (TPC)
#define kSvtIdentifier                 2  ///< Silicon Vertex Tracker (SVT)
#define kRichIdentifier                3  ///< Ring-Imaging Cherenkov detector (RICH)
#define kFtpcWestIdentifier            4  ///< Forward TPC, west
#define kFtpcEastIdentifier            5  ///< Forward TPC, east
#define kTofIdentifier                 6  ///< Time-of-Flight (TOF / BTof)
#define kCtbIdentifier                 7  ///< Central Trigger Barrel (CTB)
#define kSsdIdentifier                 8  ///< Silicon Strip Detector (SSD)
#define kBarrelEmcTowerIdentifier      9  ///< Barrel EMC tower
#define kBarrelEmcPreShowerIdentifier 10  ///< Barrel EMC pre-shower layer
#define kBarrelSmdEtaStripIdentifier  11  ///< Barrel Shower Maximum Detector (SMD) eta strip
#define kBarrelSmdPhiStripIdentifier  12  ///< Barrel SMD phi strip
#define kEndcapEmcTowerIdentifier     13  ///< Endcap EMC tower
#define kEndcapEmcPreShowerIdentifier 14  ///< Endcap EMC pre-shower layer
#define kEndcapSmdUStripIdentifier    15  ///< Endcap SMD U-strip
#define kEndcapSmdVStripIdentifier    16  ///< Endcap SMD V-strip
#define kZdcWestIdentifier            17  ///< Zero Degree Calorimeter (ZDC), west
#define kZdcEastIdentifier            18  ///< Zero Degree Calorimeter (ZDC), east
#define kMwpcWestIdentifier           19  ///< Multi-Wire Proportional Chamber (MWPC), west
#define kMwpcEastIdentifier           20  ///< Multi-Wire Proportional Chamber (MWPC), east
#define kTpcSsdIdentifier             21  ///< Combined TPC + SSD tracking
#define kTpcSvtIdentifier             22  ///< Combined TPC + SVT tracking
#define kTpcSsdSvtIdentifier          23  ///< Combined TPC + SSD + SVT tracking
#define kSsdSvtIdentifier             24  ///< Combined SSD + SVT tracking
#define kPhmdCpvIdentifier            25  ///< Photon Multiplicity Detector charged-particle veto (PMD-CPV)
#define kPhmdIdentifier               26  ///< Photon Multiplicity Detector (PMD)


/*
**  The following are for the inner and forward
**  tracking upgrades. tu (Oct 11, 2007)
*/
#define kPxlIdentifier                27  ///< HFT Pixel detector (PXL)
#define kIstIdentifier                28  ///< HFT Intermediate Silicon Tracker (IST)
#define kFgtIdentifier                29  ///< Forward GEM Tracker (FGT)

/*
**  The following are for the forward
**  spectrometers (tu April 6, 2009)
*/
#define kFpdWestIdentifier            30  ///< Forward Pion Detector (FPD), west
#define kFpdEastIdentifier            31  ///< Forward Pion Detector (FPD), east
#define kFmsIdentifier                32  ///< Forward Meson Spectrometer (FMS)

/*
 **  The following are for the Roman Pot
 **  silicon detectors (pp2pp) (tu November 6, 2009)
 */
#define kRpsIdentifier                33  ///< Roman Pot silicon detectors (pp2pp / RPS)

/*
 **  The following are for the Muon Telescope Detector
 */
#define kMtdIdentifier                34  ///< Muon Telescope Detector (MTD)

/*
 **  The following are for the Endcap TRD in ETTIE detector
 */
#define kEtrIdentifier                35  ///< Endcap TRD (ETTIE) prototype

/*
 **  The following are for the SST (Beware: not same as SSD)
 */
#define kSstIdentifier                36  ///< HFT Silicon Strip Tracker (SST; distinct from SSD)
/*
**  The following is the addition of the GEM chambers
**  to improve TPC tracking and alignment. RW (Mar 27, 2013)
*/
#define kGmtIdentifier                37  ///< GEM Muon Tagger (GMT) for TPC alignment

/*
** Add the FTS tracker
*/
#define kFtsIdentifier                38  ///< Forward Tracking System (FTS)

/*
 ** iTPC
 */
#define kiTpcIdentifier               39  ///< Inner TPC upgrade (iTPC)

/*
 ** eTOF
 */
#define kETofIdentifier               40  ///< Endcap Time-of-Flight (eTOF)

/*
** Add the FCS Wcal and Hcal
*/
#define kFcsWcalIdentifier            41  ///< Forward Calorimeter System (FCS) W-Shashlyk ECAL
#define kFcsHcalIdentifier            42  ///< FCS hadronic calorimeter (HCAL)
#define kFcsPresIdentifier            43  ///< FCS pre-shower detector

/*
 ** Add the sTGC (Ftt)
 */

#define kFttIdentifier                44  ///< Forward sTGC Tracker (FTT)

/*
 *  ** Add the Forward Silicon Tracker (Fst)
 *   */

#define kFstIdentifier                45  ///< Forward Silicon Tracker (FST)

/*
 ** RHICf
 */
#define kRHICfIdentifier              46  ///< RHICf calorimeter (very forward neutral particle detector)

/*
**  The following are more or less virtual detectors.
**  Depending on funding or policy this stuff might
**  happen or not. (OBSOLETE)
*/
/*
#define kHftIdentifier                27
#define kIstIdentifier                28
#define kIgtIdentifier                29
#define kFstIdentifier                30
#define kFgtIdentifier                31
#define kHpdIdentifier                32
*/

#endif /*STDETECTORDEFINITIONS*/

/* $Id: StDetectorDefinitions.h,v 2.16 2021/01/11 20:27:40 ullrich Exp $
**
** $Log: StDetectorDefinitions.h,v $
** Revision 2.16  2021/01/11 20:27:40  ullrich
** Updated to add FCS elements (Akio).
**
** Revision 2.15  2019/02/11 18:49:59  ullrich
** Added EToF.
**
** Revision 2.14  2018/03/27 02:40:12  genevb
** Introduce kiTpcId
**
** Revision 2.13  2017/05/04 00:52:53  perev
** Fts added
**
** Revision 2.12  2015/12/24 00:14:44  fisyak
** Add GMT and SST Id and new dE/dx method
**
** Revision 2.11  2015/05/13 17:06:13  ullrich
** Added hooks and interfaces to Sst detector (part of HFT).
**
** Revision 2.10  2012/01/24 02:58:21  perev
** Etr detector added
**
** Revision 2.9  2011/04/25 21:25:09  ullrich
** Modifications to hold MTD data.
**
** Revision 2.8  2009/11/23 22:22:25  ullrich
** Minor cleanup performed and hooks for RPS added.
**
** Revision 2.7  2009/04/06 19:23:53  ullrich
** Add detector Ids for FPD East/West and FMS.
**
** Revision 2.6  2007/10/11 21:50:19  ullrich
** Added new enums for PXL and IST detectors.
**
** Revision 2.5  2006/08/15 14:34:02  ullrich
** Added kHpdIdentifier.
**
** Revision 2.4  2006/01/20 15:11:59  jeromel
** ... meant needs to be C style, not C++
**
** Revision 2.3  2006/01/20 15:11:26  jeromel
** Comments need to be FORtran style
**
** Revision 2.2  2006/01/19 21:51:26  ullrich
** Added new RnD detectors.
**
** Revision 2.1  2004/04/26 16:35:19  fisyak
** Move enumerations from pams/global/inc => StEvent
**
** Revision 1.10  2002/12/19 21:52:38  lbarnby
** Corrected CVS tags
**
*/
