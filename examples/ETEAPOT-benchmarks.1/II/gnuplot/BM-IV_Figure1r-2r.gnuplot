# gnuplot > load "gnuplot/BM-IV_E-ValLeb-Figure1r-2r.gnuplot"
set xlabel "longitudinal position (m)"
set xrange [0:21.5]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "E-ValLeb2-sl4.sxf, m=0.2"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,20"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set ylabel "horizontal beta function, {/Symbol b}_x(m)"
 set output "pdf/BM-IV_Figure1r.pdf"
 plot 'betaFunctions' u 2:3 w l

 set ylabel "vertical beta function, {/Symbol b}_y(m)"
 set output "pdf/BM-IV_Figure2r.pdf"
 plot 'betaFunctions' u 2:4 w l
