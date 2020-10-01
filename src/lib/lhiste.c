/* Compute Histogram Equalization */
/* Pietro Morri - september 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lhiste.h>

/* ==================================== */
uint32_t lhiste(struct xvimage * image){  	/* input: image to process */  
                                     		    /* output: modified image  */     
/* ==================================== */

  uint32_t indexpixel;
  uint8_t *ptrimage,  val;
  double  C[256], newval, R[256];
  uint32_t rs, cs, N, i;

  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  
  for (i = 0; i < 256; i++){      
    R[i] = 0.;
    C[i] = 0.;
  }

  ptrimage = UCHARDATA(image);
  
  for (indexpixel = 0; indexpixel < N; indexpixel++) {
    val = ptrimage[indexpixel];
    R[val] += 1.;
  }

  C[0] = (double)(R[0])/(double)(N);
  for (i = 1; i < 256; i++) 
    C[i] = C[i-1] + (double)(R[i])/(double)(N);
  
  
  for (indexpixel = 0; indexpixel < N; indexpixel++) {
    val = (ptrimage[indexpixel]);
    newval = 255*C[val];  
      
    ptrimage[indexpixel] = (uint8_t)newval;
  }

  return 1; /* Everythng went fine / tout s'est bien passe */
}





