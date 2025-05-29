#!/usr/local/bin/perl

# perl theta0-evolution.pl MunozRTcompareOUT >! theta0-evolution.data

$, = ' ';		# set output field separator
$\ = "\n";		# set output record separator

$ielem = 0;
while (<>) {
    chop;	# strip record separator
    $LineTemp = $_;
    if( $LineTemp =~ /^JDTRT-pOf4theta0/ ){
	$ielem++;
	@Fld = split(' ', $LineTemp, 9999);
	$p0[$ielem] = $Fld[1];
	}
}
$nelem = $ielem;

for ( $ielem=1; $ielem<$nelem; $ielem++ ){
    $LineOut = $ielem." ".$p0[$ielem];
    print $LineOut;
}
