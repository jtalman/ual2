set terminal x11 size 1366, 768
set lmargin at screen 0.10  # Left margin
set rmargin at screen 0.98
set tmargin at screen 0.98
set bmargin at screen 0.02

set size 1,1
show size

LIMX=7e10
LIMY=5e11
LIM=1e12

#LIM_Xl = +5.93
#LIM_Xl = +0.6
#LIM_Xr = +1.1
 LIM_Xl = +7e10
 LIM_Xr = +1.6e11

#LIM_Yb = -0.1
#LIM_Yt = +1.
 LIM_Yb = -1e10
 LIM_Yt = +1.3e11

set xrange [-1e11:+5e10]
set xlabel "x[m]"

set yrange [-1.41e11:-1.1e11]
set ylabel "y[m]"

p "Sun-Earth-Moon-MKS.out" u 1:2 w lines lc rgb "0xff0000", "Sun-Earth-Moon-MKS.out" u 3:4 w lines lc rgb "0x00ff00"
