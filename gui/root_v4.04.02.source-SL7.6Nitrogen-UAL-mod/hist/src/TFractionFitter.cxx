// @(#)root/hist:$Name:  $:$Id: TFractionFitter.cxx,v 1.10 2004/08/18 12:29:44 brun Exp $
// Author: Frank Filthaut filthaut@hef.kun.nl  20/05/2002
// with additions by Bran Wijngaarden <dwijngaa@hef.kun.nl>

///////////////////////////////////////////////////////////////////////////////
//
// Fits MC fractions to data histogram (a la HMCMLL, see R. Barlow and C. Beeston,
// Comp. Phys. Comm. 77 (1993) 219-228, and http://www.hep.man.ac.uk/~roger/hfrac.f).
//
// The virtue of this fit is that it takes into account both data and Monte Carlo
// statistical uncertainties. The way in which this is done is through a standard
// likelihood fit using Poisson statistics; however, the template (MC) predictions
// are also varied within statistics, leading to additional contributions to the
// overall likelihood. This leads to many more fit parameters (one per bin per
// template), but the minimisation with respect to these additional parameters is
// done analytically rather than introducing them as formal fit parameters. Some
// special care needs to be taken in the case of bins with zero content. For more
// details please see the original publication cited above.
//
// An example application of this fit is given below. For a TH1* histogram
// ("data") fitted as the sum of three Monte Carlo sources ("mc"):
//
// {
//   TH1F *data;                              //data histogram
//   TH1F *mc0;                               // first MC histogram
//   TH1F *mc1;                               // second MC histogram
//   TH1F *mc2;                               // third MC histogram
//   ....                                     // retrieve histograms
//   TObjArray *mc = new TObjArray(3);        // MC histograms are put in this array
//   mc->Add(mc0);
//   mc->Add(mc1);
//   mc->Add(mc2);
//   TFractionFitter* fit = new TFractionFitter(data, mc); // initialise
//   fit->Constrain(1,0.0,1.0);               // constrain fraction 1 to be between 0 and 1
//   fit->SetRangeX(1,15);                    // use only the first 15 bins in the fit
//   Int_t status = fit->Fit();               // perform the fit
//   cout << "fit status: " << status << endl;
//   if (status == 0) {                       // check on fit status
//     TH1F* prediction = (TH1F*) fit->GetPlot();
//     data->Draw("Ep");
//     result->Draw("same");
//   }
// }
//
//
// Instantiation
// =============
// A fit object is instantiated through
//     TFractionFitter* fit = new TFractionFitter(data, mc);
// A number of basic checks (intended to ensure that the template histograms
// represent the same "kind" of distribution as the data one) are carried out.
// The TVirtualFitter object is then addressed and all fit parameters (the
// template fractions) declared (initially unbounded).
//
// Applying constraints
// ====================
// Fit parameters can be constrained through
//     fit->Constrain(parameter #, lower bound, upper bound);
// Setting lower bound = upper bound = 0 removes the constraint (a la Minuit);
// however, a function
//     fit->Unconstrain(parameter #)
// is also provided to simplify this.
//
// Setting parameter values
// ========================
// The function
//     TVirtualFitter* vFit = fit->GetFitter();
// is provided for direct access to the TVirtualFitter object. This allows to
// set and fix parameter values, and set step sizes directly.
//
// Restricting the fit range
// =========================
// The fit range can be restricted through
//     fit->SetRangeX(first bin #, last bin #);
// and freed using
//     fit->ReleaseRangeX();
// For 2D histograms the Y range can be similarly restricted using
//     fit->SetRangeY(first bin #, last bin #);
//     fit->ReleaseRangeY();
// and for 3D histograms also
//     fit->SetRangeZ(first bin #, last bin #);
//     fit->ReleaseRangeZ();
//
// Weights histograms
// ==================
// Weights histograms (for a motivation see the above publication) can be specified
// for the individual MC sources through
//     fit->SetWeight(parameter #, pointer to weights histogram);
// and unset by specifying a null pointer.
//
// Obtaining fit results
// =====================
// The fit is carried out through
//     Int_t status = fit->Fit();
// where  status  is the code returned from the "MINIMIZE" command. For fits
// that converged, parameter values and errors can be obtained through
//     fit->GetResult(parameter #, value, error);
// and the histogram corresponding to the total Monte Carlo prediction (which
// is not the same as a simple weighted sum of the input Monte Carlo distributions)
// can be obtained by
//     TH1* result = fit->GetPlot();
//
// Using different histograms
// ==========================
// It is possible to change the histogram being fitted through
//     fit->SetData(TH1* data);
// and to change the template histogram for a given parameter number through
//     fit->SetMC(parameter #, TH1* MC);
// This can speed up code in case of multiple data or template histograms;
// however, it should be done with care as any settings are taken over from
// the previous fit. In addition, neither the dimensionality nor the numbers of
// bins of the histograms should change (in that case it is better to instantiate
// a new TFractionFitter object).
//
// Errors
// ======
// Any serious inconsistency results in an error.
//
///////////////////////////////////////////////////////////////////////////////

