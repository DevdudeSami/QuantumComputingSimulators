//
//  Gates.hpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef Gates_hpp
#define Gates_hpp

#include <stdio.h>
#include "SparseTensor.hpp"

SparseTensor I();
SparseTensor X();
SparseTensor Y();
SparseTensor Z();
SparseTensor P();
SparseTensor H();

SparseTensor CNOT();
SparseTensor SWAP();

SparseTensor TOFF();

#endif /* Gates_hpp */
