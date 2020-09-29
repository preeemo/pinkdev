/* Compute Histogram Normalization */
/* Pietro Morri - september 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lhistn.h>

/* ==================================== */
uint32_t lhistn(struct xvimage * image, /* input: image to process */  
                                           /* output: modified image  */  
                uint32_t low,		/* input: low threshold */
		uint32_t high           /* input: high threshold */     
             )
/* ==================================== */
{
  uint32_t indexpixel, R[255];
  uint8_t *ptrimage;
  uint32_t newval, val;
  uint32_t rs, cs, N;
  uint32_t counter = 0;

  rs = image->row_size;
  cs = image->col_size;  
  N = rs * cs;
  
   for (int i = 0; i < 255; i++){
     R[indexpixel] = 0;
   }
  
  
  ptrimage = UCHARDATA(image);
  for (indexpixel = 0; indexpixel < N; indexpixel++){
    val = (uint32_t)(ptrimage[indexpixel]);
    R[val]++;
    newval = val*(high-low)/255 + low;
    ptrimage[indexpixel] = (uint8_t)newval;
  }

  return 1; /* Everythng went fine / tout s'est bien passe */
}





