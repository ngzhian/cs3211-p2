#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int is_prime(long);
long find_prime_factors(long, long*);
long is_gen(long candidate, long factor, long p);
long modexp(long t, long u, long n);

int main(int argc, char *argv[]) {
  long p = atol(argv[1]);
  if (p == 0) return 0;

  double start, end;
  int my_rank, num_procs;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  
  long num_per_proc = p/num_procs;

  long *factors = malloc(2 * sqrt(p-1) * sizeof(long));
  long num_factors = find_prime_factors(p-1, factors);

  long candidate_start = my_rank * num_per_proc + 1;
  long candidate_end = num_per_proc * (my_rank + 1);

  long candidate;
  long num_gens = 0;

  start = MPI_Wtime();

  printf("rank: %ld, start: %ld, end: %ld\n", my_rank, candidate_start, candidate_end);
  for (candidate = candidate_start; candidate <= candidate_end; candidate++) {
    if (candidate == 0 || candidate == 1 || candidate == p) {
      continue;
    }
    long is_generator = 1;
    long j;
    for (j = 0; j < num_factors; j++) {
      long factor = factors[j];
      long m = modexp(candidate, (p-1)/factor, p);
      if (m == 1L) {
        is_generator = 0;
        break;
      }
    }
    if (is_generator) {
      //printf("%ld is a generator\n", candidate);
      num_gens++;
    }
  }

  end = MPI_Wtime();

  long total_gens;
  int success = MPI_Reduce(&num_gens, &total_gens, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  if (my_rank == 0) {
    printf("%d: %ld total\n", my_rank, total_gens);
    printf("time taken: %f\n", end - start);
  } 

}

int is_prime(long x) {
  return 1;
}

/* Find all prime factors of @num and store them
 * into @factors
 * @return the number of prime factors @num has
 */
long find_prime_factors(long num, long *factors) {
  long i = 0, s;
  for (s = 2; s < sqrt(num); s++) {
    if (num % s == 0) {
      factors[i++] = s;
      factors[i++] = num/s;
    }
  }
  return i;
}

long is_gen(long candidate, long factor, long p) {
  return (modexp(candidate, ((p-1)/factor), p) != 1);
}

/* computes s = (t ^ u) mod n
 */
long modexp(long t, long u, long n) {
  long s = 1;
  while (u > 0) {
    if (u & 1) {
      s = (s * t) % n;
    }
    u >>= 1;
    t = (t * t) % n;
  }
  return s;
}
