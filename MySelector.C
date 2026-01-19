#define MySelector_cxx
// The class definition in MySelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("MySelector.C")
// root> T->Process("MySelector.C","some options")
// root> T->Process("MySelector.C+")
//

#include "MySelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TH1F.h>
#include <TCanvas.h>

void MySelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void MySelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   // 1. Initialize Histograms Here
   // We use 'new' to create them on the heap
   TH1F *h_pot = new TH1F("h_pot", "Potential Distribution;Voltage [V];Entries", 100, 0, 11);
   TH1F *h_cur = new TH1F("h_cur", "Current Distribution;Current [A];Entries", 100, 0, 2);
   TH2F *h_corr = new TH2F("h_corr", "Current vs Potential;Voltage [V];Current [A]", 50, 0, 11, 50, 0, 2);

   // 2. Add them to fOutput
   // fOutput is a list that manages objects created by the selector.
   // Crucial for PROOF (Parallel processing) to merge results from different workers.
   fOutput->Add(h_pot);
   fOutput->Add(h_cur);
   fOutput->Add(h_corr);
}

bool MySelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   // 1. Load the data for this entry
   fReader.SetLocalEntry(entry);

   // 2. Access the data using the TTreeReaderValue objects generated in Header
   // Note: The dereating operator (*) gives the value
   float current_val = *Current;
   float potential_val = *Potential;

   // 3. Selection (Example: Only analyze valid measurements)
   if (current_val > 0)
   {
      // 4. Fill Histograms
      // We retrieve them from fOutput by name
      TH1F *h_pot = (TH1F *)fOutput->FindObject("h_pot");
      TH1F *h_cur = (TH1F *)fOutput->FindObject("h_cur");
      TH2F *h_corr = (TH2F *)fOutput->FindObject("h_corr");

      if (h_pot)
         h_pot->Fill(potential_val);
      if (h_cur)
         h_cur->Fill(current_val);
      if (h_corr)
         h_corr->Fill(potential_val, current_val);
   }

   return true;
}

void MySelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
}

void MySelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   // 1. Retrieve the filled histograms from the output list
   TH1F *h_pot = (TH1F *)fOutput->FindObject("h_pot");
   TH2F *h_corr = (TH2F *)fOutput->FindObject("h_corr");

   // 2. Plot them
   if (h_corr)
   {
      TCanvas *c = new TCanvas("c_selector", "Selector Results", 800, 400);
      c->Divide(2, 1);

      c->cd(1);
      h_pot->Draw();

      c->cd(2);
      h_corr->Draw("COLZ");
   }
}