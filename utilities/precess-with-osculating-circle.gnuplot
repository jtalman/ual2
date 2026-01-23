set parametric
set trange [0:2*pi]
set size square

set size ratio 1
set xrange [-2.5e13:+2.5e13]
set xlabel "x[m]"
set yrange [-2.5e13:+2.5e13]
set ylabel "y[m]"

set object 1 circle at 0,0 size 1.78e+13 fillcolor rgb "red" lw 1

#r=+1.5e13
#p r*cos(t), r*sin(t) notitle

p "precess" u 1:2 w l
