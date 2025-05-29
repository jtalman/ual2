# gnuplot > load "gnuplot/Fig_II-1_3.gnuplot"
set xlabel "longitudinal position (m)"
set ylabel "horizontal beta function, {/Symbol b}_x(m)"
set xrange [0:16.5]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "E-BM-P1.0-sl4, m=1"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,20"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set output "pdf/Fig_II-1.pdf"
 plot 'betaFunctions' u 2:3 w l

 set ylabel "vertical beta function, {/Symbol b}_y(m)"
 set output "pdf/Fig_II-3.pdf"
 plot 'betaFunctions' u 2:4 w l
