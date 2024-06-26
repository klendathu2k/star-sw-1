*
* $Id: gtrase.F,v 1.1 2004/01/15 20:41:19 fisyak Exp $
*
* $Log: gtrase.F,v $
* Revision 1.1  2004/01/15 20:41:19  fisyak
* The first version to test
*
* Revision 1.1.1.1  1995/10/24 10:20:47  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.32  by  S.Giani
*-- Author :
      SUBROUTINE GTRASE(PAR)
C-
C-   Created  12-JUN-1991   Nils Joar Hoimyr
C-   Modified 21.02.1992  Jouko Vuoskoski
C-
C-  Calculates the coordinates of the corner points of a GEANT TRAP  shape
C-  from the shape parameters.   The end faces are generated by drawing
C-  connected polylines between the corner points at each Z-end of the shape.
C-  A ruled solid is then defined between the 2 faces, and written out in SET.
C---------------------------------------------------------
C
#include "geant321/gcsetf.inc"
 
C
      DIMENSION PAR(50)
C
      REAL P1X,P1Y,P1Z,P2X,P2Y,P2Z,P3X,P3Y,P3Z,P4X,P4Y,P4Z
      REAL P5X,P5Y,P5Z,P6X,P6Y,P6Z,P7X,P7Y,P7Z,P8X,P8Y,P8Z
C
C----------------------------------------------------------------------
C
C             TRAP
C
 
      DZ=PAR(1)
      TX=PAR(2)
      TY=PAR(3)
      H1=PAR(4)
      BL1=PAR(5)
      TL1=PAR(6)
      H2=PAR(8)
      BL2=PAR(9)
      TL2=PAR(10)
C       Parameter conv for faces..
C       FACE parameters:
C       FACE 1:
      P1X= -BL1-DZ*SIN(TX)
      P1Y= -H1-DZ*SIN(TY)
      P1Z= -DZ
C
      P2X= BL1-DZ*SIN(TX)
      P2Y= -H1-DZ*SIN(TY)
      P2Z= -DZ
C
      P3X= TL1-DZ*SIN(TX)
      P3Y= H1-DZ*SIN(TY)
      P3Z= -DZ
C
      P4X= -TL1-DZ*SIN(TX)
      P4Y= H1-DZ*SIN(TY)
      P4Z= -DZ
C       Next FACE:
      P5X= -BL2+DZ*SIN(TX)
      P5Y= -H2+DZ*SIN(TY)
      P5Z= DZ
C
      P6X= BL2+DZ*SIN(TX)
      P6Y= -H2+DZ*SIN(TY)
      P6Z= DZ
C
      P7X= TL2+DZ*SIN(TX)
      P7Y= H2+DZ*SIN(TY)
      P7Z= DZ
C
      P8X= -TL2+DZ*SIN(TX)
      P8Y= H2+DZ*SIN(TY)
      P8Z= DZ
C       DESCRIBED BY 2 FACES represented by polylines
C       connected by the 4 corners of the shape.
C *WRITE SET@103,F1#3,3,2,P1X,P1Y,P1Z,P2X,P2Y,P2Z,P3X,P3Y,P3Z,P4X,P4Y,P4Z
C *WRITE SET@103,F2#3,3,2,P5X,P5Y,P5Z,P6X,P6Y,P6Z,P7X,P7Y,P7Z,P8X,P8Y,P8Z
C
C *WRITE SET @100,N#145,!F1,!F2,
C-------------------------------------------------------------------
C
      WRITE(BLKSTR,10000)N1,P1X,P1Y,P1Z,P2X,P2Y,P2Z, P3X,P3Y,P3Z,P4X,
     +P4Y,P4Z
      CALL GJWRIT
      N1=N1+1
      WRITE(BLKSTR,10000)N1,P5X,P5Y,P5Z,P6X,P6Y,P6Z, P7X,P7Y,P7Z,P8X,
     +P8Y,P8Z
      CALL GJWRIT
      N1=N1+1
      WRITE(BLKSTR,10100)N1,N1-2,N1-1
      CALL GJWRIT
C
10000   FORMAT('@103,',I10,',:5,2#3,3,2,',G14.7,',',G14.7,','
     +  ,G14.7,',',G14.7,',',G14.7,',',G14.7,',',G14.7,','
     +  ,G14.7,',',G14.7,',',G14.7,',',G14.7,',',G14.7)
10100   FORMAT('@100,',I10,',:5,2#145,!',I10,',!',I10)
C
      END
