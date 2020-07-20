#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

/* Standard C Function: Greatest Common Divisor */
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

void friendly_numbers(long int start, long int end, int argc, char **argv) 
{
  int c=0, total = 0, index = 0, meurank, procs, tamanhoMsg;
  long int resto = 0, last, i, j, factor, ii, sum, done, n;

  MPI_Status st;
    
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &meurank);

  if (procs != 2 && procs != 4 && procs != 8 && procs != 16 && procs != 32 && procs != 64)
  {
    if (meurank == 0)
      printf("Número inválido de Processos!\n");

    MPI_Finalize();
    return;
  }
  
  last = (end - start + 1) / (procs - 1);
  int teste = end - start;
  resto = 0;

  resto = end % last;
  
  long int *num;
  num = (long int*) malloc(sizeof(long int) * teste);
  
  long int *den;
  den = (long int*) malloc(sizeof(long int) * teste);
 
  long int *rec_num;
  rec_num = (long int*) malloc(sizeof(long int) * teste);
  
  long int *rec_den;  
  rec_den = (long int*) malloc(sizeof(long int) * teste);


  if (meurank == 0)
  {
    index = 0;
    
    for(i=1; i < procs; i++) 
    {
      MPI_Probe(i, 1, MPI_COMM_WORLD, &st);
      MPI_Get_count(&st, MPI_INT, &tamanhoMsg);

        MPI_Recv(&num[index], tamanhoMsg, MPI_LONG, i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
      MPI_Recv(&den[index], tamanhoMsg, MPI_LONG, i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
   
      index += last;
    }
  }
  else 
  {
    int fim    = start + (last * meurank) - 1; 
    int inicio = fim - last + 1; 

    if (meurank == (procs - 1) && resto > 0)
    {
      last += resto;
      fim = end;    
    }

    for (i = inicio; i <= fim; i++) 
    {
      ii = i - start;
      sum = 1 + i;
      done = i;
      factor = 2;
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
      
      rec_num[ii] = sum;
      rec_den[ii] = i;
      n = gcd(rec_num[ii], rec_den[ii]);
      rec_num[ii] /= n;
      rec_den[ii] /= n;
    }

    MPI_Send(&rec_num[inicio - start], last, MPI_LONG, 0, 1, MPI_COMM_WORLD);
    MPI_Send(&rec_den[inicio - start], last, MPI_LONG, 0, 1, MPI_COMM_WORLD);
  }

  long int intervalo = (end - start) + 1;

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Bcast(num, intervalo, MPI_LONG, 0, MPI_COMM_WORLD);
  MPI_Bcast(den, intervalo, MPI_LONG, 0, MPI_COMM_WORLD);

  int tamanhoChunk = (intervalo / procs);
  int comecoLaco       = tamanhoChunk * meurank;
  int fimLaco          = intervalo;

  if (meurank != (procs - 1))
    fimLaco = (tamanhoChunk * (meurank + 1));

  for (i = comecoLaco; i < fimLaco; i++)
  {
    for (j = i + 1; j < intervalo; j++)
    {
      if ((num[i] == num[j]) && (den[i] == den[j]))
        c++;
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&c, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (meurank == 0 )
    printf("Founded number friendly %i\n", total);
 
  free(num);
  free(den);
  free(rec_num);
  free(rec_den);
  
  MPI_Finalize();
}

int main(int argc, char **argv) 
{

  long int start, end;

  //start = 1;
  //end = 662143;

  start = atoi(argv[1]);
  end = atoi(argv[2]);

  friendly_numbers(start, end, argc, argv);
  return EXIT_SUCCESS;
}
