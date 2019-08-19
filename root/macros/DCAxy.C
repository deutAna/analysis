void MakeRawRatio(){

    TFile* filep  = new TFile("DCAxyPurity_prot.root");
    TFile* fileap = new TFile("DCAxyPurity_aprot.root");

    TH1F* hRawHist_p = (TH1F*)filep ->Get("hPurity")->Clone("hRawHist_p");
    TH1F* hRawHist_ap = (TH1F*)fileap->Get("hPurity")->Clone("hRawHist_ap");
hRawHist_p->SetTitle("Purity Proton / AntiProton");

    hRawHist_p->SetStats(kFALSE);
    hRawHist_ap->SetStats(kFALSE);
    hRawHist_p->SetLineColor(kRed);
   hRawHist_ap->SetLineColor(kBlue);
TCanvas *cs = new TCanvas("cs","cs");
   TLegend *legend = new TLegend(0.9,0.3,0.6,0.5);

    legend->SetHeader("Raw Spectrum"); // option "C" allows to center the header
    legend->AddEntry(hRawHist_p,"Purity Proton","pasme , Z");
    legend->AddEntry(hRawHist_ap,"Purity Anti-Proton","l");

hRawHist_p->SetMarkerStyle(kOpenSquare);
hRawHist_p->SetMarkerColor(kRed);
    hRawHist_p->Draw("psame , Z");
    cs->Update();
    //Float_t scale = 1*gPad->GetUymax();
    hRawHist_ap->Scale(1);

    hRawHist_ap->Draw("SAME");
    legend->Draw();

    TFile* fileout = new TFile("RawSpectrum_antid_d.root","RECREATE");
    hRawHist_p->Write();
    fileout->Close();

}
