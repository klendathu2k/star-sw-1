*
* $Id: vunit.F,v 1.1.1.1 2004/07/17 20:01:57 perev Exp $
*
* $Log: vunit.F,v $
* Revision 1.1.1.1  2004/07/17 20:01:57  perev
* STAR version of Geant321 TGeant3 etc
*
* Revision 1.1.1.1  2002/07/24 15:56:28  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:47  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:32  fca
* AliRoot sources
*
* Revision 1.1.1.1  1996/02/15 17:50:18  mclareni
* Kernlib
*
*
#include "kerngen/pilot.h"
      SUBROUTINE VUNIT (A,B,N)
C
C CERN PROGLIB# F121    VUNIT           .VERSION KERNFOR  1.0   710701
C ORIG. 01/07/71
C
      DIMENSION A(*),B(*)
C
      IF (N.LE.0)  RETURN
      XX= 0.
      DO 9 I=1,N
    9 XX= XX + A(I)*A(I)
      IF (XX.LE.0.)  RETURN
      YY=1./ SQRT (XX)
      DO 19 I= 1,N
   19 B(I)=A(I) * YY
      RETURN
      END
