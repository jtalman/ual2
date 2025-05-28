// @(#)root/mlp:$Name:  $:$Id: TMLPAnalyzer.cxx,v 1.10 2005/02/03 20:32:55 brun Exp $
// Author: Christophe.Delaere@cern.ch   25/04/04

/*************************************************************************
 * Copyright (C) 1995-2003, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

///////////////////////////////////////////////////////////////////////////
//
// TMLPAnalyzer
//
// This utility class contains a set of tests usefull when developing
// a neural network.
// It allows you to check for unneeded variables, and to control
// the network structure.
//
///////////////////////////////////////////////////////////////////////////

#include "TSynapse.h"
#include "TNeuron.h"
#include "TMultiLayerPerceptron.h"
#include "TMLPAnalyzer.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TEventList.h"
#include "TH1D.h"
#include "TProfile.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "Riostream.h"

ClassImp(TMLPAnalyzer)

//______________________________________________________________________________
TMLPAnalyzer::~TMLPAnalyzer() { 
   delete fAnalysisTree;
   delete fIOTree;
}

//______________________________________________________________________________
Int_t TMLPAnalyzer::GetLayers()
{
   // Returns the number of layers.

   TString fStructure = fNetwork->GetStructure();
   return fStructure.CountChar(':')+1;
}

//______________________________________________________________________________
Int_t TMLPAnalyzer::GetNeurons(Int_t layer)
{
   // Returns the number of neurons in given layer.

   if(layer==1) {
      TString fStructure = fNetwork->GetStructure();
      TString input      = TString(fStructure(0, fStructure.First(':')));
      return input.CountChar(',')+1;
   }
   else if(layer==GetLayers()) {
      TString fStructure = fNetwork->GetStructure();
      TString output = TString(fStructure(fStructure.Last(':') + 1,
                               fStructure.Length() - fStructure.Last(':')));
      return output.CountChar(',')+1;
   }
   else {
     Int_t cnt=1;
     TString fStructure = fNetwork->GetStructure();
     TString hidden = TString(fStructure(fStructure.First(':') + 1,
                              fStructure.Last(':') - fStructure.First(':') - 1));
     Int_t beg = 0;
     Int_t end = hidden.Index(":", beg + 1);
     Int_t num = 0;
     while (end != -1) {
       num = atoi(TString(hidden(beg, end - beg)).Data());
       cnt++;
       beg = end + 1;
       end = hidden.Index(":", beg + 1);
       if(layer==cnt) return num;
     }
     num = atoi(TString(hidden(beg, hidden.Length() - beg)).Data());
     cnt++;
     if(layer==cnt) return num;
   }
   return -1;
}

//______________________________________________________________________________
TString TMLPAnalyzer::GetNeuronFormula(Int_t idx)
{
   // Returns the formula used as input for neuron (idx) in
   // the first layer.

   TString fStructure = fNetwork->GetStructure();
   TString input      = TString(fStructure(0, fStructure.First(':')));
   Int_t beg = 0;
   Int_t end = input.Index(",", beg + 1);
   TString brName;
   Int_t cnt = 0;
   while (end != -1) {
      brName = TString(input(beg, end - beg));
      beg = end + 1;
      end = input.Index(",", beg + 1);
      if(cnt==idx) return brName;
      cnt++;
   }
   brName = TString(input(beg, input.Length() - beg));
   return brName;
}

//______________________________________________________________________________
const char* TMLPAnalyzer::GetInputNeuronTitle(Int_t in) {
   TNeuron* neuron=(TNeuron*)fNetwork->fFirstLayer[in];
   return neuron ? neuron->GetName() : "NO SUCH NEURON";
}

//______________________________________________________________________________
const char* TMLPAnalyzer::GetOutputNeuronTitle(Int_t out) {
   TNeuron* neuron=(TNeuron*)fNetwork->fLastLayer[out];
   return neuron ? neuron->GetName() : "NO SUCH NEURON";
}

//______________________________________________________________________________
void TMLPAnalyzer::CheckNetwork()
{
   // Gives some information about the network in the terminal.

   TString fStructure = fNetwork->GetStructure();
   cout << "Network with structure: " << fStructure.Data() << endl;
   cout << "inputs with low values in the differences plot may not be needed" << endl;
   // Checks if some input variable is not needed
   char var[64], sel[64];
   for (Int_t i = 0; i < GetNeurons(1); i++) {
      sprintf(var,"Diff>>tmp%d",i);
      sprintf(sel,"InNeuron==%d",i);
      fAnalysisTree->Draw(var, sel, "goff");
      TH1F* tmp = (TH1F*)gDirectory->Get(Form("tmp%d",i));
      cout << GetInputNeuronTitle(i)
           << " -> " << tmp->GetMean()
           << " +/- " << tmp->GetRMS() << endl;
   }
}

//______________________________________________________________________________
void TMLPAnalyzer::GatherInformations()
{
   // Collect informations about what is usefull in the network.
   // This method has to be called first when analyzing a network.
   // Fills the two analysis trees.

   Double_t shift = 0.1;

   TTree* data = fNetwork->fData;
   TEventList* test = fNetwork->fTest;
   Int_t nEvents = test->GetN();
   Int_t NN = GetNeurons(1);
   Double_t* params = new Double_t[NN];
   Double_t* rms    = new Double_t[NN];
   TTreeFormula** formulas = new TTreeFormula*[NN];
   TString formula;
   Int_t i(0), j(0), k(0), l(0);
   for(i=0; i<NN; i++){
      formula = GetNeuronFormula(i);
      formulas[i] = new TTreeFormula(Form("NF%d",this),formula,data);
      TH1D tmp("tmpb", "tmpb", 1, -FLT_MAX, FLT_MAX);
      data->Draw(Form("%s>>tmpb",formula.Data()),"","goff");
      rms[i]  = tmp.GetRMS();
   }

   Int_t InNeuron = 0;
   Double_t Diff = 0.;
   if(fAnalysisTree) delete fAnalysisTree;
   fAnalysisTree = new TTree("result","analysis");
   fAnalysisTree->SetDirectory(0);
   fAnalysisTree->Branch("InNeuron",&InNeuron,"InNeuron/I");
   fAnalysisTree->Branch("Diff",&Diff,"Diff/D");

   Int_t numOutNodes=GetNeurons(GetLayers());
   Double_t *outVal=new Double_t[numOutNodes];
   Double_t *trueVal=new Double_t[numOutNodes];

   delete fIOTree;
   fIOTree=new TTree("MLP_iotree","MLP_iotree");
   fIOTree->SetDirectory(0);
   TString leaflist;
   for (i=0; i<NN; i++)
      leaflist+=Form("In%d/D:",i);
   leaflist.Remove(leaflist.Length()-1);
   fIOTree->Branch("In", params, leaflist);

   leaflist="";
   for (i=0; i<numOutNodes; i++)
      leaflist+=Form("Out%d/D:",i);
   leaflist.Remove(leaflist.Length()-1);
   fIOTree->Branch("Out", outVal, leaflist);

   leaflist="";
   for (i=0; i<numOutNodes; i++)
      leaflist+=Form("True%d/D:",i);
   leaflist.Remove(leaflist.Length()-1);
   fIOTree->Branch("True", trueVal, leaflist);

   Double_t v1 = 0.;
   Double_t v2 = 0.;
   // Loop on the events in the test sample
   for(j=0; j< nEvents; j++) {
      fNetwork->GetEntry(test->GetEntry(j));

      // Loop on the neurons to evaluate
      for(k=0; k<GetNeurons(1); k++) 
         params[k] = formulas[k]->EvalInstance();

      for(k=0; k<GetNeurons(GetLayers()); k++) {
         outVal[k] = fNetwork->Evaluate(k,params);
         trueVal[k] = ((TNeuron*)fNetwork->fLastLayer[k])->GetBranch();
      }

      fIOTree->Fill();

      // Loop on the input neurons
      for (i = 0; i < GetNeurons(1); i++) {
         InNeuron = i;
         Diff = 0;
         // Loop on the neurons in the output layer
         for(l=0; l<GetNeurons(GetLayers()); l++){
            params[i] += shift*rms[i];
            v1 = fNetwork->Evaluate(l,params);
            params[i] -= 2*shift*rms[i];
            v2 = fNetwork->Evaluate(l,params);
	    Diff += (v1-v2)*(v1-v2);
            // reset to original vealue
            params[i] += shift*rms[i];
	 }
         Diff = TMath::Sqrt(Diff);
         fAnalysisTree->Fill();
      }
   }
   delete[] params;
   delete[] rms;
   delete[] outVal;
   delete[] trueVal;
   for(i=0; i<GetNeurons(1); i++) delete formulas[i]; delete [] formulas;
   fAnalysisTree->ResetBranchAddresses();
   fIOTree->ResetBranchAddresses();
}

//______________________________________________________________________________
void TMLPAnalyzer::DrawDInput(Int_t i)
{
   // Draws the distribution (on the test sample) of the
   // impact on the network output of a small variation of
   // the ith input.

   char sel[64];
   sprintf(sel, "InNeuron==%d", i);
   fAnalysisTree->Draw("Diff", sel);
}

//______________________________________________________________________________
void TMLPAnalyzer::DrawDInputs()
{
   // Draws the distribution (on the test sample) of the
   // impact on the network output of a small variation of
   // each input.

   THStack* stack  = new THStack("differences","differences (impact of variables on ANN)");
   TLegend* legend = new TLegend(0.75,0.75,0.95,0.95);
   TH1F* tmp = NULL;
   char var[64], sel[64];
   for(Int_t i = 0; i < GetNeurons(1); i++) {
      sprintf(var, "Diff>>tmp%d", i);
      sprintf(sel, "InNeuron==%d", i);
      fAnalysisTree->Draw(var, sel, "goff");
      tmp = (TH1F*)gDirectory->Get(Form("tmp%d",i));
      tmp->SetDirectory(0);
      tmp->SetLineColor(i+1);
      stack->Add(tmp);
      legend->AddEntry(tmp,GetInputNeuronTitle(i),"l");
   }
   stack->Draw("nostack");
   legend->Draw();
   gPad->SetLogy();
}

//______________________________________________________________________________
void TMLPAnalyzer::DrawNetwork(Int_t neuron, const char* signal, const char* bg)
{
   // Draws the distribution of the neural network (using ith neuron).
   // Two distributions are drawn, for events passing respectively the "signal"
   // and "background" cuts. Only the test sample is used.

   TTree* data = fNetwork->fData;
   TEventList* test = fNetwork->fTest;
   TEventList* current = data->GetEventList();
   data->SetEventList(test);
   THStack* stack = new THStack("__NNout_TMLPA",Form("Neural net output (neuron %d)",neuron));
   TH1F *bgh  = new TH1F("__bgh_TMLPA", "NN output", 50, -0.5, 1.5);
   TH1F *sigh = new TH1F("__sigh_TMLPA", "NN output", 50, -0.5, 1.5);
   bgh->SetDirectory(0);
   sigh->SetDirectory(0);
   Int_t nEvents = 0;
   Int_t j=0;
   // build event lists for signal and background
   TEventList* signal_list = new TEventList("__tmpSig_MLPA");
   TEventList* bg_list     = new TEventList("__tmpBkg_MLPA");
   data->Draw(">>__tmpSig_MLPA",signal,"goff");
   data->Draw(">>__tmpBkg_MLPA",bg,"goff");

   // fill the background
   nEvents = bg_list->GetN();
   for(j=0; j< nEvents; j++) {
      bgh->Fill(fNetwork->Result(bg_list->GetEntry(j),neuron));
   }
   // fill the signal
   nEvents = signal_list->GetN();
   for(j=0; j< nEvents; j++) {
      sigh->Fill(fNetwork->Result(signal_list->GetEntry(j),neuron));
   }
   // draws the result
   bgh->SetLineColor(kBlue);
   bgh->SetFillStyle(3008);
   bgh->SetFillColor(kBlue);
   sigh->SetLineColor(kRed);
   sigh->SetFillStyle(3003);
   sigh->SetFillColor(kRed);
   bgh->SetStats(0);
   sigh->SetStats(0);
   stack->Add(bgh);
   stack->Add(sigh);
   TLegend *legend = new TLegend(.75, .80, .95, .95);
   legend->AddEntry(bgh, "Background");
   legend->AddEntry(sigh,"Signal");
   stack->Draw("nostack"); 
   legend->Draw();
   // restore the default event list
   data->SetEventList(current);
   delete signal_list;
   delete bg_list;
}

//______________________________________________________________________________
TProfile* TMLPAnalyzer::DrawTruthDeviation(Int_t outnode /*=0*/, 
                                           Option_t *option /*=""*/) 
{
   // Create a profile of the difference of the MLP output minus the
   // true value for a given output node outnode, vs the true value for 
   // outnode, for all test data events. This method is mainly useful 
   // when doing regression analysis with the MLP (i.e. not classification, 
   // but continuous truth values).
   // The resulting TProfile histogram is returned.
   // It is not drawn if option "goff" is specified.
   // Options are passed to TProfile::Draw

   if (!fIOTree) GatherInformations();
   TString pipehist=Form("MLP_truthdev_%d",outnode);
   TString drawline;
   drawline.Form("Out.Out%d-True.True%d:True.True%d>>", 
                 outnode, outnode, outnode);
   fIOTree->Draw(drawline+pipehist+"(20)", "", "goff prof");
   TProfile* h=(TProfile*)gDirectory->Get(pipehist);
   h->SetDirectory(0);
   const char* title=GetOutputNeuronTitle(outnode);
   if (title) {
      h->SetTitle(Form("#Delta(output - truth) vs. truth for %s",
                      title));
      h->GetXaxis()->SetTitle(title);
      h->GetYaxis()->SetTitle(Form("#Delta(output - truth) for %s", title));
   }
   if (!strstr(option,"goff"))
      h->Draw();
   return h;
}

