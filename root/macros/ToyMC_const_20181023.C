// Plotting macro for the toy Monte Carlo 2 (S(4,2) with only non-flow)
// Version: 2018-10-17

// Save the figure in pdf, eps and png formats:
Bool_t bSaveFigures = kTRUE;

// Functions:
void SetStyle(Bool_t graypalette=kFALSE);
void myPadSetUp(TPad *currentPad, float currentLeft=0.11, float currentTop=0.04, float currentRight=0.04, float currentBottom=0.15);
void DrawLogo (Int_t logo=0, Double_t xmin =  0.28, Double_t ymin= 0.68);
void FakeHistosOnlyForExample(TH1*&hstat, TH1*&hsyst, TH1*&hsystCorr);
void LoadLibs();

// Main:
void ToyMC_const_20181023()
{
// Load necessary librairies
  LoadLibs();
// Set the default style
 SetStyle();

// Prepare Figure, please stick to the default canvas size(s) unless absolutely necessary in your case
// a) Rectangular:
 TCanvas *cfig = new TCanvas("cfig", "Alice Figure Template", 800, 600);
// b) Square:
 // TCanvas *cfig = new TCanvas("cfig", "Alice Figure Template", 800, 800);
// cfig->SetLogy();

// Set titles etc..
 TH1 *h = cfig->DrawFrame(-100,-1e-05,5100,5e-05);
// Choose titles for the axis:
 const char *xTitle = "Multiplicity";
 const char *yTitle = "SC(4,2)";
// Set titles:
 h->SetXTitle(xTitle);
 h->SetYTitle(yTitle);
// Set title offsets:
 h->GetXaxis()->SetTitleOffset(0.98844);
 h->GetYaxis()->SetTitleOffset(1.1); //h->GetYaxis()->SetTitleOffset(0.744);

//======================================================================================//

// SC(4,2) with the usual expression:
  Double_t x_SC42_Usual[6] = {100., 1000., 250., 500., 5000., 750.};
  Double_t y_SC42_Usual[6] = {0.000000072627546, -0.000000022019113, -0.000000106064182, -0.000000037657444, -0.000000006805611, -0.000000021593101};
  Double_t xErr_SC42_Usual[6] = {0.};
  Double_t yErr_SC42_Usual[6] = {0.000000580769307, 0.000000031396155, 0.000000144281435, 0.000000051867890, 0.000000005722975, 0.000000039504665};
 Int_t nPoints_SC42_Usual = sizeof(x_SC42_Usual)/sizeof(Double_t)+1;
 TGraphErrors *ge_SC42_Usual = new TGraphErrors(nPoints_SC42_Usual-1,x_SC42_Usual,y_SC42_Usual,xErr_SC42_Usual,yErr_SC42_Usual);
 ge_SC42_Usual->SetMarkerStyle(kOpenCircle);
 ge_SC42_Usual->SetMarkerColor(kBlue+1);
 ge_SC42_Usual->SetMarkerSize(1.2);
 ge_SC42_Usual->SetLineColor(kBlue+1);
 ge_SC42_Usual->SetFillColor(kBlue-8);
 ge_SC42_Usual->SetFillStyle(3012);
 ShiftAlongXaxis(ge_SC42_Usual,-50);

//======================================================================================//

// SC(4,2) with the alternative expression:
  Double_t x_SC42_Alternative[6] = {100., 1000., 250., 500., 5000., 750.};
  Double_t y_SC42_Alternative[6] = {0.000038026760316, 0.000003596688981, 0.000014570955210, 0.000007230389625, 0.000000713852324, 0.000004808893687};
 Double_t xErr_SC42_Alternative[6] = {0.};
  Double_t yErr_SC42_Alternative[6] = {0.000004061497140, 0.000000380389244, 0.000001543837802, 0.000000763955998, 0.000000075455819, 0.000000508440191};
 Int_t nPoints_SC42_Alternative = sizeof(x_SC42_Alternative)/sizeof(Double_t)+1;

 TGraphErrors *ge_SC42_Alternative = new TGraphErrors(nPoints_SC42_Alternative-1,x_SC42_Alternative,y_SC42_Alternative,xErr_SC42_Alternative,yErr_SC42_Alternative);
 ge_SC42_Alternative->SetMarkerStyle(kOpenSquare);
 ge_SC42_Alternative->SetMarkerColor(kRed);
 ge_SC42_Alternative->SetMarkerSize(1.);
 ge_SC42_Alternative->SetLineColor(kRed);
 ge_SC42_Alternative->SetFillColor(kRed-10);
 ge_SC42_Alternative->SetFillStyle(3001);
 ge_SC42_Alternative->SetFillColor(kRed-8);
 ge_SC42_Alternative->SetFillStyle(3008);
 ShiftAlongXaxis(ge_SC42_Alternative, 0);

// Corresponding fit
  TF1 *fFit_Alternative = new TF1("fFit_Alternative", "[0]/(TMath::Power(x, 2.)) + [1]/(TMath::Power(x, 1.)) + [2]", 0., 5100.);
  //fFit_Alternative2->SetParLimits(0, 0., 0.);
  fFit_Alternative->SetParName(0, "#alpha");
  fFit_Alternative->SetParName(1, "#beta");
  fFit_Alternative->SetParName(2, "#gamma");

  ge_SC42_Alternative->Fit("fFit_Alternative", "MR");
  fFit_Alternative->SetLineColor(kCyan+1);

//======================================================================================//

 // Draw the logo:
 //  0: Just "ALICE" (for final data), to be added only if ALICE does not appear otherwise (e.g. in the legend)
 //  >0: ALICE Preliminary
 // DrawLogo(1, 0.59, 0.81);

 // You should always specify the colliding system:
 // NOTATION: pp, p-Pb, Pb-Pb.
 // Don't forget to use #sqrt{s_{NN}} for p-Pb and Pb-Pb
 // You can change the position of this with
 /*
 TLatex *text = new TLatex (45, 2.89e-6,"Pb-Pb #sqrt{#it{s}}_{NN} = 2.76 TeV");
 text->SetTextSizePixels(24);
 text->Draw();
 */

 // Draw line at y=0:
 TLine *line0 = new TLine(-100., 0., 5100., 0.);
 line0->SetLineColor(1);
 line0->SetLineWidth(2);
 line0->SetLineStyle(1);
 line0->Draw("lsame");

 // Legend
 TLegend *legend = new TLegend(0.4,0.6,0.7,0.8);
 legend->SetFillStyle(0); // white legend background
 legend->SetTextSize(0.035);
 //legend->SetBorderSize(0.0);
 //legend->SetTextFont(22);
 legend->SetMargin(0.3);
 legend->SetHeader("Toy MC, Nevents = 1e6, v_{2} = 0.15, v_{4} = 0.1");
 legend->AddEntry(ge_SC42_Usual,"Usual","p");
 legend->AddEntry(ge_SC42_Alternative,"Alternative","p");
  legend->AddEntry(fFit_Alternative, "Fit: #alpha/M^{2} + #beta/M + #gamma", "l");

 // Final drawing (ordering matters!):
 //legend_ALICE->Draw("same");
 legend->Draw("same");
 ge_SC42_Usual->Draw("psame,Z");
 ge_SC42_Alternative->Draw("psame,Z");
 /*ge_SysError_SC32->Draw("2same");
 ge_SC32->Draw("psame,Z");
 ge_SysError_SC42->Draw("2same");
 ge_SC42->Draw("psame,Z");*/
  gStyle->SetOptFit();

 // Save or die:
 if(bSaveFigures)
 {
  cfig->SaveAs("ToyMC_SC42_const_2018-10-23.pdf");
  //cfig->SaveAs("ToyMC_SC42_const_2018-10-23.eps");
  cfig->SaveAs("ToyMC_SC42_const_2018-10-23.png");
 } // if(bSaveFigures)

} // Figure_1_20161006()

