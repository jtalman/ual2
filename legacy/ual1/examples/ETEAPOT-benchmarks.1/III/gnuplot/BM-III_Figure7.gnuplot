# gnuplot > load "gnuplot/BM-III_Figure7.gnuplot"
# "Q_sVSsqrtVRF.data" has to be up-to-date

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

# set terminal x11
# set terminal postscript eps enhanced color 20
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
set title "Q_s versus sqrt(V\_R\_F)"

set grid
set key left
set xlabel "sqrt(V_RF(kV))"
set ylabel "synchrotron tune Q\_s"
set style line 2 lc rgb 'black' pt 6

# set output "eps/BM-III_Figure7.eps"
set output "pdf/BM-III_Figure7.pdf"

p "Q_sVSsqrtVRF.data" u 2:4 w p, 0.0068*x
