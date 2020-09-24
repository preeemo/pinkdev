/* 
le filtre lineaire recursif general de Deriche est defini par :

   f : le signal a filtrer
   g : le signal resultat
   g_, g+, r : resultats intermediaires
   alpha : parametre donnant la "taille" du filtre
   a1..8 : parametres calcules a partir de alpha, differents selon les applications
   b1..4 : parametres calcules a partir de alpha, differents selon les applications

   g_[m,n] = a1 f[m,n] + a2 f[m,n-1] + b1 g_[m,n-1] + b2 g_[m,n-2]
     f[m,-1] = 0
     g_[m,-2] = g_[m,-1] = 0
   g+[m,n] = a3 f[m,n+1] + a4 f[m,n+2] + b1 g+[m,n+1] + b2 g+[m,n+2]
     f[m,N] = f[m,N+1] = 0
     g+[m,N] = g+[m,N+1] = 0
   r[m,n] = g_[m,n] + g+[m,n]

   g_[m,n] = a5 r[m,n] + a6 r[m-1,n] + b3 g_[m-1,n] + b4 g_[m-2,n]
     r[-1,n] = 0
     g_[-2,n] = g_[-1,n] = 0
   g+[m,n] = a7 r[m+1,n] + a8 r[m+2,n] + b3 g+[m+1,n] + b4 g+[m+2,n]
     r[M,n] = r[M+1,n] = 0
     g+[M,n] = g+[M+1,n] = 0
   g[m,n] = g_[m,n] + g+[m,n]
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#define _INCLUDE_XOPEN_SOURCE
#include <math.h>

/* quelques macros utiles : */

#define abs(X) ((X)>=0?(X):-(X))
#define max(X,Y) ((X)>=(Y)?(X):(Y))
#define min(X,Y) ((X)<=(Y)?(X):(Y))

/* ==================================== */
void derichegen(double *f,               /* image a traiter */
                uint32_t M,                   /* taille ligne */
                uint32_t N,                   /* taille colonne */
                double *g1,              /* zone temporaire de taille max(M,N) */
                double *g2,              /* zone temporaire de taille max(M,N) */
                double *g,               /* stocke un resultat temporaire, puis le resultat final (taille M*N) */ 
                double a1, double a2, double a3, double a4, /* parametres */
                double a5, double a6, double a7, double a8, 
                double b1, double b2, double b3, double b4)
/* ==================================== */
{
  int32_t n, m;

  for (m = 0; m < M; m++)     /* filtrage vertical sur toutes les colonnes */
  {
    /* filtre causal vertical */
    g1[0] = a1 * f[m+M*0];
    g1[1] = a1 * f[m+M*1] + a2 * f[m+M*0] + b1 * g1[0];
    for (n = 2; n < N; n++)
      g1[n] = a1*f[m+M*n] + a2*f[m+M*(n-1)] + b1*g1[n-1] + b2*g1[n-2];

    /* filtre anticausal vertical */
    g2[N-1] = 0;
    g2[N-2] = a3 * f[m+M*(N-1)] + b1 * g2[N-1];
    for (n = N-3; n >= 0; n--)
      g2[n] = a3*f[m+M*(n+1)] + a4*f[m+M*(n+2)] + b1*g2[n+1] + b2*g2[n+2];

    for (n = 0; n < N; n++)
      g[m+M*n] = g1[n] + g2[n];
  }

  for (n = 0; n < N; n++)     /* filtrage horizontal sur toutes les lignes */
  {
    /* filtre causal horizontal */
    g1[0] = a5 * g[0+M*n];
    g1[1] = a5 * g[1+M*n] + a6 * g[0+M*n] + b3 * g1[0];
    for (m = 2; m < M; m++)
      g1[m] = a5 * g[m+M*n] + a6 * g[m-1+M*n] + b3 * g1[m-1] + b4 * g1[m-2];

    /* filtre anticausal horizontal */
    g2[M-1] = 0;
    g2[M-2] = a7 * g[M-1+M*n] + b3 * g2[M-1];
    for (m = M-3; m >= 0; m--)
      g2[m] = a7 * g[m+1+M*n] + a8 * g[m+2+M*n] + b3 * g2[m+1] + b4 * g2[m+2];

    for (m = 0; m < M; m++)
      g[m+M*n] = g1[m] + g2[m];
  }

} /* derichegen() */

/* ==================================== */
uint32_t lderiche(struct xvimage *image, double alpha)
/* ==================================== */
/*
    image : en entree, l'image a traiter ; en sortie, l'image traitee
    alpha : parametre (1/taille) du filtre
*/
{ 
  uint32_t i, j;
  uint8_t *ima = UCHARDATA(image);
  uint32_t rs = rowsize(image);
  uint32_t cs = colsize(image);
  uint32_t tailleimage = rs * cs;
  double *Im1;    /* image intermediaire */
  double *Im2;    /* image intermediaire */
  double *Imd;    /* image intermediaire */
  double *buf1;   /* buffer ligne ou colonne */
  double *buf2;   /* buffer ligne ou colonne */
  double k;       /* constante de normalisation pour le lisseur */
  double kp;      /* constante de normalisation pour le derivateur */
  double e_a;     /* stocke exp(-alpha) */
  double e_2a;    /* stocke exp(-2alpha) */
  double a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4;
  double t1, t2;
  double lmax, lmin;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lderiche: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  Im1 = (double *)malloc(tailleimage * sizeof(double));
  Im2 = (double *)malloc(tailleimage * sizeof(double));
  Imd = (double *)malloc(tailleimage * sizeof(double));
  buf1 = (double *)malloc(max(rs, cs) * sizeof(double));
  buf2 = (double *)malloc(max(rs, cs) * sizeof(double));
  if ((Im1==NULL) || (Im2==NULL) || (Imd==NULL) || (buf1==NULL) || (buf2==NULL))
  {   printf("lderiche() : malloc failed\n");
      return(0);
  }

  /* convertit les donnees d'entree en "double" et les stocke dans "Imd" */

  for (i = 0; i < tailleimage; i++) Imd[i] = (double)ima[i];










  /* VOTRE TRAITEMENT */











  /* ne pas oublier de convertir le resultat en "uint8_t" et de les stocker dans "ima" */


  free(Im1);
  free(Im2);
  free(Imd);
  free(buf1);
  free(buf2);

  return 1;
}

