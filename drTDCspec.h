#include <TMath.h>

double drTDCspec(double *x, double *par)
{
  float t = x[0];
  double F=0;

  double Ad = par[0];
  double As = par[1];
  double Af = par[2];
  double tau_dr = par[3];
  double tau_s = par[4];
  double tau_f = par[5];

  F = Ad/tau_dr*TMath::Exp(-t/tau_dr)
     +As/tau_s*TMath::Exp(-t/tau_s)
     +Af/tau_f*TMath::Exp(-t/tau_f);

  return F;
}
