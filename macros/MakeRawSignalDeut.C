// TOF mass2 fit range
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

// input settings
const char* file = "MergedOutput.root";
const char* list = "Deuterons"; // Deuterons, ADeuterons
const char* part = "deut";      // deut, adeut
const char* outp = "Output_default";
const char* hist = "fHistTOFmass2_DCAxy_p";
const char* hITS = "fHistITSnSigmaDeut";
const char* outd = "TOFm2fits";
const char* fout = "RawSpectrum_FitLoose";

//declare functions
void FillSpectrumLowP(TH3F* hist, TH1F* rawSpectrum);
void FitTOFm2(TH3F* hist, TH1F* rawSpectrum);
void WriteHistos(TH1F* spectrum);

void MakeRawSignalDeut(){
    
    // hist raw spectrum
    // TH1F* hRawSpectrum = new TH1F("hRawSpectrum", Form("Raw spectrum %s",list), 35, 0, 3.5);
    TH1F* hRawSpectrum = new TH1F("hRawSpectrum", Form("Raw spectrum %s",list), nbins_lowp+nbins_highp, binsx_p);
    hRawSpectrum->GetXaxis()->SetTitle("#it{p}, GeV/#it{c}");
    hRawSpectrum->GetYaxis()->SetTitle("counts");
    
    TFile* file = new TFile(file);
    // 3D hist
    TH3F* hTOFm2_DCAxy_p = (TH3F*)file->Get(outp)->FindObject(list)->FindObject(hist)->Clone("hTOFm2_DCAxy_p");
    hTOFm2_DCAxy_p->Sumw2();
    // 2d hist ITSnSigma vs p (for low momentum part)
    TH2F* hITSnSigmaDeut_p = (TH2F*)file->Get(outp)->FindObject(list)->FindObject(hITS)->Clone("hITSnSigmaDeut_p");
    
    gSystem->Exec(Form("mkdir -p %s_%s", outd, part));
    gStyle->SetOptTitle(0);
    
    // fill low mometum part using counts from ITSnSigma hist (with cut on DCAxy < 0.1 cm!)
    FillSpectrumLowP(hITSnSigmaDeut_p,hRawSpectrum);
    
    // fill high momentum part using fit to TOFm2
    FitTOFm2(hTOFm2_DCAxy_p, hRawSpectrum);
    
    // divide by bin width
    hRawSpectrum->Scale(0.1, "width");
    
    WriteHistos(hRawSpectrum);
}

void FillSpectrumLowP(TH2F* hist, TH1F* rawSpectrum){
    
    // cut on ITSnSigma
    hist->GetYaxis()->SetRangeUser(ITSmin, ITSmax);
    TH1F* h_p = (TH1F*)hist->ProjectionX("oe")->Clone("h_p");
    
    for (Int_t i = binlow; i< binmid; i++){
        rawSpectrum->SetBinContent(i, h_p->GetBinContent(i));
        rawSpectrum->SetBinError(i, h_p->GetBinError(i));
    }
    
}

