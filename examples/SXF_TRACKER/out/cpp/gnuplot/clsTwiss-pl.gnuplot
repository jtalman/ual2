# gnuplot > load "gnuplot/twiss/clsTwiss.gnuplot"

 set xlabel "s (m)"
 set ylabel "betax (m), betay (m), eta_x(m)"
 # set xrange [0:14.2401]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 80,1.4 font "Helvetica"
 set title "CLS Lattice Functions"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,12"
# set term png enhanced notransparent 
#  set output '| display png:-'
set output "pdf/clsTwiss-pl.pdf"

 plot "lattice.twiss" using 3:4 title "cls-beta_x",\
      "lattice.twiss" using 3:9 title "cls-beta_y",\
      "lattice.twiss" using 3:7 title "cls-eta_x"