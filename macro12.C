#include "TChain.h"
#include <TROOT.h>
#include <iostream>

void macro12()
{
    // 1. Create a Chain
    // TChain is like a playlist of TFiles. It allows you to process
    // multiple ROOT files as if they were a single large tree.
    TChain *ch = new TChain("cond_data", "Chain for Example N-Tuple");

    // 2. Add Files
    // The asterisk (*) wildcards allow adding multiple files
    int nFiles = ch->Add("conductivity_experiment*.root");

    // 3. Check if files were actually found
    if (nFiles == 0)
    {
        std::cout << "No files matching 'conductivity_experiment*.root' found." << std::endl;
        std::cout << "Generating data now..." << std::endl;
        gROOT->ProcessLine(".x generate_large_data.C");
        ch->Add("conductivity_experiment_large.root");
    }

    if (ch->GetEntries() == 0)
    {
        std::cout << "Error: Chain is empty." << std::endl;
        return;
    }

    std::cout << "Chain contains " << ch->GetEntries() << " entries." << std::endl;
    std::cout << "Processing with MySelector..." << std::endl;

    // 4. Process the Chain
    // "MySelector.C+" means:
    //  - Load MySelector.C
    //  - The "+" tells ROOT to COMPILE it (ACLiC) for speed (~10x faster than interpreting)
    ch->Process("MySelector.C+");
}