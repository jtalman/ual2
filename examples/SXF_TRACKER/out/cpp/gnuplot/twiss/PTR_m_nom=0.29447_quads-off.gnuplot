# gnuplot > load "gnuplot/twiss/PTR_m_nom=0.29447_quads-off.gnuplot"

 set xlabel "s [m]"
 set ylabel "betas [m]"
# set xrange [0:50]
# set yrange [0:200]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 0,1.4 font "Helvetica,18"
 set title "mnom=0.29447-quads-off"
# set terminal x11 enhanced font "Arial,18"
# set term postscript enhanced font "Arial,18" clip
 set term pdfcairo enhanced font "Arial,18"
# set term png enhanced notransparent 
# set output '| display png:-'
 set output "pdf/PTR-mnom=0.29447-quads-off-beta-horz.pdf
 plot "ring.twiss" using 3:4 w lp title "PTR-mnom=0.29447-quads-off-beta-horz"
#        "ring.twiss" using 3:7 w lp title "PTR-mnom=0.29447-quads-off-beta-vert"
set output "pdf/PTR-mnom=0.29447-quads-off-phase-advance.pdf
set ylabel "horizontal phase advance"
plot  "ring.twiss" using 3:6 w lp title "mnom=0.29447-quads-off-phase-advance"
