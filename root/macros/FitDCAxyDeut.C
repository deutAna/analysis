
int bins_num[23]  = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 28, 30, 35, 40};
int binlow  = 4;
int binmid  = 7;
//#include "Functions.C"

TObjArray*  getHistograms();
void MakePrimFracDeuteron(TString Name, TH1F *primary_fraction, TH2F *DCAData);
void MakeDCAPlot(TString Name,TString PtRangeName, TH1F *ProjDCAData, TF1 *Signal, TF1 *Background, Int_t PtBin);
void DCAFit(TH1F* histo,Int_t PtBin,Int_t ParticleType,TF1 *Signal,TF1 *BackGr,Double_t *ValueErrorArray);
void plotting(TH2F* hData_DCAxy_p ,TH1F* hMCprim_DCAxy,TH2F* hMCsecM_DCAxy_p,TH1F* mPurity);
void LabelDCAHisto(TH1F *Histo, TString Title, EColor color);
void FitDCAxyDeut(){


  TObjArray* mHistograms = getHistograms();

  TH3F* hTOFm2_DCAxy_p = mHistograms->At(0);
  TH1F* hMC_prim = mHistograms->At(1);
  TH2F* hMC_Mat = mHistograms->At(2);

hMC_prim->Rebin(4);
        TH1F* mPurity = new TH1F("hPurity",Form("Purity %s","Deuteron"),45,0,4.5);
        mPurity->GetXaxis()->SetTitle("#it{p}, GeV/#it{c}");
        mPurity->GetYaxis()->SetTitle("purity");
        mPurity->GetYaxis()->SetRange(0,100);

        TH2F* hData_DCAxy_p = (TH2F*)hTOFm2_DCAxy_p->Project3D("zxo")->Clone("hData_DCAxy_p");
      MakePrimFracDeuteron("Deuteron",hMC_prim,hData_DCAxy_p);
      //plotting(hData_DCAxy_p,hMC_prim,hMC_Mat,mPurity);



}


