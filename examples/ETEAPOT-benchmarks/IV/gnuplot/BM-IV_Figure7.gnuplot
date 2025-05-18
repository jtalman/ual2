# gnuplot > load "gnuplot/BM-IV_Figure7.gnuplot"
 # set xrange [0:14.2401]
 # set yrange [-0.02:0.04]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 1,1 font "Helvetica"
 set title "userBunchUniform-x, synch. phase space: E-BM-Z.RF.sxf"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

 set xlabel "ct(m)"
 set ylabel "energy offset {/Symbol D}E/p_0c"

 set output "pdf/BM-IV_Figure7.pdf"
 plot 'IG' i  5 u 8:9 w l,\
      'IG' i  6 u 8:9 w l,\
      'IG' i  7 u 8:9 w l,\
      'IG' i  8 u 8:9 w l,\
      'IG' i  9 u 8:9 w l,\
      'IG' i 10 u 8:9 w l,\
      'IG' i 11 u 8:9 w l,\
      'IG' i 12 u 8:9 w l,\
      'IG' i 13 u 8:9 w l

