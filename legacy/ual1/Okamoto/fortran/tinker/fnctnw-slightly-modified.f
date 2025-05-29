C     @(#)fnctnw.f	1.1       8/24/94         
C
C            FNCTNW.FORTRAN
C     THIS PROGRAM EVALUATES W(ZI,ZR) ON A GRID, FOR EACH OF THE
C     METHODS OF APPROXIMATION.

      IMPLICIT none
      DOUBLE PRECISION wr, wi, zr, zi
      DOUBLE PRECISION dzr, dzi

      INTEGER i, j
      dzr=0.5
      dzi=0.5
      OPEN(25,FILE="wexct")
      WRITE(25,*)"           wexct"
C     WRITE(26,*)"           pade1"
C     WRITE(27,*)"           pade2"
C     WRITE(28,*)"           asymp"
C     WRITE(29,*)"           fcnw"
      DO i=0,6
        zr=i*dzr
        DO j=0,6
          zi=j*dzi
          CALL wexct(wr,wi,zr,zi)
          WRITE(25,1000)zr,zi,wr,wi
          CALL pade1(wr,wi,zr,zi)
C         WRITE(26,1000)zr,zi,wr,wi
C         CALL pade2(wr,wi,zr,zi)
C         WRITE(27,1000)zr,zi,wr,wi
C         CALL asymp(wr,wi,zr,zi)
C         WRITE(28,1000)zr,zi,wr,wi
C         CALL fcnw(wr,wi,zr,zi)
C         WRITE(29,1000)zr,zi,wr,wi
        ENDDO
      ENDDO
 1000 FORMAT(/1X,2F6.2,2F20.15)
      STOP
      END
