#include "TMVA/Reader.h"
#include <iostream>

void apply_bdt()
{
    TMVA::Reader *reader = new TMVA::Reader("!Color:!Silent");

    float x;
    reader->AddVariable("x", &x);

    reader->BookMVA("BDT", "dataset/weights/TestBDT_BDT.weights.xml");

    float testValues[] = {1.1, 2.0, 2.7, 3.2};

    for (int i = 0; i < 4; i++)
    {
        x = testValues[i];

        double score = reader->EvaluateMVA("BDT");

        std::cout << "x = " << x << " --> BDT score = " << score;

        if (score > 0)
            std::cout << " (Signal)";
        else
            std::cout << " (Background)";

        std::cout << std::endl;
    }
}