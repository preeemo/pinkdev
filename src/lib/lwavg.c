/* Compute Weighted Averaging  */
/* Pietro Morri - october 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <math.h>
#include <lwavg.h>

/* ==================================== */
uint32_t lwavg(struct xvimage * image, uint32_t k){     /* input: image to process */  
                                                        /* output: modified image  */  
             
/* ==================================== */
  

  uint8_t r = 10; //r: kernel 'radius'  might pass this as function parameter
  uint32_t index, i;
  double temp, n = 0.0, kernel[(2*r+1)*(2*r+1)], tmp, R, sigma = 0.075;
  int  h, v;
  uint8_t *ptrimage, *ptrimagetemp, *ptrborder1, *ptrborder2, *ptrborder3;
  uint32_t rs, cs, N;
  enum x {D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12} label;
  struct xvimage * imagetemp;
  struct xvimage * border1, * border2, * border3;
  
  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  imagetemp = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 
  border1 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);  
  border2 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);  
  border3 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 
  ptrborder1 = UCHARDATA(border1);
  ptrborder2 = UCHARDATA(border2);
  ptrborder3 = UCHARDATA(border3);

  ptrimage = UCHARDATA(image);
  ptrimagetemp = UCHARDATA(imagetemp);

  //-------------------------Kernel initialization - simple average---------------------------------

  for (v = -r; v < r+1; v++) {
    for (h = -r; h < r+1; h++) {

      // Unitary kernel - average
      if (k == 0)                   
        kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)] = 1.0;

      //  Gaussian Kernel     
      else if (k == 1) {
        R = sqrt(h*h + v*v);
        kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)] = (exp(-(R * R) / 2*sigma*sigma* M_PI)) / (M_PI * 2*sigma*sigma);       
      }  

      n += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)];
        
    }
  }

  // kernel normalization is done when computing the average to save a couple of loops
  //------------------------------------------------------------------------------------------------

  //-------------------------------------Create maps------------------------------------------------

  for (i = 0; i < N; i++){
    // Vertical axis
    if (i%rs < rs/2)
      tmp = ptrimage[i+rs/2];
    else
      tmp = ptrimage[i-rs/2];
    ptrborder1[i] = tmp; 	

    //horiziontal axis
    if (i<rs*cs/2)
      tmp = ptrimage[i+rs*cs/2];
    else
      tmp = ptrimage[i-rs*cs/2];
    ptrborder2[i] = tmp;

    // cross
    if (i%rs < rs/2 && i<rs*cs/2)
      tmp = ptrimage[i+rs*cs/2+rs/2];
    else if (i%rs > rs/2 && i<rs*cs/2)
      tmp = ptrimage[i+rs*cs/2-rs/2];
    else if (i%rs < rs/2 && i>rs*cs/2)
      tmp = ptrimage[i-rs*cs/2+rs/2];
    else if (i%rs > rs/2 && i>rs*cs/2)
      tmp = ptrimage[i-rs*cs/2-rs/2];
    ptrborder3[i] = tmp;
      
  }

  //------------------------------------------------------------------------------------------------

  //----------------------------------Compute Moving Average----------------------------------------
  
  for (i = 0; i < N; i++) {

    temp = 0;

    if (i <= rs*r || i >= N-rs*r-1 || i % rs  <= r-1 || i % rs >= rs-1-r+1){              //in order to keep memory access low we make strange computations only on the border even though the allocated memory is kinda wasted

      // Choose copy from where to fetch the image

      if (i%rs<rs/4 && i < cs/4*rs)
        label = D1;
      else if(i%rs<3*rs/4 && i < cs/4*rs)
        label = D2;
      else if(i%rs<rs && i < cs/4*rs)
        label = D3;
      else if(i%rs<rs/4 && i < cs/2*rs)
        label = D4;
      else if(i%rs<3*rs/4 && i < cs/2*rs)
        label = D5;
      else if(i%rs<rs && i < cs/2*rs)
        label = D6;
      else if(i%rs<rs/4 && i < 3*cs/4*rs)
        label = D7;
      else if(i%rs<3*rs/4 && i < 3*cs/4*rs)
        label = D8;
      else if(i%rs<rs && i < 3*cs/4*rs)
        label = D9;
      else if(i%rs<rs/4 && i < cs*rs)
        label = D10;
      else if(i%rs<3*rs/4 && i < cs*rs)
        label = D11;
      else if(i%rs<=rs && i <= cs*rs)
        label = D12;


      // Compute weighted average

      for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++){

            switch (label){
              case D1:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder3[(i+h+v*rs+cs/2*rs+rs/2)]/(n);      
                break;
              case D2:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder2[(i+h+v*rs+cs/2*rs)]/(n);          
                break;
              case D3:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder3[(i+h+v*rs+ cs/2*rs-rs/2)]/(n);          
                break;
              case D4:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder1[(i+h+v*rs+rs/2)]/(n);                           
                break;
              case D5:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrimage[(i+h+v*rs)]/(n);      
                break;
              case D6:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder1[(i+h+v*rs-rs/2)]/(n);             
                break;
              case D7:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder1[(i+h+v*rs+rs/2)]/(n);            
                break;
              case D8:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrimage[(i+h+v*rs)]/(n);                
                break;
              case D9:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder1[(i+h+v*rs-rs/2)]/(n);
                break;
              case D10:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder3[(i+h+v*rs-cs/2*rs+rs/2)]/(n);          
                break;
              case D11:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder2[(i+h+v*rs-cs/2*rs)]/(n); 
                break;
              case D12:
                temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder3[(i+h+v*rs-cs/2*rs-rs/2)]/(n);     
                break;
            }       
        }
      }
    }

    // Safe part of the image

    else {

      for (v = -r; v < r+1; v++) {
        for (h = -r; h < r+1; h++)  {
          temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrimage[(i+h+v*rs)]/(n);          
        }
      }
    
    }
    
    // Save computation into buffer 

    ptrimagetemp[i] = temp;

  }
  //------------------------------------------------------------------------------------------------


  //-------------------------------------Rewrite on Output------------------------------------------

  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];

  //------------------------------------------------------------------------------------------------

  return 1;

}