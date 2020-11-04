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
  

  uint8_t r = 5, f = 2, sigma = 26; 
  double d, esp, hpar = 0.4*sigma, weight, CP, up, tmp;
  int  h, v, hf, vf;
  uint8_t *ptrimage, *ptrimagetemp, *ptrborder1, *ptrborder2, *ptrborder3; 
  uint32_t rs, cs, N, index, i;

  struct xvimage * imagetemp;
  struct xvimage * border1, * border2, * border3;
  enum x {D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12} label;

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

  //----------------------------------Compute Graph Averaging---------------------------------------

  for (i = 0; i < N; i++) {
    
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
    


    
    up=0;
    CP = 0;

    for (v = -r; v < r+1; v++)  {           //this couple of for loops search trhough B(r); in pixel q
      for (h = -r; h < r+1; h++) {

        d = 0;         
        
        for (vf = -f; vf < f+1; vf++) {      // this couple loops through B(f);   
          for (hf = -f; hf < f+1; hf++){

            switch (label) {                //compute distance in a safe zone
              
              case D1: 
                d += pow((ptrborder3[(i+hf+vf*rs+cs/2*rs+rs/2)]/1. - ptrborder3[(i+hf+h+(v+vf)*rs+cs/2*rs+rs/2)]/1.), 2.);
                break;
              case D2: 
                d += pow((ptrborder2[(i+hf+vf*rs+cs/2*rs)]/1. - ptrborder2[(i+hf+h+(v+vf)*rs+cs/2*rs)]/1.), 2.);
                break;
              case D3:
                d += pow((ptrborder3[(i+hf+vf*rs+cs/2*rs-rs/2)]/1. - ptrborder3[(i+hf+h+(v+vf)*rs+cs/2*rs-rs/2)]/1.), 2.);
                break;
              case D4:
                d += pow((ptrborder1[(i+hf+vf*rs+rs/2)]/1. - ptrborder1[(i+hf+h+(v+vf)*rs+rs/2)]/1.), 2.);
                break; 
              case D5:
                d += pow((ptrimage[(i+hf+vf*rs)]/1. - ptrimage[(i+hf+h+(v+vf)*rs)]/1.), 2.);
                break;
              case D6:
                d += pow((ptrborder1[(i+hf+vf*rs-rs/2)]/1. - ptrborder1[(i+hf+h+(v+vf)*rs-rs/2)]/1.), 2.);
                break;
              case D7:
                d += pow((ptrborder1[(i+hf+vf*rs+rs/2)]/1. - ptrborder1[(i+hf+h+(v+vf)*rs+rs/2)]/1.), 2.);
                break;
              case D8:
                d += pow((ptrimage[(i+hf+vf*rs)]/1. - ptrimage[(i+hf+h+(v+vf)*rs)]/1.), 2.);
                break;
              case D9:
                d += pow((ptrborder1[(i+hf+vf*rs-rs/2)]/1. - ptrborder1[(i+hf+h+(v+vf)*rs-rs/2)]/1.), 2.);
                break;
              case D10:
                d += pow((ptrborder3[(i+hf+vf*rs-cs/2*rs+rs/2)]/1. - ptrborder3[(i+hf+h+(v+vf)*rs-cs/2*rs+rs/2)]/1.), 2.);
                break;
              case D11:
                d += pow((ptrborder2[(i+hf+vf*rs-cs/2*rs)]/1. - ptrborder2[(i+hf+h+(v+vf)*rs-cs/2*rs)]/1.), 2.);
                break;
              case D12:
                d += pow((ptrborder3[(i+hf+vf*rs-cs/2*rs-rs/2)]/1. - ptrborder3[(i+hf+h+(v+vf)*rs-cs/2*rs-rs/2)]/1.), 2.);
                break;
              default:  
                printf("Somwthing went wrong!\n");
                break;
            }
          }
        }     

        esp = (pow(2*f+1, -2) * d) - 2 * sigma * sigma;
        if (esp < 0) esp = 0;
        weight = exp(-esp/(hpar*hpar));
        CP += weight;

        switch (label){

          case D1: 
            up += weight * ptrborder3[i+h+v*rs+cs/2*rs+rs/2];
            break;
          case D2: 
            up += weight * ptrborder2[i+h+v*rs+cs/2*rs];
            break;
          case D3: 
            up += weight * ptrborder3[i+h+v*rs+cs/2*rs-rs/2];
            break;
          case D4: 
            up += weight * ptrborder1[i+h+v*rs+rs/2]; 
            break;
          case D5: 
            up += weight * ptrimage[i+h+v*rs];
            break;
          case D6: 
            up += weight * ptrborder1[i+h+v*rs-rs/2]; 
            break;
          case D7: 
            up += weight * ptrborder1[i+h+v*rs+rs/2];
            break;
          case D8: 
            up += weight * ptrimage[i+h+v*rs];
            break;
          case D9: 
            up += weight * ptrborder1[i+h+v*rs-rs/2];
            break;
          case D10: 
            up += weight * ptrborder3[i+h+v*rs-cs/2*rs+rs/2];
            break;
          case D11: 
            up += weight * ptrborder2[i+h+v*rs-cs/2*rs];
            break;
          case D12:
            up += weight * ptrborder3[i+h+v*rs-cs/2*rs-rs/2]; 
            break;
          default:  
            printf("Somwthing went wrong!\n");
            break;

        }
      }
    } 

    // Write on buffer
    ptrimagetemp[i] = up / CP;


  }
  
  //------------------------------------------------------------------------------------------------


  //----------------------------------Writing Output Image------------------------------------------

  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];

  //------------------------------------------------------------------------------------------------

  return 1;

}
