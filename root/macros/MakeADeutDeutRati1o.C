// input settings

const char* fDeut = "Prim_FracDeuteron_Lucas.root"; //1
const char* fRawd = "RawSpectrum_FitLoose_deut.root"; //2
const char* fRawAd = "RawSpectrum_FitLoose_adeut.root"; //3
const char* out = "adeut_deut_ratio"; // to save the output (not sure if it is correct ?)
const char* prim = "fractionsprimaryDeuteron"
const char* raw ="hRawSpectrum";
// the output should be ratio of 3/(1*2)
//declare functions

//void WriteHistos(TH2F* hADeutDeutRatio);


void MakeADeutDeutRatio(){
  TFile* fDeut = new TFile(fDeut);
  TFile* fRawd = new TFile(fRawd);
  TFile* fRawAd = new TFile(fRawAd);

  TH1F* hFracPrimDeut = (TH1F*) fDeut->Get(prim)->Clone("hFracPrimDeut");//1
  TH1F* hFitLooseDeut = (TH1F*) fRawd->Get(raw)->Clone("hFitLooseDeut");//2
  TH1F* hFitLooseADeut= (TH1F*) fRawAd->Get(raw)->Clone("hFitLooseADeut");//3

hFracPrimDeut->Multiply(hFitLooseDeut);
hFitLooseADeut->Divide(hFracPrimDeut);
  //hFracPrimDeut->GetXaxis()->SetRangeUser(0.,4.);
TH2F* hADeutDeutRatio = new TH2F("hADeutDeutRatio","ADeutDeutRatio", 0.,4.0,10.,0.,1.,8. );] // range of the momentum

hADeutDeutRatio->GetXaxis()->SetTitle("#it{p}, GeV/#it{c}");
hADeutDeutRatio->GetYaxis()->SetTitle("Antideuteron/Deuteron");

for (Int_t i = ; i < hFitLooseADeut->GetNbinsX(); i++) {
  /* code */
  hADeutDeutRatio->SetBinContent(i,hFitLooseADeut->GetBinContent(i));
  hADeutDeutRatio->SetBinError(i,hFitLooseADeut->GetBinError(i));
}
// to read the histograms

//TFile f("histos.root");
//TH1F *h = (TH1F*)f.Get("hgaus");

TCanvas* c1 = new TCanvas("c1","c1");
hADeutDeutRatio->Draw();


}
