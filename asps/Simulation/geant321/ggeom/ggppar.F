*
* $Id: ggppar.F,v 1.1.1.1 2004/01/15 00:12:07 potekhin Exp $
*
* $Log: ggppar.F,v $
* Revision 1.1.1.1  2004/01/15 00:12:07  potekhin
*
* Revision 1.1.1.1  1997/11/03 15:30:01  atlascvs
* Importing CERNLIB version 08.21.
*
* Revision 1.1.1.1  1995/10/24 10:20:50  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.28  by  S.Giani
*-- Author :
      SUBROUTINE GGPPAR (JVOM, IN, NVAR, LVAR, LVOM, NPAR, PAR)
C.
C.    ******************************************************************
C.    *                                                                *
C.    *    SUBR. GGPPAR (JVOM,IN,NVAR,LVAR,LVOM,NPAR*,PAR*)            *
C.    *                                                                *
C.    *   Computes the actual parameters for the INth content inside   *
C.    *    the mother volume at address JVOM                           *
C.    *   Returns them in NPAR, PAR                                    *
C.    *                                                                *
C.    *   Called by : GGDVLP                                           *
C.    *   Authors   : F.Bruyant, S.Banerjee                            *
C.    *               (original algorithms from A.McPherson)           *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gcbank.inc"
#include "geant321/gcflag.inc"
#include "geant321/gcunit.inc"
C.
      PARAMETER (NPAMAX=50)
C.
      DIMENSION LVAR(*), PAR(*)
      DIMENSION DXYZ(3), PARM(NPAMAX)
      DIMENSION IOPT(12,12)
      SAVE IOPT
C.
      DATA IOPT /  1 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
     +             0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
     +             0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
     +             0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
     +             0 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
     +             0 ,0 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,
     +             0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,
     +             0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,
     +             0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,
     +             0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,0 ,
     +             0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,
     +             0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 /
C.
C.    ------------------------------------------------------------------
*
* *** Check request
*
      JIN  = LQ(JVOM-IN)
*
      IVO  = Q(JIN+2)
      JVO  = LQ(JVOLUM-IVO)
      NPAR = Q(JVO+5)
      IF (NPAR.EQ.0) THEN
         NPAR = Q(JIN+9)
         CALL UCOPY (Q(JIN+10), PAR, NPAR)
         IF (NVAR.LT.0) GO TO 999
      ELSE
         CALL UCOPY (Q(JVO+7), PAR, NPAR)
      ENDIF
      IROT = Q(JIN+4)
      IF (IROT.NE.0) THEN
         WRITE (CHMAIL, 10300)
         CALL GMAIL (0, 0)
         IEORUN = 1
      ENDIF
*
      ISH  = Q(JVO+2)
      ISHM = Q(JVOM+2)
      IF(ISH.GT.12.OR.ISHM.GT.12.OR.IOPT(ISHM, ISH).EQ.0) THEN
         WRITE (CHMAIL, 10400) ISH, ISHM
         CALL GMAIL (0, 0)
         IEORUN = 1
      ENDIF
*
* *** Prepares parameters for mother, in PARM, for content, in PAR
*      and the translation DXYZ (position of content inside mother)
*
      IF (LQ(JVOM).EQ.LVOM) THEN
*
*      Case when current volume is source of local development
*
         NPARM = Q(JVOM+5)
         CALL UCOPY (Q(JVOM+7), PARM, NPARM)
      ELSE
*
*      Other cases
*
         NPARM = IQ(LVOM+5)
         CALL UCOPY (Q(LVOM+6), PARM, NPARM)
      ENDIF
*
      DXYZ(1) = Q(JIN+5)
      DXYZ(2) = Q(JIN+6)
      DXYZ(3) = Q(JIN+7)
*
* *** Compute the actual parameters
*
      IF (ISH.EQ.1) THEN
*
* BOX
*
         IF (ISHM.EQ.1) THEN
