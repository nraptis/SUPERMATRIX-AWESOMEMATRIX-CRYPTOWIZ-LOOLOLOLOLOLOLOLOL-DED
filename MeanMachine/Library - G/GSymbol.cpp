//
//  GSymbol.cpp
//  Superfox
//
//  Created by Dope on 4/20/26.
//

#include "GSymbol.hpp"

GSymbol::GSymbol() {
    Invalidate();
}

GSymbol GSymbol::Var(const std::string &pName) {
    GSymbol aSymbol;
    aSymbol.mType = GSymbolType::kVar;
    aSymbol.mName = pName;
    aSymbol.mSlot = TwistWorkSpaceSlot::kInvalid;
    return aSymbol;
}

GSymbol GSymbol::Buf(const TwistWorkSpaceSlot pSlot) {
    GSymbol aSymbol;
    aSymbol.mType = GSymbolType::kBuf;
    aSymbol.mSlot = pSlot;
    aSymbol.mName = BufName(pSlot);
    return aSymbol;
}

void GSymbol::Set(const GSymbol &pOther) {
    mName = pOther.mName;
    mType = pOther.mType;
    mSlot = pOther.mSlot;
}

void GSymbol::Invalidate() {
    mName = "";
    mType = GSymbolType::kInv;
    mSlot = TwistWorkSpaceSlot::kInvalid;
}

bool GSymbol::IsInvalid() const {
    switch (mType) {
        case GSymbolType::kVar:
            return mName.empty();
        case GSymbolType::kBuf:
            return (mSlot == TwistWorkSpaceSlot::kInvalid);
        default:
            return true;
    }
}

bool GSymbol::IsVar() const { return (mType == GSymbolType::kVar); }
bool GSymbol::IsBuf() const { return (mType == GSymbolType::kBuf); }

bool operator == (const GSymbol &pSymbolLHS, const GSymbol &pSymbolRHS) {
    if (pSymbolLHS.mType != pSymbolRHS.mType) {
        return false;
    }

    switch (pSymbolLHS.mType) {
        case GSymbolType::kInv:
            return true;
        case GSymbolType::kVar:
            return pSymbolLHS.mName == pSymbolRHS.mName;
        case GSymbolType::kBuf:
            return pSymbolLHS.mSlot == pSymbolRHS.mSlot;
        default:
            return false;
    }
}

bool operator != (const GSymbol &pSymbolLHS, const GSymbol &pSymbolRHS) {
    return !(pSymbolLHS == pSymbolRHS);
}


std::string BufName(TwistWorkSpaceSlot pSlot) {
    switch (pSlot) {
        case TwistWorkSpaceSlot::kSource: return "src";
        case TwistWorkSpaceSlot::kDest:   return "dst";

        case TwistWorkSpaceSlot::kSaltA: return "salt_a";
        case TwistWorkSpaceSlot::kSaltB: return "salt_b";
        case TwistWorkSpaceSlot::kSaltC: return "salt_c";
        case TwistWorkSpaceSlot::kSaltD: return "salt_d";

        case TwistWorkSpaceSlot::kSBoxA: return "sbox_a";
        case TwistWorkSpaceSlot::kSBoxB: return "sbox_b";
        case TwistWorkSpaceSlot::kSBoxC: return "sbox_c";
        case TwistWorkSpaceSlot::kSBoxD: return "sbox_d";

        case TwistWorkSpaceSlot::kSeedExpansionLaneA: return "seed_a";
        case TwistWorkSpaceSlot::kSeedExpansionLaneB: return "seed_b";
        case TwistWorkSpaceSlot::kSeedExpansionLaneC: return "seed_c";
        case TwistWorkSpaceSlot::kSeedExpansionLaneD: return "seed_d";

        case TwistWorkSpaceSlot::kWorkLaneA: return "work_a";
        case TwistWorkSpaceSlot::kWorkLaneB: return "work_b";
        case TwistWorkSpaceSlot::kWorkLaneC: return "work_c";
        case TwistWorkSpaceSlot::kWorkLaneD: return "work_d";

        case TwistWorkSpaceSlot::kOperationLaneA: return "op_a";
        case TwistWorkSpaceSlot::kOperationLaneB: return "op_b";

        case TwistWorkSpaceSlot::kMaskLaneA: return "mask_a";
        case TwistWorkSpaceSlot::kMaskLaneB: return "mask_b";

        case TwistWorkSpaceSlot::kKeyBoxUnrolledA: return "key_u_a";
        case TwistWorkSpaceSlot::kKeyBoxUnrolledB: return "key_u_b";
        case TwistWorkSpaceSlot::kKeyRowReadA:     return "key_r_a";
        case TwistWorkSpaceSlot::kKeyRowReadB:     return "key_r_b";
        case TwistWorkSpaceSlot::kKeyRowWriteA:    return "key_w_a";
        case TwistWorkSpaceSlot::kKeyRowWriteB:    return "key_w_b";

        case TwistWorkSpaceSlot::kMaskBoxUnrolledA: return "mask_u_a";
        case TwistWorkSpaceSlot::kMaskBoxUnrolledB: return "mask_u_b";
        case TwistWorkSpaceSlot::kMaskRowReadA:     return "mask_r_a";
        case TwistWorkSpaceSlot::kMaskRowReadB:     return "mask_r_b";
        case TwistWorkSpaceSlot::kMaskRowWriteA:    return "mask_w_a";
        case TwistWorkSpaceSlot::kMaskRowWriteB:    return "mask_w_b";

        default: return "inv";
    }
}

