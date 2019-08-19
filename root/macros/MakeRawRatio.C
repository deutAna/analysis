
// Save the figure in pdf, eps and png formats:
Bool_t bSaveFigures = kTRUE;


void MakeRawRatio(){

    TFile* filed  = new TFile("RawSpectrum_FitLoose_deut.root");
    TFile* filead = new TFile("RawSpectrum_FitLoose_adeut.root");

    TH1F* hRawHist_d  = (TH1F*)filed ->Get("hRawSpectrum")->Clone("hRawHist_d");
    TH1F* hRawHist_ad = (TH1F*)filead->Get("hRawSpectrum")->Clone("hRawHist_ad");

    hRawHist_d->SetTitle("Raw Spectrum Anti-D /D");
    hRawHist_d->SetStats(kFALSE);
    hRawHist_ad->SetStats(kFALSE);
    hRawHist_d->SetLineColor(kRed);
    hRawHist_ad->SetLineColor(kBlue);

   TCanvas *cfig = new TCanvas("cfig", "Alice Figure Template"); 
   TLegend *legend = new TLegend(0.9,0.9,0.6,0.75);

    legend->SetHeader("Raw Spectrum"); // option "C" allows to center the header
    legend->AddEntry(hRawHist_d,"Raw Deuteron","l");
    legend->AddEntry(hRawHist_ad,"Raw Anti-Deuteron","l");

    //hRawHist_d->GetYaxis()->SetTitle("p (GeV/c)");

    hRawHist_d->Draw("e1");
    hRawHist_ad->Draw("e1 same");
    legend->Draw();

    TFile* fileout = new TFile("RawSpectrum_antid_d.root","RECREATE");
    hRawHist_d->Write();
    fileout->Close();

if(bSaveFigures)
 {
  //cfig->SaveAs("ToyMC_SC42_const_2018-10-23.pdf");
  //cfig->SaveAs("ToyMC_SC42_const_2018-10-23.eps");
  cfig->SaveAs("RawSpectrum_AD_D.png");
 } // if(bSaveFigures)

}
