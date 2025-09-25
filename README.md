Unified Accelerator Libraries for Alma Linux 9.

AlmaLinux 8 is probably ok.

AlmaLinux 10 is probably ok.

The main issue is the C/C++ compiler gcc/g++.

The latest, 11.5.0, as of 6/1/2025, is mostly ok. A legacy root (CERN) build requires gcc 4.8.5 or so.

This detail, and others are covered in more depth later.

________________________________________________________________________

UAL software goes back ca 30 years.

There's a "legacy" component. It is is primarily text based. It is "bootstrapped" into existence. Follow
          https://github.com/talman/ual2/tree/main/bootstrap-instructions
is probably best.

There's a "latest" component. It is gui based. It is, perhaps, more configurable. A suggested run mode follows.
