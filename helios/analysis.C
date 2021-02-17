{

  TFile * file = new TFile("000Everything.root");

  TTree * tree = (TTree *) file->FindObjectAny("Clover");

  TH1F * h0 = new TH1F("h0", "Crystal-0; Energy[keV]; count / 1 keV", 7000,  1000, 8000); h0->SetLineColor(2);
  TH1F * h1 = new TH1F("h1", "Crystal-1; Energy[keV]; count / 1 keV", 7000,  1000, 8000); h1->SetLineColor(4);
  TH1F * h2 = new TH1F("h2", "Crystal-2; Energy[keV]; count / 1 keV", 7000,  1000, 8000); h2->SetLineColor(6);
  TH1F * h3 = new TH1F("h3", "Crystal-3; Energy[keV]; count / 1 keV", 7000,  1000, 8000); h3->SetLineColor(7);

  TCanvas * c1 = new TCanvas("c1", "Clover plots", 1200, 400);
  //c1->Divide(1,4);

  /*c1->cd(1);*/ tree->Draw("energy[0]>>h0", "energy[0]>0");
  /*c1->cd(2);*/ tree->Draw("energy[1]>>h1", "energy[1]>0");
  /*c1->cd(3);*/ tree->Draw("energy[2]>>h2", "energy[2]>0");
  /*c1->cd(4);*/ tree->Draw("energy[3]>>h3", "energy[3]>0");

  //find maximum height
  double yMax = 0;

  if( h0->GetMaximum() > yMax) yMax = h0->GetMaximum();
  if( h1->GetMaximum() > yMax) yMax = h1->GetMaximum();
  if( h2->GetMaximum() > yMax) yMax = h2->GetMaximum();
  if( h3->GetMaximum() > yMax) yMax = h3->GetMaximum();

  h0->SetMaximum( yMax * 1.2);
  h1->SetMaximum( yMax * 1.2);
  h2->SetMaximum( yMax * 1.2);
  h3->SetMaximum( yMax * 1.2);

  h0->Draw("");
  h1->Draw("same");
  h2->Draw("same");
  h3->Draw("same");

}
