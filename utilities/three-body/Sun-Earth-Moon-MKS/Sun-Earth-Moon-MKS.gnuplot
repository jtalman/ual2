#set terminal x11 size 1366, 768
 set terminal x11 size 700, 700
set lmargin at screen 0.02
set rmargin at screen 0.98
set tmargin at screen 0.98
set bmargin at screen 0.02

#LIM = 0.01
#LIM = 0.004
#LIM = 1e9
 LIM = 5e8

#set size ratio 1
#set size 2,1
 set size 1,1

set xrange [-LIM:+LIM]
set xlabel "x[m]"
set yrange [-LIM:+LIM]
set ylabel "y[m]"

p "Sun-Earth-Moon-MKS.out" u 5:6 w lines lc rgb "0xff0000"
