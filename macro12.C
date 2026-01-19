#include "TChain.h"
#include <iostream>

void macro12()
{
    // 1. Create a Chain
    // TChain is like a playlist of TFiles. It allows you to process
    // multiple ROOT files as if they were a single large tree.
    TChain *ch = new TChain("cond_data", "Chain for Example N-Tuple");

    // 2. Add Files
    // The asterisk (*) wildcards allow adding multiple files (e.g., data_run1.root, data_run2.root)
    // Here we just have one file, but the syntax is generic.
    ch->Add("conductivity_experiment*.root");

    std::cout << "Chain contains " << ch->GetEntries() << " entries." << endl;
    std::cout << "Processing with MySelector..." << endl;

    // 3. Process the Chain
    // "MySelector.C+" means:
    //  - Load MySelector.C
    //  - The "+" tells ROOT to COMPILE it (ACLiC) for speed (~10x faster than interpreting)
    ch->Process("MySelector.C+");
}