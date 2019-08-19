
// Save the figure in pdf, eps and png formats:
Bool_t bSaveFigures = kTRUE;


void  MakeAppRatio(){
gStyle->SetLegendBorderSize(0);


    TFile* file1 = new TFile("ratio_antip_p_stat_syst.root"); // priliminary
    TFile* file2  = new TFile("Hist_antip_p_Corrected.root"); // my result

    TH1F* h1  = (TH1F*)file1->Get("hstat")->Clone("h1"); //hstat: antip_p_stat
    TH1F* h12 = (TH1F*)file1->Get("hsyst")->Clone("h12"); //hsyst: antip_p_syst
    TH1F* h2 = (TH1F*)file2->Get("hRawHist_apc")->Clone("h2"); //h2: antip_p

   h1->SetTitle("Raw Primary Spectrum Anti-p / p ");
   h1->SetLineColor(kBlue);
   h12->SetLineColor(kGreen);
   h2->SetLineColor(kRed);

    TCanvas *cs = new TCanvas("cs", "cs");
    TLegend *legend = new TLegend(0.9,0.5,0.6,0.65); //(0.9,0.6,0.6,0.7);
    // Set title offsets:
     h1->GetXaxis()->SetTitleOffset(0.98844);
     h1->GetYaxis()->SetTitleOffset(0.9); //h->GetYaxis()->SetTitleOffset(0.744);
     h1->Draw();
     cs->Update();
     h12->Draw("same");
     cs->Update();
     h2->Draw("same");

     legend->SetHeader("Raw Primary Spectrum");
     legend->AddEntry(h1,"stat");
     legend->AddEntry(h12,"syst");
     legend->AddEntry(h2,"ratio_antip_p");


    TFile* fileout = new TFile("RatioCompare_antip_p.root","RECREATE");
    h1->Write();
    h12->Write();
    h2->Write();
    legend->Draw();
    fileout->Close();

  if(bSaveFigures)
  {
  //cfig->SaveAs("ToyMC_SC42_const_2018-10-23.pdf");
  //cfig->SaveAs("ToyMC_SC42_const_2018-10-23.eps");
  cs->SaveAs("RatioCompare_antip_p.png");
  } // if(bSaveFigures)


}
