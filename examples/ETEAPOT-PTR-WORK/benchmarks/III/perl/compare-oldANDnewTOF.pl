#!/usr/local/bin/perl

# perl $UAL/examples/ETEAPOT/perl/compare-oldANDnewTOF.pl MunozRTcompareOUT >! MunozRTCompare.data
###### perl $UAL/examples/ETEAPOT/perl/compare-oldANDnewTOF.pl newTOF-OUT >! newTOF.data

$, = ' ';		# set output field separator
$\ = "\n";		# set output record separator

$ielem = 0;
$begun = 0;
while (<>) {
    chop;	# strip record separator
    $LineTemp = $_;
    if( $begun == 0 && $LineTemp =~ /^JDTRT-MunozRT/ ){
	$ielem++;
	$begun = 1;
	}
    if( $begun == 1 && $LineTemp =~ /^JDTRT-pOf4-xi_out_RT/ ){
	@Fld = split(' ', $LineTemp, 9999);
	$p0[$ielem] = $Fld[1];
	}
    if( $begun == 1 && $LineTemp =~ /^JDTRT-pOf4-drMunoz/ ){
	@Fld = split(' ', $LineTemp, 9999);
	$p1[$ielem] = $Fld[1];
        $begun = 0;
	}
}
$nelem = $ielem;

for ( $ielem=1; $ielem<$nelem; $ielem++ ){
    $LineOut = $ielem." ".$p0[$ielem]." ".$p1[$ielem];
    print $LineOut;
}
