set terminal x11 size 1366, 768
set lmargin at screen 0.05  # Left margin
set rmargin at screen 0.98
set tmargin at screen 0.98
set bmargin at screen 0.02

set size 1,1
show size

#LIM_Xl = +5.93
#LIM_Xl = +0.6
#LIM_Xr = +1.1
 LIM_Xl = +1.2e11
 LIM_Xr = +1.55e11

#LIM_Yb = -0.1
#LIM_Yt = +1.
 LIM_Yb = -1e10
#LIM_Yt = +1.2e11
 LIM_Yt = +1.0e11

set xrange [+LIM_Xl:+LIM_Xr]
set xlabel "x[m]"

set yrange [+LIM_Yb:+LIM_Yt]
set ylabel "y[m]"

p "Sun-Earth-Moon-MKS.out" u 1:2 w lines lc rgb "0xff0000", "Sun-Earth-Moon-MKS.out" u 3:4 w lines lc rgb "0x00ff00"
