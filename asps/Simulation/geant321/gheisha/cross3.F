*
* $Id: cross3.F,v 1.1.1.1 2004/01/15 00:12:10 potekhin Exp $
*
* $Log: cross3.F,v $
* Revision 1.1.1.1  2004/01/15 00:12:10  potekhin
*
* Revision 1.1.1.1  1997/11/03 15:30:15  atlascvs
* Importing CERNLIB version 08.21.
*
* Revision 1.1.1.1  1995/10/24 10:21:07  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.38  by  S.Giani
*-- Author :
C
C *** VARIOUS VECTOR OPERATIONS ***
C
C COPIED FROM F14BLO.PAMLIB 23.4.82
C AUTHOR: V.BLOBEL (UNIVERSITY OF HAMBURG)
C DESYLIB
C
C *** BLANK COMMON REPLACED BY /VECUTY/ TO MATCH GEANT/GHEISHA CODE ***
C *** NOTE THAT P(10,100) HAS BECOME PV(10,200) DUE TO THIS         ***
C
C UN-USED ENTRIES REMOVED :
C     "PCOP" "PEXC" "PZER" "PWRT" "DOT4" "IMPU" "IMPULI" "ADD3"
C     "SUB3" "CROSS" "DOT" "SMUL" "NORZ" "PARPER" "PUNIT" "TRAP"
C
C *** ALL ENTRIES RE-WRITTEN AS SUBROUTINES USING ONLY NECESSARY ***
C *** "DOUBLE PRECISION" STMTS. AND ALL SPECIFIC FUNCTIONS HAVE  ***
C *** BEEN CHANGED TO THEIR GENERIC EQUIVALENCES                 ***
C *** NVE 29-MAR-1988 CERN GENEVA ***
C
C ORIGIN : H.FESEFELDT (22-JUNE-1984)
C
      SUBROUTINE CROSS3(K,L,M)
C
#include "geant321/mxgkgh.inc"
#include "geant321/s_blankp.inc"
      DIMENSION G(3)
      DOUBLE PRECISION A,B,G
C
      A=PV(2,K)*PV(3,L)
      B=PV(3,K)*PV(2,L)
      G(1)=A-B
      A=PV(3,K)*PV(1,L)
      B=PV(1,K)*PV(3,L)
      G(2)=A-B
      A=PV(1,K)*PV(2,L)
      B=PV(2,K)*PV(1,L)
      G(3)=A-B
      DO 26 I=1,3
   26 PV(I,M)=G(I)
      RETURN
      END
