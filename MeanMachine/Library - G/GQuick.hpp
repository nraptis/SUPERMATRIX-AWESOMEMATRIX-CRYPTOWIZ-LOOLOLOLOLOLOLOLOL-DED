//
//  GQuick.hpp
//  MeanMachine
//
//  Created by John Snow on 4/24/26.
//

#ifndef GQuick_hpp
#define GQuick_hpp

#include "TwistWorkSpace.hpp"
#include "TwistFunctional.hpp"
#include "GSymbol.hpp"
#include "GExpr.hpp"
#include "GSeedProgram.hpp"
#include <string>
#include <vector>

class GQuick {

public:

    static GExpr                BufferRead(GSymbol pSymbol, GSymbol pSlotSymbol);
    static GExpr                BufferReadRandomOffset(GSymbol pSymbol,
                                                       GSymbol pSlotSymbol,
                                                       GSymbol pOracleSymbol);
    static GExpr                BufferReadRandomOffset(GSymbol pSymbol, GSymbol pSlotSymbol);
    
    
};

#endif /* GQuick_hpp */
