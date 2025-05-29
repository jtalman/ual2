C     @(#)gafeld.f	1.1       8/24/94         
C
c                    gafeld.fortran
c
c     this program evaluates the electric field of a
c     two-dimensional gaussian charge distribution
c
      IMPLICIT none
      DOUBLE PRECISION wr, wi, zr, zi
      DOUBLE PRECISION sx, sy, delta, check
      DOUBLE PRECISION x, y, xs, ys, e, w1r, w1i 
      DOUBLE PRECISION we1r, we1i, zi1, w2r, w2i, we2r, we2i
      DOUBLE PRECISION exexct, eyexct, ex, ey, y1
      DOUBLE PRECISION pcntx, pcnty, ab, c, wer, wei

      INTEGER ictr, i, j, k 

      DATA sx/1.e0/
      DATA delta/.002e0/,check/0.e0/
      DATA ictr/4/
      DO 10 i=1,3
        sy=.2e0*i
        ab=2.e0*(sx*sx-sy*sy)
        c=sqrt(ab)
      DO 20 j=1,7
        ictr=ictr+1
        if(ictr.lt.4)go to 1
        ictr=0
      WRITE(30,999)
 999  FORMAT(t35,'electric field',4x,
     &      t6,'sx',t12,'sy',t18,'x',t24,'y',t35,'ex',
     &      t48,'% error',t64,'ey',t77,'% error')
 1    CONTINUE
      DO 30 k=1,7
        x=.5e0*(j-1)
        y=.5e0*(k-1)
        xs=x/sx
 100    ys=y/sy
        e=exp(-xs*xs/2.e0-ys*ys/2.e0)
        zr=x/c
        zi=y/c
        CALL fcnw(wr,wi,zr,zi)
        w1r=wr
        w1i=wi
        if(1.e0-check)200,200,150
 150    CALL wexct(wer, wei, zr, zi)
        we1r=wer
        we1i=wei
        zi1=zi
 200    zr=xs*sy/c
        zi=ys*sx/c
        CALL fcnw(wr, wi, zr, zi)
        w2r=wr
        w2i=wi
        if(1.e0-check)450,450,250
 250    CALL wexct(wer,wei,zr,zi)
        we2r=wer
        we2i=wei
        exexct=(we1i-e*we2i)/c
        eyexct=(we1r-e*we2r)/c
        ex=(w1i-e*w2i)/c
        IF(y)350,300,350
 300    ey=0.e0
        GO TO 550
 350    IF(zi1-delta)400,450,450
 400    check=1.e0
        y1=y
        y=y*delta/zi1
        GO TO 100
 450    ey=(w1r-e*w2r)/c
        if(1.e0-check)500,500,550
 500    ey=ey*zi1/delta
        check=0.e0
        y=y1
 550    pcntx=100.e0*(ex-exexct)/exexct
        pcnty=100.e0*(ey-eyexct)/eyexct
        WRITE(30,1000)sx,sy,x,y,ex,pcntx,ey,pcnty
 1000   FORMAT(/1x,4f6.1,f17.9,f12.5,f17.9,f12.5)
 30   CONTINUE
 20   CONTINUE
 10   CONTINUE
      STOP
      END

