* $Id: calp28.F,v 1.1.1.1 2004/01/15 00:08:45 potekhin Exp $
* $Name:  $
#include <commons/config.h>
      SUBROUTINE CALP28
#include "gcalor/cbert.inc"
#include "geant321/crandm.inc"
      REAL *8 R
C
      SAVE
C
      R = RANDC(ISEED)
      IF(ISW(13))230,10,230
   10 IF(R-6.0D-1)20,20,120
   20 PT(4)=PNMS
      R = RANDC(ISEED)
      IF(ISW(4))30,90,30
   30 IF(R-3.3333333D-1)40,40,70
   40 PT(26)=5.0
   50 PT(28)=PNMS
   60 RETURN
   70 PT(26)=4.0
   80 PT(38)=2.0
      GO TO 60
   90 PT(2)=5.0
      PT(14)=2.0
      IF(R-3.3333333D-1)100,100,110
  100 PT(28)=PNMS
      GO TO 80
  110 PT(26)=4.0
      GO TO 60
  120 R = RANDC(ISEED)
      IF(ISW(4))130,180,130
  130 IF(R-6.6666667D-1)140,140,160
  140 PT(2)=4.0
  150 R = RANDC(ISEED)
      IF(R-6.6666667D-1)110,110,100
  160 PT(14)=2.0
  170 PT(4)=PNMS
      GO TO 150
  180 IF(R-6.6666667D-1)190,190,220
  190 PT(2)=4.0
  200 PT(14)=2.0
  210 R = RANDC(ISEED)
      IF(R-6.6666667D-1)70,70,40
  220 PT(2)=5.0
      PT(4)=PNMS
      GO TO 210
  230 IF(R-VALUE1)240,240,270
  240 PT(4)=PNMS
      IF(ISW(4))260,250,260
  250 PT(2)=5.0
      PT(14)=2.0
      GO TO 50
  260 PT(38)=2.0
      GO TO 40
  270 R = RANDC(ISEED)
      IF(ISW(4))280,310,280
  280 IF(R-3.3333333D-1)290,290,300
  290 PT(4)=PNMS
      GO TO 200
  300 PT(2)=4.0
      GOTO210
  310 IF(R-3.3333333D-1)320,320,330
  320 PT(2)=5.0
      GOTO170
  330 PT(14)=2.0
      GOTO140
      END
