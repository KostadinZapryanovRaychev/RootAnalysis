#include "TAttLine.h"
#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TLatex.h"
#include <iostream>

using namespace std;

// Helper function to format lines (color, style, width)
void format_line(TAttLine *line, int col, int sty)
{
    line->SetLineWidth(5);
    line->SetLineColor(col);
    line->SetLineStyle(sty);
}

// Custom function: Gaussian + Parabola (Signal + Background)
// vars[0] is x
// pars[0]: Norm, pars[1]: Mean, pars[2]: Sigma (Gaussian)
// pars[3]: const, pars[4]: linear, pars[5]: quadratic (Parabola)
double the_gausppar(double *vars, double *pars)
{
    return pars[0] * TMath::Gaus(vars[0], pars[1], pars[2]) +
           pars[3] + pars[4] * vars[0] + pars[5] * vars[0] * vars[0];
}

int macro8()
{
    // Global Style Settings
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111); // Show probability, chi2, errors, parameters
    gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(.89);
    gStyle->SetStatY(.89);

    // 1. Define the component functions (for later drawing)
    TF1 parabola("parabola", "[0]+[1]*x+[2]*x**2", 0, 20);
    format_line(&parabola, kBlue, 2);

    TF1 gaussian("gaussian", "[0]*TMath::Gaus(x,[1],[2])", 0, 20);
    format_line(&gaussian, kRed, 2);

    // 2. Define the Combined Function
    // We use the C++ function 'the_gausppar' defined above
    TF1 gausppar("gausppar", the_gausppar, 0, 20, 6);

    // Set "True" parameters to generate data
    double a = 15;
    double b = -1.2;
    double c = .03;
    double norm = 4;
    double mean = 7;
    double sigma = 1;
    gausppar.SetParameters(norm, mean, sigma, a, b, c);
    gausppar.SetParNames("Norm", "Mean", "Sigma", "a", "b", "c");
    format_line(&gausppar, kBlue, 1);

    // 3. Create Data (Histogram)
    TH1F histo("histo", "Signal plus background;X vals;Y Vals", 50, 0, 20);
    histo.SetMarkerStyle(8);

    // Fake the data
    for (int i = 1; i <= 5000; ++i)
    {
        histo.Fill(gausppar.GetRandom());
    }

    // 4. Prepare for Fitting
    // Reset parameters to "initial guesses" so the fit has actual work to do
    // Guess: Peak at 6, Area approx 50
    gausppar.SetParameter(0, 50);
    gausppar.SetParameter(1, 6);

    // Set other parameters to 1 as a rough start
    int npar = gausppar.GetNpar();
    for (int ipar = 2; ipar < npar; ++ipar)
    {
        gausppar.SetParameter(ipar, 1);
    }

    // 5. Perform Fit
    auto c1 = new TCanvas(); // Create a canvas to see the result
    // "S" option is important: it ensures the Fit method returns a TFitResultPtr
    auto fitResPtr = histo.Fit(&gausppar, "S");

    // 6. Retrieve and Print Results
    fitResPtr->Print(); // print fit results to console

    // Get Covariance Matrix and print it
    TMatrixDSym covMatrix(fitResPtr->GetCovarianceMatrix());
    covMatrix.Print();

    // 7. Visualizing the Components
    // Now that we have the "Best Fit" parameters in 'gausppar',
    // we copy them back to the separate 'gaussian' and 'parabola' functions
    // so we can draw them overlaid.
    for (int ipar = 0; ipar < 3; ipar++)
    {
        gaussian.SetParameter(ipar, gausppar.GetParameter(ipar));
        parabola.SetParameter(ipar, gausppar.GetParameter(ipar + 3));
    }

    histo.GetYaxis()->SetRangeUser(0, 250);
    histo.DrawClone("PE");
    parabola.DrawClone("Same");
    gaussian.DrawClone("Same");

    TLatex latex(2, 220, "#splitline{Signal Peak over}{background}");
    latex.DrawClone("Same");

    return 0;
}