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
  

  uint8_t r = 5; //r: kernel 'radius'  might pass this as function parameter
  uint32_t index, i;
  double temp, kernel[(2*r+1)*(2*r+1)];
  int  h, v;
  uint8_t *ptrimage, *ptrimagetemp; // = {1,2,1,2,4,2,1,2,1}, gaussian kernel
  uint32_t rs, cs, N, n = 0;
  struct xvimage * imagetemp;

  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  
  imagetemp = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 

  //------------Kernel initialization - simple average--------------
  for (i = 0; i < (2*r+1)*(2*r+1); i++){
    kernel[i] = 1.0; 	
    n += kernel[i];
  }
  //----------------------------------------------------------------

  ptrimage = UCHARDATA(image);
  ptrimagetemp = UCHARDATA(imagetemp);

  for (index = 0; index < N; index++) {
    if (index <= rs*r || index >= N-rs*r-1)
      ptrimagetemp[index] =  ptrimage[index];
    
    
    else if (index % rs  <= r-1 || index % rs >= rs-1-r+1)
      ptrimagetemp[index] =  ptrimage[index];


    else {
      
      temp = 0;
      
      for (v = -r; v < r+1; v++) {
        for (h = -r; h < r+1; h++)           
          temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrimage[(index+h+v*rs)]/(n);  
      }
      
      ptrimagetemp[index] = temp;
    
    }
  }
  
  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];


  return 1;

}



/* Compute Weighted Averaging  */
/* Pietro Morri - september 2020 */
/*
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lwavg.h>
/*
/* ==================================== */
//uint32_t lwavg(struct xvimage * image){     /* input: image to process */  
                                            /* output: modified image  */  
             
/* ==================================== */
/*
  uint32_t index, j;
  int h;
  float temp, kernel[9];
  uint8_t *ptrimage, *ptrimagetemp, i; // = {1,2,1,2,4,2,1,2,1}, gaussian kernel
  uint32_t rs, cs, N, n = 0;
  struct xvimage * imagetemp;

  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  
  imagetemp = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 

  //------------Kernel initialization - simple average--------------
  for (i = 0; i < 2*r +1; i++){
    kernel[i] = 1.0; 	
    n += kernel[i];
    
  }
  //----------------------------------------------------------------

  ptrimage = UCHARDATA(image);
  ptrimagetemp = UCHARDATA(imagetemp);

  for (index = 0; index < N; index++) {
    if (index <= rs || index >= N-rs)
      ptrimagetemp[index] = ptrimage[index];
    
    
    else if (index % rs  == 0 || index % rs == rs-1)
      ptrimagetemp[index] = ptrimage[index];
    else {
      temp = 0;     
      
      //temp = (float)((ptrimage[index-rs-1]*kernel[0] + ptrimage[index-1]*kernel[3] + ptrimage[index+rs-1]*kernel[6] + ptrimage[index-rs]*kernel[1] + ptrimage[index]*kernel[4] + ptrimage[index+rs]*kernel[7] + ptrimage[index-rs+1]*kernel[2] + ptrimage[index+1]*kernel[5] + ptrimage[index+rs+1]*kernel[8])/n);
      
      for (h=-1; h<2; h++){
        temp = temp + (ptrimage[index-rs+h]*kernel[1+h] + ptrimage[index+h]*kernel[4+h] + ptrimage[index+rs+h]*kernel[7+h])/(n);
      }
      ptrimagetemp[index] = (uint8_t)temp;
    }
          
  }

      printf("%d\n", n);
  
  for (j = 0; j < N; j++)  
    ptrimage[j] = ptrimagetemp[j];


  return 1;

}
*/