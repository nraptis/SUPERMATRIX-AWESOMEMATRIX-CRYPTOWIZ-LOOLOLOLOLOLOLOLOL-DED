//
//  GStatement.cpp
//  MeanMachine
//
//  Created by John Snow on 4/21/26.
//

#include "GStatementRecipe.hpp"
#include "GTermExpander.hpp"
#include "Random.hpp"

#include <algorithm>
#include <cstdint>
#include <sstream>

namespace {

void SetError(std::string *pError,
              const std::string &pMessage) {
    if (pError != NULL) {
        *pError = pMessage;
    }
}

std::string SlotKey(const GStatementSlot &pSlot) {
    if (pSlot.mType == GStatementSlotType::kSymbol) {
        if (pSlot.mSymbol.IsVar()) {
            return "symbol:var:" + pSlot.mSymbol.mName;
        }
        if (pSlot.mSymbol.IsBuf()) {
            return "symbol:buf:" + std::to_string(static_cast<int>(pSlot.mSymbol.mSlot));
        }
        return "symbol:invalid";
    }

    if (pSlot.mType == GStatementSlotType::kExpr) {
        return "expr:" + GExprKey(pSlot.mExpr);
    }

    return "invalid";
}

std::unordered_map<int, int> DefaultOpWeights() {
    std::unordered_map<int, int> aWeights;
    aWeights[static_cast<int>(GOperType::kXor)] = 55U;
    aWeights[static_cast<int>(GOperType::kAdd)] = 45U;
    return aWeights;
}

bool IsSupportedRecipeOp(const GOperType pOper) {
    switch (pOper) {
        case GOperType::kAdd:
        case GOperType::kSub:
        case GOperType::kMul:
        case GOperType::kXor:
        case GOperType::kAnd:
        case GOperType::kRotL8:
            return true;
        default:
            return false;
    }
}

GExpr ApplyRecipeOp(const GOperType pOper,
                    const GExpr &pLeft,
                    const GExpr &pRight) {
    switch (pOper) {
        case GOperType::kAdd: return GExpr::Add(pLeft, pRight);
        case GOperType::kSub: return GExpr::Sub(pLeft, pRight);
        case GOperType::kMul: return GExpr::Mul(pLeft, pRight);
        case GOperType::kXor: return GExpr::Xor(pLeft, pRight);
        case GOperType::kAnd: return GExpr::And(pLeft, pRight);
        case GOperType::kRotL8: return GExpr::RotL8(pLeft, pRight);
        default: return GExpr();
    }
}

bool ShouldIncludeSlot(const int pProbability) {
    return Random::Chance(pProbability);
}

bool SlotKeysEqual(const GStatementSlot &pLeft,
                   const GStatementSlot &pRight) {
    return SlotKey(pLeft) == SlotKey(pRight);
}

struct PoolCandidate {
    GStatementSlot                       mSlot;
    std::string                          mKey;
    int                         mWeight = 0U;
    int                                  mOverdrawnCount = 0;
};

int RandomChoiceInRange(const int pMinimum,
                                  const int pMaximumInclusive) {
    if (pMaximumInclusive <= pMinimum) {
        return pMinimum;
    }
    return pMinimum + Random::Get((pMaximumInclusive - pMinimum) + 1U);
}

int SlotReadCount(const GStatementSlot &pSlot,
                  const GScopeState &pScopeState) {
    if (pSlot.mType == GStatementSlotType::kSymbol) {
        return pScopeState.GetReadCount(pSlot.mSymbol);
    }
    if (pSlot.mType == GStatementSlotType::kExpr) {
        return pScopeState.GetReadCount(pSlot.mExpr);
    }
    return 0;
}

std::vector<PoolCandidate> BuildPoolCandidates(const std::vector<GPoolEntry> &pEntries) {
    std::vector<PoolCandidate> aCandidates;
    for (const GPoolEntry &aEntry : pEntries) {
        if (aEntry.mSlot.IsInvalid()) {
            continue;
        }
        PoolCandidate aCandidate;
        aCandidate.mSlot = aEntry.mSlot;
        aCandidate.mKey = SlotKey(aEntry.mSlot);
        aCandidate.mWeight = aEntry.mWeight;
        aCandidates.push_back(aCandidate);
    }
    return aCandidates;
}

std::size_t ChooseWeightedCandidateIndex(const std::vector<PoolCandidate> &pCandidates) {
    int aPositiveTotal = 0;
    for (const PoolCandidate &aCandidate : pCandidates) {
        if (aCandidate.mWeight > 0) {
            aPositiveTotal += aCandidate.mWeight;
        }
    }

    if (aPositiveTotal > 0) {
        int aPick = Random::Get(static_cast<int>(aPositiveTotal));
        for (std::size_t aIndex = 0U; aIndex < pCandidates.size(); ++aIndex) {
            const int aWeight = pCandidates[aIndex].mWeight;
            if (aWeight <= 0) {
                continue;
            }
            if (aPick < static_cast<int>(aWeight)) {
                return aIndex;
            }
            aPick -= static_cast<int>(aWeight);
        }
    }

    return static_cast<std::size_t>(Random::Get(static_cast<int>(pCandidates.size())));
}

std::size_t ChooseLeastOverdrawnIndex(const std::vector<PoolCandidate> &pCandidates) {
    int aLeastOverdrawn = -1;
    std::vector<PoolCandidate> aLeastOverdrawnCandidates;
    std::vector<std::size_t> aIndexes;

    for (std::size_t aIndex = 0U; aIndex < pCandidates.size(); ++aIndex) {
        const int aOverdrawnCount = pCandidates[aIndex].mOverdrawnCount;
        if ((aLeastOverdrawn < 0) || (aOverdrawnCount < aLeastOverdrawn)) {
            aLeastOverdrawn = aOverdrawnCount;
            aLeastOverdrawnCandidates.clear();
            aIndexes.clear();
        }
        if (aOverdrawnCount == aLeastOverdrawn) {
            aLeastOverdrawnCandidates.push_back(pCandidates[aIndex]);
            aIndexes.push_back(aIndex);
        }
    }

    return aIndexes[ChooseWeightedCandidateIndex(aLeastOverdrawnCandidates)];
}

bool RemoveFirstMatchingEntry(std::vector<GPoolEntry> *pEntries,
                              const GStatementSlot &pRequiredSlot) {
    if (pEntries == NULL) {
        return false;
    }

    for (auto aIterator = pEntries->begin(); aIterator != pEntries->end(); ++aIterator) {
        if (SlotKeysEqual(aIterator->mSlot, pRequiredSlot)) {
            pEntries->erase(aIterator);
            return true;
        }
    }

    return false;
}

} // namespace

