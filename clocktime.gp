reset
set ylabel 'time(sec)'
set xlabel 'N'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'
set logscale x 10
set datafile separator ","


plot [1000:][0:] "result_clock_gettime.csv" using 1:2 smooth csplines lw 2 title 'baseline', \
'' using 1:3 smooth csplines lw 2 title 'openMP(2)', \
'' using 1:4 smooth csplines lw 2 title 'openMp(4)', \
'' using 1:5 smooth csplines lw 2 title 'AVX', \
'' using 1:6 smooth csplines lw 2 title 'AVX_unroll', \
'' using 1:7 smooth csplines lw 2 title 'euler', \
'' using 1:8 smooth csplines lw 2 title 'leibniz'


