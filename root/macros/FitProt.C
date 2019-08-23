// DCA fit range
float xmin = -1.0;
float xmax =  1.0;
// DCA cut
float DCAlow  = -0.1;
float DCAhigh =  0.1;
// TOF mass2 cut for protons
float mass_low  = 0.6;
float mass_high = 1.4;
// momentum bins to extract raw signal
int binlow  = 4;
int binmid  = 7;
int binhigh = 20; // set purity to 1 above this bin

bool mIsEmpty = kFALSE;


const char* file = "High_Mult_TOF_PID_LHC16_k_l_o_p.root";
const char* list = "Protons"; // Protons, AProtons
const char* part = "prot";    // prot, aprot
const char* hist = "fHistTOFmass2_DCAxy_p";


const char* listMCprim = "DCAPtBinningPri";
const char* listMCsecM = "DCAPtBinningMat";
const char* listMCsecW = "DCAPtBinningSec";

void Fiting(TH2F* hData_DCAxy_p ,TH2F* hMCprim_DCAxy_p,TH2F* hMCsecM_DCAxy_p,TH2F* hMCsecW_DCAxy_p,TH1F* mPurity );
void WriteHistos(TH1F* purity);
int countBins(TH1F* h);
void plotFitterNoMat(int bin,TH1F* hData_DCAxy, TH1F* hMCprim_DCAxy, TH1F* hMCsecW_DCAxy,TH1F* mPurity);
void FitProt(){

  TFile* file = new TFile(file);
  TFile*  fileMC = TFile::Open("LHC18f3_cent_2_MC_p-pb.root");

  TDirectoryFile* d;
  TList* t ; TList* listMC;

  //MC Histograms
  TH2F* hMCprim_DCAxy_p;
  TH2F* hMCsecM_DCAxy_p;
  TH2F* hMCsecW_DCAxy_p;

  TH1F* hMCprim_DCAxy;
  TH1F* hMCsecM_DCAxy;
  TH1F* hMCsecW_DCAxy;

  //output
  TH1F* hData_DCAxy;
  TH1F* hMCsum;

  fileMC->GetObject("ProtonTrkCutsMC",d);
  d->GetObject("ProtonTrkCutsMC",t);
  listMC = (TList*) t->FindObject("DCAPtBinning");

  // 3D hist
  TH3F* hTOFm2_DCAxy_p = (TH3F*)file->Get("Output_highMult_TOFpid")->FindObject(list)->FindObject(hist)->Clone("hTOFm2_DCAxy_p");
  hTOFm2_DCAxy_p->Sumw2();

   //RebinY MCs 1:5 to make the total bins in rang 200 as DATA Histogram to fitting

  hMCprim_DCAxy_p= (TH2F*) listMC->FindObject(listMCprim);
  hMCprim_DCAxy_p->RebinY(5);
  hMCsecM_DCAxy_p= (TH2F*) listMC->FindObject(listMCsecM);
  hMCsecM_DCAxy_p->RebinY(5);
  hMCsecW_DCAxy_p= (TH2F*) listMC->FindObject(listMCsecW);
  hMCsecW_DCAxy_p->RebinY(5);

  // 2D histo
  TH2F* hData_DCAxy_p = (TH2F*)hTOFm2_DCAxy_p->Project3D("zxo")->Clone("hData_DCAxy_p");




      TH1F* mPurity = new TH1F("hPurity",Form("Purity %s",list),45,0,4.5);
      mPurity->GetXaxis()->SetTitle("#it{p}, GeV/#it{c}");
      mPurity->GetYaxis()->SetTitle("purity");
      mPurity->GetYaxis()->SetRange(0,100);

      Fiting(hData_DCAxy_p ,hMCprim_DCAxy_p,hMCsecM_DCAxy_p,hMCsecW_DCAxy_p,mPurity);
  //    TH1F* hMCsecM_DCAxy = (TH1F*)hMCsecM_DCAxy_p->ProjectionY("_py",33,33,"oe")->Clone(Form("hMCsecM_DCAxy_ptbin_%i",31));
//hMCsecM_DCAxy->Draw();
}
void Fiting(TH2F* hData_DCAxy_p ,TH2F* hMCprim_DCAxy_p,TH2F* hMCsecM_DCAxy_p,TH2F* hMCsecW_DCAxy_p,TH1F* mPurity ){

  for(int bin = 4;bin<=45;bin++){


    Double_t par0, errpar0, par1, errpar1, par2, errpar2;
    // yield of prim/secM/secW
    Double_t yieldPrim, yieldSecM, yieldSecW;
    Double_t weightPrim, weightSecM, weightSecW;
    int binx_low  = 0;
    int binx_high = 0;

    // canvas and QA dir
    TCanvas* c1 = new TCanvas("c1","c1");
    c1->SetLogy();

    int binstart = binlow;
    int binend   = binmid-1;

    int rebinFactor = 2;

    // data
    hData_DCAxy = (TH1F*)hData_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hData_DCAxy_ptbin_%i",bin));

    //MCs
    hMCprim_DCAxy = (TH1F*)hMCprim_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hMCprim_DCAxy_ptbin_%i",bin));
    hMCsecM_DCAxy = (TH1F*)hMCsecM_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hMCsecM_DCAxy_ptbin_%i",bin));
    hMCsecW_DCAxy = (TH1F*)hMCsecW_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hMCsecW_DCAxy_ptbin_%i",bin));

    // try to rebin
    hData_DCAxy->Rebin(rebinFactor);
    hMCprim_DCAxy->Rebin(rebinFactor);
    hMCsecM_DCAxy->Rebin(rebinFactor);
    hMCsecW_DCAxy->Rebin(rebinFactor);
// Fixing purity bins with no Fiitting
if (part == "prot"){
  if(bin == 33||bin == 37 || bin == 38 || bin == 39 || bin == 44){
    plotFitterNoMat(bin,hData_DCAxy,hMCprim_DCAxy,hMCsecW_DCAxy,mPurity);
    mIsEmpty = kTRUE;
  }else{
    mIsEmpty = kFALSE;
  }
}else{
  if(bin == 31||bin == 34 || bin == 37 || bin == 38){
    plotFitterNoMat(bin,hData_DCAxy,hMCprim_DCAxy,hMCsecW_DCAxy,mPurity);
    mIsEmpty = kTRUE;
  }else{
    mIsEmpty = kFALSE;
  }
}


  if(!mIsEmpty){
    if(countBins(hMCsecM_DCAxy) == 0){

      plotFitterNoMat(bin,hData_DCAxy,hMCprim_DCAxy,hMCsecW_DCAxy,mPurity);
    }else{


    // MC histograms are put in this array
    TObjArray *mc = new TObjArray(3);
    mc->Add(hMCprim_DCAxy);
    mc->Add(hMCsecM_DCAxy);
    mc->Add(hMCsecW_DCAxy);
    mc->ls();

        TFractionFitter* fit = new TFractionFitter(hData_DCAxy, mc); // initialise
        // constrain fractions
if(bin == 31){
  if (part == "prot"){
      fit->Constrain(1,0.93,1.0);
      fit->Constrain(2,-0.01,0.01);
      fit->Constrain(3,0.01,0.07);
  }

  // aprotons
  if (part == "aprot"){
      fit->Constrain(1,0.93,1.0);
      fit->Constrain(2,-0.01,0.01);
      fit->Constrain(3,0.01,0.07);
  }
}else{
        // protons
        if (part == "prot"){
            fit->Constrain(1,0.93,1.0);
            fit->Constrain(2,0.0,0.01);
            fit->Constrain(3,0.01,0.07);
        }

        // aprotons
        if (part == "aprot"){
            fit->Constrain(1,0.93,1.0);
            fit->Constrain(2,0.0,0.01);
            fit->Constrain(3,0.01,0.07);
        }
}
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
            weightSecW = par2*(hData_DCAxy->Integral())/(hMCsecW_DCAxy->Integral());

            hMCprim_DCAxy->Scale(weightPrim);
            hMCsecM_DCAxy->Scale(weightSecM);
            hMCsecW_DCAxy->Scale(weightSecW);

            hMCsum = (TH1F*)hMCprim_DCAxy->Clone();
            hMCsum->Add(hMCsecM_DCAxy);
            hMCsum->Add(hMCsecW_DCAxy);

            hMCprim_DCAxy->SetLineColor(kRed);
            hMCprim_DCAxy->SetMarkerColor(kRed);
            hMCprim_DCAxy->SetMarkerStyle(kFullCircle);
            hMCprim_DCAxy->Draw("same");

            hMCsecM_DCAxy->SetLineColor(kGreen);
            hMCsecM_DCAxy->SetMarkerColor(kGreen);
            hMCsecM_DCAxy->SetMarkerStyle(kFullCircle);
            hMCsecM_DCAxy->Draw("same");

            hMCsecW_DCAxy->SetLineColor(kMagenta);
            hMCsecW_DCAxy->SetMarkerColor(kMagenta);
            hMCsecW_DCAxy->SetMarkerStyle(kFullCircle);
            hMCsecW_DCAxy->Draw("same");

            hMCsum->SetLineColor(kBlue+1);
            hMCsum->SetMarkerColor(kBlue+1);
            hMCsum->SetMarkerStyle(kFullStar);
            hMCsum->Draw("same");

            c1->Print(Form("DCAxyFit_%s_bin_%i.png",part,bin));

            // extract purity
            binx_low  = hData_DCAxy->FindBin(-0.1+0.0001);
            binx_high = hData_DCAxy->FindBin(0.1-0.0001);

            yieldPrim = hMCprim_DCAxy->Integral(binx_low,binx_high);
            yieldSecM = hMCsecM_DCAxy->Integral(binx_low,binx_high);
            yieldSecW = hMCsecW_DCAxy->Integral(binx_low,binx_high);

            double purity =yieldPrim/(yieldPrim+yieldSecW);
                    cout << "purity = " <<purity<< endl;
                    mPurity->SetBinContent(bin,purity);
    }
  }
}
  }
  WriteHistos(mPurity);
}

