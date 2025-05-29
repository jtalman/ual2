C     @(#)testbmbm.f	1.1       8/24/94         
C
C                    testbmbm.f
c
c *********************************************************************
c
c   SUBROUTINE trkbmbm uses the complex error function routines for
c   calculating beam-beam kicks, unless the strong beam is almost round---
c   parameter ``outofrnd'', close to 1, establishes the degree of
c   roundness required.
c
c   This program, ~tpot/ftpot/erfcomplex/testbmbm can be used to exploit
c   this as a check on the calculations. It performs essentially the same
c   calculations just inside and just outside the outofrnd boundary.
c
c *********************************************************************
c
      IMPLICIT NONE
      DOUBLE PRECISION x, y, vxibyc, vyibyc, del, dummy
      DOUBLE PRECISION dx, dy, sx, sy, nstr, amass, betgam
      INTEGER i, j, k

      DATA sx/1.0/

      DATA vxibyc/0.0/
      DATA vyibyc/0.0/
      DATA del/0.0/
      DATA dx/0.0/
      DATA dy/0.0/
      DATA nstr/1.0d14/
      DATA amass/0.000511034/
      DATA betgam/10000.0/
      DATA dummy/0.0/

C      parameter (emass = 0.000511034)
C      parameter (pmass = 0.938279600)

      DO i=1, 2
         sy=sx*1.0001 + 2.0*(i-1.5)*0.00001
         DO j=1,7
            WRITE(30,999)
 999        FORMAT(T35,'DEFLECTIONS',
     &      T6,'SX',T15,'SY',T24,'X',T30,'Y',T41,'vxibyc',
     &      T54,'DUMMY  ',T70,'vyibyc',T85,'DUMMY  ')
            DO k=1,7
               x=(j-1)*0.5
               y=(k-1)*0.5
               CALL trkbmbm(x, y, vxibyc, vyibyc, del, 
     1                  dx, dy, sx, sy, nstr, amass, betgam)
               WRITE(30,1000)sx,sy,x,y,vxibyc,dummy,vyibyc,dummy
               WRITE(6,1000)sx,sy,x,y,vxibyc,dummy,vyibyc,dummy
 1000          FORMAT(F6.1,F12.7,F6.1,F6.1,F17.9,F12.5,
     1                 F17.9,F12.5)
               vxibyc = 0.0
               vyibyc = 0.0
            ENDDO
         ENDDO
      ENDDO

      STOP
      END







