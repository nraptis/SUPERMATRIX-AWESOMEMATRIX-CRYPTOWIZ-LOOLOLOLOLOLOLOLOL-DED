//
//  GQuick.cpp
//  MeanMachine
//
//  Created by John Snow on 4/24/26.
//

#include "GQuick.hpp"
#include "Random.hpp"

GExpr GQuick::BufferRead(GSymbol pSymbol, GSymbol pSlotSymbol) {
    GExpr aIndexExpression = GExpr::Symbol(pSlotSymbol);
    return GExpr::Read(pSymbol, aIndexExpression);
}

GExpr GQuick::BufferReadRandomOffset(GSymbol pSymbol,
                                     GSymbol pSlotSymbol,
                                     GSymbol pOracleSymbol) {
    int aBufferLength = TwistWorkSpace::GetBuferLength(pSymbol.mSlot);
    if (aBufferLength <= 0) {
        return BufferRead(pSymbol, pSlotSymbol);
    }
    int aOffset = (int)Random::Get(aBufferLength);

    if (!pOracleSymbol.IsInvalid() && (pSymbol.mSlot != TwistWorkSpaceSlot::kInvalid)) {
        return GExpr::ReadBlockWrap(pSymbol, pSlotSymbol, pOracleSymbol, aOffset);
    }

    GExpr aIndexExpression = GExpr::Symbol(pSlotSymbol);
    if (aOffset != 0) {
        aIndexExpression = GExpr::Add(aIndexExpression, GExpr::Const(aOffset));
    }
    return GExpr::Read(pSymbol, aIndexExpression);
}

GExpr GQuick::BufferReadRandomOffset(GSymbol pSymbol, GSymbol pSlotSymbol) {
    return BufferReadRandomOffset(pSymbol, pSlotSymbol, GSymbol());
}
