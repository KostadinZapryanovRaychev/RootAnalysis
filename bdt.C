#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include <vector>

/**
 * BDT training macro — all data comes from external files
 *
 * signalFile    : path to plain text file with signal data (one value per line)
 * backgroundFile: path to plain text file with background data (one value per line)
 * outputFile    : path to output ROOT file to save the trained BDT
 *
 * Execution example:
 * [] .x bdt.C("signal.txt", "background.txt", "myoutput.root")
 */

void bdt(const char *signalFile = "signal.txt",
         const char *backgroundFile = "background.txt",
         const char *outputFile = "myoutput.root")
{
    TMVA::Tools::Instance();

    if (gSystem->AccessPathName(signalFile))
    {
        std::cerr << "ERROR: signal file not found: " << signalFile << std::endl;
        return;
    }
    if (gSystem->AccessPathName(backgroundFile))
    {
        std::cerr << "ERROR: background file not found: " << backgroundFile << std::endl;
        return;
    }

    // --- load signal data from file ---
    std::vector<float> signalX;
    std::ifstream sigStream(signalFile);
    float val;
    while (sigStream >> val)
    {
        signalX.push_back(val);
    }
    sigStream.close();

    if (signalX.empty())
    {
        std::cerr << "ERROR: no signal data loaded from " << signalFile << std::endl;
        return;
    }

    // --- load background data from file ---
    std::vector<float> backgroundX;
    std::ifstream bkgStream(backgroundFile);
    while (bkgStream >> val)
    {
        backgroundX.push_back(val);
    }
    bkgStream.close();

    if (backgroundX.empty())
    {
        std::cerr << "ERROR: no background data loaded from " << backgroundFile << std::endl;
        return;
    }

    std::cout << "\n  Signal file       : " << signalFile << std::endl;
    std::cout << "  Signal events     : " << signalX.size() << std::endl;
    std::cout << "  Background file   : " << backgroundFile << std::endl;
    std::cout << "  Background events : " << backgroundX.size() << std::endl;
    std::cout << "  Output file       : " << outputFile << "\n"
              << std::endl;

    // --- extract output directory and create if it does not exist ---
    std::string outputPath(outputFile);
    size_t lastSlash = outputPath.find_last_of("/");
    if (lastSlash != std::string::npos)
    {
        std::string outputDir = outputPath.substr(0, lastSlash);
        if (gSystem->AccessPathName(outputDir.c_str()))
            gSystem->mkdir(outputDir.c_str(), kTRUE);
    }

    TFile *outFile = TFile::Open(outputFile, "RECREATE");
    if (!outFile || outFile->IsZombie())
    {
        std::cerr << "ERROR: could not create output file: " << outputFile << std::endl;
        return;
    }

    TMVA::Factory *factory = new TMVA::Factory("TestBDT", outFile);
    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

    dataloader->AddVariable("x", 'F');

    // --- populate signal tree ---
    TTree *sigTree = new TTree("sigTree", "Signal Tree");
    float x;
    sigTree->Branch("x", &x, "x/F");
    for (size_t i = 0; i < signalX.size(); i++)
    {
        x = signalX[i];
        sigTree->Fill();
    }

    // --- populate background tree ---
    TTree *bkgTree = new TTree("bkgTree", "Background Tree");
    bkgTree->Branch("x", &x, "x/F");
    for (size_t i = 0; i < backgroundX.size(); i++)
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

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();
    std::cout << "BDT training and evaluation completed!" << std::endl;

    outFile->Close();
    delete factory;
    delete dataloader;
}