
int bins_num[23]  = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 28, 30, 35, 40};
int binlow  = 4;
int binmid  = 7;
const int* fDCABinning_arraySize = 27;

Double_t arryBins[fDCABinning_arraySize] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,-0.025,-0.05,-0.075,0.0,0.025,0.05,0.075,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0}; // should be defined more ...

//#include "Functions.C"
#include "Secondary.C"
TObjArray*  getHistograms();
void MakePrimFracDeuteron(TString Name, TH1F *primary_fraction, TH2F *DCAData);
void CalculatePrimFrac(TH1F *primary_fraction,TH1F *Primary_DCAxy,TH1F *Secondary_DCAxy,Int_t PtBin);
void MakeDCAPlot(TString Name,TString PtRangeName,TString PurityName, TH1F *ProjDCAData, TH1F *Primary_DCAxy, TH1F *Secondary_DCAxy, TH1F *result,Int_t PtBin);
void plotting(TH2F* hData_DCAxy_p ,TH2F* prihTOFm2_DCAxy_p,TH2F* hMCsecM_DCAxy_p,TH1F* mPurity  );
void LabelDCAHisto(TH1F *Histo, TString Title, EColor color);
void Scaling(TH1F* hist);


void FitDCAxyDeut(){


  TObjArray* mHistograms = getHistograms();

  TH3F* hTOFm2_DCAxy_p = mHistograms->At(0);
  TH3F* prihTOFm2_DCAxy_p = mHistograms->At(1);
  TH2F* hMC_Mat = mHistograms->At(2);



        TH1F* mPurity = new TH1F("hPurity",Form("Purity %s","Deuteron"),45,0,4.5);
        mPurity->GetXaxis()->SetTitle("#it{p}, GeV/#it{c}");
        mPurity->GetYaxis()->SetTitle("purity");
        mPurity->GetYaxis()->SetRange(0,100);

        TH2F* hData_DCAxy_p = (TH2F*)hTOFm2_DCAxy_p->Project3D("zxo")->Clone("hData_DCAxy_p");

        TH2F* hPrim_DCAxy_p = (TH2F*)prihTOFm2_DCAxy_p->Project3D("zxo")->Clone("hPrim_DCAxy_p");

   plotting(hData_DCAxy_p,hPrim_DCAxy_p,hMC_Mat,mPurity);

      cout<<hData_DCAxy_p->GetNbinsY()<<endl;//200
      cout<<hPrim_DCAxy_p->GetNbinsY()<<endl;//500
      cout<<hMC_Mat->GetNbinsY()<<endl;//1000




}
void WriteHistos(TH1F* purity){

    TFile* fileout = new TFile(Form("%s_%s.root","Purity","Deutron"),"RECREATE");
    purity->SetStats(kFALSE);
    purity->Draw("h");
    purity->Write();
    fileout->Close();

}

