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
int arrayInPPE1[ARRAY_SIZE][ARRAY_SIZE]  QWORD_ALIGNED;
int arrayResult[ARRAY_SIZE][ARRAY_SIZE]  QWORD_ALIGNED;
//============================================================================

int main(int argc, char *argv[]) {
  
  int row1,col1,row2;

  //============================================================================  
  // Initialize array elements
  for(row1=0; row1<ARRAY_SIZE; row1++)
  {	
	for(col1=0;col1<ARRAY_SIZE;col1++)
	{
		 arrayInPPE1[row1][col1] = rand()%100;
	
	}
  }


  long unsigned int start_t,end_t;
  start_t = __mftb();

//=========================================================================
/*Loop for array computation*/
for(row1 = 0;row1<ARRAY_SIZE;row1++)
{
	for(col1=0;col1< ARRAY_SIZE;col1++)
	{
		arrayResult[row1][col1] = 0;
		for(row2 = 0;row2< ARRAY_SIZE;row2++)
		{
			arrayResult[row1][col1] += arrayInPPE1[row1][row2]*arrayInPPE1[row2][col1];
		}
	}
}
	
end_t = __mftb();	  

//=========================================================================
   /* Print out the multiplication result */
  for(row1=0; row1<ARRAY_SIZE; row1++)
{
	printf("\n");
	for(col1=0;col1<ARRAY_SIZE;col1++)
	{
	      printf("%d\t",arrayResult[row1][col1]);

	}
}

printf("\nArray multiplication computation time is %f us\n",(float)(end_t-start_t)/79.8);
  
  return 0;
}



