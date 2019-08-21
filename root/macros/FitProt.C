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


const char* file = "High_Mult_TOF_PID_LHC16_k_l_o_p.root";
const char* list = "Protons"; // Protons, AProtons
const char* part = "prot";    // prot, aprot
const char* hist = "fHistTOFmass2_DCAxy_p";


const char* listMCprim = "DCAPtBinningPri";
const char* listMCsecM = "DCAPtBinningMat";
const char* listMCsecW = "DCAPtBinningSec";

void FitProt(){

  TFile* file = new TFile(file);
  TFile*  fileMC = TFile::Open("LHC18f3_cent_2_MC_p-pb.root");

  TDirectoryFile* d;
  TList* t ; TList* listMC;

  //MC Histograms
  TH1F* hMCprim_DCAxy;
  TH1F* hMCsecM_DCAxy;
  TH1F* hMCsecW_DCAxy;

  fileMC->GetObject("ProtonTrkCutsMC",d);
  d->GetObject("ProtonTrkCutsMC",t);
  listMC = (TList*) t->FindObject("DCAPtBinning");

  // 3D hist
  TH3F* hTOFm2_DCAxy_p = (TH3F*)file->Get("Output_highMult_TOFpid")->FindObject(list)->FindObject(hist)->Clone("hTOFm2_DCAxy_p");
  hTOFm2_DCAxy_p->Sumw2();

  hTOFm2_DCAxy_p->GetZaxis()->SetRangeUser(DCAlow,DCAhigh);


  hMCprim_DCAxy= (TH1F*) listMC->FindObject(listMCprim);
  hMCsecM_DCAxy= (TH1F*) listMC->FindObject(listMCsecM);
  hMCsecW_DCAxy= (TH1F*) listMC->FindObject(listMCsecW);
  //hMCsecM_DCAxy->Draw();

  // 2D histo
  TH2F* hData_DCAxy_p = (TH2F*)hTOFm2_DCAxy_p->Project3D("zxo")->Clone("hData_DCAxy_p");

  TH1F* hData_DCAxy;
  TH1F* hMCsum;


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
  hData_DCAxy = (TH1F*)hData_DCAxy_p->ProjectionY("_py",5,5,"oe")->Clone(Form("hData_DCAxy_ptbin_%i",9));
hData_DCAxy->Draw();

/*
  // try to rebin
  hData_DCAxy  ->Rebin(rebinFactor);
  hMCprim_DCAxy->Rebin(rebinFactor);
  hMCsecM_DCAxy->Rebin(rebinFactor);
  hMCsecW_DCAxy->Rebin(rebinFactor);

  TObjArray *mc = new TObjArray(3);        // MC histograms are put in this array
  mc->Add(hMCprim_DCAxy);
  mc->Add(hMCsecM_DCAxy);
  mc->Add(hMCsecW_DCAxy);
  mc->ls();


  TFractionFitter* fit = new TFractionFitter(hData_DCAxy, mc); // initialise
  // constrain fractions

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

      cout << "purity = " << yieldPrim/(yieldPrim+yieldSecM+yieldSecW) << endl;

  }
*/
}
