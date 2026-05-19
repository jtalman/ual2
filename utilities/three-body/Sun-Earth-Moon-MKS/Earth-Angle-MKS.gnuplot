#set terminal x11 size 1366, 768
 set terminal x11 size 700, 700
set lmargin at screen 0.1
set rmargin at screen 0.98
set tmargin at screen 0.98
set bmargin at screen 0.1

#LIM = 0.01
#LIM = 0.004
 LIM = 40

#set size ratio 1
#set size 2,1
 set size 1,1

set xrange [-5:+LIM]
set xlabel "Days"
set yrange [-5:+LIM]
set ylabel "Angle with X Axis"

p "Sun-Earth-Moon-MKS.out" u 8:9 w lines lc rgb "0xff0000"
