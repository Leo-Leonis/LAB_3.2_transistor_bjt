#include "TCanvas.h"
#include "TF1.h"
#include "TFunction.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TStyle.h"

#include <iostream>
#include <string>

Double_t lin_fit(Double_t *x, Double_t *par) {
  Float_t xx = x[0];
  Double_t a = par[0]; // V_A
  Double_t b = par[1]; // R

  return (xx + a) / b;
  // return b * xx + a;
}

void fit() {
  std::cout << '\n' << '\n';
  std::cout << "-----------------------------------------------" << '\n';
  std::cout << "-----------------------------------------------" << '\n';
  Double_t range_min = 0;   // valore minimo del dominio del fit
  Double_t range_max = 4.5; // valore massimo del dominio del fit
  // Double_t fit_min = 0.39; // valore minimo di fit
  Double_t fit_min = 0.99; // valore minimo di fit
  Double_t fit_max = 4;    // valore massimo di fit
  std::string filename;
  std::string graphtitle;
  std::string fit_name;
  Double_t a = 30;   // V_A
  Double_t b = 2000; // R
  int ded;           // posizione dell'ultimo carattere in graphtitle

  TCanvas *c1 = new TCanvas("c1", "I-V caratteristiche", 1280, 720);
  c1->cd();
  c1->SetGridx();
  c1->SetGridy();
  TF1 *f_fit = new TF1("f_fit", lin_fit, range_min, range_max, 2);
  TGraphErrors *graph[2];

  graph[0] = new TGraphErrors("100micro.txt", "%lg %lg %lg %lg", "");
  graph[0]->SetTitle(
      "Caratteristica I-V a -100 {#micro}A;|V_{CE}| (V);|I_{C}| (A)");
  graph[0]->SetMarkerStyle(20);
  graph[0]->SetMarkerColor(kBlue);
    graph[0]->SetLineWidth(2);

  graph[1] = new TGraphErrors("50micro.txt", "%lg %lg %lg %lg", "");
  graph[1]->SetTitle(
      "Caratteristica I-V a -50 {#micro}A;|V_{CE}| (V);|I_{C}| (A)");
  graph[1]->SetMarkerStyle(20);
  graph[1]->SetMarkerColor(kGreen);
  graph[1]->SetLineWidth(2);

  std::cout << "FITTING of "
            << "-100 microA"
            << "--------------" << '\n';

  f_fit->SetParameter(0, a);
  f_fit->SetParameter(1, b);

  f_fit->SetParName(0, "V_A");
  f_fit->SetParName(1, "R");

  // f_fit->SetParLimits(0, -100, 100);
  // f_fit->SetParLimits(1, 0.9943, 1);

  gStyle->SetOptFit(0);

  graph[1]->Fit("f_fit", "R", "", fit_min, fit_max);
  //graph[1]->Draw("APE");
  //f_fit->Draw("same");
  graph[0]->Fit("f_fit", "R", "", fit_min, fit_max);
  //graph[0]->Draw("APE,same");
  //f_fit->Draw("same");

  // graph->Draw("3, same"); // per mostrare le fasce di errori

  TF1 *fitRes = graph[0]->GetFunction("f_fit");

  std::cout << "fit result of "
            << "-100microA"
            << "--------------" << '\n';
  std::cout << "           Chi^2: " << fitRes->GetChisquare() << '\n'
            << "             NDF: " << fitRes->GetNDF() << '\n'
            << "       Chi^2/NDF: " << fitRes->GetChisquare() / fitRes->GetNDF()
            << '\n'
            << "     Probabilità: " << fitRes->GetProb() << '\n'
            << "\033[32m"
            << "             V_A: " << fitRes->GetParameter(0) << " ± "
            << fitRes->GetParError(0) << "\033[0m" << '\n'
            << "\033[32m"
            << "               R: " << fitRes->GetParameter(1) << " ± "
            << fitRes->GetParError(1) << "\033[0m" << '\n';

  fitRes = graph[1]->GetFunction("f_fit");

  std::cout << "fit result of "
            << "-50microA"
            << "--------------" << '\n';
  std::cout << "           Chi^2: " << fitRes->GetChisquare() << '\n'
            << "             NDF: " << fitRes->GetNDF() << '\n'
            << "       Chi^2/NDF: " << fitRes->GetChisquare() / fitRes->GetNDF()
            << '\n'
            << "     Probabilità: " << fitRes->GetProb() << '\n'
            << "\033[32m"
            << "             V_A: " << fitRes->GetParameter(0) << " ± "
            << fitRes->GetParError(0) << "\033[0m" << '\n'
            << "\033[32m"
            << "               R: " << fitRes->GetParameter(1) << " ± "
            << fitRes->GetParError(1) << "\033[0m" << '\n';

  TCanvas *c2 = new TCanvas("c2", "Caratteristiche I-V del BJT", 1280, 720);
  c2->cd();
  c2->SetGridx();
  c2->SetGridy();

  TMultiGraph *mg = new TMultiGraph("mg", "Caratteristiche I-V del BJT PNP;|V_{CE}| (V);|I_{C}| (A)");
  mg->Add(graph[1]);
  mg->Add(graph[0]);
  mg->Draw("APE");

  TLegend *leg = new TLegend(.6, .1, .9, .3);
  leg->SetHeader("Legenda", "C");
  leg->AddEntry(graph[1], "I_{B1} = -50#muA");
  leg->AddEntry(graph[0], "I_{B2} = -100#muA");
  leg->AddEntry(f_fit, "Fit");
  leg->Draw("Same");
  return;
}