void plotting(TH2F* hData_DCAxy_p ,TH1F* hMCprim_DCAxy,TH2F* hMCsecM_DCAxy_p,TH1F* mPurity){
  for (int bin= 6;bin < 28 ; bin++){

        Double_t par0, errpar0, par1, errpar1, par2, errpar2;
        // yield of prim/secM/secW
        Double_t yieldPrim, yieldSecM, yieldSecW;
        Double_t weightPrim, weightSecM, weightSecW;
        int binx_low  = 0;
        int binx_high = 0;

        // canvas and QA dir
        TCanvas* c1 = new TCanvas("c1","c1");


        // data
        hData_DCAxy = (TH1F*) hData_DCAxy_p->ProjectionY("_py",bins_num[bin-6]+1,bins_num[bin-5],"oe")->Clone(Form("hData_DCAxy_ptbin_%i",bin));
        hMCsecM_DCAxy = (TH1F*) hMCsecM_DCAxy_p->ProjectionY("_py",bins_num[bin-6]+1,bins_num[bin-5],"oe")->Clone(Form("hMCsecM_DCAxy_ptbin_%i",bin));
        hData_DCAxy->Rebin(2);
        hMCsecM_DCAxy->Rebin(5);


       cout<<hData_DCAxy->GetNbinsX()<<endl;
       cout<<hMCsecM_DCAxy->GetNbinsX()<<endl;
       cout<<hMCprim_DCAxy->GetNbinsX()<<endl;

        // MC histograms are put in this array
        TObjArray *mc = new TObjArray(2);
        mc->Add(hMCprim_DCAxy);
        mc->Add(hMCsecM_DCAxy);
        mc->ls();


        TFractionFitter* fit = new TFractionFitter(hData_DCAxy, mc);

        fit->Constrain(1,-0.01,0.01);
        fit->Constrain(2,-0.01,0.01);


        Int_t status = fit->Fit();               // perform the fit
        cout << "fit status: " << status << endl;


                if (status == 0) {                       // check on fit status
                    TH1F* result = (TH1F*) fit->GetPlot();
                    // obtain weights
                    fit->GetResult(0, par0, errpar0);
                    fit->GetResult(1, par1, errpar1);
                    fit->GetResult(2, par2, errpar2);

                    cout << "par0 = " << par0 << endl;
                    cout << "par1 = " << par1 << endl;
                    cout << "par2 = " << par2 << endl;

                    hData_DCAxy->SetLineColor(kBlack);
                    hData_DCAxy->SetMarkerColor(kBlack);
                    hData_DCAxy->SetMarkerStyle(kFullCircle);
                    hData_DCAxy->Draw("Ep");

                    result->SetLineColor(kBlue);
                    result->Draw("same");

                    weightPrim = par0*(hData_DCAxy->Integral())/(hMCprim_DCAxy->Integral());
                    weightSecM = par1*(hData_DCAxy->Integral())/(hMCsecM_DCAxy->Integral());


                    hMCprim_DCAxy->Scale(weightPrim);
                    hMCsecM_DCAxy->Scale(weightSecM);


                    hMCsum = (TH1F*)hMCprim_DCAxy->Clone();
                    hMCsum->Add(hMCsecM_DCAxy);


                    hMCprim_DCAxy->SetLineColor(kRed);
                    hMCprim_DCAxy->SetMarkerColor(kRed);
                    hMCprim_DCAxy->SetMarkerStyle(kFullCircle);
                    hMCprim_DCAxy->Draw("same");

                    hMCsecM_DCAxy->SetLineColor(kGreen);
                    hMCsecM_DCAxy->SetMarkerColor(kGreen);
                    hMCsecM_DCAxy->SetMarkerStyle(kFullCircle);
                    hMCsecM_DCAxy->Draw("same");



                    hMCsum->SetLineColor(kBlue+1);
                    hMCsum->SetMarkerColor(kBlue+1);
                    hMCsum->SetMarkerStyle(kFullStar);
                    hMCsum->Draw("same");

                    c1->Print(Form("DCAxyFit_%s_bin_%i.png","Deuteron",bin));

                    // extract purity
                    binx_low  = hData_DCAxy->FindBin(-0.1+0.0001);
                    binx_high = hData_DCAxy->FindBin(0.1-0.0001);

                    yieldPrim = hMCprim_DCAxy->Integral(binx_low,binx_high);
                    yieldSecM = hMCsecM_DCAxy->Integral(binx_low,binx_high);


                    double purity =yieldPrim/(yieldPrim+yieldSecW);
                            cout << "purity = " <<purity<< endl;
                            mPurity->SetBinContent(bin,purity);
            }


  }



}

