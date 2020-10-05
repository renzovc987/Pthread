#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

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
   long       thread2;  
   pthread_t* thread_handles2;
   Get_args(argc, argv);
   double start2, finish2, elapsed2;
   pthread_mutex_init(&mutex, NULL);
   thread_handles2 = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   
   sum = 0.0;
   GET_TIME(start2);
   for (thread2 = 0; thread2 < thread_count; thread2++)  
      pthread_create(&thread_handles2[thread2], NULL,
          Thread_summutex, (void*)thread2);  

   for (thread2 = 0; thread2 < thread_count; thread2++) 
      pthread_join(thread_handles2[thread2], NULL); 

   GET_TIME(finish2);
   elapsed2 = finish - start2;
   sum = 4.0*sum;
   printf("%s","MUTEX(sum) \n");
   printf("Con n = %lld terminos,\n", n);
   printf("Tiempo = %e seconds\n", elapsed2);
   GET_TIME(start2);
   sum = Serial_pi(n);
   GET_TIME(finish2);
   elapsed2 = finish2 - start2;
   printf("%s","MUTEX(pi) \n");
   printf("Con n = %lld terminos,\n", n);
   printf("Tiempo = %e seconds\n", elapsed2);

   
   pthread_mutex_destroy(&mutex);
   
   //MUTEX
   
   free(thread_handles2);
}