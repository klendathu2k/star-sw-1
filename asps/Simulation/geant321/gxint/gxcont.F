*
* $Id: gxcont.F,v 1.1.1.1 2004/01/15 00:12:18 potekhin Exp $
*
* $Log: gxcont.F,v $
* Revision 1.1.1.1  2004/01/15 00:12:18  potekhin
*
* Revision 1.3  2001/11/20 21:37:02  nevski
* gcdraw variables correctly described
*
* Revision 1.2  1998/02/10 13:05:15  atgeant3
* *** empty log message ***
*
*
#include "geant321/pilot.h"
      SUBROUTINE GXCONT
**
**    ******************************************************************
**    *                                                                *
**    *      General control commands                                  *
**    *                                                                *
**    *       Authors:   R.Brun      **********                        *
**    *                  P.Zanarini  **********                        *
**    *                  S.Giani     **********                        *
**    *                                                                *
**    * Modifications:                                 PN, 06 July 96  *
**    *  - strong typing introduced to find undefined variables        *
**    *  - GCTIME common was missing                                   *
**    *  - SPART command corrected ( a la NA49 and Star): Ubuf,        *
**    *    branching ratios and decay modes are KUIP vectors!          *
**    *  - RNDM command with single parameter ISEQ (1-215) recals      *
**    *    standard precomputed sequences.                             *
**    *    If ISEQ=0, sequence is elected according to IGTIME.         *
**    *  - Time control introduced - upon the limit IQUEST(1) is set   *
**    ******************************************************************
**
#include "commons/typing.inc"
#include "commons/quest.inc"
#include "geant321/pawc.inc"
#include "geant321/gcbank.inc"
#include "geant321/gctmed.inc"
#include "geant321/gcunit.inc"
#include "geant321/gcflag.inc"
#include "geant321/gckine.inc"
#include "geant321/gcphys.inc"
#include "geant321/gcmulo.inc"
#include "geant321/gclist.inc"
#include "geant321/gcstak.inc"
#include "geant321/gcdraw.inc"
#include "geant321/gctime.inc"
*
      Integer      IGIWTY,IwkSty,i,N,Npar,Npk,Nch,Level,Numb,Imat,
     +             NumHit,Llow,Lhigh,Llo1,Lhig1,Llo2,Lhig2,NW,
     +             Idm,Nlmat,Nmec,Kmec,ItMed,Is1,Is2
      Real         A,Z,Dens,RadL,AbsL,ParVal,TimNow,T
      CHARACTER*20 CHNAME
      CHARACTER*4  CHMECA,CHDEB,CHSET,CHDET,CHMECS(20)
      CHARACTER*32 CHPATL,CHPAR,Cmate,Cpart,Cvec
      External     IGIWTY
**
**    ------------------------------------------------------------------
**
      IWKSTY = IGIWTY(1)
      CALL KUPATL(CHPATL,NPAR)
*
      IF (CHPATL.EQ.'KINE') THEN
         CALL KUGETI(IKINE)
         NPK=NPAR-1
         DO 10 I=1,NPK
            CALL KUGETR(PKINE(I))
   10    CONTINUE
*
      ELSEIF (CHPATL.EQ.'SORD') THEN
         CALL KUGETI(ISTORD)
*
*     ELSEIF (CHPATL.EQ.'PATR') THEN
*        CALL KUGETI(NJTMAX)
*        CALL KUGETI(NJTMIN)
*        WRITE(CHMAIL,'('' *** GXCONT: Parallel tracking disabled, '',
*    +                  ''see documentation'')')
*        CALL GMAIL(1,1)
*
      ELSEIF (CHPATL.EQ.'RUNG') THEN
         CALL KUGETI(IDRUN)
         CALL KUGETI(IDEVT)
*
      ELSEIF (CHPATL.EQ.'GTIME') THEN
         CALL KUGETR(T)
         CALL KUGETR(TIMEND)
         CALL KUGETI(ITIME)
*
      ELSEIF (CHPATL.EQ.'TRACK') THEN
         IF(JXYZ.NE.0)CALL MZDROP(IXDIV,JXYZ,' ')
         IF(JHITS.NE.0)CALL MZDROP(IXDIV,JHITS,' ')
         CALL MZGARB(IXDIV,0)
         CALL GTREVE
*
      ELSEIF (CHPATL.EQ.'TRIGGER') THEN
         CALL KUGETI(N)
         IF ( NEVENT .EQ. 10000000)  NEVENT=0
         NEVENT   = NEVENT + N
         IQUEST(1)= 0
         IEORUN   = 0
