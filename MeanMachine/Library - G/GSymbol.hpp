//
//  GSymbol.hpp
//  Superfox
//

#ifndef GSymbol_hpp
#define GSymbol_hpp

#include <cstdint>
#include <string>
#include "TwistWorkSpace.hpp"

enum class GSymbolType : std::uint8_t {
    kInv = 0,
    kVar = 1,
    kBuf = 2
};

struct GSymbol {
    std::string                         mName;
    GSymbolType                         mType;
    TwistWorkSpaceSlot                  mSlot;
    
    GSymbol();
    static GSymbol                      Var(const std::string &pName);
    static GSymbol                      Buf(const TwistWorkSpaceSlot pSlot);

    void                                Set(const GSymbol &pOther);
    void                                Invalidate();
    bool                                IsInvalid() const;
    
    bool                                IsVar() const;
    bool                                IsBuf() const;
    
};

bool                                    operator == (const GSymbol &pSymbolLHS, const GSymbol &pSymbolRHS);
bool                                    operator != (const GSymbol &pSymbolLHS, const GSymbol &pSymbolRHS);


std::string                             BufName(TwistWorkSpaceSlot slot);
std::string                             BufAliasName(TwistWorkSpaceSlot slot);

GSymbol                                 VarSymbol(const std::string &pName);
GSymbol                                 BufSymbol(const TwistWorkSpaceSlot pSlot);

#endif
