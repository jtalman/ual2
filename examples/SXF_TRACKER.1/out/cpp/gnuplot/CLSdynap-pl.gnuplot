# gnuplot > load "CLSdynap.gnuplot"

 set xlabel "sqrt(epsilon_x) (m^0.5)"
 set ylabel "sqrt(epsilon_y) (m^0.5)"
 set xrange [0:0.015]
 set yrange [0:0.015]

 set data style points

 set terminal postscript 20
 set output "CLSdynap.ps"
 set title "100 Turn Dynamic Aperture "
 set label "" at -0.005, 0.0075 
 plot "../dynap" using 5:8 title " 4"
