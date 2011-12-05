//exponentiell abfallende Funktione für thermische pulse
#include <TMath.h>


Double_t tpTDCspec(Double_t *x, Double_t *par){
	Float_t t = x[0];
	Double_t F=0;
	Double_t A1 = par[0];
	Double_t tau1 = par[1];

	F = A1/tau1*TMath::Exp(-t/tau1);

	return F;
}
