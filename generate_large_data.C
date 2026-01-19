#include "TFile.h"
#include "TNtuple.h"
#include "TRandom3.h"
#include <iostream>

using namespace std;

// Generate a LARGE n-tuple (10 million events) to test PROOF performance
void generate_large_data()
{
    cout << "--- Generating 10 Million Events for PROOF Test ---" << endl;
    cout << "This might take a moment..." << endl;

    // 1. Create the File
    TFile ofile("conductivity_experiment_large.root", "RECREATE");

    // 2. Initialise the TNtuple
    TNtuple cond_data("cond_data",
                      "Example N-Tuple",
                      "Potential:Current:Temperature:Pressure");

    // 3. Fill it randomly
    TRandom3 rndm;
    float pot, cur, temp, pres;

    int n_events = 10000000; // 10 Million

    for (int i = 0; i < n_events; ++i)
    {
        if (i % 1000000 == 0)
            cout << "Processing event " << i << "..." << endl;

        // Generate physical parameters
        pot = rndm.Uniform(0., 10.);
        temp = rndm.Uniform(250., 350.);
        pres = rndm.Uniform(0.5, 1.5);

        // Calculate physics
        cur = pot / (10. + 0.05 * (temp - 300.) - 0.2 * (pres - 1.));

        // Add noise
        pot *= rndm.Gaus(1., 0.01);
        temp += rndm.Gaus(0., 0.3);
        pres *= rndm.Gaus(1., 0.02);
        cur *= rndm.Gaus(1., 0.01);

        cond_data.Fill(pot, cur, temp, pres);
    }

    // 4. Save and Close
    cond_data.Write();
    ofile.Close();

    cout << "File 'conductivity_experiment_large.root' written successfully." << endl;
}