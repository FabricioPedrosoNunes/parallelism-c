#include <stdio.h>
#include <cuda.h>

#define THREADSPERBLOCK 1024

__global__ void primeiroLaco(long int* d_num, long int* d_den, long int start, long int end, int size)
{
  int num_aux, den_aux, aux, resto;
  long int factor, ii, sum, done, n;
  int i = blockIdx.x * blockDim.x + threadIdx.x;

  if (i < size)
  {
    ii     = i - start;
    sum    = 1 + i;
    done   = i;
    factor = 2;

    while (factor < done)
    {
      resto = i / factor;
      resto = i - (factor * resto);

      if (resto == 0)
      {
        sum += (factor + (i / factor));

		    if ((done = i / factor) == factor)
		      sum -= factor;
      }

	    factor++;
    }

    d_num[ii] = sum;
    d_den[ii] = i;

    num_aux = d_num[ii];
    den_aux = d_den[ii];

  	while (num_aux != 0)
  	{
      aux     = num_aux; 
	    resto   = den_aux / num_aux;
  	  resto   = den_aux - (num_aux * resto);
	    num_aux = resto;  
	    den_aux = aux;
	  }

    n          = den_aux;
    d_num[ii] /= n;
    d_den[ii] /= n;
  }
}

__global__ void segundoLaco(long int* d_num, long int* d_den, long int* d_arrSomaC, int size)
{
  int j, i = blockIdx.x * blockDim.x + threadIdx.x;

  if (i < size)
  {
    for (j = i + 1; j < size; j++)
    {
      if ((d_num[i] == d_num[j]) && (d_den[i] == d_den[j]))
        d_arrSomaC[i]++;
    }
  }
}

void friendly_numbers(long int start, long int end)
{
  cudaSetDevice(0);

  long int *d_num, *d_den, last = end - start + 1;
  size_t   size = last * sizeof(long int);
  int      c    = 0;

  int tamanho = end - start;
  int nBlocks = (tamanho + THREADSPERBLOCK - 1) / THREADSPERBLOCK;

  printf("BLOCOS %d THREADS %d\n", nBlocks, THREADSPERBLOCK);

  long int *num;
  num = (long int*) malloc(size);

  long int *den;
  den = (long int*) malloc(size);

  long int *arrSomaC;
  arrSomaC = (long int*) malloc(size);

  long int i;

  for (i = 0; i < last; i++)
    arrSomaC[i] = 0;

  cudaMalloc((void**)&d_num, size);
  cudaMalloc((void**)&d_den, size);

  cudaMemcpy(d_num, num, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_den, den, size, cudaMemcpyHostToDevice);

  primeiroLaco<<<nBlocks, THREADSPERBLOCK>>>(d_num, d_den, start, end, last);

  long int *d_arrSomaC;

  cudaMalloc((void**)&d_arrSomaC, size);
  cudaMemcpy(d_arrSomaC, arrSomaC, size, cudaMemcpyHostToDevice);

  segundoLaco<<<nBlocks, THREADSPERBLOCK>>>(d_num, d_den, d_arrSomaC, last);

  cudaMemcpy(arrSomaC, d_arrSomaC, size, cudaMemcpyDeviceToHost);

  for (i = 0; i < last; i++)
    c += arrSomaC[i];

  printf("Founded %d pairs of mutually friendly numbers\n", c);

  cudaFree(d_num);
  cudaFree(d_den);
  cudaFree(d_arrSomaC);

  free(num);
  free(den);
  free(arrSomaC);
}

int main(int argc, char **argv)
{
  long int start;
  long int end;

  start = atoi(argv[1]);
  end   = atoi(argv[2]);

  printf("NUMBER %ld TO %ld\n", start, end);
  friendly_numbers(start, end);

  return EXIT_SUCCESS;
}