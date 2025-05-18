# gnuplot > load "gnuplot/BM-I_Figure3-Z-sl.gnuplot"
set xlabel "split-bend index"
set ylabel "horizontal displacement, x(m)"
 # set xrange [0:14.2401]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "10 slices/bend, cosine-like standard particles, q=-0.0000596"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set output "pdf/Fig_I-3-sl-t.pdf"
 plot 'E-BM-Z-sl.sxf__plus_X' u 2 w l, 'E-BM-Z-sl.sxf_minus_X' u 2 w l
 
 set ylabel "vertical displacement, x(m)"
 set output "pdf/Fig_I-3-sl-b.pdf"
 plot 'E-BM-Z-sl.sxf__plus_Y' u 2 w l, 'E-BM-Z-sl.sxf_minus_Y' u 2 w l