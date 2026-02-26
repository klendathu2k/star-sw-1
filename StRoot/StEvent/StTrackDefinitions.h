#ifndef StTrackDefinitions_hh
#define StTrackDefinitions_hh

/// @file StTrackDefinitions.h
/// @brief Preprocessor constants for track reconstruction method encoding.
///
/// The encoded track method stored in @c StTrack::mEncodedMethod (and the
/// legacy @c dst_track::method) is a packed integer combining a 4-bit
/// fitting-method code (bits 0–3) and a bitmap of finder flags
/// (bits 4–15).  The constants below define the fitting-method codes
/// and the bit positions of each track finder.

/* Numbering scheme for StTrack::mEncodedMethod and dst_track::method
** includes finder scheme + fitting method
** 
** fitting method byte 0-3 (only fit method per track)
** finder scheme bits (4-14) 
** This is a bitmap scheme. Places are reserved for detector trackers.
** Total number of bits reserved (including sign bit) is 12-bits for
** finders (trackers) and first 4-bits for fitting (see below).
**________________________________________________________
**   StTrack::fittingMethod    byte (0:3)
**   Unknown/undefined fitter     0
**   Helix 2 Step                 1   !bending/non-bending plane fits
**   Helix 3D                     2
**   Kalman (local helix)         3 
**   Line 2 Step                  4
**   Line 3D                      5
**   L3 Fitter                    6
**   ITTF Kalman Fit              7
**   Spare                    upto 15 (total of 4 bits)
**_______________________________________________________
**   StTrack::finderMethod     bits                                    
**   SVT Grouper                  4 (LSB) on/off                       
**   SVT Stk                      5                                    
**   SVT Other                    6                                    
**   TPC Standard                 7                                    
**   TPC Other                    8                                    
**   FTPC Conformal               9                                    
**   FTPC Current                10                                    
**   SVT-TPC svm                 11       vector-vector matcher        
**   SVT-TPC est                 12       TPCvector-SVTpoint matcher   
**   SVT-TPC pattern             13      under development             
**   Spare                    14-15 (MSB)                            
**_______________________________________________________
**   StTrack::mEncodedMethod =
**   dst_track.method = fittig_method (4 bits word, 0:3) + 1<< finding_method [+  1<< finding_method2 ..]
**  
*/
/*    Fitting method                 byte[0:3] */
#define kUndefinedFitterIdentifier      0  ///< Unknown or undefined fitting method
#define kHelix2StepIdentifier           1  ///< Helix fit: separate bending / non-bending plane fits
#define kHelix3DIdentifier              2  ///< Full 3-D helix fit
#define kKalmanFitIdentifier            3  ///< Kalman filter fit (local helix parameterisation)
#define kLine2StepIdentifier            4  ///< Straight-line fit: separate 2-D plane fits
#define kLine3DIdentifier               5  ///< Full 3-D straight-line fit
#define kL3FitIdentifier                6  ///< Level-3 online trigger fitter
#define kITKalmanFitIdentifier          7  ///< ITTF Kalman fit
/* numbering for track finder scheme has to be in synch. with StEnumerations.h */
/* Finder schema                      bit                                      */
#define ksvtGrouperIdentifier           4  ///< SVT Grouper finder (bit 4)
#define ksvtStkIdentifier               5  ///< SVT Stk finder (bit 5)
#define ksvtOtherIdentifier             6  ///< SVT other finder (bit 6)
#define ktpcStandardIdentifier          7  ///< TPC standard finder (bit 7)
#define ktpcOtherIdentifier             8  ///< TPC other finder (bit 8)
#define kftpcConformalIdentifier        9  ///< FTPC conformal finder (bit 9)
#define kftpcCurrentIdentifier         10  ///< FTPC current finder (bit 10)
#define ksvtTpcSvmIdentifier           11  ///< SVT-TPC vector–vector matcher (bit 11)
#define ksvtTpcEstIdentifier           12  ///< SVT-TPC TPC-vector / SVT-point matcher (bit 12)
#define ksvtTpcPatternIdentifier       13  ///< SVT-TPC pattern finder (bit 13, under development)
#define kl3StandardIdentifier          14  ///< L3 standard finder (bit 14)


#endif
