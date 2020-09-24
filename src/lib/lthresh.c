/* Compute thresholding  */
/* Pietro Morri - september 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lthresh.h>

/* ==================================== */
uint32_t lthresh(struct xvimage * image, /* input: image to process */  
                                           /* output: modified image  */  
                uint32_t low,		/* input: low threshold */
		uint32_t high           /* input: high threshold */     
             )
/* ==================================== */
{
  uint32_t indexpixel;
  uint8_t *ptrimage;
  uint32_t newval;
  uint32_t rs, cs, N;

  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  
  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  ptrimage = UCHARDATA(image);
  for (indexpixel = 0; indexpixel < N; indexpixel++)
  {
    newval = (uint32_t)(ptrimage[indexpixel]);
    if (newval < low) newval = 0;
    if (newval > high) newval = 255;
    ptrimage[indexpixel] = (uint8_t)newval;
  }

  return 1; /* Everythng went fine / tout s'est bien passe */
}





