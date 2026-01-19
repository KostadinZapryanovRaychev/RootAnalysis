#include <TChain.h>
#include <TSystem.h>
#include <TROOT.h>
#include <iostream>

void RunMySelector()
{
    // 1. Create a TChain
    TChain *ch = new TChain("cond_data", "My Chain for Example N-Tuple");

    // 2. data file paths
    const char *largeFile = "conductivity_experiment_large.root";
    const char *smallFile = "conductivity_experiment.root";

    // 3. Check and load files safely
    if (!gSystem->AccessPathName(largeFile))
    {
        std::cout << "Found large dataset: " << largeFile << std::endl;
        ch->Add(largeFile);
    }
    else if (!gSystem->AccessPathName(smallFile))
    {
        std::cout << "Large file missing. Found small dataset: " << smallFile << std::endl;
        ch->Add(smallFile);
    }
    else
    {
        std::cout << "No data found. Generating 10M events (this takes a moment)..." << std::endl;
        // Execute the generation macro
        gROOT->ProcessLine(".x generate_large_data.C");

        // Try to add again
        if (!gSystem->AccessPathName(largeFile))
        {
            ch->Add(largeFile);
        }
        else
        {
            std::cout << "Error: File generation failed." << std::endl;
            return;
        }
    }

    if (ch->GetEntries() == 0)
    {
        std::cout << "Error: Chain is empty!" << std::endl;
        return;
    }

    std::cout << "Processing " << ch->GetEntries() << " events." << std::endl;

    // 4. Process
    // The "+" ensures compilation, which is mandatory for PROOF capabilities
    ch->Process("MySelector.C+");
}