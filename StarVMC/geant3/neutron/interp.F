*
* $Id: interp.F,v 1.1.1.2 2009/02/01 17:10:34 fisyak Exp $
*
* $Log: interp.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:34  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:49:01  fisyak
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
* Revision 1.1.1.1  1995/10/24 10:21:57  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/04 23/02/95  14.46.01  by  S.Giani
*-- Author :
      SUBROUTINE INTERP(X,Y,X1,Y1,X2,Y2,INT)
C       THIS ROUTINE PERFORMS THE INTERPOLATION ACCORDING
C       TO THE ENDF/B INTERPOLATION SCHEME INT
#include "geant321/minput.inc"
      SAVE
      IF(INT.LT.1.OR.INT.GT.5)GO TO 60
      IF(X2.EQ.X1)GO TO 10
      GO TO (10,20,30,40,50),INT
   10 Y=Y1
      RETURN
   20 Y=Y1+(X-X1)*(Y2-Y1)/(X2-X1)
      RETURN
   30 IF(X1.EQ.0.0.OR.X2.EQ.0.0)GO TO 20
      Y=Y1+ALOG(X/X1)*(Y2-Y1)/ALOG(X2/X1)
      RETURN
   40 IF(Y1.EQ.0.0.OR.Y2.EQ.0.0)GO TO 20
      Y=Y1*EXP((X-X1)*ALOG(Y2/Y1)/(X2-X1))
      RETURN
   50 IF(Y1.EQ.0.0.OR.Y2.EQ.0.0)GO TO 30
      IF(X1.EQ.0.0.OR.X2.EQ.0.0)GO TO 40
      Y=Y1*EXP(ALOG(X/X1)*ALOG(Y2/Y1)/ALOG(X2/X1))
      RETURN
   60 WRITE(IOUT,10000)INT
10000 FORMAT(' MICAP: INTERP-INVALID INTERPOLATION SCHEME',I11)
      WRITE(6,*) ' CALOR: ERROR in INTERP ====> STOP '
      STOP
      END
