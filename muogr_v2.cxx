#include <stdio.h>
#include "TROOT.h"
#include "TFile.h"
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

void muogr_v2(const char* fileName18, const char* fileName24, const char* histoPath)
{
  TFile *file24 = TFile::Open(fileName24);
  file24->cd();
  gDirectory->cd(histoPath);
  TIter iter24(gDirectory->GetListOfKeys());
  

  TFile *file18 = TFile::Open(fileName18);
  file18->cd();
  gDirectory->cd(histoPath);
  TIter iter18(gDirectory->GetListOfKeys());
  TKey *key18;
  TClass *cl18;

  TCanvas *c18 = new TCanvas("c18","c18",558,409,900,600);
  TCanvas *c24 = new TCanvas("c24","c24",558,409,900,600);
//  TCanvas *c24RelRat = new TCanvas("c24RelRat", "c24RelRat",558,409,900,600);
  TCanvas *crelDif = new TCanvas("crelDif","crelDif",558,409,900,600);

  TH1F *hmyAssymetry = new TH1F("hmyAssymetry", "Relative assymetry Eff(2018) vs Eff(2024)",44, -1.1, 1.1);
  double myMean = 9.;
  double mySigma = 99.;
  double fractionOne = 9.;
//  std::vector<double> vmyMean;
//  std::vector<double> vmySigma;
//  std::vector<double> vfractionOne;

//  gSystem->RedirectOutput("out.txt", "w");

  int myCount = 0;  
  while((key18 = (TKey*) iter18.Next()))
  {
    myCount++;
    cl18 = gROOT->GetClass(key18->GetClassName());
//    if (cl18->InheritsFrom("TH1") && (myCount>=1) && (myCount <= 12))

    std::string strRE11 = (std::string(key18->GetName())).substr (0,22);
    std::string sre11 = "Muography_W-2_RB4-_S10";
    if (strRE11.compare(sre11) != 0) continue;	//sre11 e ostanalo ot predi. V sluchaia go izpolzvam za da izbera kamera po ime. 22 na red 54 otgovaria na broia simvoli v imeto na	kamerata i obiknoveneo go konfiguriram na ryka. Prosto ne mozhah da izmislia neshto po-umno :). A ako ne ogranicha analiza za konkretna kamera, triabva da chakam okolo chas da napravi kartinki za vsichki...


//    if (cl18->InheritsFrom("TH1") && (myCount <= 1200))
    {
      TH1 *h18 = (TH1*)key18->ReadObj();
      std::string outName18 = std::string(key18->GetName()) + "_18.png";
      std::replace(outName18.begin(), outName18.end(), '-', 'M');
      std::replace(outName18.begin(), outName18.end(), '+', 'P');
      std::string strforComp18 = std::string(key18->GetName());
      std::replace(strforComp18.begin(), strforComp18.end(), '-', 'M');
      std::replace(strforComp18.begin(), strforComp18.end(), '+', 'P');
      std::cout << outName18.c_str() << std::endl;
      c18->cd();
//      gStyle->SetOptStat(0);
      gStyle->SetOptStat("nemriou");
      gStyle->SetOptFit(1);
      h18->Draw("colz");
      c18->SaveAs(outName18.c_str());
//      gStyle->SetOptStat("nemriou");
      int countZeros = 0;

//////////////////////
      std::string outName18RelRatio = "(Eff(2018)-Eff(2024))/(Eff(2018)+Eff(2024)) " + std::string(key18->GetName());
//      std::replace(outName18RelRatio.begin(), outName18RelRatio.end(), '-', 'M');
//      std::replace(outName18RelRatio.begin(), outName18RelRatio.end(), '+', 'P');      
      TH1F *myRelDiff1D = new TH1F ("myRelDiff1D", outName18RelRatio.c_str(), 201, -2., +2.);

//////////////////////      
      file24->cd();      
      gDirectory->cd(histoPath);
      TIter iter24(gDirectory->GetListOfKeys());
      TKey *key24;
      TClass *cl24;
      while((key24 = (TKey*) iter24.Next())){
        cl24 = gROOT->GetClass(key24->GetClassName());
        if (cl24->InheritsFrom("TH1")){
          TH1 *h24 = (TH1*)key24->ReadObj();  
          std::string outName24 = std::string(key18->GetName()) + "_24.png";
          std::replace(outName24.begin(), outName24.end(), '-', 'M');
          std::replace(outName24.begin(), outName24.end(), '+', 'P');
          std::string strforComp24 = std::string(key24->GetName());
          std::replace(strforComp24.begin(), strforComp24.end(), '-', 'M');
          std::replace(strforComp24.begin(), strforComp24.end(), '+', 'P');
          if (strforComp18.compare(strforComp24) == 0){
            myMean = 9.;
            mySigma = 99.;
            fractionOne = 9.;
            std::cout << outName24.c_str() << std::endl;
            c24->cd();
            h24->Draw("COLZ");
            c24->SaveAs(outName24.c_str());
            
/////////////////
//            c24RelRat->cd();
            std::string outNameRelRatio = strforComp18 + "_relDiff.png";
            std::replace(outNameRelRatio.begin(), outNameRelRatio.end(), '-', 'M');
            std::replace(outNameRelRatio.begin(), outNameRelRatio.end(), '+', 'P');

            std::string outNameRelRatio1Dpng = strforComp18 + "_relDiff1D.png";
            std::string outNameRelRatio1D = strforComp18 + "_relDiff1D.C";
            std::replace(outNameRelRatio1D.begin(), outNameRelRatio1D.end(), '-', 'M');
            std::replace(outNameRelRatio1D.begin(), outNameRelRatio1D.end(), '+', 'P');

            crelDif->cd();
            int xmax = h18->GetXaxis()->GetNbins();
            int ymax = h18->GetYaxis()->GetNbins();
            std::cout << "\n" << "number of bins = " << xmax << "*" << ymax << " = " << xmax*ymax << std::endl;
            double rel = 99.;
            double a18 = -1;
            double a24 = -1;
            
             for(int i = 1; i <= xmax; i++){
              for(int j = 1; j <= ymax; j++){
                a18 = h18->GetBinContent(i,j);
                a24 = h24->GetBinContent(i,j);
                if (a18==0 && a24==0){
                  countZeros++;
                  myRelDiff1D->Fill(-2);	//v idealnia sluchaj, tezi triabva da otgovariat na praznite binove, deto predi gi rezhehme po geometrichni razmeri. Sega raboti i za sluchaia na trapecovidnite kameri. Pylnia gi na ryka v bin -2, zashoto kolegite napraviha spravedliv komentar, che tazi brojka binove ne se vizhda v krajnoto statistichesko obobshtenie. V neidealnia sluchaj - kamera koiato ne e e rabotila i v dvata sluchaia.
                  
                }
                else{
                  rel = (a18-a24)/(a18+a24);
                  myRelDiff1D->Fill(rel);
                }
              } 
            }
            gStyle->SetOptStat("eiou");
            gStyle->SetOptFit(1);
            myRelDiff1D->SetFillColor(kBlue+1);
            myRelDiff1D->Draw();
            
            int findFitMin = myRelDiff1D->GetXaxis()->FindBin(-0.15);
            int findFitMax = myRelDiff1D->GetXaxis()->FindBin(0.15);
            
            if (myRelDiff1D->Integral(findFitMin, findFitMax) == 0){
              std::cout << "at least one of the muography is empty and will not fit" << std::endl;
            }
            else{            
            TF1 *funcG = new TF1("funcG", "gaus",-0.15, 0.15);
            funcG->SetLineColor(kRed+1);
            funcG->SetLineWidth(3);
            myRelDiff1D->Fit("funcG", "Lre");	//loglikely range errors	https://root.cern.ch/root/htmldoc/guides/users-guide/FittingHistograms.html
//            if (funcG == nullptr) {
//              std::cout << "fit does not converge" << std::endl;
//            }
//            else{
            crelDif->SaveAs(outNameRelRatio1D.c_str());
            crelDif->SaveAs(outNameRelRatio1Dpng.c_str());

            myMean = funcG->GetParameter(1);
            mySigma = funcG->GetParameter(2);            
            std::cout << "myMean " << myMean << std::endl;
            std::cout << "mySigma " << mySigma << std::endl;
            delete funcG;
//            }
            }
      
//            crelDif->SaveAs(outNameRelRatio1D.c_str());
//            crelDif->SaveAs(outNameRelRatio1Dpng.c_str());

//            myMean = funcG->GetParameter(1);
//            mySigma = funcG->GetParameter(2);

//            vmyMean.push_back(myMean);
//            vmySigma.push_back(mySigma);
            
            int findOne = myRelDiff1D->GetXaxis()->FindBin(1.);
            int findMOne = myRelDiff1D->GetXaxis()->FindBin(-1.);
            int findZero = myRelDiff1D->GetXaxis()->FindBin(0.);

            int entriesAtOne = myRelDiff1D->GetBinContent(findOne);
            int entriesAtMOne = myRelDiff1D->GetBinContent(findMOne);
            int entriesAtZero = myRelDiff1D->GetBinContent(findZero);
            
//            double myIntegralNegative = myRelDiff1D->Integral(findMOne+1, findZero-1);
//            double myIntegralpositive = myRelDiff1D->Integral(findZero+1, findOne-1);
            
            double myIntegralNegative = myRelDiff1D->Integral(findMOne, findZero-1);
            double myIntegralpositive = myRelDiff1D->Integral(findZero+1, findOne);
            
            double myAssimetry = (myIntegralpositive-myIntegralNegative)/(myIntegralpositive+myIntegralNegative);
            hmyAssymetry->Fill(myAssimetry);
            int allEntries = myRelDiff1D->Integral();
            fractionOne = (entriesAtOne*100.)/(allEntries*1.);
            std::cout << "myAssimetry " << myAssimetry << std::endl;
//            std::cout << "myMean " << myMean << std::endl;
//            std::cout << "mySigma " << mySigma << std::endl;
            std::cout << "fraction at one " << fractionOne << std::endl;
            std::cout << std::endl;        
            std::cout << std::endl;        
          }
          delete h24;
        }
      }
      delete h18;      
      delete myRelDiff1D;
    }    
  }

  TCanvas *crelAssym = new TCanvas("crelAssym","crelAssym",558,409,900,600);
  crelAssym->cd();
  gStyle->SetOptStat("emriou");
  hmyAssymetry->SetFillColor(kBlue+1);
  hmyAssymetry->Draw();
  crelAssym->SaveAs("summary_relAssym_Muography_WM2_RB1.png");
  crelAssym->SaveAs("summary_relAssym_Muography_WM2_RB1.C");


  file18->Close();
  file24->Close();
  delete c18;
  delete c24;
//  delete c24RelRat;
  delete cl18;
  delete crelDif;
  delete crelAssym;
//  delete cl24;
  delete key18;
//  delete key24;
  delete file18;
  delete file24;
  exit(0);
}
//gApplication->Terminate(0);
