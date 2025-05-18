# gnuplot > load "gnuplot/BM-III_Figure9.gnuplot"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

# set terminal x11
# set terminal postscript eps enhanced color 20
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
set title "dispersion: E-BM-M1.0.RF.sxf "

set grid
set key left
set xlabel 'momentum offset {/Symbol d}-UAL'
set ylabel "{/Symbol d}x (m)"
set style line 2 lc rgb 'black' pt 6

# set output "eps/BM-III_Figure9.eps"
set output "pdf/BM-III_Figure9.pdf"

p 'saveIG-BM_M1.0.RF' i 10 u 9:4, 66.9*x w l, "<echo '-0.000001 0.0'" with points ls 2
