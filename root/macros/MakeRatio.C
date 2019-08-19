void MakeRatio(){
    
    TFile* filed  = new TFile("RawSpectrum_FitLoose_deut.root");
    TFile* filead = new TFile("RawSpectrum_FitLoose_adeut.root");
    
    TH1F* hRawHist_d  = (TH1F*)filed ->Get("hRawSpectrum")->Clone("hRawHist_d");
    TH1F* hRawHist_ad = (TH1F*)filead->Get("hRawSpectrum")->Clone("hRawHist_ad");
    
    TFile* filepur = new TFile("Prim_FracDeuteron_Lucas.root");
    TH1F* hpurity  = (TH1F*)filepur->Get("fractions primary Deuteron")->Clone("hpurity");
    
    // correct deuterons for purity
    hRawHist_d->Multiply(hpurity);
    
    TH1F* ratio_antid_d = (TH1F*)hRawHist_ad->Clone("ratio_antid_d");
    
    ratio_antid_d->Divide(hRawHist_d);
    ratio_antid_d->Draw("e1");
    
//    TFile* fileout = new TFile("Ratio_antid_d.root","RECREATE");
//    ratio_antid_d->Write();
//    fileout->Close();
    
}
