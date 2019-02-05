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

using namespace std;

typedef complex<double> cxd;

cxd H[4] = {cxd(1/sqrt(2)), cxd(1/sqrt(2)), cxd(1/sqrt(2)), cxd(-1/sqrt(2))};
cxd X[4] = {0, 1, 1, 0};

#endif /* Gates_hpp */
