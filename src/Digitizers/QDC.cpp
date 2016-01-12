#include "Digitizers/QDC.h"

QDC::QDC()
{
	pedestal = 50;
	conv = 1;
	nbins = 1024;
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

void QDC::AddENoise()
{
	TRandom3 r;
	qdc_value += r.Gaus(0,enoise);
}