* in BOX
            DO 10 I = 1,NVAR
               IAX = LVAR(I)
               PAR(IAX) = PARM(IAX) -ABS(DXYZ(IAX))
               IF (PAR(IAX).LT.0.) THEN
                  WRITE (CHMAIL, 10500) ISH, ISHM, IAX
                  CALL GMAIL (0, 0)
                  IEORUN = 1
               ENDIF
   10       CONTINUE
         ELSE IF (ISHM.EQ.2) THEN
* in TRD1
            DO 20 I = 1,NVAR
               IAX = LVAR(I)
               IF (IAX.EQ.1) THEN
                  DZ = PAR(3)
                  IF (DZ.LT.0.) DZ = PARM(4)
                  DXDZ = 0.5*(PARM(2)-PARM(1))/PARM(4)
                  DXME = 0.5*(PARM(2)+PARM(1)) +DXDZ*DXYZ(3)
                  DX   = DXME -ABS(DXDZ*DZ)
                  PAR(IAX) = DX -ABS(DXYZ(1))
                  IF (PAR(IAX).LT.0.) THEN
                     WRITE (CHMAIL, 10500) ISH, ISHM, IAX
                     CALL GMAIL (0, 0)
                     IEORUN = 1
                  ENDIF
               ELSE
                  PAR(IAX) = PARM(IAX+1) -ABS(DXYZ(IAX))
                  IF (IAX.EQ.3) THEN
                     DXDZ = 0.5*(PARM(2)-PARM(1))/PARM(4)
                     DX0  = 0.5*(PARM(2)+PARM(1)) -ABS(DXYZ(1))
                     DZ   = (DX0 -PAR(1))/ABS(DXDZ)
                     IF (DZ.LT.0.) THEN
                        WRITE (CHMAIL, 10600) ISH, ISHM, IAX, DZ
                        CALL GMAIL (0, 0)
                        IEORUN = 1
                     ELSE
                        IF (PAR(3).GT.DZ) PAR(3) = DZ
                     ENDIF
                  ENDIF
               ENDIF
   20       CONTINUE
         ELSE IF (ISHM.EQ.3) THEN
* in TRD2
            DO 30 I = 1,NVAR
               IAX   = LVAR(I)
               IF (IAX .LT. 3) THEN
                  DZ  = PAR(3)
                  IP  = 2*IAX -1
                  IF (DZ .LT. 0.) DZ = PARM(5)
                  DXDZ = 0.5*(PARM(IP+1) - PARM(IP))/PARM(5)
                  DXME = 0.5*(PARM(IP+1) + PARM(IP)) + DXDZ*DXYZ(3)
                  DX   = DXME - ABS(DXDZ*DZ)
                  PAR(IAX) = DX - ABS(DXYZ(IAX))
               ELSE
                  PAR(3) = PARM(5) - ABS(DXYZ(3))
                  DXDZ   = 0.5*(PARM(2) - PARM(1))/PARM(5)
                  DX0    = 0.5*(PARM(2) + PARM(1)) - ABS(DXYZ(1))
                  DZ     = (DX0 - PAR(1))/ABS(DXDZ)
                  IF (DZ.LT.0.0) THEN
                     WRITE (CHMAIL, 10600) ISH, ISHM, IAX, DZ
                     CALL GMAIL (0, 0)
                     IEORUN = 1
                  ELSE
                     IF (PAR(IAX).GT.DZ) PAR(IAX) = DZ
                  ENDIF
                  DXDZ   = 0.5*(PARM(4) - PARM(3))/PARM(5)
                  DX0    = 0.5*(PARM(4) + PARM(3)) - ABS(DXYZ(2))
                  DZ     = (DX0 - PAR(2))/ABS(DXDZ)
                  IF (DZ.LT.0.0) THEN
                     WRITE (CHMAIL, 10600) ISH, ISHM, IAX, DZ
                     CALL GMAIL (0, 0)
                     IEORUN = 1
                  ELSE
                     IF (PAR(IAX).GT.DZ) PAR(IAX) = DZ
                  ENDIF
               ENDIF
   30       CONTINUE
         ELSE IF (ISHM.EQ.4) THEN
