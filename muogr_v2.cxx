#include <stdio.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <set>
#include "TROOT.h"
#include "TFile.h"
#include "TKey.h"
#include "TClass.h"
#include "TH1.h"
#include "TH1F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"

/**
 * fileName18  : path to the 2018 ROOT file (reference)
 * fileName25  : path to the 2025 ROOT file (comparison)
 * histoPath   : internal directory inside each file (e.g. "Muogr")
 *
 * Execution example:
 * [] .x muogr_v2.cxx("AnalyzeEfficiency_321475_325172_RPCMon_2018D.root", "AnalyzeEfficiency_396805_397817_RPCMon2025F.root", "Muogr");
 */

void muogr_v2(const char *fileName18, const char *fileName25, const char *histoPath)
{
  const std::string outDir = "output/";

  TFile *file18 = TFile::Open(fileName18);
  file18->cd();
  gDirectory->cd(histoPath);
  TDirectory *dir18 = gDirectory;
  TIter iter18(dir18->GetListOfKeys());
  TKey *key18;

  TFile *file25 = TFile::Open(fileName25);
  file25->cd();
  gDirectory->cd(histoPath);
  TDirectory *dir25 = gDirectory;

  TCanvas *c18 = new TCanvas("c18", "c18", 558, 409, 900, 600);
  TCanvas *c25 = new TCanvas("c25", "c25", 558, 409, 900, 600);
  TCanvas *crelDif = new TCanvas("crelDif", "crelDif", 558, 409, 900, 600);

  TH1F *hmyAssymetry = new TH1F("hmyAssymetry", "Relative assymetry Eff(2018) vs Eff(2025)", 44, -1.1, 1.1);
  double myMean = 9.;
  double mySigma = 99.;
  double fractionOne = 9.;

  const std::set<std::string> targetChambers = {
      "Muography_W-2_RB1in_S07_Backward",
      "Muography_W-2_RB1in_S07_Forward",
      "Muography_W-2_RB1out_S07_Backward",
      "Muography_W-2_RB1out_S07_Forward",
      "Muography_W-2_RB1in_S09_Backward",
      "Muography_W-2_RB1in_S09_Forward",
      "Muography_W-2_RB1out_S09_Backward",
      "Muography_W-2_RB1out_S09_Forward",
      "Muography_W-2_RB4-_S10_Backward",
      "Muography_W-2_RB4-_S10_Forward"};

  int myCount = 0;
  while ((key18 = (TKey *)iter18.Next()))
  {
    myCount++;

    TClass *cl18 = gROOT->GetClass(key18->GetClassName());
    if (!cl18 || !cl18->InheritsFrom("TH1"))
      continue;

    if (targetChambers.find(key18->GetName()) == targetChambers.end())
      continue;

    TH1 *h18 = (TH1 *)key18->ReadObj();
    std::string outName18 = outDir + std::string(key18->GetName()) + "_2018.png";
    std::replace(outName18.begin(), outName18.end(), '-', 'M');
    std::replace(outName18.begin(), outName18.end(), '+', 'P');
    std::string strforComp18 = std::string(key18->GetName());
    std::replace(strforComp18.begin(), strforComp18.end(), '-', 'M');
    std::replace(strforComp18.begin(), strforComp18.end(), '+', 'P');
    std::cout << outName18.c_str() << std::endl;
    c18->cd();
    gStyle->SetOptStat("nemriou");
    gStyle->SetOptFit(1);
    h18->Draw("colz");
    c18->SaveAs(outName18.c_str());
    int countZeros = 0;

    std::string outName18RelRatio = "(Eff(2018)-Eff(2025))/(Eff(2018)+Eff(2025)) " + std::string(key18->GetName());
    TH1F *myRelDiff1D = new TH1F("myRelDiff1D", outName18RelRatio.c_str(), 201, -2., +2.);

    TH1 *h25 = (TH1 *)dir25->FindObjectAny(key18->GetName());

    if (h25 && gROOT->GetClass(h25->ClassName())->InheritsFrom("TH1"))
    {
      std::string outName25 = outDir + std::string(h25->GetName()) + "_2025.png";
      std::replace(outName25.begin(), outName25.end(), '-', 'M');
      std::replace(outName25.begin(), outName25.end(), '+', 'P');
      std::string strforComp25 = std::string(h25->GetName());
      std::replace(strforComp25.begin(), strforComp25.end(), '-', 'M');
      std::replace(strforComp25.begin(), strforComp25.end(), '+', 'P');

      myMean = 9.;
      mySigma = 99.;
      fractionOne = 9.;
      std::cout << outName25.c_str() << std::endl;
      c25->cd();
      h25->Draw("COLZ");
      c25->SaveAs(outName25.c_str());

      std::string outNameRelRatio1Dpng = outDir + strforComp18 + "_2018vs2025_relDiff1D.png";
      std::string outNameRelRatio1D = outDir + strforComp18 + "_2018vs2025_relDiff1D.C";

      crelDif->cd();
      int xmax = h18->GetXaxis()->GetNbins();
      int ymax = h18->GetYaxis()->GetNbins();
      std::cout << "\nnumber of bins = " << xmax << "*" << ymax << " = " << xmax * ymax << std::endl;
      double rel = 99.;
      double a18 = -1;
      double a25 = -1;

      for (int i = 1; i <= xmax; i++)
      {
        for (int j = 1; j <= ymax; j++)
        {
          a18 = h18->GetBinContent(i, j);
          a25 = h25->GetBinContent(i, j);
          if (a18 == 0 && a25 == 0)
          {
            countZeros++;
            myRelDiff1D->Fill(-2);
          }
          else
          {
            rel = (a18 - a25) / (a18 + a25);
            myRelDiff1D->Fill(rel);
          }
        }
      }

      gStyle->SetOptStat("eiou");
      gStyle->SetOptFit(1);
      myRelDiff1D->SetFillColor(kBlue + 1);
      myRelDiff1D->Draw();

      int findFitMin = myRelDiff1D->GetXaxis()->FindBin(-0.15);
      int findFitMax = myRelDiff1D->GetXaxis()->FindBin(0.15);

      if (myRelDiff1D->Integral(findFitMin, findFitMax) == 0)
      {
        std::cout << "at least one of the muography is empty and will not fit" << std::endl;
      }
      else
      {
        TF1 *funcG = new TF1("funcG", "gaus", -0.15, 0.15);
        funcG->SetLineColor(kRed + 1);
        funcG->SetLineWidth(3);
        myRelDiff1D->Fit("funcG", "Lre");
        crelDif->SaveAs(outNameRelRatio1D.c_str());
        crelDif->SaveAs(outNameRelRatio1Dpng.c_str());
        myMean = funcG->GetParameter(1);
        mySigma = funcG->GetParameter(2);
        std::cout << std::fixed << std::setprecision(5);
        std::cout << "myMean " << myMean << std::endl;
        std::cout << "mySigma " << mySigma << std::endl;
        delete funcG;
      }

      int findOne = myRelDiff1D->GetXaxis()->FindBin(1.);
      int findMOne = myRelDiff1D->GetXaxis()->FindBin(-1.);
      int findZero = myRelDiff1D->GetXaxis()->FindBin(0.);

      int entriesAtOne = myRelDiff1D->GetBinContent(findOne);
      int entriesAtMOne = myRelDiff1D->GetBinContent(findMOne);
      int entriesAtZero = myRelDiff1D->GetBinContent(findZero);

      double myIntegralNegative = myRelDiff1D->Integral(findMOne, findZero - 1);
      double myIntegralpositive = myRelDiff1D->Integral(findZero + 1, findOne);

      if ((myIntegralpositive + myIntegralNegative) > 0)
      {
        double myAssimetry = (myIntegralpositive - myIntegralNegative) / (myIntegralpositive + myIntegralNegative);
        hmyAssymetry->Fill(myAssimetry);
        int allEntries = myRelDiff1D->Integral();
        fractionOne = (entriesAtOne * 100.) / (allEntries * 1.);
        std::cout << std::fixed << std::setprecision(5);
        std::cout << "myAssimetry " << myAssimetry << std::endl;
        std::cout << "fraction at one " << fractionOne << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
      }
    }

    delete h18;
    delete myRelDiff1D;
  }

  TCanvas *crelAssym = new TCanvas("crelAssym", "crelAssym", 558, 409, 900, 600);
  crelAssym->cd();
  gStyle->SetOptStat("emriou");
  hmyAssymetry->SetFillColor(kBlue + 1);
  hmyAssymetry->Draw();
  crelAssym->SaveAs((outDir + "summary_relAssym_2018vs2025_WM2_S07_S09_S10.png").c_str());
  crelAssym->SaveAs((outDir + "summary_relAssym_2018vs2025_WM2_S07_S09_S10.C").c_str());

  file18->Close();
  file25->Close();

  delete c18;
  delete c25;
  delete crelDif;
  delete crelAssym;
}
