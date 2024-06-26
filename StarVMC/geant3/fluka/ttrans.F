*
* $Id: ttrans.F,v 1.1.1.2 2009/02/01 17:10:18 fisyak Exp $
*
* $Log: ttrans.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:18  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:56  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:24  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:24  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:37  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:16  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:04  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.44  by  S.Giani
*-- Author :
*$ CREATE TTRANS.FOR
*COPY TTRANS
      SUBROUTINE TTRANS(XO,YO,ZO,CDE,SDE,SFE,CFE,X,Y,Z)
 
#include "geant321/dblprc.inc"
#include "geant321/dimpar.inc"
#include "geant321/iounit.inc"
C********************************************************************
C     VERSION BY                     J. RANFT
C                                    LEIPZIG
C
C     THIS IS A SUBROUTINE OF FLUKA TO GIVE NEW DIRECTION COSINES
C
C     INPUT VARIABLES:
C        XO,YO,ZO = ORIGINAL DIRECTION COSINES
C        CDE,SDE  = COSINE AND SINE OF THE POLAR (THETA)
C                   ANGLE OF "SCATTERING"
C        SDE      = SINE OF THE POLAR (THETA) ANGLE OF "SCATTERING"
C        SFE,CFE  = SINE AND COSINE OF THE AZIMUTHAL (PHI) ANGLE
C                   OF "SCATTERING"
C
C     OUTPUT VARIABLES:
C        X,Y,Z     = NEW DIRECTION COSINES
C
C     ROTATION OF COORDINATE SYSTEM (SEE CERN 64-47 )
C********************************************************************
C
*
*  Changed by A. Ferrari
*
      A = XO**2 + YO**2
      IF ( A .LT. ANGLSQ ) THEN
         X=SDE*CFE
         Y=SDE*SFE
C     Z=CDE CORRECTED AUGUST 88 PA
         Z=CDE*ZO
      ELSE
         XI=SDE*CFE
         YI=SDE*SFE
         ZI=CDE
         A=SQRT(A)
         X=-YO*XI/A-ZO*XO*YI/A+XO*ZI
         Y=XO*XI/A-ZO*YO*YI/A+YO*ZI
         Z=A*YI+ZO*ZI
      END IF
      RETURN
      END
