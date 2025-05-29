# gnuplot > load "gnuplot/twiss/blue-18-19Twiss.gnuplot"

 set xlabel "s (m)"
 set ylabel "beta_x (m), beta_y (m), eta_x(m)"
# set xrange [0:50]
# set yrange [0:200]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 80,1.4 font "Helvetica"
 set title "blue-18-19 Lattice Functions"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
 set term pdfcairo enhanced font "Arial,12"
# set term png enhanced notransparent 
# set output '| display png:-'
 set output "pdf/blue-18-19Twiss.pdf

 plot "blue-18-19.twiss" using 3:4 w lp title "blue-18-19-beta_x",\
      "blue-18-19.twiss" using 3:9 w lp title "blue-18-19-beta_y",\
      "blue-18-19.twiss" using 3:7 w lp title "blue-18-19-eta_x"