//______________________________________________________________________________
THStack* TMLPAnalyzer::DrawTruthDeviations(Option_t *option /*=""*/)
{
   // Creates TProfiles of the difference of the MLP output minus the
   // true value vs the true value, one for each output, filled with the 
   // test data events. This method is mainly useful when doing regression 
   // analysis with the MLP (i.e. not classification, but continuous truth 
   // values).
   // The returned THStack contains all the TProfiles. It is drawn unless
   // the option "goff" is specified.
   // Options are passed to TProfile::Draw.
   THStack *hs=new THStack("MLP_TruthDeviation",
                           "Deviation of MLP output from truth");

   // leg!=0 means we're drawing
   TLegend *leg=0;
   if (!option || !strstr(option,"goff"))
      leg=new TLegend(.4,.85,.95,.95,"#Delta(output - truth) vs. truth for:");

   const char* xAxisTitle=0;

   // create profile for each input neuron, 
   // adding them into the THStack and the TLegend 
   for (Int_t outnode=0; outnode<GetNeurons(GetLayers()); outnode++) {
      TProfile* h=DrawTruthDeviation(outnode, "goff");
      h->SetLineColor(1+outnode);
      hs->Add(h, option);
      if (leg) leg->AddEntry(h,GetOutputNeuronTitle(outnode));
      if (!outnode)
         // Xaxis title is the same for all, extract it from the first one.
         xAxisTitle=h->GetXaxis()->GetTitle();
   }

   if (leg) {
      hs->Draw("nostack");
      leg->Draw();
      // gotta draw before accessing the axes
      hs->GetXaxis()->SetTitle(xAxisTitle);
      hs->GetYaxis()->SetTitle("#Delta(output - truth)");
   }

   return hs;
}