GStatementSlot::GStatementSlot() {
    Invalidate();
}

GStatementSlot GStatementSlot::Symbol(const GSymbol &pSymbol) {
    GStatementSlot aSlot;
    aSlot.mType = GStatementSlotType::kSymbol;
    aSlot.mSymbol = pSymbol;
    return aSlot;
}

GStatementSlot GStatementSlot::Expr(const GExpr &pExpr) {
    GStatementSlot aSlot;
    aSlot.mType = GStatementSlotType::kExpr;
    aSlot.mExpr = pExpr;
    return aSlot;
}

void GStatementSlot::Invalidate() {
    mType = GStatementSlotType::kInv;
    mSymbol.Invalidate();
    mExpr.Invalidate();
}

bool GStatementSlot::IsInvalid() const {
    if (mType == GStatementSlotType::kSymbol) {
        return mSymbol.IsInvalid();
    }
    if (mType == GStatementSlotType::kExpr) {
        return mExpr.IsInvalid();
    }
    return true;
}

GExpr GStatementSlot::ToExpr() const {
    if (mType == GStatementSlotType::kSymbol) {
        return GExpr::Symbol(mSymbol);
    }
    if (mType == GStatementSlotType::kExpr) {
        return mExpr;
    }
    return GExpr();
}

GPool::GPool(GScopeRules *pScopeRules) {
    mScopeRules = pScopeRules;    
}

static void AddSlotCopies(std::unordered_map<std::string, std::vector<GPoolEntry>> *pCategories,
                          const GStatementSlot &pSlot,
                          const std::string &pCategory,
                          const int pWeight,
                          const int pDuplicates);

static int RuleMaximumForSlot(const GStatementSlot &pSlot,
                              const GScopeRules *pScopeRules,
                              const int pDefaultMaximum) {
    if (pScopeRules == NULL) {
        return pDefaultMaximum;
    }

    if (pSlot.mType == GStatementSlotType::kSymbol) {
        const int aRuleMaximum = pScopeRules->GetReadPreferredMaximum(pSlot.mSymbol);
        return (aRuleMaximum > 0) ? aRuleMaximum : pDefaultMaximum;
    }

    return pDefaultMaximum;
}

void GPool::Add(GSymbol pSymbol, std::string pCategory, int pWeight, int pDuplicates) {
    AddSlotCopies(&mCategories, GStatementSlot::Symbol(pSymbol), pCategory, pWeight, pDuplicates);
}

