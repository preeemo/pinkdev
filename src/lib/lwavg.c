/* Compute Weighted Averaging  */
/* Pietro Morri - september 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lwavg.h>

/* ==================================== */
uint32_t lwavg(struct xvimage * image /* input: image to process */  
                                           /* output: modified image  */  
             )
/* ==================================== */
{
  uint32_t index;
  uint8_t *ptrimage, kernel[9];
  int i;
  uint32_t rs, cs, N;

  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  
  uint32_t newval[N];
  
  //------------Kernel initialization - simple average-----------------
  for (i = 0; i < 9; i++){
    kernel[i] = 1; 	
    printf("%d", kernel[i]);
  }
  //----------------------------------------------------------------
 
  ptrimage = UCHARDATA(image);
  
//  for (index = rs ; index < N - rs; index++) {
  for (index = 0; index < N; index++) {
    
    if (index <= rs || index >= N-rs-1)
      newval[index] = ptrimage[index];
    
    if (index % rs  == 0 || index % rs == rs-1)
    //continue;
      newval[index] = ptrimage[index];
    else {
      newval[index] = (uint32_t)((ptrimage[index-rs-1]*kernel[0] + ptrimage[index-1]*kernel[3] + ptrimage[index+rs-1]*kernel[6] + ptrimage[index-rs]*kernel[1] + ptrimage[index]*kernel[4] + ptrimage[index+rs]*kernel[7] + ptrimage[index-rs+1]*kernel[2] + ptrimage[index+1]*kernel[5] + ptrimage[index+rs+1]*kernel[8])/9);
       
    }
    
    
  }
 /* for (index = 0; index < N; index++) {
 
    ptrimage[index] = (uint8_t)newval[index];
  
  }*/
  return 1;
}





