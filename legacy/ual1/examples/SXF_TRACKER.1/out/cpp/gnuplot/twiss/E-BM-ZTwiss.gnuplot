# gnuplot > load "gnuplot/twiss/E-BM-ZTwiss.gnuplot"

 set xlabel "s (m)"
 set ylabel "beta_x (m), beta_y (m), eta_x(m)"
 # set xrange [0:14.2401]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 0,1.4 font "Helvetica"
 set title "E-BM-Z Lattice Functions"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,12"
# set term png enhanced notransparent 
#  set output '| display png:-'
set output "pdf/E-BM-ZTwiss.pdf"

 plot "ring.twiss" using 3:4 title "E-BM-Z-beta_x",\
       "ring.twiss" using 3:8 title "E_BM-Z-beta_y",\
       "ring.twiss" using 3:7 title "E_BM-Z-eta_x"