#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TF2.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TStyle.h"
#include <iostream>

void macro7()
{
    gStyle->SetPalette(kBird);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    auto bidi_h = new TH2F("bidi_h", "2D Histo;Gaussian Vals;Exp. Vals",
                           30, -5, 5,  // X axis
                           30, 0, 10); // Y axis
    TRandom3 rgen;
    for (int i = 0; i < 500000; i++)
    {
        bidi_h->Fill(rgen.Gaus(0, 2), 10 - rgen.Exp(4), .1);
    }
    auto c = new TCanvas("Canvas", "Canvas", 800, 800);
    c->Divide(2, 2);
    c->cd(1);
    bidi_h->Draw("Cont1");
    c->cd(2);
    bidi_h->Draw("Colz");
    c->cd(3);
    bidi_h->Draw("Lego2");
    c->cd(4);
    bidi_h->Draw("Surf3");
    // Profiles and Projections
    auto c2 = new TCanvas("Canvas2", "Canvas2", 800, 800);
    c2->Divide(2, 2);
    c2->cd(1);
    bidi_h->ProjectionX()->Draw();
    c2->cd(2);
    bidi_h->ProjectionY()->Draw();
    c2->cd(3);
    bidi_h->ProfileX()->Draw();
    c2->cd(4);
    bidi_h->ProfileY()->Draw();
}

void hstack()
{
    auto a = new THStack("a", "Stacked 2D histograms");

    auto f1 = new TF2("f1", "xygaus + xygaus(5) + xylandau(10)", -4, 4, -4, 4);
    Double_t params1[] = {130, -1.4, 1.8, 1.5, 1, 150, 2, 0.5, -2, 0.5, 3600, -2, 0.7, -3, 0.3};
    f1->SetParameters(params1);

    auto h2sta = new TH2F("h2sta", "h2sta", 20, -4, 4, 20, -4, 4);
    h2sta->SetFillColor(kBlue - 9); // Color 38 in ROOT is approx kBlue-9
    h2sta->FillRandom("f1", 4000);

    auto f2 = new TF2("f2", "xygaus + xygaus(5)", -4, 4, -4, 4);
    Double_t params2[] = {100, -1.4, 1.9, 1.1, 2, 80, 2, 0.7, -2, 0.5};
    f2->SetParameters(params2);

    auto h2stb = new TH2F("h2stb", "h2stb", 20, -4, 4, 20, -4, 4);
    h2stb->SetFillColor(kRed - 3); // Color 46 in ROOT is approx kRed-3
    h2stb->FillRandom("f2", 3000);

    a->Add(h2sta);
    a->Add(h2stb);

    // Create a new canvas for this stack so it doesn't overwrite the others
    auto c_stack = new TCanvas("c_stack", "Stacked 2D Histograms", 600, 500);
    a->Draw();
}