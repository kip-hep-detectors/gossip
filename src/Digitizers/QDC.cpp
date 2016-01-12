#include "Digitizers/QDC.h"

QDC::QDC( double kenoise, int knbins, double kpedestal, double kconv )
{
	pedestal = kpedestal;
	conv = kconv;
	nbins = knbins;
	enoise = kenoise;
}

QDC::~QDC()
{
}

void QDC::Run()
{
	double charge = sipm->GetCharge();
	qdc_value = pedestal + conv*charge;
	AddENoise();
}

void QDC::BWFilter()
{
}

void QDC::AddENoise()
{
	TRandom3 r(0);
	qdc_value += r.Gaus(0,enoise);
}

