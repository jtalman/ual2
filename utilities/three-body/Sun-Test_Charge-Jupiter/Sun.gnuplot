set terminal x11 size 700, 700
set lmargin at screen 0.1
set rmargin at screen 0.98
set tmargin at screen 0.98
set bmargin at screen 0.1

 LIM = 1e9

 set size ratio -1

set xrange [-0e9:+2e9]
set xlabel "x[m]"
set yrange [-LIM:+LIM]
set ylabel "y[m]"

 p "Gravity__Sun-Test_Mass-Jupiter-Lorentz_prelude.out" u 2:3 w lines lc rgb "0xff0000"
