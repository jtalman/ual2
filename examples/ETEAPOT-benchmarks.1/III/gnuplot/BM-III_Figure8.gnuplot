# gnuplot > load "gnuplot/BM-III_Figure8.gnuplot"
# "BunchlengthVSoneBySqrtVRF.data" has to be up-to-date

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

# set terminal x11
# set terminal postscript eps enhanced color 20
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
set title "bunch-length versus 1/sqrt(V\_R_F)"

set grid
set key left
set xlabel '1/sqrt(V_R\_F(kV))'
set ylabel "bunch-length [m]"
set style line 2 lc rgb 'black' pt 6

set output "pdf/BM-III_Figure8.pdf"
p "BunchlengthVSoneBySqrtVRF.data" u 3:5 w p, 0.112*x
