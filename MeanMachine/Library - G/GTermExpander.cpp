//
//  GTermExpander.cpp
//  MeanMachine
//

#include "GTermExpander.hpp"
#include "Random.hpp"

#include <array>
#include <cstdint>

namespace {

enum class GTermPattern : std::uint8_t {
    kAdd = 0,
    kSub = 1,
    kXor = 2,
    kMul = 3,
    kRotate = 4,
    kMulThenAdd = 5,
    kMulThenSub = 6,
    kMulThenXor = 7,
    kRotateAdd = 8,
    kRotateSub = 9,
    kRotateXor = 10,
    kRotateMul = 11,
    kPostRotateAdd = 12,
    kPostRotateSub = 13,
    kPostRotateXor = 14,
    kPostRotateMul = 15
};

using RotateBuilder = GExpr (*)(const GExpr &, const GExpr &);

struct GTermExpandConfig {
    RotateBuilder                   mRotateLeft = nullptr;
    std::uint8_t                    (*mNextScalar)() = nullptr;
    std::uint8_t                    (*mNextOddScalar)() = nullptr;
    std::uint8_t                    (*mNextRotate)() = nullptr;
};

std::size_t NextPatternIndex(const std::size_t pMaximumInclusive) {
    return static_cast<std::size_t>(Random::Get(static_cast<int>(pMaximumInclusive + 1U)));
}

std::uint8_t NextByteInRange(const std::uint8_t pMinimum,
                             const std::uint8_t pMaximum) {
    if (pMinimum >= pMaximum) {
        return pMinimum;
    }
    return static_cast<std::uint8_t>(Random::Get(static_cast<int>(pMinimum),
                                                 static_cast<int>(pMaximum)));
}

constexpr std::array<GTermPattern, 16> kAllPatterns = {
    GTermPattern::kAdd,
    GTermPattern::kSub,
    GTermPattern::kXor,
    GTermPattern::kMul,
    GTermPattern::kRotate,
    GTermPattern::kMulThenAdd,
    GTermPattern::kMulThenSub,
    GTermPattern::kMulThenXor,
    GTermPattern::kRotateAdd,
    GTermPattern::kRotateSub,
    GTermPattern::kRotateXor,
    GTermPattern::kRotateMul,
    GTermPattern::kPostRotateAdd,
    GTermPattern::kPostRotateSub,
    GTermPattern::kPostRotateXor,
    GTermPattern::kPostRotateMul,
};

constexpr std::array<GTermPattern, 10> kNoMultiplyPatterns = {
    GTermPattern::kAdd,
    GTermPattern::kSub,
    GTermPattern::kXor,
    GTermPattern::kRotate,
    GTermPattern::kRotateAdd,
    GTermPattern::kRotateSub,
    GTermPattern::kRotateXor,
    GTermPattern::kPostRotateAdd,
    GTermPattern::kPostRotateSub,
    GTermPattern::kPostRotateXor,
};

std::uint8_t NextScalar8() {
    return NextByteInRange(1U, 255U);
}

std::uint8_t NextOddScalar8() {
    return static_cast<std::uint8_t>((NextByteInRange(0U, 126U) * 2U) + 3U);
}

std::uint8_t NextRotate8() {
    return NextByteInRange(1U, 7U);
}

GExpr ExpandWithConfig(const GExpr &pBase,
                       const bool pAllowMultiply,
                       const GTermExpandConfig &pConfig) {
    if (pBase.IsInvalid() || (pConfig.mRotateLeft == nullptr) ||
        (pConfig.mNextScalar == nullptr) ||
        (pConfig.mNextOddScalar == nullptr) ||
        (pConfig.mNextRotate == nullptr)) {
        return GExpr();
    }

    const GExpr &aBase = pBase;
    const GExpr aScalar1 = GExpr::Const(pConfig.mNextScalar());
    const GExpr aScalar2 = GExpr::Const(pConfig.mNextScalar());
    const GExpr aOddScalar = GExpr::Const(pConfig.mNextOddScalar());
    const GExpr aRotate1 = GExpr::Const(pConfig.mNextRotate());
    const GExpr aRotate2 = GExpr::Const(pConfig.mNextRotate());

    const GTermPattern aPattern = pAllowMultiply
        ? kAllPatterns[NextPatternIndex(kAllPatterns.size() - 1U)]
        : kNoMultiplyPatterns[NextPatternIndex(kNoMultiplyPatterns.size() - 1U)];

    switch (aPattern) {
        case GTermPattern::kAdd:
            return GExpr::Add(aBase, aScalar1);
        case GTermPattern::kSub:
            return GExpr::Sub(aBase, aScalar1);
        case GTermPattern::kXor:
            return GExpr::Xor(aBase, aScalar1);
        case GTermPattern::kMul:
            return GExpr::Mul(aBase, aOddScalar);
        case GTermPattern::kRotate:
            return pConfig.mRotateLeft(aBase, aRotate1);
        case GTermPattern::kMulThenAdd:
            return GExpr::Add(GExpr::Mul(aBase, aOddScalar), aScalar2);
        case GTermPattern::kMulThenSub:
            return GExpr::Sub(GExpr::Mul(aBase, aOddScalar), aScalar2);
        case GTermPattern::kMulThenXor:
            return GExpr::Xor(GExpr::Mul(aBase, aOddScalar), aScalar2);
        case GTermPattern::kRotateAdd:
            return pConfig.mRotateLeft(GExpr::Add(aBase, aScalar1), aRotate2);
        case GTermPattern::kRotateSub:
            return pConfig.mRotateLeft(GExpr::Sub(aBase, aScalar1), aRotate2);
        case GTermPattern::kRotateXor:
            return pConfig.mRotateLeft(GExpr::Xor(aBase, aScalar1), aRotate2);
        case GTermPattern::kRotateMul:
            return pConfig.mRotateLeft(GExpr::Mul(aBase, aOddScalar), aRotate2);
        case GTermPattern::kPostRotateAdd:
            return GExpr::Add(pConfig.mRotateLeft(aBase, aRotate2), aScalar1);
        case GTermPattern::kPostRotateSub:
            return GExpr::Sub(pConfig.mRotateLeft(aBase, aRotate2), aScalar1);
        case GTermPattern::kPostRotateXor:
            return GExpr::Xor(pConfig.mRotateLeft(aBase, aRotate2), aScalar1);
        case GTermPattern::kPostRotateMul:
            return GExpr::Mul(pConfig.mRotateLeft(aBase, aRotate2), aOddScalar);
    }

    return GExpr();
}

} // namespace

GExpr GTermExpander::Expand(GSymbol pSymbol, bool pAllowMultiply) {
    const GTermExpandConfig aConfig = {
        &GExpr::RotL8,
        &NextScalar8,
        &NextOddScalar8,
        &NextRotate8,
    };
    return ExpandWithConfig(GExpr::Symbol(pSymbol), pAllowMultiply, aConfig);
}

GExpr GTermExpander::Expand(GExpr pExpr, bool pAllowMultiply) {
    const GTermExpandConfig aConfig = {
        &GExpr::RotL8,
        &NextScalar8,
        &NextOddScalar8,
        &NextRotate8,
    };
    return ExpandWithConfig(pExpr, pAllowMultiply, aConfig);
}
