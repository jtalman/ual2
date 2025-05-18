# gnuplot > load "gnuplot/E-PTR2-1p0-COSY-sl4-BM-VI_Figure9_11.gnuplot"
set xlabel "longitudinal position (m)"
 set xrange [0:182]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "E-PTR2-1p0-sl4, m=0.002"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,20"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set ylabel "horizontal beta function, {/Symbol b}_x(m)"
 set output "pdf/E-PTR2-1p0-COSY-sl4-Fig_VI-9-ETEAPOT.pdf"
 plot 'betaFunctions' u 2:3 w l

 set ylabel "vertical beta function, {/Symbol b}_y(m)"
 set output "pdf/E-PTR2-1p0-COSY-sl4-Fig_VI-11-ETEAPOT.pdf"
 plot 'betaFunctions' u 2:4 w l
