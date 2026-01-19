#include <TChain.h>
#include <TSystem.h>
#include <TROOT.h>
#include <iostream>

void macro13()
{
    // 1. Create a TChain
    // TChain connects multiple files into one logical tree
    TChain *ch = new TChain("cond_data", "My Chain for Example N-Tuple");

    // 2. Add Data Files safely
    const char* largeFile = "conductivity_experiment_large.root";
    const char* smallFile = "conductivity_experiment.root";

    // gSystem->AccessPathName returns kFALSE (0) if file exists
    bool largeExists = !gSystem->AccessPathName(largeFile);
    bool smallExists = !gSystem->AccessPathName(smallFile);

    if (largeExists) 
    {
        std::cout << "Found large dataset. Loading..." << std::endl;
        ch->Add(largeFile);
    }
    else if (smallExists) 
    {
        std::cout << "Large file not found. Falling back to small file..." << std::endl;
        ch->Add(smallFile);
    }
    else 
    {
        std::cout << "No data files found! Automatically generating large dataset..." << std::endl;
        std::cout << "This may take a minute..." << std::endl;
        
        // Run the generation macro automatically
        gROOT->ProcessLine(".x generate_large_data.C");
        
        // Check again
        if (!gSystem->AccessPathName(largeFile)) {
            ch->Add(largeFile);
        } else {
            std::cout << "Error: Data generation failed. Please check generate_large_data.C" << std::endl;
            return;
        }
    }

    Long64_t nEntries = ch->GetEntries();
    if (nEntries == 0)
    {
        std::cout << "Error: Chain is empty." << std::endl;
        return;
    }

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Processing " << nEntries << " events locally." << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    // 3. Process
    // "MySelector.C+" compiles the selector using ACLiC
    ch->Process("MySelector.C+");
}