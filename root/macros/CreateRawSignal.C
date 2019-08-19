
float xmin = 2.0;
float xmax = 5.0;
// ITSnSigma cut range
float ITSmin = -2.0;
float ITSmax = 10.;
// DCA cut
float DCAlow  = -0.1;
float DCAhigh =  0.1;
// momentum bins to extract raw signal
int binlow  = 6;  // first bin: 0.5 to 0.6
int binmid  = 15; // first bin with TOF: 1.4 to 1.5

// int binhigh = 35; // last bin: 3.4 to 3.5
// do projections in momentum bins from high to low p
bool backward = false;

// binning of the raw spectra
// be consistent between bin numbers and momentum ranges
static const int   nbins_highp = 13;
static const int   nbins_lowp  = 14;
int   binsx_n[nbins_highp+1]  = {14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 28, 30, 35, 40};
float binsx_p[nbins_highp+nbins_lowp+1] = {0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.2,2.4,2.6,2.8,3.0,3.5,4.0};

const char* file = "LHC16_k_l_o_p_He3_V2_20190718.root";
const char* out = "Output_highMult";
const char* list = "Deuterons";
const char* histogram = "fHistTOFmass2_DCAxy_p";
const char* sigmaITS = "fHistITSnSigmaDeut";


void FillSpectrumLowP(TH3F* hist, TH1F* rawSpectrum);

void WriteHistos(TH1F* spectrum);


void CreateRawSignal(){


  TH1F* hRawSpectrum = new TH1F("hRawSpectrum", Form("Raw spectrum %s",list), nbins_lowp+nbins_highp, binsx_p);
  hRawSpectrum->GetXaxis()->SetTitle("#it{p}, GeV/#it{c}");
  hRawSpectrum->GetYaxis()->SetTitle("counts");


  TFile *file = new TFile(file);

  TH3F *hTOFmass2_DCAxy_p = (TH3F*)file->Get(out)->FindObject(list)->FindObject(histogram)->Clone("hTOFmass2_DCAxy_p");
  hTOFmass2_DCAxy_p->Sumw2();
  //hTOFmass2_DCAxy_p->Draw();
  TH2F* hITSnSigmaDeut_p = (TH2F*)file->Get(out)->FindObject(list)->FindObject(sigmaITS)->Clone("hITSnSigmaDeut_p");

  FillSpectrumLowP(hITSnSigmaDeut_p,hRawSpectrum);



  WriteHistos(hRawSpectrum);
} // end of method CreateRawSignal



void FillSpectrumLowP(TH2F* hist, TH1F* rawSpectrum){

    // cut on ITSnSigma
    hist->GetYaxis()->SetRangeUser(ITSmin, ITSmax);
    TH1F* h_p = (TH1F*)hist->ProjectionX("oe")->Clone("h_p");
    std::cout<<"----------NBins 2D-----------"<<std::endl;
    std::cout<<hist->GetNbinsX()<<std::endl;

  for(Int_t i = 0; i <100;++i){
    double d =  hist->GetBinContent(i);
    if (d !=0.0){

      std::cout<<"*****"<<d<<"****** At  : "<<i<<std::endl;
    }

  }

    for (Int_t i = binlow; i< binmid; i++){
        rawSpectrum->SetBinContent(i, h_p->GetBinContent(i));
        rawSpectrum->SetBinError(i, h_p->GetBinError(i));
    }

}


void WriteHistos(TH1F* hist){

    TFile* fileout = new TFile(Form("%s_%s.root",out,list),"RECREATE");
    hist->Write();
    fileout->Close();

}
