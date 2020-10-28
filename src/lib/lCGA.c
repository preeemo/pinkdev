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
  

  uint8_t r = 10, f = 1, sigma = 6;
  double dp, dq, esp, hpar = 0.4*sigma, weight, CP, up, B;  
  int  h, v, hf, vf;
  uint8_t *ptrimage, *ptrimagetemp; 
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
      up = 0;
      CP = 0;
      B = 0;
      Q = 0;
      for (v = -r; v < r+1; v++)  {           //this couple of for loops search trhough B(r); in pixel q
        for (h = -r; h < r+1; h++) {
         // index + h + rs * v  ------> q 
          B += ptrimage[(index+h+v*rs)];
        }
      }    
      B /= (2*r+1);    
      for (v = -r; v < r+1; v++)  {           //this couple of for loops search trhough B(r); in pixel q
        for (h = -r; h < r+1; h++) {
        
          for (vf = -f; vf < f+1; vf++) {       
            for (hf = -f; hf < f+1; hf++) {   
                  
              Q += ptrimage[(index+hf+h+(v+vf)*rs)];  
            }
          }    
          Q /= (2*f+1);          
                
          esp = Q - B;      
          esp = (esp/((2*f+1)*(2*f+1))) - 2 * sigma * sigma;
          if (esp < 0) esp = 0;
          weight = exp(-esp/(hpar*hpar));
          up += weight * Q;
          CP += weight;
    
        }
      }    
      //B /= (2*r+1);
      CP

        }
      }  

      ptrimagetemp[index] = 1/CP * up;

    

  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];


  return 1;

}
