/* Call to wavg */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lwavg.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * image1;
  uint32_t kernel = 0;
  

  if (argc != 4)  {

    fprintf(stderr, "usage %s:\nin1.pgm,\nk: kernel type\n\t'0': unitary kernel (simple average);\n\t'1': gaussian kernel;\n\t'2': vertical edge detection\n\t'3': horizontal edge detection\n\t'4': edge detection\n\t'5': sharpening\n\t'6': input kernel\noutput image\n", argv[0]);
    exit(0);
  }

  image1 = readimage(argv[1]);  

  if (image1 == NULL)  {
    fprintf(stderr, "wavg: readimage failed\n");
    exit(0);
  }

  kernel = atoi(argv[2]);

  if (kernel > 5){
    fprintf(stderr, "wavg: wrong kernel type \n");
    exit(0);
  }

  if (! lwavg(image1, kernel))  {
    fprintf(stderr, "wavg: function wavg failed\n");
    exit(0);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */


