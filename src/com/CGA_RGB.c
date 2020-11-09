/* Call to CGA_RGB*/

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lCGA_RGB.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */

  struct xvimage * imageR, * imageG, * imageB;
  

  if (argc != 3)  {
    fprintf(stderr, "usage: %s in1.pgm,  output image\n", argv[0]);
    exit(0);
  }

  if (readrgbimage(argv[1], &imageR, &imageG, &imageB) == 0)
  {
    fprintf(stderr, "CGA_RGB: readrgbimage failed\n");
    exit(1);
  }

  if (! lCGA_RGB(imageR, imageG, imageB))  {
    fprintf(stderr, "CGA_RGB: function CGA failed\n");
    exit(0);
  }


  writergbimage(imageR, imageG, imageB, argv[argc-1]);
  freeimage(imageR);
  freeimage(imageG);
  freeimage(imageB);



  return 0;
} /* main */