#include "Riostream.h"
#include "TH1.h"
#include "TMath.h"
#include "TClass.h"

#include "TFractionFitter.h"

TVirtualFitter *fractionFitter=0;

ClassImp(TFractionFitter)

//______________________________________________________________________________
TFractionFitter::TFractionFitter() :
  fFitDone(kFALSE),fData(0), fPlot(0)  {
  // TFractionFitter default constructor.

  fractionFitter = 0;
  fIntegralMCs   = 0;
  fFractions     = 0;

  fNpfits        = 0;
  fNDF           = 0;
  fChisquare     = 0;
}

//______________________________________________________________________________
TFractionFitter::TFractionFitter(TH1* data, TObjArray  *MCs) :
  fFitDone(kFALSE), fChisquare(0), fPlot(0)  {
  // TFractionFitter constructor. Does a complete initialisation (including
  // consistency checks, default fit range as the whole histogram but without
  // under- and overflows, and declaration of the fit parameters). Note that
  // the histograms are not copied, only references are used.
  // Arguments:
  //     data: histogram to be fitted
  //     MCs:  array of TH1* corresponding template distributions

  fData = data;
  // Default: include all of the histogram (but without under- and overflows)
  fLowLimitX = 1;
  fHighLimitX = fData->GetNbinsX();
  if (fData->GetDimension() > 1) {
     fLowLimitY = 1;
     fHighLimitY = fData->GetNbinsY();
     if (fData->GetDimension() > 2) {
        fLowLimitZ = 1;
        fHighLimitZ = fData->GetNbinsZ();
     }
  }
  fNpar = MCs->GetEntries();
  Int_t par;
  for (par = 0; par < fNpar; ++par) {
     fMCs.Add(MCs->At(par));
     // Histogram containing template prediction
     TString s = Form("Prediction for MC sample %i",par);
     TH1* pred = (TH1*) ((TH1*)MCs->At(par))->Clone(s);
     pred->SetTitle(s);
     fAji.Add(pred);
  }
  fIntegralMCs = new Double_t[fNpar];
  fFractions = new Double_t[fNpar];

  CheckConsistency();
  fWeights.Expand(fNpar);

  fractionFitter = TVirtualFitter::Fitter(this, fNpar);
  fractionFitter->Clear();
  fractionFitter->SetObjectFit(this);
  fractionFitter->SetFCN(TFractionFitFCN);

  Double_t defaultFraction = 1.0/((Double_t)fNpar);
  Double_t defaultStep = 0.01;
  for (par = 0; par < fNpar; ++par) {
     TString name("frac"); name += par;
     fractionFitter->SetParameter(par, name.Data(), defaultFraction, defaultStep, 0, 0);
  }
}

//______________________________________________________________________________
TFractionFitter::~TFractionFitter() {
  // TFractionFitter default destructor

  delete fractionFitter;
  delete[] fIntegralMCs;
  delete[] fFractions;
}

