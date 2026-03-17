#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>

void bdt()
{
    TMVA::Tools::Instance();

    TFile *outputFile = TFile::Open("myoutput.root", "RECREATE");

    TMVA::Factory *factory = new TMVA::Factory("TestBDT", outputFile);
    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

    dataloader->AddVariable("x", 'F');

    const int nEvents = 20;
    float signalX[nEvents] = {1.0, 2.0, 1.5, 2.5, 1.2};
    float backgroundX[nEvents] = {3.0, 2.8, 3.5, 2.9, 3.2};

    TTree *sigTree = new TTree("sigTree", "Signal Tree");
    TTree *bkgTree = new TTree("bkgTree", "Background Tree");

    float x;
    sigTree->Branch("x", &x, "x/F");
    for (int i = 0; i < nEvents; i++)
    {
        x = signalX[i];
        sigTree->Fill();
    }
    bkgTree->Branch("x", &x, "x/F");
    for (int i = 0; i < nEvents; i++)
    {
        x = backgroundX[i];
        bkgTree->Fill();
    }

    dataloader->AddSignalTree(sigTree, 1.0);
    dataloader->AddBackgroundTree(bkgTree, 1.0);

    dataloader->PrepareTrainingAndTestTree("", "SplitMode=Random:NormMode=NumEvents:!V");

    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
                        "!H:!V:NTrees=2:MaxDepth=1");
    std::cout << "BDT method booked successfully!" << std::endl;
    std::cout << "Dummy data added and prepared for training." << std::endl;

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();
    std::cout << "BDT training and evaluation completed!" << std::endl;

    outputFile->Close();
}