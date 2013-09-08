#include <stdio.h>
#include <string.h>
#include <spu_mfcio.h>
#include "common.h"

 int arrayInSPE[ARRAY_SIZE] QWORD_ALIGNED;

//==============================================================================
CONTROL_BLOCK cb QWORD_ALIGNED;
//==============================================================================

int main(uint64_t speid,uint64_t argp, uint64_t envp){
  int i,j;
  
  
  //============================================================================
  // This part is used to Data input using DMA to get it from PPE
  // DMA in control block and wait for completion
  mfc_get(&cb,argp,sizeof(cb),0,0, 0);
  mfc_write_tag_mask(1 << 0);
  mfc_read_tag_status_all();
    
  // DMA in arrayInSPE and wait for completion, Be careful of 16 KB
  int* spuptr = &arrayInSPE[0];  // dst, start addr 
  int* ppuptr = cb.data;           // src, start addr
  const int totalwords = sizeof(arrayInSPE) >> 2;
  
  const int dt_unit = 4096;    // in words, or 4 bytes
  int* spulast = spuptr + totalwords;
  //int iter = 0;
  while ( spuptr < spulast ) {
    int nwords = ( spuptr + dt_unit > spulast ) ? (spulast - spuptr) : dt_unit;
    
    mfc_get(spuptr, ppuptr, 4*nwords, 0,0,0);
    mfc_write_tag_mask(1 << 0);
    mfc_read_tag_status_all();
    spuptr += dt_unit;
    ppuptr += dt_unit;
  }   
  //============================================================================
  
  /* Do computing */
  for(i=0; i<ARRAY_SIZE; i++)
	  arrayInSPE[i] *= 2;
  //send data back to ppu
	
	spuptr = &arrayInSPE[0];  
	ppuptr = cb.data;  

    while ( spuptr < spulast ) {
    int nwords = ( spuptr + dt_unit > spulast ) ? (spulast - spuptr) : dt_unit;
    
    mfc_put(spuptr, ppuptr, 4*nwords, 0,0,0);
    mfc_write_tag_mask(1 << 2);
    mfc_read_tag_status_all();
    spuptr += dt_unit;
    ppuptr += dt_unit;
  }   

  exit(0);  
}