void MakePrimFracDeuteron(TString Name, TH1F *primary_fraction, TH2F *DCAData){

    TH1F *ProjDCAData[45];
    TH1F *ProjPrimMC[45];
    TF1 *DCA_Signal[45];
    TF1 *DCA_Background[45];
    TString PurityDCAName[45];
    TString PtRangeName[45];
    gSystem->Exec(Form("mkdir -p %s_%s",Name.Data(),"fit"));
    int bins_num[23]  = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 28, 30, 35, 40};

    for (Int_t PtBin=6; PtBin<28; PtBin++) {

        std::cout << "*******************************" << std::endl;
        std::cout << "DCA fit of PtBin: " << PtBin << std::endl;
        std::cout << Name.Data() << std::endl;
        std::cout << "*******************************" << std::endl;

        Double_t BinLowerEdge = DCAData->GetXaxis()->GetBinLowEdge(PtBin);
        Double_t BinUpperEdge = DCAData->GetXaxis()->GetBinLowEdge(PtBin+1);
        PtRangeName[PtBin] = Form("%1.2f< #it{p} <%1.2f",BinLowerEdge,BinUpperEdge);

TH1F* h = (TH1F*)(DCAData->ProjectionY(Form("DCABin %d %s",PtBin,Name.Data()),bins_num[PtBin-6]+1,bins_num[PtBin-5],"e"));
         h->Rebin(2);
        //Data
        ProjDCAData[PtBin] = h;
        Double_t BinlowData = ProjDCAData[PtBin]->FindBin(-0.5);
        Double_t BinupData = ProjDCAData[PtBin]->FindBin(0.5);
        Double_t Scaling_Intgral = (ProjDCAData[PtBin]->Integral(BinlowData,BinupData));

        Double_t PrimaryFractionErrorArray[2] = {0,0};
        DCA_Signal[PtBin] = new TF1("Signal",DCAGauss,-2.,2.,6);
        DCA_Background[PtBin] = new TF1("BackGr",DCAQuadraticFunc,-2.,2.,3);

        if(PtBin>14){
            primary_fraction->SetBinContent(PtBin,1);
        }else{
          cout<<"before fit"<<endl;
            DCAFit(ProjDCAData[PtBin],PtBin,1,DCA_Signal[PtBin],DCA_Background[PtBin],PrimaryFractionErrorArray);

            primary_fraction->SetBinContent(PtBin,PrimaryFractionErrorArray[0]);
            primary_fraction->SetBinError(PtBin,PrimaryFractionErrorArray[1]);
            PurityDCAName[PtBin] = Form("Purity: %1.3f",PrimaryFractionErrorArray[0]);
        }

        MakeDCAPlot(Name.Data(),PtRangeName[PtBin],ProjDCAData[PtBin], DCA_Signal[PtBin], DCA_Background[PtBin], PtBin);

    }
}


void MakeDCAPlot(TString Name,TString PtRangeName, TH1F *ProjDCAData, TF1 *Signal, TF1 *Background, Int_t PtBin){

    LabelDCAHisto(ProjDCAData, Form("Data %s",Name.Data()) ,kBlack);

    TCanvas *DCAPlot = new TCanvas("DCAPlot","DCAPlot",0,0,800,600);
    TString pdfDCAName = Form("%s/DCAFit/%s_DCAFit_Bin_%d.png",Name.Data(),Name.Data(),PtBin);
    TLatex PtBinRange;
    gPad->SetLogy();
    gStyle->SetOptStat("nic");
    gStyle->SetOptFit(1100);
    gStyle->SetOptTitle(0);
    gStyle->SetStatW(0.1);
    gStyle->SetStatH(0.1);
  cout<<"MakePlot at draw "<<endl;
    ProjDCAData->DrawCopy("Ep");
    Signal->DrawCopy("lsame");
    Background->DrawCopy("lsame");
    gPad->BuildLegend(0.35,0.9,0.15,0.7);
    PtBinRange.SetNDC(kTRUE);
    PtBinRange.SetTextSize(0.035);
    PtBinRange.DrawLatex(.15,.6,PtRangeName);

    DCAPlot->Print(pdfDCAName);
    delete DCAPlot;

    cout<<"MakePlot is done "<<endl;
    return;
}

