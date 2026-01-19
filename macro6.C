#include "TH1F.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TStyle.h"
#include <iostream>

// Helper function to format histograms
void format_h(TH1F *h, int linecolor)
{
    h->SetLineWidth(3);
    h->SetLineColor(linecolor);
}

void macro6()
{
    // 1. Create Histograms
    auto sig_h = new TH1F("sig_h", "Signal Histo", 50, 0, 10);
    auto gaus_h1 = new TH1F("gaus_h1", "Gauss Histo 1", 30, 0, 10);
    auto gaus_h2 = new TH1F("gaus_h2", "Gauss Histo 2", 30, 0, 10);
    auto bkg_h = new TH1F("exp_h", "Exponential Histo", 50, 0, 10);

    // 2. Simulate the measurements
    TRandom3 rndgen;
    for (int imeas = 0; imeas < 4000; imeas++)
    {
        bkg_h->Fill(rndgen.Exp(4));

        // Add signals only some of the time to vary statistics
        if (imeas % 4 == 0)
            gaus_h1->Fill(rndgen.Gaus(5, 2));
        if (imeas % 4 == 0)
            gaus_h2->Fill(rndgen.Gaus(5, 2));
        if (imeas % 10 == 0)
            sig_h->Fill(rndgen.Gaus(5, .5));
    }

    // 3. Format Histograms using helper function
    int i = 0;
    // Using initializer list to loop over pointers
    for (auto hist : {sig_h, bkg_h, gaus_h1, gaus_h2})
    {
        format_h(hist, 1 + i++);
    }

    // ---------------------------------------------------------
    // Operation 1: SUM (Background + Signal)
    // ---------------------------------------------------------
    // Create a copy of background to start with
    auto sum_h = new TH1F(*bkg_h);
    // Add signal with weight 1.0
    sum_h->Add(sig_h, 1.);
    sum_h->SetTitle("Exponential + Gaussian;X variable;Y variable");
    format_h(sum_h, kBlue);

    // Draw Sum
    auto c_sum = new TCanvas("c_sum", "Sum Calculation");
    sum_h->Draw("hist");     // Draw the sum
    bkg_h->Draw("SameHist"); // Draw components on top for comparison
    sig_h->Draw("SameHist");

    // ---------------------------------------------------------
    // Operation 2: DIVISION (Ratio of two similar distributions)
    // ---------------------------------------------------------
    auto dividend = new TH1F(*gaus_h1);
    dividend->Divide(gaus_h2);

    // Graphical Maquillage (Makeup/Styling)
    dividend->SetTitle(";X axis;Gaus Histo 1 / Gaus Histo 2");
    format_h(dividend, kOrange);
    gaus_h1->SetTitle(";;Gaus Histo 1 and Gaus Histo 2");
    gStyle->SetOptStat(0); // Turn off the statistics box

    // Create a canvas divided into two pads (stacked vertically)
    TCanvas *c_divide = new TCanvas("c_divide", "Division Calculation");
    c_divide->Divide(1, 2, 0, 0); // 1 column, 2 rows, 0 margin between them

    // Top Pad: The distributions
    c_divide->cd(1);
    c_divide->GetPad(1)->SetRightMargin(.01);
    gaus_h1->DrawNormalized("Hist");
    gaus_h2->DrawNormalized("HistSame");

    // Bottom Pad: The Ratio
    c_divide->cd(2);
    dividend->GetYaxis()->SetRangeUser(0, 2.49);
    c_divide->GetPad(2)->SetGridy(); // Grid only on Y
    c_divide->GetPad(2)->SetRightMargin(.01);
    dividend->Draw();
}