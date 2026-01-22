unset border
set zeroaxis lt -1

set xtics axis
set ytics axis

AxisMax = 1.0e14

set size ratio 1
set xrange [-AxisMax:+AxisMax]
set xlabel "x[m]"
set yrange [-AxisMax:+AxisMax]
set ylabel "y[m]"

#set style line 1 linewidth 0.25
 set style line 1 linecolor rgb "black" linewidth 1

plot [-1e14:+1e14] "special-relativistic-Kepler.out" ls 1 w l