//______________________________________________________________________________
void TFractionFitter::SetData(TH1* data) {
  // Change the histogram to be fitted to. Notes:
  // - Parameter constraints and settings are retained from a possible previous fit.
  // - Modifying the dimension or number of bins results in an error (in this case
  //   rather instantiate a new TFractionFitter object)

  fData = data;
  fFitDone = kFALSE;
  CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter::SetMC(Int_t parm, TH1* MC) {
  // Change the histogram for template number <parm>. Notes:
  // - Parameter constraints and settings are retained from a possible previous fit.
  // - Modifying the dimension or number of bins results in an error (in this case
  //   rather instantiate a new TFractionFitter object)

  CheckParNo(parm);
  fMCs.RemoveAt(parm);
  fMCs.AddAt(MC,parm);
  fFitDone = kFALSE;
  CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter::SetWeight(Int_t parm, TH1* weight) {
  // Set bin by bin weights for template number <parm> (the parameter numbering
  // follows that of the input template vector).
  // Weights can be "unset" by passing a null pointer.
  // Consistency of the weights histogram with the data histogram is checked at
  // this point, and an error in case of problems.

  CheckParNo(parm);
  if (fWeights[parm]) {
     fWeights.RemoveAt(parm);
  }
  if (weight) {
     if (weight->GetNbinsX() != fData->GetNbinsX() ||
	 (fData->GetDimension() > 1 && weight->GetNbinsY() != fData->GetNbinsY()) ||
	 (fData->GetDimension() > 2 && weight->GetNbinsZ() != fData->GetNbinsZ())) {
        Error("SetWeight","Inconsistent weights histogram for source %d", parm);
        return;
     }
     TString ts = "weight hist: "; ts += weight->GetName();
     fWeights.AddAt(weight,parm);
  }
}

//______________________________________________________________________________
TVirtualFitter* TFractionFitter::GetFitter() const {
  // Give direct access to the underlying minimisation class. This can be
  // used e.g. to modify parameter values or step sizes.

  return fractionFitter;
}

//______________________________________________________________________________
void TFractionFitter::CheckParNo(Int_t parm) const {
  // Function for internal use, checking parameter validity
  // An invalid parameter results in an error.

  if (parm < 0 || parm > fNpar) {
     Error("CheckParNo","Invalid parameter number %d",parm);
  }
}

//______________________________________________________________________________
void TFractionFitter::SetRangeX(Int_t low, Int_t high) {
  // Set the X range of the histogram to be used in the fit.
  // Use ReleaseRangeX() to go back to fitting the full histogram.
  // The consistency check ensures that no empty fit range occurs (and also
  // recomputes the bin content integrals).
  // Arguments:
  //     low:  lower X bin number
  //     high: upper X bin number

  if (low <=0 || high <= 0) {
     Error("SetRangeX","Invalid fit range");
     return;
  }
  fLowLimitX = (low > 0) ? low : 1;
  fHighLimitX = (high <= fData->GetNbinsX()) ? high : fData->GetNbinsX();
  CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter::ReleaseRangeX() {
  // Release restrictions on the X range of the histogram to be used in the fit.

  fLowLimitX = 1;
  fHighLimitX = fData->GetNbinsX();
  CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter::SetRangeY(Int_t low, Int_t high) {
  // Set the Y range of the histogram to be used in the fit (2D or 3D histograms only).
  // Use ReleaseRangeY() to go back to fitting the full histogram.
  // The consistency check ensures that no empty fit range occurs (and also
  // recomputes the bin content integrals).
  // Arguments:
  //     low:  lower Y bin number
  //     high: upper Y bin number

  if (fData->GetDimension() < 2) {
     Error("SetRangeY","Y range cannot be set for 1D histogram");
     return;
  }
  if (low <=0 || high <= 0) {
     Error("SetRangeY","Invalid fit range");
     return;
  }

  fLowLimitY = (low > 0) ? low : 1;
  fHighLimitY = (high <= fData->GetNbinsY()) ? high : fData->GetNbinsY();
  CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter::ReleaseRangeY() {
  // Release restrictions on the Y range of the histogram to be used in the fit.

  fLowLimitY = 1;
  fHighLimitY = fData->GetNbinsY();
  CheckConsistency();
}


//______________________________________________________________________________
void TFractionFitter::SetRangeZ(Int_t low, Int_t high) {
  // Set the Z range of the histogram to be used in the fit (3D histograms only).
  // Use ReleaseRangeY() to go back to fitting the full histogram.
  // The consistency check ensures that no empty fit range occurs (and also
  // recomputes the bin content integrals).
  // Arguments:
  //     low:  lower Y bin number
  //     high: upper Y bin number

  if (fData->GetDimension() < 3) {
     Error("SetRangeZ","Z range cannot be set for 1D or 2D histogram");
     return;
  }

  if (low <=0 || high <= 0) {
     Error("SetRangeZ","Invalid fit range");
     return;
  }

  fLowLimitZ = (low > 0) ? low : 1;
  fHighLimitZ = (high <= fData->GetNbinsZ()) ? high : fData->GetNbinsZ();
  CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter::ReleaseRangeZ() {
  // Release restrictions on the Z range of the histogram to be used in the fit.

  fLowLimitZ = 1;
  fHighLimitZ = fData->GetNbinsZ();
  CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter::Constrain(Int_t parm, Double_t low, Double_t high) {
  // Constrain the values of parameter number <parm> (the parameter numbering
  // follows that of the input template vector).
  // Use UnConstrain() to remove this constraint.

  CheckParNo(parm);
  Double_t plist[3];
  plist[0] = (Double_t) parm;
  plist[1] = low;
  plist[2] = high;
  fractionFitter->ExecuteCommand("SET LIMIT", plist, 3);
}

//______________________________________________________________________________
void TFractionFitter::UnConstrain(Int_t parm) {
  // Remove the constraints on the possible values of parameter <parm>.

  CheckParNo(parm);
  Double_t plist[3];
  plist[0] = (Double_t) parm;
  plist[1] = 0;
  plist[2] = 0;
  fractionFitter->ExecuteCommand("SET LIMIT", plist, 3);
}

//______________________________________________________________________________
void TFractionFitter::CheckConsistency() {
  // Function used internally to check the consistency between the
  // various histograms. Checks are performed on nonexistent or empty
  // histograms, the precise histogram class, and the number of bins.
  // In addition, integrals over the "allowed" bin ranges are computed.
  // Any inconsistency results in a error.

  if (! fData) {
     Error("CheckConsistency","Nonexistent data histogram");
     return;
  }
  Int_t minX, maxX, minY, maxY, minZ, maxZ;
  Int_t x,y,z,par;
  GetRanges(minX, maxX, minY, maxY, minZ, maxZ);
  fIntegralData = 0;
  fNpfits = 0;
  for (z = minZ; z <= maxZ; ++z) {
     for (y = minY; y <= maxY; ++y) {
        for (x = minX; x <= maxX; ++x) {
	   fNpfits++;
	   fIntegralData += fData->GetBinContent(x, y, z);
        }
     }
  }
  if (fIntegralData <= 0) {
     Error("CheckConsistency","Empty data histogram");
     return;
  }
  TClass* cl = fData->Class();

  fNDF = fNpfits - fNpar;

  if (fNpar < 2) {
     Error("CheckConsistency","Need at least two MC histograms");
     return;
  }

  for (par = 0; par < fNpar; ++par) {
     TH1 *h = (TH1*)fMCs.At(par);
     if (! h) {
        Error("CheckConsistency","Nonexistent MC histogram for source #%d",par);
        return;
     }
     if ((! h->Class()->InheritsFrom(cl)) || h->GetNbinsX() != fData->GetNbinsX() ||
	 (fData->GetDimension() > 1 && h->GetNbinsY() != fData->GetNbinsY()) ||
	 (fData->GetDimension() > 2 && h->GetNbinsZ() != fData->GetNbinsZ())) {
        Error("CheckConsistency","Histogram inconsistency for source #%d",par);
        return;
     }
     fIntegralMCs[par] = 0;
     for (z = minZ; z <= maxZ; ++z) {
        for (y = minY; y <= maxY; ++y) {
	   for (x = minX; x <= maxX; ++x) {
	      fIntegralMCs[par] += h->GetBinContent(x, y, z);
	   }
        }
     }
     if (fIntegralMCs[par] <= 0) {
        Error("CheckConsistency","Empty MC histogram #%d",par);
     }
  }
}

//______________________________________________________________________________
Int_t TFractionFitter::Fit() {
  // Perform the fit with the default UP value.
  // The value returned is the minimisation status.

  Double_t plist[1];
  plist[0] = 0.5;
  // set the UP value to 0.5
  fractionFitter->ExecuteCommand("SET ERRDEF",plist,1);
  // remove any existing output histogram
  if (fPlot) {
     delete fPlot; fPlot = 0;
  }

  // Make sure the correct likelihood computation is used
  fractionFitter->SetObjectFit(this);

  // fit
  Int_t status = fractionFitter->ExecuteCommand("MINIMIZE",0,0);
  if (status == 0) fFitDone = kTRUE;

  // determine goodness of fit
  ComputeChisquareLambda();

  return status;
}

//______________________________________________________________________________
void TFractionFitter::ErrorAnalysis(Double_t UP) {
  // Set UP to the given value (see class TMinuit), and perform a MINOS minimisation.

  if (! fFitDone) {
     Error("ErrorAnalysis","Fit not yet performed");
     return;
  }

  // Make sure the correct likelihood computation is used
  fractionFitter->SetObjectFit(this);

  Double_t plist[1];
  plist[0] = UP > 0 ? UP : 0.5;
  fractionFitter->ExecuteCommand("SET ERRDEF",plist,1);
  Int_t status = fractionFitter->ExecuteCommand("MINOS",0,0);
  if (status != 0) {
      Error("ErrorAnalysis","Error return from MINOS: %d",status);
  }
}

//______________________________________________________________________________
void TFractionFitter::GetResult(Int_t parm, Double_t& value, Double_t& error) const {
  // Obtain the fit result for parameter <parm> (the parameter numbering
  // follows that of the input template vector).

  CheckParNo(parm);
  if (! fFitDone) {
      Error("GetResult","Fit not yet performed");
      return;
   }
  char parname[100];
  Double_t vlow, vhigh;

  fractionFitter->GetParameter(parm, parname, value, error, vlow, vhigh);
}

//______________________________________________________________________________
TH1* TFractionFitter::GetPlot() {
  // Return the "template prediction" corresponding to the fit result (this is not
  // the same as the weighted sum of template distributions, as template statistical
  // uncertainties are taken into account).
  // Note that the name of this histogram will simply be the same as that of the
  // "data" histogram, prefixed with the string "Fraction fit to hist: ".

  if (! fFitDone) {
      Error("GetPlot","Fit not yet performed");
      return 0;
  }
  if (! fPlot) {
     Double_t plist[1];
     plist[0] = 3;
     fractionFitter->ExecuteCommand("CALL FCN", plist, 1);
  }
  return fPlot;
}

//______________________________________________________________________________
void TFractionFitter::GetRanges(Int_t& minX, Int_t& maxX, Int_t& minY, Int_t& maxY,
				Int_t& minZ, Int_t& maxZ) const {
  // Used internally to obtain the bin ranges according to the dimensionality of
  // the histogram and the limits set by hand.

  if (fData->GetDimension() < 2) {
    minY = maxY = minZ = maxZ = 0;
    minX = fLowLimitX;
    maxX = fHighLimitX;
  } else if (fData->GetDimension() < 3) {
    minZ = maxZ = 0;
    minX = fLowLimitX;
    maxX = fHighLimitX;
    minY = fLowLimitY;
    maxY = fHighLimitY;
  } else {
    minX = fLowLimitX;
    maxX = fHighLimitX;
    minY = fLowLimitY;
    maxY = fHighLimitY;
    minZ = fLowLimitZ;
    maxZ = fHighLimitZ;
  }
}

//______________________________________________________________________________
void TFractionFitter::ComputeFCN(Int_t& /*npar*/, Double_t* /*gin*/,
                                 Double_t& f, Double_t* xx, Int_t flag)
{
  // Used internally to compute the likelihood value.

  // normalise the fit parameters
  Int_t bin, mc;
  Int_t minX, maxX, minY, maxY, minZ, maxZ;
  Int_t x,y,z;
  GetRanges(minX, maxX, minY, maxY, minZ, maxZ);
  for (mc = 0; mc < fNpar; ++mc) {
     Double_t tot;
     TH1 *h  = (TH1*)fMCs[mc];
     TH1 *hw = (TH1*)fWeights[mc];
     if (hw) {
        tot = 0;
	for (z = minZ; z <= maxZ; ++z) {
	   for (y = minY; y <= maxY; ++y) {
	      for (x = minX; x <= maxX; ++x) {
	         Double_t weight = hw->GetBinContent(x, y, z);
	         if (weight <= 0) {
		    Error("ComputeFCN","Invalid weight encountered for MC source %d",mc);
		    return;
	         }
	         tot += weight * h->GetBinContent(x, y, z);
	      }
	   }
	}
    } else tot = fIntegralMCs[mc];
    fFractions[mc] = xx[mc] * fIntegralData / tot;
  }

  if (flag == 3) {
     TString ts = "Fraction fit to hist: "; ts += fData->GetName();
     fPlot = (TH1*) fData->Clone(ts.Data());
     fPlot->Reset();
  }
  // likelihood computation
  Double_t result = 0;
  for (z = minZ; z <= maxZ; ++z) {
    for (y = minY; y <= maxY; ++y) {
      for (x = minX; x <= maxX; ++x) {
	bin = fData->GetBin(x, y, z);

	// Solve for the "predictions"
	int k0;
	Double_t Ti; Double_t Aki;
	FindPrediction(bin, fFractions, Ti, k0, Aki);

	Double_t prediction = 0;
	for (mc = 0; mc < fNpar; ++mc) {
	  TH1 *h  = (TH1*)fMCs[mc];
	  TH1 *hw = (TH1*)fWeights[mc];
	  Double_t binPrediction;
	  Double_t binContent = h->GetBinContent(bin);
	  Double_t weight = hw ? hw->GetBinContent(bin) : 1;
	  if (k0 >= 0 && fFractions[mc] == fFractions[k0]) {
	     binPrediction = Aki;
	  } else {
	     binPrediction = binContent > 0 ? binContent / (1+weight*fFractions[mc]*Ti) : 0;
	  }

	  prediction += fFractions[mc]*weight*binPrediction;
	  result -= binPrediction;
	  if (binContent > 0 && binPrediction > 0)
	    result += binContent*TMath::Log(binPrediction);

	  if (flag == 3) {
	     ((TH1*)fAji.At(mc))->SetBinContent(bin, binPrediction);
	  }
	}

	if (flag == 3) {
	   fPlot->SetBinContent(bin, prediction);
	}

	result -= prediction;
	Double_t found = fData->GetBinContent(bin);
	if (found > 0)
	   result += found*TMath::Log(prediction);
      }
    }
  }

  f = -result;
}

//______________________________________________________________________________
void TFractionFitter::FindPrediction(int bin, Double_t *fractions, Double_t &Ti, int& k0, Double_t &Aki) const {
  // Function used internally to obtain the template prediction in the individual bins

  // Sanity check: none of the fractions should be =0
  Int_t par;
  TH1 *hw;
  for (par = 0; par < fNpar; ++par) {
     hw = (TH1*)fWeights.At(par);
     Double_t weightedFraction = hw ?
        hw->GetBinContent(bin) * fractions[par] : fractions[par];
     if (weightedFraction == 0) {
        Error("FindPrediction","Fraction[%d] = 0!",par);
        return;
     }
  }

  // Case data = 0
  if (TMath::Nint(fData->GetBinContent(bin)) == 0) {
     Ti = 1;
     k0 = -1;
     Aki=0;
     return;
  }

  // Case one or more of the MC bin contents = 0: find largest fraction
  k0 = 0;
  TH1 *hw0 = (TH1*)fWeights.At(0);
  Double_t refWeightedFraction = hw0 ?
     hw0->GetBinContent(bin) * fractions[0] : fractions[0];
  for (par = 1; par < fNpar; ++par) {
     hw = (TH1*)fWeights.At(par);
     Double_t weightedFraction = hw ?
      hw->GetBinContent(bin) * fractions[par] : fractions[par];
     if (weightedFraction > refWeightedFraction) {
        k0 = par;
        refWeightedFraction = weightedFraction;
     }
  }
  int nMax = 1;
  Double_t contentsMax = ((TH1*)fMCs.At(k0))->GetBinContent(bin);
  for (par = 0; par < fNpar; ++par) {
     if (par == k0) continue;
     hw = (TH1*)fWeights.At(par);
     Double_t weightedFraction = hw ?
      hw->GetBinContent(bin) * fractions[par] : fractions[par];
     if (weightedFraction == refWeightedFraction) {
        nMax++;
        contentsMax += ((TH1*)fMCs.At(par))->GetBinContent(bin);
     }
  }
  Double_t Tmin = -1/refWeightedFraction;

  if (contentsMax == 0) {
     Aki = fData->GetBinContent(bin)/(1+refWeightedFraction);
     for (par = 0; par < fNpar; ++par) {
        hw = (TH1*)fWeights.At(par);
        if (par != k0) {
	   Double_t weightedFraction = hw ?
	     hw->GetBinContent(bin) * fractions[par] : fractions[par];
	   if (weightedFraction != refWeightedFraction)
	     Aki -= ((TH1*)fMCs.At(par))->GetBinContent(bin)*weightedFraction/ (refWeightedFraction - weightedFraction);
        }
     }
     if (Aki > 0) {
        if (nMax) Aki /= nMax;
        Ti = Tmin;
        return;
     }
  }
  k0 = -1;

  // Case of nonzero histogram contents: solve for Ti using Newton's method
  Ti = 0;
  for (Double_t step = 0.2;;) {
     if (Ti >= 1 || Ti < Tmin) {
        step /= 10;
        Ti = 0;
     }
     Double_t Function   = - fData->GetBinContent(bin)/(1-Ti);
     Double_t Derivative = Function / (1-Ti);
     for (par = 0; par < fNpar; ++par) {
        TH1 *h = (TH1*)fMCs.At(par);
        hw = (TH1*)fWeights.At(par);
        Double_t weightedFraction = hw ?
	  hw->GetBinContent(bin) * fractions[par] : fractions[par];
        Double_t D = 1/(Ti+1/weightedFraction);
        Function   += h->GetBinContent(bin)*D;
        Derivative -= h->GetBinContent(bin)*D*D;
     }
     if (TMath::Abs(Function) < 1e-12) break;
     Double_t Delta = -Function/Derivative;
     if (TMath::Abs(Delta) > step)
        Delta = (Delta > 0) ? step : -step;
     Ti += Delta;
     if (TMath::Abs(Delta) < 1e-13) break;
  }

  return;
}


//______________________________________________________________________________
void TFractionFitFCN(Int_t& npar, Double_t* gin, Double_t& f, Double_t* par, Int_t flag) {
  // Function called by the minimisation package. The actual functionality is passed
  // on to the TFractionFitter::ComputeFCN member function.

  TFractionFitter* fitter = dynamic_cast<TFractionFitter*>(fractionFitter->GetObjectFit());
  if (!fitter) {
     fitter->Error("TFractionFitFCN","Invalid fit object encountered!");
     return;
  }
  fitter->ComputeFCN(npar, gin, f, par, flag);
}


//______________________________________________________________________________
Double_t TFractionFitter::GetChisquare() const
{
// Return the likelihood ratio Chi-squared (chi2) for the fit.
// The value is computed when the fit is executed successfully.
// Chi2 calculation is based on the "likelihood ratio" lambda,
// lambda = L(y;n) / L(m;n),
// where L(y;n) is the likelihood of the fit result <y> describing the data <n>
// and L(m;n) is the likelihood of an unknown "true" underlying distribution
// <m> describing the data <n>. Since <m> is unknown, the data distribution is
// used instead,
// lambda = L(y;n) / L(n;n).
// Note that this ratio is 1 if the fit is perfect. The chi2 value is then
// computed according to
// chi2 = -2*ln(lambda).
// This parameter can be shown to follow a Chi-square distribution. See for
// example S. Baker and R. Cousins, "Clarification of the use of chi-square
// and likelihood functions in fits to histograms", Nucl. Instr. Meth. A221, 
// pp. 437-442 (1984)

   return fChisquare;
}

//______________________________________________________________________________
 Int_t TFractionFitter::GetNDF() const
{
// return the number of degrees of freedom in the fit
// the fNDF parameter has been previously computed during a fit.
// The number of degrees of freedom corresponds to the number of points
// used in the fit minus the number of templates.

   if (fNDF == 0) return fNpfits-fNpar;
   return fNDF;
}

//______________________________________________________________________________
 Double_t TFractionFitter::GetProb() const
{
// return the fit probability
   
   Int_t ndf = fNpfits - fNpar;
   if (ndf <= 0) return 0;
   return TMath::Prob(fChisquare,ndf);
}

//______________________________________________________________________________
 void TFractionFitter::ComputeChisquareLambda()
{
// Method used internally to compute the likelihood ratio chi2
// See the function GetChisquare() for details

   if ( !fFitDone ) {
      Error("ComputeChisquareLambda","Fit not yet (successfully) performed");
      fChisquare = 0;
      return;
   }

   // fPlot must be initialized and filled. Leave this to the GetPlot() method.
   if (! fPlot) 
     GetPlot();

   Int_t minX, maxX, minY, maxY, minZ, maxZ;
   GetRanges(minX, maxX, minY, maxY, minZ, maxZ);
   
   Double_t logLyn = 0; // likelihood of prediction
   Double_t logLmn = 0; // likelihood of data ("true" distribution)
   for(Int_t x = minX; x <= maxX; x++) {
      for(Int_t y = minY; y <= maxY; y++) {
	 for(Int_t z = minZ; z <= maxZ; z++) {
	    Double_t di = fData->GetBinContent(x, y, z);
	    Double_t fi = fPlot->GetBinContent(x, y, z);
	    if(fi != 0) logLyn += di * TMath::Log(fi) - fi;
	    if(di != 0) logLmn += di * TMath::Log(di) - di;
	    for(Int_t j = 0; j < fNpar; j++) {
	       Double_t aji = ((TH1*)fMCs.At(j))->GetBinContent(x, y, z);
	       Double_t Aji = ((TH1*)fAji.At(j))->GetBinContent(x, y, z);
	       if(Aji != 0) logLyn += aji * TMath::Log(Aji) - aji;
	       if(aji != 0) logLmn += aji * TMath::Log(aji) - aji;
	    }
	 }
      }
   }

   fChisquare = -2*logLyn + 2*logLmn;

   return;
}

//______________________________________________________________________________
TH1* TFractionFitter::GetMCPrediction(Int_t parm) const
{
// Return the adjusted MC template (Aji) for template (parm).
// Note that the (Aji) times fractions only sum to the total prediction
// of the fit if all weights are 1.

   CheckParNo(parm);
   if ( !fFitDone ) {
      Error("GetMCPrediction","Fit not yet performed");
      return 0;
   }
   return (TH1*) fAji.At(parm);
}
