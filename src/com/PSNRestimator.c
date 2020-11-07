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

  struct xvimage * image1;
  struct xvimage * image2;
  double PSNR, MSE;
  FILE *fp;
  char sentence[255];


  if (argc != 3) {
    fprintf(stderr, "usage: %s in1.pgm, in2.pmg, out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);

  if (image1 == NULL) {
    fprintf(stderr, "PSNRestimator: readimage failed\n");
    exit(1);
  }

  image2 = readimage(argv[2]); 

  if (image2 == NULL) {
    fprintf(stderr, "PSNRestimator: readimage failed\n");
    exit(1);
  }

  MSE = lPSNRestimator(image1, image2); 
  PSNR = 10*log10((255*255)/MSE);
  
  if (!MSE) {
    fprintf(stderr, "PSNRestimator: function PSNRestimator failed\n");
    exit(1);
  }

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


  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */


