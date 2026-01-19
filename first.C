#include "TH1F.h"

void first()
{
    TH1F h("h", "My first ROOT histogram; x; Entries", 100, -4, 4);
    h.FillRandom("gaus", 10000);
    h.Draw();
}

// root for start
// .x first.C
// .q for quit
// the ROOT header files. If you want adaptive code of this type, use the ROOT type declarations

// TODO add this in each code as good practice
// gROOT->SetStyle("Plain"); // set plain TStyle
// gStyle->SetOptStat(111111); // draw statistics on plots,
// (0) for no output
// gStyle->SetOptFit(1111); // draw fit results on plot,
// (0) for no ouput
// gStyle->SetPalette(57); // set color map
// gStyle->SetOptTitle(0); // suppress title box
// TCanvas c1("c1", "<Title>", 0, 0, 400, 300);
// create a canvas, specify position and size in pixels
// c1.Divide(2, 2);
// set subdivisions, called pads
// c1.cd(1);
// change to pad 1 of canvas c1