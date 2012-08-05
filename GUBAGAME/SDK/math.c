#include "math.h"

#define mult(a,b,c)  { a=(short int )b*(short int)c; }


short int tsen[ANGLE_360+1]={
#include "sen.inc"
};

short int tcos[ANGLE_360+1]={
#include "cos.inc"
};

short int tinvsen[ANGLE_360+1]
=
{
#include "invsen.inc"	
};

short int tinvcos[ANGLE_360+1]
=
{
#include "invcos.inc"	
};
short int ttan[ANGLE_360+1]
=
{
#include "tan.inc"	
};

short int arctan[32768+1]
=
{
#include "arctan.inc"	
};

short int tproj[32768+1]
=
{
#include "proj.inc"	
};


int tcosAG2;
int tsenAG2;
int tcosAG1;
int tsenAG1tsenAG2;
int tsenAG1tcosAG2;

float fcosAG2;
float fsenAG2;
float fcosAG1;
float fsenAG1tsenAG2;
float fsenAG1tcosAG2;


/*----------------------------------------------------------------------------
 F: Get arc-tangente
 ---------------------------------------------------------------------------*/

short int get_vetor_arctan_table(int fx,int fy)
{long coef;
 short int agg=ANGLE_180;
 short int tfx,tfy;
 
    
    

	if(fy==0)
	{	
		if(fx<0) agg=ANGLE_180;
		if(fx>0) agg=ANGLE_0;
		return agg;
	}
	
	if(fx==0)
	{		
		if(fy>0) agg=ANGLE_90;
		if(fy<0) agg=ANGLE_270;	
	}
	else
	{	
		tfx=fx;
		tfy=fy;
		if(tfx<0) tfx=-tfx;
		if(tfy<0) tfy=-tfy;
					
		mult(coef,tfy,tproj[tfx]);
		coef>>=(PDPJ_FIX-PDDT_FIX);
		
			
		if((coef < 32768L) && (coef >= 0))
		{
			agg=arctan[coef];							
		}
		else
		{
			if(tfx<tfy)
			{
				agg=ANGLE_90;								
			}
			else
			{
				agg=ANGLE_0;	
			}
			
		}
					
		if(fx<0)
		{
			if(fy>0) agg=ANGLE_180-agg;
			if(fy<0) agg=ANGLE_180+agg;	
		}
		else
		{
			if(fy<0) agg=ANGLE_360-agg;	
		}								
		
	}

	return agg;
}

float fsen(int a)
{float s;

	s=tsen[a]; 
	s=(float)(s/KDD_FIX);

	return s;
}

float fcos(int a)
{float s;

	s=tcos[a];
	s=(float)(s/KDD_FIX);

	return s;
}

int isen(int a)
{
	return tsen[a]; 
}

int icos(int a)
{
	return tcos[a];
}

void initProjOrtho3D(void)
{
	tcosAG2=tcos[AG2];
	tsenAG2=tsen[AG2];
	tsenAG1tsenAG2=tsen[AG1]*tsen[AG2];
	tsenAG1tsenAG2>>=PDD_FIX;
	tsenAG1tcosAG2=tsen[AG1]*tcos[AG2];
	tsenAG1tcosAG2>>=PDD_FIX;
	tcosAG1=tcos[AG1];


}

int projZOrtho3D(int z)
{int zt;

	zt=z*tcosAG1;
	zt>>=PDD_FIX;

	return zt;
}


void projOrtho3D(int x,int y,int z,int *xp,int *yp)
{int xt,yt;
 //int xa,ya,dif;

//	xa=*xp;
//	ya=*yp;
 
	xt=x*tcosAG2-y*tsenAG2;
	*xp=xt>>PDD_FIX;
	
	yt=x*tsenAG1tsenAG2+ y*tsenAG1tcosAG2;
	if(z) yt+=z*tcosAG1;
	*yp=yt>>PDD_FIX; 

	
//	dif=*xp-xa;
//	if(dif<0) dif=-dif;
//	if(dif==1) *xp=xa;

//	dif=*yp-ya;
//	if(dif<0) dif=-dif;
//	if(dif==1) *yp=ya;
	
}
