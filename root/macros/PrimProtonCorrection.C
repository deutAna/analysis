

  const char* rawFile = "RawSpectrum_prot.root";
  const char* purityFile = "Purity_prot.root";


  void PrimProtonCorrection(){

    TH1F* hRawSpectrum = new TH1F("hRawSpectrum", Form("Raw spectrum %s","proton"), 45, 0, 4.5);
    hRawSpectrum->GetXaxis()->SetTitle("#it{p}, GeV/#it{c}");
    hRawSpectrum->GetYaxis()->SetTitle("counts");

    TFile* rawFile =TFile::Open(rawFile);
    TFile* purityFile = TFile::Open(purityFile);
    rawFile->ls();

  TH1F* hRaw ;
  TH1F* hPurity;
  rawFile->GetObject("hRawSpectrum",hRaw);
  purityFile->GetObject("hPurity",hPurity);

   Multiply(hRaw,hPurity,hRawSpectrum);
   WriteHistos(hRawSpectrum);



  }

  void Multiply(TH1F* h1,TH1F* h2,TH1F* h3){

    for(int i = 0;i<h1->GetNbinsX();i++){
      double dRaw = h1->GetBinContent(i);
      double dPurity = h2->GetBinContent(i);
      if(dRaw != 0){
        h3->SetBinContent(i,dRaw*dPurity);
      }
    }

  }
  void WriteHistos(TH1F* h){

      TFile* fileout = new TFile(Form("%s_%s.root","Corrected","proton"),"RECREATE");
      h->SetStats(kFALSE);
      h->SetMarkerStyle(20);
      h->SetMarkerColor(kRed);
      h->SetOption("P");
      h->Write();
      fileout->Close();

  }
