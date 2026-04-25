//
//  GTermExpander.hpp
//  MeanMachine
//

#ifndef GTermExpander_hpp
#define GTermExpander_hpp

#include "GExpr.hpp"

struct GTermExpander {
    
    static GExpr                Expand(GSymbol pSymbol, bool pAllowMultiply);
    static GExpr                Expand(GExpr pExpr, bool pAllowMultiply);
};

#endif /* GTermExpander_hpp */
