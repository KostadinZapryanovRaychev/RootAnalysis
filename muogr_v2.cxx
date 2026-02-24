#include <stdio.h>
#include <string>
#include <algorithm>
#include <iostream>
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
 * filename is the name of the root file
 * histopath is the path within the root document to the directory, containing
 *            all the histograms
 *
 * Execution example:
 * [] .x muogr_v2.cxx("AnalyzeEfficiency_321475_325172_RPCMon_2018D.root", "AnalyzeEfficiency_Aug_16Sept_2024G_RPCMon2024.root", "Muogr");
 */

void muogr_v2(const char *fileName18, const char *fileName24, const char *histoPath)
{
  // BUG 1 FIX: open file18 first, then file24. The original code opened file24,
  // created iter24, then immediately invalidated it by switching gDirectory to
  // file18. The top-level iter24 was dead code. Now each file just gets its
  // TDirectory pointer and we never rely on gDirectory staying put.
  TFile *file18 = TFile::Open(fileName18);
  file18->cd();
  gDirectory->cd(histoPath);
  TDirectory *dir18 = gDirectory;
  TIter iter18(dir18->GetListOfKeys());
  TKey *key18;

  // BUG 1 FIX (continued): file24 is opened after file18 and we only keep its
  // TDirectory pointer. No top-level iter24 is created here.
  TFile *file24 = TFile::Open(fileName24);
  file24->cd();
  gDirectory->cd(histoPath);
  TDirectory *dir24 = gDirectory;

  TCanvas *c18 = new TCanvas("c18", "c18", 558, 409, 900, 600);
  TCanvas *c24 = new TCanvas("c24", "c24", 558, 409, 900, 600);
  TCanvas *crelDif = new TCanvas("crelDif", "crelDif", 558, 409, 900, 600);

  TH1F *hmyAssymetry = new TH1F("hmyAssymetry", "Relative assymetry Eff(2018) vs Eff(2024)", 44, -1.1, 1.1);
  double myMean = 9.;
  double mySigma = 99.;
  double fractionOne = 9.;

  int myCount = 0;
  while ((key18 = (TKey *)iter18.Next()))
  {
    myCount++;

    // BUG 4 FIX: the InheritsFrom("TH1") guard was commented out, meaning any
    // non-histogram object in the directory would be blindly cast to TH1* and
    // crash. Now we always check before reading.
    TClass *cl18 = gROOT->GetClass(key18->GetClassName());
    if (!cl18 || !cl18->InheritsFrom("TH1"))
      continue;

    // BUG 3 FIX: the original code had a hardcoded substr(0,22) filter that
    // silently skipped every chamber except "Muography_W-2_RB4-_S10", making
    // the macro process only one chamber unless the developer edited the string
    // and the magic number 22 by hand. Removed entirely — all chambers are now
    // processed.

    TH1 *h18 = (TH1 *)key18->ReadObj();
    std::string outName18 = std::string(key18->GetName()) + "_18.png";
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

    std::string outName18RelRatio = "(Eff(2018)-Eff(2024))/(Eff(2018)+Eff(2024)) " + std::string(key18->GetName());
    TH1F *myRelDiff1D = new TH1F("myRelDiff1D", outName18RelRatio.c_str(), 201, -2., +2.);

    // BUG 2 FIX: the original code rebuilt iter24 from scratch inside the outer
    // loop, scanning all ~2748 keys of the 2024 directory for every single 2018
    // chamber — O(N²), ~1 hour runtime. Replaced with a direct O(1) lookup
    // using FindObjectAny() on the saved dir24 pointer.
    TH1 *h24 = (TH1 *)dir24->FindObjectAny(key18->GetName());

    if (h24 && gROOT->GetClass(h24->ClassName())->InheritsFrom("TH1"))
    {
      // BUG 8 FIX: outName24 was built from key18->GetName() instead of the
      // matched 2024 object name. Now uses h24->GetName() which is correct.
      std::string outName24 = std::string(h24->GetName()) + "_24.png";
      std::replace(outName24.begin(), outName24.end(), '-', 'M');
      std::replace(outName24.begin(), outName24.end(), '+', 'P');
      std::string strforComp24 = std::string(h24->GetName());
      std::replace(strforComp24.begin(), strforComp24.end(), '-', 'M');
      std::replace(strforComp24.begin(), strforComp24.end(), '+', 'P');

      myMean = 9.;
      mySigma = 99.;
      fractionOne = 9.;
      std::cout << outName24.c_str() << std::endl;
      c24->cd();
      h24->Draw("COLZ");
      c24->SaveAs(outName24.c_str());

      std::string outNameRelRatio1Dpng = strforComp18 + "_relDiff1D.png";
      std::string outNameRelRatio1D = strforComp18 + "_relDiff1D.C";

      crelDif->cd();
      int xmax = h18->GetXaxis()->GetNbins();
      int ymax = h18->GetYaxis()->GetNbins();
      std::cout << "\n"
                << "number of bins = " << xmax << "*" << ymax << " = " << xmax * ymax << std::endl;
      double rel = 99.;
      double a18 = -1;
      double a24 = -1;

      for (int i = 1; i <= xmax; i++)
      {
        for (int j = 1; j <= ymax; j++)
        {
          a18 = h18->GetBinContent(i, j);
          a24 = h24->GetBinContent(i, j);
          if (a18 == 0 && a24 == 0)
          {
            countZeros++;
            myRelDiff1D->Fill(-2);
          }
          else
          {
            rel = (a18 - a24) / (a18 + a24);
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

      // BUG 5 FIX: the original code computed myAssimetry unconditionally,
      // even when both integrals were zero (chamber off in both years), causing
      // a 0/0 = NaN which silently corrupted the summary histogram. Now guarded.
      if ((myIntegralpositive + myIntegralNegative) > 0)
      {
        double myAssimetry = (myIntegralpositive - myIntegralNegative) / (myIntegralpositive + myIntegralNegative);
        hmyAssymetry->Fill(myAssimetry);
        int allEntries = myRelDiff1D->Integral();
        fractionOne = (entriesAtOne * 100.) / (allEntries * 1.);
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
  crelAssym->SaveAs("summary_relAssym_Muography_WM2_RB1.png");
  crelAssym->SaveAs("summary_relAssym_Muography_WM2_RB1.C");

  // BUG 6 FIX: the original code called delete on cl18 (a ROOT-owned TClass
  // singleton) and key18 (owned by the TDirectory). Both caused heap corruption.
  // Removed — we must not delete ROOT-owned objects.

  // BUG 7 FIX: the original code called both Close() and delete on the same
  // TFile pointer, causing a double-free. Close() is sufficient — it triggers
  // ROOT's internal cleanup. The explicit delete calls are removed.
  file18->Close();
  file24->Close();

  delete c18;
  delete c24;
  delete crelDif;
  delete crelAssym;
  delete hmyAssymetry;
}
