# gnuplot > load "gnuplot/BM-III_Figure10.gnuplot"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

# set terminal x11
# set terminal postscript eps enhanced color 20
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
set title "dispersion: E-BM\_M1.0.RF.sxf"

set grid
set key left
set xlabel 'momentum offset {/Symbol d}-UAL'
set ylabel "{/Symbol d}x (m)"
set style line 2 lc rgb 'black' pt 6
set label 1 "xprime_0=-0.00055" at -1.5e-6,-0.0185

# set output "eps/BM-III_Figure10.eps"
set output "pdf/BM-III_Figure10.pdf"

p 'saveIG-BM_M1.0.RF' i 13 u 9:4 lw 1, 66.9*x w l lw 2, "<echo '-0.000001 -0.0000669'" with points ls 2
