/* Andrea Baronchelli */
#include<iostream>
#include "time.h"
#include "stdlib.h"
#include "rangen.h"
#include "math.h"
using namespace std;

/* 
usa la funzione rand() per generare il seme
per un generatore di numeri casuali; genera un po' di numeri con 
rand() prima di sceglierne uno.
*/
int seed_for_random(){
    int appoggio, orario, i;
    srand(time(0));
    appoggio = 100;
    for(i=0;i<appoggio;i++){
	orario=rand();
    }
    orario= -1 * rand(); //ran2 vuole un numero negativo.
    if((orario%2)==0){   //e dispari.
	orario=orario+1;
    }
    //cout << "#seme " << orario << endl;
    return orario;
}

double ran0(int &idum){
    const int IA=16807,IM=2147483647,IQ=127773;
    const int IR=2836,MASK=123459876;
    const double AM=1.0/double(IM);
    int k;
    double ans;
    idum ^= MASK;
    k=idum/IQ;
    idum=IA*(idum-k*IQ)-IR*k;
    if (idum < 0) idum += IM;
    ans=AM*idum;
    idum ^= MASK;
    return ans;
}

double ran2(int &idum){
    const int IM1=2147483563, IM2=2147483399;
    const double AM=(1.0/IM1);
    const int IMM1=(IM1-1);
    const int IA1=40014, IA2=40692, IQ1=53668, IQ2=52774, IR1=12211, IR2=3791;
    const int NTAB=32;
    const int NDIV=(1+IMM1/NTAB); //double o int?
    const double EPS=1.2e-7;
    const double RNMX=(1-EPS);

    int j;
    long k;
    static int idum2=123456789;
    static int iy=0;
    static int iv[NTAB];
    double temp;

    if(idum <=0 ){
	if (-(idum) < 1){
	    idum = 1;
	}
	else{
	    idum = -(idum);
	}
	idum2=idum;
	for(j=NTAB+7;j>=0;j--){
	    k=idum/IQ1;
	    idum=IA1*(idum-k*IQ1)-k*IR1;
	    if (idum<0){
		idum=idum += IM1;
	    }
	    if(j<NTAB){
		iv[j]=idum;
	    }
	}
	iy=iv[0];
    }
    k=idum/IQ1;
    idum=IA1*(idum-k*IQ1)-k*IR1;
    if (idum<0){
	idum=idum += IM1;
    }
    k=idum2/IQ2;
    idum2=IA2*(idum-k*IQ2)-k*IR2;
    if (idum2<0){
	idum2=idum2 += IM2;
    }
    j=iy/NDIV;
    iy=iv[j]-idum2;
    iv[j]=idum;
    if(iy < 1){
	iy += IMM1;
    }
    if((temp=AM*iy) > RNMX){ 
	return RNMX;
    }
    else return temp;
} 

//Andrea Baronchelli.

double gasdev(int &idumg){
    static int iset=0;
    static double gset;
    double fac, rsq, v1, v2;

    if(idumg < 0){
	iset=0;
    }
    if(iset==0){
	do{
	    v1=2.0*ran2(idumg)-1.0;
	    v2=2.0*ran2(idumg)-1.0;
	    rsq=v1*v1+v2*v2;
	} while(rsq >= 1.0 || rsq == 0.0);
	fac = sqrt(-2.0*log(rsq)/rsq);
	gset=v1*fac;
	iset=1;
	return v2*fac;
    }
    else{
	iset=0;
	return gset;
    }
}

double expdev(int &idume){
    double appoggio;
    do{
	appoggio=ran2(idume);
    } while(appoggio==0.0);
    return -log(appoggio);
}

double powdev(int &idume, double exponent){
    double appoggio,pow_exp;
    do{
	appoggio=ran2(idume);
    } while(appoggio<0);
    pow_exp = pow( ((exponent-1)*appoggio), 1.0/(1.0-exponent) ); 
    //pow_exp = pow( (appoggio), 1.0/(1.0-exponent) ); 
    //cout << appoggio << " " << pow_exp << endl;
    return pow_exp;
}


