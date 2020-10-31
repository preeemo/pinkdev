/* Compute Weighted Averaging  */
/* Pietro Morri - september 2020 */

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
  

  uint8_t r = 25; //r: kernel 'radius'  might pass this as function parameter
  uint32_t index, i;
  double temp, kernel[(2*r+1)*(2*r+1)], tmp;
  int  h, v;
  uint8_t *ptrimage, *ptrimagetemp, *ptrborder1, *ptrborder2, *ptrborder3; // = {1,2,1,2,4,2,1,2,1}, gaussian kernel
  uint32_t rs, cs, N, n = 0;
  struct xvimage * imagetemp;
  struct xvimage * border1, * border2, * border3;//, * border2, * border3;

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

  //------------Kernel initialization - simple average--------------
  for (i = 0; i < (2*r+1)*(2*r+1); i++){
    kernel[i] = 1.0; 	
    n += kernel[i];
  }
  //----------------------------------------------------------------

  //------------------------Create maps-----------------------------
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
  //----------------------------------------------------------------


  ptrimagetemp = UCHARDATA(imagetemp);
  
  for (i = 0; i < N; i++) {

    temp = 0;

    //if (i <= rs*r || i >= N-rs*r-1 || i % rs  <= r-1 || i % rs >= rs-1-r+1){              //in order to keep memory access low we make strange computations only on the border even though the allocated memory is kinda wasted
      
      if (i%rs<rs/4 && i < cs/4*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder3[(i+h+v*rs+cs/2*rs+rs/2)]/(n);          
            
        }
      }

      else if (i%rs<3*rs/4 && i < cs/4*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder2[(i+h+v*rs+cs/2*rs)]/(n);          
        }

      }

      
      else if (i%rs<rs && i < cs/4*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder3[(i+h+v*rs+ cs/2*rs-rs/2)]/(n);          
        }

      }
      
      else if (i%rs<rs/4 && i < cs/2*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder1[(i+h+v*rs+rs/2)]/(n);                           
        }
      }

      else if (i%rs<3*rs/4 && i < cs/2*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrimage[(i+h+v*rs)]/(n);        
        }
      }

      else if (i%rs<rs && i < cs/2*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder1[(i+h+v*rs-rs/2)]/(n);        
        }
      }


      else if (i%rs<rs/4 && i < 3*cs/4*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder1[(i+h+v*rs+rs/2)]/(n);        
        }
      }

      else if (i%rs<3*rs/4 && i < 3*cs/4*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrimage[(i+h+v*rs)]/(n);        
        }
      }

      else if (i%rs<rs && i < 3*cs/4*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder1[(i+h+v*rs-rs/2)]/(n);        
        }
      }


      else if (i%rs<rs/4 && i < cs*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder3[(i+h+v*rs-cs/2*rs+rs/2)]/(n);        
        }
      }

      else if (i%rs<3*rs/4 && i < cs*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder2[(i+h+v*rs-cs/2*rs)]/(n);        
        }
      }

      else if (i%rs<rs && i < cs*rs){
        for (v = -r; v < r+1; v++) {
          for (h = -r; h < r+1; h++)
            temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrborder3[(i+h+v*rs-cs/2*rs-rs/2)]/(n);        
        }
        
      }
      
    //}

    // safe part of the image
    //else{
      //for (v = -r; v < r+1; v++) {
        //for (h = -r; h < r+1; h++)  {
        //  temp += kernel[(h+v*(2*r+1)+(2*r+1)*(2*r+1)/2)]*ptrimage[(i+h+v*rs)]/(n);          
      //}
    //}
      //ptrimagetemp[i] = temp;
    //}
    
    ptrimagetemp[i] = temp;

  }
  
  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];


  return 1;

}



/* if (index <= rs*r || index >= N-rs*r-1)   // horizontal borders check
          
        
    
        else if (index % rs  <= r-1 || index % rs >= rs-1-r+1) //vertical border check
          continue;
    
        else */