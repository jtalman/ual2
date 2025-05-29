# gnuplot > load "gnuplot/twiss/041209_COSY-qu1f-mod-mod.gnuplot"

 set xlabel "s (m)"
 set ylabel "beta_x [m], beta_y [m]"
 # set xrange [0:14.2401]
 # set yrange [0:30]

 set grid
 set style data linespoints
 set timestamp "%d/%m/%y %H:%M" offset 0,1.4 font "Helvetica"

# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,12"
# set term png enhanced notransparent 
#  set output '| display png:-'

set label "proton,  E= 2.5 GeV,  {/Symbol b} =0.93" at 100, 23
set title "041209-COSY-qu1f-mod-mod.gnuplot Beta Functions"
set arrow 1 from 63.22, 0.0 to 63.22, 3.857
set label "mx06=sf" at 60.22, 3.857 right
set arrow 2 from 69.54, 0.0 to 69.54, 8.906
set label "mx07=sd" at 72.54, 8.906 left
set output "pdf/041209-COSY-qu1f-mod-mod.gnuplot.pdf"
plot "ring.twiss" using 3:4 title "041209-COSY-qu1f-mod-mod.gnuplot-beta_x",\
      "ring.twiss" using 3:8 title "041209-COSY-qu1f-mod-mod.gnuplot-beta_y"

unset label
set label "proton,  E= 2.5 GeV,  {/Symbol b} =0.93" at 100, 25
set ylabel "D [m]"
set title "041209-COSY-qu1f-mod-mod.gnuplot Dispersion Function"
set arrow 1 from 63.22, 0.0 to 63.22, 17.61
set label "mx06=sf" at 60.22, 17.61 right
set arrow 2 from 69.54, 0.0 to 69.54, 31.51
set label "mx07=sd" at 72.54, 31.51 left
set output "pdf/041209-COSY-qu1f-mod-mod.gnuplotDispersionFunction.pdf"
plot  "ring.twiss" using 3:7 title "041209-COSY-qu1f-mod-mod.gnuplot-eta_x"
        




