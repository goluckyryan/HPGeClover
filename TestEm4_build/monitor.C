void monitor(){

  TFile * file = new TFile("testem4.root", "r");

  TH1F* h1 = (TH1F *) file->FindObjectAny("h1");

  TCanvas * can = new TCanvas("c", "c", 1200, 400);
  gStyle->SetOptStat("neiou");

  h1->Draw("hist");

}
