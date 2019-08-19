void checkFile(TFile *tfile);
{
  TFile *MyFile = new TFile("testcorrelations1.root","RECREATE");
   checkFile(*MyFile);

  TProfile *hprofile = new TProfile("hprofile","hprofile",1,0.,1.);

  TH1F *hist = new TH1F("hist", "Fourier", 10, 0.,TMath::TwoPi());

  TF1 *phi = new TF1("phi","(1./(TMath::TwoPi()))*(1.+2.*[0]*cos(2.*(x-[1])))",0.,TMath::TwoPi()); //fourier equation number 4

  phi->SetParameter(0,0.05); //v2

  Int_t nevent=10000;
  for (Int_t iev=0;iev<nevent;iev++) {

    if (iev%1000==0) std::cout<<Form("Processing event: %d",iev)<<endl;
    Double_t epsi2 = gRandom->Uniform(0.,TMath::TwoPi()); //random orientation of epsi2 in each event
    phi->SetParameter(1,epsi2); //epsi2 from equation 4 of fourier


// need to look for the two nested loops to calculate all possible values of phi1 and phi2

int N = 500;
Double_t angle[N];
Double_t xSqr = 0 ;
for (Int_t i=0;i<N;i++)
{
angle[i]=phi->GetRandom(); // sampling angle according  to fourier series equation 4

}
for (Int_t j = 0;j<N;j++)
{
  for(Int_t k = j+1;k<N;k++)
  {
    Double_t phi1 = angle[j];
    hist->Fill(phi1);  //sampling in each event 500 particles
    Double_t phi2 = angle[k];
    hist->Fill(phi2);  //sampling in each event 500 particles
    //xSqr += cos(2*(phi1-phi2)); //change the equation  eq no 12
  }
//Double_t as =abs(xSqr);
//std::cout<<xSqr<<endl;
//Double_t x = sqrt(xSqr);
hprofile->Fill(0.5,x);
}
}

std::cout<<xSqr<<endl;
std::cout<<"Vn Value = "<<hprofile->GetBinContent(1)<<endl;
std::cout<<"Vn Error = "<<hprofile->GetBinError(1)<<endl;
hprofile->Draw();
void checkFile(TFile*  &tfile){

    if ( tfile->IsOpen() ) cout << "File opened successfully" << endl;

    else {
      std::cout << "Error opening file"; exit(0);
    }

}

}