void plotFitterNoMat(int bin,TH1F* hData_DCAxy, TH1F* hMCprim_DCAxy, TH1F* hMCsecW_DCAxy,TH1F* mPurity){


    Double_t par0, errpar0, par1, errpar1, par2, errpar2;
    // yield of prim/secM/secW
    Double_t yieldPrim, yieldSecM, yieldSecW;
    Double_t weightPrim, weightSecM, weightSecW;
    int binx_low  = 0;
    int binx_high = 0;

   TObjArray *mc = new TObjArray(2);        // MC histograms are put in this array
    mc->Add(hMCprim_DCAxy);
    mc->Add(hMCsecW_DCAxy);
    mc->ls();

    TFractionFitter* fit = new TFractionFitter(hData_DCAxy, mc); // initialise
    // constrain fractions

    // protons
    if (part == "prot"){
        fit->Constrain(1,0.93,1.0);
        fit->Constrain(2,0.01,0.07);
    }

    // aprotons
    if (part == "aprot"){
        fit->Constrain(1,0.93,1.0);
        //fit->Constrain(2,0.0,0.01);
        fit->Constrain(2,0.01,0.07);
    }

    Int_t status = fit->Fit();               // perform the fit
    cout << "fit status: " << status << endl;

status = fit->Fit();               // perform the fit
cout << "fit status: " << status << endl;
    if (status == 0) {                       // check on fit status
        TH1F* result = (TH1F*) fit->GetPlot();
        // obtain weights
        fit->GetResult(0, par0, errpar0);
      //  fit->GetResult(1, par1, errpar1);
        fit->GetResult(1, par1, errpar1);

        cout << "par0 = " << par0 << endl;

        cout << "par1 = " << par1 << endl;

        hData_DCAxy->SetLineColor(kBlack);
        hData_DCAxy->SetMarkerColor(kBlack);
        hData_DCAxy->SetMarkerStyle(kFullCircle);
        hData_DCAxy->Draw("Ep");

        result->SetLineColor(kBlue);
        result->Draw("same");

        weightPrim = par0*(hData_DCAxy->Integral())/(hMCprim_DCAxy->Integral());
      //  weightSecM = par1*(hData_DCAxy->Integral())/(hMCsecM_DCAxy->Integral());
        weightSecW = par1*(hData_DCAxy->Integral())/(hMCsecW_DCAxy->Integral());


        hMCprim_DCAxy->Scale(weightPrim);

        hMCsecW_DCAxy->Scale(weightSecW);

        hMCsum = (TH1F*)hMCprim_DCAxy->Clone();
        //hMCsum->Add(hMCsecM_DCAxy);
        hMCsum->Add(hMCsecW_DCAxy);

        hMCprim_DCAxy->SetLineColor(kRed);
        hMCprim_DCAxy->SetMarkerColor(kRed);
        hMCprim_DCAxy->SetMarkerStyle(kFullCircle);
        hMCprim_DCAxy->Draw("same");


        hMCsecW_DCAxy->SetLineColor(kMagenta);
        hMCsecW_DCAxy->SetMarkerColor(kMagenta);
        hMCsecW_DCAxy->SetMarkerStyle(kFullCircle);
        hMCsecW_DCAxy->Draw("same");

        hMCsum->SetLineColor(kBlue+1);
        hMCsum->SetMarkerColor(kBlue+1);
        hMCsum->SetMarkerStyle(kFullStar);
        hMCsum->Draw("same");

        //c1->Print(Form("DCAxyFit_%s_bin_%i.pdf",part,bin));
        c1->Print(Form("DCAxyFit_%s_bin_%i.png",part,bin));

        // extract purity
        binx_low  = hData_DCAxy->FindBin(-0.1+0.0001);
        binx_high = hData_DCAxy->FindBin(0.1-0.0001);

        yieldPrim = hMCprim_DCAxy->Integral(binx_low,binx_high);
        yieldSecW = hMCsecW_DCAxy->Integral(binx_low,binx_high);

double purity =yieldPrim/(yieldPrim+yieldSecW);
        cout << "purity = " <<purity<< endl;
        mPurity->SetBinContent(bin,purity);

    }



}
int countBins(TH1F* h){
  int M = 0;
  for(int z = 0;z<h->GetNbinsX();z++){
    double ax = h->GetBinContent(z);
    if(ax != 0){
      M++;
    }
  }
  return M;
}

void WriteHistos(TH1F* purity){

    TFile* fileout = new TFile(Form("%s_%s.root","Purity",part),"RECREATE");
    purity->SetStats(kFALSE);
    purity->Draw("h");
    purity->Write();
    fileout->Close();

}
