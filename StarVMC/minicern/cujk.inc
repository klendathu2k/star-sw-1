*
* $Id: cujk.inc,v 1.1 2005/06/09 00:05:11 fisyak Exp $
*
* $Log: cujk.inc,v $
* Revision 1.1  2005/06/09 00:05:11  fisyak
* Add stuff for reconstruction
*
* Revision 1.1  2005/05/26 22:28:01  fisyak
* Add stuff necessary for reconstruction packages
*
* Revision 1.1.1.1  1996/02/15 17:48:54  mclareni
* Kernlib
*
*
*
* cujk.inc
*
#if defined(CERNLIB_NUMAP)||defined(CERNLIB_NUMCD)||defined(CERNLIB_NUMCV)||defined(CERNLIB_NUMIB2)||defined(CERNLIB_NUMND)
          JU  =  (LOCF(U12) - LOCF(U)) / 2
          KU  =  (LOCF(U22) - LOCF(U)) / 2
#endif
#if defined(CERNLIB_NUMCR)||defined(CERNLIB_NUMUC)
          JU  =  (LOC(U12) - LOC(U)) / 2
          KU  =  (LOC(U22) - LOC(U)) / 2
#endif
#if defined(CERNLIB_NUMDE)
          JU  =  (%LOC(U12) - %LOC(U)) / 8
          KU  =  (%LOC(U22) - %LOC(U)) / 8
#endif
#if defined(CERNLIB_NUMIB1)
          JU  =  (IADDR(U12) - IADDR(U)) / 8
          KU  =  (IADDR(U22) - IADDR(U)) / 8
#endif
