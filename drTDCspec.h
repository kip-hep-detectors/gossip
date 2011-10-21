#include <TMath.h>

double drTDCspec(double *x, double *par){
	float t = x[0];
	double F=0;
	/*double N = par[0];
	double tau_dr = par[1];
	double tau_ap = par[2];
	double Pap = par[3];
	
*/
	double N = par[0];
	double As = par[1];
	double Af = par[2];
	double tau_dr = par[3];
	double tau_s = par[4];
	double tau_f = par[5];
	double Px = par[6];
/*
	
	double Ad = par[0];
	double As = par[1];
	double Af = par[2];
	double tau_dr = par[3];
	double tau_s = par[4];
	double tau_f = par[5];
	
	
	F = Ad/tau_dr*TMath::Exp(-t/tau_dr)
	   +As/tau_s*TMath::Exp(-t/tau_s)
	   +Af/tau_f*TMath::Exp(-t/tau_f);
	   
/*
	Pap = Pap*(tau_dr+tau_ap)/tau_dr;

	F = N/tau_dr*TMath::Exp(-t/tau_dr)*(Pap*TMath::Exp(-t/tau_ap)+(1-Pap))
	    +N/tau_ap*TMath::Exp(-t/tau_ap)*Pap*TMath::Exp(-t/tau_dr);
*/
/*
	const int k=10;
	double af[k+1]={0},as[k+1]={0};
	af[0]=Af;
	as[0]=As;
	for(int i=1; i<=k; i++){
	  af[i] = af[0]*((tau_f+tau_dr)*(tau_s*tau_dr+tau_f*tau_s+tau_dr*tau_f))/(tau_dr*(tau_s*tau_dr+tau_f*(tau_s+tau_dr-tau_dr*as[i-1])));
	  as[i] = af[0]*((tau_s+tau_dr)*(tau_s*tau_dr+tau_f*tau_s+tau_dr*tau_f))/(tau_dr*(tau_f*tau_dr+tau_s*(tau_f+tau_dr-tau_dr*af[i-1])));
	}

*/
	double Pd=1/tau_dr*TMath::Exp(-t/tau_dr);
	double Xd=TMath::Exp(-t/tau_dr);
	double Ps=As/tau_s*TMath::Exp(-t/tau_s);
	double Xs=As*TMath::Exp(-t/tau_s)+(1-As);
	double Pf=Af/tau_f*TMath::Exp(-t/tau_f);
	double Xf=Af*TMath::Exp(-t/tau_f)+(1-Af);
	
	double q = 1-pow(1-Px,0.25);
	double Px0=pow(1-q,4);
	double Px1=q*4*pow(1-q,6);
	double Px2=q*(6*pow(1-q,8)+12*pow(1-q,7));

	F = N*(Pd*Xf*Xs+Pf*Xd*Xs+Ps*Xd*Xf);
	     // +Px1*2*(Pd*Xd*pow(Xf,2)*pow(Xs,2)+Pf*Xf*pow(Xd,2)*pow(Xs,2)+Ps*Xs*pow(Xd,2)*pow(Xf,2))
	     // +Px2*3*(Pd*pow(Xd,2)*pow(Xf,3)*pow(Xs,3)+Pf*pow(Xf,2)*pow(Xd,3)*pow(Xs,3)+Ps*pow(Xs,2)*pow(Xd,3)*pow(Xf,3)));
	
	return F;
}
