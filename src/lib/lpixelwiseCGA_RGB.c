/* Compute Pixelwise Complete Graph Averaging  */
/* Pietro Morri - october 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mcimage.h>
#include <lpixelwiseCGA.h>

/* ==================================== */
uint32_t lpixelwiseCGA_RGB( struct xvimage * imageR,         /* input: image to process */  
                            struct xvimage * imageG,
                            struct xvimage * imageB

){                                                     /* output: modified image  */  
             
/* ==================================== */
  

  uint8_t r, f; 
  double dR, dG, dB, d, esp, hpar, weight, CP, upR, upG, upB, tmpR, tmpG, tmpB, sigma = 20.66;
  int  h, v, hf, vf;
  uint8_t *ptrimageR, *ptrimageG, *ptrimageB, *ptrimagetempR, *ptrimagetempG, *ptrimagetempB, *ptrborder1R, *ptrborder2R, *ptrborder3R, *ptrborder1G, *ptrborder2G, *ptrborder3G, *ptrborder1B, *ptrborder2B, *ptrborder3B; 
  uint32_t rs, cs, N, index, i;

  struct xvimage * imagetempR, * imagetempG, * imagetempB;
  struct xvimage * border1R, * border2R, * border3R;
  struct xvimage * border1G, * border2G, * border3G;
  struct xvimage * border1B, * border2B, * border3B;
  enum x {D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12} label;

  rs = imageR->row_size;
  cs = imageR->col_size;
  N = rs * cs;
  


  // Red channel allocation
  imagetempR = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 
  border1R = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);  
  border2R = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);  
  border3R = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 
  ptrborder1R = UCHARDATA(border1R);
  ptrborder2R = UCHARDATA(border2R);
  ptrborder3R = UCHARDATA(border3R);
  ptrimagetempR = UCHARDATA(imagetempR);  
  ptrimageR = UCHARDATA(imageR);
  
  // Green channel allocation
  imagetempG = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 
  border1G = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);  
  border2G = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);  
  border3G = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 
  ptrborder1G = UCHARDATA(border1G);
  ptrborder2G = UCHARDATA(border2G);
  ptrborder3G = UCHARDATA(border3G);
  ptrimagetempG = UCHARDATA(imagetempG);  
  ptrimageG = UCHARDATA(imageG);
  
  // Blue channel allocation
  imagetempB = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 
  border1B = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);  
  border2B = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);  
  border3B = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); 
  ptrborder1B = UCHARDATA(border1B);
  ptrborder2B = UCHARDATA(border2B);
  ptrborder3B = UCHARDATA(border3B);
  ptrimagetempB = UCHARDATA(imagetempB);  
  ptrimageB = UCHARDATA(imageB);


  //----------------------------------Choose parameters---------------------------------------------
  if(sigma < 25){
    r = 10;
    f = 1;
    hpar = 0.55*sigma;
  } 
  else if (sigma < 55){
    r = 17;
    f = 2;
    hpar = 0.4*sigma;
  }
  else {
    sigma = 100;
    r = 17;
    f = 3;
    hpar = 0.35*sigma;
  }

  //------------------------------------------------------------------------------------------------


  //-------------------------------------Create maps------------------------------------------------
  for (i = 0; i < N; i++){
    // Vertical axis
    if (i%rs < rs/2){
      tmpR = ptrimageR[i+rs/2];
      tmpG = ptrimageG[i+rs/2];
      tmpB = ptrimageB[i+rs/2];
    }
    else{
      tmpR = ptrimageR[i-rs/2];
      tmpG = ptrimageG[i-rs/2];
      tmpB = ptrimageB[i-rs/2];
    }
    ptrborder1R[i] = tmpR; 	
    ptrborder1G[i] = tmpG; 	
    ptrborder1B[i] = tmpB; 	

    //horiziontal axis
    if (i<rs*cs/2){
      tmpR = ptrimageR[i+rs*cs/2];
      tmpG = ptrimageG[i+rs*cs/2];
      tmpB = ptrimageB[i+rs*cs/2];
    }
    else{
      tmpR = ptrimageR[i-rs*cs/2];
      tmpG = ptrimageG[i-rs*cs/2];
      tmpB = ptrimageB[i-rs*cs/2];
    }
    ptrborder2R[i] = tmpR; 	
    ptrborder2G[i] = tmpG; 	
    ptrborder2B[i] = tmpB; 	

    // cross
    if (i%rs < rs/2 && i<rs*cs/2){
      tmpR = ptrimageR[i+rs*cs/2+rs/2];
      tmpG = ptrimageG[i+rs*cs/2+rs/2];
      tmpB = ptrimageB[i+rs*cs/2+rs/2];
    }
    else if (i%rs > rs/2 && i<rs*cs/2){
      tmpR = ptrimageR[i+rs*cs/2-rs/2];
      tmpG = ptrimageG[i+rs*cs/2-rs/2];
      tmpB = ptrimageB[i+rs*cs/2-rs/2];
    }
    else if (i%rs < rs/2 && i>rs*cs/2){
      tmpR = ptrimageR[i-rs*cs/2+rs/2];
      tmpG = ptrimageG[i-rs*cs/2+rs/2];
      tmpB = ptrimageB[i-rs*cs/2+rs/2];
    }
    else if (i%rs > rs/2 && i>rs*cs/2){
      tmpR = ptrimageR[i-rs*cs/2-rs/2];
      tmpG = ptrimageG[i-rs*cs/2-rs/2];
      tmpB = ptrimageB[i-rs*cs/2-rs/2];
    }
    ptrborder3R[i] = tmpR; 	
    ptrborder3G[i] = tmpG; 	
    ptrborder3B[i] = tmpB; 	
      
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
    
    
    upR=0;
    upG=0;
    upB=0;
    CP = 0;

    for (v = -r; v < r+1; v++)  {           //this couple of for loops search trhough B(r); in pixel q
      for (h = -r; h < r+1; h++) {
        
        d = 0;         
        dR = 0;         
        dG = 0;       
        dB = 0;         
        
        for (vf = -f; vf < f+1; vf++) {      // this couple loops through B(f);   
          for (hf = -f; hf < f+1; hf++){

            switch (label) {                //compute distance in a safe zone
              
              case D1: 
                dR += pow((ptrborder3R[(i+hf+vf*rs+cs/2*rs+rs/2)]/1. - ptrborder3R[(i+hf+h+(v+vf)*rs+cs/2*rs+rs/2)]/1.), 2.);
                dG += pow((ptrborder3G[(i+hf+vf*rs+cs/2*rs+rs/2)]/1. - ptrborder3G[(i+hf+h+(v+vf)*rs+cs/2*rs+rs/2)]/1.), 2.);
                dB += pow((ptrborder3B[(i+hf+vf*rs+cs/2*rs+rs/2)]/1. - ptrborder3B[(i+hf+h+(v+vf)*rs+cs/2*rs+rs/2)]/1.), 2.);
                break;
              case D2: 
                dR += pow((ptrborder2R[(i+hf+vf*rs+cs/2*rs)]/1. - ptrborder2R[(i+hf+h+(v+vf)*rs+cs/2*rs)]/1.), 2.);
                dG += pow((ptrborder2G[(i+hf+vf*rs+cs/2*rs)]/1. - ptrborder2G[(i+hf+h+(v+vf)*rs+cs/2*rs)]/1.), 2.);
                dB += pow((ptrborder2B[(i+hf+vf*rs+cs/2*rs)]/1. - ptrborder2B[(i+hf+h+(v+vf)*rs+cs/2*rs)]/1.), 2.);
                break;
              case D3:
                dR += pow((ptrborder3R[(i+hf+vf*rs+cs/2*rs-rs/2)]/1. - ptrborder3R[(i+hf+h+(v+vf)*rs+cs/2*rs-rs/2)]/1.), 2.);
                dG += pow((ptrborder3G[(i+hf+vf*rs+cs/2*rs-rs/2)]/1. - ptrborder3G[(i+hf+h+(v+vf)*rs+cs/2*rs-rs/2)]/1.), 2.);
                dB += pow((ptrborder3B[(i+hf+vf*rs+cs/2*rs-rs/2)]/1. - ptrborder3B[(i+hf+h+(v+vf)*rs+cs/2*rs-rs/2)]/1.), 2.);
                break;
              case D4:
                dR += pow((ptrborder1R[(i+hf+vf*rs+rs/2)]/1. - ptrborder1R[(i+hf+h+(v+vf)*rs+rs/2)]/1.), 2.);
                dG += pow((ptrborder1G[(i+hf+vf*rs+rs/2)]/1. - ptrborder1G[(i+hf+h+(v+vf)*rs+rs/2)]/1.), 2.);
                dB += pow((ptrborder1B[(i+hf+vf*rs+rs/2)]/1. - ptrborder1B[(i+hf+h+(v+vf)*rs+rs/2)]/1.), 2.);
                break; 
              case D5:
                dR += pow((ptrimageR[(i+hf+vf*rs)]/1. - ptrimageR[(i+hf+h+(v+vf)*rs)]/1.), 2.);
                dG += pow((ptrimageG[(i+hf+vf*rs)]/1. - ptrimageG[(i+hf+h+(v+vf)*rs)]/1.), 2.);
                dB += pow((ptrimageB[(i+hf+vf*rs)]/1. - ptrimageB[(i+hf+h+(v+vf)*rs)]/1.), 2.);
                break;
              case D6:
                dR += pow((ptrborder1R[(i+hf+vf*rs-rs/2)]/1. - ptrborder1R[(i+hf+h+(v+vf)*rs-rs/2)]/1.), 2.);
                dG += pow((ptrborder1G[(i+hf+vf*rs-rs/2)]/1. - ptrborder1G[(i+hf+h+(v+vf)*rs-rs/2)]/1.), 2.);
                dB += pow((ptrborder1B[(i+hf+vf*rs-rs/2)]/1. - ptrborder1B[(i+hf+h+(v+vf)*rs-rs/2)]/1.), 2.);
                break;
              case D7:
                dR += pow((ptrborder1R[(i+hf+vf*rs+rs/2)]/1. - ptrborder1R[(i+hf+h+(v+vf)*rs+rs/2)]/1.), 2.);
                dG += pow((ptrborder1G[(i+hf+vf*rs+rs/2)]/1. - ptrborder1G[(i+hf+h+(v+vf)*rs+rs/2)]/1.), 2.);
                dB += pow((ptrborder1B[(i+hf+vf*rs+rs/2)]/1. - ptrborder1B[(i+hf+h+(v+vf)*rs+rs/2)]/1.), 2.);
                break;
              case D8:
                dR += pow((ptrimageR[(i+hf+vf*rs)]/1. - ptrimageR[(i+hf+h+(v+vf)*rs)]/1.), 2.);
                dG += pow((ptrimageG[(i+hf+vf*rs)]/1. - ptrimageG[(i+hf+h+(v+vf)*rs)]/1.), 2.);
                dB += pow((ptrimageB[(i+hf+vf*rs)]/1. - ptrimageB[(i+hf+h+(v+vf)*rs)]/1.), 2.);
                break;
              case D9:
                dR += pow((ptrborder1R[(i+hf+vf*rs-rs/2)]/1. - ptrborder1R[(i+hf+h+(v+vf)*rs-rs/2)]/1.), 2.);
                dG += pow((ptrborder1G[(i+hf+vf*rs-rs/2)]/1. - ptrborder1G[(i+hf+h+(v+vf)*rs-rs/2)]/1.), 2.);
                dB += pow((ptrborder1B[(i+hf+vf*rs-rs/2)]/1. - ptrborder1B[(i+hf+h+(v+vf)*rs-rs/2)]/1.), 2.);
                break;
              case D10:
                dR += pow((ptrborder3R[(i+hf+vf*rs-cs/2*rs+rs/2)]/1. - ptrborder3R[(i+hf+h+(v+vf)*rs-cs/2*rs+rs/2)]/1.), 2.);
                dG += pow((ptrborder3G[(i+hf+vf*rs-cs/2*rs+rs/2)]/1. - ptrborder3G[(i+hf+h+(v+vf)*rs-cs/2*rs+rs/2)]/1.), 2.);
                dB += pow((ptrborder3B[(i+hf+vf*rs-cs/2*rs+rs/2)]/1. - ptrborder3B[(i+hf+h+(v+vf)*rs-cs/2*rs+rs/2)]/1.), 2.);
                break;
              case D11:
                dR += pow((ptrborder2R[(i+hf+vf*rs-cs/2*rs)]/1. - ptrborder2R[(i+hf+h+(v+vf)*rs-cs/2*rs)]/1.), 2.);
                dG += pow((ptrborder2G[(i+hf+vf*rs-cs/2*rs)]/1. - ptrborder2G[(i+hf+h+(v+vf)*rs-cs/2*rs)]/1.), 2.);
                dB += pow((ptrborder2B[(i+hf+vf*rs-cs/2*rs)]/1. - ptrborder2B[(i+hf+h+(v+vf)*rs-cs/2*rs)]/1.), 2.);
                break;
              case D12:
                dR += pow((ptrborder3R[(i+hf+vf*rs-cs/2*rs-rs/2)]/1. - ptrborder3R[(i+hf+h+(v+vf)*rs-cs/2*rs-rs/2)]/1.), 2.);
                dG += pow((ptrborder3G[(i+hf+vf*rs-cs/2*rs-rs/2)]/1. - ptrborder3G[(i+hf+h+(v+vf)*rs-cs/2*rs-rs/2)]/1.), 2.);
                dB += pow((ptrborder3B[(i+hf+vf*rs-cs/2*rs-rs/2)]/1. - ptrborder3B[(i+hf+h+(v+vf)*rs-cs/2*rs-rs/2)]/1.), 2.);
                break;
              default:  
                printf("Somwthing went wrong!\n");
                break;
            }
          }
        }     

        d = (dR + dG + dB)/3;
        esp = (pow(2*f+1, -2) * d) - 2 * sigma * sigma;
        if (esp < 0) esp = 0;
        weight = exp(-esp/(hpar*hpar));
        CP += weight;

        switch (label){

          case D1: 
            upR += weight * ptrborder3R[i+h+v*rs+cs/2*rs+rs/2];
            upG += weight * ptrborder3G[i+h+v*rs+cs/2*rs+rs/2];
            upB += weight * ptrborder3B[i+h+v*rs+cs/2*rs+rs/2];
            break;
          case D2: 
            upR += weight * ptrborder2R[i+h+v*rs+cs/2*rs];
            upG += weight * ptrborder2G[i+h+v*rs+cs/2*rs];
            upB += weight * ptrborder2B[i+h+v*rs+cs/2*rs];
            break;
          case D3: 
            upR += weight * ptrborder3R[i+h+v*rs+cs/2*rs-rs/2];
            upG += weight * ptrborder3G[i+h+v*rs+cs/2*rs-rs/2];
            upB += weight * ptrborder3B[i+h+v*rs+cs/2*rs-rs/2];
            break;
          case D4: 
            upR += weight * ptrborder1R[i+h+v*rs+rs/2]; 
            upG += weight * ptrborder1G[i+h+v*rs+rs/2]; 
            upB += weight * ptrborder1B[i+h+v*rs+rs/2]; 
            break;
          case D5: 
            upR += weight * ptrimageR[i+h+v*rs];
            upG += weight * ptrimageG[i+h+v*rs];
            upB += weight * ptrimageB[i+h+v*rs];
            break;
          case D6: 
            upR += weight * ptrborder1R[i+h+v*rs-rs/2]; 
            upG += weight * ptrborder1G[i+h+v*rs-rs/2]; 
            upB += weight * ptrborder1B[i+h+v*rs-rs/2]; 
            break;
          case D7: 
            upR += weight * ptrborder1R[i+h+v*rs+rs/2];
            upG += weight * ptrborder1G[i+h+v*rs+rs/2];
            upB += weight * ptrborder1B[i+h+v*rs+rs/2];
            break;
          case D8: 
            upR += weight * ptrimageR[i+h+v*rs];
            upG += weight * ptrimageG[i+h+v*rs];
            upB += weight * ptrimageB[i+h+v*rs];
            break;
          case D9: 
            upR += weight * ptrborder1R[i+h+v*rs-rs/2];
            upG += weight * ptrborder1G[i+h+v*rs-rs/2];
            upB += weight * ptrborder1B[i+h+v*rs-rs/2];
            break;
          case D10: 
            upR += weight * ptrborder3R[i+h+v*rs-cs/2*rs+rs/2];
            upG += weight * ptrborder3G[i+h+v*rs-cs/2*rs+rs/2];
            upB += weight * ptrborder3B[i+h+v*rs-cs/2*rs+rs/2];
            break;
          case D11: 
            upR += weight * ptrborder2R[i+h+v*rs-cs/2*rs];
            upG += weight * ptrborder2G[i+h+v*rs-cs/2*rs];
            upB += weight * ptrborder2B[i+h+v*rs-cs/2*rs];
            break;
          case D12:
            upR += weight * ptrborder3R[i+h+v*rs-cs/2*rs-rs/2]; 
            upG += weight * ptrborder3G[i+h+v*rs-cs/2*rs-rs/2]; 
            upB += weight * ptrborder3B[i+h+v*rs-cs/2*rs-rs/2]; 
            break;
          default:  
            printf("Somwthing went wrong!\n");
            break;

        }
      }
    } 

    // Write on buffer
    ptrimagetempR[i] = upR / CP;
    ptrimagetempG[i] = upG / CP;
    ptrimagetempB[i] = upB / CP;

  }
  
  //------------------------------------------------------------------------------------------------


  //----------------------------------Writing Output Image------------------------------------------

  for (index = 0; index < N; index++) {

    ptrimageR[index] = ptrimagetempR[index];
    ptrimageG[index] = ptrimagetempG[index];    
    ptrimageB[index] = ptrimagetempB[index];
    }

  //------------------------------------------------------------------------------------------------

  freeimage(ptrimagetempR);
  freeimage(ptrimagetempG);
  freeimage(ptrimagetempB);
  freeimage(ptrborder1R);
  freeimage(ptrborder2R);
  freeimage(ptrborder3R);
  freeimage(ptrborder1G);
  freeimage(ptrborder2G);
  freeimage(ptrborder3G);
  freeimage(ptrborder1B);
  freeimage(ptrborder2B);
  freeimage(ptrborder3B);

  return 1;

}
