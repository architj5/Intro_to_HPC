/*
File:    matrix_mult.c
Purpose: Estimate time required for serial and parallel computation of matrix multiplication.
		 The program prints time required to execute serially and parallely for thread count 1 to 16 and prints

Compile: gcc -g -Wall -fopenmp -o matrix_mult matrix_mult.c
Usage:   ./matrix_mult <number of threads>

Output:
Time taken by serial prog is: 0.004185
Time taken by 1 thread/s in parallel prog is: 0.005510
Time taken by 2 thread/s in parallel prog is: 0.002834
Time taken by 3 thread/s in parallel prog is: 0.003396
Time taken by 4 thread/s in parallel prog is: 0.001602
Time taken by 5 thread/s in parallel prog is: 0.001556
Time taken by 6 thread/s in parallel prog is: 0.001623
Time taken by 7 thread/s in parallel prog is: 0.001003
Time taken by 8 thread/s in parallel prog is: 0.000888
Time taken by 9 thread/s in parallel prog is: 0.001411
Time taken by 10 thread/s in parallel prog is: 0.001435
Time taken by 11 thread/s in parallel prog is: 0.001155
Time taken by 12 thread/s in parallel prog is: 0.003099
Time taken by 13 thread/s in parallel prog is: 0.000984
Time taken by 14 thread/s in parallel prog is: 0.000936
Time taken by 15 thread/s in parallel prog is: 0.000867
Time taken by 16 thread/s in parallel prog is: 0.004563


From the above statistics it is seen that the time needed for parallel program
roughly reduces as the number of threads is increased from 1 to 16.
For one thread the time needed for parallel program could be more than that of serial one.
This could be due to some unavoidable over heads.
*/





#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<string.h>

void serMatVec(double** A, double* x, double* y, int n);
void ompMatVec(double** A, double* x, double* y, int n, int th);

void Usage(char *prog_name) {
	fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
	fprintf(stderr, "   thread_count should be positive\n");
	exit(0);
}



int main()
{
	// Uncomment to take number of threads as input from the user thorugh command line.
/*	int thread_count;
	if (argc != 2) Usage(argv[0]);
	thread_count = strtol(argv[1], NULL, 10);
	if (thread_count <= 0) Usage(argv[0]);
*/

	int i, j, n, th;
	n=1000;
	double** A= (double**)malloc(n*sizeof(double*));
	double* x= (double*) malloc(n*sizeof(double*));
	double* y= (double*) malloc(n*sizeof(double*));


	for (i=0; i<n; i++){
		A[i]=(double*)malloc(n*sizeof(double));
		for (j=0; j<n; j++)
			A[i][j]= (5) * ( (double)rand() / (double)RAND_MAX );		// Filling random double values in matrix A
	}

	for (i=0; i<n; i++){
		x[i]= (5) * ( (double)rand() / (double)RAND_MAX );				// Filling random double values in matrix B
	}

	for(i=0; i<n; i++)
		y[i]= 0;		// Making the elements in output matrix 0

/*	for (i=0; i<n; i++){
		for(j=0; j<n; j++){
			printf ("%lf    ",A[i][j]);
		}
		printf("\n");
	}
*/
	serMatVec(A, x, y, n);					// Serial computation

	for (th=1; th<=16; th++)
		ompMatVec(A, x, y, n, th);			// Parallel computation

	return 0;
}


void serMatVec(double** A, double* x, double* y, int n)
{
	int i,j;
	double  time1= omp_get_wtime();
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			y[i]+= A[i][j]*x[i];
		}
	}
	double time2= omp_get_wtime();
	double diff= time2-time1;
	printf("Time taken by serial prog is: %lf \n", diff);
}


void ompMatVec(double** A, double* x, double* y, int n, int th){
	int i, j;
	double  time1= omp_get_wtime();
	#pragma omp parallel for num_threads(th) default(none) shared(A,n,x,y) private (i, j)
	for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            y[i]+= A[i][j]*x[i];
            }
        }


	double time2= omp_get_wtime();
	double diff= time2-time1;
	printf("Time taken by %d thread/s in parallel prog is: %lf \n", th, diff);
}





