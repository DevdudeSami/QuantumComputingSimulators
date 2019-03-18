//
//  Gates.hpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#ifndef Gates_hpp
#define Gates_hpp

#include <stdio.h>
#include <complex>
#include "Typedefs.hpp"

using namespace std;

typedef complex<double> cxd;

Gate H();
Gate X();

Gate SWAP();

Gate Rm(uint m);

Gate QFT(uint n);

#endif /* Gates_hpp */
