#include "TFile.h"
#include "TNtuple.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include <iostream>

using namespace std;

// Fill an n-tuple and write it to a file simulating measurement of
// conductivity of a material in different conditions of pressure
// and temperature.
void write_ntuple_to_file()
{
    cout << "--- Generating NTuple Data ---" << endl;

    // 1. Create the File
    TFile ofile("conductivity_experiment.root", "RECREATE");

    // 2. Initialise the TNtuple
    // Syntax: Name, Title, Colon-separated list of variables
    TNtuple cond_data("cond_data",
                      "Example N-Tuple",
                      "Potential:Current:Temperature:Pressure");

    // 3. Fill it randomly to fake the acquired data
    TRandom3 rndm;
    float pot, cur, temp, pres;

    for (int i = 0; i < 10000; ++i)
    {
        // Generate physical parameters
        pot = rndm.Uniform(0., 10.);     // Voltage 0-10 V
        temp = rndm.Uniform(250., 350.); // Temperature 250-350 K
        pres = rndm.Uniform(0.5, 1.5);   // Pressure 0.5-1.5 atm

        // Calculate physics: I = V / R(T, P)
        cur = pot / (10. + 0.05 * (temp - 300.) - 0.2 * (pres - 1.));

        // Add some random smearing (measurement errors)
        pot *= rndm.Gaus(1., 0.01);  // 1% error on voltage
        temp += rndm.Gaus(0., 0.3);  // 0.3 abs. error on temp.
        pres *= rndm.Gaus(1., 0.02); // 2% error on pressure
        cur *= rndm.Gaus(1., 0.01);  // 1% error on current

        // Write row to ntuple
        cond_data.Fill(pot, cur, temp, pres);
    }

    // 4. Save and Close
    cond_data.Write();
    ofile.Close();

    cout << "File 'conductivity_experiment.root' written successfully." << endl;
}

// Helper function to verify the file content
void read_ntuple_from_file()
{
    cout << "--- Reading NTuple Data ---" << endl;

    // Open file (Heap allocation to keep it open)
    auto f = new TFile("conductivity_experiment.root", "READ");

    // Retrieve NTuple
    TNtuple *cond_data = (TNtuple *)f->Get("cond_data");

    if (!cond_data)
    {
        cout << "Error: TNtuple not found!" << endl;
        return;
    }

    cout << "Loaded NTuple with " << cond_data->GetEntries() << " entries." << endl;

    // Visual Check: Plot Current vs Potential
    // This should look linear (Ohm's Law)
    auto c1 = new TCanvas("c_ntuple", "Conductivity Check", 800, 600);
    cond_data->Draw("Current:Potential", "", "COLZ");
}

// Main function
void test_tuple_macro11()
{
    write_ntuple_to_file();
    read_ntuple_from_file();
}