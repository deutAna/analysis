
// Save the figure in pdf, eps and png formats:
Bool_t bSaveFigures = kTRUE;
gStyle->SetLegendBorderSize(0);

void MakeFinalRatio(){


     TFile* file1 = new TFile("ratio_antid_d_stat_syst.root"); //
     TFile* file2 = new TFile("Ratio_antid_d.root"); //


    TH1F* h1  = (TH1F*)file1->Get("hstat")->Clone("h1"); //hstat: antid_d_stat
    TH1F* h12 = (TH1F*)file1->Get("hsyst")->Clone("h12"); //hsyst: antid_d_syst
    TH1F* h2 = (TH1F*)file2->Get("ratio_antid_d")->Clone("h2"); //h2: antid_d

   h1->SetLineColor(kBlue);
   h12->SetLineColor(kGreen);
   h2->SetLineColor(kRed);

    TCanvas *cfig = new TCanvas("cfig", "Alice Figure Template");



   // Set title offsets:
    h1->GetXaxis()->SetTitleOffset(0.98844);
    h1->GetYaxis()->SetTitleOffset(1.1); //h->GetYaxis()->SetTitleOffset(0.744);

    TLegend *legend = new TLegend(0.9,0.6,0.6,0.4);// (x1,y1,x2,y2)x1->> 1st parameter
legend->SetFillStyle(0);
    legend->SetHeader("Raw Primary Spectrum");
    legend->AddEntry(h1,"stat","l");
    legend->AddEntry(h12,"syst","l");
    legend->AddEntry(h2,"ratio_antid_d","l");

     h1->Draw("e1");
     cfig->Update();
     h12->Draw("e1 same");
     cfig->Update();
     h2->Draw("e1 same");

    TFile* fileout = new TFile("RatioCompare_antid_d.root","RECREATE");
    h1->Write();
    h12->Write();
    h2->Write();
    legend->Draw("same");


  if(bSaveFigures)
  {
  //cfig->SaveAs("ToyMC_SC42_const_2018-10-23.pdf");
  //cfig->SaveAs("ToyMC_SC42_const_2018-10-23.eps");
  cfig->SaveAs("RatioCompare_antid_d.png");
  } // if(bSaveFigures)
fileout->Close();
}
