#include "Digitizers/Filters/BWFilters.h"

#include "Digitizers/Filters/liir.h"

Waveform BWFilters::BWLP( Waveform wf, double freq, int order, bool scale )
{
	/*--- Calulate filter coefficients ---*/
	double fcf;       // cutoff frequency (fraction of pi)
	double *dcof;     // d coefficients
	int *ccof;        // c coefficients

	double fsf;
	vector<double> fcx;
	vector<double> fcy;

	fcf = 2 * freq / (1/wf.GetSampling());

	/* Calculate the d coefficients */
	dcof = dcof_bwlp( order, fcf );
	if( dcof == NULL )
	{
		perror( "Unable to calculate d coefficients" );
		return wf;
	}

	/* Calculate the c coefficients */
	ccof = ccof_bwlp( order );
	if( ccof == NULL )
	{
		perror( "Unable to calculate c coefficients" );
		return wf;
	}

	/* Scaling factor for the c coefficients */
	fsf = sf_bwlp( order, fcf );

	/* Output the c coefficients */
	fcx.clear();
	if( scale == 0 )
		for(int i = 0; i <= order; ++i)
			fcx.push_back(ccof[i]);
	else
		for(int i = 0; i <= order; ++i)
			fcx.push_back(ccof[i]*fsf);

	/* Output the d coefficients */
	fcy.clear();
	for(int i = order; i >= 0; i--)
		fcy.push_back(dcof[i]);

	free(dcof);
	free(ccof);


	/* --- Apply filter ---*/

	Waveform wf_out = wf;
	wf_out.Clear();

	vector<float> xv,yv;
	xv.resize(order+1);
	yv.resize(order+1);

	for(int i = 0; i < wf.GetNsamples(); i++)
	{
		for(int k = 0; k < order; k++)
		{
			xv[k] = xv[k+1];
			yv[k] = yv[k+1];
		}

		xv[order] = wf.GetSample(i) * fsf;
		yv[order] = 0;

		for(int k = 0; k <= order; k++)
		{
			yv[order] += ( fcx[order-k] * xv[order-k]);
		}

		for(int k = 1; k <= order; k++)
		{
			yv[order] -= ( fcy[order-k] * yv[order-k]);
		}

		wf_out.SetSample(i,yv[order]);
	}

	return wf_out;
}

