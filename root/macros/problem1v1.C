
{

 TFile *MyFile = new TFile("testcorrelations1.root","RECREATE");

 if ( MyFile->IsOpen() ) std::cout << "File opened successfully" << endl;

 else {
   std::cout << "Error opening file"; exit(0);
 }

   TProfile *hprofile = new TProfile ("hprofile",0.,1.);

// fill some events with random numbers
  Int_t nevent=10000;
  for (Int_t iev=0;iev<nevent;iev++) {
    if (iev%1000==0) cout<<"Processing event "<<iev<<"..."<<endl;

 Double_t epsi2 = gRandom->Uniform(0.,TMath::TwoPi());

  TF1 *phi = new TF1("phi","(1/2*pi)*(1+2*[v2]*cos(2*(phi-[epsi2])))",0.,2*pi);

  phi->SetParameter(0,0.05);
  phi->SetParameter(epsi2);

  TH1F *h = new TH1F("h", "Fourier", 10, 0., 2*pi); // check


  //f(phi) = (1/2*pi) * (1+2*[v2]*cos(2*(phi-[epsi2])))

  for (Int_t i=0;i<500;i++) {h->Fill(phi->GetRandom());} //



   for (Int_t i=0, i<500; i++) { // average over the 500 particles in v2 equation

     x = cos*(2*(phi-epsi2));

if ( i == 250){
  std::cout<<"value of i is  " <<i<<endl;
}
     hprofile->Fill(0.5,x);
   }


  }

  hprofile->GetBinContent(1);

 }
