#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int is_prime(int);
int find_prime_factors(int, int*);

int main(int argc, char *argv[]) {
  int p = atoi(argv[1]);
  if (p == 0) return 0;

  double start, end;
  int my_rank, num_procs;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  
  int num_per_proc = p/num_procs;

  // all will calculate all prime factors for p
  // each processor will take a range of generator candidate
  // range of generators from [2 .. p-1]

  int *factors = malloc(2 * sqrt(p-1) * sizeof(int));
  int num_factors = find_prime_factors(p-1, factors);

  int candidate_start = my_rank * num_per_proc + 1;
  int candidate_end = num_per_proc * (my_rank + 1);

  int candidate;
  int num_gens = 0;

  start = MPI_Wtime();

  for (candidate = candidate_start; candidate <= candidate_end; candidate++) {
    if (candidate == 0 || candidate == 1 || candidate == p) {
      continue;
    }
    int is_generator = 1;
    int j;
    for (j = 0; j < num_factors; j++) {
      int factor = factors[j];
      if (!is_gen(candidate, factor, p)) {
        is_generator = 0;
        break;
      }
    }
    if (is_generator) {
      num_gens++;
    }
  }

  end = MPI_Wtime();

  int total_gens;
  int success = MPI_Reduce(&num_gens, &total_gens, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (my_rank == 0) {
    printf("%d: %d total\n", my_rank, total_gens);
    printf("time taken: %f\n", end - start);
  } 

}

int is_prime(int x) {
  return 1;
}

/* Find all prime factors of @num and store them
 * into @factors
 * @return the number of prime factors @num has
 */
int find_prime_factors(int num, int *factors) {
  int i = 0, s;
  for (s = 2; s < sqrt(num); s++) {
    if (num % s == 0) {
      factors[i++] = s;
      factors[i++] = num/s;
    }
  }
  return i;
}

int is_gen(int candidate, int factor, int p) {
  return (modexp(candidate, ((p-1)/factor), p) != 1);
}

/* computes s = (t ^ u) mod n
 */
int modexp(int t, int u, int n) {
  int s = 1;
  while (u > 0) {
    if (u & 1) {
      s = (s * t) % n;
    }
    u >>= 1;
    t = (t * t) % n;
  }
  return s;
}
