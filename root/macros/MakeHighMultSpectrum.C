


MakeHighMultSpectrumP(){

TFile *file_p = new TFile("RawSpectrum_prot.root");

TFile *file_high_p = new TFile("Raw_Spectra_Proton.root");


TH1F *hRawSpec_p = (TH1F*) file_p->Get("hRawSpectrum")->Clone("hRawSpec_p");

TH1F *hRawSpec_high_p = (TH1F*) file_high_p->Get("hRawSpectrum Proton")->Clone("hRawSpec_ap");

hRawSpec_high_p->SetLineColor(kRed);
hRawSpec_high_p->SetStats(false);
count(hRawSpec_p);

Tcanvas *c = new TCanvas ("c","c");
normlaize(hRawSpec_high_p);
hRawSpec_high_p->Draw();

c->Update();
//Float_t scale = 1*gPad->GetUymax();
//hRawSpec_p->Scale(1);
normlaize(hRawSpec_p);
hRawSpec_p->Draw("same");


}

void normlaize(TH1F* hist){
  //hist->Sumw2();
  cout<<"***************** SCALE IS : "<<(1.0 / hist->Integral())/0.1<<endl;
  hist->Scale(1.0 / hist->Integral(),"width");
}

count(TH1F* h){

  cout<<"*****************num of bins is :  "<<h->GetNbinsX()<<endl;
  for(int i = 0 ; i < h->GetNbinsX();i++){

    double d = h->GetBinContent(i);

    if (d!=0){
      cout<<"*************** At : "<<i<<"**** Value is :  "<<d<<endl;
    }

  }
}
