Unified Accelerator Libraries (UAL) for Alma Linux 9.

AlmaLinux 8 is probably ok.

AlmaLinux 10 is probably ok.

An important issue is the C/C++ compiler gcc/g++.

The latest, 11.5.0, as of 6/1/2025, is mostly ok. A legacy root (CERN) build requires gcc 4.8.5 or so.

This detail, and others are covered in more depth later.

________________________________________________________________________

UAL software goes back ca 30 years, and is quite extensive.

There's a "legacy" component. It is is primarily text based. It must be built first. It can be "bootstrapped" into existence:

          https://github.com/talman/ual2/tree/main/bootstrap-instructions/minimal

Following this syntax exactly is probably best.

________________________________________________________________________

There's a "latest" component that requires the legacy component. It is gui based. It is intended to be (fairly) easily configurable. A suggested run mode follows.

"GUI-LATEST":
          cp $UAL2/OS-and-SHELL/TCSHRC-ual2 ~/.tcshrc
