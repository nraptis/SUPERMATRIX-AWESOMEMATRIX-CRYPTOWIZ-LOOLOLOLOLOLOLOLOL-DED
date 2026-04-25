//
//  GExpr.cpp
//  Moose Rib
//
//  Created by Whale Farts on 4/20/26.
//

#include "GExpr.hpp"

GOperType OperTypeForExprType(const GExprType pType) {
    switch (pType) {
        case GExprType::kRead: return GOperType::kRead;
        case GExprType::kAdd: return GOperType::kAdd;
        case GExprType::kSub: return GOperType::kSub;
        case GExprType::kMul: return GOperType::kMul;
        case GExprType::kXor: return GOperType::kXor;
        case GExprType::kAnd: return GOperType::kAnd;
        case GExprType::kRotL8: return GOperType::kRotL8;
        default: return GOperType::kInv;
    }
}

GOperType OperTypeForExpr(const GExpr pExpr) {
    return OperTypeForExprType(pExpr.mType);
}

namespace {

GExpr BinaryExpr(const GExprType pType,
                 const GExpr &pA,
                 const GExpr &pB) {
    GExpr aExpr;
    aExpr.mType = pType;
    aExpr.mA = std::make_shared<GExpr>(pA);
    aExpr.mB = std::make_shared<GExpr>(pB);
    return aExpr;
}

bool IsLoopIndexSymbol(const GSymbol &pSymbol) {
    if (!pSymbol.IsVar()) {
        return false;
    }
    return pSymbol.mName.find("LoopIndex") != std::string::npos;
}

bool HasReadWrap(const GExpr &pExpr) {
    return pExpr.mReadWrapType != GReadWrapType::kNone;
}

GExpr BuildWrappedRead(const GReadWrapType pWrapType,
                       const GSymbol &pSymbol,
                       const GSymbol &pIndex,
                       const GSymbol &pIndexOracle,
                       const int pOffset) {
    if (pSymbol.IsInvalid() || pIndex.IsInvalid()) {
        return GExpr();
    }

    if (!IsLoopIndexSymbol(pIndex) || pIndexOracle.IsInvalid()) {
        return GExpr::Read(pSymbol, GExpr::Symbol(pIndex));
    }

    GExpr aExpr = GExpr::Read(pSymbol, GExpr::Symbol(pIndexOracle));
    aExpr.mReadWrapType = pWrapType;
    aExpr.mReadWrapIndexSymbol = pIndex;
    aExpr.mReadWrapOracleSymbol = pIndexOracle;
    aExpr.mReadWrapOffset = pOffset;
    return aExpr;
}

void AppendSymbols(const GExpr &pExpr,
                   std::vector<GSymbol> *pSymbols) {
    if (pSymbols == NULL || pExpr.IsInvalid()) {
        return;
    }

    switch (pExpr.mType) {
        case GExprType::kSymbol:
            pSymbols->push_back(pExpr.mSymbol);
            return;

        case GExprType::kRead:
            pSymbols->push_back(pExpr.mSymbol);
            if (pExpr.mIndex != nullptr) {
                AppendSymbols(*pExpr.mIndex, pSymbols);
            }
            if (HasReadWrap(pExpr)) {
                pSymbols->push_back(pExpr.mReadWrapIndexSymbol);
                pSymbols->push_back(pExpr.mReadWrapOracleSymbol);
            }
            return;

        case GExprType::kAdd:
        case GExprType::kSub:
        case GExprType::kMul:
        case GExprType::kXor:
        case GExprType::kAnd:
        case GExprType::kRotL8:
            if (pExpr.mA != nullptr) {
                AppendSymbols(*pExpr.mA, pSymbols);
            }
            if (pExpr.mB != nullptr) {
                AppendSymbols(*pExpr.mB, pSymbols);
            }
            return;

        default:
            return;
    }
}

void AppendOps(const GExpr &pExpr,
               std::vector<GOperType> *pOps) {
    if (pOps == NULL || pExpr.IsInvalid()) {
        return;
    }

    switch (pExpr.mType) {
        case GExprType::kRead:
            if (pExpr.mIndex != nullptr) {
                AppendOps(*pExpr.mIndex, pOps);
            }
            pOps->push_back(GOperType::kRead);
            return;

        case GExprType::kAdd:
        case GExprType::kSub:
        case GExprType::kMul:
        case GExprType::kXor:
        case GExprType::kAnd:
        case GExprType::kRotL8:
            if (pExpr.mA != nullptr) {
                AppendOps(*pExpr.mA, pOps);
            }
            if (pExpr.mB != nullptr) {
                AppendOps(*pExpr.mB, pOps);
            }
            pOps->push_back(OperTypeForExprType(pExpr.mType));
            return;

        default:
            return;
    }
}

bool ExprPtrEqual(const std::shared_ptr<GExpr> &pLHS,
                  const std::shared_ptr<GExpr> &pRHS) {
    if (pLHS == nullptr || pRHS == nullptr) {
        return pLHS == pRHS;
    }
    return (*pLHS == *pRHS);
}

std::string ExprKeyInner(const GExpr &pExpr) {
    switch (pExpr.mType) {
        case GExprType::kInv:
            return "inv";

        case GExprType::kSymbol:
            if (pExpr.mSymbol.IsVar()) {
                return "sym:var:" + pExpr.mSymbol.mName;
            }
            if (pExpr.mSymbol.IsBuf()) {
                return "sym:buf:" + std::to_string(static_cast<int>(pExpr.mSymbol.mSlot));
            }
            return "sym:invalid";

        case GExprType::kConst:
            return "const:" + std::to_string(pExpr.mConstVal);

        case GExprType::kRead:
            return "read(" + ExprKeyInner(GExpr::Symbol(pExpr.mSymbol)) + "," +
                   ((pExpr.mIndex != nullptr) ? ExprKeyInner(*pExpr.mIndex) : "null") +
                   ",wrap=" + std::to_string(static_cast<int>(pExpr.mReadWrapType)) +
                   ",base=" + ExprKeyInner(GExpr::Symbol(pExpr.mReadWrapIndexSymbol)) +
                   ",oracle=" + ExprKeyInner(GExpr::Symbol(pExpr.mReadWrapOracleSymbol)) +
                   ",offset=" + std::to_string(pExpr.mReadWrapOffset) + ")";

        case GExprType::kAdd:
        case GExprType::kSub:
        case GExprType::kMul:
        case GExprType::kXor:
        case GExprType::kAnd:
        case GExprType::kRotL8:
            return std::to_string(static_cast<int>(pExpr.mType)) + "(" +
                   ((pExpr.mA != nullptr) ? ExprKeyInner(*pExpr.mA) : "null") + "," +
                   ((pExpr.mB != nullptr) ? ExprKeyInner(*pExpr.mB) : "null") + ")";
    }

    return "inv";
}

} // namespace

