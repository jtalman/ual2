#!/usr/bin/perl

my $job_name   = "UI_CLS_SXF";

use File::Path;
mkpath(["./out/" . $job_name], 1, 0755);

# ------------------------------------------------------
# Create the ALE::UI::Shell  instance 
# ------------------------------------------------------

use lib ("$ENV{UAL_EXTRA}/ALE/api");
use ALE::UI::Shell;

print "Create the ALE::UI::Shell instance", "\n";

my $shell = new ALE::UI::Shell("print" => "./out/" . $job_name . "/log");

# ------------------------------------------------------
# Define the space of Taylor maps
# ------------------------------------------------------

print "Define the space of Taylor maps", "\n";

$shell->setMapAttributes("order" => 5);

# ------------------------------------------------------
# Define design elements and beam lines
# ------------------------------------------------------

# Read the SXF file

print "read the SXF file   ", time, "\n";

use lib ("$ENV{UAL_SXF}/api");  # Note that this finds "api" in the "UAL_SXF" directory
use UAL::SXF::Parser;
my $sxf_parser = new UAL::SXF::Parser();

$sxf_parser->read("./data/CLS.sxf", "./out/" . $job_name . "/echo.sxf");

# ------------------------------------------------------
# Select and initialize a lattice for operations
# ------------------------------------------------------

print "Select and initialize a lattice", "\n";

# Select an accelerator for operations

$shell->use("lattice" => "CLS");

# ------------------------------------------------------
# Add apertures
# ------------------------------------------------------

$shell->addAperture("elements" => "^dxmp", 
                    "aperture" => [1., 0.116, 0.079]);

# ------------------------------------------------------
# Write the lattice state to the SXF file
# ------------------------------------------------------

$sxf_parser->write("./out/" . $job_name . "/echo.sxf");

# ------------------------------------------------------
# Define beam parameters
# ------------------------------------------------------

print "Define beam parameters", "\n";

$shell->setBeamAttributes("energy" => 3.0, "mass" => 0.00051099895069 );  # energy=3GeV

# ------------------------------------------------------
# Analysis
# ------------------------------------------------------

print "Linear analysis", "\n";

$shell->analysis("print" => "./out/" . $job_name . "/analysis"); 

print "Make the second-order map", "\n";

$shell->map("order" => 2, "print" => "./out/" . $job_name . "/map2"); 

# ------------------------------------------------------
# Track bunch of particles 
# ------------------------------------------------------

print "Track bunch of particles", "\n";

my ($i, $size) = (0, 1);

my $bunch = new ALE::UI::Bunch($size);

#  $bunch->setBeamAttributes(1.93827231, 0.93827231);  # This was for proton tracking
$bunch->setBeamAttributes(3.0, 0.00051099895069);        # This is for electron tracking

for($i =0; $i < $size; $i++){$bunch->setPosition($i, 1.e-2*($i+1), 0.0, 1.e-2*($i+1), 0.0, 0.0, 1.e-3*($i+1));
}
# $shell->run("turns" => 512, "step" => 1, "print" => "./out/" . "/test/" . "/fort.8" ,  "bunch" => $bunch);
$shell->run("turns" => 512, "step" => 1, "print" => "./out/" . "/fort.8RAW" ,  "bunch" => $bunch);

# ------------------------------------------------------------------------
# Copy only the last 512 turns of out/fort.8RAW to out/test/fort.8
# ------------------------------------------------------------------------

print "Copy only the last 512 turns of out/fort.8RAW to out/test/fort.8', p. 320 of Perl Cookbook---NOT", "\n";
system 'tail -512 "out/fort.8RAW" > "out/test/fort.8" ';

# ------------------------------------------------------------------------
# Plot turn-by-turn data
# ------------------------------------------------------------------------

system 'xmgrace -noask -b xyzfft.com &';
print "End", "\n";

1;
