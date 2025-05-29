# gnuplot > load "gnuplot/Longitudinal.gnuplot"

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
set title "longitudinal phase space: E-BM-Z-RF "

set output "pdf/BM-III_Figure3.pdf"

set grid
set key right
set style line 2 lc rgb 'black' pt 6
p 'IG' i 5 u 8:9 w l,\
  'IG' i 9 u 8:9 w l,\
  'IG' i 13 u 8:9 w l