void DCAFit(TH1F* histo,Int_t PtBin,Int_t ParticleType,TF1 *Signal,TF1 *BackGr,Double_t *ValueErrorArray)
{
    TF1 *DCAxyFit = new TF1("DCAxyFit",DCAFitFunction,-2.0,2.0,9);
    DCAxyFit->SetLineColor(kBlue+2);

    if(PtBin==5){
        DCAxyFit->SetParameters(-30,-1,20,200,0,0.08,700,0,0.02);
        DCAxyFit->SetParLimits(4,-0.1,0.1);
        DCAxyFit->SetParLimits(7,-0.1,0.1);
    }
    DCAxyFit->SetParameters(-10,-1,100,1500,0,0.1,500,0,0.05);
    DCAxyFit->SetParLimits(4,-0.1,0.1);
    DCAxyFit->SetParLimits(7,-0.1,0.1);
    if(PtBin>10){
        DCAxyFit->SetParameters(0,1,50,2400,0,0.07,200,0,0.04);
        DCAxyFit->SetParLimits(4,-0.1,0.1);
        DCAxyFit->SetParLimits(7,-0.1,0.1);
    }
    if(PtBin>17){
        DCAxyFit->SetParameters(0,0,3,1500,0,0.02,200,0,0.05);
        DCAxyFit->SetParLimits(4,-0.1,0.1);
        DCAxyFit->SetParLimits(7,-0.1,0.1);
    }

cout<<"******* result"<<endl;
    auto Result = histo->Fit("DCAxyFit","SB","",-1.0,1.0);
cout<<"******* after result"<<endl;
    Signal->SetLineColor(kRed+2);
    BackGr->SetLineColor(kMagenta);

    Double_t par[9];
    DCAxyFit->GetParameters(par);
    BackGr->SetParameters(par);
    Signal->SetParameters(&par[3]);

    //calculation of fraction of primaries and respective error
   Double_t Binlow = -0.1;
    Double_t Binup = 0.1;
cout<<"******* befor integral "<<endl;
    Double_t DCAFitInt = DCAxyFit->Integral(Binlow,Binup);
    Double_t SignalInt = Signal->Integral(Binlow,Binup);
    cout<<"*************************"<<endl;
    Double_t errGlobal = DCAxyFit->IntegralAndError(Binlow,Binup);
    Double_t errSignal1 = Signal->IntegralAndError(Binlow,Binup);
cout<<"******* after integral "<<endl;
cout<<DCAFitInt<<"\n"<<SignalInt<<"\n"<<errGlobal<<"\n"<<errSignal1<<endl;
    Double_t PrimaryFraction = 0;
    if(!(DCAFitInt==0))PrimaryFraction = SignalInt/DCAFitInt;
    Double_t RelError1;
    if(!(SignalInt==0)&&!(DCAFitInt==0))RelError1 = TMath::Sqrt((errSignal1/SignalInt)*(errSignal1/SignalInt)+(errGlobal/DCAFitInt)*(errGlobal/DCAFitInt));
    ValueErrorArray[0] = PrimaryFraction;
    ValueErrorArray[1] = PrimaryFraction*RelError1;

    cout<<"end of fit fsun"<<endl;
}
void LabelDCAHisto(TH1F *Histo, TString Title, EColor color){

    Histo->SetMarkerColor(color);
    Histo->SetMarkerSize(0.8);
    Histo->SetMarkerStyle(20);
    Histo->SetTitle(Title.Data());
    Histo->GetXaxis()->SetRangeUser(-0.7, 0.7);
    Histo->GetXaxis()->SetTitle("DCAxy (cm)");
    Histo->GetYaxis()->SetTitle("Counts");
    Histo->GetXaxis()->SetTitleOffset(1.0);

}
void count(TH1F* h){
  for(int i = 0;i<h->GetNbinsX();i++){

    double d = h->GetBinContent(i);
    if(d != 0){

      cout<<"************ At I : "<<i<<"  ***** Bin Value is : "<<d<<" ********"<<endl;
    }

  }
}



