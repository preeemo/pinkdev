/* Compute difference of two images - threshold if overflow */
/* Pietro Morri - september 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <imgdiff.h>

/* ==================================== */
uint32_t imgdiff(struct xvimage * image1, /* input: first image to process */  
                                          /* output: modified image  */  
              struct xvimage * image2     /* input: second image to process */     
             )
/* ==================================== */
{
  uint32_t indexpixel;
  uint8_t *ptrimage1, *ptrimage2;
  uint32_t newval;
  uint32_t rs1, cs1, rs2, cs2, N;

  rs1 = image1->row_size;
  cs1 = image1->col_size;  
  rs2 = image2->row_size;
  cs2 = image2->col_size;
  N = rs1 * cs1;
  if(rs2*cs2 != N){
    fprintf(stderr, "Dimension mismatch");
    exit(0);
  }
  /* ---------------------------------------------------------- */
  /* calculate the result */
  /* ---------------------------------------------------------- */

  ptrimage1 = UCHARDATA(image1);
  ptrimage2 = UCHARDATA(image2);
  for (indexpixel = 0; indexpixel < N; indexpixel++)  {
    newval = (uint32_t)(ptrimage1[indexpixel]) - (uint32_t)(ptrimage2[indexpixel]);
    if (newval < NDG_MIN) newval = NDG_MIN;
    if (newval > NDG_MAX) newval = NDG_MAX;
    ptrimage1[indexpixel] = (uint8_t)newval;
  }

  return 1; /* Everythng went fine */
}





