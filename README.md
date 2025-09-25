Unified Accelerator Libraries (UAL) for Alma Linux 9.

AlmaLinux 8 is probably ok. AlmaLinux 10 is probably ok.

An important issue is the C/C++ compiler gcc/g++.

The latest, 11.5.0, as of 6/1/2025, is mostly ok. A legacy root (CERN) build requires gcc 4.8.5 or so. This is provided in this git clone download.

This detail, and others, are covered in more depth elsewhere. Hopefully, they can be mostly transparent to the user.

UAL software goes back ca 25 years, and is multifaceted.

________________________________________________________________________

UAL has a "legacy" component. It is is primarily command line based. It must be built first. It can be "bootstrapped" into existence:

          https://github.com/jtalman/ual2/tree/main/bootstrap-instructions/minimal

Following this syntax exactly is probably best.

________________________________________________________________________

UAL has a "latest" component that requires the previous step, a fully built legacy component. It is gui based. It is intended to be (fairly) easily configurable. A suggested run mode follows.

"GUI-LATEST":

          cd /home/ualusr2011/git-gui-2/ual2
          source OS-and-SHELL/TCSHRC-ual2
          ./run-script

________________________________________________________________________

ENJOY!!
