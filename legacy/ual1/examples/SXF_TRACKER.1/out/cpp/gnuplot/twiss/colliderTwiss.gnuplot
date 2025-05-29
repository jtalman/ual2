# gnuplot > load "gnuplot/twiss/colliderTwiss.gnuplot"

 set xlabel "s (m)"
 set ylabel "beta_x (m), beta_y (m), eta_x(m)"
# set xrange [0:50]
# set yrange [0:200]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 80,1.4 font "Helvetica"
 set title "Collider Lattice Functions"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
 set term pdfcairo enhanced font "Arial,12"
# set term png enhanced notransparent 
# set output '| display png:-'
 set output "pdf/colliderTwiss.pdf

 plot "collider.twiss" using 3:4 w lp title "collider-beta_x",\
      "collider.twiss" using 3:8 w lp title "collider-beta_y",\
      "collider.twiss" using 3:7 w lp title "collider-eta_x"