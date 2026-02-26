/*!
 * \file StVertexDefinitions.h
 * \brief Preprocessor constants for vertex-type identifiers (Fortran-compatible).
 *
 * \author unknown
 *
 * Numbering scheme for vertex types.
 * The definitions listed here are used in StEnumeration
 * to define the actual enumerations to be used in C/C++
 * code. The definitions here are only to allow fortran
 * code to use the same identifiers and thus maintain
 * backwards compatibility.
 * 
 */
/***************************************************************************
 *
 * $Id: StVertexDefinitions.h,v 2.3 2004/11/05 04:06:15 jeromel Exp $
 *
 * Author: unknown
 ***************************************************************************
 *
 *
 ***************************************************************************
 *
 * $Log: StVertexDefinitions.h,v $
 * Revision 2.3  2004/11/05 04:06:15  jeromel
 * doxygenized doc so defines will become cross refs in auto-documentation
 *
 * Revision 2.2  2004/11/03 18:30:16  ullrich
 * Added definitions for FTPC calibration vertices.
 *
 *
 **************************************************************************/
#ifndef StVertexDefinitions_hh
#define StVertexDefinitions_hh

#define kUndefinedVertexIdentifier           0                         ///< Unknown or undefined vertex type
#define kEventVertexIdentifier               1                         ///< Primary collision vertex
#define kV0DecayIdentifier                   2                         ///< V0 decay vertex (e.g. K0s, Lambda)
#define kXiDecayIdentifier                   3                         ///< Xi (cascade) decay vertex
#define kKinkDecayIdentifier                 4                         ///< Kink decay vertex
#define kOtherTypeIdentifier                 5                         ///< Other / unclassified vertex type
#define kFtpcEastCalibrationVertexIdentifier 6                         ///< FTPC east laser calibration vertex
#define kFtpcWestCalibrationVertexIdentifier 7                         ///< FTPC west laser calibration vertex

#endif

