/* Compute Histogram Equalization */
/* Pietro Morri - september 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lhiste.h>

/* ==================================== */
uint32_t lhiste(struct xvimage * image  	/* input: image to process */  
                                     		/* output: modified image  */       
             )
/* ==================================== */
{
	
	
  uint32_t indexpixel;
  uint8_t *ptrimage, R[255],i ;
  float  C[255], Rt[255];
  uint32_t newval, val;
  uint32_t rs, cs, N;

  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  
   for (i = 0; i < 255; i++){
     R[i] = 0;
     Rt[i] = 0;
     C[i] = 0;
   }
  ptrimage = UCHARDATA(image);
  for (indexpixel = 0; indexpixel < N; indexpixel++) {
    //val = (uint32_t)(ptrimage[indexpixel]);
    val = (uint8_t)(ptrimage[indexpixel]);
    R[val]++;
    //printf("%d val \n%d R[val]\n", val, R[val]);
  }

  C[0] = (float)R[0]/N;
  printf("%f\n%d\n", C[0], R[0]);
  for (i = 1; i < 255; i++) {
    Rt[i] = (float)R[i]/N;
    C[i] = C[i-1] + Rt[i];
  }
  
  //  printf("ok"); exit(0);
  for (indexpixel = 0; indexpixel < N; indexpixel++) {
    val = (uint8_t)(ptrimage[indexpixel]);
       newval = 255*C[val];  
    
    
    //printf("%d\n%d\n", newval,(uint8_t)newval);
    ptrimage[indexpixel] = (uint8_t)newval;
  }

  return 1; /* Everythng went fine / tout s'est bien passe */
}





