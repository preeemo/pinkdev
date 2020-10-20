/* Compute Complete Graph Averaging  */
/* Pietro Morri - october 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mcimage.h>
#include <lCGA.h>

/* ==================================== */
uint32_t lCGA(struct xvimage * image){     /* input: image to process */  
                                            /* output: modified image  */  
             
/* ==================================== */
  

  uint8_t r = 10, f = 1, sigma = 10; //r: kernel 'radius'  might pass this as function parameter
  double dp, dq, esp, hpar = 0.4*sigma, weight, CP, up;  //RW[(2*r+1)*(2*r+1)], W[(2*f+1)*(2*f+1)]
  int  h, v, hf, vf;
  uint8_t *ptrimage, *ptrimagetemp; // = {1,2,1,2,4,2,1,2,1}, gaussian kernel
  uint32_t rs, cs, N, index;
  struct xvimage * imagetemp;

  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  
  imagetemp = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 

  ptrimage = UCHARDATA(image);
  ptrimagetemp = UCHARDATA(imagetemp);

  for (index = 0; index < N; index++) {
    if (index <= rs*(r + f)|| index >= N-rs*(r-1+f))
      ptrimagetemp[index] =  ptrimage[index];
    
    
    else if (index % rs  <= r-1+f || index % rs >= rs-1-r-f)
      ptrimagetemp[index] =  ptrimage[index];


    else {                                    //index identifies current pixel in pos p
      
    
      for (v = -r; v < r+1; v++)  {           //this couple of for loops search trhough B(r); in pixel q
        for (h = -r; h < r+1; h++) {
         // index + h + rs * v  ------> q 
          dp = 0;
          dq = 0;
          CP = 0;
          for (vf = -f; vf < f+1; vf++) {       
            for (hf = -f; hf < f+1; hf++) {   
              dq += ptrimage[(index+hf+h+(v+vf)*rs)];  // u(p + j)  - u (q- j)      
            }
          }   
          for (vf = -f; vf < f+1; vf++) {       
            for (hf = -f; hf < f+1; hf++) {   
              dp += ptrimage[(index+hf+h+(v+vf)*rs)];  // u(p + j)  - u (q- j)   
            }
          }   
          esp = dp - dq;      
          esp = (pow(2*f+1, -2) * esp) - 2 * sigma * sigma;
          //printf("d = %f\n esp = %f\n", d, esp);
          if (esp < 0) esp = 0;
          weight = exp(-esp/(hpar*hpar));
          up += weight * ptrimage[index+h+v*rs];
          CP += weight;
        }
      }  

      ptrimagetemp[index] = 1/CP * up;

    }
  }   

  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];


  return 1;

}
