


void CompareRawProton(){

  TFile* primFile = TFile::Open("Prim_Spectra_Proton.root");
  TFile* rawFile = TFile::Open("Corrected_proton.root");

  TH1F* hPrim;
  TH1F* hRaw;

  primFile->GetObject("PrimarySpectrum Proton",hPrim);
  rawFile->GetObject("hRawSpectrum",hRaw);

hPrim->SetMarkerStyle(kOpenSquare);
hRaw->SetLineColor(kRed);
hPrim->SetLineColor(kBlack);

  Tcanvas *hRawSpectrum = new TCanvas ("hRawSpectrum","hRawSpectrum");

    TLegend * legend = new TLegend(0.9,0.7,0.6,0.75);
    legend->AddEntry(hPrim,"Num. of Events 2.943e+10","l");
    legend->AddEntry(hRaw,"Num. of Events 1.727e+08","l");
  normlaize(hRaw);
  hRaw->Draw();

  hRawSpectrum->Update();
  //Float_t scale = 1*gPad->GetUymax();
  //hRawSpec_p->Scale(1);
  normlaize(hPrim);
  hPrim->Draw("same");
  hRawSpectrum->Update();
  legend->Draw();
  hRawSpectrum->SaveAs("Normalization_Corected_proton.root");

}

void normlaize(TH1F* hist){
  //hist->Sumw2();
  cout<<"***************** SCALE IS : "<<(1.0 / hist->Integral())/0.1<<endl;
  hist->Scale(1.0 / hist->Integral(),"width");
}