//______________________________________________________________________________
TProfile* TMLPAnalyzer::DrawTruthDeviationInOut(Int_t innode, 
                                                Int_t outnode /*=0*/, 
                                                Option_t *option /*=""*/)
{
   // Creates a profile of the difference of the MLP output outnode minus
   // the true value of outnode vs the input value innode, for all test
   // data events.
   // The resulting TProfile histogram is returned.
   // It is not drawn if option "goff" is specified.
   // Options are passed to TProfile::Draw

   if (!fIOTree) GatherInformations();
   TString pipehist=Form("MLP_truthdev_i%d_o%d", innode, outnode);
   TString drawline;
   drawline.Form("Out.Out%d-True.True%d:In.In%d>>", 
                 outnode, outnode, innode);
   fIOTree->Draw(drawline+pipehist+"(50)", "", "goff prof");
   TProfile* h=(TProfile*)gROOT->FindObject(pipehist);
   h->SetDirectory(0);
   const char* titleInNeuron=GetInputNeuronTitle(innode);
   const char* titleOutNeuron=GetOutputNeuronTitle(outnode);
   h->SetTitle(Form("#Delta(output - truth) of %s vs. input %s",
                    titleOutNeuron, titleInNeuron));
   h->GetXaxis()->SetTitle(Form("%s", titleInNeuron));
   h->GetYaxis()->SetTitle(Form("#Delta(output - truth) for %s", 
                                titleOutNeuron));
   if (!strstr(option,"goff"))
      h->Draw(option);
   return h;
}