void GPool::Add(GExpr pExpr, std::string pCategory, int pWeight, int pDuplicates) {
    AddSlotCopies(&mCategories, GStatementSlot::Expr(pExpr), pCategory, pWeight, pDuplicates);
}

static void AddSlotCopies(std::unordered_map<std::string, std::vector<GPoolEntry>> *pCategories,
                          const GStatementSlot &pSlot,
                          const std::string &pCategory,
                          const int pWeight,
                          const int pDuplicates) {
    if (pCategories == NULL || pSlot.IsInvalid() || pCategory.empty()) {
        return;
    }

    const int aCount = std::max(1, pDuplicates);
    std::vector<GPoolEntry> &aSlots = (*pCategories)[pCategory];
    for (int aIndex = 0; aIndex < aCount; ++aIndex) {
        GPoolEntry aEntry;
        aEntry.mSlot = pSlot;
        aEntry.mWeight = pWeight;
        aSlots.push_back(aEntry);
    }
}

int GPool::Count(std::string pCategory) const {
    const auto aIterator = mCategories.find(pCategory);
    if (aIterator == mCategories.end()) {
        return 0;
    }
    return static_cast<int>(BuildPoolCandidates(aIterator->second).size());
}

std::vector<std::string> GPool::GetCategories() const {
    std::vector<std::string> aCategories;
    for (const auto &aPair : mCategories) {
        aCategories.push_back(aPair.first);
    }
    std::sort(aCategories.begin(), aCategories.end());
    return aCategories;
}

static bool FetchSlotsFromEntries(const std::vector<GPoolEntry> &pEntries,
                                  const std::string &pCategory,
                                  int pMinimumCount,
                                  int pMaximumCount,
                                  std::vector<GStatementSlot> &pResult,
                                  const GScopeRules *pScopeRules,
                                  GScopeState &pScopeStateLocal,
                                  GScopeState &pScopeStateGlobal,
                                  const std::unordered_map<std::string, int> &pPreselectedCounts,
                                  std::string &pError) {
    pResult.clear();

    if ((pMinimumCount < 0) || (pMaximumCount < 0)) {
        SetError(&pError, "Pool fetch count cannot be negative.");
        return false;
    }

    if (pMaximumCount < pMinimumCount) {
        SetError(&pError, "Pool fetch maximum count was below the minimum count.");
        return false;
    }

    if (pMaximumCount == 0) {
        return true;
    }

    if (pEntries.empty()) {
        SetError(&pError, "Pool category was empty: " + pCategory);
        return false;
    }

    std::vector<PoolCandidate> aAllCandidates = BuildPoolCandidates(pEntries);
    if (static_cast<int>(aAllCandidates.size()) < pMinimumCount) {
        SetError(&pError, "Pool could not achieve the requested minimum slot count in category: " + pCategory);
        return false;
    }

    const int aTargetCount = static_cast<int>(
        RandomChoiceInRange(static_cast<int>(pMinimumCount),
                            static_cast<int>(pMaximumCount))
    );

    std::vector<PoolCandidate> aGoodCandidates;
    std::vector<PoolCandidate> aOverdrawnCandidates;
    std::unordered_map<std::string, std::vector<PoolCandidate>> aGroups;
    for (const PoolCandidate &aCandidate : aAllCandidates) {
        aGroups[aCandidate.mKey].push_back(aCandidate);
    }

    for (auto &aPair : aGroups) {
        std::vector<PoolCandidate> &aGroup = aPair.second;
        if (aGroup.empty()) {
            continue;
        }

        std::stable_sort(aGroup.begin(),
                         aGroup.end(),
                         [](const PoolCandidate &pLeft, const PoolCandidate &pRight) {
                             return pLeft.mWeight > pRight.mWeight;
                         });

        const PoolCandidate &aLeadCandidate = aGroup.front();
        const int aLocalReadCount = SlotReadCount(aLeadCandidate.mSlot, pScopeStateLocal);
        const int aGlobalReadCount = SlotReadCount(aLeadCandidate.mSlot, pScopeStateGlobal);
        const auto aPreselectedIterator = pPreselectedCounts.find(aLeadCandidate.mKey);
        const int aPreselectedCount = (aPreselectedIterator == pPreselectedCounts.end()) ? 0 : aPreselectedIterator->second;
        const int aDefaultMaximum = static_cast<int>(aGroup.size()) + aPreselectedCount;
        const int aRuleMaximum = RuleMaximumForSlot(aLeadCandidate.mSlot, pScopeRules, aDefaultMaximum);
        const int aRangeMaximum = std::max(0, std::min(aDefaultMaximum, aRuleMaximum));
        const int aPhysicalRemaining = std::max(0, static_cast<int>(aGroup.size()) - aLocalReadCount);
        const int aHistoricalDrawCount = aLocalReadCount + aGlobalReadCount + aPreselectedCount;
        const int aSoftRemaining = std::max(0, aRangeMaximum - aHistoricalDrawCount);
        const int aGoodCount = std::min(aPhysicalRemaining, aSoftRemaining);
        const int aOverdrawBase = std::max(0, aHistoricalDrawCount - aRangeMaximum);
        const bool aZeroDrawn = (aHistoricalDrawCount <= 0);

        for (int aIndex = 0; aIndex < static_cast<int>(aGroup.size()); ++aIndex) {
            if (aIndex >= aPhysicalRemaining) {
                break;
            }

            PoolCandidate aCandidate = aGroup[static_cast<std::size_t>(aIndex)];
            if (aIndex < aGoodCount) {
                if (aZeroDrawn && aCandidate.mWeight > 0) {
                    aCandidate.mWeight *= 2;
                }
                aGoodCandidates.push_back(aCandidate);
            } else {
                aCandidate.mOverdrawnCount = aOverdrawBase + ((aIndex - aGoodCount) + 1);
                aOverdrawnCandidates.push_back(aCandidate);
            }
        }
    }

    while ((static_cast<int>(pResult.size()) < aTargetCount) && !aGoodCandidates.empty()) {
        const std::size_t aChosenIndex = ChooseWeightedCandidateIndex(aGoodCandidates);
        pResult.push_back(aGoodCandidates[aChosenIndex].mSlot);
        aGoodCandidates.erase(aGoodCandidates.begin() + static_cast<std::ptrdiff_t>(aChosenIndex));
    }

    if (static_cast<int>(pResult.size()) >= pMinimumCount) {
        return true;
    }

    while ((static_cast<int>(pResult.size()) < pMinimumCount) && !aOverdrawnCandidates.empty()) {
        const std::size_t aChosenIndex = ChooseLeastOverdrawnIndex(aOverdrawnCandidates);
        pResult.push_back(aOverdrawnCandidates[aChosenIndex].mSlot);
        aOverdrawnCandidates.erase(aOverdrawnCandidates.begin() + static_cast<std::ptrdiff_t>(aChosenIndex));
    }

    if (static_cast<int>(pResult.size()) < pMinimumCount) {
        SetError(&pError, "Pool could not achieve the requested minimum slot count in category: " + pCategory);
        return false;
    }

    return true;
}

