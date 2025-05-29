source $UAL/setup-linux-ual
setenv LD_LIBRARY_PATH $SXF/lib/$SXF_ARCH/:$LD_LIBRARY_PATH

make clean

\cp $UAL/examples/ETEAPOT/*.hh .
\cp $UAL/examples/ETEAPOT/extractParameters.h .
\cp $UAL/examples/ETEAPOT/simulatedProbeValues .
\cp $UAL/examples/ETEAPOT/trtrin .

: \cp $UAL/examples/ETEAPOT/trtrout .
: \cp ../1/trtrout .

\cp $UAL/examples/ETEAPOT/printProbeValues .
\cp $UAL/examples/ETEAPOT/data/eteapotMARKER.apdf data/eteapot.apdf
\cp $UAL/examples/ETEAPOT/userBunch data
\cp $UAL/examples/ETEAPOT/determineTwiss.cc .

\cp $UAL/examples/ETEAPOT/sip* .
\cp $UAL/examples/ETEAPOT/probeDataForTwiss .
\cp $UAL/examples/ETEAPOT/transferMatrices.cc .

make

./determineTwiss  data/E_ValLeb2-sl4.sxf 0.2 40 >! OUT

: VERIFY   <<<<----
./transferMatrices 0.2 -1.08781e-09 29.222 -1.41916e-10 110.653 982>! betaFunctions
: VERIFY   <<<<----

gnuplot
   load "gnuplot/BM-IV_Figure1r-2r.gnuplot"

