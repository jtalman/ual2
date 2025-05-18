# gnuplot > load "gnuplot/twiss/2009pp-blueTwiss.gnuplot"

 set xlabel "s (m)"
 set ylabel "beta_x (m), beta_y (m), eta_x(m)"
# set xrange [0:50]
# set yrange [0:200]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 80,1.4 font "Helvetica"
 set title "2009pp-blue Lattice Functions"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
 set term pdfcairo enhanced font "Arial,12"
# set term png enhanced notransparent 
# set output '| display png:-'
 set output "pdf/2009pp-blueTwiss.pdf

 plot "2009pp-blue.twiss" using 3:4 w lp title "2009pp-blue-beta_x",\
      "2009pp-blue.twiss" using 3:9 w lp title "2009pp-blue-beta_y",\
      "2009pp-blue.twiss" using 3:7 w lp title "2009pp-blue-eta_x"