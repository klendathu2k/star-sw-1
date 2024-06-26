*
* $Id: gkokri.F,v 1.1.1.2 2009/02/01 17:10:32 fisyak Exp $
*
* $Log: gkokri.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:32  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:48:49  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:56  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.2  2003/11/28 11:23:56  brun
* New version of geant321 with all geant3 routines renamed from G to G3
*
* Revision 1.1.1.1  2002/07/24 15:56:27  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:41  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:20  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:21:37  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.25  by  S.Giani
*-- Author :
      FUNCTION G3KOKRI(E,EMINEV,EMAXEV)
C.
C.    ******************************************************************
C.    *                                                                *
C.    *     Input energy in eV, Sandia tables in keV                   *
C.    *                                                                *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gcbank.inc"
#include "geant321/gcjloc.inc"
#include "geant321/gconsp.inc"
#include "geant321/gcmate.inc"
#include "geant321/gc10ev.inc"
#if !defined(CERNLIB_SINGLE)
      DOUBLE PRECISION EINV,ECUR,ONE,RES,ZERO,EBEG,EEND
      DOUBLE PRECISION ALPH,BETA,GAMM,E1,E2,G3PSCIN,POLE
      DOUBLE PRECISION EMAX,EMIN,REST
#endif
      PARAMETER (ONE=1,ZERO=0)
C.
C.    ------------------------------------------------------------------
C.
      REST   = ZERO
      EMAX   = EMAXEV*1E-3
      EMIN   = EMINEV*1E-3
      ECUR   = E*1E-3
      EINV   = ONE/ECUR
C Use Sandia data
      JPHXS  = LQ(JPHOT-1)
      NZ     = Q(JPHXS+1)
      JWEIGH = JPHXS+1+2*NZ
      DO 30 JZ=1,NZ
         RES    = ZERO
         EBEG   = EMIN
         WEIGHT = Q(JWEIGH+JZ)
         JPHFN  = LQ(JPHXS-JZ)
         IPOINT = JPHFN+1
         IMAX   = Q(IPOINT)
         IPOINT = IPOINT-4
         DO 10 I = 1,IMAX
            IPOINT = IPOINT+5
            EEND   = Q(IPOINT)
            IF(EEND.GT.EMIN) THEN
               E1     = MAX(EBEG,EMIN)
               E2     = MIN(EEND,EMAX)
               J      = IPOINT+1
               IF(ECUR.LE.E2.AND.ECUR.GE.E1) THEN
*
* *** The pole of the integration is in this interval
                  EPS1 = (ECUR-EBEG)/ECUR
                  EPS2 = (EEND-ECUR)/EEND
                  IF(EPS1.LT.EPS2) THEN
*
* *** First the pole and then a simple integration
                     ALPH = ONE-EPS1
                     E1 = ECUR/ALPH
                     E2 = EEND
                  ELSE
*
* *** First a simple integration and then the pole
                     ALPH = ONE-EPS2
                     E1 = EBEG
                     E2 = ECUR*ALPH
                  ENDIF
                  BETA = -LOG(ALPH)
                  GAMM = ONE/ALPH
                  POLE = EINV*(Q(J)*BETA+EINV*( Q(J+1)*(GAMM-ALPH)+
     +            EINV*( Q(J+2)*(0.5*GAMM**2+BETA-0.5*ALPH**2)+EINV*
     +            Q(J+3)*(GAMM**3/3+GAMM-ALPH-ALPH**3/3))))
                  RES = RES - POLE
               ENDIF
*
* *** This is a normal integration
               RES = RES + G3PSCIN(E1,E2,ECUR,Q(J))
            ENDIF
            EBEG = EEND
            IF(EBEG.GE.EMAX) GO TO 20
   10    CONTINUE
   20    REST = REST+WEIGHT*RES
   30 CONTINUE
C RES value is in cm**2/(g keV)
      G3KOKRI = REST*1E-6*A/AVO
C Now in Megabarns/eV
C
      END
