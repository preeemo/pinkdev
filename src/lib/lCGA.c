/* Compute Complete Graph Averaging  */
/* Pietro Morri - october 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mcimage.h>
#include <lCGA.h>

/* ==================================== */
uint32_t lCGA(struct xvimage * image){      /* input: image to process */  
                                            /* output: modified image  */           
/* ==================================== */
  

  uint8_t r = 10, f = 2, sigma = 26;
  double d, esp, hpar = 0.4*sigma, weight, CP, up, Qest[(2*f+1)*(2*f+1)], UQ, Q;  
  int  h, v, hf, vf, hq, vq;

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
    if (index <= rs*(r + 2*f)|| index >= N-rs*(r-1+2*f))
      ptrimagetemp[index] =  ptrimage[index];
    
    
    else if (index % rs  <= r-1+2*f || index % rs >= rs-1-r-2*f)
      ptrimagetemp[index] =  ptrimage[index];

    else {
    

  
      for (vq = -f; vq < f+1; vq++) {                                         // get estimate Qi for all Q in B(p,f)
        for (hq = -f; hq < f+1; hq++) {
              
          //d = 0;         
          Q = 0;
          CP = 0;

          //calculate w(B,Q)
          for (v = -r; v < r+1; v++) {           //this couple of for loops search trhough B(r); in pixel q
            for (h = -r; h < r+1; h++) {
              
              d = 0;
              UQ = 0;

              for (vf = -f; vf < f+1; vf++) {         
                for (hf = -f; hf < f+1; hf++) {
                  d += pow((ptrimage[(index+hf+hq+(vf+vq)*rs)]/1. - ptrimage[(index+hf+h+hq+(v+vf+hq)*rs)]/1.), 2.);
                  UQ += pow((2*f+1), -2.) * ptrimage[(index+h+hf+hq+(vf+v+vq)*rs)];
                
                }
              }
              
              esp = (pow(2*f+1, -2) * d) - 2 * sigma * sigma;
              if (esp < 0) esp = 0;
              weight = exp(-esp/(hpar*hpar));
              CP += weight; 
              Q += UQ * weight;
            }
          }
          
          Qest[(hq+vq*(2*f+1)+(2*f+1)*(2*f+1)/2)] = Q/CP;

        }                  

      }  

      up = 0; 


      for (vf = -f; vf < f+1; vf++) {                                         // get color for pixel p, u(p) by averaging the estimates Q
        for (hf = -f; hf < f+1; hf++)                   
          up += pow((2*f+1), -2) * Qest[(hf+vf*(2*f+1)+(2*f+1)*(2*f+1)/2)];
      }  

      ptrimagetemp[index] = up;
   
    }

  }  


    

  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];


  return 1;

}

