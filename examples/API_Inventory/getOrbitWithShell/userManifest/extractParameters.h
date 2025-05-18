 std::string sxfFile = argv[1];

 std::string apdfFile = "./getOrbitWithShell/apdf/eteapot.apdf";

 int split = 1;            // old split specification
 int splitForBends = 0;    // new split specification
 int splitForQuads = 0;    // new split specification
 int splitForSexts = 0;    // new split specification
 int order = 2;
 int turns;                // specified as 1 in trtrin (for post processing)
                           // might be overwritten tp multiple turns (e.g. 10) in simulatedProbeValues
