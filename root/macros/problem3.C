{

 TFile *MyFile = new TFile("test.root","RECREATE");

 if ( MyFile->IsOpen() ) cout << "File opened successfully" << endl;

 else {
   std::cout << "Error opening file"; exit(0);
 }

  TF1 *fa1 = new TF1("fa1","[0]*x*exp(-1*[1]*x)",0,1);

  fa1->SetParameter(0,0.1);
  fa1->SetParameter(1,0.4);


   TH1F * h = new TH1F("h", "Exponential", 1000, 0., 1.);

   h->SetDirectory(0);

   for (Int_t i=0;i<50000;i++) {h->Fill(fa1->GetRandom());}

    Double_t mm = h->GetMean();
   cout <<"mean: " << mm << endl;


   TH1F *hnew = dynamic_cast<TH1F*>(h->Rebin(100,"hnew"));

   hnew->SetDirectory(0);

   hnew->SetLineColor(2);
   hnew->SetMarkerStyle(25);

   TCanvas *c1 = new TCanvas("c1","c1", 2000, 700);
   c1->Divide(3,1);


   c1->cd(1);
   fa1->Draw();
   fa1->Write();


   c1->cd(2);
   h->Draw();
   h->Write();

  c1->cd(3);
  hnew->Draw();
  hnew->Write();

  MyFile->Print();
  MyFile->Close();



  }