void plotting(TH2F* hData_DCAxy_p ,TH2F* prihTOFm2_DCAxy_p,TH2F* hMCsecM_DCAxy_p,TH1F* mPurity){


  TH1F *hData_DCAxy[45];
  TH1F *Primary_DCAxy[45];
  TH1F *Secondary_DCAxy[45];
  TH1F *results[45];
  TString PtRangeName[45];
  TString PurityDCA;



  for (int bin= 6;bin <24 ; bin++){


    mPurity->SetBinContent(bin,1);
    mPurity->SetBinError(bin,0);

        Double_t par0, errpar0, par1, errpar1, par2, errpar2;
        // yield of prim/secM/secW
        Double_t yieldPrim, yieldSecM, yieldSecW;
        Double_t weightPrim, weightSecM, weightSecW;
        int binx_low  = 0;
        int binx_high = 0;

        // canvas and QA dir
        TCanvas* c1 = new TCanvas("c1","c1");

        // projection .. rebinning ... scaling
        hData_DCAxy[bin] = (TH1F*) hData_DCAxy_p->ProjectionY("_py",bins_num[bin-6]+1,bins_num[bin-5],"oe")->Clone(Form("hData_DCAxy_bin_%i",bin));
        hData_DCAxy[bin] = (TH1F*) hData_DCAxy[bin]->Rebin(26,Form("hData_%d",bin),arryBins); // value of rebin ... first getNbinY  to see ......... check the cout first
        hData_DCAxy[bin]->Scale(0.025, "width");
       // finding intergral range and integrate
       Double_t BinlowData = hData_DCAxy[bin]->FindBin(-0.5);
       Double_t BinupData = hData_DCAxy[bin]->FindBin(0.5);
       Double_t Scaling_Intgral = (hData_DCAxy[bin]->Integral(BinlowData,BinupData));

        Primary_DCAxy[bin] = (TH1F*) prihTOFm2_DCAxy_p->ProjectionY("_py",bins_num[bin-6]+1,bins_num[bin-5],"oe")->Clone(Form("prihTOFm2_DCAxy_p_bin_%i",bin));
        Primary_DCAxy[bin] = (TH1F*) Primary_DCAxy[bin]->Rebin(26,"hPrim",arryBins);  // check the cout first
        Primary_DCAxy[bin]->Scale(0.025,"width");
        Primary_DCAxy[bin]->Scale(Scaling_Intgral/(Primary_DCAxy[bin]->Integral(BinlowData,BinupData)));


        Secondary_DCAxy[bin] = (TH1F*) hMCsecM_DCAxy_p->ProjectionY("_py",bins_num[bin-6]+1,bins_num[bin-5],"oe")->Clone(Form("hMCsecM_DCAxy_bin_%i",bin));
        Secondary_DCAxy[bin] = (TH1F*) Secondary_DCAxy[bin]->Rebin(26,"hMat",arryBins);    // check the cout first
        Secondary_DCAxy[bin]->Scale(0.025,"width");
        Secondary_DCAxy[bin]->Scale(Scaling_Intgral/(Secondary_DCAxy[bin]->Integral(BinlowData,BinupData)));




// to check the binns after rebinning and after setting the x-axis range
       cout<<hData_DCAxy[bin]->GetNbinsX()<<endl;
       cout<<Primary_DCAxy[bin]->GetNbinsX()<<endl;
       cout<<Secondary_DCAxy[bin]->GetNbinsX()<<endl;




        // MC histograms are put in this array
        TObjArray* mc = new TObjArray(2);
        mc->Add(Primary_DCAxy[bin]);
        mc->Add( Secondary_DCAxy[bin]);
        mc->ls();




        TFractionFitter* fit = new TFractionFitter(hData_DCAxy[bin], mc);


        fit->Constrain(0,0.5,1.0);
        fit->Constrain(1,0.0001,0.5);


        Int_t status = fit->Fit();               // perform the fit
        cout << "fit status: " << status << endl;


        results[bin] = 0;
        if (status == 0) {
            // obtain weights:
            fit->GetResult(0, par0, errpar0);
            fit->GetResult(1, par1, errpar1);

            Primary_DCAxy[bin]->Scale(par0);
            Secondary_DCAxy[bin]->Scale(par1);
            results[bin] = (TH1F*)fit->GetPlot();
            results[bin]->SetTitle(Form("Fit result %d",bin));
            CalculatePrimFrac(mPurity,Primary_DCAxy[bin],Secondary_DCAxy[bin],bin);
            PurityDCA = Form("Purity: %1.3f",mPurity->GetBinContent(bin));
            MakeDCAPlot("Deuteron","X",PurityDCA,hData_DCAxy[bin],Primary_DCAxy[bin],Secondary_DCAxy[bin],results[bin],bin);
        }

/*

                if (status == 0) {                       // check on fit status
                    TH1F* result = (TH1F*) fit->GetPlot();
                    // obtain weights
                    fit->GetResult(0, par0, errpar0);
                    fit->GetResult(1, par1, errpar1);
                    //fit->GetResult(2, par2, errpar2);

                    cout << "par0 = " << par0 << endl;
                    cout << "par1 = " << par1 << endl;
                    //cout << "par2 = " << par2 << endl;

                    hData_DCAxy[bin]->SetLineColor(kBlack);
                    hData_DCAxy[bin]->SetMarkerColor(kBlack);
                    hData_DCAxy[bin]->SetMarkerStyle(kFullCircle);
                    hData_DCAxy[bin]->Draw("Ep");

                    result->SetLineColor(kBlue);
                    result->Draw("same");

                    weightPrim = par0*(hData_DCAxy[bin]->Integral())/(Primary_DCAxy[bin]->Integral());
                    weightSecM = par1*(hData_DCAxy[bin]->Integral())/(Secondary_DCAxy[bin]->Integral());


                    Primary_DCAxy[bin]->Scale(weightPrim);
                    Secondary_DCAxy[bin]->Scale(weightSecM);


                    hMCsum = (TH1F*)Secondary_DCAxy[bin]->Clone();
                    hMCsum->Add(Secondary_DCAxy[bin]);


                    Primary_DCAxy[bin]->SetLineColor(kRed);
                    Primary_DCAxy[bin]->SetMarkerColor(kRed);
                    Primary_DCAxy[bin]->SetMarkerStyle(kFullCircle);
                    Primary_DCAxy[bin]->Draw("same");

                    Secondary_DCAxy[bin]->SetLineColor(kGreen);
                    Secondary_DCAxy[bin]->SetMarkerColor(kGreen);
                    Secondary_DCAxy[bin]->SetMarkerStyle(kFullCircle);
                    Secondary_DCAxy[bin]->Draw("same");



                    hMCsum->SetLineColor(kBlue+1);
                    hMCsum->SetMarkerColor(kBlue+1);
                    hMCsum->SetMarkerStyle(kFullStar);
                    hMCsum->Draw("same");

                    c1->Print(Form("DCAxyFit_%s_bin_%i.png","Deuteron",bin));

                    // extract purity
                    binx_low  = hData_DCAxy[bin]->FindBin(-0.1);
                    binx_high = hData_DCAxy[bin]->FindBin(0.1);

                    yieldPrim = Primary_DCAxy[bin]->Integral(binx_low,binx_high);
                    yieldSecM = Secondary_DCAxy[bin]->Integral(binx_low,binx_high);


                    double purity =yieldPrim/(yieldPrim+yieldSecW);
                            cout << "purity = " <<purity<< endl;
                            mPurity->SetBinContent(bin,purity);
            }
*/
  }

 WriteHistos(mPurity);

}
void MakeDCAPlot(TString Name,TString PtRangeName,TString PurityName, TH1F *ProjDCAData, TH1F *Primary_DCAxy, TH1F *Secondary_DCAxy, TH1F *result,Int_t PtBin){


    LabelDCAHisto(ProjDCAData, Form("Data %s",Name.Data()) ,kBlack);
    LabelDCAHisto(Primary_DCAxy, "Primary",kRed);
    LabelDCAHisto(Secondary_DCAxy, "Material",kGreen);

    TCanvas *DCAPlot = new TCanvas("DCAPlot","DCAPlot",0,0,800,600);
    TString pdfDCAName = Form("%s/DCAFit/%s_DCAFit_Bin_%d.pdf",Name.Data(),Name.Data(),PtBin);
    TLatex PtBinRange;
    gPad->SetLogy();

    //gStyle->SetOptStat("nic");
    gStyle->SetOptFit(1111);
    gStyle->SetOptTitle(0);
    gStyle->SetStatW(0.2);
    gStyle->SetStatH(0.1);

    ProjDCAData->Draw("Ep");
    Secondary_DCAxy->DrawCopy("same");
    Primary_DCAxy->DrawCopy("same");
    if(result){
        result->SetLineColor(kBlue);
        result->SetLineWidth(2);
        result->DrawCopy("same");
    }
    gPad->BuildLegend(0.35,0.9,0.15,0.7);
    PtBinRange.SetNDC(kTRUE);
    PtBinRange.SetTextSize(0.03);
    //PtBinRange.DrawLatex(.15,.62,PtRangeName);
    //PtBinRange.DrawLatex(.15,.55,PurityName);

    //DCAPlot->Print(pdfDCAName);
      DCAPlot->Print(Form("DCAxyFit_%s_bin_%i.png","Deuteron",PtBin));
    delete DCAPlot;
    return;
}