GExpr::GExpr() {
    Invalidate();
}

GExpr GExpr::Symbol(const GSymbol &pSymbol) {
    GExpr aExpr;
    aExpr.mType = GExprType::kSymbol;
    aExpr.mSymbol = pSymbol;
    return aExpr;
}

GExpr GExpr::Const(int pVal) {
    GExpr aExpr;
    aExpr.mType = GExprType::kConst;
    aExpr.mConstVal = pVal;
    return aExpr;
}

GExpr GExpr::Read(const GSymbol &pSymbol,
                  const GExpr &pIndex) {
    GExpr aExpr;
    aExpr.mType = GExprType::kRead;
    aExpr.mSymbol = pSymbol;
    aExpr.mIndex = std::make_shared<GExpr>(pIndex);
    aExpr.mReadWrapType = GReadWrapType::kNone;
    aExpr.mReadWrapIndexSymbol.Invalidate();
    aExpr.mReadWrapOracleSymbol.Invalidate();
    aExpr.mReadWrapOffset = 0;
    return aExpr;
}

GExpr GExpr::Add(const GExpr &a, const GExpr &b) {
    return BinaryExpr(GExprType::kAdd, a, b);
}

GExpr GExpr::Sub(const GExpr &a, const GExpr &b) {
    return BinaryExpr(GExprType::kSub, a, b);
}

GExpr GExpr::Mul(const GExpr &a, const GExpr &b) {
    return BinaryExpr(GExprType::kMul, a, b);
}

GExpr GExpr::Xor(const GExpr &a, const GExpr &b) {
    return BinaryExpr(GExprType::kXor, a, b);
}

GExpr GExpr::And(const GExpr &a, const GExpr &b) {
    return BinaryExpr(GExprType::kAnd, a, b);
}

GExpr GExpr::RotL8(const GExpr &a, const GExpr &b) {
    return BinaryExpr(GExprType::kRotL8, a, b);
}

GExpr GExpr::ReadBlockWrap(const GSymbol &pSymbol,
                           const GSymbol &pIndex,
                           const GSymbol &pIndexOracle,
                           int pOffset) {
    return BuildWrappedRead(GReadWrapType::kBlock, pSymbol, pIndex, pIndexOracle, pOffset);
}

