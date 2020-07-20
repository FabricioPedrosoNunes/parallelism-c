#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int gcd ( int a, int b )
{
  int c;

  while ( a != 0 )
  {
    c = a;
    a = b % a;
    b = c;
  }

  return b;
}

void friendly_numbers(long int start, long int end, int threads)
{
  double inicio;	
  int c = 0;
  long int last = end - start + 1;

  long int *the_num;
  the_num = (long int*) malloc(sizeof(long int) * last);

  long int *num;
  num = (long int*) malloc(sizeof(long int) * last);

  long int *den;
  den = (long int*) malloc(sizeof(long int) * last);

  long int i, j, factor, ii, sum, done, n;

  #pragma omp parallel for private(i, ii, done, sum, n, factor) shared(num, den, the_num, start, end) schedule(runtime) num_threads(threads) 
  for (i = start; i <= end; i++)
  {
    ii          = i - start;
    sum         = 1 + i;
    done        = i;
    factor      = 2;
    the_num[ii] = i;

    while (factor < done)
    {
      if ((i % factor) == 0)
      {
        sum += (factor + (i / factor));

        if ((done = i / factor) == factor)
          sum -= factor;
      }

      factor++;
    }

    num[ii]  = sum;
    den[ii]  = i;
    n        = gcd(num[ii], den[ii]);
    num[ii] /= n;
    den[ii] /= n;
  }

  #pragma omp parallel for private (i, j) shared(num, den, last) reduction(+:c) schedule(runtime) num_threads(threads)
  for (i = 0; i < last; i++)
  {
    for (j = i + 1; j < last; j++)
    {
      if ((num[i] == num[j]) && (den[i] == den[j]))
        c++;
    }
  }
  
  printf("Founded %d pairs of mutually friendly numbers\n", c);

  free(the_num);
  free(num);
  free(den);
}

int main(int argc, char **argv)
{
  long int start;
  long int end;
  int num;

  //threads = 4;
  //start   = 1;
  //end     = 662143;

  //scanf("%ld %ld", &start, &end);
  start = atoi(argv[1]);
  end   = atoi(argv[2]);
  num   = atoi(argv[3]);

  printf("Number %ld to %ld with %d threads \n", start, end, num);
  friendly_numbers(start, end, num);

  return EXIT_SUCCESS;
}
