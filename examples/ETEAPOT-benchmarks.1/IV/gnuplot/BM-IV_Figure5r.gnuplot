# gnuplot > load "gnuplot/BM-IV_Figure5r.gnuplot"
set xlabel "turn number"
set ylabel "horizontal displacement, x(m)"
 # set xrange [0:14.2401]
 # set yrange [-0.02:0.04]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "YKS Runge-Kutta comparison, {/Symbol D}p/p=2e-4"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set output "pdf/BM-IV_Figure5r.pdf"
 plot 'IG' i 7  u 2:4 w l, 'IG' i 9 u 2:4 w l, 'IG' i 11  u 2:4 w l
 
 set xrange [0:50]
 set output "pdf/BM-IV_Figure5r-early.pdf"
 plot 'IG' i 7  u 2:4 w l, 'IG' i 9 u 2:4 w l, 'IG' i 11  u 2:4 w l
