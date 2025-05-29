# gnuplot > load "gnuplot/twiss/COSY-Vera-with-disp-bumpTwiss.gnuplot"

 set xlabel "s (m)"
 set ylabel "beta_x (m), beta_y (m), eta_x(m)"
 # set xrange [0:14.2401]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 0,1.4 font "Helvetica"
 set title "COSY-Vera-with-disp-bump Lattice Functions"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,12"
# set term png enhanced notransparent 
#  set output '| display png:-'
set output "pdf/COSY-Vera-with-disp-bumpTwiss.pdf"
 plot "ring.twiss" using 3:4 title "COSY-Vera-with-disp-bump-beta_x",\
      "ring.twiss" using 3:8 title "COSY-Vera-with-disp-bump-beta_y",\
      "ring.twiss" using 3:7 title "COSY-Vera-with-disp-bump-eta_x"

 set title "COSY-Vera-with-disp-bump Dispersion Bumps"
set output "pdf/COSY-Vera-with-disp-bumpDispBump.pdf"
plot  "ring.twiss" using 3:7 title "COSY-Vera-with-disp-bump-eta_x"

set title "COSY-Vera-with-disp-bump Dispersion Bumps-local 1"
set xrange [0:50]
set arrow 1 from 22.0, 0.0 to 22.0, 0.5
set label "mx02" at 22.0, -0.1 center 
set arrow 2 from 27.5, 0.0 to 27.5, 0.5
set label "mx03" at 27.5, -0.1 center 
set arrow 3 from 37.4, 0.0 to 37.4, 0.5
set label "mx04" at 37.4, -0.1 center 

set arrow 4 from 4.08, 0.0 to  4.08, 0.5
set label "msh43, 0.05 rad" at 4.08, -0.1 center 
set arrow 5 from 45.7, 0.0 to  45.7, -0.5
set label "msh09, -0.05 rad" at 45.7,  0.1 center 

set output "pdf/COSY-Vera-with-disp-bumpDispBump-loca1.pdf"
plot  "ring.twiss" using 3:7 title "COSY-Vera-with-disp-bump-eta_x"

set title "COSY-Vera-with-disp-bump Dispersion Bumps-local 2"
set xrange [90:140]
set arrow 1 from 111.8, 0.0 to 111.8, 0.5
set label "mx11" at 111.8, 0.6 center 
set arrow 2 from 121.2, 0.0 to 121.2, 0.5
set label "mx12" at 121.2, 0.6 center 
set arrow 3 from 131.1, 0.0 to 131.1, 0.5
set label "mx13" at 131.1, 0.6 center 

set arrow 4 from 91.7, 0.0 to  91.7, -0.5
set label "msh19, -0.05 rad" at 91.7,  0.1 center 
set arrow 5 from 130.2, 0.0 to  130.2, 0.5
set label "mshblwd3,  0.05 rad" at 130.2, -0.1 center 

set output "pdf/COSY-Vera-with-disp-bumpDispBump-loca2.pdf"
plot  "ring.twiss" using 3:7 title "COSY-Vera-with-disp-bump-eta_x"
