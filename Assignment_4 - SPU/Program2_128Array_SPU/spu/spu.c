#include <stdio.h>
#include <string.h>
#include <spu_mfcio.h>
#include "common.h"


int MatrixSPE[16][ARRAY_SIZE] QWORD_ALIGNED;
int TransposeSPE[16][ARRAY_SIZE] QWORD_ALIGNED;
int MultSPE[16][16] QWORD_ALIGNED;
//==============================================================================
CONTROL_BLOCK cb QWORD_ALIGNED;
//==============================================================================

int main(uint64_t speid,uint64_t argp, uint64_t envp){
  int i,j,k;
 
  speid=speid;envp=envp; //avoid warnings
  //============================================================================
  // This part is used to Data input using DMA to get it from PPE
  // DMA in control block and wait for completion
  mfc_get(&cb,argp,sizeof(cb),0,0, 0);
  mfc_write_tag_mask(1 << 0);
  mfc_read_tag_status_all();
    
  // DMA in MatrixSPE and wait for completion
  int* spuptr =(int *) &MatrixSPE[0][0];  // dst, start addr 
  int* ppuptr = (int *) cb.data;           // src, start addr
  int totalwords = sizeof(MatrixSPE) >> 2;
 
  const int dt_unit = 4096;    // in words, or 4 bytes
  int* spulast = spuptr + totalwords;
  while ( spuptr < spulast ) {
    int nwords = ( spuptr + dt_unit > spulast ) ? (spulast - spuptr) : dt_unit;
    
    mfc_get(spuptr,(unsigned int) ppuptr, 4*nwords, 0,0,0);
    mfc_write_tag_mask(1 << 0);
    mfc_read_tag_status_all();
    spuptr += dt_unit;
    ppuptr += dt_unit;
  }   

   // DMA in TransposeSPE and wait for completion
   spuptr =(int *)  &TransposeSPE[0][0];  // dst, start addr 
   ppuptr = (int *) cb.data1;           // src, start addr
   totalwords = sizeof(TransposeSPE) >> 2;

   spulast = spuptr + totalwords;
   while ( spuptr < spulast ) {
       int nwords = ( spuptr + dt_unit > spulast ) ? (spulast - spuptr) : dt_unit;

          mfc_get(spuptr,(unsigned int) ppuptr, 4*nwords, 0,0,0);
          mfc_write_tag_mask(1 << 0);
          mfc_read_tag_status_all();
          spuptr += dt_unit;
          ppuptr += dt_unit;
                     }

  //============================================================================


/* Do computing...  Compute a 16*16 matrix output on each SPU,.. from 16rows and 16columns passed from PPU */
  for(i=0; i<16; i++)
	for(j=0;j<16;j++)
	for(k=0;k<ARRAY_SIZE;k++)
	  		MultSPE[i][j]+=MatrixSPE[i][k]*TransposeSPE[j][k];


 //============================================================================

 /* Send result to PPU */

 	spuptr = (int *)&MultSPE[0][0];  
	ppuptr = (int *)cb.result;  
	totalwords = sizeof(MultSPE) >> 2;

	spulast = spuptr + totalwords;
   	while ( spuptr < spulast ) {
   		 int nwords = ( spuptr + dt_unit > spulast ) ? (spulast - spuptr) : dt_unit;
   	 	mfc_put(spuptr,(unsigned int) ppuptr, 4*nwords,2,0,0);
    		mfc_write_tag_mask(1 << 2);
    		mfc_read_tag_status_all();
    		spuptr += dt_unit;
    		ppuptr += dt_unit;
  		}   

  //exit(0);  
}
