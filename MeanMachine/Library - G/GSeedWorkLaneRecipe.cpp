//
//  GSeedWorkLaneRecipe.cpp
//  MeanMachine
//
//  Created by John Snow on 4/22/26.
//

#include "GSeedWorkLaneRecipe.hpp"
#include <algorithm>
#include <set>

namespace {

std::vector<int> SeedWorkLaneRandomIndices(int pCount) {
    
    int aIndex[SEED_WORK_LANE_COUNT];
    for (int i=0; i<SEED_WORK_LANE_COUNT; i++) {
        aIndex[i] = i;
    }
    
    for (int i=1;i<SEED_WORK_LANE_COUNT;i++) {
        std::swap(aIndex[i], aIndex[Random::Get(i + 1)]);
    }
    std::vector<int> aResult;
    for (int i=0;i<pCount;i++) {
        aResult.push_back(aIndex[i]);
    }
    return aResult;
}


struct SaltBehaviorAssignment {
    LoopSaltBehavior mValue = LoopSaltBehavior::kInv;
    LoopSaltBehavior mCarry = LoopSaltBehavior::kInv;
    LoopSaltBehavior mPermute = LoopSaltBehavior::kInv;
};

struct LoopContractPhaseChoice {
    std::vector<LoopSaltBehavior> mPreferenceOrder;
    LoopSaltBehavior mSelected = LoopSaltBehavior::kInv;
};

std::vector<LoopSaltBehavior> RandomOrder(const std::set<LoopSaltBehavior> &pSet) {
    std::vector<LoopSaltBehavior> aResult;
    for (const LoopSaltBehavior &aBehavior: pSet) {
        aResult.push_back(aBehavior);
    }
    Random::Shuffle(&aResult);
    return aResult;
}

SaltBehaviorAssignment AssignLoopContractBehaviors(
    const std::set<LoopSaltBehavior> &pValueOptions,
    const std::set<LoopSaltBehavior> &pCarryOptions,
    const std::set<LoopSaltBehavior> &pPermuteOptions) {
    
    LoopContractPhaseChoice aChoices[3];
    aChoices[0].mPreferenceOrder = RandomOrder(pValueOptions);
    aChoices[1].mPreferenceOrder = RandomOrder(pCarryOptions);
    aChoices[2].mPreferenceOrder = RandomOrder(pPermuteOptions);
    
    auto isEnabled = [](const std::vector<LoopSaltBehavior> &pList) {
        return pList.empty() == false;
    };
    
    auto isUniqueIfEnabled = [](LoopSaltBehavior a,
                                bool aEnabled,
                                LoopSaltBehavior b,
                                bool bEnabled) {
        if (!aEnabled || !bEnabled) { return true; }
        return a != b;
    };
    
    SaltBehaviorAssignment aResult;
    aResult.mValue = LoopSaltBehavior::kInv;
    aResult.mCarry = LoopSaltBehavior::kInv;
    aResult.mPermute = LoopSaltBehavior::kInv;
    
    bool aValueEnabled = isEnabled(aChoices[0].mPreferenceOrder);
    bool aCarryEnabled = isEnabled(aChoices[1].mPreferenceOrder);
    bool aPermuteEnabled = isEnabled(aChoices[2].mPreferenceOrder);
    
    for (int aTry = 0; aTry < 2; aTry++) {
        if (aValueEnabled) {
            aResult.mValue = Random::Choice(aChoices[0].mPreferenceOrder);
        }
        if (aCarryEnabled) {
            aResult.mCarry = Random::Choice(aChoices[1].mPreferenceOrder);
        }
        if (aPermuteEnabled) {
            aResult.mPermute = Random::Choice(aChoices[2].mPreferenceOrder);
        }
        
        if (isUniqueIfEnabled(aResult.mValue, aValueEnabled, aResult.mCarry, aCarryEnabled) &&
            isUniqueIfEnabled(aResult.mValue, aValueEnabled, aResult.mPermute, aPermuteEnabled) &&
            isUniqueIfEnabled(aResult.mCarry, aCarryEnabled, aResult.mPermute, aPermuteEnabled)) {
            return aResult;
        }
    }
    
    std::set<LoopSaltBehavior> aUsed;
    auto pick = [&](const std::vector<LoopSaltBehavior>& pBehaviorList) {
        for (auto aBehavior : pBehaviorList) {
            if (!aUsed.contains(aBehavior)) {
                aUsed.insert(aBehavior);
                return aBehavior;
            }
        }
        return LoopSaltBehavior::kInv;
    };
    
    aResult.mValue = pick(aChoices[0].mPreferenceOrder);
    aResult.mCarry = pick(aChoices[1].mPreferenceOrder);
    aResult.mPermute = pick(aChoices[2].mPreferenceOrder);
    
    return aResult;
}

void SetWorkLaneError(std::string *pError,
                      const std::string &pMessage) {
    if (pError != nullptr) {
        *pError = pMessage;
    }
}

bool AppendLoopStatement(GLoop *pLoop,
                         GScopeState *pScopeState,
                         const GStatement &pStatement,
                         std::string *pError) {
    if ((pLoop == nullptr) || (pScopeState == nullptr)) {
        SetWorkLaneError(pError, "Internal error: loop append target was null.");
        return false;
    }
    if (pStatement.IsInvalid()) {
        SetWorkLaneError(pError, "Internal error: attempted to append an invalid statement.");
        return false;
    }

    GStatement aStatement = pStatement;
    pLoop->AddBody(aStatement);
    pScopeState->Consume(aStatement);
    return true;
}

bool EmitAssign(GLoop *pLoop,
                GScopeState *pScopeState,
                const GTarget &pTarget,
                const GExpr &pExpression,
                std::string *pError) {
    return AppendLoopStatement(pLoop,
                               pScopeState,
                               GStatement::Assign(pTarget, pExpression),
                               pError);
}

bool EmitXorAssign(GLoop *pLoop,
                   GScopeState *pScopeState,
                   const GTarget &pTarget,
                   const GExpr &pExpression,
                   std::string *pError) {
    return AppendLoopStatement(pLoop,
                               pScopeState,
                               GStatement::XorAssign(pTarget, pExpression),
                               pError);
}

bool EmitRecipeAssign(GLoop *pLoop,
                      GScopeState *pScopeState,
                      GStatementRecipe *pRecipe,
                      GSymbol pTarget,
                      std::string &pError) {
    if (pRecipe == nullptr) {
        pError = "Internal error: recipe assign target was null.";
        return false;
    }

    GStatement aStatement;
    if (!pRecipe->Bake(pTarget, aStatement, pError)) {
        return false;
    }
    return AppendLoopStatement(pLoop, pScopeState, aStatement, &pError);
}

bool EmitRecipeMix(GLoop *pLoop,
                   GScopeState *pScopeState,
                   GStatementRecipe *pRecipe,
                   GSymbol pTarget,
                   std::string &pError) {
    if (pRecipe == nullptr) {
        pError = "Internal error: recipe mix target was null.";
        return false;
    }

    GStatement aStatement;
    if (!pRecipe->BakeMix(pTarget, aStatement, pError)) {
        return false;
    }
    return AppendLoopStatement(pLoop, pScopeState, aStatement, &pError);
}

GExpr SaltReadExpr(GSymbol pSalt,
                   LoopSaltBehavior pBehavior,
                   GSymbol pLoopIndex,
                   GSymbol pSourceByteA,
                   bool pSecondSourceEnabled,
                   GSymbol pSourceByteB,
                   GSymbol pValue,
                   GSymbol pCarry,
                   GSymbol pOracle,
                   int pOffset) {
    switch (pBehavior) {
        case LoopSaltBehavior::kSourceA:
            return GQuick::BufferRead(pSalt, pSourceByteA);
            
        case LoopSaltBehavior::kSourceB:
            if (pSecondSourceEnabled) {
                return GQuick::BufferRead(pSalt, pSourceByteB);
            }
            return GExpr::ReadSaltWrap(pSalt, pLoopIndex, pOracle, pOffset);
        case LoopSaltBehavior::kValue:
            return GQuick::BufferRead(pSalt, pValue);
        case LoopSaltBehavior::kCarry:
            return GQuick::BufferRead(pSalt, pCarry);
        case LoopSaltBehavior::kLoopIndex:
        case LoopSaltBehavior::kInv:
        default:
            return GExpr::ReadSaltWrap(pSalt, pLoopIndex, pOracle, pOffset);
    }
}

} // namespace


