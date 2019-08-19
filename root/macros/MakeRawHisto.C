void MakeRawHisto(){


     TFile* file1 = new TFile("RawSpectrum_aprot.root"); //
     TFile* file2 = new TFile("RawSpectrum_prot.root"); //


    TH1F* h1  = (TH1F*) file1->Get("hRawSpectrum")->Clone("h1"); //
    TH1F* h2 = (TH1F*) file2->Get("hRawSpectrum")->Clone("h2"); //



    h1->SetLineColor(kBlue);
    h2->SetLineColor(kRed);
    h1->SetStats(kFALSE);
    h2->SetStats(kFALSE);
    TCanvas *cs1 = new TCanvas("cs1","cs1");
    TLegend *legend = new TLegend(0.9,0.6,0.6,0.7);// (x1,y1,x2,y2)x1->> 1st parameter

    legend->SetHeader("Raw Spectrum");
    legend->AddEntry(h1,"Ap","l");
    legend->AddEntry(h2,"p","l");




    h1->Draw("e1p"); //"e1"

     cs1->Update();

     h2->Draw("e1p same"); //"e1 same"

    TFile* fileout = new TFile("RawSpectrum_antip_p.root","RECREATE");
    h1->Write();
    h2->Write();
    legend->Draw();
    fileout->Close();

}

// the red histogram - proton is the one drawn
// the blue one is not drawn
// set the title to: Raw Spectrum AP - P