* in TRAP
*
*        Case of box in trap. Let's keep it simple: we just deal with
*        the case in which phi=0 or 180. If theta .ne. 0, the position along
*        x-axis, and the angles parm(7) and parm(11) must be 0.
*
            IF(ABS(PARM(3)).GT.0.1E-5) THEN
               WRITE(CHMAIL,10000) IQ(JVOLUM+IVO)
10000          FORMAT
     +      (' GGPPAR : Cannot use negative parameters for box ',A4)
               CALL GMAIL(0,0)
               WRITE(CHMAIL,10100)
10100       FORMAT('          in a trap if PAR(2) .ne. 0 or 180')
               CALL GMAIL(0,0)
               IEORUN=1
            ELSEIF(PARM(7).NE.PARM(11)) THEN
               WRITE(CHMAIL,10000) IQ(JVOLUM+IVO)
               CALL GMAIL(0,0)
               WRITE(CHMAIL,10200)
10200       FORMAT('          in a trap if PAR(7) .ne. PAR(11)')
               CALL GMAIL(0,0)
            ELSE
               IVARX=0
               IVARY=0
               IVARZ=0
               DO 40 J=1,NVAR
                  IF(LVAR(J).EQ.1) THEN
                     IVARX=1
                  ELSEIF(LVAR(J).EQ.2) THEN
                     IVARY=1
                  ELSEIF(LVAR(J).EQ.3) THEN
                     IVARZ=1
                  ENDIF
   40          CONTINUE
               DZM = PARM(1)
               IF(IVARZ.EQ.1) THEN
                  PAR(3) = DZM-ABS(DXYZ(3))
               ENDIF
               DYDZ=0.5*(PARM(8)-PARM(4))/PARM(1)
               DYM =0.5*(PARM(8)+PARM(4))
               DY1 = DYM+(DXYZ(3)+PAR(3))*DYDZ
               DY2 = DYM+(DXYZ(3)-PAR(3))*DYDZ
               IF(IVARY.EQ.1) THEN
                  PAR(2) = MIN(DY1,DY2)-ABS(DXYZ(2))
               ENDIF
               IF(IVARX.EQ.1) THEN
                  IF(PARM(7).EQ.0.0.AND.ABS(PARM(2)).LT..1E-5) THEN
                     DXDZL = 0.5*(PARM(9)-PARM(5))/PARM(1)
                     DXDZH = 0.5*(PARM(10)-PARM(6))/PARM(1)
                     DXML = 0.5*(PARM(9)+PARM(5))
                     DXMH = 0.5*(PARM(10)+PARM(6))
                     DXL1 = DXML+(DXYZ(3)+PAR(3))*DXDZL
                     DXL2 = DXML+(DXYZ(3)-PAR(3))*DXDZL
                     DXH1 = DXMH+(DXYZ(3)+PAR(3))*DXDZH
                     DXH2 = DXMH+(DXYZ(3)-PAR(3))*DXDZH
                     DXDY1 = 0.5*(DXH1-DXL1)/DY1
                     DXDY2 = 0.5*(DXH2-DXL2)/DY2
                     DXM1 = 0.5*(DXH1+DXL1)
                     DXM2 = 0.5*(DXH2+DXL2)
                     DX1 = DXM1+(DXYZ(2)+PAR(2))*DXDY1
                     DX2 = DXM1+(DXYZ(2)-PAR(2))*DXDY1
                     DX3 = DXM2+(DXYZ(2)+PAR(2))*DXDY2
                     DX4 = DXM2+(DXYZ(2)-PAR(2))*DXDY2
                     PAR(1) = MIN(DX1,DX2,DX3,DX4)-ABS(DXYZ(1))
* Note; position along x-axis should be 0, when theta .ne. 0:
                  ELSE IF(PARM(7).EQ..0.AND.ABS(PARM(2)).GT..1E-5)THEN
* the maximum length of the lower (DX2) and upper (DX1) lines along x
                     DXDZ= 0.5*(PARM(9)-PARM(5))/PARM(1)
                     DXM = 0.5*(PARM(9)+PARM(5))
                     DX1 = DXM+(DXYZ(3)+PAR(3))*DXDZ
                     DX2 = DXM+(DXYZ(3)-PAR(3))*DXDZ
