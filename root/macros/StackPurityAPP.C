
void StackPurityAPP(){


    TFile* filep = new TFile("RawSpectrum_prot.root");
    TFile* fileap  = new TFile("RawSpectrum_aprot.root");


    TH1F* hRawHist_p  = (TH1F*) filep->Get("hRawSpectrum")->Clone("hRawHist_p");
    TH1F* h1 = (TH1F*) hRawHist_p->Clone("h1");

    TH1F* hRawHist_ap = (TH1F*) fileap->Get("hRawSpectrum")->Clone("hRawHist_ap");
    TH1F* h2 = (TH1F*) hRawHist_ap->Clone("h2");
    gROOT->ForceStyle();
    h1->SetLineColor(kRed);

    h2->SetLineColor(kBlue);




    TCanvas *cs = new TCanvas("cs","cs");
    TLegend *legend = new TLegend(0.9,0.9,0.6,0.7);// (x1,y1,x2,y2)x1->> 1st parameter  -> left from 0.0 to 0.49 and Right from 0.51 to 0.99
    //y1 ->>> the same as x1
    // x2 , y2 is the sie of box
    legend->SetHeader("Fraction of Primary"); // option "C" allows to center the header
    legend->AddEntry(h1,"Protons","l");
    legend->AddEntry(h2,"Anti-Proton","l");

    h1->SetStats(kFALSE);
    h1->Draw("");
    



    h2->Draw("same");




//cs->Update();

legend->Draw();

       TFile* fileout = new TFile("Hist_antip_p.root","RECREATE");
       cs->Write();
       fileout->Close();

}