bool GPool::FetchSlots(std::string pCategory,
                       int pMinimumCount,
                       int pMaximumCount,
                       std::vector<GStatementSlot> &pResult,
                       GScopeState &pScopeStateLocal,
                       GScopeState &pScopeStateGlobal,
                       std::string &pError) {
    const auto aIterator = mCategories.find(pCategory);
    if (aIterator == mCategories.end()) {
        pResult.clear();
        SetError(&pError, "Pool category was empty: " + pCategory);
        return false;
    }

    return FetchSlotsFromEntries(aIterator->second,
                                 pCategory,
                                 pMinimumCount,
                                 pMaximumCount,
                                 pResult,
                                 mScopeRules,
                                 pScopeStateLocal,
                                 pScopeStateGlobal,
                                 std::unordered_map<std::string, int>(),
                                 pError);
}

bool GPool::FetchSlots(std::string pCategory,
                       int pCount,
                       std::vector<GStatementSlot> &pResult,
                       GScopeState &pScopeStateLocal,
                       GScopeState &pScopeStateGlobal,
                       std::string &pError) {
    return FetchSlots(pCategory, pCount, pCount, pResult, pScopeStateLocal, pScopeStateGlobal, pError);
}

bool GPool::FetchSlots(std::string pCategory,
                       int pMinimumCount,
                       int pMaximumCount,
                       GExpr pRequiredExpr,
                       std::vector<GStatementSlot> &pResult,
                       GScopeState &pScopeStateLocal,
                       GScopeState &pScopeStateGlobal,
                       std::string &pError) {
    const GStatementSlot aRequiredSlot = GStatementSlot::Expr(pRequiredExpr);
    if (aRequiredSlot.IsInvalid()) {
        SetError(&pError, "Required pool expression was invalid.");
        return false;
    }
    if (pMaximumCount <= 0) {
        pResult.clear();
        return (pMinimumCount == 0);
    }

    const auto aIterator = mCategories.find(pCategory);
    std::vector<GPoolEntry> aEntries = (aIterator != mCategories.end()) ? aIterator->second : std::vector<GPoolEntry>();
    const bool aDidReserveRequired = RemoveFirstMatchingEntry(&aEntries, aRequiredSlot);
    if (!aDidReserveRequired) {
        SetError(&pError, "Required pool expression was not present in category: " + pCategory);
        return false;
    }

    std::unordered_map<std::string, int> aPreselectedCounts;
    aPreselectedCounts[SlotKey(aRequiredSlot)] = 1;
    if (!FetchSlotsFromEntries(aEntries,
                               pCategory,
                               std::max(0, pMinimumCount - 1),
                               std::max(0, pMaximumCount - 1),
                               pResult,
                               mScopeRules,
                               pScopeStateLocal,
                               pScopeStateGlobal,
                               aPreselectedCounts,
                               pError)) {
        return false;
    }

    pResult.insert(pResult.begin(), aRequiredSlot);
    return static_cast<int>(pResult.size()) >= pMinimumCount;
}

