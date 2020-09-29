/* Compute Weighted Averaging  */
/* Pietro Morri - september 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lwavg.h>

/* ==================================== */
uint32_t lwavg(struct xvimage * image){     /* input: image to process */  
                                            /* output: modified image  */  
             
/* ==================================== */

  uint32_t index, j;
  float temp;
  uint8_t *ptrimage, *ptrimagetemp, i, kernel[9]; // = {1,2,1,2,4,2,1,2,1}, guassian kernel
  uint32_t rs, cs, N, n = 0;
  struct xvimage * imagetemp;
  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  
  imagetemp = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 

  //------------Kernel initialization - simple average--------------
  for (i = 0; i < 9; i++){
    kernel[i] = 1; 	
    n = n + kernel[i];
  }
  //----------------------------------------------------------------

  ptrimage = UCHARDATA(image);
  ptrimagetemp = UCHARDATA(imagetemp);

  for (index = 0; index < N; index++) {
    if (index <= rs || index >= N-rs-1)
      ptrimagetemp[index] = ptrimage[index];
    
    else if (index % rs  == 0 || index % rs == rs-1)
      ptrimagetemp[index] = ptrimage[index];
    else {
      temp = (float)((ptrimage[index-rs-1]*kernel[0] + ptrimage[index-1]*kernel[3] + ptrimage[index+rs-1]*kernel[6] + ptrimage[index-rs]*kernel[1] + ptrimage[index]*kernel[4] + ptrimage[index+rs]*kernel[7] + ptrimage[index-rs+1]*kernel[2] + ptrimage[index+1]*kernel[5] + ptrimage[index+rs+1]*kernel[8])/n);
      
      ptrimagetemp[index] = (uint8_t)temp;
    }      
  }
  
  for (j = 0; j < N; j++)  
    ptrimage[j] = ptrimagetemp[j];
  
  return 1;

}