
void StackPurityADD(){


    TFile* filed = new TFile("RawSpectrum_FitLoose_deut.root");
    TFile* filead  = new TFile("RawSpectrum_FitLoose_adeut.root");


    TH1F* hRawHist_d  = (TH1F*) filed->Get("hRawSpectrum")->Clone("hRawHist_d");
    TH1F* h1 = (TH1F*) hRawHist_d->Clone("h1");

    TH1F* hRawHist_ad = (TH1F*) filead->Get("hRawSpectrum")->Clone("hRawHist_ad");
    TH1F* h2 = (TH1F*) hRawHist_ad->Clone("h2");
    gROOT->ForceStyle();
    h1->SetStats(kFALSE);
    h2->SetStats(kFALSE);
    h1->SetLineColor(kRed);

    h2->SetLineColor(kBlue);




    TCanvas *cs = new TCanvas("cs","cs");
    TLegend *legend = new TLegend(0.9,0.9,0.6,0.7);// (x1,y1,x2,y2)x1->> 1st parameter  -> left from 0.0 to 0.49 and Right from 0.51 to 0.99
    //y1 ->>> the same as x1
    // x2 , y2 is the sie of box
    legend->SetHeader("Fraction of Primary"); // option "C" allows to center the header
    legend->AddEntry(h1,"Deuterons","l");
    legend->AddEntry(h2,"Anti-Deuterons","l");


    h1->Draw("C l");

    cs->Update();
    h2->Draw("C l same");
    cs->Update();

    legend->Draw();

    TFile* fileout = new TFile("Hist_antid_d.root","RECREATE");
    cs->Write();
    fileout->Close();

}
