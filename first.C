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