TObjArray* getHistograms(){

  TH3F* hTOFm2_DCAxy_p;
  TH1F* hMC_prim;
  TH2F* hMC_Mat;
  TObjArray *histo = new TObjArray(3);
  TDirectoryFile* dir;
  TList* main_list;
  TList* subList;
  TList* mainListMC;
  TList* subListMC;

  TFile* dataFile = TFile::Open("High_Mult_TOF_PID_LHC16_k_l_o_p.root");
  hTOFm2_DCAxy_p = (TH3F*) dataFile->Get("Output_highMult_TOFpid")->FindObject("Protons")->FindObject("fHistTOFmass2_DCAxy_p")->Clone("hTOFm2_DCAxy_p");
  hTOFm2_DCAxy_p->Sumw2();
  histo->Add(hTOFm2_DCAxy_p);

  hMC_prim = (TH1F*) dataFile->Get("Output_highMult_TOFpid")->FindObject("ADeuterons")->FindObject("fHistDCAxy")->Clone("hMC_prim");
  histo->Add(hMC_prim);

  /*TFile* dataFile = TFile::Open("High_Mult_TOF_PID_LHC16_k_l_o_p.root");
  dataFile->GetObject("Output_HighMult_TOFpid",main_list);
  subList = (TList*) main_list->FindObject("Protons");
  hTOFm2_DCAxy_p = (TH3F*) subList->FindObject(fHistTOFmass2_DCAxy_p);*/


  TFile* mcFile = TFile::Open("LHC18f3_cent_2_MC_p-pb.root");
  mcFile->GetObject("DeuteronTrkCutsMC",dir);
  dir->GetObject("DeuteronTrkCutsMC",mainListMC);
  subListMC = (TList*)  mainListMC->FindObject("DCAPtBinning");
  hMC_Mat = (TH2F*) subListMC->FindObject("DCAPtBinningMat");
  hMC_Mat->RebinY(2);
  histo->Add(hMC_Mat);

  return histo;
}


Double_t background(Double_t *x, Double_t *par) {
    Float_t xval = x[0];
    Float_t par0 = par[0];
    Float_t par1 = par[1];
    Float_t par2 = par[2];
    Float_t par3 = par[3];
    return par0 + par1*xval + par2*TMath::Exp(xval*par3);
}

Double_t backGrProton(Double_t *x, Double_t *par) {
    Float_t xval = x[0];
    Float_t par0 = par[0];
    Float_t par1 = par[1];
    Float_t par2 = par[2];
    Float_t par3 = par[3];
    Float_t par4 = par[4];
    Float_t par5 = par[5];
    Float_t par6 = par[6];
    if(xval <= (par4 + par5*par6)){
        return TMath::Exp(par0+par1*xval+par2*xval*xval) + par3*TMath::Gaus(xval, par4, par5);
    }else{
        return TMath::Exp(par0+par1*xval+par2*xval*xval) + par3*TMath::Exp(-(xval-par4-par6*par5*0.5)*par6/par5);
    }

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Double_t gaussian(Double_t *x, Double_t *par) {

    Float_t xval = x[0];
    Float_t par0 = par[0];
    Float_t par1 = par[1];
    Float_t par2 = par[2];
    return par0*TMath::Gaus(xval,par1,par2);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Double_t gaussianSignal(Double_t *x, Double_t *par) {

    Float_t xval = x[0];
    Float_t par0 = par[0];
    Float_t par1 = par[1];
    Float_t par2 = par[2];
    Float_t par3 = par[3];
    if(xval < par1+par3*par2){
        return par0*1/(TMath::Sqrt(0.5*TMath::Pi())*(par2+par2*TMath::Erf(par3/TMath::Sqrt(2)))+par2/par3*TMath::Exp(-par3*par3/2))*TMath::Exp(-(xval-par1)*(xval-par1)/2/par2/par2);
    }else{
        return par0*1/(TMath::Sqrt(0.5*TMath::Pi())*(par2+par2*TMath::Erf(par3/TMath::Sqrt(2)))+par2/par3*TMath::Exp(-par3*par3/2))*TMath::Exp(-(xval-par1-par3*par2*0.5)*par3/par2);
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


Double_t MassFitFunction(Double_t *x, Double_t *par) {
    return gaussianSignal(x,par) + background(x,&par[4]);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Double_t MassFitFunctionProton(Double_t *x, Double_t *par) {
    return gaussianSignal(x,par) + background(x,&par[4]);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


Double_t DCAGauss(Double_t *x, Double_t *par) {
    return gaussian(x,par) + gaussian(x,&par[3]);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Double_t DCAQuadraticFunc(Double_t *x, Double_t *par){
    return par[0]*x[0]*x[0] + par[1]*x[0] + par[2];
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Double_t DCAFitFunction(Double_t *x, Double_t *par){
    return DCAQuadraticFunc(x,par) + DCAGauss(x,&par[3]);
}
