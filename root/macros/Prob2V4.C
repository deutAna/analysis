
{

TFile *MyFile = new TFile("test2_logscale.root","RECREATE");

 if ( MyFile->IsOpen() ) cout << "File opened successfully" << endl;



 else {
   std::cout << "Error opening file"; exit(0);
 }

  TF1 *h1 = new TF1("h1","[0]*x*exp(-1*[1]*x)",0,1);

  h1->SetParameter(0,0.1);
  h1->SetParameter(1,0.4);


  TProfile *h2 = new TProfile("h2", "P.D.F.", 10, 0., 1.);

  h2->SetDirectory(0);

  for (int i=0;i<50000;i++)
    {
      h2->Fill(gRandom->Uniform(1.),h1->GetRandom());
    }



  // for (int i_bin = 0; i_bin < 10; i_bin++){
  // Double_t mm =  h2->GetBinError(i_bin);
  // cout <<"BinContent: " << mm << endl;
  // }

  Double_t me =0;
  Double_t mm =0;

  for (Int_t i_bin = 0; i_bin < 10; i_bin++){

    me =  h2->GetBinContent(i_bin);
    mm = h2->GetBinError(i_bin);
     printf("%.2d %.8f %.8f\n", h2->GetBinCenter(i_bin), me, mm);


    cout<<"BinContent:"<<h2->GetBinContent(i_bin)<<endl;
    cout<<"BinError:"<<h2->GetBinError(i_bin)<<endl;

    // cout <<"BinError: " << me << endl;
    // cout <<i_bin<< " " << me << " " << mm << " " << endl;
    // cout << Form("%d %.8f %.8f", i_bin, me, mm) << endl;


  }


   h2->GetXaxis()->SetTitle("X");
   h2->GetYaxis()->SetTitle("Y");
   h2->SetLineColor(kRed);
   h2->SetMarkerStyle(kFullSquare);

   TCanvas *c1 = new TCanvas("c1","c1", 2000, 700);
   // c1->SetLogx(1);
   // c1->SetLogy(1);

   c1->Divide(2,1);

   c1->cd(1);
   gPad->SetLogx(1);
   gPad->SetLogy(1);
   h1->Draw();
   h1->Write();

   c1->cd(2);
   gPad->SetLogx(1);
   gPad->SetLogy(1);
   h2->Draw();
   h2->Write();

   MyFile->Print();
   MyFile->Close();


}
