//
//  Gates.cpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 06/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#include "Gates.hpp"

cxd _H[4] = {cxd(1/sqrt(2)), cxd(1/sqrt(2)), cxd(1/sqrt(2)), cxd(-1/sqrt(2))};
cxd _X[4] = {0, 1, 1, 0};
cxd _Z[4] = {1, 0, 0, -1};

cxd _SWAP[16] = {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1};

Gate H() { return _H; }
Gate X() { return _X; }
Gate Z() { return _Z; }

Gate SWAP() { return _SWAP; }

Gate Rm(uint m) {
  cxd omega = exp(2*M_PI*cxd(0,1)/pow(2, m));
  
  cxd* R = new cxd[4] {1, 0, 0, omega};
  return R;
}

Gate QFT(uint n) {
  uint N = pow(2,n);
  cxd omega = exp(2*M_PI*cxd(0,1)/pow(2, n));
  
  cxd* g = new cxd[N*N];
  
  #pragma omp parallel for
  for(int i = 0; i < N; i++) {
    #pragma omp parallel for
    for(int j = 0; j < N; j++) {
      g[i*N+j] = pow(omega, i*j)/sqrt(N);
    }
  }
  
  return g;
}