//===================================================================================================

void ShiftAlongXaxis(TGraphErrors *ge, Double_t shift)
{
 // Shift original TGraphErrors along x-axis by Double_t shift.

 if(!ge){cout<<"!ge"<<endl;exit(0);}

 Int_t nPoints = ge->GetN();
 Double_t x = 0.;
 Double_t y = 0.;
 for(Int_t p=0;p<nPoints;p++)
 {
  ge->GetPoint(p,x,y);
  x+=shift;
  ge->SetPoint(p,x,y);
 } // end of for(Int_t p=0;p<nPoints;p++)

} // end of void ShiftAlongXaxis(TGraphErrors *ge, Double_t shift)

//===================================================================================================

// Functions from ALICE Figures Template macro:

void LoadLibs() {
  gSystem->Load("libCore.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libPhysics.so");
  gSystem->Load("libVMC");
  gSystem->Load("libTree");
  gSystem->Load("libMinuit");
  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libAOD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  gSystem->Load("libCORRFW");
  gSystem->Load("libPWGTools");
}

void DrawLogo (Int_t logo, Double_t xmin, Double_t ymin) {

  // Logo is not needed anymore, now we only write alice preliminary
  // Logo:
  // 0: Justr writes "ALICE" (for final data)
  // Anything eles: writes "ALICE Preliminary"

  TLatex *   tex = new TLatex(xmin,ymin, logo ? "ALICE Preliminary" : "ALICE");
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->Draw();

  // OLD logo
  //  TPad * currentPad = gPad;
  // Double_t AliLogo_LowX =xmin;
  // Double_t AliLogo_LowY = ymin;
  // Double_t AliLogo_Height = size;
  // //ALICE logo is a  file that is 821x798 pixels->should be wider than a square
  // Double_t AliLogo_Width  = (821./798.) * AliLogo_Height * gPad->GetWh() / gPad->GetWw();

  // TPad *myPadLogo = new TPad("myPadLogo", "Pad for ALICE Logo",AliLogo_LowX,AliLogo_LowY,AliLogo_LowX+AliLogo_Width,AliLogo_LowY+AliLogo_Height);
  // myPadSetUp(myPadLogo,0,0,0,0);
  // //myPadLogo->SetFixedAspectRatio(1);
  // myPadLogo->Draw();
  // myPadLogo->cd();
  // if (logo == 0) {
  //   myPadLogo->SetFillColor(2); // color to first figure out where is the pad then comment !
  // } else if (logo == 1){
  //   TASImage *myAliceLogo = new TASImage(performanceLogoPath);
  //   myAliceLogo->Draw();
  // } else if (logo == 2) {
  //   TASImage *myAliceLogo = new TASImage(preliminaryLogoPath);
  //   myAliceLogo->Draw();
  // }
  // // go back to the old pad
  // currentPad->cd();

}