GSeedWorkLaneRecipeWorkSpace::GSeedWorkLaneRecipeWorkSpace() {
    
}

bool GSeedWorkLaneRecipeWorkSpace::Plan(bool pSecondPass, std::string &pErrorString) {
    
    const std::string aCategryIdentifierSalt = "t";
    const std::string aCategryIdentifierSBox = "s";
    
    GScopeRules aScopeRules;
    
    GPool aGlobalPool(&aScopeRules);
    
    GScopeState aStateGlobal;
    GScopeState aStateLocal[SEED_WORK_LANE_COUNT];
    
    std::vector<GPool> mLoopPools;
    mLoopPools.reserve(SEED_WORK_LANE_COUNT);
    for (int i = 0; i < SEED_WORK_LANE_COUNT; i++) {
        mLoopPools.emplace_back(&aScopeRules);
    }
    
    aStateGlobal.Clear();
    for (int i=0;i<SEED_WORK_LANE_COUNT;i++) {
        aStateLocal[i].Clear();
    }
    
    mSource = BufSymbol(TwistWorkSpaceSlot::kSource);
    
    GSymbol aDests[SEED_WORK_LANE_COUNT];
    GSymbol aSalts[SEED_WORK_LANE_COUNT];
    GSymbol aSBoxes[SEED_WORK_LANE_COUNT];
    
    aSalts[0] = BufSymbol(TwistWorkSpaceSlot::kSaltA);
    aSalts[1] = BufSymbol(TwistWorkSpaceSlot::kSaltB);
    aSalts[2] = BufSymbol(TwistWorkSpaceSlot::kSaltC);
    aSalts[3] = BufSymbol(TwistWorkSpaceSlot::kSaltD);
    
    aDests[0] = BufSymbol(TwistWorkSpaceSlot::kWorkLaneA);
    aDests[1] = BufSymbol(TwistWorkSpaceSlot::kWorkLaneB);
    aDests[2] = BufSymbol(TwistWorkSpaceSlot::kWorkLaneC);
    aDests[3] = BufSymbol(TwistWorkSpaceSlot::kWorkLaneD);
    
    mLoopIndex = VarSymbol("aLoopIndex");
    mValue = VarSymbol("aValue");
    mValueKey = VarSymbol("aValueKey");
    mPermute = VarSymbol("aPermute");
    mPermuteKey = VarSymbol("aPermuteKey");
    mCarry = VarSymbol("aCarry");
    
    mSourceByteA = VarSymbol("aSourceByteA");
    mSourceByteB = VarSymbol("aSourceByteB");
    
    mSaltByte = VarSymbol("aSaltByte");
    
    
    mOracleSourceA = VarSymbol("aOracleSourceA");
    mOracleSourceB = VarSymbol("aOracleSourceB");
    
    mOracleValueSalt = VarSymbol("aOracleValueSalt");
    mOracleCarrySalt = VarSymbol("aOracleCarrySalt");
    mOraclePermuteSalt = VarSymbol("aOraclePermuteSalt");
    
    mOracleValueSBox = VarSymbol("aOracleValueSBox");
    mOracleCarrySBox = VarSymbol("aOracleCarrySBox");
    mOraclePermuteSBox = VarSymbol("aOraclePermuteSBox");
    
    
    //Random::Shuffle(aDests, SEED_WORK_LANE_COUNT);
    //for (int i=1;i<SEED_WORK_LANE_COUNT;i++) { std::swap(aDests[i], aDests[RandomGet(i + 1)]); }
    
    mWorkerA = aDests[0];
    mWorkerB = aDests[1];
    mWorkerC = aDests[2];
    mWorkerD = aDests[3];
    
    //for (int i=1;i<SEED_WORK_LANE_COUNT;i++) { std::swap(aSalts[i], aSalts[RandomGet(i + 1)]); }
    //Random::Shuffle(aSalts, SEED_WORK_LANE_COUNT);
    mSaltA = aSalts[0];
    mSaltB = aSalts[1];
    mSaltC = aSalts[2];
    mSaltD = aSalts[3];
    
    aSBoxes[0] = BufSymbol(TwistWorkSpaceSlot::kSBoxA);
    aSBoxes[1] = BufSymbol(TwistWorkSpaceSlot::kSBoxB);
    aSBoxes[2] = BufSymbol(TwistWorkSpaceSlot::kSBoxC);
    aSBoxes[3] = BufSymbol(TwistWorkSpaceSlot::kSBoxD);
    //for (int i=1;i<SEED_WORK_LANE_COUNT;i++) { std::swap(aSBoxes[i], aSBoxes[RandomGet(i + 1)]); }
    //Random::Shuffle(aSalts, SEED_WORK_LANE_COUNT);
    mSBoxA = aSBoxes[0];
    mSBoxB = aSBoxes[1];
    mSBoxC = aSBoxes[2];
    mSBoxD = aSBoxes[3];
    
    for (int i=0;i<SEED_WORK_LANE_COUNT;i++) {
        aScopeRules.SetReadPreferredMaximum(aSBoxes[i], 3);
        aScopeRules.SetReadPreferredMaximum(aSalts[i], 3);
    }

    
    if (pSecondPass == false) {

        // Pass1:
        //
        // SBoxes:
        // L1: must A, not D  → {A,B,C}
        // L2: must B, not A  → {B,C,D}
        // L3: must C, not B  → {C,D,A}
        // L4: must D, not C  → {D,A,B}
        mLoopPools[0].Add(mSBoxA, aCategryIdentifierSBox, 0, 2);
        mLoopPools[0].Add(mSBoxB, aCategryIdentifierSBox, 0, 2);
        mLoopPools[0].Add(mSBoxC, aCategryIdentifierSBox, 0, 2);
        
        mLoopPools[1].Add(mSBoxB, aCategryIdentifierSBox, 0, 2);
        mLoopPools[1].Add(mSBoxC, aCategryIdentifierSBox, 0, 2);
        mLoopPools[1].Add(mSBoxD, aCategryIdentifierSBox, 0, 2);
        
        mLoopPools[2].Add(mSBoxC, aCategryIdentifierSBox, 0, 2);
        mLoopPools[2].Add(mSBoxD, aCategryIdentifierSBox, 0, 2);
        mLoopPools[2].Add(mSBoxA, aCategryIdentifierSBox, 0, 2);
        
        mLoopPools[3].Add(mSBoxD, aCategryIdentifierSBox, 0, 2);
        mLoopPools[3].Add(mSBoxA, aCategryIdentifierSBox, 0, 2);
        mLoopPools[3].Add(mSBoxB, aCategryIdentifierSBox, 0, 2);
        
        // Salts:
        // L1: must C, not A  → {C,B,D}
        // L2: must D, not B  → {D,C,A}
        // L3: must A, not C  → {A,D,B}
        // L4: must B, not D  → {B,A,C}
        
        mLoopPools[0].Add(mSaltB, aCategryIdentifierSalt, 0, 2);
        mLoopPools[0].Add(mSaltC, aCategryIdentifierSalt, 0, 2);
        mLoopPools[0].Add(mSaltD, aCategryIdentifierSalt, 0, 2);
        
        mLoopPools[1].Add(mSaltC, aCategryIdentifierSalt, 0, 2);
        mLoopPools[1].Add(mSaltD, aCategryIdentifierSalt, 0, 2);
        mLoopPools[1].Add(mSaltA, aCategryIdentifierSalt, 0, 2);
        
        mLoopPools[2].Add(mSaltD, aCategryIdentifierSalt, 0, 2);
        mLoopPools[2].Add(mSaltA, aCategryIdentifierSalt, 0, 2);
        mLoopPools[2].Add(mSaltB, aCategryIdentifierSalt, 0, 2);
        
        mLoopPools[3].Add(mSaltA, aCategryIdentifierSalt, 0, 2);
        mLoopPools[3].Add(mSaltB, aCategryIdentifierSalt, 0, 2);
        mLoopPools[3].Add(mSaltC, aCategryIdentifierSalt, 0, 2);
        
    } else {
        
        // Pass2:
        //
        // SBoxes:
        // L1: must C, not A  → {C,B,D}
        // L2: must D, not B  → {D,C,A}
        // L3: must A, not C  → {A,D,B}
        // L4: must B, not D  → {B,A,C}
        mLoopPools[0].Add(mSBoxC, aCategryIdentifierSBox, 0, 2);
        mLoopPools[0].Add(mSBoxB, aCategryIdentifierSBox, 0, 2);
        mLoopPools[0].Add(mSBoxD, aCategryIdentifierSBox, 0, 2);
        
        mLoopPools[1].Add(mSBoxD, aCategryIdentifierSBox, 0, 2);
        mLoopPools[1].Add(mSBoxC, aCategryIdentifierSBox, 0, 2);
        mLoopPools[1].Add(mSBoxA, aCategryIdentifierSBox, 0, 2);
        
        mLoopPools[2].Add(mSBoxA, aCategryIdentifierSBox, 0, 2);
        mLoopPools[2].Add(mSBoxD, aCategryIdentifierSBox, 0, 2);
        mLoopPools[2].Add(mSBoxB, aCategryIdentifierSBox, 0, 2);
        
        mLoopPools[3].Add(mSBoxB, aCategryIdentifierSBox, 0, 2);
        mLoopPools[3].Add(mSBoxA, aCategryIdentifierSBox, 0, 2);
        mLoopPools[3].Add(mSBoxC, aCategryIdentifierSBox, 0, 2);
        
        
        // Salts:
        // L1: must B, not C  → {B,A,D}
        // L2: must C, not D  → {C,A,B}
        // L3: must D, not B  → {D,C,A}
        // L4: must A, not C  → {A,B,D}
        mLoopPools[0].Add(mSaltB, aCategryIdentifierSalt, 0, 2);
        mLoopPools[0].Add(mSaltA, aCategryIdentifierSalt, 0, 2);
        mLoopPools[0].Add(mSaltD, aCategryIdentifierSalt, 0, 2);
        
        mLoopPools[1].Add(mSaltC, aCategryIdentifierSalt, 0, 2);
        mLoopPools[1].Add(mSaltA, aCategryIdentifierSalt, 0, 2);
        mLoopPools[1].Add(mSaltB, aCategryIdentifierSalt, 0, 2);
        
        mLoopPools[2].Add(mSaltD, aCategryIdentifierSalt, 0, 2);
        mLoopPools[2].Add(mSaltC, aCategryIdentifierSalt, 0, 2);
        mLoopPools[2].Add(mSaltA, aCategryIdentifierSalt, 0, 2);
        
        mLoopPools[3].Add(mSaltA, aCategryIdentifierSalt, 0, 2);
        mLoopPools[3].Add(mSaltB, aCategryIdentifierSalt, 0, 2);
        mLoopPools[3].Add(mSaltD, aCategryIdentifierSalt, 0, 2);
    }
    
    
    
    bool aUseCarry[SEED_WORK_LANE_COUNT];
    int aUseCarryCount = Random::Get(SEED_WORK_LANE_CARRY_MIN, SEED_WORK_LANE_CARRY_MAX);
    memset(aUseCarry, 0, sizeof(aUseCarry));
    std::vector<int> aCarryIndices = SeedWorkLaneRandomIndices(aUseCarryCount);
    for (int i=0;i<aCarryIndices.size();i++) {
        aUseCarry[aCarryIndices[i]] = true;
    }
    
    bool aUsePermute[SEED_WORK_LANE_COUNT];
    int aUsePermuteCount = Random::Get(SEED_WORK_LANE_PERMUTE_MIN, SEED_WORK_LANE_PERMUTE_MAX);
    memset(aUsePermute, 0, sizeof(aUsePermute));
    std::vector<int> aPermuteIndices = SeedWorkLaneRandomIndices(aUsePermuteCount);
    for (int i=0;i<aPermuteIndices.size();i++) {
        aUsePermute[aPermuteIndices[i]] = true;
    }
    
    // Always use permute or carry.
    for (int i=0;i<aCarryIndices.size();i++) {
        if ((aUseCarry[i] == false) && (aUsePermute[i] == false)) {
            int aPick = Random::Get(2);
            if (aPick == 0) {
                aUseCarry[i] = true;
            } else {
                aUsePermute[i] = true;
            }
        }
    }
    
    
    // Pick sources
    for (int aWhich=0; aWhich<SEED_WORK_LANE_COUNT; aWhich++) {
        
        // Pass1:
        // (       src ) => w_a
        // (src? + w_a ) => w_b
        // (w_a? + w_b ) => w_c
        // (w_b? + w_c ) => w_d

        // Pass2:
        // (w_c? + w_d ) => w_a
        // (w_d? + w_a ) => w_b
        // (w_a? + w_b ) => w_c
        // (w_b? + w_c ) => w_d
        
        mLoopContracts[aWhich].mSourceA.Invalidate();
        mLoopContracts[aWhich].mSourceB.Invalidate();
        
        bool aAllowSecondSource = true;
        bool aUseSecondSource = false;
        if (aWhich == 0) {
            if (pSecondPass) {
                aAllowSecondSource = true;
            } else {
                aAllowSecondSource = false;
            }
        }
        
        if (aAllowSecondSource) {
            aUseSecondSource = (Random::Get(3) != 1);
        }
        
        if (pSecondPass) {
            if (aWhich == 3) { mLoopContracts[aWhich].mSourceA = mWorkerC; }
            else if (aWhich == 2) { mLoopContracts[aWhich].mSourceA = mWorkerB; }
            else if (aWhich == 1) { mLoopContracts[aWhich].mSourceA = mWorkerA; }
            else  { mLoopContracts[aWhich].mSourceA = mWorkerD; }
            if ((aAllowSecondSource == true) && (aUseSecondSource == true)) {
                if (aWhich == 3) { mLoopContracts[aWhich].mSourceB = mWorkerB; }
                else if (aWhich == 2) { mLoopContracts[aWhich].mSourceB = mWorkerA; }
                else if (aWhich == 1) { mLoopContracts[aWhich].mSourceB = mWorkerD; }
                else  { mLoopContracts[aWhich].mSourceB = mWorkerC; }
            }
        } else {
            if (aWhich == 3) { mLoopContracts[aWhich].mSourceA = mWorkerC; }
            else if (aWhich == 2) { mLoopContracts[aWhich].mSourceA = mWorkerB; }
            else if (aWhich == 1) { mLoopContracts[aWhich].mSourceA = mWorkerA; }
            else  { mLoopContracts[aWhich].mSourceA = mSource; }
            if ((aAllowSecondSource == true) && (aUseSecondSource == true)) {
                if (aWhich == 3) { mLoopContracts[aWhich].mSourceB = mWorkerB; }
                else if (aWhich == 2) { mLoopContracts[aWhich].mSourceB = mWorkerA; }
                else if (aWhich == 1) { mLoopContracts[aWhich].mSourceB = mSource; }
            }
        }
        if (mLoopContracts[aWhich].mSourceB.IsInvalid()) {
            mLoopContracts[aWhich].mSecondSourceEnabled = false;
        } else {
            mLoopContracts[aWhich].mSecondSourceEnabled = true;
        }
        
        if (mLoopContracts[aWhich].mSourceB.IsInvalid() == false) {
            std::vector<LoopSecondSourceMode> aSecondSourceOptions = {
                LoopSecondSourceMode::kValue,
            };
            if (aUseCarry[aWhich] == true) {
                aSecondSourceOptions.push_back(LoopSecondSourceMode::kCarry);
            }
            if (aUsePermute[aWhich] == true) {
                aSecondSourceOptions.push_back(LoopSecondSourceMode::kPermute);
            }
            mLoopContracts[aWhich].mSecondSourceMode = Random::Choice(aSecondSourceOptions);
        } else {
            mLoopContracts[aWhich].mSecondSourceMode = LoopSecondSourceMode::kInv;
        }
    }
    
    // Pick destinations
    for (int aWhich=0; aWhich<SEED_WORK_LANE_COUNT; aWhich++) {
        
        // Pass1 and Pass2:
        // w_a
        // w_b
        // w_c
        // w_d
        if (aWhich == 0) { mLoopContracts[aWhich].mDest = mWorkerA; }
        else if (aWhich == 1) { mLoopContracts[aWhich].mDest = mWorkerB; }
        else if (aWhich == 2) { mLoopContracts[aWhich].mDest = mWorkerC; }
        else  { mLoopContracts[aWhich].mDest = mWorkerD; }
    }
    
    for (int aWhich=0; aWhich<SEED_WORK_LANE_COUNT; aWhich++) {
        
        mLoopContracts[aWhich].mCarryEnabled = aUseCarry[aWhich];
        mLoopContracts[aWhich].mPermuteEnabled = aUsePermute[aWhich];
        
    }
    
    for (int aWhich=0; aWhich<SEED_WORK_LANE_COUNT; aWhich++) {
        mLoopContracts[aWhich].mCarrySBoxBehavior = LoopSBoxBehavior::kInv;
        mLoopContracts[aWhich].mPermuteSBoxBehavior = LoopSBoxBehavior::kInv;
        if (mLoopContracts[aWhich].mPermuteEnabled == true) {
            if (mLoopContracts[aWhich].mCarryEnabled == true) {
                int aChoice = Random::Get(2);
                if (aChoice == 1) {
                    mLoopContracts[aWhich].mCarrySBoxBehavior = LoopSBoxBehavior::kMixWithLoopIndex;
                } else {
                    mLoopContracts[aWhich].mPermuteSBoxBehavior = LoopSBoxBehavior::kMixWithLoopIndex;
                }
            } else {
                mLoopContracts[aWhich].mPermuteSBoxBehavior = LoopSBoxBehavior::kMixWithLoopIndex;
            }
        } else if (mLoopContracts[aWhich].mCarryEnabled == true) {
            mLoopContracts[aWhich].mCarrySBoxBehavior = LoopSBoxBehavior::kMixWithLoopIndex;
        }
    }
    
    for (int aWhich=0; aWhich<SEED_WORK_LANE_COUNT; aWhich++) {
        
        std::set<LoopSaltBehavior> aValueSaltBehaviors;
        if (true) {
            aValueSaltBehaviors.insert(LoopSaltBehavior::kLoopIndex);
            aValueSaltBehaviors.insert(LoopSaltBehavior::kSourceA);
            if (mLoopContracts[aWhich].mCarryEnabled == true) {
                aValueSaltBehaviors.insert(LoopSaltBehavior::kCarry);
            }
            if (mLoopContracts[aWhich].mSecondSourceEnabled == true) {
                aValueSaltBehaviors.insert(LoopSaltBehavior::kSourceB);
            }
        }
        
        std::set<LoopSaltBehavior> aCarrySaltBehaviors;
        if (mLoopContracts[aWhich].mCarryEnabled == true) {
            aCarrySaltBehaviors.insert(LoopSaltBehavior::kLoopIndex);
            aCarrySaltBehaviors.insert(LoopSaltBehavior::kValue);
            aCarrySaltBehaviors.insert(LoopSaltBehavior::kSourceA);
            if (mLoopContracts[aWhich].mSecondSourceEnabled == true) {
                aCarrySaltBehaviors.insert(LoopSaltBehavior::kSourceB);
            }
        }
        
        std::set<LoopSaltBehavior> aPermuteSaltBehaviors;
        if (mLoopContracts[aWhich].mPermuteEnabled == true) {
            aPermuteSaltBehaviors.insert(LoopSaltBehavior::kLoopIndex);
            aPermuteSaltBehaviors.insert(LoopSaltBehavior::kValue);
            aPermuteSaltBehaviors.insert(LoopSaltBehavior::kSourceA);
            if (mLoopContracts[aWhich].mCarryEnabled == true) {
                aPermuteSaltBehaviors.insert(LoopSaltBehavior::kCarry);
            }
            if (mLoopContracts[aWhich].mSecondSourceEnabled == true) {
                aPermuteSaltBehaviors.insert(LoopSaltBehavior::kSourceB);
            }
        }
        
        SaltBehaviorAssignment aSaltBehaviorAssignment = AssignLoopContractBehaviors(aValueSaltBehaviors,
                                                                                     aCarrySaltBehaviors,
                                                                                     aPermuteSaltBehaviors);
        
        mLoopContracts[aWhich].mValueSaltBehavior = aSaltBehaviorAssignment.mValue;
        mLoopContracts[aWhich].mCarrySaltBehavior = aSaltBehaviorAssignment.mCarry;
        mLoopContracts[aWhich].mPermuteSaltBehavior = aSaltBehaviorAssignment.mPermute;
        
    }
    
    for (int aWhich=0; aWhich<SEED_WORK_LANE_COUNT; aWhich++) {
        
        std::vector<LoopKeyOrdering> aKeyOrderingChoices = { LoopKeyOrdering::kBeforeSBox, LoopKeyOrdering::kAfterSBox };
        
        mLoopContracts[aWhich].mValueKeyOrdering_Carry = Random::Choice(aKeyOrderingChoices);
        mLoopContracts[aWhich].mValueKeyOrdering_SourceB = Random::Choice(aKeyOrderingChoices);
        mLoopContracts[aWhich].mValueKeyOrdering_Salt = Random::Choice(aKeyOrderingChoices);
        
        mLoopContracts[aWhich].mCarryKeyOrdering_Value = Random::Choice(aKeyOrderingChoices);
        mLoopContracts[aWhich].mCarryKeyOrdering_SourceB = Random::Choice(aKeyOrderingChoices);
        mLoopContracts[aWhich].mCarryKeyOrdering_Salt = Random::Choice(aKeyOrderingChoices);
        
        mLoopContracts[aWhich].mPermuteKeyOrdering_Carry = Random::Choice(aKeyOrderingChoices);
        mLoopContracts[aWhich].mPermuteKeyOrdering_Value = Random::Choice(aKeyOrderingChoices);
        mLoopContracts[aWhich].mPermuteKeyOrdering_Salt = Random::Choice(aKeyOrderingChoices);
    }
    
    enum class LoopPermuteAndCarryOrdering : std::uint8_t {
        kValueCarryPermute = 0,
        kValuePermuteCarry = 1,
    };
    
    for (int aWhich=0; aWhich<SEED_WORK_LANE_COUNT; aWhich++) {
        
        std::vector<GSymbol> aFetchedSalts;
        std::vector<GSymbol> aFetchedSBoxes;
        
        aStateLocal[aWhich].Clear();
        
        
        

        
        
        GSymbol aRequired;
        if (pSecondPass == false) {
            // Pass1:
            // Salts:
            // L1: must C, not A  → {C,B,D}
            // L2: must D, not B  → {D,C,A}
            // L3: must A, not C  → {A,D,B}
            // L4: must B, not D  → {B,A,C}
            if (aWhich == 1) { aRequired = mSaltD; }
            else if (aWhich == 2) { aRequired = mSaltA; }
            else if (aWhich == 3) { aRequired = mSaltB; }
            else { aRequired = mSaltC; }
        } else {
            // Pass2:
            // Salts:
            // L1: must B, not C  → {B,A,D}
            // L2: must C, not D  → {C,A,B}
            // L3: must D, not B  → {D,C,A}
            // L4: must A, not C  → {A,B,D}
            if (aWhich == 0) { aRequired = mSaltB; }
            else if (aWhich == 1) { aRequired = mSaltC; }
            else if (aWhich == 2) { aRequired = mSaltD; }
            else { aRequired = mSaltA; }
        }
        
        std::vector<GStatementSlot> aSlotListSalts;
        if (!mLoopPools[aWhich].FetchSlots(aCategryIdentifierSalt,
                                           3,
                                           aRequired,
                                           aSlotListSalts,
                                           aStateLocal[aWhich],
                                           aStateGlobal,
                                           pErrorString)) {
            return false;
        }
        
        for (int i=0; i<aSlotListSalts.size(); i++) {
            switch (aSlotListSalts[i].mType) {
                case GStatementSlotType::kInv:
                    pErrorString = "Plan: Invalid Slot Type, Expected Symbol (Salt)";
                    return false;
                case GStatementSlotType::kSymbol:
                    aFetchedSalts.push_back(aSlotListSalts[i].mSymbol);
                    break;
                case GStatementSlotType::kExpr:
                    pErrorString = "Plan: Invalid Slot Type, Expected Symbol (Salt)";
                    return false;
            }
        }
        
        if (aSlotListSalts.size() < 3) {
            pErrorString = "Plan: We needed to get 3 salts here. (Salt)";
            return false;
        }
        
        if (pSecondPass == false) {
            // Pass1:
            // SBoxes:
            // L1: must A, not D  → {A,B,C}
            // L2: must B, not A  → {B,C,D}
            // L3: must C, not B  → {C,D,A}
            // L4: must D, not C  → {D,A,B}
            if (aWhich == 1) { aRequired = mSBoxB; }
            else if (aWhich == 2) { aRequired = mSBoxC; }
            else if (aWhich == 3) { aRequired = mSBoxD; }
            else { aRequired = mSBoxA; }
        } else {
            // Pass2:
            // SBoxes:
            // L1: must C, not A  → {C,B,D}
            // L2: must D, not B  → {D,C,A}
            // L3: must A, not C  → {A,D,B}
            // L4: must B, not D  → {B,A,C}
            if (aWhich == 0) { aRequired = mSBoxC; }
            else if (aWhich == 1) { aRequired = mSBoxD; }
            else if (aWhich == 2) { aRequired = mSBoxA; }
            else { aRequired = mSBoxB; }
        }
        
        std::vector<GStatementSlot> aSlotListSBoxes;
        if (!mLoopPools[aWhich].FetchSlots(aCategryIdentifierSBox,
                                           3,
                                           aRequired,
                                           aSlotListSBoxes,
                                           aStateLocal[aWhich],
                                           aStateGlobal,
                                           pErrorString)) {
            return false;
        }
        
        for (int i=0; i<aSlotListSBoxes.size(); i++) {
            switch (aSlotListSBoxes[i].mType) {
                    
                case GStatementSlotType::kInv:
                    pErrorString = "Plan: Invalid Slot Type, Expected Symbol (S-Box)";
                    return false;
                case GStatementSlotType::kSymbol:
                    aFetchedSBoxes.push_back(aSlotListSBoxes[i].mSymbol);
                    break;
                case GStatementSlotType::kExpr:
                    pErrorString = "Plan: Invalid Slot Type, Expected Symbol (S-Box)";
                    return false;
            }
        }
        
        if (aSlotListSBoxes.size() < 3) {
            pErrorString = "Plan: We needed to get 3 s-boxes here. (S-Box)";
            return false;
        }
        
        
        mLoopContracts[aWhich].mValueSalt = aFetchedSalts[0];
        mLoopContracts[aWhich].mValueSBox = aFetchedSBoxes[0];
        aStateLocal[aWhich].Consume(aFetchedSalts[0]);
        aStateLocal[aWhich].Consume(aFetchedSBoxes[0]);
        
        if (mLoopContracts[aWhich].mCarryEnabled) {
            mLoopContracts[aWhich].mCarrySalt = aFetchedSalts[1];
            mLoopContracts[aWhich].mCarrySBox = aFetchedSBoxes[1];
            aStateLocal[aWhich].Consume(aFetchedSalts[1]);
            aStateLocal[aWhich].Consume(aFetchedSBoxes[1]);
        }
        
        if (mLoopContracts[aWhich].mPermuteEnabled) {
            mLoopContracts[aWhich].mPermuteSalt = aFetchedSalts[2];
            mLoopContracts[aWhich].mPermuteSBox = aFetchedSBoxes[2];
            aStateLocal[aWhich].Consume(aFetchedSalts[2]);
            aStateLocal[aWhich].Consume(aFetchedSBoxes[2]);
        }
        
        aStateGlobal.Consume(aStateLocal[aWhich]);
        
    }
    
    return true;
}


