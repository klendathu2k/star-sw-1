*
* $Id: mbytor.F,v 1.1 2005/06/09 00:05:15 fisyak Exp $
*
* $Log: mbytor.F,v $
* Revision 1.1  2005/06/09 00:05:15  fisyak
* Add stuff for reconstruction
*
* Revision 1.1  2005/05/26 22:28:11  fisyak
* Add stuff necessary for reconstruction packages
*
* Revision 1.1.1.1  1996/02/15 17:50:13  mclareni
* Kernlib
*
*
#include "kerngen/pilot.h"
#if defined(CERNLIB_QMSUN)
#include "sungs/mbytor.F"
#else
      FUNCTION MBYTOR (MZ,IZW,IZP,NZB)
C
C CERN PROGLIB# M421    MBYTOR          .VERSION KERNFOR  4.23  891215
C ORIG. 13/03/89  JZ
C
C     This non-ANSI code is a default which may be slow, if so
C     it should be replaced by a machine-specific fast routine
#include "kerngen/wordsize.inc"
#if !defined(CERNLIB_QISASTD)
#include "kerngen/q_andor.inc"
#include "kerngen/q_shift.inc"
* Ignoring t=pass
#endif

#if !defined(CERNLIB_QISASTD)
      MBYTOR = IOR (IZW, ISHFTR(ISHFTL(MZ,NBITPW-NZB),
     +                           NBITPW+1-IZP-NZB) )
#endif
#if defined(CERNLIB_QISASTD)
      MBYTOR = IOR (IZW, ISHFT (ISHFT (MZ,NBITPW-NZB),
     +                          -NBITPW-1+IZP+NZB) )
#endif
      RETURN
      END
#endif