//______________________________________________________________________________
THStack* TMLPAnalyzer::DrawTruthDeviationInsOut(Int_t outnode /*=0*/, 
                                                Option_t *option /*=""*/)
{
   // Creates a profile of the difference of the MLP output outnode minus the
   // true value of outnode vs the input value, stacked for all inputs, for
   // all test data events.
   // The returned THStack contains all the TProfiles. It is drawn unless 
   // the option "goff" is specified.
   // Options are passed to TProfile::Draw.
   TString sName;
   sName.Form("MLP_TruthDeviationIO_%d", outnode);
   const char* outputNodeTitle=GetOutputNeuronTitle(outnode);
   THStack *hs=new THStack(sName,
                           Form("Deviation of MLP output %s from truth",
                                outputNodeTitle));

   // leg!=0 means we're drawing.
   TLegend *leg=0;
   if (!option || !strstr(option,"goff"))
      leg=new TLegend(.4,.75,.95,.95,
                      Form("#Delta(output - truth) of %s vs. input for:",
                           outputNodeTitle));

   // create profile for each input neuron, 
   // adding them into the THStack and the TLegend 
   Int_t numInNodes=GetNeurons(1);
   Int_t innode=0;
   for (innode=0; innode<numInNodes; innode++) {
      TProfile* h=DrawTruthDeviationInOut(innode, outnode, "goff");
      h->SetLineColor(1+innode);
      hs->Add(h, option);
      if (leg) leg->AddEntry(h,h->GetXaxis()->GetTitle());
   }

   if (leg) {
      hs->Draw("nostack");
      leg->Draw();
      // gotta draw before accessing the axes
      hs->GetXaxis()->SetTitle("Input value");
      hs->GetYaxis()->SetTitle(Form("#Delta(output - truth) for %s", 
                                 outputNodeTitle));
   }

   return hs;
}
