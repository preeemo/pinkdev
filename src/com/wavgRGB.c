/* Call to wavg */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lwavg.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * imageR, * imageG, * imageB;
  uint32_t kernel = 0;
  
  if (argc != 4) {
    fprintf(stderr, "usage %s:\nin1.pgm,\nk: kernel type\n\t'0': unitary kernel (simple average);\n\t'1': gaussian kernel;\n\t'2': vertical edge detection\n\t'3': horizontal edge detection\n\t'4': laplacian kernel\n\t'5': input kernel\noutput image\n", argv[0]);
    exit(0);
  }

  if (readrgbimage(argv[1], &imageR, &imageG, &imageB) == 0) {
    fprintf(stderr, "wavgRGB: readrgbimage failed\n");
    exit(1);
  }

  kernel = atoi(argv[2]);

  if (kernel > 5){
    fprintf(stderr, "wavg: wrong kernel type \n");
    exit(0);
  }

  if (! lwavg(imageR, kernel)) {
    fprintf(stderr, "wavgRGB: function wavgRGB failed\n");
    exit(0);
  }

  if (! lwavg(imageG, kernel)) {
    fprintf(stderr, "wavgRGB: function wavgRGB failed\n");
    exit(0);
  }
  
  if (! lwavg(imageB, kernel)) {
    fprintf(stderr, "wavgRGB: function wavgRGB failed\n");
    exit(0);
  }


  writergbimage(imageR, imageG, imageB, argv[argc-1]);
  freeimage(imageR);
  freeimage(imageG);
  freeimage(imageB);


  return 0;
} /* main */


