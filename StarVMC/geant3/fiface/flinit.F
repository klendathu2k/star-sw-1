*
* $Id: flinit.F,v 1.1.1.2 2009/02/01 17:10:16 fisyak Exp $
*
* $Log: flinit.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:16  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:39  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:23  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:36  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:15  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:19:53  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/03 07/10/94  18.34.51  by  S.Giani
*-- Author :
      SUBROUTINE FLINIT
#include "geant321/gcflag.inc"
#include "geant321/gcunit.inc"
#include "geant321/gccuts.inc"
#include "geant321/gsecti.inc"
#include "geant321/gcbank.inc"
#include "geant321/gcnum.inc"
#include "geant321/dimpar.inc"
#if defined(CERNLIB_DOUBLE)
#include "geant321/comcont.inc"
#endif
#include "geant321/comcon.inc"
#include "geant321/iounit.inc"
#include "geant321/gfkdis.inc"
      CHARACTER*(*) SDUM
      PARAMETER (SDUM='        ')
#if defined(CERNLIB_DOUBLE)
      DOUBLE PRECISION WHAT(6)
#endif
#if defined(CERNLIB_SINGLE)
      REAL WHAT(6)
#endif
      DATA WHAT/6*0./
#if defined(CERNLIB_UNIX)||defined(CERNLIB_VAX)
      OPEN(UNIT=LUNERR,FILE='flukaerr.dat',STATUS='UNKNOWN')
#endif
#if defined(CERNLIB_IBM)
      OPEN(UNIT=LUNERR,FILE='/FLUKAERR DAT')
#endif
      CALL FDNOPT
      CALL FLKDT1
      CALL FLKDT2
      CALL FLKDT3
      CALL FLKDT4
      CALL FLKDT5
      CALL FLKDT6
      CALL FLKDT7
      CALL FDEVAP
      CALL FDPREE
      CALL FKZERO
      CALL EVVINI(WHAT,SDUM)
      IFINIT(5)=1
      print *,'*****************************************************'
      print *,'**  You are requesting the GEANT-FLUKA interface:  **'
      print *,'**  for conditions and warnings see the comments   **'
      print *,'**  in GUPHAD/GUHADR and/or the GEANT manual.      **'
      print *,'*****************************************************'
*
      END
