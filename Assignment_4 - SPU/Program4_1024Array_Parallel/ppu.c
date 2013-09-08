/* Parallelize Program by computing 16*16 elements on one SPU by passing 16rows and 16columns.
Parallizing to 32 rows and 32 columns was not possible as it exceeded the local storage needed on a SPU.
Computation done by dividing the 1024*1024 matrix, into smaller chunks and computing 16*16 matrix on one SPE and then combining the output from all SPE's*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <libspe2.h>
#include <cbe_mfc.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include "common.h"

//============================================================================
// This part is input
int Matrix[ARRAY_SIZE][ARRAY_SIZE] QWORD_ALIGNED;
int Transpose[ARRAY_SIZE][ARRAY_SIZE] QWORD_ALIGNED;
int Mult[ARRAY_SIZE][ARRAY_SIZE] QWORD_ALIGNED;
unsigned int *ptr=&Mult[0][0];
void printMatrix();
//============================================================================
CONTROL_BLOCK cb[8] QWORD_ALIGNED;
extern spe_program_handle_t spu;

// Data structure for running SPE thread
typedef struct ppu_pthread_data {
  spe_context_ptr_t speid;
  pthread_t pthread;
  void *argp;
} ppu_pthread_data_t;

ppu_pthread_data_t data[8];
int id=0;

// create and run one SPE thread
void *ppu_pthread_function(void *arg) {

  ppu_pthread_data_t *datap = (ppu_pthread_data_t *)arg;
  int rc;
  unsigned int entry = SPE_DEFAULT_ENTRY;
  if ((rc = spe_context_run(datap->speid, &entry, 0, datap->argp, NULL, NULL)) < 0) {
    fprintf (stderr, "Failed spe_context_run(rc=%d, errno=%d, strerror=%s)\n", rc, errno, strerror(errno));
    exit (1);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  
  int rc;
  unsigned int i,j,l,m;
  //============================================================================  
  // Initialize array elements
for(i=0; i<ARRAY_SIZE; i++)
  {
	for(j=0;j<ARRAY_SIZE;j++)
	 { 
	   Matrix[i][j] = rand()%1000;
	   Transpose[j][i]=Matrix[i][j]; 
	}
  }

printf("Computing... Wait for some time!!!\n");
#ifdef TIMING  
 /********************************************************/
  long unsigned int start_t, end_t;
  start_t=__mftb();
 /********************************************************/
#endif



  // Fill in control block
 //============================================================================
  // create SPE context and load SPE program into the SPE context

for(i=0;i<64;i++)                    // From 8 available SPU 2048 elements were computed in parallel, So we needed i*j parallel iterations to compute entire matrix
{
  for(j=0;j<8;j++)                  // Total Multiplication matrix computed in i*j=512 parallel iterations on 8 SPUs available.
    {
	for(l=0;l<8;l++)               //Parallel Iteration on 8 available SPU.. Each SPU giving 256 elements.
	{
	//printf("\n%d %d %d",i,j,l);
	cb[l].data=(unsigned int)Matrix[i*16];
  	cb[l].data1=(unsigned int)Transpose[((j*8)+l)*16];
  	cb[l].result=(unsigned int)&Mult[0][(i*64+j*8+l)*256];
  
	if ((data[l].speid = spe_context_create (0, NULL)) == NULL)
    	{
      		fprintf (stderr, "Failed spe_context_create(errno=%d strerror=%s)\n", errno, strerror(errno));
      		exit (3);
    	}

 
 //Load program

  	if ((rc = spe_program_load (data[l].speid, &spu)) != 0)
    	{	
      		fprintf (stderr, "Failed spe_program_load(errno=%d strerror=%s)\n", errno, strerror(errno));
      		exit (3);
    	}
  
  	data[l].argp = (unsigned long long *) &cb[l];

 // create SPE pthreads
  	if ((rc = pthread_create (&data[l].pthread, NULL, &ppu_pthread_function, &data[l])) != 0)
    	{
      		fprintf (stderr, "Failed pthread_create(errno=%d strerror=%s)\n", errno, strerror(errno));
      		exit (3);
    	}      

     }
 
   // wait for SPE to complete
     for(m=0;m<8;m++)
     {
	if ((rc = pthread_join (data[m].pthread, NULL)) != 0)
    	{
      		fprintf (stderr, "Failed pthread_join(rc=%d, errno=%d strerror=%s)\n", rc, errno, strerror(errno));
      		exit (1);
    	}


  	if ((rc = spe_context_destroy (data[m].speid)) != 0)
    	{
      		fprintf (stderr, "Failed spe_context_destroy(rc=%d, errno=%d strerror=%s)\n", rc, errno, strerror(errno));
      		exit (1);
    	}
     }	

   }
}

#ifdef TIMING  
  //compute the timein us(10E-6)       
    end_t=__mftb();
      printf("\n\nTotal time used by SPE: %f us\n", (float)((end_t-start_t)/79.8));  
#endif
 printf("\n\nPrinting matrix in 3 seconds..\n");
 sleep(3);
 printMatrix();

 printf("\n\nTotal time used by SPE: %f us\n\n", (float)((end_t-start_t)/79.8));

}

void printMatrix()
{
        int i,j,k,l;
        int *ptr = &Mult[0][0];
	int result[1024][1024];
        int *ptr_store =(int *) &result;
        int *temp,*ptr1;	

	for(l=0; l<64; l++)  // for vertical 64 matrices, of 16*16 size
	{
		ptr =(int *) &Mult[l*16];
		for (i=0; i < 16; i++) // for each row of 16*16 matrix
		{
			ptr1 = ptr;
			for (j=0; j< 64 ; j++) // for horizontal 64 matrices of 16*16 size.
			{
				temp = ptr1;
				for(k=0; k<16; k++, temp++) // for 16 elements per row
				{
					printf("%u\t", *temp);
					*ptr_store++ = *temp;					
				}
				ptr1 = ptr1 + 16 * 16;
			}
			printf("\n");
			ptr = ptr + 16;
		}
	}
}