void FitTOFm2(TH3F* hist, TH1F* rawSpectrum){
    
    // cut on DCA
    hist->GetZaxis()->SetRangeUser(DCAlow,DCAhigh);
    // 2D hist
    TH2F* hTOFm2_p = (TH2F*)hist->Project3D("yx")->Clone("hTOFm2_p");
    // 1D hist
    TH1F* hTOFm2;
    // fit functions
    TF1 fTOFcombined("fTOFcombined", "[0]*1/(TMath::Sqrt(0.5*TMath::Pi())*([2]+[2]*TMath::Erf([3]/TMath::Sqrt(2)))+[2]/[3]*TMath::Exp(-[3]*[3]/2))*TMath::Exp(-(x-[1])*(x-[1])/2/[2]/[2])* (x < [1]+[3]*[2]) + (x > [1]+[3]*[2])*[0]*1/(TMath::Sqrt(0.5*TMath::Pi())*([2]+[2]*TMath::Erf([3]/TMath::Sqrt(2)))+[2]/[3]*TMath::Exp(-[3]*[3]/2))*TMath::Exp(-(x-[1]-[3]*[2]*0.5)*[3]/[2]) + TMath::Exp([4]+[5]*x+[6]*x*x)", xmin, xmax);
    
    TF1 fTOFsignal("fTOFsignal","[0]*1/(TMath::Sqrt(0.5*TMath::Pi())*([2]+[2]*TMath::Erf([3]/TMath::Sqrt(2)))+[2]/[3]*TMath::Exp(-[3]*[3]/2))*TMath::Exp(-(x-[1])*(x-[1])/2/[2]/[2])* (x < [1]+[3]*[2]) + (x > [1]+[3]*[2])*[0]*1/(TMath::Sqrt(0.5*TMath::Pi())*([2]+[2]*TMath::Erf([3]/TMath::Sqrt(2)))+[2]/[3]*TMath::Exp(-[3]*[3]/2))*TMath::Exp(-(x-[1]-[3]*[2]*0.5)*[3]/[2])",xmin,xmax);
    
    TF1 fTOFbackground("fTOFbackground", "TMath::Exp([0]+[1]*x+[2]*x*x)", xmin, xmax);
    
    // start values for first fit
    Double_t StartValues[7]      = {63.5, 3.63, 0.1, 0.8, -4.2, 2.3, -0.3};
    
    // set par limits
    fTOFcombined.SetParLimits(1,3.0,4.0); // mean
    fTOFcombined.SetParLimits(2,0.0,0.5); // width
    fTOFcombined.SetParLimits(3,0.,2.);   // tail parameter
    
    // set par names
    fTOFcombined.SetParName(0,"yield d"); // yield deuteron (norm)
    fTOFcombined.SetParName(1,"M^{2}_{d}"); // mean deuteron
    fTOFcombined.SetParName(2,"#sigma_{d}"); // width deuteron
    fTOFcombined.SetParName(3,"#tau_{d}");   // tail deuteron
    
    // yield and yield error
    Double_t yield = 1.0;
    Double_t yieldErr = 0.0;
    
    // canvas and QA dir
    TCanvas* c1 = new TCanvas("c1","c1");
    c1->SetLogy();
        
    // fit in momentum bins
    for (Int_t ii = 1; ii <= nbins_highp; ii++){ // nbins_highp
        
        int ibin = backward ? nbins_highp + 1 - ii : ii;
        
        hTOFm2 = (TH1F*)hTOFm2_p->ProjectionY("_py",binsx_n[ibin-1]+1,binsx_n[ibin],"oe")->Clone(Form("hTOFm2_ptbin_%i_%i",binsx_n[ibin-1],binsx_n[ibin]));
        
        // set (start) values
        for (Int_t i = 0; i <= 6; i++){
            fTOFcombined.SetParameter(i,StartValues[i]);
        }
        
        hTOFm2->Fit(&fTOFcombined,"QNR");
        
        // draw options
        hTOFm2->SetMarkerStyle(kOpenCircle);
        hTOFm2->SetMarkerColor(kBlue);
        hTOFm2->SetLineColor(kBlue);
        hTOFm2->GetXaxis()->SetRangeUser(xmin,xmax);
        hTOFm2->Draw("e1");
        // total fit
        fTOFcombined.SetLineColor(kBlack);
        fTOFcombined.DrawCopy("same");
        // signal and background parameters
        for(Int_t ival = 0; ival <= 6; ival++) {
            if (ival <=3){
                fTOFsignal.SetParameter(ival,fTOFcombined.GetParameter(ival));
            } else {
                fTOFbackground.SetParameter(ival-4,fTOFcombined.GetParameter(ival));
            }
        }
        fTOFsignal.SetLineColor(kGreen);
        fTOFsignal.DrawCopy("same");
        fTOFbackground.SetLineColor(kRed);
        fTOFbackground.DrawCopy("same");
        
        
        TLegend * leg = new TLegend(0.1, 0.8, 0.4, 0.95);
        leg->AddEntry(hTOFm2, Form("Data %s",part == "deut"? "deuteron" : "anti-deuteron"),   "P");
        leg->AddEntry("fTOFsignal", "Signal", "L");
        leg->AddEntry("fTOFbackground", "Background", "L");
        leg->AddEntry("fTOFcombined", "Total fit", "L");
        leg->SetFillColor(0);
        leg->SetTextSize(gStyle->GetTextSize()*0.8);
        leg->Draw();
        
        TLatex *momentumbin = new TLatex(0.15, 0.75, TString::Format("%g < #it{p} < %g GeV/#it{c}",binsx_n[ibin-1]*0.1,binsx_n[ibin]*0.1));
        momentumbin->SetNDC();
        momentumbin->SetTextFont(42);
        momentumbin->SetTextSize(0.035);
        momentumbin->Draw();
        
        
        for(Int_t ival = 0; ival <= 6; ival++) {
            StartValues[ival] = fTOFcombined.GetParameter(ival);
            // cout << StartValues[ival] << endl;
        }
        
        // Create and plot a box with stats
        c1->Modified();
        c1->Update();
        
        // Retrieve the stat box
        TPaveStats *ps = (TPaveStats*)c1->GetPrimitive("stats");
        ps->SetName("mystats");
        
        ps->SetX1NDC(0.7);
        ps->SetX2NDC(0.99);
        ps->SetY1NDC(0.5);
        ps->SetY2NDC(0.9);
        
        TList *list = ps->GetListOfLines();
        
        // Remove Entries, mean and RMS lines
        TText *tEntries = ps->GetLineWith("Entries");
        TText *tMean    = ps->GetLineWith("Mean");
        TText *tRMS     = ps->GetLineWith("RMS");
        list->Remove(tEntries);
        list->Remove(tMean);
        list->Remove(tRMS);
        
        int textFont = 42;
        float textSize = 0.03;
        int npoints = 300;
        
        // Add lines in the stat box
        TLatex *myt = new TLatex(0, 0, TString::Format("#chi^{2} / ndf = %g / %i", fTOFcombined.GetChisquare(), npoints));
        myt->SetTextFont(textFont);
        myt->SetTextSize(textSize);
        
        TLatex *myt0 = new TLatex(0, 0, TString::Format("%s = %g #pm %g", fTOFcombined.GetParName(0), (fTOFcombined.GetParameter(0))/(hTOFm2->GetBinWidth(1)), (fTOFcombined.GetParError(0))/((hTOFm2->GetBinWidth(1)))));
        myt0->SetTextFont(textFont);
        myt0->SetTextSize(textSize);
        
        TLatex *myt1 = new TLatex(0, 0, TString::Format("%s = %g #pm %g", fTOFcombined.GetParName(1), fTOFcombined.GetParameter(1), fTOFcombined.GetParError(1)));
        myt1->SetTextFont(textFont);
        myt1->SetTextSize(textSize);
        
        TLatex *myt2 = new TLatex(0, 0, TString::Format("%s = %g #pm %g", fTOFcombined.GetParName(2), fTOFcombined.GetParameter(2), fTOFcombined.GetParError(2)));
        myt2->SetTextFont(textFont);
        myt2->SetTextSize(textSize);
        
        TLatex *myt3 = new TLatex(0, 0, TString::Format("%s = %g #pm %g", fTOFcombined.GetParName(3), fTOFcombined.GetParameter(3), fTOFcombined.GetParError(3)));
        myt3->SetTextFont(textFont);
        myt3->SetTextSize(textSize);
        
        list->Add(myt);
        list->Add(myt0);
        list->Add(myt1);
        list->Add(myt2);
        list->Add(myt3);
        
        // the following line is needed to avoid that the automatic redrawing of stats
        hTOFm2->SetStats(0);
        
        c1->Modified();
        c1->Update();
        
        c1->Print(Form("%s_%s/TOFm2fit_%s_bin_%i_%i.pdf",outd, part, part,binsx_n[ibin-1],binsx_n[ibin]));
        
//        yield    = (fTOFsignal.Integral(3.0,4.5))/(hTOFm2->GetBinWidth(1));
//        yieldErr = TMath::Sqrt(yield);
//        rawSpectrum->SetBinContent(ibin+nbins_lowp, yield);
//        rawSpectrum->SetBinError(ibin+nbins_lowp, yieldErr);
        
        rawSpectrum->SetBinContent(ibin+nbins_lowp, (fTOFcombined.GetParameter(0))/(hTOFm2->GetBinWidth(1)));
        rawSpectrum->SetBinError(ibin+nbins_lowp, (fTOFcombined.GetParError(0))/(hTOFm2->GetBinWidth(1)));
        
    }

}

void WriteHistos(TH1F* hist){
    
    TFile* fileout = new TFile(Form("%s_%s.root",fout,part),"RECREATE");
    hist->Write();
    fileout->Close();
    
}
