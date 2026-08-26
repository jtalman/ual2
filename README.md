Unified Accelerator Libraries (UAL) for Alma Linux 9.

AlmaLinux 8 is probably ok. AlmaLinux 10 is probably ok.

Any current linux flavor is likely to be readily tractable.

An important issue is the C/C++ compiler gcc/g++.

The ca latest, gcc version 11.5.0, as of ca 2025, is mostly ok. A legacy root (CERN) build requires gcc version 4.8.5 or so. This is provided in this git clone download.

This detail, and others, are covered in more depth elsewhere. Hopefully, they can be mostly transparent to the user.

UAL software goes back ca 25 years, and is multifaceted.

________________________________________________________________________

UAL has a "legacy" component. It is is primarily command line based. It must be built first. It can be "bootstrapped" into existence:

          https://github.com/jtalman/ual2/tree/main/bootstrap-instructions/minimal

Following this syntax exactly is recommended.

"ual2" is the successor to "ual1", both short hand for a UAL repository.

________________________________________________________________________

UAL has a "latest" component that requires the previous step, a fully built legacy component. It is gui based. It is intended to be (fairly) easily configurable. A suggested run mode follows.

"GUI-LATEST":

          cd /home/ualusr2011/git-gui-2/ual2
          source OS-and-SHELL/TCSHRC-ual2
          pushd player
          make clean
          make
          popd
          ./run-script

NOTE:

    NOTE that the sxf file and apdf file are set via the source command! 
              This, in turn, sets the initial timestamp.
              This initial timestamp can be edited.
                        Don't leave a blank space after "apdf"!

________________________________________________________________________

ENJOY!!
________________________________________________________________________
________________________________________________________________________
________________________________________________________________________


UAL has a newish, "developmental / exploratory", component.

This is oriented towards Solar System physics, most prominently the Sun's magnetic field.

Away from the solid and gaseous regions, spatial low pressure makes coherent "accelerator processes" feasible.

Probly not at the level of granularity assumed for the legacy and latest codebases!

It's not clear where this newest codebase is headed. Possibly just an archive where tentative code can be recovered for different project(s).

For now, this newish code is all in directory:
     https://github.com/jtalman/ual2/tree/main/utilities/


John Talman wishes to gratefully acknowledge:

     Song Ho Ahn (안성호)!
          https://www.songho.ca/index.html
     Thanks for some wonderful code!

     Paul Rademacher:
          https://github.com/libglui/glui
     Thanks for some wonderful code!

     Mike Bailey:
          https://web.engr.oregonstate.edu/~mjb/WebMjb/mjb.html
     Thanks for some wonderful code!
     Thanks for some brilliant instruction!
          glut, with glui, is a nice combo!!


YMMV!!   ;-)
________________________________________________________________________
