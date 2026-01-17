AxisMax = 1.0e14

set size ratio 1
set xrange [-AxisMax:+AxisMax]
set xlabel "x[m]"
set yrange [-AxisMax:+AxisMax]
set ylabel "y[m]"
p "precess" u 1:2 w l
