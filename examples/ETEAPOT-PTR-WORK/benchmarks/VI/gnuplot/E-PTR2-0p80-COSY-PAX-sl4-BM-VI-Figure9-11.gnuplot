# gnuplot > load "gnuplot/E-PTR2-0p80-COSY-PAX-sl4-BM-VI-Figure9-11.gnuplot"
set xlabel "longitudinal position (m)"
   set xrange [0:148.11]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "E-PTR2-0p80-COSY-PAX-sl4, m=+/-0.002"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,20"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set ylabel "horizontal beta function, {/Symbol b}_x(m)"
 set output "pdf/E-PTR2-0p80-COSY-PAX-sl4-Fig-VI-9-ETEAPOT.pdf"
 plot 'betaFunctions' u 2:3 w l

 set ylabel "vertical beta function, {/Symbol b}_y(m)"
 set output "pdf/E-PTR2-0p80-COSY-PAX-sl4-Fig-VI-11-ETEAPOT.pdf"
 plot 'betaFunctions' u 2:4 w l
