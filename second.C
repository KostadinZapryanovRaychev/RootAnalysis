#include "TH1F.h"
#include "TRandom.h"
#include "TCanvas.h"

void second()
{
    // Създаваме canvas
    TCanvas *c = new TCanvas("c2", "My second canvas", 800, 600);

    // Създаваме хистограма: 50 бинa, от -5 до 5
    TH1F *h2 = new TH1F("h2", "Sinus & Gaussian; x; Entries", 50, -5, 5);

    // Попълваме хистограмата с комбинация от случайна гаусова и синус
    TRandom rand;
    for (int i = 0; i < 10000; i++)
    {
        double x = rand.Gaus(0, 1); // Gaussian
        h2->Fill(x + sin(x));
    }

    // Рисуваме
    h2->Draw();
}
