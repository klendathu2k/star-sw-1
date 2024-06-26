*
* $Id: fkener.F,v 1.1.1.2 2009/02/01 17:10:17 fisyak Exp $
*
* $Log: fkener.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:17  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:46:49  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:23  fisyak
* Alice version of geant3 (-minicern)
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
*=== energy ===========================================================*
*                                                                      *
      FUNCTION FKENER(A,Z)
 
#include "geant321/dblprc.inc"
#include "geant321/dimpar.inc"
#include "geant321/iounit.inc"
C--------------------------------------------------------------------
C SUBNAME = ENERGY
C--------------------------------------------------------------------
#include "geant321/eva0.inc"
C--------------------------------------------------------------------
      KA0 = NINT ( A )
      KZ0 = NINT ( Z )
      N   = KA0 - KZ0
      IF ( N .LE. 0 ) THEN
         IF ( KA0 .NE. 1 ) THEN
            IF ( N .LT. 0 ) THEN
               WRITE ( LUNOUT, * )
     &         ' Stopped in energy: mass number =< atomic number !!',
     &           KA0, KZ0
            END IF
         ELSE
            FKENER = WAPS ( 1, 3 )
            RETURN
         END IF
      END IF
      IZZ = NINT ( WAPS ( KA0, 1 ) )
      IF ( KZ0 .LT. IZZ .OR. KZ0 .GT. IZZ + 9 ) THEN
         FKENER = ENRG ( A, Z )
         RETURN
      ELSE
         IZ0 = KZ0 - IZZ + 2
         FKENER = WAPS ( KA0, IZ0 )
         IF ( FKENER .EQ. 0.D+00 .AND. (KA0 .NE. 12 .OR. KZ0 .NE. 6) )
     &        FKENER = ENRG ( A, Z )
         RETURN
      END IF
*     RETURN
      END
