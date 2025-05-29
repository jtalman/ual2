# gnuplot > load "gnuplot/BM-III_Figure12.gnuplot"
set grid
set key left
set xlabel 'turn index'
set ylabel "ct (m)"

set terminal postscript eps enhanced color 20
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
set title "small and large ampl. synch. oscillations"
set output "eps/BM-III_Figure12.eps"

set multiplot layout 2,1
   # set ytics -0.06, 0.02, 0.06
   p 'saveIG-BM_M1.0.RF' i 13 u 2:8 w l
   # set ytics -0.6, 0.2, 0.6
   p 'saveIG-BM_M1.0.RF' i 14 u 2:8 w l
   # set xrange [0:200]
   # set ytics -1.5, 0.5, 1.5
   # p 'saveIG-BM_M1.0.RF' i 14 u 2:8 w l
unset multiplot


