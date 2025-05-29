#!/usr/local/bin/perl

# Re-sort all lines in "./NikolayOut" into groups with all turn of particle 0
# then two blank lines (the required gnuplot data separator) then all turns for
# particle 1 then two blanks, and so on. 

# For bunches with other than 23 particles change "$NUMPARTICLES".
# This is not necessary if the number of particles is less than 23

# "SplitNikolayOut-mod.pl" separates output by particle number 
# into the "IGs" directory into the "IGs/IG1, IGs/IG2, ..." files   

# perl ~/perl/SplitNikolayOut-mod.pl NikolayOut >! InputForGnuplot

$, = ' ';		# set output field separator
$\ = "\n";		# set output record separator

$NUMPARTICLES = 23;
while (<>) {
    chop;	# strip record separator
    $LineTemp = $_;
    @Fld = split(' ', $LineTemp, 9999);
    $ParamsByPartNum[$Fld[0]] .= $LineTemp . "\n";
}

open(IG1, ">IG1") || die "Could not open IG1\n";
open(IG2, ">IG2") || die "Could not open IG2\n";
open(IG3, ">IG3") || die "Could not open IG3\n";
open(IG4, ">IG4") || die "Could not open IG4\n";
open(IG5, ">IG5") || die "Could not open IG5\n";
open(IG6, ">IG6") || die "Could not open IG6\n";
open(IG7, ">IG7") || die "Could not open IG7\n";
open(IG8, ">IG8") || die "Could not open IG8\n";
open(IG9, ">IG9") || die "Could not open IG9\n";
open(IG10, ">IG10") || die "Could not open IG10\n";
open(IG11, ">IG11") || die "Could not open IG11\n";
open(IG12, ">IG12") || die "Could not open IG12\n";
open(IG13, ">IG13") || die "Could not open IG13\n";
open(IG14, ">IG14") || die "Could not open IG14\n";
open(IG15, ">IG15") || die "Could not open IG15\n";
open(IG16, ">IG16") || die "Could not open IG16\n";
open(IG17, ">IG17") || die "Could not open IG17\n";
open(IG18, ">IG18") || die "Could not open IG18\n";
open(IG19, ">IG19") || die "Could not open IG19\n";
open(IG20, ">IG20") || die "Could not open IG20\n";
open(IG21, ">IG21") || die "Could not open IG21\n";
open(IG22, ">IG22") || die "Could not open IG22\n";
open(IG23, ">IG23") || die "Could not open IG23\n";

for ($i=0; $i<$NUMPARTICLES; $i++) {
    if ($i == 1) {print IG1 $ParamsByPartNum[1] . "\n";}
    if ($i == 2) {print IG2 $ParamsByPartNum[2] . "\n";}
    if ($i == 3) {print IG3 $ParamsByPartNum[3] . "\n";}
    if ($i == 4) {print IG4 $ParamsByPartNum[4] . "\n";}
    if ($i == 5) {print IG5 $ParamsByPartNum[5] . "\n";}
    if ($i == 6) {print IG6 $ParamsByPartNum[6] . "\n";}
    if ($i == 7) {print IG7 $ParamsByPartNum[7] . "\n";}
    if ($i == 8) {print IG8 $ParamsByPartNum[8] . "\n";}
    if ($i == 9) {print IG9 $ParamsByPartNum[9] . "\n";}
    if ($i == 10) {print IG10 $ParamsByPartNum[10] . "\n";}
    if ($i == 11) {print IG11 $ParamsByPartNum[11] . "\n";}
    if ($i == 12) {print IG12 $ParamsByPartNum[12] . "\n";}
    if ($i == 13) {print IG13 $ParamsByPartNum[13] . "\n";}
    if ($i == 14) {print IG14 $ParamsByPartNum[14] . "\n";}
    if ($i == 15) {print IG15 $ParamsByPartNum[15] . "\n";}
    if ($i == 16) {print IG16 $ParamsByPartNum[16] . "\n";}
    if ($i == 17) {print IG17 $ParamsByPartNum[17] . "\n";}
    if ($i == 18) {print IG18 $ParamsByPartNum[18] . "\n";}
    if ($i == 19) {print IG19 $ParamsByPartNum[19] . "\n";}
    if ($i == 20) {print IG20 $ParamsByPartNum[20] . "\n";}
    if ($i == 21) {print IG21 $ParamsByPartNum[21] . "\n";}
    if ($i == 22) {print IG22 $ParamsByPartNum[22] . "\n";}
    if ($i == 23) {print IG13 $ParamsByPartNum[23] . "\n";}
}
