#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

            
pthread_mutex_t list_mutex;
time_t t;
double start,finish,elapsed;
struct list_node_s {
   int    data;
   struct list_node_s* next;
};

int  Insert(int value, struct list_node_s** head_p);
void Print(struct list_node_s* head_p);
int  Member(int value, struct list_node_s* head_p);
int  Delete(int value, struct list_node_s** head_p);
void Free_list(struct list_node_s** head_p);
int  Is_empty(struct list_node_s* head_p);
char Get_command(void);
int  Get_value(void);

int main(void) {
   char command;
   int  value;
   struct list_node_s* head_p = NULL;  

   command = Get_command();
   while (command != 'q' && command != 'Q') {
      switch (command) {
         case 'i': 
         case 'I': 
         	srand(time(&t));
            value = (rand() % (101));
            GET_TIME(start);
            pthread_mutex_lock(&list_mutex);
            for(int i = 1;i<=1000;i++)
            	Insert(value, &head_p);
            pthread_mutex_unlock(&list_mutex);  
            GET_TIME(finish);
            elapsed = finish - start;
            printf("Tiempo = %e segundos\n", elapsed);

            break;
         case 'p':
         case 'P':
            Print(head_p);
            break;
         case 'm': 
         case 'M':
         	srand(time(&t));
            value = (rand() % (101));
            GET_TIME(start);
            pthread_mutex_lock(&list_mutex);
            for(int i = 1;i<=100000;i++)
            	Member(value, head_p);
            pthread_mutex_unlock(&list_mutex); 
            GET_TIME(finish);  
            elapsed = finish - start;
            printf("Tiempo = %e segundos\n", elapsed);
            break;
         case 'd':
         case 'D':
         	srand(time(&t));
            value = (rand() % (101));
            GET_TIME(start);
            pthread_mutex_lock(&list_mutex);
            for(int i = 1;i<=500;i++)
            	Delete(value, &head_p);  
            pthread_mutex_unlock(&list_mutex);
            GET_TIME(finish);
            elapsed = finish - start;
            printf("Tiempo = %e segundos\n", elapsed);
            break;
         default:
            printf("There is no %c command\n", command);
            printf("Please try again\n");
      }
      command = Get_command();
   }
   Free_list(&head_p);

   return 0;
}  
            
int Insert(int value, struct list_node_s** head_pp) {
   struct list_node_s* curr_p = *head_pp;
   struct list_node_s* pred_p = NULL;
   struct list_node_s* temp_p;
   
   while (curr_p != NULL && curr_p->data <= value) {
      pred_p = curr_p;
      curr_p = curr_p->next;
   }

   if (curr_p == NULL || curr_p->data >= value) {
      temp_p = malloc(sizeof(struct list_node_s));
      temp_p->data = value;
      temp_p->next = curr_p;
      if (pred_p == NULL)
         *head_pp = temp_p;
      else
         pred_p->next = temp_p;
      return 1;
   } else { 
      return 0;
   }
} 

void Print(struct list_node_s* head_p) {
   struct list_node_s* curr_p;

   printf("list = ");

   curr_p = head_p;
   while (curr_p != (struct list_node_s*) NULL) {
      printf("%d ", curr_p->data);
      curr_p = curr_p->next;
   }
   printf("\n");
}  

int  Member(int value, struct list_node_s* head_p) {
   struct list_node_s* curr_p;

   curr_p = head_p;
   while (curr_p != NULL && curr_p->data < value)
      curr_p = curr_p->next;

   if (curr_p == NULL || curr_p->data > value) {
      printf("%d is not in the list\n", value);
      return 0;
   } else {
      printf("%d is in the list\n", value);
      return 1;
   }
} 


int Delete(int value, struct list_node_s** head_pp) {
   struct list_node_s* curr_p = *head_pp;
   struct list_node_s* pred_p = NULL;
   while (curr_p != NULL && curr_p->data < value) {
      pred_p = curr_p;
      curr_p = curr_p->next;
   }
   
   if (curr_p != NULL && curr_p->data == value) {
      if (pred_p == NULL) { 
         *head_pp = curr_p->next;
#ifdef DEBUG
         printf("Freeing %d\n", value);
#endif
         free(curr_p);
      } else { 
         pred_p->next = curr_p->next;
#ifdef DEBUG
         printf("Freeing %d\n", value);
#endif
         free(curr_p);
      }
      return 1;
   } else {
   printf("%d no esta en la lista\n", value);
      return 0;
   }
}  


void Free_list(struct list_node_s** head_pp) {
   struct list_node_s* curr_p;
   struct list_node_s* succ_p;

   if (Is_empty(*head_pp)) return;
   curr_p = *head_pp; 
   succ_p = curr_p->next;
   while (succ_p != NULL) {
#ifdef DEBUG
      printf("Freeing %d\n", curr_p->data);
#endif
      free(curr_p);
      curr_p = succ_p;
      succ_p = curr_p->next;
   }
#ifdef DEBUG
   printf("Freeing %d\n", curr_p->data);
#endif
   free(curr_p);
   *head_pp = NULL;
}  


int  Is_empty(struct list_node_s* head_p) {
   if (head_p == NULL)
      return 1;
   else
      return 0;
} 

char Get_command(void) {
   char c;
   printf("%s","Ingrese comando:  ");
   scanf(" %c", &c);
   return c;
}  

int  Get_value(void) {
   int val;
   printf("Ingrese valor:  ");
   scanf("%d", &val);
   return val;
}  
