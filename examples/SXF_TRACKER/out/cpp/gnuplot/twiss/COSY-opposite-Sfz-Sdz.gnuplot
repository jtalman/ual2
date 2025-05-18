# gnuplot > load "gnuplot/twiss/COSY-opposite-Sfz-Sdz.gnuplot"

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

set label "deuteron,  E= 2.11 GeV,  {/Symbol b} =0.497" at 100, 40
set title "COSY-opposite-Sfz-Sdz Beta Functions"

set arrow 1 from 63.22, 0.0 to 63.22, 3.857       # MXL 
set label "mx06 " at 63.22, 3 right
set arrow 2 from 78.06, 0.0 to 78.06, 3.857       # MXL 
set label " mx08" at 78.30, 3 left 
set arrow 3 from 154.94, 0.0 to 154.94, 3.857       # MXL 
set label "mx15 " at 154.94, 3.857 right
set arrow 4 from 169.79, 0.0 to 169.79, 3.857     # MXL 
set label "mx17 " at 169.79, 3.857 right

set arrow 5 from 69.54, 0.0 to 69.54, 8.906       # MXG     
set label "mx07" at 69.54, 11 center
set arrow 6 from 161.25, 0.0 to 161.25, 8.906        # MXG 
set label "mx16" at 161.25, 11 center

set arrow 7 from 57.63, 0.0 to 57.63, 8.906        # MXS
set label "mx05 " at 57.63, 9 right
set arrow 8 from 83.74, 0.0 to 83.74, 8.906        # MXS
set label " mx09" at 83.74, 8.906 left
set arrow 9 from 149.37, 0.0 to 149.37, 8.906        # MXS
set label "mx14 " at 149.37, 8.906 right
set arrow 10 from 175.49, 0.0 to 175.49, 8.906       # MXS
set label " mx18" at 175.49, 8.906 left

set arrow 11 from 22.05, 0.0 to 22.05, 8.906       # MXS
set label "mx02 " at 22.05, 8.906 right
set arrow 12 from 27.5, 0.0 to 27.5, 7 
set label " mx03" at 27.5, 7 left
set arrow 13 from 37.4, 0.0 to 37.4, 5
set label " mx04" at  37.4, 5 left
set arrow 14 from 102, 0.0 to 102, 5
set label "mx10 " at 102, 5 right
set arrow 15 from 112, 0.0 to 112, 10
set label " mx11" at 102, 10 left
set arrow 16 from 121, 0.0 to 121, 13
set label " mx12" at 121, 13 left
set arrow 17 from 131, 0.0 to 131, 5
set label " mx13" at 131, 5 left

set output "pdf/COSY-opposite-Sfz-Sdz-Twiss.pdf"
plot "ring.twiss" using 3:4 title "COSY-opposite-Sfz-Sdz-beta_x",\
      "ring.twiss" using 3:8 title "COSY-opposite-Sfz-Sdz-beta_y"

unset label 
set label "deuteron,  E= 2.11 GeV,  {/Symbol b} =0.497" at 100, 0
set ylabel "D [m]"
set title "COSY-opposite-Sfz-Sdz Dispersion Function"
set arrow 1 from 63.22, -15.0 to 63.22, 15      # MXL 
set label " mx06" at 63.22,  15 left
set arrow 2 from 78.06, -15.0 to 78.06, 18           # MXL 
set label " mx08" at 78.30, 18 left 
set arrow 3 from 154.94, -15.0 to 154.94, 15    # MXL 
set label "mx15 " at 154.94, 15 right
set arrow 4 from 169.79, -15.0 to 169.79, 17   # MXL 
set label "mx17 " at 169.79, 17 right

set arrow 5 from 69.54, -15.0 to 69.54, 35        # MXG     
set label "mx07 " at 69.54, 35 right
set arrow 6 from 161.25, -15.0 to 161.25, 35    # MXG 
set label "mx16 " at 161.25, 35 right

set arrow 7 from 57.63, -15.0 to 57.63, 2           # MXS
set label "mx05 " at 57.63, 2 right
set arrow 8 from 83.74, -15.0 to 83.74, 5        # MXS
set label " mx09" at 83.74, 5 left
set arrow 9 from 149.37, -15.0 to 149.37, 5      # MXS
set label "mx14 " at 149.37, 5 right
set arrow 10 from 175.49, -15.0 to 175.49, 2      # MXS
set label " mx18" at 175.49, 2 left

set arrow 11 from 22.05, -15  to 22.05, 5       # MXS
set label "mx02 " at 22.05, 5 right
set arrow 12 from 27.5, -15 to 27.5, 5
set label " mx03" at 27.5, 5 left
set arrow 13 from 37.4, -15 to 37.4, -8
set label "mx04 " at  37.4, -8 right
set arrow 14 from 102, -15 to 102, -6
set label " mx10" at 102, -6 left
set arrow 15 from 112, -15 to 112, 6
set label "mx11 " at 112, 6 right
set arrow 16 from 121, -15 to 121, 6
set label " mx12" at 121, 6 left
set arrow 17 from 131, -15 to 131, -6
set label "mx13 " at 131, -6 right

set output "pdf/COSY-opposite-Sfz-Sdz-DispersionFunction.pdf"
plot  "ring.twiss" using 3:7 title "COSY-opposite-Sfz-Sdz-eta_x"
        




