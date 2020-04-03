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


int matrixA[4][2];
int matrixb[2][5];
int matrixc[4][5];

int baris = 0;
int kolom = 0;

void* buat_kali(void* arg) {
  if(kolom >= 5){
    kolom = 0;
    baris++;
  }
  for(int i = 0; i < 2;i++){
    matrixc[baris][kolom] += matrixA[baris][i] * matrixb[i][kolom];
  }
  kolom++;
}

int main() {
  srand(time(NULL));
  printf("Matrix A : \n");
  for (int i = 0; i < 4; i++) {
    /* code */
    for (int j = 0; j < 2; j++) {
      matrixA[i][j] = rand()%19+1;
      printf("%2d", matrixA[i][j]);
      /* code */
    }
    printf("\n");
  }

  printf("Matrix B : \n");
  for (int i = 0; i < 2; i++) {
    /* code */
    for (int j = 0; j < 5; j++) {
      /* code */
      matrixb[i][j] = rand()%19+1;
      printf("%2d", matrixb[i][j]);
    }
    printf("\n");
  }

  pthread_t tid[20];

  for (int i = 0; i < 20; i++) {
    /* code */
    pthread_create(&(tid[i]), NULL, &buat_kali, NULL);
  }

  for (int i = 0; i < 20; i++) {
    /* code */
    pthread_join(tid[i], NULL);
  }


  printf("Hasil kali A dan B :\n");
  for (int i = 0; i < 4; i++) {
    /* code */
    for (int j = 0; j < 5; j++) {
      printf("%4d", matrixc[i][j]);
      /* code */
    }
    printf("\n");
  }

   key_t          ShmKEY;
   int            ShmID;
   struct shared  *ShmPTR;

   ShmKEY = ftok("key",100);
   ShmID = shmget(ShmKEY,sizeof(struct shared),IPC_CREAT|0666);
   if(ShmID < 0){
     printf("Shmget error\n");
     exit(1);
   }
   ShmPTR = (struct shared *) shmat(ShmID, NULL, 0);

   ShmPTR->status = BELUM_READY;
   int j = 0;
   int k = 0;

   for(int i = 0; i < 20; i++){
     /* code */
     if(k >= 5){
       /* code */
       j++;
       k = 0;
     }
     ShmPTR->data[i] = matrixc[j][k];
     k++;
   }
    ShmPTR->status = READY;

    printf("Jalanin yang B \n");
    while (ShmPTR->status != SIAP)
        sleep(1);
    printf("B udah jalan\n");
    shmdt((void *) ShmPTR);
    printf("Dah share ke B\n");
    shmctl(ShmID, IPC_RMID, NULL);
    exit(0);
    return 0;
}
