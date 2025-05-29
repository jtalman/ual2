# gnuplot > load "gnuplot/twiss/PTR_m_nom=0.32349_vary-ORIG.gnuplot"

 set xlabel "s [m]"
 set ylabel "betas [m]"
# set xrange [0:50]
# set yrange [0:200]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 0,1.4 font "Helvetica,18"
 set title "PTR_m_nom=0.32349_sl4-vary-ORIG.gnuplot"
# set terminal x11 enhanced font "Arial,18"
# set term postscript enhanced font "Arial,18" clip
 set term pdfcairo enhanced font "Arial,18"
# set term png enhanced notransparent 
# set output '| display png:-'
 set output "pdf/PTR_m_nom=0.32349_sl4-vary-ORIG-betas.pdf
 plot "ring.twiss" using 3:4 w lp title "PTR_m_nom=0.32349_sl4-vary-ORIG-beta-horz",\
        "ring.twiss" using 3:8 w lp title "PTR_m_nom=0.32349_sl4-vary-ORIG-beta-vert"
set output "pdf/PTR_m_nom=0.32349_sl4-vary-ORIG-phase-advances.pdf
set ylabel "horizontal phase advance"
plot  "ring.twiss" using 3:6 w lp title "mnom=0.32349-vary-ORIG-phase-adv-horz",\
        "ring.twiss" using 3:10 w lp title "mnom=0.32349-vary-ORIG-phase-adv-vert"