*
* the shift in the endpoints caused by theta angle compared with
* the symmetrical case when theta = 0
                     TANTHE = PARM(2)
                     SHFX1 = TANTHE*(DXYZ(3)+PAR(3))
                     SHFX2 = TANTHE*(DXYZ(3)-PAR(3))
*
                     DX1P = DX1 + SHFX1
                     DX1N = DX1 - SHFX1
                     DX2P = DX2 + SHFX2
                     DX2N = DX2 - SHFX2
*
* DXP is the lenght of the box that fits in the positive side
* DXN is the one in the negative side
                     DXP  = MIN(DX1P,DX2P)
                     DXN  = MIN(DX1N,DX2N)
                     XPOS = .5*(DXP-DXN)
                     Q(JIN+5) = XPOS
                     PAR(1) = .5*(DXP+DXN)
                  ELSE
                     TANALF = PARM(7)
                     DXDZL = 0.5*(PARM(9)-PARM(5))/PARM(1)
                     DXDZH = 0.5*(PARM(10)-PARM(6))/PARM(1)
                     DXML = 0.5*(PARM(9)+PARM(5))
                     DXMH = 0.5*(PARM(10)+PARM(6))
                     DXL1 = DXML+(DXYZ(3)+PAR(3))*DXDZL
                     DXL2 = DXML+(DXYZ(3)-PAR(3))*DXDZL
                     DXH1 = DXMH+(DXYZ(3)+PAR(3))*DXDZH
                     DXH2 = DXMH+(DXYZ(3)-PAR(3))*DXDZH
                     SHFX1 = TANALF*DY1
                     SHFX2 = TANALF*DY2
*
                     DXH1P = DXH1+SHFX1
                     DXH1N = DXH1-SHFX1
                     DXL1P = DXL1-SHFX1
                     DXL1N = DXL1+SHFX1
                     DXH2P = DXH2+SHFX2
                     DXH2N = DXH2-SHFX2
                     DXL2P = DXL2-SHFX2
                     DXL2N = DXL2+SHFX2
*
                     DXDY1P = 0.5*(DXH1P-DXL1P)/DY1
                     DXDY2P = 0.5*(DXH2P-DXL2P)/DY2
                     DXDY1N = 0.5*(DXH1N-DXL1N)/DY1
                     DXDY2N = 0.5*(DXH2N-DXL2N)/DY2
*
                     DXM1P = 0.5*(DXH1P+DXL1P)
                     DXM2P = 0.5*(DXH2P+DXL2P)
                     DXM1N = 0.5*(DXH1N+DXL1N)
                     DXM2N = 0.5*(DXH2N+DXL2N)
*
                     DX1P = DXM1P+(DXYZ(2)+PAR(2))*DXDY1P
                     DX2P = DXM1P+(DXYZ(2)-PAR(2))*DXDY1P
                     DX3P = DXM2P+(DXYZ(2)+PAR(2))*DXDY2P
                     DX4P = DXM2P+(DXYZ(2)-PAR(2))*DXDY2P
                     DX1N = DXM1N+(DXYZ(2)+PAR(2))*DXDY1N
                     DX2N = DXM1N+(DXYZ(2)-PAR(2))*DXDY1N
                     DX3N = DXM2N+(DXYZ(2)+PAR(2))*DXDY2N
                     DX4N = DXM2N+(DXYZ(2)-PAR(2))*DXDY2N
*
                     PAR(1) =MAX(0.,MIN( MIN(DX1P,DX2P,DX3P,DX4P)-
     +               DXYZ(1), MIN(DX1N,DX2N,DX3N,DX4N)+DXYZ(1)))
                  ENDIF
               ENDIF
            ENDIF
         ENDIF
*
      ELSE IF (ISH.EQ.4) THEN
*
* TRAP
*
         IF (ISHM.EQ.ISH) THEN
