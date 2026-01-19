#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

void read_root_file()
{
    // 1. Open the TFile on the Heap ('new') so it doesn't close when the function ends
    auto in_file = new TFile("my_rootfile.root", "READ");

    // Check if file is open
    if (!in_file || in_file->IsZombie())
    {
        printf("Error: Could not open file!\n");
        return;
    }

    // 2. Get the Histogram out
    TH1F *h = nullptr;
    in_file->GetObject("my_histogram", h);

    if (!h)
    {
        printf("Error: Histogram 'my_histogram' not found in file!\n");
        return;
    }

    // 3. Draw it
    // We create a canvas on the heap too
    auto myCanvas = new TCanvas("c_read_root", "Read Root File", 800, 600);

    // We can use Draw() since the file stays open
    h->Draw();

    // Or usually we detach the histogram from the file so we can close the file:
    // h->SetDirectory(0);
    // in_file->Close();
}