GExpr GExpr::ReadSBoxWrap(const GSymbol &pSymbol,
                          const GSymbol &pIndex,
                          const GSymbol &pIndexOracle,
                          int pOffset) {
    return BuildWrappedRead(GReadWrapType::kSBox, pSymbol, pIndex, pIndexOracle, pOffset);
}

GExpr GExpr::ReadSaltWrap(const GSymbol &pSymbol,
                          const GSymbol &pIndex,
                          const GSymbol &pIndexOracle,
                          int pOffset) {
    return BuildWrappedRead(GReadWrapType::kSalt, pSymbol, pIndex, pIndexOracle, pOffset);
}

void GExpr::Set(const GExpr &pOther) {
    mType = pOther.mType;
    mSymbol = pOther.mSymbol;
    mConstVal = pOther.mConstVal;
    mIndex = pOther.mIndex;
    mA = pOther.mA;
    mB = pOther.mB;
    mReadWrapType = pOther.mReadWrapType;
    mReadWrapIndexSymbol = pOther.mReadWrapIndexSymbol;
    mReadWrapOracleSymbol = pOther.mReadWrapOracleSymbol;
    mReadWrapOffset = pOther.mReadWrapOffset;
}

void GExpr::Invalidate() {
    mType = GExprType::kInv;
    mSymbol.Invalidate();
    mConstVal = 0;
    mIndex.reset();
    mA.reset();
    mB.reset();
    mReadWrapType = GReadWrapType::kNone;
    mReadWrapIndexSymbol.Invalidate();
    mReadWrapOracleSymbol.Invalidate();
    mReadWrapOffset = 0;
}

bool GExpr::IsInvalid() const {
    switch (mType) {
        case GExprType::kSymbol:
            return mSymbol.IsInvalid();

        case GExprType::kConst:
            return false;

        case GExprType::kRead:
            return mSymbol.IsInvalid() || (mIndex == nullptr) || mIndex->IsInvalid() ||
                   ((mReadWrapType != GReadWrapType::kNone) &&
                    (mReadWrapIndexSymbol.IsInvalid() || mReadWrapOracleSymbol.IsInvalid()));

        case GExprType::kAdd:
        case GExprType::kSub:
        case GExprType::kMul:
        case GExprType::kXor:
        case GExprType::kAnd:
        case GExprType::kRotL8:
            return (mA == nullptr) || (mB == nullptr) ||
                   mA->IsInvalid() || mB->IsInvalid();

        default:
            return true;
    }
}

bool GExpr::IsSymbol() const {
    return mType == GExprType::kSymbol;
}

bool GExpr::IsConst() const {
    return mType == GExprType::kConst;
}

bool GExpr::IsRead() const {
    return mType == GExprType::kRead;
}

std::vector<GSymbol> GExpr::GetSymbols() const {
    std::vector<GSymbol> aSymbols;
    AppendSymbols(*this, &aSymbols);
    return aSymbols;
}

std::vector<GOperType> GExpr::GetOps() const {
    std::vector<GOperType> aOps;
    AppendOps(*this, &aOps);
    return aOps;
}

bool operator == (const GExpr &pLHS, const GExpr &pRHS) {
    if (pLHS.mType != pRHS.mType) {
        return false;
    }

    switch (pLHS.mType) {
        case GExprType::kInv:
            return true;

        case GExprType::kSymbol:
            return pLHS.mSymbol == pRHS.mSymbol;

        case GExprType::kConst:
            return pLHS.mConstVal == pRHS.mConstVal;

        case GExprType::kRead:
            return (pLHS.mSymbol == pRHS.mSymbol) &&
                   ExprPtrEqual(pLHS.mIndex, pRHS.mIndex) &&
                   (pLHS.mReadWrapType == pRHS.mReadWrapType) &&
                   (pLHS.mReadWrapOffset == pRHS.mReadWrapOffset) &&
                   (pLHS.mReadWrapIndexSymbol == pRHS.mReadWrapIndexSymbol) &&
                   (pLHS.mReadWrapOracleSymbol == pRHS.mReadWrapOracleSymbol);

        case GExprType::kAdd:
        case GExprType::kSub:
        case GExprType::kMul:
        case GExprType::kXor:
        case GExprType::kAnd:
        case GExprType::kRotL8:
            return ExprPtrEqual(pLHS.mA, pRHS.mA) &&
                   ExprPtrEqual(pLHS.mB, pRHS.mB);
    }

    return false;
}

bool operator != (const GExpr &pLHS, const GExpr &pRHS) {
    return !(pLHS == pRHS);
}

std::string GExprKey(const GExpr &pExpr) {
    return ExprKeyInner(pExpr);
}