bool GPool::FetchSlots(std::string pCategory,
                       int pCount,
                       GExpr pRequiredExpr,
                       std::vector<GStatementSlot> &pResult,
                       GScopeState &pScopeStateLocal,
                       GScopeState &pScopeStateGlobal,
                       std::string &pError) {
    return FetchSlots(pCategory, pCount, pCount, pRequiredExpr, pResult, pScopeStateLocal, pScopeStateGlobal, pError);
}

bool GPool::FetchSlots(std::string pCategory,
                       int pMinimumCount,
                       int pMaximumCount,
                       GSymbol pRequiredSymbol,
                       std::vector<GStatementSlot> &pResult,
                       GScopeState &pScopeStateLocal,
                       GScopeState &pScopeStateGlobal,
                       std::string &pError) {
    const GStatementSlot aRequiredSlot = GStatementSlot::Symbol(pRequiredSymbol);
    if (aRequiredSlot.IsInvalid()) {
        SetError(&pError, "Required pool symbol was invalid.");
        return false;
    }
    if (pMaximumCount <= 0) {
        pResult.clear();
        return (pMinimumCount == 0);
    }

    const auto aIterator = mCategories.find(pCategory);
    std::vector<GPoolEntry> aEntries = (aIterator != mCategories.end()) ? aIterator->second : std::vector<GPoolEntry>();
    const bool aDidReserveRequired = RemoveFirstMatchingEntry(&aEntries, aRequiredSlot);
    if (!aDidReserveRequired) {
        SetError(&pError, "Required pool symbol was not present in category: " + pCategory);
        return false;
    }

    std::unordered_map<std::string, int> aPreselectedCounts;
    aPreselectedCounts[SlotKey(aRequiredSlot)] = 1;
    if (!FetchSlotsFromEntries(aEntries,
                               pCategory,
                               std::max(0, pMinimumCount - 1),
                               std::max(0, pMaximumCount - 1),
                               pResult,
                               mScopeRules,
                               pScopeStateLocal,
                               pScopeStateGlobal,
                               aPreselectedCounts,
                               pError)) {
        return false;
    }

    pResult.insert(pResult.begin(), aRequiredSlot);
    return static_cast<int>(pResult.size()) >= pMinimumCount;
}

bool GPool::FetchSlots(std::string pCategory,
                       int pCount,
                       GSymbol pRequiredSymbol,
                       std::vector<GStatementSlot> &pResult,
                       GScopeState &pScopeStateLocal,
                       GScopeState &pScopeStateGlobal,
                       std::string &pError) {
    return FetchSlots(pCategory, pCount, pCount, pRequiredSymbol, pResult, pScopeStateLocal, pScopeStateGlobal, pError);
}

void GStatementRecipe::Clear() {
    mSlots.clear();
    mOpWeights.clear();
    mOpPairDisallows.clear();
}

void GStatementRecipe::Add(GSymbol pSymbol, int pProbability) {
    Add(GStatementSlot::Symbol(pSymbol), pProbability);
}

void GStatementRecipe::Add(GExpr pExpr, int pProbability) {
    Add(GStatementSlot::Expr(pExpr), pProbability);
}

void GStatementRecipe::Add(GStatementSlot pSlot, int pProbability) {
    if (pSlot.IsInvalid()) {
        return;
    }
    GWeightedSlot aEntry;
    aEntry.mSlot = pSlot;
    aEntry.mProbability = pProbability;
    mSlots.push_back(aEntry);
}

void GStatementRecipe::Add(GSymbol pSymbol) {
    Add(pSymbol, 100);
}

void GStatementRecipe::Add(GExpr pExpr) {
    Add(pExpr, 100);
}

void GStatementRecipe::Add(GStatementSlot pSlot) {
    Add(pSlot, 100);
}

