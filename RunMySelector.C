#include <TChain.h>
// #include <TProof.h> // PROOF not installed
#include <iostream>

void RunMySelector()
{
    // 1. Create a TChain
    TChain *ch = new TChain("cond_data", "My Chain for Example N-Tuple");

    // Add the newly created LARGE file
    // Note: We use wildcards so you can add multiple files if you generated them
    ch->Add("conductivity_experiment_large.root");

    // If you don't have the large file yet, fallback to the small one
    if (ch->GetEntries() == 0)
    {
        std::cout << "Large file not found, using small file..." << std::endl;
        ch->Add("conductivity_experiment.root");
    }

    std::cout << "Processing " << ch->GetEntries() << " events." << std::endl;

    // 2. Start PROOF Lite
    // workers=4 opens 4 parallel processes.
    // If you have more cores, you can increase this number (e.g., workers=8)
    // If you leave it empty (""), it uses all available cores.
    // TProof::Open("workers=4"); // Disabled: PROOF not installed

    // 3. Enable PROOF on the chain
    // ch->SetProof(); // Disabled: Run locally instead

    // 4. Process
    // The "+" ensures compilation, which is mandatory for PROOF
    ch->Process("MySelector.C+");
}