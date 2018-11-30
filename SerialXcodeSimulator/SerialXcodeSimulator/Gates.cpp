//
//  Gates.cpp
//  SerialXcodeSimulator
//
//  Created by Youssef Moawad on 09/11/2018.
//  Copyright © 2018 Youssef Moawad. All rights reserved.
//

#include "Gates.hpp"

/************* Q Gates ***************/
Tensor I() {
  Tensor t (2,2);
  t.setElementAt(0,0,1);
  t.setElementAt(1,1,1);
  return t;
}

Tensor H() {
  Tensor t (2,2);
  t.setElementAt(0,0,1);
  t.setElementAt(0,1,1);
  t.setElementAt(1,0,1);
  t.setElementAt(1,1,-1);
  t = t.multiplyBy(1/sqrt(2));
  return t;
}

Tensor SWAP() {
  Row row1 = {1,0,0,0};
  Row row2 = {0,0,1,0};
  Row row3 = {0,1,0,0};
  Row row4 = {0,0,0,1};
  
  return Tensor({row1, row2, row3, row4});
}
/************* END Q Gates ***************/
