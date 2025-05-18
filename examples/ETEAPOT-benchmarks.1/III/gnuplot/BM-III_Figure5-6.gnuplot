# gnuplot > load "gnuplot/BM-III_Figure5-6.gnuplot"

# gnuplot > load "gnuplot/BM-III_Figure3.gnuplot"
# set terminal x11 enhanced font "Arial,15"
# set term postscript enhanced font "Arial,16" clip
set term pdfcairo enhanced font "Arial,15"
# set term png enhanced notransparent 
#  set output '| display png:-'

# set terminal postscript eps enhanced color 20

set grid

set output "pdf/delta_vs_turn_V5p0.pdf"
p 'sIG_V5.0' i 10 u 2:9 lw 1, 'sIG_V5.0' i 11 u 2:9 lw 1, 'sIG_V5.0' i 12 u 2:9 lw 1

set output "pdf/delta_vs_ct_V5p0.pdf"
p 'sIG_V5.0' i 10 u 8:9 lw 1, 'sIG_V5.0' i 11 u 8:9 lw 1, 'sIG_V5.0' i 12 u 8:9 lw 1

set output "pdf/delta_vs_turn_V2p0.pdf"
p 'sIG_V2.0' i 10 u 2:9 lw 1, 'sIG_V2.0' i 11 u 2:9 lw 1, 'sIG_V2.0' i 12 u 2:9 lw 1

set output "pdf/delta_vs_ct_V2p0.pdf"
p 'sIG_V2.0' i 10 u 8:9 lw 1, 'sIG_V2.0' i 11 u 8:9 lw 1, 'sIG_V2.0' i 12 u 8:9 lw 1

set output "pdf/delta_vs_turn_V1p0.pdf"
p 'sIG_V1.0' i 10 u 2:9 lw 1, 'sIG_V1.0' i 11 u 2:9 lw 1, 'sIG_V1.0' i 12 u 2:9 lw 1

set output "pdf/delta_vs_ct_V1p0.pdf"
p 'sIG_V1.0' i 10 u 8:9 lw 1, 'sIG_V1.0' i 11 u 8:9 lw 1, 'sIG_V1.0' i 12 u 8:9 lw 1

set output "pdf/delta_vs_turn_V0p5.pdf"
p 'sIG_V0.5' i 10 u 2:9 lw 1, 'sIG_V0.5' i 11 u 2:9 lw 1, 'sIG_V0.5' i 12 u 2:9 lw 1

set output "pdf/delta_vs_ct_V0p5.pdf"
p 'sIG_V0.5' i 10 u 8:9 lw 1, 'sIG_V0.5' i 11 u 8:9 lw 1, 'sIG_V0.5' i 12 u 8:9 lw 1

set output "pdf/delta_vs_turn_V0p2.pdf"
p 'sIG_V0.2' i 10 u 2:9 lw 1, 'sIG_V0.2' i 11 u 2:9 lw 1, 'sIG_V0.2' i 12 u 2:9 lw 1

set output "pdf/delta_vs_ct_V0p2.pdf"
p 'sIG_V0.2' i 10 u 8:9 lw 1, 'sIG_V0.2' i 11 u 8:9 lw 1, 'sIG_V0.2' i 12 u 8:9 lw 1

set output "pdf/delta_vs_turn_V0p1.pdf"
p 'sIG_V0.1' i 10 u 2:9 lw 1, 'sIG_V0.1' i 11 u 2:9 lw 1, 'sIG_V0.1' i 12 u 2:9 lw 1

set output "pdf/delta_vs_ct_V0p1.pdf"
p 'sIG_V0.1' i 10 u 8:9 lw 1, 'sIG_V0.1' i 11 u 8:9 lw 1, 'sIG_V0.1' i 12 u 8:9 lw 1

set output "pdf/delta_vs_turn_V0p05.pdf"
p 'sIG_V0.05' i 10 u 2:9 lw 1, 'sIG_V0.05' i 11 u 2:9 lw 1, 'sIG_V0.05' i 12 u 2:9 lw 1

set output "pdf/delta_vs_ct_V0p05.pdf"
p 'sIG_V0.05' i 10 u 8:9 lw 1, 'sIG_V0.05' i 11 u 8:9 lw 1, 'sIG_V0.05' i 12 u 8:9 lw 1

set output "pdf/delta_vs_turn_V0p02.pdf"
p 'sIG_V0.02' i 10 u 2:9 lw 1, 'sIG_V0.02' i 11 u 2:9 lw 1, 'sIG_V0.02' i 12 u 2:9 lw 1

set output "pdf/delta_vs_ct_V0p02.pdf"
p 'sIG_V0.02' i 10 u 8:9 lw 1, 'sIG_V0.02' i 11 u 8:9 lw 1, 'sIG_V0.02' i 12 u 8:9 lw 1

set timestamp "%d/%m/%y %H:%M" offset 0,1.2 font "Helvetica"

