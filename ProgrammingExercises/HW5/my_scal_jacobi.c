
#include <stdio.h>
#include <math.h>

// 1D length
#define N 2048

// Maximum number of iterations
#define ITER_MAX 1000000

// How often to check the relative residual
#define RESID_FREQ 1000 

// The residual
#define RESID 1e-6

double magnitude(double** x);
void jacobi(double** x, double** b, double** tmp);
double getResid(double**x, double** b);

int main(int argc, char** argv)
{
   int i,j, totiter;
   int done = 0;
   double** x = new double*[N+2];
   double** xtmp = new double*[N+2];
   double** b = new double*[N+2]; 
   double bmag, resmag;

   for (i=0;i<N+2;i++) 
   	{ 
   		for(j=0;j<N+2;j++)
		{
			x[i][j] = 0.0; 
			xtmp[i][j] = 0.0; 
			b[i][j] = 0.0; 
		}
	}
   b[(N+2)/2][(N+2)/2] = 1.0;
   //Get magnitude of rhs
   bmag = magnitude(b);
   printf("bmag: %.8e\n", bmag);

   for (totiter=RESID_FREQ;totiter<ITER_MAX && done==0;totiter+=RESID_FREQ)
   {

      // do RESID_FREQ jacobi iterations
      jacobi(x, b, xtmp);

      resmag = getResid(x, b);

      printf("%d res %.8e bmag %.8e rel %.8e\n", totiter, resmag, bmag, resmag/bmag);
      if (resmag/bmag < RESID) { done = 1; }
   }

   
   return 0;
}

double magnitude(double** x)
{
   int i, j;
   double bmag;

   i, j = 0;
   bmag = 0.0;  
   for (i=2; i<N; i++)
   {
   	for(j=2; j<N;j++)
   	{
     	bmag = bmag + x[i][j]*x[i][j];
   	}
   }
   
   return sqrt(bmag);
}

void jacobi(double** x, double** b, double** tmp)
{
   int iter,i, j;

   iter = 0; i = 0;

   for (iter=0;iter<RESID_FREQ;iter++)
   {
      for (i=2;i<N;i++)
      {
      	for(j=2;j<N;j++)
      	{
         tmp[i][j] = (1/4)*(tmp[i+1][j] + tmp[i-1][j] + tmp[i][j+1] + tmp[i][j-1]) + b[i][j];
      	}
      }

      for (i=2;i<N;i++)
      {
      	for(j=2;j<N;j++)
      	{
         x[i][j] = tmp[i][j];
      	}
      }
   }
}

double getResid(double** x, double** b)
{
   int i, j;
   double localres,resmag;

   i = 0;
   localres = 0.0;
   resmag = 0.0;

   for (i=2;i<N;i++)
   {
   	for(j=2;j<N;j++)
   	{
      localres = (b[i][j] - x[i][j] + 0.5*(x[i+1][j] + x[i-1][j] + x[i][j+1] + x[i][j-1]));
      localres = localres*localres;
      resmag = resmag + localres;
  	}
   }

   resmag = sqrt(resmag);

   return resmag;
}

