#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"

void macro2()
{
    // 1. Create a Canvas
    // We allocate it on the heap ('new') so it stays open after the macro finishes.
    // SetGrid() adds a grid to the background for easier reading of values.
    auto c = new TCanvas("c2", "Graph from File Example", 800, 600);
    c->SetGrid();

    // 2. Define the "Expected" Graph (Theory/Prediction)
    // We read 3 columns: X, Y, and ErrorY from the file.
    // The format "%lg %lg %lg" means: "Long Float, Long Float, Long Float".
    // 1st %lg -> X
    // 2nd %lg -> Y
    // 3rd %lg -> Error on Y (used for the band width)
    TGraphErrors graph_expected("./macro2_input_expected.txt", "%lg %lg %lg");

    graph_expected.SetTitle("Measurement XYZ and Expectation;length [cm];Arb.Units");
    graph_expected.SetFillColor(kYellow); // The band will be yellow

    // DrawClone "E3AL"
    // A = Axis (draw axes)
    // L = Line (draw a simple line through points)
    // E3 = Draw a fill area (band) through the end points of the vertical error bars
    graph_expected.DrawClone("E3AL");

    // 3. Define the "Measured" Graph (Experiment)
    TGraphErrors graph("./macro2_input.txt", "%lg %lg %lg");
    graph.SetMarkerStyle(kCircle); // Use a circle marker
    graph.SetFillColor(0);         // No fill

    // DrawClone "PESame"
    // P = Points (draw markers)
    // E = Errors (draw simple error bars)
    // Same = Don't erase the previous plot, draw on top of it
    graph.DrawClone("PESame");

    // 4. Draw the Legend
    // Coordinates (x1, y1, x2, y2) are normalized (0 to 1)
    TLegend leg(.1, .7, .3, .9, "Lab. Lesson 2");
    leg.SetFillColor(0); // White background
    leg.AddEntry(&graph_expected, "Expected Points");
    leg.AddEntry(&graph, "Measured Points");
    leg.DrawClone("Same");

    // 5. Print graph contents to the terminal
    // This dumps the numerical values of the points to the console
    graph.Print();
}