# gnuplot > load "gnuplot/Fig_I-3.gnuplot"
set xlabel "split-bend index"
set ylabel "horizontal displacement, x(m)"
 # set xrange [0:14.2401]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "cosine-like standard particles, q=-0.0000596"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set output "pdf/Fig_I-3t.pdf"
 plot 'E_BM_Z.sxf__plus_X' u 2 w l, 'E_BM_Z.sxf_minus_X' u 2 w l
 
 set ylabel "vertical displacement, x(m)"
 set output "pdf/Fig_I-3b.pdf"
 plot 'E_BM_Z.sxf__plus_Y' u 2 w l, 'E_BM_Z.sxf_minus_Y' u 2 w l