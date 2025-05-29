# gnuplot > load "gnuplot/BM-IV_Figure6.gnuplot"
 # set xrange [0:14.2401]
 # set yrange [-0.02:0.04]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "synch. phase space: E-BM-Z.RF.sxf"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
  set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set xlabel "ct(m)"
 set ylabel "energy offset {/Symbol D}E/p_0c"

 set output "pdf/BM-IV_Figure6l.pdf"
 plot 'IG' i  5 u 8:9 w l, "<echo ' 0.0003 0'" with points ls 2, \
      'IG' i  9 u 8:9 w l, "<echo ' 0.0000 0'" with points ls 2, \
      'IG' i 13 u 8:9 w l, "<echo '-0.0003 0'" with points ls 2

 set xlabel "turn number"
 set ylabel "horizontal displacement, x(m)"
 set title "{/Symbol d}=0.0002 off-momentum orbit: E-BM-Z-RF.sxf"
 
set output "pdf/BM-IV_Figure6r.pdf"
 plot 'IG' i 5  u 2:4 w l, \
      'IG' i 9  u 2:4 w l, \
      'IG' i 13 u 2:4 w l
 