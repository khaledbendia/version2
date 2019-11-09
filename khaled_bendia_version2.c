#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <time.h> 

#define NB_DIV_LINE 3
#define NB_DIV_COLUMN 4
#define NUMTHREADS (NB_DIV_LINE*NB_DIV_COLUMN)


#define NB_LINE_A 7
#define NB_COLUMN_A 5

#define NB_LINE_B  5
#define NB_COLUMN_B 9

#define NB_LINE_BLOC (NB_LINE_A/NB_DIV_LINE)
#define NB_COLUMN_BLOC (NB_COLUMN_B/NB_DIV_COLUMN)

int a[NB_LINE_A][NB_COLUMN_A]
   ,b[NB_LINE_B][NB_COLUMN_B]

   ,c[NB_LINE_A][NB_COLUMN_B];



void *line_c(void *threadid) { 
   long tid; 
   tid = (long)threadid;
   
   

   int debut_line,fin_line;
   int debut_column,fin_column;
   
   debut_line=(tid/NB_DIV_COLUMN)*NB_LINE_BLOC;
   fin_line=debut_line+NB_LINE_BLOC;

   debut_column=(tid%NB_DIV_COLUMN)*NB_COLUMN_BLOC;
   fin_column=debut_column+NB_COLUMN_BLOC;

   if(((tid+1)%NB_DIV_COLUMN)==0)
      fin_column=NB_COLUMN_B; 

   if((tid+1)>(NUMTHREADS-NB_DIV_COLUMN))
      fin_line=NB_LINE_A;


   for (int k = debut_line; k < fin_line; ++k)
   {
      for (int i = debut_column; i < fin_column; ++i)
      {
          int t=0;
          for (int j = 0; j < NB_LINE_B; ++j)
          {    
             t+=a[k][j]*b[j][i];
          }
          c[k][i]=t;
      }
   }

   pthread_exit(0); 
} 
void afficher(int NB_LINE,int NB_COLUMN,int x[NB_LINE][NB_COLUMN])
{
   for (int i = 0; i < NB_LINE; ++i)
   {
       for (int j = 0; j < NB_COLUMN; ++j)
       {
           printf("%d ", x[i][j]); 
       }
       printf("\n"); 
   }
}
int main (int argc, char *argv[]) { 
   pthread_t threads[NUMTHREADS]; 
   int rc; 
   long th; 
   void *ret ;
   double debut,fin;
   srand(time(NULL));

   for (int i = 0; i < NB_LINE_A; ++i)
   {
       for (int j = 0; j < NB_COLUMN_A; ++j)
       {
           a[i][j] = rand()%NB_LINE_A;
       }
   }
   for (int i = 0; i < NB_LINE_B; ++i)
   {
       for (int j = 0; j < NB_COLUMN_B; ++j)
       {
           b[i][j] = rand()%NB_LINE_B;
       }
   }
   printf("Matrice A :\n"); 
   afficher(NB_LINE_A,NB_COLUMN_A,a);
   printf("\n"); 
   printf("Matrice B :\n"); 
   afficher(NB_LINE_B,NB_COLUMN_B,b);
   //parallele
   debut=clock();
   for(th = 0; th < NUMTHREADS; th++){ 
      rc = pthread_create(&threads[th], NULL, line_c, (void *)th); 
      if (rc){ 
         printf("Erreur de creation de thread; code erreur = %d\n", rc); 
         exit(-1); 
      } 
   }
   for(th = 0; th < NUMTHREADS; th++){ 
      (void)pthread_join(threads[th],&ret);
   }
   fin=clock();
   printf("\n"); 
   printf("temps parallele=%f\n", (double)(fin-debut)/CLOCKS_PER_SEC); 
   printf("Matrice C en parallele :\n"); 
   afficher(NB_LINE_A,NB_COLUMN_B,c);
   
int cc[NB_LINE_A][NB_COLUMN_B];
   //sequentielle
   debut=clock();
   for (int k = 0; k < NB_LINE_A; ++k)
   {
       for (int i = 0; i < NB_COLUMN_B; ++i)
       {
          int t=0;
          for (int j = 0; j < NB_LINE_B; ++j)
          {    
             t+=a[k][j]*b[j][i];
          }
          cc[k][i]=t;
       } 
   } 
   fin=clock();
   printf("\n"); 
   printf("temps seq=%f\n",  (double)(fin-debut)/CLOCKS_PER_SEC); 
   printf("Matrice C en sequencielle :\n");
   afficher(NB_LINE_A,NB_COLUMN_B,cc);
}