* in TRAP
            IF (PAR(1).EQ.PARM(1)) THEN
               IZCUT = 0
            ELSE
               IZCUT = 1
            ENDIF
            DO 50 I = 1,NVAR
               IAX = LVAR(I)
               PAR(IAX) = PARM(IAX)
               IF (IZCUT.EQ.0) GO TO 50
               IF (IAX.NE.1.AND.IAX.LE.6) THEN
                  DZ   = DXYZ(3) +PARM(1) -PAR(1)
                  DPDZ = 0.5*(PARM(IAX+4) -PARM(IAX))/PARM(1)
                  PAR(IAX) = PARM(IAX) + DPDZ*DZ
               ELSE IF (IAX.GT.6) THEN
                  DZ   = DXYZ(3) +PARM(1) +PAR(1)
                  DPDZ = 0.5*(PARM(IAX) -PARM(IAX-4))/PARM(1)
                  PAR(IAX) = PARM(IAX-4) + DPDZ*DZ
               ENDIF
   50       CONTINUE
            IF (IZCUT.NE.0) THEN
               HTAH = PARM(8)*PARM(11)
               HTAL = PARM(4)*PARM(7)
               ZZ1  = 0.5*(DXYZ(3) +PARM(1) -PAR(1))/PARM(1)
               PAR(7) = (HTAL*(1.-ZZ1) + HTAH*ZZ1)/PAR(4)
               ZZ2  = 0.5*(DXYZ(3) +PARM(1) +PAR(1))/PARM(1)
               PAR(11)= (HTAL*(1.-ZZ2) + HTAH*ZZ2)/PAR(8)
            ENDIF
            IF(IAX.EQ.2.AND.IZCUT.EQ.0) THEN
               DXDY1 = 0.5*(PARM(6)-PARM(5))/PARM(4)
               DXDY2 = 0.5*(PARM(10)-PARM(9))/PARM(8)
               DXM1  = 0.5*(PARM(6)+PARM(5))
               DXM2  = 0.5*(PARM(10)+PARM(9))
               DXH1  = DXM1+(DXYZ(2)+PAR(2))*DXDY1
               DXH2  = DXM2+(DXYZ(2)+PAR(2))*DXDY2
               DXL1  = DXM1+(DXYZ(2)-PAR(2))*DXDY1
               DXL2  = DXM2+(DXYZ(2)-PAR(2))*DXDY2
               PAR(5) = DXL1
               PAR(6) = DXH1
               PAR(9) = DXL2
               PAR(10) = DXH2
            ENDIF
         ENDIF
         CALL GNOTR1 (PAR)
*
      ELSE IF (ISH.EQ.5) THEN
*
* TUBE
*
         IF(ISHM.EQ.ISH) THEN
            IRMIN = 0
            IRMAX = 0
            IDZED = 0
            DO 60 JVAR=1,NVAR
               IF(LVAR(JVAR).EQ.1) IRMIN=1
               IF(LVAR(JVAR).EQ.2) IRMAX=1
               IF(LVAR(JVAR).EQ.3) IDZED=1
   60       CONTINUE
            RPOS = SQRT(DXYZ(1)**2+DXYZ(2)**2)
            IF(IRMIN.EQ.1) THEN
*
* Here we settle the minimum radius.
*
               IF(PARM(1).GT.0.) THEN
                  PAR(1) = PARM(1)+RPOS
               ELSE
                  PAR(1) = 0.
               ENDIF
            ENDIF
            IF(IRMAX.EQ.1) THEN
*
* Case in which the max radius is variable.
*
               IF(PAR(1).LE.RPOS-PARM(1).AND.PARM(1).GT.0.) THEN
*
* This is the case in which the 'hole' in the tube does not
* intersect the 'hole' in the mother.
*
                  PAR(2) = MIN(PARM(2)-RPOS,RPOS-PARM(1))
               ELSEIF(PAR(1).GE.RPOS+PARM(1).OR.PARM(1).EQ.0.) THEN
*
* This is the case in which the 'hole' in the tube contains the
* 'hole' in the mother, or there is no 'hole' in the mother.
*
                  PAR(2) = PARM(2)-RPOS
               ELSE