void CalculatePrimFrac(TH1F *primary_fraction,TH1F *Primary_DCAxy,TH1F *Secondary_DCAxy,Int_t PtBin){

    //Find DCA integration range
    Double_t Binlow = Primary_DCAxy->FindBin(-1);
    Double_t Binup = Primary_DCAxy->FindBin(1);

    //Calculate Integral and respective Error
    Double_t ErrorPrim;
    Double_t ErrorSec;
    Double_t ErrorMat;
    Double_t Integral_primary = Primary_DCAxy->IntegralAndError(Binlow,Binup,ErrorPrim);
    Double_t Integral_secondary = Secondary_DCAxy->IntegralAndError(Binlow,Binup,ErrorSec);
    Double_t RelError = 0;
    if(Integral_secondary>0)RelError = TMath::Sqrt( (ErrorPrim/Integral_primary)*(ErrorPrim/Integral_primary) + (ErrorPrim/(Integral_primary+Integral_secondary))*(ErrorPrim/(Integral_primary+Integral_secondary)) );

    //Calculate the primary fraction with error
    Double_t Val_PrimFraction = 1;
    if(Integral_secondary>0)Val_PrimFraction = Integral_primary/(Integral_primary+Integral_secondary);
    primary_fraction->SetBinContent(PtBin,Val_PrimFraction);
    primary_fraction->SetBinError(PtBin,Val_PrimFraction*RelError);
    return;
}

