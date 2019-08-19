
void StackPurityAPPMult(){


    TFile* filep = new TFile("RawSpectrum_prot.root"); // raw protons
    TFile* fileap  = new TFile("RawSpectrum_aprot.root"); // raw anti-protons
    TFile* filepc = new TFile("DCAxyPurity_prot.root"); // primary raw protons
    TFile* fileapc  = new TFile("DCAxyPurity_aprot.root"); // primary raw anti-protons


    TH1F* hRawHist_p  = (TH1F*) filep->Get("hRawSpectrum")->Clone("hRawHist_p");
    //TH1F* h1 = (TH1F*) hRawHist_p->Clone("h1"); //

    TH1F* hRawHist_ap = (TH1F*) fileap->Get("hRawSpectrum")->Clone("hRawHist_ap");
    //TH1F* h2 = (TH1F*) hRawHist_ap->Clone("h2");

    TH1F* hRawHist_pc = (TH1F*) filepc->Get("hPurity")->Clone("hRawHist_pc");
    //TH1F* h3 = (TH1F*) hRawHist_pc->Clone("h3");

    TH1F* hRawHist_apc = (TH1F*) fileapc->Get("hPurity")->Clone("hRawHist_apc");
   // TH1F* h4 = (TH1F*) hRawHist_apc->Clone("h4");

 TH1F* h1 =new TH1F("h1", "h1", 40, 0., 4.);
  TH1F* h2 =new TH1F("h2", "h2", 40, 0., 4.);



for (Int_t i = 0; i < 40;i++){
h1->SetBinContent(i,hRawHist_p->GetBinContent(i));
h2->SetBinContent(i,hRawHist_ap->GetBinContent(i));
}




    hRawHist_pc->Multiply(h1);
    hRawHist_apc->Multiply(h2);


    hRawHist_apc->Divide(hRawHist_pc);
hRawHist_apc->SetTitle("Anti-Proton / Proton Corrected");
hRawHist_apc->SetStats(kFALSE);

    TCanvas *cs = new TCanvas("cs","cs");


    hRawHist_apc->Draw();

    TFile* fileout = new TFile("Hist_antip_p_Corrected.root","RECREATE");
    hRawHist_apc->Write();
    fileout->Close();

}
