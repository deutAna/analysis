
int bins_num[23]  = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 28, 30, 35, 40};
int binlow  = 4;
int binmid  = 7;
//#include "Functions.C"
#include "/home/ibrahim/Downloads/Secondary.C"
TObjArray*  getHistograms();
void MakePrimFracDeuteron(TString Name, TH1F *primary_fraction, TH2F *DCAData);
void MakeDCAPlot(TString Name,TString PtRangeName, TH1F *ProjDCAData, TF1 *Signal, TF1 *Background, Int_t PtBin);
void DCAFit(TH1F* histo,Int_t PtBin,Int_t ParticleType,TF1 *Signal,TF1 *BackGr,Double_t *ValueErrorArray);
void plotting(TH2F* hData_DCAxy_p ,TH1F* hMCprim_DCAxy,TH2F* hMCsecM_DCAxy_p,TH1F* mPurity);
void LabelDCAHisto(TH1F *Histo, TString Title, EColor color);
void Scaling(TH1F* hist);
void FitDCAxyDeut(){


  TObjArray* mHistograms = getHistograms();

  TH3F* hTOFm2_DCAxy_p = mHistograms->At(0);
  TH1F* hMC_prim = mHistograms->At(1);
  TH2F* hMC_Mat = mHistograms->At(2);
  TH2F* hMC_DCAxy_prim = mHistograms->At(3);

hMC_prim->Rebin(4);
//Scaling(hMC_prim);
        TH1F* mPurity = new TH1F("hPurity",Form("Purity %s","Deuteron"),45,0,4.5);
        mPurity->GetXaxis()->SetTitle("#it{p}, GeV/#it{c}");
        mPurity->GetYaxis()->SetTitle("purity");
        mPurity->GetYaxis()->SetRange(0,100);

        TH2F* hData_DCAxy_p = (TH2F*)hTOFm2_DCAxy_p->Project3D("zxo")->Clone("hData_DCAxy_p");
      //MakePrimFracDeuteron("Deuteron",hMC_prim,hData_DCAxy_p);
      plotting(hData_DCAxy_p,hMC_prim,hMC_Mat,mPurity);

      //MakePrimFracDeuteron("Deuteron",hMC_prim,hData_DCAxy_p,hMC_DCAxy_prim,hMC_Mat);
      cout<<hData_DCAxy_p->GetNbinsY()<<endl;
      cout<<hMC_Mat->GetNbinsY()<<endl;



}


void plotting(TH2F* hData_DCAxy_p ,TH1F* hMCprim_DCAxy,TH2F* hMCsecM_DCAxy_p,TH1F* mPurity){
  for (int bin= 6;bin < 24 ; bin++){

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
        hMCsecM_DCAxy->Rebin(2);


       cout<<hData_DCAxy->GetNbinsX()<<endl;
       cout<<hMCsecM_DCAxy->GetNbinsX()<<endl;
       cout<<hMCprim_DCAxy->GetNbinsX()<<endl;
      //Scaling(hData_DCAxy);
      //Scaling(hMCsecM_DCAxy);
        // MC histograms are put in this array
        TObjArray *mc = new TObjArray(2);
        mc->Add(hMCprim_DCAxy);
        mc->Add(hMCsecM_DCAxy);
        mc->ls();
        hData_DCAxy->Draw();
        c1->Print(Form("data%s_bin_%i.png","Deuteron",bin));

/*
        TFractionFitter* fit = new TFractionFitter(hData_DCAxy, mc);

        //fit->Constrain(0,-0.05,0.05);
        //fit->Constrain(1,-0.01,0.01);
        fit->Constrain(0,0.5,1.0);
        fit->Constrain(1,0.0001,0.5);


        Int_t status = fit->Fit();               // perform the fit
        cout << "fit status: " << status << endl;


                if (status == 0) {                       // check on fit status
                    TH1F* result = (TH1F*) fit->GetPlot();
                    // obtain weights
                    fit->GetResult(0, par0, errpar0);
                    fit->GetResult(1, par1, errpar1);
                    //fit->GetResult(2, par2, errpar2);

                    cout << "par0 = " << par0 << endl;
                    cout << "par1 = " << par1 << endl;
                    //cout << "par2 = " << par2 << endl;

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

*/
  }



}

void Scaling(TH1F* hist){
  hist->Scale(hist->Integral(),"width");
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
  TH2F* hMC_DCAxy_prim;
  TObjArray *histo = new TObjArray(4);
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
  hMC_Mat->RebinY(5);
  histo->Add(hMC_Mat);

  hMC_DCAxy_prim = (TH2F*) subListMC->FindObject("DCAPtBinningPri");
  hMC_DCAxy_prim->RebinY(5);
  histo->Add(hMC_DCAxy_prim);


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