void myPadSetUp(TPad *currentPad, float currentLeft, float currentTop, float currentRight, float currentBottom){
  currentPad->SetLeftMargin(currentLeft);
  currentPad->SetTopMargin(currentTop);
  currentPad->SetRightMargin(currentRight);
  currentPad->SetBottomMargin(currentBottom);
  return;
}

void SetStyle(Bool_t graypalette) {
  cout << "Setting style!" << endl;

  gStyle->Reset("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  if(graypalette) gStyle->SetPalette(8,0);
  else gStyle->SetPalette(1);
  gStyle->SetCanvasColor(10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetFrameFillColor(kWhite);
  gStyle->SetPadColor(10);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetHistLineWidth(1);
  gStyle->SetHistLineColor(kRed);
  gStyle->SetFuncWidth(2);
  gStyle->SetFuncColor(kCyan+1);
  gStyle->SetLineWidth(2);
  gStyle->SetLabelSize(0.045,"xyz");
  gStyle->SetLabelOffset(0.01,"y");
  gStyle->SetLabelOffset(0.01,"x");
  gStyle->SetLabelColor(kBlack,"xyz");
  gStyle->SetTitleSize(0.05,"xyz");
  gStyle->SetTitleOffset(1.25,"y");
  gStyle->SetTitleOffset(1.2,"x");
  gStyle->SetTitleFillColor(kWhite);
  gStyle->SetTextSizePixels(26);
  gStyle->SetTextFont(42);
  //  gStyle->SetTickLength(0.04,"X");  gStyle->SetTickLength(0.04,"Y");

  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(kWhite);
  //  gStyle->SetFillColor(kWhite);
  gStyle->SetLegendFont(42);

}

void FakeHistosOnlyForExample(TH1* &hstat, TH1* &hsyst, TH1*&hsystCorr) {

  TF1 * fExpo = new TF1 ("fExpo", "expo");
  fExpo->SetParameters(10, -0.3);
  hstat     = new TH1F("hstat", "hstat", 100, 0, 10);
  hsyst     = new TH1F("hsyst", "hsyst", 100, 0, 10);
  hsystCorr = new TH1F("hsystCorr", "hsystCorr", 100, 0, 10);
  hstat     ->Sumw2();
  hsyst     ->Sumw2();
  hsystCorr ->Sumw2();
  hstat->FillRandom("fExpo",20000);
  Int_t nbinx = hstat->GetNbinsX();

  for(Int_t ibinx = 1; ibinx <= nbinx; ibinx++){
    hsyst->SetBinContent (ibinx, hstat->GetBinContent(ibinx));
			  hsyst->SetBinError   (ibinx, hstat->GetBinContent(ibinx)*0.08);
			  hsystCorr->SetBinContent (ibinx, hstat->GetBinContent(ibinx));
			  hsystCorr->SetBinError   (ibinx, hstat->GetBinContent(ibinx)*0.15);
  }

}
