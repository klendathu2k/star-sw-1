*
* $Id: gspset.F,v 1.1.1.2 2009/02/01 17:10:24 fisyak Exp $
*
* $Log: gspset.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:24  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:47:38  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:35  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:38  hristov
* Separate distribution  of Geant3
*
* Revision 1.2  2001/03/20 06:36:26  alibrary
* 100 parameters now allowed for geant shapes
*
* Revision 1.1.1.1  1999/05/18 15:55:17  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:47  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.32  by  S.Giani
*-- Author :
      SUBROUTINE GSPSET(PAR)
C-
C-   Created  12-JUN-1991   Nils Joar Hoimyr
C-   Modified 21.02.1992  Jouko Vuoskoski
C-
C-  Takes a GEANT SPHE shape, and wites it out as a primitive solid
C-  sphere in SET.
C----------------------------------------------------------------------
 
#include "geant321/gcsetf.inc"
 
C
      DIMENSION PAR(100)
C
      REAL RMIN, RMAX
C
C Segments not yet ready:
C    REAL ALPH1, ALPH2, THET1, THET2
C
C----------------------------------------------------------------------
C
      RMIN= PAR(1)
      RMAX= PAR(2)
C
C        IF segment of a sphere  THEN (Currently not implemented)
C      *WRITE SET @50,N..#34,RMAX,ALPH1,ALPH2,THET1,THET2,RMIN
C        ELSE
C      *WRITE SET @50,N..#34,RMAX,,,,,RMIN
C----------------------------------------------------------------------
C
      WRITE(BLKSTR,10000)N1,RMAX,RMIN
      CALL GJWRIT
C
10000   FORMAT('@50,',I10,',:5,2#34,',G14.7,',,,,,',G14.7)
C
      END
