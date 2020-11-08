/* Compute Complete Graph Averaging  */
/* Pietro Morri - october 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mcimage.h>
#include <lCGA_RGB.h>

/* ==================================== */
uint32_t lCGA_RGB(struct xvimage * imageR,
                  struct xvimage * imageG,
                  struct xvimage * imageB

){      /* input: image to process */  
                                            /* output: modified image  */           
/* ==================================== */
  

  uint8_t r, f, sigma = 26;
  double d, dR, dG, dB, esp, hpar, weight, CP, upR, upG, upB, QestR[(2*f+1)*(2*f+1)], QestG[(2*f+1)*(2*f+1)], QestB[(2*f+1)*(2*f+1)], UQR, QR, UQG, QG, UQB, QB, tmpR, tmpG, tmpB;  
  int  h, v, hf, vf, hq, vq;

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
    




    for (vq = -f; vq < f+1; vq++) {                                         // get estimate Qi for all Q in B(p,f)
      for (hq = -f; hq < f+1; hq++) {
            
        //d = 0;         
        QR = 0;
        QG = 0;
        QB = 0;
        CP = 0;

        //calculate w(B,Q)
        for (v = -r; v < r+1; v++) {           //this couple of for loops search trhough B(r); in pixel q
          for (h = -r; h < r+1; h++) {
            
            dR = 0;
            UQR = 0;
            dG = 0;
            UQG = 0;
            dB = 0;
            UQB = 0;

            for (vf = -f; vf < f+1; vf++) {         
              for (hf = -f; hf < f+1; hf++) {
                switch (label) {                //compute distance in a safe zone
              
                  case D1: 
                    dR += pow((ptrborder3R[(i+hf+hq+(vf+vq)*rs+cs/2*rs+rs/2)]/1. - ptrborder3R[(i+hf+hq+h+(v+vf+vq)*rs+cs/2*rs+rs/2)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder3R[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs+rs/2)];
                    dG += pow((ptrborder3G[(i+hf+hq+(vf+vq)*rs+cs/2*rs+rs/2)]/1. - ptrborder3G[(i+hf+hq+h+(v+vf+vq)*rs+cs/2*rs+rs/2)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder3G[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs+rs/2)];
                    dB += pow((ptrborder3B[(i+hf+hq+(vf+vq)*rs+cs/2*rs+rs/2)]/1. - ptrborder3B[(i+hf+hq+h+(v+vf+vq)*rs+cs/2*rs+rs/2)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder3B[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs+rs/2)];
                    break;
                  case D2: 
                    dR += pow((ptrborder2R[(i+hf+hq+(vf+vq)*rs+cs/2*rs)]/1. - ptrborder2R[(i+hf+h+hq+(v+vf+vq)*rs+cs/2*rs)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder2R[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs)];
                    dG += pow((ptrborder2G[(i+hf+hq+(vf+vq)*rs+cs/2*rs)]/1. - ptrborder2G[(i+hf+h+hq+(v+vf+vq)*rs+cs/2*rs)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder2G[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs)];
                    dB += pow((ptrborder2B[(i+hf+hq+(vf+vq)*rs+cs/2*rs)]/1. - ptrborder2B[(i+hf+h+hq+(v+vf+vq)*rs+cs/2*rs)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder2B[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs)];
                    break;
                  case D3:
                    dR += pow((ptrborder3R[(i+hf+hq+(vf+vq)*rs+cs/2*rs-rs/2)]/1. - ptrborder3R[(i+hq+hf+h+(v+vf+vq)*rs+cs/2*rs-rs/2)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder3R[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs-rs/2)];
                    dG += pow((ptrborder3G[(i+hf+hq+(vf+vq)*rs+cs/2*rs-rs/2)]/1. - ptrborder3G[(i+hq+hf+h+(v+vf+vq)*rs+cs/2*rs-rs/2)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder3G[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs-rs/2)];
                    dB += pow((ptrborder3B[(i+hf+hq+(vf+vq)*rs+cs/2*rs-rs/2)]/1. - ptrborder3B[(i+hq+hf+h+(v+vf+vq)*rs+cs/2*rs-rs/2)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder3B[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs-rs/2)];
                    break;
                  case D4:
                    dR += pow((ptrborder1R[(i+hf+hq+(vf+vq)*rs+rs/2)]/1. - ptrborder1R[(i+hq+hf+h+(v+vf+vq)*rs+rs/2)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder1R[(i+h+hf+hq+(vf+v+vq)*rs+rs/2)];
                    dG += pow((ptrborder1G[(i+hf+hq+(vf+vq)*rs+rs/2)]/1. - ptrborder1G[(i+hq+hf+h+(v+vf+vq)*rs+rs/2)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder1G[(i+h+hf+hq+(vf+v+vq)*rs+rs/2)];
                    dB += pow((ptrborder1B[(i+hf+hq+(vf+vq)*rs+rs/2)]/1. - ptrborder1B[(i+hq+hf+h+(v+vf+vq)*rs+rs/2)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder1B[(i+h+hf+hq+(vf+v+vq)*rs+rs/2)];
                    break; 
                  case D5:
                    dR += pow((ptrimageR[(i+hf+hq+(vf+vq)*rs)]/1. - ptrimageR[(i+hf+h+hq+(v+vf+vq)*rs)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrimageR[(i+h+hf+hq+(vf+v+vq)*rs)];
                    dG += pow((ptrimageG[(i+hf+hq+(vf+vq)*rs)]/1. - ptrimageG[(i+hf+h+hq+(v+vf+vq)*rs)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrimageG[(i+h+hf+hq+(vf+v+vq)*rs)];
                    dB += pow((ptrimageB[(i+hf+hq+(vf+vq)*rs)]/1. - ptrimageB[(i+hf+h+hq+(v+vf+vq)*rs)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrimageB[(i+h+hf+hq+(vf+v+vq)*rs)];
                    break;
                  case D6:
                    dR += pow((ptrborder1R[(i+hf+hq+(vf+vq)*rs-rs/2)]/1. - ptrborder1R[(i+hf+hq+h+(v+vf+vq)*rs-rs/2)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder1R[(i+h+hf+hq+(vf+v+vq)*rs-rs/2)];
                    dG += pow((ptrborder1G[(i+hf+hq+(vf+vq)*rs-rs/2)]/1. - ptrborder1G[(i+hf+hq+h+(v+vf+vq)*rs-rs/2)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder1G[(i+h+hf+hq+(vf+v+vq)*rs-rs/2)];
                    dB += pow((ptrborder1B[(i+hf+hq+(vf+vq)*rs-rs/2)]/1. - ptrborder1B[(i+hf+hq+h+(v+vf+vq)*rs-rs/2)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder1B[(i+h+hf+hq+(vf+v+vq)*rs-rs/2)];
                    break;
                  case D7:
                    dR += pow((ptrborder1R[(i+hf+hq+(vf+vq)*rs+rs/2)]/1. - ptrborder1R[(i+hq+hf+h+(v+vf+vq)*rs+rs/2)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder1R[(i+h+hf+hq+(vf+v+vq)*rs+rs/2)];
                    dG += pow((ptrborder1G[(i+hf+hq+(vf+vq)*rs+rs/2)]/1. - ptrborder1G[(i+hq+hf+h+(v+vf+vq)*rs+rs/2)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder1G[(i+h+hf+hq+(vf+v+vq)*rs+rs/2)];
                    dG += pow((ptrborder1B[(i+hf+hq+(vf+vq)*rs+rs/2)]/1. - ptrborder1B[(i+hq+hf+h+(v+vf+vq)*rs+rs/2)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder1B[(i+h+hf+hq+(vf+v+vq)*rs+rs/2)];
                    break;
                  case D8:
                    dR += pow((ptrimageR[(i+hf+hq+(vf+vq)*rs)]/1. - ptrimageR[(i+hf+h+hq+(v+vf+vq)*rs)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrimageR[(i+h+hf+hq+(vf+v+vq)*rs)];
                    dG += pow((ptrimageG[(i+hf+hq+(vf+vq)*rs)]/1. - ptrimageG[(i+hf+h+hq+(v+vf+vq)*rs)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrimageG[(i+h+hf+hq+(vf+v+vq)*rs)];
                    dB += pow((ptrimageB[(i+hf+hq+(vf+vq)*rs)]/1. - ptrimageB[(i+hf+h+hq+(v+vf+vq)*rs)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrimageB[(i+h+hf+hq+(vf+v+vq)*rs)];
                    break;
                  case D9:
                    dR += pow((ptrborder1R[(i+hf+hq+(vf+vq)*rs-rs/2)]/1. - ptrborder1R[(i+hf+h+hq+(v+vf+vq)*rs-rs/2)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder1R[(i+h+hf+hq+(vf+v+vq)*rs-rs/2)];
                    dG += pow((ptrborder1G[(i+hf+hq+(vf+vq)*rs-rs/2)]/1. - ptrborder1G[(i+hf+h+hq+(v+vf+vq)*rs-rs/2)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder1G[(i+h+hf+hq+(vf+v+vq)*rs-rs/2)];
                    dB += pow((ptrborder1B[(i+hf+hq+(vf+vq)*rs-rs/2)]/1. - ptrborder1B[(i+hf+h+hq+(v+vf+vq)*rs-rs/2)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder1B[(i+h+hf+hq+(vf+v+vq)*rs-rs/2)];
                    break;
                  case D10:
                    dR += pow((ptrborder3R[(i+hf+hq+(vf+vq)*rs-cs/2*rs+rs/2)]/1. - ptrborder3R[(i+hf+hq+h+(v+vf+vq)*rs-cs/2*rs+rs/2)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder3R[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs+rs/2)];
                    dG += pow((ptrborder3G[(i+hf+hq+(vf+vq)*rs-cs/2*rs+rs/2)]/1. - ptrborder3G[(i+hf+hq+h+(v+vf+vq)*rs-cs/2*rs+rs/2)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder3G[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs+rs/2)];
                    dB+= pow((ptrborder3B[(i+hf+hq+(vf+vq)*rs-cs/2*rs+rs/2)]/1. - ptrborder3B[(i+hf+hq+h+(v+vf+vq)*rs-cs/2*rs+rs/2)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder3B[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs+rs/2)];
                    break;
                  case D11:
                    dR += pow((ptrborder2R[(i+hf+hq+(vf+vq)*rs-cs/2*rs)]/1. - ptrborder2R[(i+hf+h+hq+(v+vf+vq)*rs-cs/2*rs)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder2R[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs)];
                    dG += pow((ptrborder2G[(i+hf+hq+(vf+vq)*rs-cs/2*rs)]/1. - ptrborder2G[(i+hf+h+hq+(v+vf+vq)*rs-cs/2*rs)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder2G[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs)];
                    dB += pow((ptrborder2B[(i+hf+hq+(vf+vq)*rs-cs/2*rs)]/1. - ptrborder2B[(i+hf+h+hq+(v+vf+vq)*rs-cs/2*rs)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder2B[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs)];
                    break;
                  case D12:
                    dR += pow((ptrborder3R[(i+hf+hq+(vf+vq)*rs-cs/2*rs-rs/2)]/1. - ptrborder3R[(i+hf+h+hq+(v+vf+vq)*rs-cs/2*rs-rs/2)]/1.), 2.);
                    UQR += pow((2*f+1), -2.) * ptrborder3R[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs-rs/2)];
                    dG += pow((ptrborder3G[(i+hf+hq+(vf+vq)*rs-cs/2*rs-rs/2)]/1. - ptrborder3G[(i+hf+h+hq+(v+vf+vq)*rs-cs/2*rs-rs/2)]/1.), 2.);
                    UQG += pow((2*f+1), -2.) * ptrborder3G[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs-rs/2)];
                    dB += pow((ptrborder3B[(i+hf+hq+(vf+vq)*rs-cs/2*rs-rs/2)]/1. - ptrborder3B[(i+hf+h+hq+(v+vf+vq)*rs-cs/2*rs-rs/2)]/1.), 2.);
                    UQB += pow((2*f+1), -2.) * ptrborder3B[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs-rs/2)];
                    break;
                  default:  
                    printf("Somwthing went wrong!\n");
                    break;
                }


                //----------------------------------------------------------------------------------------------------
                //d += pow((ptrimage[(index+hf+hq+(vf+vq)*rs)]/1. - ptrimage[(index+hf+h+hq+(v+vf+hq)*rs)]/1.), 2.);
                //UQ += pow((2*f+1), -2.) * ptrimage[(index+h+hf+hq+(vf+v+vq)*rs)];
                //----------------------------------------------------------------------------------------------------

              }
            }
            d = (dR+dG+dB)/3;
            esp = (pow(2*f+1, -2) * d) - 2 * sigma * sigma;
            if (esp < 0) esp = 0;
            weight = exp(-esp/(hpar*hpar));
            CP += weight; 
            QR += UQR * weight;
            QG += UQG * weight;
            QB += UQB * weight;
          }
        }
        
        QestR[(hq+vq*(2*f+1)+(2*f+1)*(2*f+1)/2)] = QR/CP;
        QestG[(hq+vq*(2*f+1)+(2*f+1)*(2*f+1)/2)] = QG/CP;
        QestB[(hq+vq*(2*f+1)+(2*f+1)*(2*f+1)/2)] = QB/CP;

      }  
    }                


    upR = 0; 
    upG = 0; 
    upB = 0; 
    

    for (vf = -f; vf < f+1; vf++) {                                         // get color for pixel p, u(p) by averaging the estimates Q
      for (hf = -f; hf < f+1; hf++) {                
        upR += pow((2*f+1), -2) * QestR[(hf+vf*(2*f+1)+(2*f+1)*(2*f+1)/2)];
        upG += pow((2*f+1), -2) * QestG[(hf+vf*(2*f+1)+(2*f+1)*(2*f+1)/2)];
        upB += pow((2*f+1), -2) * QestB[(hf+vf*(2*f+1)+(2*f+1)*(2*f+1)/2)];
      }
    }  

    ptrimagetempR[i] = upR;
    ptrimagetempG[i] = upG;
    ptrimagetempB[i] = upB;
   
    
    d = (i*100)/N;
        //printf("Working...  %f done", d); 

  }  


    

  for (index = 0; index < N; index++) { 
    ptrimageR[index] = ptrimagetempR[index]; 
    ptrimageG[index] = ptrimagetempG[index]; 
    ptrimageB[index] = ptrimagetempB[index];
  }

  
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