void GStatementRecipe::ResetOpWeights(GSymbol pSymbol) {
    ResetOpWeights(GStatementSlot::Symbol(pSymbol));
}

void GStatementRecipe::ResetOpWeights(GExpr pExpr) {
    ResetOpWeights(GStatementSlot::Expr(pExpr));
}

void GStatementRecipe::ResetOpWeights(GStatementSlot pSlot) {
    mOpWeights[SlotKey(pSlot)] = DefaultOpWeights();
}

void GStatementRecipe::ClearOpWeights(GSymbol pSymbol) {
    ClearOpWeights(GStatementSlot::Symbol(pSymbol));
}

void GStatementRecipe::ClearOpWeights(GExpr pExpr) {
    ClearOpWeights(GStatementSlot::Expr(pExpr));
}

void GStatementRecipe::ClearOpWeights(GStatementSlot pSlot) {
    mOpWeights[SlotKey(pSlot)].clear();
}

void GStatementRecipe::SetOpWeight(GSymbol pSymbol, GOperType pOp, int pWeight) {
    SetOpWeight(GStatementSlot::Symbol(pSymbol), pOp, pWeight);
}

void GStatementRecipe::SetOpWeight(GExpr pExpr, GOperType pOp, int pWeight) {
    SetOpWeight(GStatementSlot::Expr(pExpr), pOp, pWeight);
}

void GStatementRecipe::SetOpWeight(GStatementSlot pSlot, GOperType pOp, int pWeight) {
    std::unordered_map<int, int> &aWeights = mOpWeights[SlotKey(pSlot)];
    aWeights[static_cast<int>(pOp)] = pWeight;
}

void GStatementRecipe::SetOpPairDisallow(GSymbol pSymbolA, GSymbol pSymbolB, GOperType pOp) {
    SetOpPairDisallow(GStatementSlot::Symbol(pSymbolA), GStatementSlot::Symbol(pSymbolB), pOp);
}

void GStatementRecipe::SetOpPairDisallow(GSymbol pSymbolA, GExpr pExprB, GOperType pOp) {
    SetOpPairDisallow(GStatementSlot::Symbol(pSymbolA), GStatementSlot::Expr(pExprB), pOp);
}

void GStatementRecipe::SetOpPairDisallow(GSymbol pSymbolA, GStatementSlot pSlotB, GOperType pOp) {
    SetOpPairDisallow(GStatementSlot::Symbol(pSymbolA), pSlotB, pOp);
}

void GStatementRecipe::SetOpPairDisallow(GExpr pExprA, GSymbol pSymbolB, GOperType pOp) {
    SetOpPairDisallow(GStatementSlot::Expr(pExprA), GStatementSlot::Symbol(pSymbolB), pOp);
}

void GStatementRecipe::SetOpPairDisallow(GExpr pExprA, GStatementSlot pSlotB, GOperType pOp) {
    SetOpPairDisallow(GStatementSlot::Expr(pExprA), pSlotB, pOp);
}

void GStatementRecipe::SetOpPairDisallow(GStatementSlot pSlotA, GStatementSlot pSlotB, GOperType pOp) {
    GOpPairRule aRule;
    aRule.mSlotA = pSlotA;
    aRule.mSlotB = pSlotB;
    aRule.mOp = pOp;
    mOpPairDisallows.push_back(aRule);
}

static bool IsDisallowed(const std::vector<GStatementRecipe::GOpPairRule> &pRules,
                         const GStatementSlot &pLeft,
                         const GStatementSlot &pRight,
                         const GOperType pOp) {
    for (const GStatementRecipe::GOpPairRule &aRule : pRules) {
        if (aRule.mOp == pOp &&
            SlotKeysEqual(aRule.mSlotA, pLeft) &&
            SlotKeysEqual(aRule.mSlotB, pRight)) {
            return true;
        }
    }
    return false;
}

static std::unordered_map<int, int> EffectiveOpWeights(
    const std::unordered_map<std::string, std::unordered_map<int, int>> &pOpWeights,
    const GStatementSlot &pSlot) {
    const auto aIterator = pOpWeights.find(SlotKey(pSlot));
    if (aIterator != pOpWeights.end()) {
        return aIterator->second;
    }
    return DefaultOpWeights();
}