*
         DO I=1,N
            CALL GTRIGC
            CALL TIMEL(TIMNOW)

*          Check time left
            IF (ITIME.GT.0 .and. TIMNOW.LE.TIMEND) Then
               WRITE(CHMAIL,10001) TIMEND
10001          FORMAT(' ***** THE JOB STOPS NOW because the TIME',
     >                ' left is less than ',F12.3,' seconds *****')
               CALL GMAIL(0,1)
               IQUEST(1) = 1
               Return
            ELSE
               IEVENT=IEVENT+1
               CALL GTRIGI
               CALL GRNDMQ(IQ(JRUNG+19),IQ(JRUNG+20),0,'G')
               IQ(JRUNG+29)=IEVENT
               CALL GTRIG
            ENDIF
*
            IF(IEORUN.NE.0) Then
               WRITE(CHMAIL,10002) TIMNOW
10002          FORMAT(' ***** THE JOB STOPS NOW because the IEORUN',
     >                ' flag set by user, Time left is ',F12.3,' *****')
               CALL GMAIL(0,1)
               IQUEST(1) = 1
               return
            ENDIF
*
            IF(IQUEST(1).NE.0) Then
               WRITE(CHMAIL,10003) TIMNOW
10003          FORMAT(' ***** THE JOB STOPS NOW because the IQUEST',
     >             ' flag set by program, Time left is ',F12.3,' *****')
               CALL GMAIL(0,1)
               IEORUN = 1
               return
            ENDIF
         ENDDO
*
      ELSEIF (CHPATL.EQ.'SWITCH') THEN
         CALL KUGETI(I)
         IF(I.GE.1.AND.I.LE.10) THEN
            IF(NPAR.LT.2)THEN
               WRITE(CHMAIL,'('' ISWIT('',I2,'')= '',I3)')I,ISWIT(I)
               CALL GMAIL(0,0)
            ENDIF
            CALL KUGETI(ISWIT(I))
         ENDIF
*
      ELSEIF (CHPATL.EQ.'PRINT') THEN
         CALL KUGETC(CHNAME,NCH)
         N=0
         CALL KUGETI(N)
         CALL GPRINT(CHNAME,N)
*
      ELSEIF (CHPATL.EQ.'DEBUG') THEN
         CALL KUGETC(CHDEB,NCH)
         IF(CHDEB.EQ.'ON')THEN
            IDEBUG=1
            IDEMIN=1
            IDEMAX=1000000
            ITIME=1
         ELSE
            IDEBUG=0
            IDEMIN=0
            IDEMAX=0
         ENDIF
*
      ELSEIF (CHPATL.EQ.'OUTPUT_LP') THEN
         CALL KUGETI(LOUT)
*
      ELSEIF (CHPATL.EQ.'PHITS') THEN
         CALL KUGETC(CHSET,NCH)
         CALL KUGETC(CHDET,NCH)
         CALL KUGETI(NUMHIT)
         IPKHIT=NUMHIT
         CALL GPHITS(CHSET,CHDET)
         IPKHIT=0
*
      ELSEIF (CHPATL.EQ.'PDIGI') THEN
         CALL KUGETC(CHSET,NCH)
         CALL KUGETC(CHDET,NCH)
         CALL GPDIGI(CHSET,CHDET)
*
      ELSEIF (CHPATL.EQ.'MZLOGL') THEN
         IXSTOR=0
         LEVEL=0
         CALL KUGETI(LEVEL)
         CALL MZLOGL(IXSTOR,LEVEL)
*
      ELSEIF (CHPATL.EQ.'PMATE') THEN
         CALL KUGETI(NUMB)
         IF(IWKSTY.GE.1.AND.IWKSTY.LE.10) THEN
            CALL GPMATX(NUMB)
         ELSE
            CALL GPMATE(NUMB)
         ENDIF
*
      ELSEIF (CHPATL.EQ.'SMATE') THEN
         CALL KUGETI(IMAT)
         CALL KUGETC(Cmate,NCH)
         CALL KUGETR(A)
         CALL KUGETR(Z)
         CALL KUGETR(DENS)
         CALL KUGETR(RADL)
         CALL KUGETR(ABSL)
         CALL KUGETV(Cvec,LLOW,LHIGH)
         print *,' Cvec=',Cvec
         CALL KUGETI(NW)
         CALL GSMATE(IMAT,Cmate,A,Z,DENS,RADL,ABSL,QQ(LLOW),NW)
