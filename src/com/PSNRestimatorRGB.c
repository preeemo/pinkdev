/* Call to lPSNRestimator */

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <mcimage.h>
#include <lPSNRestimator.h>
#include <stdlib.h>
#include <errno.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * imageR1, * imageG1, * imageB1;
  struct xvimage * imageR2, * imageG2, * imageB2;
  double PSNR, MSER, MSEG, MSEB, MSE;
  FILE *fp;
  char sentence[255];


  if (argc != 3) {
    fprintf(stderr, "usage: %s in1.pgm, in2.pmm \n", argv[0]);
    exit(0);
  }


  if (readrgbimage(argv[1], &imageR1, &imageG1, &imageB1) == 0) {
    fprintf(stderr, "PSNRestimatorRGB: readrgbimage failed for image #1\n");
    exit(1);
  }
  
  if (readrgbimage(argv[2], &imageR2, &imageG2, &imageB2) == 0) {
    fprintf(stderr, "PSNRestimatorRGB: readrgbimage failed for image #2\n");
    exit(1);
  }

  MSER = lPSNRestimator(imageR1, imageR2); 
  MSEG = lPSNRestimator(imageG1, imageG2); 
  MSEB = lPSNRestimator(imageB1, imageB2); 
  
  if (! MSER) {
    fprintf(stderr, "PSNRestimator: function PSNRestimator failed on red channel\n");
    exit(0);
  }
  
  if (! MSEG) {
    fprintf(stderr, "PSNRestimator: function PSNRestimator failed on green channel\n");
    exit(0);
  }
  
  if (! MSEB) {
    fprintf(stderr, "PSNRestimator: function PSNRestimator failed on blue channel\n");
    exit(0);
  }
  
  MSE = (MSER + MSEB + MSEG)/3;
  PSNR = 10*log10((255*255)/MSE);


  fp = fopen("SNR_estimates.txt", "w+");
  
  if (fp == NULL) {
    printf("PSNRestimator: can't write output file\n");
    printf("PSNR = %f dB\n", PSNR);
    printf("Error %d \n", errno);
    printf("It's null");
    exit(1);             
  }

  sprintf(sentence, "---------------// PSNR Estimator Function //---------------\n\n\nInputs:\n\t\t%s;\n\t\t%s;\n\n\nOutputs:\n\t\tMSE = %.2f\n\t\tPSNR = %.2fdB", argv[1], argv[2], MSE, PSNR);
  fprintf(fp,"%s", sentence);
  fclose(fp);
  
  printf("PSNR = %f dB\n", PSNR);

  freeimage(imageR1);
  freeimage(imageG1);
  freeimage(imageB1);
  freeimage(imageR2);
  freeimage(imageG2);
  freeimage(imageB2);

  return 0;
} /* main */


