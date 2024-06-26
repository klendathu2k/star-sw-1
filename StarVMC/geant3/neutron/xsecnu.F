*
* $Id: xsecnu.F,v 1.1.1.2 2009/02/01 17:10:35 fisyak Exp $
*
* $Log: xsecnu.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:35  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:49:06  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:58  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:43  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:21  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:22:00  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.48  by  S.Giani
*-- Author :
      SUBROUTINE XSECNU(BUF,LEN,E,XSC,L1,L2)
C       THIS ROUTINE DETERMINES A CROSS SECTION AT A GIVEN ENERGY
C       FROM A CROSS SECTION VERSUS ENERGY TABLE
      DIMENSION BUF(*)
      SAVE
      IF(E.LT.BUF(L1))GO TO 40
      DO 10 J=1,LEN
         N=L1+2*(J-1)
         IF(E.LE.BUF(N))GO TO 20
   10 CONTINUE
      XSC=BUF(L2)
      RETURN
   20 IF(J.EQ.1)GO TO 30
      XSC=BUF(N-1)+(E-BUF(N-2))*(BUF(N+1)-BUF(N-1))/
     +(BUF(N)-BUF(N-2))
      RETURN
   30 XSC=BUF(N+1)
      RETURN
   40 XSC=0.0
      RETURN
      END
