*
* $Id: eufilp.F,v 1.1.1.2 2009/02/01 17:10:15 fisyak Exp $
*
* $Log: eufilp.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:15  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:29  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:23  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:26  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:36  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:15  fca
* AliRoot sources
*
* Revision 1.1.1.1  1996/03/06 15:37:35  mclareni
* Add geane321 source directories
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.49  by  S.Giani
*-- Author :
      SUBROUTINE EUFILP (N, EIN, PLI, PLF)
*
C.    ******************************************************************
C.    *                                                                *
C.    *    User routine to fill the input values of the commons :      *
C.    *               /EROPTS/, /EROPTC/ and /ERTRIO/ for CHOPT = 'P'  *
C.    *         N     Number of predictions where to store results     *
C.    *         EIN   Input error matrix (in the 'Plane' system )      *
C.    *         PLI   Defines the start plane                          *
C.    *                      PLI(3,1) - and                            *
C.    *                      PLI(3,2) - 2 unit vectors in the plane    *
C.    *         PLF   Defines the end plane                            *
C.    *                      PLF(3,1,I) - and                          *
C.    *                      PLF(3,2,I) - 2 unit vectors in the plane  *
C.    *                      PLF(3,3,I) - point on the plane           *
C.    *                                   at intermediate point I      *
C.    *                                                                *
C.    *    ==>Called by : USER (before calling ERTRAK)                 *
C.    *       Author    M.Maire, E.Nagy  *********                     *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/ertrio.inc"
      DIMENSION EIN(15), PLF(3,3,N), PLI(3,2),PWORK(3)
*
      NEPRED = MIN0 (N, MXPRED)
*
      CALL UCOPY (EIN(1), ERRIN(1), 15)
      CALL UCOPY (PLI(1,1), ERPLI(1,1), 6)
      DO 5 I = 1, NEPRED
         CALL UCOPY (PLF(1,1,I), ERPLO(1,1,I), 9)
         CALL CROSS (PLF(1,1,I), PLF(1,2,I), PWORK(1))
         PNORM = VMOD(PWORK,3)
         CALL VSCALE(PWORK,PNORM,ERPLO(1,4,I),3)
    5 CONTINUE
*
*                                                           END EUFILP
  99  END