*
      ELSEIF (CHPATL.EQ.'SMIXT') THEN
         CALL KUGETI(IMAT)
         CALL KUGETC(Cmate,NCH)
         CALL KUGETV(Cvec,LLOW,LHIGH)
         CALL KUGETV(Cvec,LLO1,LHIG1)
         CALL KUGETR(DENS)
         CALL KUGETI(NLMAT)
         CALL KUGETV(Cvec,LLO2,LHIG2)
         CALL GSMIXT(IMAT,Cmate,QQ(LLOW),QQ(LLO1),DENS,NLMAT,QQ(LLO2))
*
      ELSEIF (CHPATL.EQ.'PRMAT') THEN
         CALL KUGETI(Imat)
         CALL KUGETI(IPART)
         CALL KUGETC(CHMECA,NCH)
         CALL GPRMAT(Imat,IPART,CHMECA,NEKBIN,ELOW)
*
      ELSEIF (CHPATL.EQ.'DRMAT') THEN
         CALL KUGETI(Imat)
         CALL KUGETI(IPART)
         IF(NPAR.GT.2) THEN
            NMEC = NPAR-2
            DO 11 KMEC=1,NMEC
               CALL KUGETC(CHMECS(KMEC),NCH)
  11        CONTINUE
         ELSE
            NMEC = 1
            CHMECS(1)='ALL'
         ENDIF
         CALL GDRMAT(Imat,IPART,CHMECS,NMEC)
*
      ELSEIF (CHPATL.EQ.'PLMAT') THEN
         CALL KUGETI(Imat)
         CALL KUGETI(IPART)
         CALL KUGETC(CHMECA,NCH)
         IDM=0
         CALL KUGETI(IDM)
         CALL GPLMAT(Imat,IPART,CHMECA,NEKBIN,ELOW,IDM)
*
      ELSEIF (CHPATL.EQ.'SPART') THEN
         CALL KUGETI(IPART)
         CALL KUGETC(Cpart,NCH)
         CALL KUGETI(ITRTYP)
         CALL KUGETR(AMASS)
         CALL KUGETR(CHARGE)
         CALL KUGETR(TLIFE)
         CALL KUGETV(Cvec,LLOW,LHIGH)
         CALL KUGETI(NW)
         CALL GSPART(IPART,Cpart,ITRTYP,AMASS,CHARGE,TLIFE,QQ(LLOW),NW)
         CALL KUGETV(CVEC,LLO1,LHIG1)
         CALL KUGETV(CVEC,LLO2,LHIG2)
         CALL GSDK  (IPART,QQ(LLO1),IQQ(LLO2))
*
      ELSEIF (CHPATL.EQ.'PPART') THEN
         CALL KUGETI(NUMB)
         IF(IWKSTY.GE.1.AND.IWKSTY.LE.10) THEN
            CALL GPPARX(NUMB)
         ELSE
            CALL GPPART(NUMB)
         ENDIF
*
      ELSEIF (CHPATL.EQ.'PRKINE') THEN
         CALL KUGETI(NUMB)
         IF(IWKSTY.GE.1.AND.IWKSTY.LE.10) THEN
            CALL GPKINX(NUMB)
         ELSE
            CALL GPKINE(NUMB)
         ENDIF
*
      ELSEIF (CHPATL.EQ.'STPAR') THEN
         CALL KUGETI(ITMED)
         CALL KUGETC(CHPAR,NCH)
         CALL KUGETR(PARVAL)
         CALL GSTPAR(ITMED,CHPAR,PARVAL)
*
      ELSEIF (CHPATL.EQ.'RNDM') THEN
         IF (NPAR.EQ.0) THEN
            CALL GRNDMQ(IS1,IS2,0,'G')
            WRITE(CHMAIL, 10000) IS1, IS2
10000       FORMAT('  RANDOM NUMBER = ',2I12)
            CALL GMAIL(0,0)
         ELSEIF (NPAR.EQ.1) THEN
            CALL KUGETI(IS1)
            If (IS1.LE.0) IS1=IGTIME
            IS2 = mod(IS1, 215 ) + 1
            CALL GRNDMQ( 0, 0, IS1, ' ')
            CALL GRNDMQ(NRNDM(1),NRNDM(2),0,'G')
         ELSEIF (NPAR.EQ.2) THEN
            CALL KUGETI(NRNDM(1))
            CALL KUGETI(NRNDM(2))
            CALL GRNDMQ(NRNDM(1),NRNDM(2),0,'S')
         ELSE
            WRITE(CHMAIL, 10100)
            CALL GMAIL(0,0)
10100       FORMAT(' Please give up to 2 seeds to initialize',
     +             ' the random number generator')
         ENDIF
      ENDIF
*
   30 END

