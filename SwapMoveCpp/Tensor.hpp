//
//  Tensor.hpp
//  MatrixBasedSimulator
//
//  Created by Youssef Moawad on 23/12/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#ifndef Tensor_hpp
#define Tensor_hpp

#include <stdio.h>

#include "SparseTensor.hpp"

class SparseTensor;
class DenseTensor;

class Tensor {
public:
  virtual uint rowCount() = 0;
  virtual uint colCount() = 0;
  
  virtual SparseTensor multiplyTo(SparseTensor t) = 0;
  
  virtual SparseTensor sparseKronWith(SparseTensor t) = 0;
  virtual SparseTensor sparseKronWith(DenseTensor t) = 0;
  virtual SparseTensor sparseKronWith(Tensor* t) = 0;
  virtual DenseTensor denseKronWith(SparseTensor t) = 0;
  virtual DenseTensor denseKronWith(DenseTensor t) = 0;
  virtual DenseTensor denseKronWith(Tensor *t) = 0;
  
  virtual uint getNNZ() = 0;
  
  virtual std::string toString() = 0;
};

#endif /* Tensor_hpp */
