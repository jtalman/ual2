# gnuplot > load "CLSDisp.gnuplot"

 set xlabel "s (m)"
 set ylabel "etax (m)"
 set xrange [0:14.2401]
 set yrange [0:0.6]

 set grid
 set data style linespoints

 set terminal postscript eps 20
 set output "CLSDisp.eps"
 set title "CLS Dispersion (Horizontal)"

 plot "../out/test/twiss" using 2:6 title "etax"
