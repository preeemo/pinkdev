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
  

  uint8_t r, f, sigma = 12.5;
  double d, esp, hpar, weight, CP, up, Qest[(2*f+1)*(2*f+1)], UQ, Q, tmp;  
  int  h, v, hf, vf, hq, vq;

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


  //----------------------------------Choose parameters---------------------------------------------
  if(sigma <= 15){
    r = 10;
    f = 1;
    hpar = 0.4*sigma;
  } 
  else if (sigma > 15 && sigma <= 30){
    r = 10;
    f = 2;
    hpar = 0.4*sigma;
  }
  else if (sigma > 30 && sigma <= 45){
    r = 17;
    f = 3;
    hpar = 0.35*sigma;
  }
  else if (sigma > 45 && sigma <= 75){
    r = 17;
    f = 4;
    hpar = 0.35*sigma;
  }
  else {
    r = 17;
    f = 5;
    hpar = 0.3*sigma;
  }

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

  //----------------------------------Compute Graph Averaging---------------------------------------

  for (i = 0; i < N; i++) {
    /*if (index <= rs*(r + 2*f)|| i >= N-rs*(r-1+2*f))
      ptrimagetemp[i] =  ptrimage[i];
    
    
    else if (i % rs  <= r-1+2*f || i % rs >= rs-1-r-2*f)
      ptrimagetemp[i] =  ptrimage[i];

    else {*/
    
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
        Q = 0;
        CP = 0;

        //calculate w(B,Q)
        for (v = -r; v < r+1; v++) {           //this couple of for loops search trhough B(r); in pixel q
          for (h = -r; h < r+1; h++) {
            
            d = 0;
            UQ = 0;

            for (vf = -f; vf < f+1; vf++) {         
              for (hf = -f; hf < f+1; hf++) {
                switch (label) {                //compute distance in a safe zone
              
                  case D1: 
                    d += pow((ptrborder3[(i+hf+hq+(vf+vq)*rs+cs/2*rs+rs/2)]/1. - ptrborder3[(i+hf+hq+h+(v+vf+vq)*rs+cs/2*rs+rs/2)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder3[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs+rs/2)];
                    break;
                  case D2: 
                    d += pow((ptrborder2[(i+hf+hq+(vf+vq)*rs+cs/2*rs)]/1. - ptrborder2[(i+hf+h+hq+(v+vf+vq)*rs+cs/2*rs)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder2[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs)];
                    break;
                  case D3:
                    d += pow((ptrborder3[(i+hf+hq+(vf+vq)*rs+cs/2*rs-rs/2)]/1. - ptrborder3[(i+hq+hf+h+(v+vf+vq)*rs+cs/2*rs-rs/2)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder3[(i+h+hf+hq+(vf+v+vq)*rs+cs/2*rs-rs/2)];
                    break;
                  case D4:
                    d += pow((ptrborder1[(i+hf+hq+(vf+vq)*rs+rs/2)]/1. - ptrborder1[(i+hq+hf+h+(v+vf+vq)*rs+rs/2)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder1[(i+h+hf+hq+(vf+v+vq)*rs+rs/2)];
                    break; 
                  case D5:
                    d += pow((ptrimage[(i+hf+hq+(vf+vq)*rs)]/1. - ptrimage[(i+hf+h+hq+(v+vf+vq)*rs)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrimage[(i+h+hf+hq+(vf+v+vq)*rs)];
                    break;
                  case D6:
                    d += pow((ptrborder1[(i+hf+hq+(vf+vq)*rs-rs/2)]/1. - ptrborder1[(i+hf+hq+h+(v+vf+vq)*rs-rs/2)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder1[(i+h+hf+hq+(vf+v+vq)*rs-rs/2)];
                    break;
                  case D7:
                    d += pow((ptrborder1[(i+hf+hq+(vf+vq)*rs+rs/2)]/1. - ptrborder1[(i+hq+hf+h+(v+vf+vq)*rs+rs/2)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder1[(i+h+hf+hq+(vf+v+vq)*rs+rs/2)];
                    break;
                  case D8:
                    d += pow((ptrimage[(i+hf+hq+(vf+vq)*rs)]/1. - ptrimage[(i+hf+h+hq+(v+vf+vq)*rs)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrimage[(i+h+hf+hq+(vf+v+vq)*rs)];
                    break;
                  case D9:
                    d += pow((ptrborder1[(i+hf+hq+(vf+vq)*rs-rs/2)]/1. - ptrborder1[(i+hf+h+hq+(v+vf+vq)*rs-rs/2)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder1[(i+h+hf+hq+(vf+v+vq)*rs-rs/2)];
                    break;
                  case D10:
                    d += pow((ptrborder3[(i+hf+hq+(vf+vq)*rs-cs/2*rs+rs/2)]/1. - ptrborder3[(i+hf+hq+h+(v+vf+vq)*rs-cs/2*rs+rs/2)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder3[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs+rs/2)];
                    break;
                  case D11:
                    d += pow((ptrborder2[(i+hf+hq+(vf+vq)*rs-cs/2*rs)]/1. - ptrborder2[(i+hf+h+hq+(v+vf+vq)*rs-cs/2*rs)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder2[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs)];
                    break;
                  case D12:
                    d += pow((ptrborder3[(i+hf+hq+(vf+vq)*rs-cs/2*rs-rs/2)]/1. - ptrborder3[(i+hf+h+hq+(v+vf+vq)*rs-cs/2*rs-rs/2)]/1.), 2.);
                    UQ += pow((2*f+1), -2.) * ptrborder3[(i+h+hf+hq+(vf+v+vq)*rs-cs/2*rs-rs/2)];
                    break;
                  default:  
                    printf("Somwthing went wrong!\n");
                    break;
                }


                //-----------------------------------------------------------------------------------
                //d += pow((ptrimage[(index+hf+hq+(vf+vq)*rs)]/1. - ptrimage[(index+hf+h+hq+(v+vf+hq)*rs)]/1.), 2.);
                //UQ += pow((2*f+1), -2.) * ptrimage[(index+h+hf+hq+(vf+v+vq)*rs)];
                //------------------------------------------------------------------------------------------
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



   // }  

    up = 0; 


    for (vf = -f; vf < f+1; vf++) {                                         // get color for pixel p, u(p) by averaging the estimates Q
      for (hf = -f; hf < f+1; hf++)                   
        up += pow((2*f+1), -2) * Qest[(hf+vf*(2*f+1)+(2*f+1)*(2*f+1)/2)];
    }  

    ptrimagetemp[i] = up;
   
    
    //d = (i*100)/N;
    //printf("Working...  %f done\n", d); 

  }  


    

  for (index = 0; index < N; index++)  
    ptrimage[index] = ptrimagetemp[index];


  return 1;

}

