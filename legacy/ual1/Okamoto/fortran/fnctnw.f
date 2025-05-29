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
      WRITE(25,*)"           wexct"
      WRITE(26,*)"           pade1"
      WRITE(27,*)"           pade2"
      WRITE(28,*)"           asymp"
      WRITE(29,*)"           fcnw"
      DO i=0,6
        zr=i*dzr
        DO j=0,6
          zi=j*dzi
          CALL wexct(wr,wi,zr,zi)
          WRITE(25,1000)zr,zi,wr,wi
          CALL pade1(wr,wi,zr,zi)
          WRITE(26,1000)zr,zi,wr,wi
          CALL pade2(wr,wi,zr,zi)
          WRITE(27,1000)zr,zi,wr,wi
          CALL asymp(wr,wi,zr,zi)
          WRITE(28,1000)zr,zi,wr,wi
          CALL fcnw(wr,wi,zr,zi)
          WRITE(29,1000)zr,zi,wr,wi
        ENDDO
      ENDDO
 1000 FORMAT(/1X,2F6.2,2F20.15)
      STOP
      END
       
