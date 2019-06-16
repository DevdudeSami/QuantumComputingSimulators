//
//  Typedefs.hpp
//  NoMatrixSimulator
//
//  Created by Youssef Moawad on 05/02/2019.
//  Copyright © 2019 Youssef Moawad. All rights reserved.
//

#ifndef Typedefs_hpp
#define Typedefs_hpp

#include <stdio.h>

using namespace std;

typedef uint QID;
typedef uint list_index;
typedef complex<double> cxd;
typedef cxd* Gate;
typedef pair<Gate, vector<QID>> ApplicableGate;

#endif /* Typedefs_hpp */
