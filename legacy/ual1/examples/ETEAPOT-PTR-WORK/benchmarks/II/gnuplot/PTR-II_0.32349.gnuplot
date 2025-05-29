# gnuplot > load "gnuplot/PTR-II_0.32349.gnuplot"
set xlabel "longitudinal position (m)"
set ylabel "horizontal beta function, {/Symbol b}_x(m)"
set xrange [0:16.5]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "PTR-m-nom=0.32349, m=0.32349"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,20"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set output "pdf/Fig_0.32349-x.pdf"
 plot 'betaFunctions' u 2:3 w l

 set ylabel "vertical beta function, {/Symbol b}_y(m)"
 set output "pdf/Fig_0.32349-y.pdf"
 plot 'betaFunctions' u 2:4 w l
