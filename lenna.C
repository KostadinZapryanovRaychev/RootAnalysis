#include "TMath.h"
#include "TF1.h"
#include "TCanvas.h"
#include <iostream>
#include <cmath>

using namespace std;

// Define pi
auto pi = TMath::Pi();

double single(double *x, double *par)
{
    // Avoid division by zero
    if (par[0] * x[0] == 0)
        return 1.0;
    return pow(sin(pi * par[0] * x[0]) / (pi * par[0] * x[0]), 2);
}

double nslit0(double *x, double *par)
{
    // Avoid division by zero when sin(pi*x) is close to 0 (at integer values of x)
    if (abs(sin(pi * x[0])) < 1e-9)
        return pow(par[1], 2);
    return pow(sin(pi * par[1] * x[0]) / sin(pi * x[0]), 2);
}

double nslit(double *x, double *par)
{
    return single(x, par) * nslit0(x, par);
}

// This is the main program matching the filename lenna.C
void lenna()
{
    // Create a canvas explicitly to ensure the window opens
    TCanvas *c1 = new TCanvas("c1", "Interference Pattern", 800, 600);

    float r, ns;

    // request user input
    cout << "slit width / g ? ";
    cin >> r; // Using cin instead of scanf for C++ style, but scanf works too
    cout << "# of slits? ";
    cin >> ns;

    cout << "interference pattern for " << ns
         << " slits, width/distance: " << r << endl;

    // define function and set options
    // func definitions: name, c-function, min, max, nparams
    TF1 *Fnslit = new TF1("Fnslit", nslit, -5.001, 5., 2);
    Fnslit->SetNpx(500);

    // set parameters, as read in above
    Fnslit->SetParameter(0, r);
    Fnslit->SetParameter(1, ns);

    // draw the interference pattern for a grid with n slits
    Fnslit->Draw();

    // Update the canvas line
    c1->Update();
}

// slit width / g: 0.2
//  # of slits: 4 or 5
// for good patterns and results