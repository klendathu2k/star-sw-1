*
* $Id: fkimpu.F,v 1.1.1.2 2009/02/01 17:10:17 fisyak Exp $
*
* $Log: fkimpu.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:17  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:49  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:24  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.2  2004/12/17 11:46:46  brun
* Several protections introduced by Rachid Guernane and Peter Hristov
*
* Revision 1.1.1.1  2002/07/24 15:56:24  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:36  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:15  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:06  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.45  by  S.Giani
*-- Author :
*=== impuls ===========================================================*
*
      SUBROUTINE FKIMPU(HE,HMA,HPS,HPX,HPY,HPZ,LT,LL,B3)
 
#include "geant321/dblprc.inc"
#include "geant321/dimpar.inc"
#include "geant321/iounit.inc"
      REAL RNDM(2)
*
*----------------------------------------------------------------------*
*  Impuls89: slight revision by A. Ferrari                             *
*----------------------------------------------------------------------*
*
C*****CHOICE OF THE TRANSVERSAL MOMENTUM AND CALCULATION OF THE
C*****LONGITUDINAL MOMENTUM
C*****HPS,HPZ ARE THE TRANSVERSAL AND LONGITUDINAL MOMENTUM
 
      ESMAX = HE - HMA
    1 CONTINUE
*
*  He is the total energy, hma the mass one (input) hpx, hpy, hpz
*  the momentum components (output values), hps the transversal
*  momentum (output)
*
*  +-------------------------------------------------------------------*
*  |                            cut off for the longitudinal momentum
      IF (ESMAX .LE. 0.05D0) THEN
*  |
*  | only transversal momentum!
*  |
         HPS = SQRT(DABS(HE**2 - HMA**2))
         HPZ = 0.D0
      ELSE
*  |
*  | hps is the transversal momentum
*  |
         CALL GRNDM(RNDM,2)
         X = RNDM(1)
         Y = RNDM(2)
*  |
*  |  Es is the transverse kinetic energy
*  |
         ES = -2.D0/(B3**2)*LOG(X*Y)
         IF (ES .GT. ESMAX) GO TO 1
         HPS = ES**2 + 2.D0*ES*HMA
         IF (LL .EQ. 0) Y1 = 1.D0
         IF (LL .EQ. 1) Y1 =-1.D0
         HPZ = SQRT(HE**2 - HMA**2 - HPS) * Y1
         HPS = SQRT (HPS)
      END IF
*  |
*  +-------------------------------------------------------------------*
      CALL SFECFE(SFE,CFE)
      SIP = SFE
      COP = CFE
      HPX = HPS*COP
      HPY = HPS*SIP
      IF (LT .EQ. 0) GO TO 2
      WRITE(LUNOUT,3) HPS,HPX,HPY,HPZ,HE,HMA,LL
    3 FORMAT(1H0,19HPS,PX,PY,PZ,E,M,LL=,6F8.4,I3)
    2 CONTINUE
      RETURN
      END
