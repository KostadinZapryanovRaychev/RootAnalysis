#include "TH1F.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include <iostream>
#include <vector>

using namespace std;

// Create, Fill and draw an Histogram which reproduces the
// counts of a scaler linked to a Geiger counter.
void macro5()
{
    // 1. Create the Histogram
    // We use heap allocation so it survives the function scope
    auto cnt_r_h = new TH1F("count_rate",
                            "Count Rate;N_{Counts};# occurencies",
                            100,   // Number of Bins
                            -0.5,  // Lower X Boundary
                            15.5); // Upper X Boundary

    // 2. Simulate Measurements
    auto mean_count = 3.6f;
    TRandom3 rndgen;

    // Create a variable (vector) to hold the data first
    vector<int> measurements;
    measurements.reserve(400); // Optimization: reserve memory

    // Generate the data and store it
    for (int imeas = 0; imeas < 400; imeas++)
    {
        measurements.push_back(rndgen.Poisson(mean_count));
    }

    // Now provide the data to the plot (Fill the histogram)
    for (int value : measurements)
    {
        cnt_r_h->Fill(value);
    }

    // 3. Draw Raw Counts
    auto c = new TCanvas("c_raw", "Raw Counts");
    cnt_r_h->Draw();

    // 4. Draw Normalized Distribution
    auto c_norm = new TCanvas("c_norm", "Normalized Distribution");
    cnt_r_h->DrawNormalized();

    // 5. Print Summary Statistics
    cout << "Moments of Distribution:\n"
         << " - Mean     = " << cnt_r_h->GetMean() << " +- " << cnt_r_h->GetMeanError() << "\n"
         << " - Std Dev  = " << cnt_r_h->GetStdDev() << " +- " << cnt_r_h->GetStdDevError() << "\n"
         << " - Skewness = " << cnt_r_h->GetSkewness() << "\n"
         << " - Kurtosis = " << cnt_r_h->GetKurtosis() << "\n";
}