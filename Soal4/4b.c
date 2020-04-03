#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BELUM_READY -1
#define READY 0
#define SIAP 1
#define MAXIMUM 100


struct shared{
    int status;
    int data[100];
};
int baris = 0;;
int KOLOM = 0;
void* jumlah(void* arg) {
  int i = *((int*)arg);
  free(arg);;
  int total = 0;
  for(int j = 0; j <= i ;j++){
    total += j;
  }
  if(KOLOM > 4){
    printf("\n");
    KOLOM = 0;
  }
  printf("%2d %6d ",i,total);
  KOLOM ++;
}


int main()
{
     key_t          ShmKEY;
     int            ShmID;
     struct shared  *ShmPTR;

     ShmKEY = ftok("key",100);
     ShmID = shmget(ShmKEY,sizeof(struct shared),0666);
     if(ShmID < 0){
       printf("Client Error\n");
       exit(1);
     }
     ShmPTR = (struct shared*) shmat(ShmID, NULL, 0);

     while (ShmPTR->status != READY);

      pthread_t tid[20];
      for(int i = 0; i < 20;i++){
         int *x =  malloc(sizeof(*x));
         if( x == NULL){
           printf("Malloc Error\n");
           exit(1);
         }
         *x = ShmPTR->data[i];
         pthread_create(&(tid[i]), NULL, &jumlah, x);
        pthread_join(tid[i], NULL);

      }

      ShmPTR->status = SIAP;
      shmdt((void *) ShmPTR);

      printf("\n");


    return 0;
}
