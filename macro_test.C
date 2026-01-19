#include "TCanvas.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"

void macro_test()
{
    const int n_points = 10;
    double x_vals[n_points] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double y_vals[n_points] = {6, 12, 14, 20, 22, 24, 35, 45, 44, 53};
    double y_errs[n_points] = {5, 5, 4.7, 4.5, 4.2, 5.1, 2.9, 4.1, 4.8, 5.43};

    TGraphErrors graph(n_points, x_vals, y_vals, nullptr, y_errs);
    graph.SetTitle("Measurement XYZ;length [cm];Arb.Units");

    graph.SetMarkerStyle(kOpenCircle);
    graph.SetMarkerColor(kBlue);
    graph.SetLineColor(kBlue);

    auto mycanvas = new TCanvas("c1", "Graph Example", 800, 600);

    graph.DrawClone("APE");

    TF1 f("Linear law", "[0]+x*[1]", .5, 10.5);
    f.SetLineColor(kRed);
    f.SetLineStyle(2);

    graph.Fit(&f);

    f.DrawClone("Same");

    TLegend leg(.1, .7, .3, .9, "Lab. Lesson 1");
    leg.SetFillColor(0);
    leg.AddEntry(&graph, "Exp. Points");
    leg.AddEntry(&f, "Th. Law");
    leg.DrawClone("Same");

    TArrow arrow(8, 8, 6.2, 23, 0.02, "|>");
    arrow.SetLineWidth(2);
    arrow.DrawClone();

    TLatex text(8.2, 7.5, "#splitline{Maximum}{Deviation}");
    text.DrawClone();

    mycanvas->Print("graph_with_law.pdf");
}