void Scaling(TH1F* hist){
  hist->Scale(hist->Integral(),"width");
}


TObjArray* getHistograms(){

  TH3F* hTOFm2_DCAxy_p;
  TH3F* prihTOFm2_DCAxy_p;
  TH2F* hMC_Mat;

  TObjArray *histo = new TObjArray(3);
  TDirectoryFile* dir;

  TList* mainListMC;
  TList* subListMC;

  TFile* dataFile = TFile::Open("High_Mult_TOF_PID_LHC16_k_l_o_p.root");

  hTOFm2_DCAxy_p = (TH3F*) dataFile->Get("Output_highMult_TOFpid")->FindObject("Protons")->FindObject("fHistTOFmass2_DCAxy_p")->Clone("hTOFm2_DCAxy_p");
  hTOFm2_DCAxy_p->Sumw2();

  histo->Add(hTOFm2_DCAxy_p);

  prihTOFm2_DCAxy_p = (TH3F*) dataFile->Get("Output_highMult_TOFpid")->FindObject("ADeuterons")->FindObject("fHistTOFmass2_DCAxy_p")->Clone("prihTOFm2_DCAxy_p");
  prihTOFm2_DCAxy_p->Sumw2();
  //prihTOFm2_DCAxy_p->RebinY(5);
  histo->Add(prihTOFm2_DCAxy_p);



  TFile* mcFile = TFile::Open("LHC18f3_cent_2_MC_p-pb.root");
  mcFile->GetObject("DeuteronTrkCutsMC",dir);
  dir->GetObject("DeuteronTrkCutsMC",mainListMC);
  subListMC = (TList*)  mainListMC->FindObject("DCAPtBinning");
  hMC_Mat = (TH2F*) subListMC->FindObject("DCAPtBinningMat");
  hMC_Mat->RebinY(5);
  histo->Add(hMC_Mat);




  return histo;
}

void count(TH1F* h){
  for(int i = 0;i<h->GetNbinsX();i++){

    double d = h->GetBinContent(i);
    if(d != 0){

      cout<<"************ At I : "<<i<<"  ***** Bin Value is : "<<d<<" ********"<<endl;
    }

  }
}
