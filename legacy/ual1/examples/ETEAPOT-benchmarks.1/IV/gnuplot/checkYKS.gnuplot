# gnuplot> load "gnuplot/checkYKS.gnuplot"

 set xlabel "turn number, j"
 set ylabel "x"
# set xrange [5000:5050]
# set yrange [36.05:36.25]
 set grid
 set title 'Replicate YKS result, m=0'
# set terminal postscript eps enhanced 20 
# set output "CheckYKS.eps"

set multiplot layout 4,1
p 'IG' i 1 u 2:4 w l
p 'IG' i 2 u 2:4 w l
p 'IG' i 3 u 2:4 w l
p 'IG' i 4 u 2:4 w l
unset multiplot

# p 'IG' i 1 u 2:4 w l, 'IG' i 2 u 2:4 w l,  'IG' i 3 u 2:4 w l,  'IG' i 4 u 2:4 w l,\
#   'IG' i 5 u 2:4 w l, 'IG' i 6 u 2:4 w l,  'IG' i 7 u 2:4 w l,  'IG' i 8 u 2:4 w l,\
#   'IG' i 9 u 2:4 w l, 'IG' i 10 u 2:4 w l, 'IG' i 11 u 2:4 w l, 'IG' i 12 u 2:4 w l

