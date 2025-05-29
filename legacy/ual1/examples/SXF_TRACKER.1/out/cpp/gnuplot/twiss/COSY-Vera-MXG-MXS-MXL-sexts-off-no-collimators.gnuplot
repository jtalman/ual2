# gnuplot > load "gnuplot/twiss/COSY-Vera-MXG-MXS-MXL-sexts-on-no-collimators.gnuplot"

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

set label "deuteron,  E= 2.11 GeV,  {/Symbol b} =0.497" at 100, 28
set title "COSY-Vera-MXG-MXS-MXL-sexts-on-no-collimators Beta Functions"

set arrow 1 from 63.22, 0.0 to 63.22, 3.857       # MXL 
set label "mx06=sf" at 63.22, 3 right
set arrow 2 from 78.06, 0.0 to 78.06, 3.857       # MXL 
set label "mx08=sf" at 78.30, 3 left 
set arrow 3 from 154.94, 0.0 to 154.94, 3.857       # MXL 
set label "mx15=sf" at 154.94, 3.857 right
set arrow 4 from 169.79, 0.0 to 169.79, 3.857     # MXL 
set label "mx17=sf" at 169.79, 3.857 left

set arrow 5 from 69.54, 0.0 to 69.54, 8.906       # MXG     
set label "mx07=sd" at 69.54, 11 center
set arrow 6 from 161.25, 0.0 to 161.25, 8.906        # MXG 
set label "mx16=sd" at 161.25, 11 center

set arrow 7 from 57.63, 0.0 to 57.63, 8.906        # MXS
set label "mx05=sd" at 57.63, 9 right
set arrow 8 from 83.74, 0.0 to 83.74, 8.906        # MXS
set label "mx09=sd" at 83.74, 8.906 left
set arrow 9 from 149.37, 0.0 to 149.37, 8.906        # MXS
set label "mx14=sd" at 149.37, 8.906 right
set arrow 10 from 175.49, 0.0 to 175.49, 8.906       # MXS
set label "mx18=sd" at 175.49, 8.906 left

set output "pdf/COSY-Vera-MXG-MXS-MXL-sexts-on-no-collimators-Twiss.pdf"
plot "ring.twiss" using 3:4 title "COSY-Vera-MXG-MXS-MXL-sexts-on-no-collimators-beta_x",\
      "ring.twiss" using 3:8 title "COSY-Vera-MXG-MXS-MXL-sexts-on-no-collimators-beta_y"

unset label
set label "deuteron,  E= 2.11 GeV,  {/Symbol b} =0.497" at 100, 25
set ylabel "D [m]"
set title "COSY-Vera-MXG-MXS-MXL-sexts-on-no-collimators Dispersion Function"
set arrow 1 from 63.22, 0.0 to 63.22, 17.61       # MXL 
set label "mx06=sf" at 63.22,  17.61 right
set arrow 2 from 78.06, 0.0 to 78.06, 18.17       # MXL 
set label "mx08=sf" at 78.30, 18.17 left 
set arrow 3 from 154.94, 0.0 to 154.94, 17.61       # MXL 
set label "mx15=sf" at 154.94, 17.61 right
set arrow 4 from 169.79, 0.0 to 169.79, 18.15   # MXL 
set label "mx17=sf" at 169.79, 18.15 left

set arrow 5 from 69.54, 0.0 to 69.54, 31.51       # MXG     
set label "mx07=sd" at 69.54, 30.0  right
set arrow 6 from 161.25, 0.0 to 161.25, 31.51        # MXG 
set label "mx16=sd" at 161.25, 30 right

set arrow 7 from 57.63, 0.0 to 57.63, 8.99      # MXS
set label "mx05=sd" at 57.63, 8.99 right
set arrow 8 from 83.74, 0.0 to 83.74, 9.11        # MXS
set label "mx09=sd" at 83.74, 9.11 left
set arrow 9 from 149.37, 0.0 to 149.37, 9.04      # MXS
set label "mx14=sd" at 149.37, 9.04 right
set arrow 10 from 175.49, 0.0 to 175.49, 9.06      # MXS
set label "mx18=sd" at 175.49, 9.06 left

set output "pdf/COSY-Vera-MXG-MXS-MXL-sexts-on-no-collimators-DispersionFunction.pdf"
plot  "ring.twiss" using 3:7 title "COSY-Vera-MXG-MXS-MXL-sexts-on-no-collimators.-eta_x"
        




