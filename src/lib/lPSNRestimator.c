/* Compute the Peak Signal-to-Noise-Ratio estimate */
/* Pietro Morri - september 2020 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <mcimage.h>
#include <lPSNRestimator.h>

/* ==================================== */
double lPSNRestimator(struct xvimage * image1, /* input: first image to process */  
                                          /* output: modified image  */  
              struct xvimage * image2     /* input: second image to process */     
             )
/* ==================================== */
{
  uint32_t indexpixel;
  uint8_t *ptrimage1, *ptrimage2;
  double MSE=0, PSNR;
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
    MSE += pow(ptrimage1[indexpixel] - ptrimage2[indexpixel],2)/N;
    printf("%f\n",MSE);
  }
   PSNR = 10*log10((255*255)/MSE);
  printf("PSNR = %f dB\n", PSNR);
  return PSNR; /* Everythng went fine */
}





