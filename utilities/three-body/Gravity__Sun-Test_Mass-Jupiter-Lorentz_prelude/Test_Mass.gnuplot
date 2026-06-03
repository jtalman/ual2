set terminal x11 size 700, 700
set lmargin at screen 0.02
set rmargin at screen 0.98
set tmargin at screen 0.98
set bmargin at screen 0.02

 LIM = 2e11

 set size 1,1

set xrange [-LIM:+LIM]
set xlabel "x[m]"
set yrange [-LIM:+LIM]
set ylabel "y[m]"

p "Gravity__Sun-Test_Mass-Jupiter-Lorentz_prelude.out" u 8:9 w lines lc rgb "0xff0000"
