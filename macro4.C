#include "TCanvas.h"
#include "TGraph2DErrors.h"
#include "TF2.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TH1.h" // Needed for the projections which return TH1D*

// Create, Draw and fit a TGraph2DErrors
void macro4()
{
    gStyle->SetPalette(kBird);
    const double e = 0.3;
    const int nd = 500;

    TRandom3 my_random_generator;

    // Define the 2D function: 1000 * (sin(cx)/x * sin(cy)/y) + offset
    // Parameters [0] and [1] control the frequency/scaling in x and y
    TF2 f2("f2",
           "1000*(([0]*sin(x)/x)*([1]*sin(y)/y))+200",
           -6, 6, -6, 6);
    f2.SetParameters(1, 1);

    TGraph2DErrors dte(nd);

    // Fill the 2D graph with noisy data based on the function
    double rnd, x, y, z, ex, ey, ez;
    for (Int_t i = 0; i < nd; i++)
    {
        // Get random x,y coordinates distributed according to f2
        f2.GetRandom2(x, y);

        // Add random noise to z
        // A random number in [-e, e]
        rnd = my_random_generator.Uniform(-e, e);
        z = f2.Eval(x, y) * (1 + rnd);

        dte.SetPoint(i, x, y, z);

        // Define errors
        ex = 0.05 * my_random_generator.Uniform();
        ey = 0.05 * my_random_generator.Uniform();
        ez = fabs(z * rnd);

        dte.SetPointError(i, ex, ey, ez);
    }

    // Fit function to generated data
    // We perturb the initial parameters to see if the fit recovers them (1,1)
    f2.SetParameters(0.7, 1.5);
    f2.SetTitle("Fitted 2D function");
    dte.Fit(&f2);

    // Plot the result
    auto c1 = new TCanvas("c1", "3D Fit", 800, 600);
    f2.SetLineWidth(1);
    f2.SetLineColor(kBlue - 5);

    // Draw the function surface
    TF2 *f2c = (TF2 *)f2.DrawClone("Surf1");

    // Styling axes
    TAxis *Xaxis = f2c->GetXaxis();
    TAxis *Yaxis = f2c->GetYaxis();
    TAxis *Zaxis = f2c->GetZaxis();
    Xaxis->SetTitle("X Title");
    Xaxis->SetTitleOffset(1.5);
    Yaxis->SetTitle("Y Title");
    Yaxis->SetTitleOffset(1.5);
    Zaxis->SetTitle("Z Title");
    Zaxis->SetTitleOffset(1.5);

    // Draw the data points on top
    dte.DrawClone("P0 Same");

    // Make the x and y projections
    auto c_p = new TCanvas("ProjCan", "The Projections", 1000, 400);
    c_p->Divide(2, 1);

    c_p->cd(1);
    // Project points onto X axis and draw
    dte.Project("x")->Draw();

    c_p->cd(2);
    // Project points onto Y axis and draw
    dte.Project("y")->Draw();
}