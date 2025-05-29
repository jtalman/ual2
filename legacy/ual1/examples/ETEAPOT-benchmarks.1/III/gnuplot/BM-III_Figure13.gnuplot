# gnuplot > load "gnuplot/BM-III_Figure13.gnuplot"
set xlabel 'turn number'
set ylabel 'x (m)'
set grid
set style data linespoints

set xrange [9500:10000]
set format x ""
set bmargin 0

set terminal postscript eps enhanced color 20
set output "eps/BM-III_Figure13.eps"

set multiplot layout 4,1
set ytics -0.002, 0.001, 0.002
p '10000-IG-BM_M1.0.RF' i 15 u 2:4 w l
set ytics -0.004, 0.002, 0.004
p '10000-IG-BM_M1.0.RF' i 16 u 2:4 w l
set ytics -0.008, 0.004, 0.008
p '10000-IG-BM_M1.0.RF' i 17 u 2:4 w l
set bmargin
set format x "%g"
set ytics -0.01, 0.01, 0.01
p '10000-IG-BM_M1.0.RF' i 18 u 2:4 w l
unset multiplot

set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"


