// DCA fit range
float xmin = -1.0;
float xmax =  1.0;
// DCA cut
float DCAlow  = -0.1;
float DCAhigh =  0.1;
// TOF mass2 cut for protons       .........change
float mass_low  = 0.6;
float mass_high = 1.4;
// momentum bins to extract raw signal     ....... change
int binlow  = 4;
int binmid  = 7;
int binhigh = 20; // set purity to 1 above this bin

// input settings
const char* file = "MergedOutput.root";
const char* list = "Deuterons"; // Deuterons, ADeuterons
const char* part = "deut";    // deut, adeut
const char* hist = "fHistTOFmass2_DCAxy_p";
// MC
const char* fileMC = "AntiOutput_MC_10_LHC17f2b_protons.root";
const char* listMC = "deuterons"; // protons, aprotons
const char* listMCprim = "primary";
const char* listMCsecM = "secondaryM";
//const char* listMCsecW = "secondaryW";     //...... no weak for deut
// output
const char* outd = "DCAxyFits";
const char* fout = "DCAxyPurity";

void FitDCAxyDeut(int bin = 9){

    TFile* file = new TFile(file);
    // 3D hist
    TH3F* hTOFm2_DCAxy_p = (TH3F*)file->Get("Output_highMult")->FindObject(list)->FindObject(hist)->Clone("hTOFm2_DCAxy_p");
    hTOFm2_DCAxy_p->Sumw2();

    // MC
    TFile* fileMC = new TFile(fileMC);
    // 3D hists: primary, secondaryM, secondaryW
    TH3F* hMCprim_3d = (TH3F*)fileMC->Get("Output")->FindObject("OutputRec")->FindObject(listMC)->FindObject(listMCprim)->FindObject(hist)->Clone("hMCprim_3d");
    hMCprim_3d->Sumw2();

    TH3F* hMCsecM_3d = (TH3F*)fileMC->Get("Output")->FindObject("OutputRec")->FindObject(listMC)->FindObject(listMCsecM)->FindObject(hist)->Clone("hMCsecM_3d");
    hMCsecM_3d->Sumw2();

    //TH3F* hMCsecW_3d = (TH3F*)fileMC->Get("Output")->FindObject("OutputRec")->FindObject(listMC)->FindObject(listMCsecW)->FindObject(hist)->Clone("hMCsecW_3d");
   // hMCsecW_3d->Sumw2();   /// no weak

    // 2D hists
    TH2F* hData_DCAxy_p = (TH2F*)hTOFm2_DCAxy_p->Project3D("zxo")->Clone("hData_DCAxy_p");

    TH2F* hMCprim_DCAxy_p = (TH2F*)hMCprim_3d->Project3D("zx")->Clone("hMCprim_DCAxy_p");
    TH2F* hMCsecM_DCAxy_p = (TH2F*)hMCsecM_3d->Project3D("zx")->Clone("hMCsecM_DCAxy_p");
    //TH2F* hMCsecW_DCAxy_p = (TH2F*)hMCsecW_3d->Project3D("zx")->Clone("hMCsecW_DCAxy_p");   /// no weak

    // 1D hists
    TH1F* hData_DCAxy;
    TH1F* hMCprim_DCAxy;
    TH1F* hMCsecM_DCAxy;
    //TH1F* hMCsecW_DCAxy;

    TH1F* hMCsum;

    // fraction of prim/secM/secW
    Double_t par0, errpar0, par1, errpar1, par2, errpar2;   /// no par2, errpar2;
    // yield of prim/secM/secW
    Double_t yieldPrim, yieldSecM;      // no yieldSecW
    Double_t weightPrim, weightSecM;
    int binx_low  = 0;
    int binx_high = 0;

    // canvas and QA dir
    TCanvas* c1 = new TCanvas("c1","c1");
    c1->SetLogy();

    int binstart = binlow;
    int binend   = binmid-1;

    int rebinFactor = 2;   // why?

    // data
    hData_DCAxy = (TH1F*)hData_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hData_DCAxy_ptbin_%i",bin));
    // mc
    hMCprim_DCAxy = (TH1F*)hMCprim_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hMCprim_DCAxy_ptbin_%i",bin));
    hMCsecM_DCAxy = (TH1F*)hMCsecM_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hMCsecM_DCAxy_ptbin_%i",bin));
    //hMCsecW_DCAxy = (TH1F*)hMCsecW_DCAxy_p->ProjectionY("_py",bin,bin,"oe")->Clone(Form("hMCsecW_DCAxy_ptbin_%i",bin));   /// no weak

    // try to rebin
    hData_DCAxy  ->Rebin(rebinFactor);
    hMCprim_DCAxy->Rebin(rebinFactor);
    hMCsecM_DCAxy->Rebin(rebinFactor);
    //hMCsecW_DCAxy->Rebin(rebinFactor);   /// no weak

    TObjArray *mc = new TObjArray(2);        // MC histograms are put in this array
    mc->Add(hMCprim_DCAxy);
    mc->Add(hMCsecM_DCAxy);
    //mc->Add(hMCsecW_DCAxy);   /// no weak
    mc->ls();
      cout<<"start fitting ********2222222****"<<std::endl;
    TFractionFitter* fit = new TFractionFitter(hData_DCAxy, mc); // initialise
    // constrain fractions

    // protons                       ..........................
    if (part == "prot"){
        fit->Constrain(1,0.93,1.0);
        fit->Constrain(2,0.0,0.01);
        //fit->Constrain(3,0.01,0.07);
    }

    // aprotons
    if (part == "aprot"){
        fit->Constrain(1,0.93,1.0);
        fit->Constrain(2,0.0,0.01);
        //fit->Constrain(3,0.01,0.07);
    }

    Int_t status = fit->Fit();               // perform the fit
    cout << "fit status: " << status << endl;
      cout<<"start fitting ********22221111111222****"<<std::endl;
    if (status == 0) {                       // check on fit status
        TH1F* result = (TH1F*) fit->GetPlot();
        // obtain weights
        fit->GetResult(0, par0, errpar0);
        fit->GetResult(1, par1, errpar1);
        //fit->GetResult(2, par2, errpar2);   /// no par2, errpar2

        cout << "par0 = " << par0 << endl;
        cout << "par1 = " << par1 << endl;
        //cout << "par2 = " << par2 << endl;  /// no par2,

        hData_DCAxy->SetLineColor(kBlack);
        hData_DCAxy->SetMarkerColor(kBlack);
        hData_DCAxy->SetMarkerStyle(kFullCircle);
        hData_DCAxy->Draw("Ep");

        result->SetLineColor(kBlue);
        result->Draw("same");

        weightPrim = par0*(hData_DCAxy->Integral())/(hMCprim_DCAxy->Integral());
        weightSecM = par1*(hData_DCAxy->Integral())/(hMCsecM_DCAxy->Integral());
        //weightSecW = par2*(hData_DCAxy->Integral())/(hMCsecW_DCAxy->Integral());    /// no weak

        hMCprim_DCAxy->Scale(weightPrim);
        hMCsecM_DCAxy->Scale(weightSecM);
        //hMCsecW_DCAxy->Scale(weightSecW);   /// no weak

        hMCsum = (TH1F*)hMCprim_DCAxy->Clone();
        hMCsum->Add(hMCsecM_DCAxy);            //  so here we have MC histo primary and secondary in hMCsum object
        //hMCsum->Add(hMCsecW_DCAxy);   /// no weak

        hMCprim_DCAxy->SetLineColor(kRed);
        hMCprim_DCAxy->SetMarkerColor(kRed);
        hMCprim_DCAxy->SetMarkerStyle(kFullCircle);
        hMCprim_DCAxy->Draw("same");

        hMCsecM_DCAxy->SetLineColor(kGreen);
        hMCsecM_DCAxy->SetMarkerColor(kGreen);
        hMCsecM_DCAxy->SetMarkerStyle(kFullCircle);
        hMCsecM_DCAxy->Draw("same");

       /* hMCsecW_DCAxy->SetLineColor(kMagenta);       /// no weak
        hMCsecW_DCAxy->SetMarkerColor(kMagenta);
        hMCsecW_DCAxy->SetMarkerStyle(kFullCircle);
        hMCsecW_DCAxy->Draw("same"); */

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
        //eldSecW = hMCsecW_DCAxy->Integral(binx_low,binx_high);    /// no weak

        cout << "purity = " << yieldPrim/(yieldPrim+yieldSecM) << endl;

    }

}
void TFractionFitter::FindPrediction(int bin, Double_t &t_i, int& k_0, Double_t &A_ki) const {
    std::vector<Double_t> wgtFrac(fNpar); // weighted fractions (strengths of the sources)
    std::vector<Double_t> a_ji(fNpar); // number of observed MC events for bin 'i' and par (source) 'j'
    Double_t d_i = fData->GetBinContent(bin); // number of events in the real data for bin 'i'

    // Cache the weighted fractions and the number of observed MC events
    // Sanity check: none of the fractions should be == 0
    for (Int_t par = 0; par < fNpar; ++par) {
       a_ji[par] = ((TH1*)fMCs.At(par))->GetBinContent(bin);
       TH1* hw = (TH1*)fWeights.At(par);
       wgtFrac[par] = hw ? hw->GetBinContent(bin) * fFractions[par] : fFractions[par];
       if (wgtFrac[par] == 0) {
          Error("FindPrediction", "Fraction[%d] = 0!", par);
          return;
       }
    }

    // Case data = 0
    if (TMath::Nint(d_i) == 0) {
       t_i = 1;
       k_0 = -1;
       A_ki = 0;
       return;
    }

    // Case one or more of the MC bin contents == 0 -> find largest fraction
    // k_0 stores the source index of the largest fraction
    k_0 = 0;
    Double_t maxWgtFrac = wgtFrac[0];
    for (Int_t par = 1; par < fNpar; ++par) {
       if (wgtFrac[par] > maxWgtFrac) {
          k_0 = par;
          maxWgtFrac = wgtFrac[par];
       }
    }
    Double_t t_min = -1 / maxWgtFrac; // t_i cannot be smaller than this value (see paper, par 5)

    // Determine if there are more sources which have the same maximum contribution (fraction)
    Int_t nMax = 1; Double_t contentsMax = a_ji[k_0];
    for (Int_t par = 0; par < fNpar; ++par) {
       if (par == k_0) continue;
       if (wgtFrac[par] == maxWgtFrac) {
          nMax++;
          contentsMax += a_ji[par];
       }
    }

    // special action if there is a zero in the number of entries for the MC source with
    // the largest strength (fraction) -> See Paper, Paragraph 5
    if (contentsMax == 0) {
       A_ki = d_i / (1.0 + maxWgtFrac);
       for (Int_t par = 0; par < fNpar; ++par) {
          if (par == k_0 || wgtFrac[par] == maxWgtFrac) continue;
          A_ki -= a_ji[par] * wgtFrac[par] / (maxWgtFrac - wgtFrac[par]);
       }
       if (A_ki > 0) {
          A_ki /= nMax;
          t_i = t_min;
          return;
       }
    }
    k_0 = -1;

    // Case of nonzero histogram contents: solve for t_i using Newton's method
    // The equation that needs to be solved:
    //    func(t_i) = \sum\limits_j{\frac{ p_j a_{ji} }{1 + p_j t_i}} - \frac{d_i}{1 - t_i} = 0
    t_i = 0; Double_t step = 0.2;
    Int_t maxIter = 100000; // maximum number of iterations
    for(Int_t i = 0; i < maxIter; ++i) {
       if (t_i >= 1 || t_i < t_min) {
          step /= 10;
          t_i = 0;
       }
       Double_t func   = - d_i / (1.0 - t_i);
       Double_t deriv = func / (1.0 - t_i);
       for (Int_t par = 0; par < fNpar; ++par) {
          Double_t r = 1.0 / (t_i + 1.0 / wgtFrac[par]);
          func   += a_ji[par] * r;
          deriv -= a_ji[par] * r * r;
       }
       if (TMath::Abs(func) < 1e-12) return; // solution found
       Double_t delta = - func / deriv; // update delta
       if (TMath::Abs(delta) > step)
          delta = (delta > 0) ? step : -step; // correct delta if it becomes too large
       t_i += delta;
       if (TMath::Abs(delta) < 1e-13) return; // solution found
    } // the loop breaks when the solution is found, or when the maximum number of iterations is exhausted

    Warning("FindPrediction", "Did not find solution for t_i in %d iterations", maxIter);

    return;
 }
