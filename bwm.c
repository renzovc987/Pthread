#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

const int MAX_THREADS = 1024;

long thread_count;
long long n;
int flag;
double sum;
pthread_mutex_t mutex;
void* Thread_summutex(void* rank) {
   long my_rank = (long) rank;
   double factor;
   long long i;
   long long my_n = n/thread_count;
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;
   double my_sum = 0.0;

   if (my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;

   for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      my_sum += factor/(2*i+1);
   }
   pthread_mutex_lock(&mutex);
   sum += my_sum;
   pthread_mutex_unlock(&mutex);

   return NULL;
}  /* Thread_sum */
void* Thread_sumbw(void* rank) {
   long my_rank = (long) rank;
   double factor;
   long long i;
   long long my_n = n/thread_count;
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;

   if (my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;

   for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      while (flag != my_rank);
      sum += factor/(2*i+1);  
      flag = (flag+1) % thread_count;
   }

   return NULL;
}  

double Serial_pi(long long n) {
   double sum = 0.0;
   long long i;
   double factor = 1.0;

   for (i = 0; i < n; i++, factor = -factor) {
      sum += factor/(2*i+1);
   }
   return 4.0*sum;

} 

void Get_args(int argc, char* argv[]) {
   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
   n = strtoll(argv[2], NULL, 10);
   if (n <= 0) Usage(argv[0]);
}  


void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
   fprintf(stderr, "   n is the number of terms and should be >= 1\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   exit(0);
}  

int main(int argc, char* argv[]) {
   long       thread;  
   pthread_t* thread_handles;
   Get_args(argc, argv);

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   clock_t t; 
   t = clock(); 
   sum = 0.0;
   flag = 0;
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Thread_sumbw, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   t = clock() - t;
   double time_taken = ((double)t)/CLOCKS_PER_SEC;

   sum = 4.0*sum;
   printf("%s","Busy - Waiting");
   printf("With n = %lld terms,\n", n);
   printf("   Multi-threaded estimate of pi  = %.15f\n", sum);
   printf("   Elapsed time = %e seconds\n", time_taken);

   clock_t t2;
   t2 = clock();
   sum = Serial_pi(n);
   t2 = clock() - t2;
   double time_taken2 = ((double)t2)/CLOCKS_PER_SEC;

   printf("   Single-threaded estimate of pi = %.15f\n", sum);
   printf("   Elapsed time = %e seconds\n", time_taken2);
   printf("   Math library estimate of pi    = %.15f\n", 
       4.0*atan(1.0));
  
   
   free(thread_handles);
   return 0;
} 