bool GSeedWorkLaneRecipeWorkSpace::Bake(std::vector<GLoop> &pResult, std::string &pErrorString) {
    return BakeAlt(pResult, pErrorString);
}

bool GSeedWorkLaneRecipeWorkSpace::BakeAlt(std::vector<GLoop> &pResult, std::string &pErrorString) {
    const GSymbol aCarryKey = VarSymbol("aCarryKey");

    pResult.clear();

    for (int aWhich = 0; aWhich < SEED_WORK_LANE_COUNT; ++aWhich) {
        GLoop aLoop;
        aLoop.mLoopVariable = mLoopIndex;
        aLoop.mLoopVariableName = "aLoopIndex";
        aLoop.mLoopBegin = 0;
        aLoop.mLoopEndText = "S_BLOCK";
        aLoop.mLoopStep = 1;

        GScopeState aStateLocal;
        aStateLocal.Clear();

        GStatementRecipe aRecipe;
        const GSeedWorkLaneRecipeLoopContract &aContract = mLoopContracts[aWhich];

        aRecipe.Clear();
        aRecipe.AddExpandable(GQuick::BufferReadRandomOffset(aContract.mSourceA,
                                                             mLoopIndex,
                                                             mOracleSourceA),
                              25,
                              true);
        if (!EmitRecipeAssign(&aLoop, &aStateLocal, &aRecipe, mSourceByteA, pErrorString)) {
            return false;
        }

        if (aContract.mSecondSourceEnabled) {
            aRecipe.Clear();
            aRecipe.AddExpandable(GQuick::BufferReadRandomOffset(aContract.mSourceB,
                                                                 mLoopIndex,
                                                                 mOracleSourceB),
                                  25,
                                  true);
            if (!EmitRecipeAssign(&aLoop, &aStateLocal, &aRecipe, mSourceByteB, pErrorString)) {
                return false;
            }
        }
        
        
        if (!EmitAssign(&aLoop,
                        &aStateLocal,
                        GTarget::Symbol(mSaltByte),
                        SaltReadExpr(aContract.mValueSalt,
                                     aContract.mValueSaltBehavior,
                                     mLoopIndex,
                                     mSourceByteA,
                                     aContract.mSecondSourceEnabled,
                                     mSourceByteB,
                                     mValue,
                                     mCarry,
                                     mOracleValueSalt,
                                     Random::Get(S_SALT)),
                        &pErrorString)) {
            return false;
        }

        aRecipe.Clear();
        aRecipe.Add(mValue);
        aRecipe.Add(mSourceByteA);
        if (aContract.mValueKeyOrdering_Salt == LoopKeyOrdering::kBeforeSBox) {
            aRecipe.Add(mSaltByte);
        }
        if (aContract.mCarryEnabled &&
            (aContract.mValueKeyOrdering_Carry == LoopKeyOrdering::kBeforeSBox)) {
            aRecipe.Add(mCarry);
        }
        if (aContract.mSecondSourceEnabled &&
            (aContract.mSecondSourceMode == LoopSecondSourceMode::kValue) &&
            (aContract.mValueKeyOrdering_SourceB == LoopKeyOrdering::kBeforeSBox)) {
            aRecipe.Add(mSourceByteB);
        }
        if (!EmitRecipeAssign(&aLoop, &aStateLocal, &aRecipe, mValueKey, pErrorString)) {
            return false;
        }

        if (!EmitAssign(&aLoop,
                        &aStateLocal,
                        GTarget::Symbol(mValue),
                        GQuick::BufferRead(aContract.mValueSBox, mValueKey),
                        &pErrorString)) {
            return false;
        }

        aRecipe.Clear();
        bool aHasValueRemainder = false;
        if (aContract.mValueKeyOrdering_Salt == LoopKeyOrdering::kAfterSBox) {
            aRecipe.Add(mSaltByte);
            aHasValueRemainder = true;
        }
        if (aContract.mCarryEnabled &&
            (aContract.mValueKeyOrdering_Carry == LoopKeyOrdering::kAfterSBox)) {
            aRecipe.Add(mCarry);
            aHasValueRemainder = true;
        }
        if (aContract.mSecondSourceEnabled &&
            (aContract.mSecondSourceMode == LoopSecondSourceMode::kValue) &&
            (aContract.mValueKeyOrdering_SourceB == LoopKeyOrdering::kAfterSBox)) {
            aRecipe.Add(mSourceByteB);
            aHasValueRemainder = true;
        }
        if (aHasValueRemainder &&
            !EmitRecipeMix(&aLoop, &aStateLocal, &aRecipe, mValue, pErrorString)) {
            return false;
        }

        if (aContract.mCarryEnabled) {
            if (!EmitAssign(&aLoop,
                            &aStateLocal,
                            GTarget::Symbol(mSaltByte),
                            SaltReadExpr(aContract.mCarrySalt,
                                         aContract.mCarrySaltBehavior,
                                         mLoopIndex,
                                         mSourceByteA,
                                         aContract.mSecondSourceEnabled,
                                         mSourceByteB,
                                         mValue,
                                         mCarry,
                                         mOracleCarrySalt,
                                         Random::Get(S_SALT)),
                            &pErrorString)) {
                return false;
            }

            aRecipe.Clear();
            aRecipe.Add(mCarry);
            if (aContract.mCarryKeyOrdering_Value == LoopKeyOrdering::kBeforeSBox) {
                aRecipe.Add(mValue);
            }
            if (aContract.mSecondSourceEnabled &&
                (aContract.mSecondSourceMode == LoopSecondSourceMode::kCarry) &&
                (aContract.mCarryKeyOrdering_SourceB == LoopKeyOrdering::kBeforeSBox)) {
                aRecipe.Add(mSourceByteB);
            }
            if (aContract.mCarryKeyOrdering_Salt == LoopKeyOrdering::kBeforeSBox) {
                aRecipe.Add(mSaltByte);
            }
            if (!EmitRecipeAssign(&aLoop, &aStateLocal, &aRecipe, aCarryKey, pErrorString)) {
                return false;
            }

            switch (aContract.mCarrySBoxBehavior) {
                case LoopSBoxBehavior::kPassSelfThrough:
                    if (!EmitXorAssign(&aLoop,
                                       &aStateLocal,
                                       GTarget::Symbol(mCarry),
                                       GQuick::BufferRead(aContract.mCarrySBox, aCarryKey),
                                       &pErrorString)) {
                        return false;
                    }
                    break;
                case LoopSBoxBehavior::kMixWithLoopIndex:
                    if (!EmitAssign(&aLoop,
                                    &aStateLocal,
                                    GTarget::Symbol(mCarry),
                                    GExpr::Xor(GExpr::Symbol(aCarryKey),
                                               GExpr::ReadSBoxWrap(aContract.mCarrySBox,
                                                                   mLoopIndex,
                                                                   mOracleCarrySBox,
                                                                   Random::Get(S_SBOX))),
                                    &pErrorString)) {
                        return false;
                    }
                    break;
                case LoopSBoxBehavior::kInv:
                default:
                    if (!EmitAssign(&aLoop,
                                    &aStateLocal,
                                    GTarget::Symbol(mCarry),
                                    GExpr::Symbol(aCarryKey),
                                    &pErrorString)) {
                        return false;
                    }
                    break;
            }

            aRecipe.Clear();
            bool aHasCarryRemainder = false;
            if (aContract.mCarryKeyOrdering_Value == LoopKeyOrdering::kAfterSBox) {
                aRecipe.Add(mValue);
                aHasCarryRemainder = true;
            }
            if (aContract.mSecondSourceEnabled &&
                (aContract.mSecondSourceMode == LoopSecondSourceMode::kCarry) &&
                (aContract.mCarryKeyOrdering_SourceB == LoopKeyOrdering::kAfterSBox)) {
                aRecipe.Add(mSourceByteB);
                aHasCarryRemainder = true;
            }
            if (aContract.mCarryKeyOrdering_Salt == LoopKeyOrdering::kAfterSBox) {
                aRecipe.Add(mSaltByte);
                aHasCarryRemainder = true;
            }
            if (aHasCarryRemainder &&
                !EmitRecipeMix(&aLoop, &aStateLocal, &aRecipe, mCarry, pErrorString)) {
                return false;
            }
        }

        if (aContract.mPermuteEnabled) {
            if (!EmitAssign(&aLoop,
                            &aStateLocal,
                            GTarget::Symbol(mSaltByte),
                            SaltReadExpr(aContract.mPermuteSalt,
                                         aContract.mPermuteSaltBehavior,
                                         mLoopIndex,
                                         mSourceByteA,
                                         aContract.mSecondSourceEnabled,
                                         mSourceByteB,
                                         mValue,
                                         mCarry,
                                         mOraclePermuteSalt,
                                         Random::Get(S_SALT)),
                            &pErrorString)) {
                return false;
            }

            aRecipe.Clear();
            int aPermuteKeyTermCount = 0;
            if (aContract.mSecondSourceEnabled &&
                (aContract.mSecondSourceMode == LoopSecondSourceMode::kPermute)) {
                aRecipe.Add(mSourceByteB);
                aPermuteKeyTermCount += 1;
            }
            if (aContract.mCarryEnabled &&
                (aContract.mPermuteKeyOrdering_Carry == LoopKeyOrdering::kBeforeSBox)) {
                aRecipe.Add(aCarryKey);
                aPermuteKeyTermCount += 1;
            }
            if (aContract.mPermuteKeyOrdering_Value == LoopKeyOrdering::kBeforeSBox) {
                aRecipe.Add(mValueKey);
                aPermuteKeyTermCount += 1;
            }
            if (aContract.mPermuteKeyOrdering_Salt == LoopKeyOrdering::kBeforeSBox) {
                aRecipe.Add(mSaltByte);
                aPermuteKeyTermCount += 1;
            }
            if (aPermuteKeyTermCount == 0) {
                if (aContract.mCarryEnabled) {
                    aRecipe.AddExpandable(aCarryKey, 100, true, 100);
                } else {
                    aRecipe.AddExpandable(mValueKey, 100, true, 100);
                }
            } else if (aPermuteKeyTermCount == 1) {
                const GStatementSlot aOnlySlot = aRecipe.mSlots.front().mSlot;
                aRecipe.Clear();
                aRecipe.AddExpandable(aOnlySlot, 100, true, 100);
            }
            if (!EmitRecipeAssign(&aLoop, &aStateLocal, &aRecipe, mPermuteKey, pErrorString)) {
                return false;
            }

            switch (aContract.mPermuteSBoxBehavior) {
                case LoopSBoxBehavior::kPassSelfThrough:
                    if (!EmitAssign(&aLoop,
                                    &aStateLocal,
                                    GTarget::Symbol(mPermute),
                                    GQuick::BufferRead(aContract.mPermuteSBox, mPermuteKey),
                                    &pErrorString)) {
                        return false;
                    }
                    break;
                case LoopSBoxBehavior::kMixWithLoopIndex:
                    if (!EmitAssign(&aLoop,
                                    &aStateLocal,
                                    GTarget::Symbol(mPermute),
                                    GExpr::Xor(GExpr::Symbol(mPermuteKey),
                                               GExpr::ReadSBoxWrap(aContract.mPermuteSBox,
                                                                   mLoopIndex,
                                                                   mOraclePermuteSBox,
                                                                   Random::Get(S_SBOX))),
                                    &pErrorString)) {
                        return false;
                    }
                    break;
                case LoopSBoxBehavior::kInv:
                default:
                    if (!EmitAssign(&aLoop,
                                    &aStateLocal,
                                    GTarget::Symbol(mPermute),
                                    GExpr::Symbol(mPermuteKey),
                                    &pErrorString)) {
                        return false;
                    }
                    break;
            }

            aRecipe.Clear();
            bool aHasPermuteRemainder = false;
            if (aContract.mCarryEnabled &&
                (aContract.mPermuteKeyOrdering_Carry == LoopKeyOrdering::kAfterSBox)) {
                aRecipe.Add(aCarryKey);
                aHasPermuteRemainder = true;
            }
            if (aContract.mPermuteKeyOrdering_Value == LoopKeyOrdering::kAfterSBox) {
                aRecipe.Add(mValueKey);
                aHasPermuteRemainder = true;
            }
            if (aContract.mPermuteKeyOrdering_Salt == LoopKeyOrdering::kAfterSBox) {
                aRecipe.Add(mSaltByte);
                aHasPermuteRemainder = true;
            }
            if (aHasPermuteRemainder &&
                !EmitRecipeMix(&aLoop, &aStateLocal, &aRecipe, mPermute, pErrorString)) {
                return false;
            }

            if (!EmitAssign(&aLoop,
                            &aStateLocal,
                            GTarget::Symbol(mValue),
                            GExpr::RotL8(GExpr::Symbol(mValue), GExpr::Symbol(mPermute)),
                            &pErrorString)) {
                return false;
            }
        }

        if (!EmitAssign(&aLoop,
                        &aStateLocal,
                        GTarget::Write(aContract.mDest, GExpr::Symbol(mLoopIndex)),
                        GExpr::Symbol(mValue),
                        &pErrorString)) {
            return false;
        }

        pResult.push_back(aLoop);
    }

    return true;
}
