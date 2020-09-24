/* Add a const to an image - threshold if overflow */
/* ajoute une constante a une image  - seuil si depassement */
/* Michel Couprie - janvier 1999 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <laddconst.h>

/* ==================================== */
uint32_t laddconst(struct xvimage * image, /* input: image to process */  
                                           /* output: modified image  */  
              uint32_t constante           /* input: value to add */     
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
    newval = (uint32_t)(ptrimage[indexpixel]) + constante;
    if (newval < NDG_MIN) newval = NDG_MIN;
    if (newval > NDG_MAX) newval = NDG_MAX;
    ptrimage[indexpixel] = (uint8_t)newval;
  }

  return 1; /* Everythng went fine / tout s'est bien passe */
}





