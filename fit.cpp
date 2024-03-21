#include "TCanvas.h"
#include "TF1.h"
#include "TFunction.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMath.h"
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

void fit(int id = 0) {
  std::cout << '\n' << '\n';
  std::cout << "-----------------------------------------------" << '\n';
  std::cout << "-----------------------------------------------" << '\n';
  // id = 1 100micro, id = 2 50micro
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
  if (id == 1) {     // 100micro
    // fit_max = 650;
    filename = "100micro.txt";
    graphtitle = "Caratteristica I-V a 100 microA";
    // b = 50;
    ded = 31;
    fit_name = "100microA";
  } else if (id == 2) { // 50micro
    // fit_max = 150;
    filename = "50micro.txt";
    graphtitle = "Caratteristica I-V a 50 microA";
    // b = 25;
    ded = 30;
    fit_name = "50microA";
  } else {
    std::cout << "id incorrect. Valid values are: 1(100micro), 2(50micro)"
              << '\n';
    return;
  }
  std::cout << "FITTING of " << fit_name << "--------------" << '\n';

  TF1 *f_fit = new TF1("f_fit", lin_fit, range_min, range_max, 2);

  f_fit->SetParameter(0, a);
  f_fit->SetParameter(1, b);

  f_fit->SetParName(0, "V_A");
  f_fit->SetParName(1, "R");

  // f_fit->SetParLimits(0, -100, 100);
  // f_fit->SetParLimits(1, 0.9943, 1);

  gStyle->SetOptFit(0);

  TCanvas *c1 = new TCanvas("c2", graphtitle.c_str(), 1280, 720);
  c1->cd();
  // c1->SetLogy();
  c1->SetGridx();
  c1->SetGridy();

  // per come leggere i file "root lezione III" del primo anno
  TGraphErrors *graph =
      new TGraphErrors(filename.c_str(), "%lg %lg %lg %lg", "");

  graph->SetTitle(graphtitle.insert(ded, (";V_{CE} (V);I_{C} (mA)")).c_str());
  // graph->SetLineColor(kBlue);
  graph->SetMarkerStyle(20);
  graph->SetMarkerColor(kBlue);
  graph->SetLineWidth(2);
  graph->SetFillColor(6);
  // su "root lezione II" del primo anno si trovano gli stili

  graph->Fit("f_fit", "R", "", fit_min, fit_max);

  graph->Draw("APE");
  // graph->Draw("3, same"); // per mostrare le fasce di errori
  f_fit->Draw("same");

  TF1 *fitRes = graph->GetFunction("f_fit");

  std::cout << "fit result of " << fit_name << "--------------" << '\n';
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
  return;
}