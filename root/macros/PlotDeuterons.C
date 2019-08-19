// input settings


const char* fRawd = "RawSpectrum_FitLoose_deut.root"; //2
const char* fRawAd = "RawSpectrum_FitLoose_adeut.root"; //3
//const char* out = "adeut_deut_ratio";
//const char* prim = "fractions primary Deuteron"
const char* raw ="hRawSpectrum"; // Deuteron


void PlotDeuterons(){
  TFile* fRawd = new TFile(fRawd);
  TFile* fRawAd = new TFile(fRawAd);



  TH1F* hFitLooseDeut = (TH1F*) fRawd->Get(raw)->Clone("hFitLooseDeut");//2
  //hFitLooseDeut->Draw();

  TH1F* hFitLooseADeut= (TH1F*) fRawAd->Get(raw)->Clone("hFitLooseADeut");//3
  //hFitLooseADeut->Draw();
//  hFitLooseDeut->GetXaxis()->SetTitle("Antideuteron / Deuteron");
hFitLooseDeut->SetStats(kFALSE);

gStyle->SetLegendBorderSize(0);
  TCanvas *c1 = new TCanvas("c1","c1");
  //hFitLooseDeut->SetMarkerStyle();
  TLegend *legend = new TLegend(0.9,0.9,0.7,0.7);// (x1,y1,x2,y2)x1->> 1st parameter
legend->SetFillStyle(0);
  //legend->SetHeader("Raw Primary Spectrum");
  legend->AddEntry(hFitLooseDeut,"Deuteron","l");
  legend->AddEntry(hFitLooseADeut,"ADeuteron","l");
  hFitLooseDeut->SetLineColor(kRed);
  hFitLooseDeut->Draw();
  c1->Update();
  //hFitLooseADeut->SetMarkerStyle(kOpenCircle);
  hFitLooseADeut->SetLineColor(kBlue);
  hFitLooseADeut->Draw("same");
legend->Draw("same");
  c1->SaveAs("AntiDeuteron_Deuterons.png");
}
