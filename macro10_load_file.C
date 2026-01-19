#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include <iostream>

using namespace std;

// Function to WRITE the histogram to a file
void write_to_file() {
    cout << "--- Writing to file... ---" << endl;

    // 1. Create Instance of our histogram
    // Stack allocation is fine here because we write it instantly
    TH1F h("my_histogram", "My Title;X;# of entries", 100, -5, 5);

    // 2. Fill it randomly with Gaussian distribution
    h.FillRandom("gaus", 10000);

    // 3. Open a TFile
    // "RECREATE": Overwrite if exists, Create if not.
    TFile out_file("my_rootfile.root", "RECREATE");

    // 4. Write the histogram to the file
    // internally uses the name of the object ("my_histogram") as the key
    h.Write();

    // 5. Close the file to save changes
    out_file.Close();
    
    cout << "File 'my_rootfile.root' created successfully." << endl;
}

// Function to READ the histogram back from the file
void read_from_file() {
    cout << "--- Reading from file... ---" << endl;

    // 1. Open the file in Read mode
    TFile *in_file = new TFile("my_rootfile.root", "READ");

    // Check if open
    if (in_file->IsZombie()) {
        cout << "Error opening file!" << endl;
        return;
    }

    // 2. Retrieve the object
    // We need to provide a pointer to hold the object.
    // We cast the result of Get() to the correct type (TH1F*)
    TH1F *h = nullptr;
    in_file->GetObject("my_histogram", h);

    if (!h) {
        cout << "Histogram not found in file!" << endl;
        return;
    }

    // 3. Draw it
    // Note: When 'in_file' closes or goes out of scope, 'h' might become invalid 
    // depending on directory association. 
    // To keep it alive after file close, we could verify SetDirectory(0).
    // Here we just draw it while file is open.
    auto c1 = new TCanvas("c_read", "Read from File");
    h->DrawClone(); // DrawClone ensures the plot stays on the canvas even if we leave the function
}

// Main entry point
void macro10_load_file() {
    // First, generate the file
    write_to_file();

    // Then, read it back to verify
    read_from_file();
}