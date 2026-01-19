#include <TChain.h>
#include <TProof.h>
#include <iostream>
#include <TProof.h>

void macro13()
{
    // 1. Create a TChain
    // TChain connects multiple files into one logical tree
    TChain *ch = new TChain("cond_data", "My Chain for Example N-Tuple");

    // Add the data file.
    // We try to add the large file first (created by generate_large_data.C)
    // If it doesn't exist, we fallback to the small one.
    // The wildcard '*' allows adding many files at once.
    if (ch->Add("conductivity_experiment_large.root") == 0)
    {
        std::cout << "Large file not found. Falling back to small file..." << std::endl;
        ch->Add("conductivity_experiment.root");
    }

    Long64_t nEntries = ch->GetEntries();
    if (nEntries == 0)
    {
        std::cout << "Error: No data found! Run 'root generate_large_data.C' first." << std::endl;
        return;
    }

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Processing " << nEntries << " events with PROOF Lite." << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    // 2. Start PROOF Lite
    // "workers=4" tells ROOT to spin up 4 separate worker processes on your CPU.
    // If you omit the argument (""), ROOT uses all available cores.
    TProof::Open("workers=4");

    // 3. Enable PROOF on the chain
    // This tells the Chain to distribute entries to the workers instead of processing locally.
    ch->SetProof();

    // 4. Process
    // "MySelector.C+" compiles the selector using ACLiC (Automatic Compiler of Libraries for C++)
    // Compilation is MANDATORY for PROOF use.
    ch->Process("MySelector.C+");
}