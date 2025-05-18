# gnuplot > load "gnuplot/BM-III_Figure11.gnuplot"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

# set terminal x11
# set terminal postscript eps enhanced color 20

set style data linespoints
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
set title "large amplitude, horizontal, E-BM-M1.0"
set grid
set key left
set xlabel '{/Symbol d}-UAL'
set ylabel "x (m)"
# set xtics -1.0e-5, 0.25e-5, 1.0e-5
set yrange [-0.02:0.02]
set label 1 "x'_0=-0.0004" at -1.5e-6,-0.017
set style line 2 lc rgb 'black' pt 6
set output "pdf/BM-III_Figure11.pdf"

p 'saveIG-BM_M1.0.RF' i 18 u 9:4 lw 1

