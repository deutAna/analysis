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

// input settings
const char* file = "High_Mult_TOF_PID_LHC16_k_l_o_p.root";
const char* list = "Protons"; // Protons, AProtons
const char* part = "prot";    // prot, aprot
const char* hist = "fHistTOFmass2_DCAxy_p";
// MC
const char* fileMC = "LHC17_fullRun_MCk_l_o_P_20190723.root";
const char* listMC = "protons"; // protons, aprotons
const char* listMCprim = "primary";
const char* listMCsecM = "secondaryM";
const char* listMCsecW = "secondaryW";
// output
const char* outd = "DCAxyFits";
const char* fout = "DCAxyPurity";

void FitDCAxyProt(int bin = 9){

    TFile* file = new TFile(file);
    // 3D hist
    TH3F* hTOFm2_DCAxy_p = (TH3F*)file->Get("Output_highMult_TOFpid")->FindObject(list)->FindObject(hist)->Clone("hTOFm2_DCAxy_p");
    hTOFm2_DCAxy_p->Sumw2();

    // MC
    TFile* fileMC = new TFile(fileMC);
    // 3D hists: primary, secondaryM, secondaryW
    TH3F* hMCprim_3d = (TH3F*)fileMC->Get("Output")->FindObject("OutputRec")->FindObject(listMC)->FindObject(listMCprim)->FindObject(hist)->Clone("hMCprim_3d");
    hMCprim_3d->Sumw2();

    TH3F* hMCsecM_3d = (TH3F*)fileMC->Get("Output")->FindObject("OutputRec")->FindObject(listMC)->FindObject(listMCsecM)->FindObject(hist)->Clone("hMCsecM_3d");
    hMCsecM_3d->Sumw2();

    TH3F* hMCsecW_3d = (TH3F*)fileMC->Get("Output")->FindObject("OutputRec")->FindObject(listMC)->FindObject(listMCsecW)->FindObject(hist)->Clone("hMCsecW_3d");
    hMCsecW_3d->Sumw2();

    // 2D hists
    TH2F* hData_DCAxy_p = (TH2F*)hTOFm2_DCAxy_p->Project3D("zxo")->Clone("hData_DCAxy_p");

    TH2F* hMCprim_DCAxy_p = (TH2F*)hMCprim_3d->Project3D("zx")->Clone("hMCprim_DCAxy_p");
    TH2F* hMCsecM_DCAxy_p = (TH2F*)hMCsecM_3d->Project3D("zx")->Clone("hMCsecM_DCAxy_p");
    TH2F* hMCsecW_DCAxy_p = (TH2F*)hMCsecW_3d->Project3D("zx")->Clone("hMCsecW_DCAxy_p");

    // 1D hists
    TH1F* hData_DCAxy;
    TH1F* hMCprim_DCAxy;
    TH1F* hMCsecM_DCAxy;
    TH1F* hMCsecW_DCAxy;

    TH1F* hMCsum;

    // fraction of prim/secM/secW
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
    // mc
    hMCprim_DCAxy = (TH1F*)hMCprim_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hMCprim_DCAxy_ptbin_%i",bin));
    hMCsecM_DCAxy = (TH1F*)hMCsecM_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hMCsecM_DCAxy_ptbin_%i",bin));
    hMCsecW_DCAxy = (TH1F*)hMCsecW_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hMCsecW_DCAxy_ptbin_%i",bin));

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

        c1->Print(Form("DCAxyFit_%s_bin_%i.pdf",part,bin));

        // extract purity
        binx_low  = hData_DCAxy->FindBin(-0.1+0.0001);
        binx_high = hData_DCAxy->FindBin(0.1-0.0001);

        yieldPrim = hMCprim_DCAxy->Integral(binx_low,binx_high);
        yieldSecM = hMCsecM_DCAxy->Integral(binx_low,binx_high);
        yieldSecW = hMCsecW_DCAxy->Integral(binx_low,binx_high);

        cout << "purity = " << yieldPrim/(yieldPrim+yieldSecM+yieldSecW) << endl;

    }

}
