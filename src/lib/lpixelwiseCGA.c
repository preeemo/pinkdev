/* Compute Pixelwise Complete Graph Averaging  */
/* Pietro Morri - october 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mcimage.h>
#include <lpixelwiseCGA.h>

/* ==================================== */
uint32_t lpixelwiseCGA(struct xvimage * image){     /* input: image to process */  
                                            /* output: modified image  */  
             
/* ==================================== */
  

  uint8_t r = 17, f = 2, sigma = 42; //r: kernel 'radius'  might pass this as function parameter
  uint32_t index;
  double d, esp, hpar = 0.4*sigma, weight, CP, up;  //RW[(2*r+1)*(2*r+1)], W[(2*f+1)*(2*f+1)]
  int  h, v, hf, vf;
  uint8_t *ptrimage, *ptrimagetemp; // = {1,2,1,2,4,2,1,2,1}, gaussian kernel
  uint32_t rs, cs, N;
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
          d = 0;
          CP = 0;
          for (vf = -f; vf < f+1; vf++) {       
            for (hf = -f; hf < f+1; hf++) {   

              d += pow(2*f+1, -2) * pow((ptrimage[(index+hf+vf*rs)] - ptrimage[(index+hf+h+(v+vf)*rs)]), 2);  // u(p + j)  - u (q- j)          
            }
          }         
          //printf("%f\n",d);
          esp = d - 2 * sigma * sigma;
          if (esp < 0) esp = 0;
          //printf("%f\n",esp);
          weight = exp(-esp/(hpar*hpar));
          CP += weight;
          //printf("%f\n%f\n", weight, CP);
        }
      }  

      for (v = -r; v < r+1; v++)  {           //this couple of for loops search trhough B(r); in pixel q
        for (h = -r; h < r+1; h++) {

          d = 0;
          up = 0;
          for (vf = -f; vf < f+1; vf++) {       
            for (hf = -f; hf < f+1; hf++) {   

              d += pow(2*f+1, -2) * pow((ptrimage[(index+hf+vf*rs)] - ptrimage[(index+hf+vf*rs+h+v*rs)]), 2);  // u(p + j)  - u (q- j)          
            }
          }         

          esp = d - 2 * sigma * sigma;
          if (esp < 0) esp = 0;
          weight = exp(-esp/(hpar*hpar));
          up += 1/CP * weight * ptrimage[index+h+v*rs];
        }
      }      
      //printf("%f\n",up);
      ptrimagetemp[index] = up;

    }
  }   

  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];


  return 1;

}