*
* And this is the error condition. The inner tube protrudes in the empty
* space inside the outer one.
*
                  WRITE(CHMAIL,11100) IQ(JVOLUM+IVO)
                  CALL GMAIL(0,0)
                  IEORUN = 1
               ENDIF
            ENDIF
            IF(IDZED.EQ.1) THEN
               PAR(3) = PARM(3)-ABS(DXYZ(3))
            ENDIF
            IF(PAR(1).GE.PAR(2).OR.PAR(2).GT.PARM(2)
     +                         .OR.PAR(3).LE.0.) THEN
               WRITE(CHMAIL,11000) IQ(JVOLUM+IVO)
               CALL GMAIL(0, 0)
               IEORUN = 1
            ENDIF
         ELSE
            WRITE (CHMAIL, 10900) ISH, ISHM
            CALL GMAIL (0, 0)
            IEORUN=1
         ENDIF
      ELSE IF (ISH.LE.10) THEN
*
* TRD1,TRD2,TUBE,TUBS,CONE,CONS,SPHE,PARA
*
         IF (ISHM.EQ.ISH) THEN
* in TRD1,TRD2,TUBE,TUBS,CONE,CONS,SPHE,PARA
            IF (DXYZ(1).NE.0..OR.DXYZ(2).NE.0..OR.DXYZ(3).NE.0.)
     +      THEN
               WRITE (CHMAIL, 10700) ISH, ISHM
               CALL GMAIL (0, 0)
               IEORUN = 1
            ELSE
               DO 70 I= 1, NVAR
                  IAX = LVAR(I)
                  PAR(IAX) = PARM(IAX)
   70          CONTINUE
            ENDIF
         ENDIF
*
      ELSE IF (ISH.LE.12) THEN
*
* PGON,PCON
*
         IF (ISHM.EQ.ISH) THEN
* in PGON,PCON
            IF (ISH.EQ.11) THEN
               IPNZ = 4
            ELSE
               IPNZ = 3
            ENDIF
            NZ = PAR(IPNZ)
            NZ1 = PARM(IPNZ)
            IF (NZ.NE.2 .OR. NZ1.NE.2) THEN
               WRITE (CHMAIL, 10900) ISH, ISHM
               CALL GMAIL (0, 0)
               IEORUN = 1
            ELSE
               ZL = PARM(IPNZ+1)
               ZH = PARM(IPNZ+4)
               DZ = ZH - ZL
               TANLOW = (PARM(IPNZ+5)-PARM(IPNZ+2))/DZ
               TANHIG = (PARM(IPNZ+6)-PARM(IPNZ+3))/DZ
               Z1 = DXYZ(3) + PAR(IPNZ+1) - PARM(IPNZ+1)
               Z2 = DXYZ(3) + PAR(IPNZ+4) - PARM(IPNZ+1)
               PAR(IPNZ+2) = PARM(IPNZ+2) + TANLOW * Z1
               PAR(IPNZ+3) = PARM(IPNZ+3) + TANHIG * Z1
               PAR(IPNZ+5) = PARM(IPNZ+2) + TANLOW * Z2
               PAR(IPNZ+6) = PARM(IPNZ+3) + TANHIG * Z2
            ENDIF
         ENDIF
*
      ENDIF
*
10300 FORMAT (' GGPPAR : Rotations not accepted at the moment')
10400 FORMAT (' GGPPAR : Shape association not accepted', 2I5)
10500 FORMAT (' GGPPAR : PAR(IAX) negative, ISH,ISHM,IAX=',3I5)
10600 FORMAT (' GGPPAR : DZ negative, ISH,ISHM,IAX,DZ=',3I5,G12.4)
10700 FORMAT (' GGPPAR : Not yet coded for ISH,ISHM=',2I5)
10800 FORMAT (' GGPPAR : PARM error for ISH,ISHM,IAX=',3I5)
10900 FORMAT (' GGPPAR : Configuration not accepted, ISH,ISHM=',2I5)
11000 FORMAT (' GGPPAR : Tube ',A4,' has inconsistent parameters')
11100 FORMAT (' GGPPAR : Tube ',A4,' protrudes into the inner space ',
     +        'of the mother')
*                                                             END GGPPAR
  999 END
