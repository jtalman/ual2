# gnuplot > load "gnuplot/BM-III_Figure13.gnuplot"
# set xlabel 'turn number'
set ylabel 'x (m)'
set grid
set style data linespoints

set xrange [9500:10000]
# set format x ""
set bmargin 0

set terminal postscript eps enhanced color 20
set output "eps/BM-III_Figure13.eps"

set size 1,1
set origin 0,0
set multiplot layout 4,1 columnsfirst scale 0.9,0.9 title "turn number"
# set key opaque
# set lmargin at screen 0.2
# set rmargin at screen 0.85
# set bmargin at screen 0.25
# set tmargin at screen 0.85

set ytics -0.002, 0.001, 0.002
p '10000-IG-BM_M1.0.RF' i 15 u 2:4 w l

set ytics -0.004, 0.002, 0.004
p '10000-IG-BM_M1.0.RF' i 16 u 2:4 w l

set ytics -0.008, 0.004, 0.008
p '10000-IG-BM_M1.0.RF' i 17 u 2:4 w l

# set bmargin
# set format x "%g"
set yrange [-0.015:+0.025]
set ytics -0.008, 0.008, 0.008
# set ytics -0.2, 0.01, 0.2
# set label 1 'd' at graph 0,0.3 font ',8'
# p 18 u 2:4 w l
p '10000-IG-BM_M1.0.RF' i 18 u 2:4 w l

unset multiplot

set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
