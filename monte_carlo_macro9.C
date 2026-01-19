#include "TH1F.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TRandom.h" // For gRandom
#include "TStyle.h"
#include "TString.h"
#include "TMath.h"
#include <iostream>

using namespace std;

// Toy Monte Carlo example.
// Check pull distribution to compare chi2 and binned
// log-likelihood methods.

void pull(int n_toys = 1000,
          int n_tot_entries = 100,
          int nbins = 40,
          bool do_chi2 = true)
{

    TString method_prefix("Log-Likelihood ");
    if (do_chi2)
        method_prefix = "#chi^{2} ";

    // 1. Create Data Histogram (The "Toy")
    // This will be filled and reset thousands of times
    TH1F h4(method_prefix + "h4",
            method_prefix + " Random Gauss",
            nbins, -4, 4);
    h4.SetMarkerStyle(21);
    h4.SetMarkerSize(0.8);
    h4.SetMarkerColor(kRed);

    // 2. Histograms to store results from the thousands of fits
    // 'sigma': distributes the measured widths
    TH1F sigma(method_prefix + "sigma",
               method_prefix + "sigma from gaus fit",
               50, 0.5, 1.5);

    // 'pull': checks if the errors are correct
    // Pull = (Measured - True) / Error
    TH1F pull(method_prefix + "pull",
              method_prefix + "pull from gaus fit",
              50, -4., 4.);

    // 3. Prepare Canvases
    // c0: Animation of the fits happening
    auto c0 = new TCanvas(method_prefix + "Gauss",
                          method_prefix + "Gauss", 0, 0, 320, 240);
    c0->SetGrid();

    // c1: The final result (the Pull distribution)
    auto c1 = new TCanvas(method_prefix + "Result",
                          method_prefix + "Sigma-Distribution",
                          0, 300, 600, 400);

    c0->cd(); // Switch focus to the animation canvas

    float sig, mean;

    // ---------------------------------------------------------
    // THE MC LOOP
    // ---------------------------------------------------------
    for (int i = 0; i < n_toys; i++)
    {
        // A. Reset the experiment
        h4.Reset(); // Clear the histogram bins

        // B. Generate new "Toy" data
        // Fill histogram with 100 random numbers from a standard Gaussian (Mean=0, Sigma=1)
        for (int j = 0; j < n_tot_entries; j++)
            h4.Fill(gRandom->Gaus(0, 1));

        // C. Perform the Fit
        // "q" = Quiet mode (don't print text)
        // "l" = Use Log Likelihood (better for low statistics)
        if (do_chi2)
            h4.Fit("gaus", "q"); // Chi2 fit (Standard)
        else
            h4.Fit("gaus", "lq"); // Log-Likelihood fit

        // D. Animation (Optional)
        // Draw every 100th experiment so the user sees something happening
        if (!(i % 100))
        {
            h4.Draw("ep");
            c0->Update();
        }

        // E. Extractions
        TF1 *fit = h4.GetFunction("gaus");
        sig = fit->GetParameter(2);  // The fitted sigma
        mean = fit->GetParameter(1); // The fitted mean

        sigma.Fill(sig);

        // F. Calculate Pull
        // Since we know the TRUE Mean is 0:
        // Pull = (MeasuredMean - TrueMean) / ErrorOnMean
        // ErrorOnMean approx = Sigma / sqrt(N)
        // So: Pull = Mean / (Sigma / sqrt(N))
        pull.Fill(mean / sig * sqrt(n_tot_entries));

    } // end of toy MC loop

    // 4. Print Result
    c1->cd();
    // Fit the pull distribution with a Gaussian to see if it is compatible with Standard Normal
    pull.Fit("gaus");
    pull.DrawClone();
}

void macro9()
{
    int n_toys = 1000;       // Number of virtual experiments
    int n_tot_entries = 100; // Statistics per experiment (low stats!)
    int n_bins = 40;

    cout << "Performing Pull Experiment with chi2 \n";
    pull(n_toys, n_tot_entries, n_bins, true);

    cout << "Performing Pull Experiment with Log Likelihood\n";
    pull(n_toys, n_tot_entries, n_bins, false);
}