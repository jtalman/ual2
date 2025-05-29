# gnuplot > load "gnuplot/PTR-BM-I_Figure-p32.gnuplot"
set xlabel "split-bend index"
set ylabel "horizontal displacement, x(m)"
 # set xrange [0:14.2401]
 # set yrange [0:30]
set title "cosine-like standard particles, quads-off"

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"

# set terminal x11 enhanced font "Arial,18"
# set term postscript enhanced font "Arial,18" clip
  set term pdfcairo enhanced font "Arial,18"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set output "pdf/Fig-p32-t.pdf"
#plot 'PTR_m_nom=0.32349_sl4.sxf__plus_X' u 2 w l, 'PTR_m_nom=0.32349_sl4.sxf_minus_X' u 2 w l
 plot 'PTR_m_nomEQ0.32349_sl4.sxf__plus_X' u 2 w l, 'PTR_m_nomEQ0.32349_sl4.sxf_minus_X' u 2 w l
 
 set ylabel "vertical displacement, y(m)"
 set output "pdf/Fig-p32-b.pdf"
 plot 'PTR_m_nomEQ0.32349_sl4.sxf__plus_Y' u 2 w l, 'PTR_m_nomEQ0.32349_sl4.sxf_minus_Y' u 2 w l
