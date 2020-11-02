/* Compute Weighted Averaging  */
/* Pietro Morri - october 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <math.h>
#include <lwavg.h>

/* ==================================== */
uint32_t lwavg(struct xvimage * image){     /* input: image to process */  
                                            /* output: modified image  */  
             
/* ==================================== */
  

  uint8_t r = 10; //r: kernel 'radius'  might pass this as function parameter
  uint32_t index, i;
  double temp, kernel[(2*r+1)*(2*r+1)], tmp;
  int  h, v;
  uint8_t *ptrimage, *ptrimagetemp; // = {1,2,1,2,4,2,1,2,1}, gaussian kernel
  uint32_t rs, cs, N, n = 0;
  struct xvimage * imagetemp;
   
  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  imagetemp = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 
  
  ptrimage = UCHARDATA(image);
  ptrimagetemp = UCHARDATA(imagetemp);

  //-------------------------Kernel initialization - simple average---------------------------------
  for (i = 0; i < (2*r+1)*(2*r+1); i++){
    kernel[i] = 1.0; 	
    n += kernel[i];
  }
  //------------------------------------------------------------------------------------------------

  //----------------------------------Compute Moving Average----------------------------------------
  for (i = 0; i < N; i++) {

    temp = 0;

    if (i <= rs*r || i >= N-rs*r-1 || i % rs  <= r-1 || i % rs >= rs-1-r+1){              //in order to keep memory access low we make strange computations only on the border even though the allocated memory is kinda wasted
      temp = ptrimage[i];
    }
      
    // safe part of the image
    else{
      for (v = -r; v < r+1; v++) {
        for (h = -r; h < r+1; h++)  {
          temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrimage[(i+h+v*rs)]/(n);          
        }
      }
    }
    
    ptrimagetemp[i] = temp;

  }
  //------------------------------------------------------------------------------------------------



  //-------------------------------------Rewrite on Output------------------------------------------

  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];

  //------------------------------------------------------------------------------------------------

  return 1;

}