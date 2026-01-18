#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include <fstream>
#include <iostream>

using namespace std;

void histogram()
{
    // -------------------------------------------------------------
    // PART 1: Create the data file (so we have something to read)
    // -------------------------------------------------------------
    TF1 *efunc = new TF1("efunc", "exp([0]+[1]*x)", 0., 5.);
    efunc->SetParameter(0, 1);
    efunc->SetParameter(1, -1);

    // Create a dummy file named 'expo.dat' and write 1000 numbers to it
    ofstream out("expo.dat");
    for (int i = 0; i < 1000; i++)
    {
        out << efunc->GetRandom() << endl;
    }
    out.close();
    cout << "Generated 'expo.dat' with 1000 random numbers." << endl;

    // -------------------------------------------------------------
    // PART 2: The Logic You Requested (Read from file)
    // -------------------------------------------------------------

    // Define the histogram
    TH1F *h = new TH1F("h", "example histogram read from file", 100, 0., 5.);

    // Open the file
    ifstream inp;
    double x;
    inp.open("expo.dat");

    if (!inp.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }

    // Read loop: while we can successfully read a number 'x'
    while (inp >> x)
    {
        h->Fill(x);
    }

    inp.close();

    // -------------------------------------------------------------
    // PART 3: Draw
    // -------------------------------------------------------------
    h->Draw();
}