/*class  MyFunctionObject {
 public:
   // use constructor to customize your function object
   double operator() (double *x, double *p) {
      // function implementation using class data members
   }
};
*/


{
/*  void settingUp(Double_t eps){
  phi->SetParameter(1,eps);
}*/
 //MyFunctionObject *fobj = new MyFunctionObject();
 //TF1 * f = new TF1("f",fobj,0,1,1,"MyFunctionObject");
 TFile *MyFile = new TFile("testcorrelations1.root","RECREATE");

 if ( MyFile->IsOpen() ){ cout << "File opened successfully" << endl;}
else {
   std::cout << "Error opening file"; exit(0);
 }
  //TCanvas *c1 = new TCanvas("c1","Profile histogram example",200,10,700,500);

   TProfile *hprofile = new TProfile("hprofile","hprofile",1,0.,1.);

   TH1F *hist = new TH1F("hist", "Fourier", 10, 0.,TMath::TwoPi());

   TF1 *phi = new TF1("phi","(1./(TMath::TwoPi()))*(1.+2.*[0]*cos(2.*(x-[1])))",0.,TMath::TwoPi());

   phi->SetParameter(0,0.05);

// fill some events with random numbers
  Int_t nevent=10000;
  for (Int_t iev=0;iev<nevent;iev++) {
    //if (iev%1000==0) std::cout<<"Processing event"<<iev<<"..."<<endl;

    if (iev%1000==0) std::cout<<Form("Processing event: %d",iev)<<endl;
    // %d is the placeholder for integer variables (like in your case for 'iev'), %f for floats, %s, for strings, etc.

 Double_t epsi2 = gRandom->Uniform(0.,TMath::TwoPi()); //random orientation of epsi2 in each event

 //hprofile->Fill();


   // it's phi, the second parameter of TF1
  //  settingUp(epsi2);
  phi->SetParameter(1,epsi2);

  //f(phi) = (1/2*pi) * (1+2*[v2]*cos(2*(phi-[epsi2])))


  for (Int_t i=0;i<500;i++)
  {
  Double_t angl = phi->GetRandom(); // sampling angle according  to fourier series equation 4

  hist->Fill(angl); //sampling in each event 500 particles

     Double_t x = cos(2*(angl-epsi2));

     hprofile->Fill(0.5,x);
   }

  }
  std::cout<<"Vn Value = "<<hprofile->GetBinContent(1)<<endl;
  //hprofile->GetBinContent(1);
  //hprofile->GetBinError(1);
  std::cout<<"Vn Error = "<<hprofile->GetBinError(1)<<endl;

  hprofile->Draw();


 }
