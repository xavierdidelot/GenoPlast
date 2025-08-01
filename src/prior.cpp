#include "prior.h"
//
Prior::Prior() {
  rateparam=1.0;
  freqCP=1.0;
}

int Prior::drawNumCP() {
  double L=exp(-freqCP);
  int k=0;
  double p=1.0;
  while (p>=L) {
      k++;
      p*=rand()/(RAND_MAX+1.0);
    }
  return k-1;
}

//
