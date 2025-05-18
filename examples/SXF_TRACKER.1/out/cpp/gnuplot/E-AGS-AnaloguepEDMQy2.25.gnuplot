# gnuplot > load "gnuplot/twiss/E-AGS-AnaloguepEDMQy2.25.gnuplot"

 set xlabel "s (m)"
 set ylabel "beta_x (m), beta_y (m), eta_x(m)"
 # set xrange [0:14.2401]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 80,1.4 font "Helvetica"
 set title "E-AGS-AnaloguepEDMQy2.25 Lattice Functions"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,12"
# set term png enhanced notransparent 
#  set output '| display png:-'
set output "pdf/E-AGS-AnaloguepEDMQy2.25Twiss.pdf"

 plot "E-AGS-AnaloguepEDMQy2.25.twiss" using 3:4 title "E-AGS-AnaloguepEDMQy2.25-beta_x",\
      "E-AGS-AnaloguepEDMQy2.25.twiss" using 3:9 title "E-AGS-AnaloguepEDMQy2.25-beta_y",\
      "E-AGS-AnaloguepEDMQy2.25.twiss" using 3:7 title "E-AGS-AnaloguepEDMQy2.25-eta_x"

 set xrange [0:5]

set output "pdf/Start-E-AGS-AnaloguepEDMQy2.25Twiss.pdf"
plot "E-AGS-AnaloguepEDMQy2.25.twiss" using 3:4 title "Start-E-AGS-AnaloguepEDMQy2.25-beta_x",\
      "E-AGS-AnaloguepEDMQy2.25.twiss" using 3:9 title "Start-E-AGS-AnaloguepEDMQy2.25-beta_y",\
      "E-AGS-AnaloguepEDMQy2.25.twiss" using 3:7 title "Start-E-AGS-AnaloguepEDMQy2.25-eta_x"
