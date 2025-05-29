# gnuplot > load "gnuplot/E-PTR2-0p5-COSY-BM-VI_Figure9_11.gnuplot"
set xlabel "longitudinal position (m)"
set ylabel "horizontal beta function, {/Symbol b}_x(m)"
 set xrange [0:90.83]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "E-PTR2-0p5, m=0.002"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,20"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set output "pdf/E-PTR2-0p5-COSY-Fig_VI-9.pdf"
 plot 'betaFunctions' u 2:3 w l

 set ylabel "vertical beta function, {/Symbol b}_y(m)"
 set output "pdf/E-PTR2-0p5-COSY-Fig_VI-11.pdf"
 plot 'betaFunctions' u 2:4 w l
