/* Call to lhiste */

#include <stdint.h>
#include <stdio.h>
#include <mcimage.h>
#include <lhiste.h>
#include <stdlib.h>

/* =============================================================== */
int main(int argc, char **argv){
/* =============================================================== */
  struct xvimage * R, * G, * B;
	
  if (argc != 3) {
    fprintf(stderr, "usage: %s image.ppm, output image\n", argv[0]);
    exit(0);
  }
  
  if (readrgbimage(argv[1], &R ,&G, &B) == 0) {
    fprintf(stderr, "histe: readimage failed\n");
    exit(0);
  }
  
  if (! lhiste(R)) {
    fprintf(stderr, "histe: function histe failed\n");
    exit(0);
  }

  if (! lhiste(G)) {
    fprintf(stderr, "histe: function histe failed\n");
    exit(0);
  }

  if (! lhiste(B)) {
    fprintf(stderr, "histe: function histe failed\n");
    exit(0);
  }
  
  writergbimage(R, G, B, argv[argc-1]);
  freeimage(R);
  freeimage(G);
  freeimage(B);

  return 0;
} /* main */