static bool ChooseOp(const GStatementSlot &pLeft,
                     const GStatementSlot &pRight,
                     const std::unordered_map<std::string, std::unordered_map<int, int>> &pOpWeights,
                     const std::vector<GStatementRecipe::GOpPairRule> &pPairRules,
                     GOperType *pOp,
                     std::string *pError) {
    if (pOp == NULL) {
        SetError(pError, "Output operator was null.");
        return false;
    }

    const std::unordered_map<int, int> aWeights = EffectiveOpWeights(pOpWeights, pRight);
    std::vector<std::pair<GOperType, int>> aCandidates;
    int aTotalWeight = 0;
    for (const auto &aPair : aWeights) {
        const GOperType aOper = static_cast<GOperType>(aPair.first);
        const int aWeight = aPair.second;
        if (!IsSupportedRecipeOp(aOper) || aWeight <= 0) {
            continue;
        }
        if (IsDisallowed(pPairRules, pLeft, pRight, aOper)) {
            continue;
        }
        aCandidates.push_back(std::make_pair(aOper, aWeight));
        aTotalWeight += aWeight;
    }

    if (aCandidates.empty() || aTotalWeight <= 0) {
        SetError(pError, "No legal operator remained for the selected slot pair.");
        return false;
    }

    int aPick = Random::Get(static_cast<int>(aTotalWeight));
    for (const auto &aCandidate : aCandidates) {
        if (aPick < static_cast<int>(aCandidate.second)) {
            *pOp = aCandidate.first;
            return true;
        }
        aPick -= static_cast<int>(aCandidate.second);
    }

    *pOp = aCandidates.back().first;
    return true;
}

static bool BakeExpression(const std::vector<GStatementRecipe::GWeightedSlot> &pSlots,
                           const std::unordered_map<std::string, std::unordered_map<int, int>> &pOpWeights,
                           const std::vector<GStatementRecipe::GOpPairRule> &pPairRules,
                           GExpr *pExpr,
                           std::string *pError) {
    if (pExpr == NULL) {
        SetError(pError, "Output expression was null.");
        return false;
    }

    std::vector<GStatementSlot> aSelectedSlots;
    for (const GStatementRecipe::GWeightedSlot &aWeightedSlot : pSlots) {
        if (ShouldIncludeSlot(aWeightedSlot.mProbability)) {
            aSelectedSlots.push_back(aWeightedSlot.mSlot);
        }
    }

    if (aSelectedSlots.empty() && !pSlots.empty()) {
        aSelectedSlots.push_back(pSlots.front().mSlot);
    }

    if (aSelectedSlots.empty()) {
        SetError(pError, "Statement recipe had no slots to bake.");
        return false;
    }

    GExpr aExpr = aSelectedSlots[0].ToExpr();
    if (aExpr.IsInvalid()) {
        SetError(pError, "The first baked slot was invalid.");
        return false;
    }

    for (std::size_t aIndex = 1U; aIndex < aSelectedSlots.size(); ++aIndex) {
        GOperType aOper = GOperType::kInv;
        if (!ChooseOp(aSelectedSlots[aIndex - 1U],
                      aSelectedSlots[aIndex],
                      pOpWeights,
                      pPairRules,
                      &aOper,
                      pError)) {
            return false;
        }

        GExpr aRightExpr = aSelectedSlots[aIndex].ToExpr();
        aExpr = ApplyRecipeOp(aOper, aExpr, aRightExpr);
        if (aExpr.IsInvalid()) {
            SetError(pError, "Applying the chosen operator produced an invalid expression.");
            return false;
        }
    }

    *pExpr = aExpr;
    return true;
}

bool GStatementRecipe::Bake(GSymbol pSymbol,
                            GAssignType pAssignType,
                            GStatement &pResult,
                            std::string &pError) {
    GExpr aExpr;
    if (!BakeExpression(mSlots, mOpWeights, mOpPairDisallows, &aExpr, &pError)) {
        return false;
    }

    switch (pAssignType) {
        case GAssignType::kInvalid:
            return false;
        case GAssignType::kSet:
            pResult = GStatement::Assign(GTarget::Symbol(pSymbol), aExpr);
            return !pResult.IsInvalid();
        case GAssignType::kAddAssign:
            pResult = GStatement::AddAssign(GTarget::Symbol(pSymbol), aExpr);
            return !pResult.IsInvalid();
        case GAssignType::kXorAssign:
            pResult = GStatement::XorAssign(GTarget::Symbol(pSymbol), aExpr);
            return !pResult.IsInvalid();
    }
}

