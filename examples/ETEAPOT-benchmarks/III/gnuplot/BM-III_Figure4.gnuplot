# gnuplot > load "gnuplot/BM-III_Figure4.gnuplot"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

# set terminal postscript eps enhanced color 20

set grid
set key left
set ylabel '{/Symbol d}_U_A_L'
set xlabel "ct [m]"
set style line 2 lc rgb 'black' pt 6

set output "eps/BM-III_Figure4t.eps"
set output "pdf/BM-III_Figure4t.pdf"

set title "longitudinal phase space: E-BM-M1.0.RF.sxf
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
p 'saveIG-BM_M1.0.RF' i 10 u 8:9 w l lw 1, "<echo '0 -0.000001'" with points ls 2, 'saveIG-BM_M1.0.RF' i 11 u 8:9 w l  lw 1, "<echo '0 -0.000002'" with points ls 2, 'saveIG-BM_M1.0.RF' i 12 u 8:9 w l lw 1, "<echo '0 -0.000003'" with points ls 2

# set output "eps/BM-III_Figure4m.eps"
set output "pdf/BM-III_Figure4m.pdf"

set title "longitudinal phase space: E-BM-Z.RF.sxf
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
p 'saveIG-BM_Z.RF' i 10 u 8:9 w l lw 1, "<echo '0 -0.000001'" with points ls 2, 'saveIG-BM_Z.RF' i 11 u 8:9 w l  lw 1, "<echo '0 -0.000002'" with points ls 2, 'saveIG-BM_Z.RF' i 12 u 8:9 w l lw 1, "<echo '0 -0.000003'" with points ls 2

set output "pdf/BM-III_Figure4b.pdf"
# set output "eps/BM-III_Figure4b.eps"

set title "longitudinal phase space: E-BM-P1.0.RF.sxf
set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"
p 'saveIG-BM_P1.0.RF' i 10 u 8:9 w l lw 1, "<echo '0 -0.000001'" with points ls 2, 'saveIG-BM_P1.0.RF' i 11 u 8:9 w l  lw 1, "<echo '0 -0.000002'" with points ls 2, 'saveIG-BM_P1.0.RF' i 12 u 8:9 w l lw 1, "<echo '0 -0.000003'" with points ls 2
