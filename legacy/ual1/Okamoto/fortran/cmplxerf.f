c    @(#)cmplxerf.f	1.2       11/25/95
C
c *********************************************************************
c   
c                                 2   2  2
c                            -(1-r )(u +v )
c      f(u,v,r) = w(u+irv) - e              w(ru+iv)
c
c *********************************************************************
c
      SUBROUTINE fbbclc(fr, fi, u, v, r)
      IMPLICIT NONE
      DOUBLE PRECISION fr, fi, u, v, r
      DOUBLE PRECISION emax, wr1, wi1, wr2, wi2, arg, expon
      PARAMETER (emax=30.)
      CALL fcnw(wr1,wi1,u,r*v)
      CALL fcnw(wr2,wi2,r*u,v)
      arg=(1.0-r**2)*(u**2+v**2)
      IF(arg.GT.emax) THEN
        arg = emax
        wr2 = 0.0
        wi2 = 0.0
      ENDIF
      expon=exp(-arg)
c
      fr=wr1-expon*wr2
      fi=wi1-expon*wi2
      RETURN
      END

c          this program evaluates the function w(z)
c       (where z = zr +  zi) in the first quadrant of
c       the complex plane (i.e. zr > 0 and zi > 0).
c       three different expressions, pade1, pade2, and
c       asymp, are used, depending on where z lies in
c       the quadrant.
c
      SUBROUTINE fcnw(wr, wi, zr, zi)
      IMPLICIT NONE
      DOUBLE PRECISION wr, wi, zr, zi
      DOUBLE PRECISION x1, x2, x3, x4, x5
      DOUBLE PRECISION y1, y2, r2
      DOUBLE PRECISION eps1, yc, yc1, yc2 

      CHARACTER sccsid*60
      sccsid = ' @(#)cmplxerf.f	1.2       11/25/95 '
 
      DATA x1/4.1e0/,x2/3.6e0/,x3/3.5e0/,x4/2.7e0/,x5/2.2e0/
      DATA y1/1.275e0/,y2/1.095e0/
      DATA r2/8.7025e0/
      IF(zr-x1)200,30,30
200   eps1=.0625e0*(zr-x3)
      IF(zr-x2)300,210,210
210   yc=-1.4e0*(zr-x2)+y2
      IF(zi-yc)220,30,30
220   IF(zr*zi.LT.eps1)THEN
      CALL asymp(wr,wi,zr,zi)
      ELSE
      CALL pade2(wr,wi,zr,zi)
      ENDIF
300   IF(zr-x4)400,310,310
310   yc=-.2e0*(zr-x4)+y1
      IF(zi-yc)320,30,30
320   IF(zr.GE.x3.and.zr*zi.LT.eps1) THEN
      CALL asymp(wr,wi,zr,zi)
      ELSE
      CALL pade2(wr,wi,zr,zi)
      ENDIF
400   IF(zr-x5)500,410,410
410   yc1=-1.4e0*(zr-x4)+y1
      yc2=1.75e0*(zr-x4)+y1
      IF(zi-yc1)420,30,30
420   IF(zi-yc2)20,10,10
500   IF(zr*zr+zi*zi-r2)10,30,30
10    CALL pade1(wr,wi,zr,zi)
      RETURN
20    CALL pade2(wr,wi,zr,zi)
      RETURN
30    CALL asymp(wr,wi,zr,zi)
      RETURN
      END
     
c                 wasymp.fortran
c
c          This program calculates an asymptotic expression of
c       w(z) valid away from the origin.
c
      SUBROUTINE asymp(wr, wi, zr, zi)
      IMPLICIT NONE
      DOUBLE PRECISION wr, wi, zr, zi
      DOUBLE PRECISION a1p, a2p, a3p, a4p, a5p
      DOUBLE PRECISION b1, b2, b3, b4, b5
      DOUBLE PRECISION pi2, x1, x2
      DOUBLE PRECISION eps, check
      DOUBLE PRECISION dr1, d1r, dr2, d2r, dr3, d3r, dr4, d4r, dr5, d5r
      DOUBLE PRECISION de1, d1e, de2, d2e, de3, d3e, de4, d4e, de5, d5e
      DOUBLE PRECISION eps1, wi0, zi0

      DATA a1p/1.94443615e-1/,a2p/7.64384940e-2/,
     1   a3p/1.07825546e-2/,a4p/4.27695730e-4/,a5p/2.43202531e-6/
      DATA b1/3.42901327e-1/,b2/1.036610830e0/,b3/1.756683649e0/,
     1   b4/2.532731674e0/,b5/3.436159119e0/
      DATA pi2/1.12837917e0/
      DATA x1/3.5e0/,x2/4.2e0/
      DATA eps/.01e0/,check/0.e0/
10    dr1=zr+b1
      d1r=zr-b1
      dr2=zr+b2
      d2r=zr-b2
      dr3=zr+b3
      d3r=zr-b3
      dr4=zr+b4
      d4r=zr-b4
      dr5=zr+b5
      d5r=zr-b5
      de1=dr1*dr1+zi*zi
      d1e=d1r*d1r+zi*zi
      de2=dr2*dr2+zi*zi
      d2e=d2r*d2r+zi*zi
      de3=dr3*dr3+zi*zi
      d3e=d3r*d3r+zi*zi
      de4=dr4*dr4+zi*zi
      d4e=d4r*d4r+zi*zi
      de5=dr5*dr5+zi*zi
      d5e=d5r*d5r+zi*zi
      IF(1.e0-check)70,70,20
20    IF(zr.LT.x1) GOTO 60
      eps1=.04e0/(zr-3.29e0)-.034e0
      IF(zr*zi.LT.eps1) GOTO 50
      IF(.not.((zr.GE.x2).and.(zr*zi.LT.eps))) GOTO 60
50    check=1.e0
      wi0=a1p*(dr1/de1+d1r/d1e)+a2p*(dr2/de2+d2r/d2e)+
     1   a3p*(dr3/de3+d3r/d3e)+a4p*(dr4/de4+d4r/d4e)+
     1   a5p*(dr5/de5+d5r/d5e)
      zi0=zi
      zi=0.e0
      GOTO 10
60    wr=(a1p*(1.e0/de1+1.e0/d1e)+a2p*(1.e0/de2+1.e0/d2e)+
     1   a3p*(1.e0/de3+1.e0/d3e)+a4p*(1.e0/de4+1.e0/d4e)+
     1   a5p*(1.e0/de5+1.e0/d5e))*zi
70    wi=a1p*(dr1/de1+d1r/d1e)+a2p*(dr2/de2+d2r/d2e)+
     1   a3p*(dr3/de3+d3r/d3e)+a4p*(dr4/de4+d4r/d4e)+
     1   a5p*(dr5/de5+d5r/d5e)
      IF(1.e0-check)80,80,90
80    wr=exp(-zr*zr)+2.e0*wi*zr*zi0-pi2*zi0
      wi=wi0
      zi=zi0
      check=0.e0
90    RETURN
      END
     
c                 wpade1.fortran
c
c       This program calculates a pade approximation of w(z)
c       around the origin.
c
      SUBROUTINE pade1(wr, wi, zr, zi)
      IMPLICIT NONE
      DOUBLE PRECISION wr, wi, zr, zi
      DOUBLE PRECISION c1, c2, c3, c4, c5, c6
      DOUBLE PRECISION d1, d2, d3, d4, d5, d6, d7
      DOUBLE PRECISION u2r, u2i, u3r, u3i, u4r, u4i
      DOUBLE PRECISION u5r, u5i, u6r, u6i, u7r, u7i
      DOUBLE PRECISION fr, fi, dr, di, de

      DATA c1/-1.25647718e0/,c2/8.25059158e-1/,
     1   c3/-3.19300157e-1/,c4/7.63191605e-2/,
     1   c5/-1.04697938e-2/,c6/6.44878652e-4/
      DATA d1/-2.38485635e0/,d2/2.51608137e0/,
     1   d3/-1.52579040e0/,d4/5.75922693e-1/,
     1   d5/-1.35740709e-1/,d6/1.85678083e-2/,
     1   d7/-1.14243694e-3/
      u2r=zi*zi-zr*zr
      u2i=-2.e0*zr*zi
      u3r=-u2r*zi-u2i*zr
      u3i=u2r*zr-u2i*zi
      u4r=-u3r*zi-u3i*zr
      u4i=u3r*zr-u3i*zi
      u5r=-u4r*zi-u4i*zr
      u5i=u4r*zr-u4i*zi
      u6r=-u5r*zi-u5i*zr
      u6i=u5r*zr-u5i*zi
      u7r=-u6r*zi-u6i*zr
      u7i=u6r*zr-u6i*zi
      fr=1.e0-c1*zi+c2*u2r+c3*u3r+c4*u4r+c5*u5r+c6*u6r
      fi=c1*zr+c2*u2i+c3*u3i+c4*u4i+c5*u5i+c6*u6i
      dr=1.e0-d1*zi+d2*u2r+d3*u3r+d4*u4r+d5*u5r+d6*u6r+d7*u7r
      di=d1*zr+d2*u2i+d3*u3i+d4*u4i+d5*u5i+d6*u6i+d7*u7i
      de=dr*dr+di*di
      wr=(fr*dr+fi*di)/de
      wi=(fi*dr-fr*di)/de
      RETURN
      END
     
c                 wpade2.fortran
c
c          this program calculates a pade approximation of w(z)
c       around the point z = 3.
c
      SUBROUTINE pade2(wr, wi, zr, zi)
      IMPLICIT NONE
      DOUBLE PRECISION wr, wi, zr, zi
      DOUBLE PRECISION c0r, c0i, c1r, c1i, c2r, c2i, c3r, c3i
      DOUBLE PRECISION d1r, d1i, d2r, d2i, d3r, d3i, d4r, d4i
      DOUBLE PRECISION z2r, z2i, z3r, z3i, z4r, z4i
      DOUBLE PRECISION fr, fi, dr, di, de

      DATA c0r/1.23409804e-4/,c0i/2.01157318e-1/,
     1   c1r/2.33746715e-1/,c1i/1.61133338e-1/,
     1   c2r/1.25689814e-1/,c2i/-4.0422725e-2/,
     1   c3r/8.92089179e-3/,c3i/-1.81293213e-2/
      DATA d1r/1.19230984e0/,d1i/-1.16495901e0/,
     1   d2r/8.9401545e-2/,d2i/-1.07372867e0/,
     1   d3r/-1.68547429e-1/,d3i/-2.70096451e-1/,
     1   d4r/-3.20997564e-2/,d4i/-1.58578639e-2/
      zr=zr-3.e0
      z2r=zr*zr-zi*zi
      z2i=2.e0*zr*zi
      z3r=z2r*zr-z2i*zi
      z3i=z2r*zi+z2i*zr
      z4r=z3r*zr-z3i*zi
      z4i=z3r*zi+z3i*zr
      fr=c0r+c1r*zr-c1i*zi+c2r*z2r-c2i*z2i+c3r*z3r-c3i*z3i
      fi=c0i+c1r*zi+c1i*zr+c2r*z2i+c2i*z2r+c3r*z3i+c3i*z3r
      dr=1.e0+d1r*zr-d1i*zi+d2r*z2r-d2i*z2i+d3r*z3r-d3i*z3i+
     1   d4r*z4r-d4i*z4i
      di=d1r*zi+d1i*zr+d2r*z2i+d2i*z2r+d3r*z3i+d3i*z3r+d4r*z4i+
     1   d4i*z4r
      de=dr*dr+di*di
      wr=(fr*dr+fi*di)/de
      wi=(fi*dr-fr*di)/de
      zr=zr+3.e0
      RETURN
      END

c                  wexct.fortran
c     this program gives an approximate value for an 
c     infinite series expression of w(z)
c
      SUBROUTINE wexct(wer, wei, zr, zi)
      IMPLICIT NONE
      DOUBLE PRECISION wer, wei, zr, zi
      DOUBLE PRECISION p, a1, a2, a3, a4, a5
      DOUBLE PRECISION pi, ex, t, twxy, c2xy, s2xy
      DOUBLE PRECISION den, er, acr, aci, xn, n2, arg, arg1 
      DOUBLE PRECISION earg, expar, exmar, ch2, sh2 
      DOUBLE PRECISION termr, termi
      INTEGER n, nmax

      DATA p/3.275911e-1/,a1/2.54829592e-1/,a2/-2.84496736e-1/,
     & a3/1.421413741e0/,a4/-1.453152027e0/,a5/1.061405429e0/
      DATA nmax/35/
      DATA pi/3.14159265e0/
      zr=-zr
      ex=exp(-zr*zr)
      t=1.e0/(1.e0+p*zi)
      twxy=2.e0*zr*zi
      c2xy=cos(twxy)
      s2xy=sin(twxy)
      den=2.e0*pi*zi
      er=((((a5*t+a4)*t+a3)*t+a2)*t+a1)*t
      IF(zi)20,10,20
 10   acr=er
      aci=-zr/pi
      GOTO 30
 20   acr=er+(c2xy-1.e0)/den
      aci=-s2xy/den
 30   DO 100 n=1,nmax
         xn=n*zr
         n2=n*n
         arg=.25e0*n2
         arg1=2.e0/pi/(4.e0*zi*zi+n2)
         earg=0.e0
         expar=0.e0
         exmar=0.e0
         IF(arg-xn-80.e0)40,40,50
 40      earg=exp(-arg)
         expar=exp(xn-arg)
         exmar=exp(-xn-arg)
 50      ch2=expar+exmar
         sh2=expar-exmar
         termr=-arg1*(2.e0*zi*earg-zi*ch2*c2xy+n/2.e0*sh2*s2xy)
         termi=-arg1*(zi*ch2*s2xy+n/2.e0*sh2*c2xy)
         acr=acr+termr
         aci=aci+termi
 100  continue
      wer=ex*(c2xy*acr-s2xy*aci)
      wei=ex*(c2xy*aci+s2xy*acr)
      zr=-zr
      RETURN
      END

c
c *********************************************************************
c
c     Round beam case:
c     Distribute charge n*e uniformally over longitudinal distance l
c     Distribute it gaussian tranversely, sigx=sigy=s 
c     All particle's speeds = c
c  
c     lam_xy(x,y)dx*dy = line charge density in area dx*dy
c                 = n*e/l * 1/2/pi/s^2*exp(-(x^2+y^2)/2/s^2)
c
c     lam_r(r) = line charge density inside circle of radius r        |r
c              = n*e/l * 1/2/pi/s^2* int(2*pi*r'*dr'*exp(-r'^2/2/s^2))|0
c                                                          |r
c              = n*e/l *int(d(r'^2/2/s^2)*exp(-r'^2/2/s^2))|0
c                                                          |r
c              = n*e/l *int(d(r'^2/2/s^2)*exp(-r'^2/2/s^2))|0
c
c              = n*e/l *( 1 - exp(-r^2/2/s^2) )
c
c       i(r) = current inside r = n*e/l*( 1 - exp(-r^2/2/s^2) )*c
c
c       b(r) = mu_0/2/pi/r*n(r)*e*c/l  by ampere's law
c
c    del_p_r = transverse momentum change in length l at radius r 
c            = el + mag = 2 * magnetic; for N.R. case this factor 
c                                       should be 1+v^2/c^2
c            = -2*e*c*b(r)*l/c
c            = -e^2*mu_0*c^2/c/pi/r*n(r)
c            = -n*e^2/pi/eps0/c*( 1 - exp(-r^2/2/s^2) )/r

C!!!!! rtrt, 25nov95, The factor of 2 should not have been included above,
C    since the relative velocity is 2c. Hence the following
C    formula needs to be divided by 2

c        dx' = -n*e^2/pi/eps0/(pc)*( 1 - exp(-r^2/2/s^2) )(x/r^2)
c
c                            e^2         1     me                      x
c            = -n* 4 *----------------*-------*--*(1 - exp(-r^2/2/s^2))---
c                     4*pi*eps0*me*c^2 bet*gam  m                     r^2
c
c                               1      me                            x
c            = -n* 4 * r_e * ------- * -- * (1 - exp(-r^2/2/s^2)) * ---
c                            bet*gam    m                           r^2
c
c      r_e = classical electron radius = 2.817941 * 10^(-15) m
c
c      dx' = bbrndfac *  (1 - exp(-r^2/2/s^2)) * x/r^2
c      dy' = bbrndfac *  (1 - exp(-r^2/2/s^2)) * y/r^2
c
c      bbrndfac = -n*4*r_e/bet/gam*me/m
c          e.g. = -4*2.817941 * 10^(-15)/10^4 = 1.127*10^(-18)
c      (x,y)=(s,0): dx'=1.127*10^(-18)*(1-e^(-.5))=4.435*10^(-19)
c
c      Elliptical beam:
c
c      From Okamoto and Talman, CBN 80-13, Sept. 1980,
c      Rational approximation of the complex error function and the 
c      electric field of a two-dimensional gaussian charge distribution.
c
c      q = "constant with the dimension of electric charge"
c
c      r=sy/sx
c      di=(2(sx^2 - sy^2))^{-1/2}
c      u=x*di
c      v=y*di/r=y*di*sx/sy
c      (1 - r^2)(u^2 + v^2) = (1 - sy^2/sx^2)di^2(x^2 + y^2*sx^2/sy^2)
c                           = (1/2)(1/sx^2)(x^2 + y^2*sx^2/sy^2)
c                           = x^2/2/sx^2 + y^2/2/sy^2
c
c      ex = q/2/eps_0/rt(2*pi)/rt(sx^2-sy^2) * im( w( (x + i*y)*di ) + ... )
c         = q/2/eps_0/rt(2*pi)/rt(sx^2-sy^2) * im( w(u+i*r*v) + ... )
c      ey = q/2/eps_0/rt(2*pi)/rt(sx^2-sy^2) * re( w(u+i*r*v) + ... )
c
c      Find const. of prop. needed for angular deflection
c          
c      From R. Talman, 1987 Accelerator School, Multiparticle
c      Phenomena and Landau Damping, p. 827 
c
c      Deflection of positron after passing one electron of other beam
c      electron, charge -e, at origin transversely
c      positron, charge e, passes at (x,y), p.828
c      r  = rt(x^2+y^2)
c      x' = dx/ds
c      dx'= x'(out)-x'(in)
c         = k*x/r^2
c      dy'= k*y/r^2
c      k  = -e^2/2/pi/eps_0/(pc)
c
c      Deflection of positron after passing gaussian n-electron beam centered
c      at origin. (Assume sx>sy). p.830. Include *2 for elec + magn;
C      for N.R. case this factor should be 1+v^2/c^2.
c      dx' = n* k *rt(pi)/rt2/rt(sx^2-sy^2) * im( w(u+i*r*v) + ... )*2 
c       = -n* e^2/2/pi/eps_0/(pc) *rt(pi)/rt2/rt(sx^2-sy^2) 
c                   * im( w(u+i*r*v) + ... )*2 
c       = -n* e^2/rt(2*pi)/eps_0/(pc)/rt(sx^2-sy^2) * im( w(u+i*r*v) + ... ) 
c       = factor * im( w(u+i*r*v) + ... ) 

C!!!!! rtrt, 25nov95, The factor of 2 should not have been included,
C    or alternatively, since the relative velocity is 2c, dx'
C    needs to be divided by 2

c
c                   1        n*e^2 
c      bbfact = - ------ * ------- 
c                 rt(pi)   eps0*pc 
c
c                       4*pi           e^2            1
c             = - n * --------- * -------------- * ------- 
c                       rt(pi)    4*pi*eps0*m*c^2  bet*gam
c
c                                      me     1
c             = - n * 4*rt(pi) * r_e * -- * ------- 
c                                       m   bet*gam
c
c      r_e = classical electron radius = 2.817941 * 10^(-15) m
c
c
c      In teapot, n, sx, and sy for other beam are read in (offsets also)
c      beta*gamma is known for particle being tracked
c
c       vxibyc += bbfact * im( w(u+i*r*v) + ... )/(1.0 + ddelta)*di
c       vyibyc += bbfact * re( w(u+i*r*v) + ... )/(1.0 + ddelta)*di
c
c           
c                                 2   2  2
c                            -(1-r )(u +v )
c      f(u,v,r) = w(u+irv) - e              w(ru+iv)
c
c      is calculated by SUBROUTINE fbbclc(fr,fi,u,v,r) which returns
c      fr and fi. Hence
c
c       vxibyc += bbfact * sign(fi,x)/(1.0 + ddelta)*di
c       vyibyc += bbfact * sign(fr,y)/(1.0 + ddelta)*di
c
c *********************************************************************
c
c   SUBROUTINE trkbmbm uses the complex error function routines for
c   calculating beam-beam kicks, unless the strong beam is almost round---
c   parameter ``outofrnd'', close to 1, establishes the degree of
c   roundness required.
c
c   Program ~tpot/ftpot/erfcomplex/testbmbm can be used to exploit this
c   as a check on the calculations. It performs essentially the same
c   calculations just inside and just outside the outofrnd boundary.
c
c *********************************************************************
c
c                   SUBROUTINE trkbmbm
c
c   This routine applies the kick to a particle as it passes
c   through the opposing (strong) beam by updating vxibyc and vyibyc
c
c     nstr = number of particles in strong beam
c     sx = sigma_x for stong beam
c     sy = sigma_y for stong beam
c     dx = horz. displ. of strong beam
c     dy = vert. displ. of strong beam
c
c     For non-relativistic particles it would be necessary to multiply
c     both bbfact and bbrndfac by a factor (1+v^2/c^2)/2 . 
c
c *********************************************************************
c
      SUBROUTINE trkbmbm(x, y, vxibyc, vyibyc, del, 
     1                  dx, dy, sx, sy, nstr, amass, betgam)
      IMPLICIT NONE

      DOUBLE PRECISION bbfact, bbrndfac, tem
      DOUBLE PRECISION x, y, vxibyc, vyibyc, del, r2
      DOUBLE PRECISION xrel, yrel
      DOUBLE PRECISION dx, dy, sx, sy, nstr, amass, betgam
      DOUBLE PRECISION u, v, ratio, denomi, vmin
      DOUBLE PRECISION fr, fi
      DOUBLE PRECISION pi, rootpi, clelrad, outofrnd, ydenimn, elmass
      DOUBLE PRECISION thinmin
 
      DATA pi /3.14159265359/
      DATA rootpi /1.77245385091/
      DATA clelrad /2.817940917d-15/
      DATA outofrnd /1.0001/
      DATA ydenimn /0.002/
      DATA elmass /0.000511034d0/
      DATA thinmin /0.001/

c     Meaning of Fortran FUNCTION sign:
c     IF y .ne. 0 THEN sign(fr,y)=fr*y/|y|, sign(fr,0)=fr

c     Purpose of ydenimn:
c     Linear interpolate ey when y*denomi < ydenimn

      IF ( sx .LE. thinmin*sy ) sx = thinmin*sy
      IF ( sy .LE. thinmin*sx ) sy = thinmin*sx

C      bbfact = -nstr*4.0*rootpi*clelrad*elmass/amass/betgam
C      bbrndfac = -nstr*4.0*clelrad*elmass/amass/betgam

C!!!!! rtrt, 25nov95, The deflection has been too great by a factor of
C     2 because the effect the relative velocity being 2c instead of c
C     has not been properly accounted for.

      bbfact = -nstr*2.0*rootpi*clelrad*elmass/amass/betgam
      bbrndfac = -nstr*2.0*clelrad*elmass/amass/betgam

         xrel = x - dx
         yrel = y - dy

      IF ( sx .GT. sy*outofrnd ) THEN

         denomi=1.0/sqrt(2.0*(sx**2-sy**2))
         ratio = sy/sx
         u=abs(xrel)*denomi
         v=abs(yrel)*denomi/ratio
         vmin = ydenimn/ratio
         IF ( v .GT. vmin ) THEN
            CALL fbbclc(fr,fi,u,v,ratio)
            vxibyc = vxibyc + bbfact*sign(fi,xrel)/(1.0 + del)*denomi
            vyibyc = vyibyc + bbfact*sign(fr,yrel)/(1.0 + del)*denomi
         ELSE
            CALL fbbclc(fr,fi,u,v,ratio)
            vxibyc = vxibyc + bbfact*sign(fi,xrel)/(1.0 + del)*denomi
            CALL fbbclc(fr,fi,u,vmin,ratio)
            vyibyc = 
     1       vyibyc + bbfact*sign(fr,yrel)/(1.0 + del)*v/vmin*denomi
         ENDIF

      ELSE IF ( sy .GT. sx*outofrnd ) THEN

         denomi=1.0/sqrt(2.0*(sy**2-sx**2))
         ratio=sx/sy
         u=abs(yrel)*denomi
         v=abs(xrel)*denomi/ratio
         vmin = ydenimn/ratio

         IF ( v .GT. vmin ) THEN
            CALL fbbclc(fr,fi,u,v,ratio)
            vxibyc = vxibyc + bbfact*sign(fr,xrel)/(1.0 + del)*denomi
            vyibyc = vyibyc + bbfact*sign(fi,yrel)/(1.0 + del)*denomi
         ELSE
            CALL fbbclc(fr,fi,u,vmin,ratio)
            vxibyc = 
     1       vxibyc + bbfact*sign(fr,xrel)/(1.0 + del)*v/vmin*denomi
            CALL fbbclc(fr,fi,u,v,ratio)
            vyibyc = vyibyc + bbfact*sign(fi,yrel)/(1.0 + del)*denomi
         ENDIF

      ELSE
     
         r2 = xrel**2+yrel**2
         IF ( r2 .NE. 0.0 ) THEN
            tem = 1.0 - exp( -r2/2/sx**2 )
            vxibyc = vxibyc + bbrndfac*tem/(1.0 + del)*xrel/r2
            vyibyc = vyibyc + bbrndfac*tem/(1.0 + del)*yrel/r2
         ENDIF
        
      ENDIF

      RETURN
      END