bool GStatementRecipe::Bake(GExpr pExpr,
                            GAssignType pAssignType,
                            GStatement &pResult,
                            std::string &pError) {
    if (pExpr.IsSymbol()) {
        return Bake(pExpr.mSymbol, pAssignType, pResult, pError);
    }
    
    if (!pExpr.IsRead() || pExpr.mIndex == nullptr) {
        SetError(&pError, "Recipe bake target expression must be a symbol or read.");
        return false;
    }
    
    GExpr aBakedExpr;
    if (!BakeExpression(mSlots, mOpWeights, mOpPairDisallows, &aBakedExpr, &pError)) {
        return false;
    }
    switch (pAssignType) {
        case GAssignType::kInvalid:
            return false;
        case GAssignType::kSet:
            pResult = GStatement::Assign(GTarget::Write(pExpr.mSymbol, *pExpr.mIndex), aBakedExpr);
            return !pResult.IsInvalid();
        case GAssignType::kAddAssign:
            pResult = GStatement::AddAssign(GTarget::Write(pExpr.mSymbol, *pExpr.mIndex), aBakedExpr);
            return !pResult.IsInvalid();
        case GAssignType::kXorAssign:
            pResult = GStatement::XorAssign(GTarget::Write(pExpr.mSymbol, *pExpr.mIndex), aBakedExpr);
            return !pResult.IsInvalid();
    }
}

bool GStatementRecipe::Bake(GSymbol pSymbol, GStatement &pResult, std::string &pError) {
    return Bake(pSymbol, GAssignType::kSet, pResult, pError);
}

bool GStatementRecipe::Bake(GExpr pExpr, GStatement &pResult, std::string &pError) {
    return Bake(pExpr, GAssignType::kSet, pResult, pError);
}

bool GStatementRecipe::BakeMix(GSymbol pSymbol, GStatement &pResult, std::string &pError) {
    if (Random::Bool()) {
        return Bake(pSymbol, GAssignType::kAddAssign, pResult, pError);
    } else {
        return Bake(pSymbol, GAssignType::kXorAssign, pResult, pError);
    }
    
}

bool GStatementRecipe::BakeMix(GExpr pExpr, GStatement &pResult, std::string &pError) {
    if (Random::Bool()) {
        return Bake(pExpr, GAssignType::kAddAssign, pResult, pError);
    } else {
        return Bake(pExpr, GAssignType::kXorAssign, pResult, pError);
    }
}


void GStatementRecipe::AddExpandable(GSymbol pSymbol, int pExpandProbability,
                                     bool pAllowMultiply,
                                     int pProbability) {
    if (pSymbol.IsInvalid()) {
        return;
    }
    Add(GTermExpander::Expand(pSymbol, pAllowMultiply), pProbability);
}

void GStatementRecipe::AddExpandable(GExpr pExpr, int pExpandProbability,
                                     bool pAllowMultiply,
                                     int pProbability) {
    if (pExpr.IsInvalid()) {
        return;
    }
    
    if (Random::Get(100) < pProbability) {
        Add(GTermExpander::Expand(pExpr, pAllowMultiply), pProbability);
    } else {
        Add(pExpr, pProbability);
    }
}

void GStatementRecipe::AddExpandable(GStatementSlot pSlot, int pExpandProbability,
                                     bool pAllowMultiply,
                                     int pProbability) {
    if (pSlot.IsInvalid()) {
        return;
    }
    if (pSlot.mType == GStatementSlotType::kSymbol) {
        AddExpandable(pSlot.mSymbol, pExpandProbability, pAllowMultiply, pProbability);
        return;
    }
    if (pSlot.mType == GStatementSlotType::kExpr) {
        AddExpandable(pSlot.mExpr, pExpandProbability, pAllowMultiply, pProbability);
    }
}

void GStatementRecipe::AddExpandable(GSymbol pSymbol,
                                                  int pExpandProbability,
                                     bool pAllowMultiply) {
    AddExpandable(pSymbol, pExpandProbability, pAllowMultiply, 100);
}

void GStatementRecipe::AddExpandable(GExpr pExpr,
                                                  int pExpandProbability,
                                     bool pAllowMultiply) {
    AddExpandable(pExpr, pExpandProbability, pAllowMultiply, 100);
}

void GStatementRecipe::AddExpandable(GStatementSlot pSlot,
                                                  int pExpandProbability,
                                     bool pAllowMultiply) {
    AddExpandable(pSlot, pExpandProbability, pAllowMultiply, 100);
}

void GStatementRecipe::AddExpandable(GSymbol pSymbol,
                                     int pExpandProbability) {
    AddExpandable(pSymbol, pExpandProbability, false, 100);
}

void GStatementRecipe::AddExpandable(GExpr pExpr,
                                     int pExpandProbability) {
    AddExpandable(pExpr, pExpandProbability, false, 100);
}

void GStatementRecipe::AddExpandable(GStatementSlot pSlot,
                                     int pExpandProbability) {
    AddExpandable(pSlot, pExpandProbability, false, 100);
}
