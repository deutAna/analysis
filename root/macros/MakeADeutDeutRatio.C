// input settings

const char* fDeut = "Prim_FracDeuteron_Lucas.root"; //1
const char* fRawd = "RawSpectrum_FitLoose_deut.root"; //2
const char* fRawAd = "RawSpectrum_FitLoose_adeut.root"; //3
//const char* out = "adeut_deut_ratio";
//const char* prim = "fractions primary Deuteron"
const char* rawD ="hRawSpectrum"; // Deuteron
const char* rawAD ="hRawSpectrum"; // ADeuteron

//declare functions

void MakeADeutDeutRatio(){

  TFile* file = new TFile(fDeut);
  TFile* fRawd = new TFile(fRawd);
  TFile* fRawAd = new TFile(fRawAd);

  TH1F* hFracPrimDeut = (TH1F*) file->Get("fractions primary Deuteron")->Clone("hFracPrimDeut");
  //hFracPrimDeut->Draw();

  TH1F* hFitLooseDeut = (TH1F*) fRawd->Get(rawD)->Clone("hFitLooseDeut");//2
  //hFitLooseDeut->Draw();

  TH1F* hFitLooseADeut= (TH1F*) fRawAd->Get(rawAD)->Clone("hFitLooseADeut");//3

  hFracPrimDeut->Multiply(hFitLooseDeut);
  //hFitLooseADeut->Draw();

  TH1F* ratio = (TH1F*) hFitLooseADeut->Clone("ratio");
  ratio->GetXaxis()->SetTitle("#it{p}, GeV/#it{c}");
  ratio->GetYaxis()->SetTitle("Antideuteron / Deuteron");
  ratio->Divide(hFracPrimDeut);
 TCanvas *c1 = new TCanvas("c1","c1");
ratio->SetStats(kFALSE);
  ratio->Draw("e1");
c1->SaveAs("AntiDeuteronRatio.png");
//https://root.cern.ch/doc/v608/classTHistPainter.html







}
