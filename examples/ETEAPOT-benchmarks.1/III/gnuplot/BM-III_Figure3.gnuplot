# gnuplot > load "gnuplot/BM-III_Figure3.gnuplot"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

# set terminal postscript eps enhanced color 20

set xlabel "ct (m)"
set ylabel "momentum offset {/Symbol d}-{UAL}"
# set xrange [0:16.5]
# set yrange [0:30]

set grid
set style data linespoints
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
set title "longitudinal phase space: E-BM-M1.0.RF.sxf "

# set output "eps/BM-III_Figure3.eps"
set output "pdf/BM-III_Figure3.pdf"

set grid
set key right
set style line 2 lc rgb 'black' pt 6
p 'saveIG-BM_M1.0.RF' i 10 u 8:9 w l, "<echo '0 -0.000001'" with points ls 2, \
  'saveIG-BM_M1.0.RF' i 11 u 8:9 w l, "<echo '0 -0.000002'" with points ls 2, \
  'saveIG-BM_M1.0.RF' i 12 u 8:9 w l, "<echo '0 -0.000003'" with points ls 2, \
  'saveIG-BM_M1.0.RF' i 14 u 8:9 w l, "<echo '-0.000669 -0.00001'" with points ls 2