std::string BufAliasName(TwistWorkSpaceSlot pSlot) {
    switch (pSlot) {
        case TwistWorkSpaceSlot::kSource: return "aSource";
        case TwistWorkSpaceSlot::kDest: return "aDestination";

        case TwistWorkSpaceSlot::kSaltA: return "aSaltA";
        case TwistWorkSpaceSlot::kSaltB: return "aSaltB";
        case TwistWorkSpaceSlot::kSaltC: return "aSaltC";
        case TwistWorkSpaceSlot::kSaltD: return "aSaltD";

        case TwistWorkSpaceSlot::kSBoxA: return "aSBoxA";
        case TwistWorkSpaceSlot::kSBoxB: return "aSBoxB";
        case TwistWorkSpaceSlot::kSBoxC: return "aSBoxC";
        case TwistWorkSpaceSlot::kSBoxD: return "aSBoxD";

        case TwistWorkSpaceSlot::kSeedExpansionLaneA: return "aExpandLaneA";
        case TwistWorkSpaceSlot::kSeedExpansionLaneB: return "aExpandLaneB";
        case TwistWorkSpaceSlot::kSeedExpansionLaneC: return "aExpandLaneC";
        case TwistWorkSpaceSlot::kSeedExpansionLaneD: return "aExpandLaneD";

        case TwistWorkSpaceSlot::kWorkLaneA: return "aWorkLaneA";
        case TwistWorkSpaceSlot::kWorkLaneB: return "aWorkLaneB";
        case TwistWorkSpaceSlot::kWorkLaneC: return "aWorkLaneC";
        case TwistWorkSpaceSlot::kWorkLaneD: return "aWorkLaneD";

        case TwistWorkSpaceSlot::kOperationLaneA: return "aOperationLaneA";
        case TwistWorkSpaceSlot::kOperationLaneB: return "aOperationLaneB";

        case TwistWorkSpaceSlot::kMaskLaneA: return "aMaskLaneA";
        case TwistWorkSpaceSlot::kMaskLaneB: return "aMaskLaneB";

        case TwistWorkSpaceSlot::kKeyBoxUnrolledA: return "aKeyBoxUnrolledA";
        case TwistWorkSpaceSlot::kKeyBoxUnrolledB: return "aKeyBoxUnrolledB";
        case TwistWorkSpaceSlot::kKeyRowReadA: return "aKeyRowReadA";
        case TwistWorkSpaceSlot::kKeyRowReadB: return "aKeyRowReadB";
        case TwistWorkSpaceSlot::kKeyRowWriteA: return "aKeyRowWriteA";
        case TwistWorkSpaceSlot::kKeyRowWriteB: return "aKeyRowWriteB";

        case TwistWorkSpaceSlot::kMaskBoxUnrolledA: return "aMaskBoxUnrolledA";
        case TwistWorkSpaceSlot::kMaskBoxUnrolledB: return "aMaskBoxUnrolledB";
        case TwistWorkSpaceSlot::kMaskRowReadA: return "aMaskRowReadA";
        case TwistWorkSpaceSlot::kMaskRowReadB: return "aMaskRowReadB";
        case TwistWorkSpaceSlot::kMaskRowWriteA: return "aMaskRowWriteA";
        case TwistWorkSpaceSlot::kMaskRowWriteB: return "aMaskRowWriteB";

        default: return "aInvalidBuffer";
    }
}

GSymbol VarSymbol(const std::string &pName) {
    return GSymbol::Var(pName);
}

GSymbol BufSymbol(const TwistWorkSpaceSlot pSlot) {
    return GSymbol::Buf(pSlot);
}
