//
//  GStatement.hpp
//  MeanMachine
//
//  Created by John Snow on 4/21/26.
//

#ifndef GStatement_hpp
#define GStatement_hpp

#include "TwistWorkSpace.hpp"
#include "GSymbol.hpp"
#include "GExpr.hpp"
#include "GSeedProgram.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>


enum class GStatementSlotType : int {
    kInv = 0,
    kSymbol = 1,
    kExpr = 2
};

struct GStatementSlot {
    GSymbol                                     mSymbol;
    GExpr                                       mExpr;
    GStatementSlotType                          mType;
    
    GStatementSlot();

    static GStatementSlot                       Symbol(const GSymbol &pSymbol);
    static GStatementSlot                       Expr(const GExpr &pExpr);

    void                                        Invalidate();
    bool                                        IsInvalid() const;
    GExpr                                       ToExpr() const;
};

struct GPoolEntry {
    GStatementSlot                              mSlot;
    int                                mWeight = 0U;
};


struct GPool {
    
public:
    GPool(GScopeRules *pScopeRules);
    
    GScopeRules                                                 *mScopeRules;
    
    std::unordered_map<std::string, std::vector<GPoolEntry>>    mCategories;
    
    // These are
    
    // void                        SetReadPreferredMinimum(GSymbol pSymbol, int pCount) { }
    // void                        SetReadPreferredMaximum(GSymbol pSymbol, int pCount) { }
    // void                        SetAvailable(GSymbol pSymbol, std::string pCategory) { }
    
    /*
     
     // Option 1:
     value = (carry_a + 1) ^ (state * 3);
     value += (carry_b + 117);
     
     // Option 2:
     value = (carry_b + 31) ^ (state rotl 3);
     value += (carry_a * 27);
     
     */
    
    // pDuplicates is the per-fetch local inventory for this slot inside the pool category.
    // Local scope consumes that inventory physically; global scope only pushes later copies into overdraw.
    void                                        Add(GSymbol pSymbol, std::string pCategory, int pWeight=0U, int pDuplicates=0);
    void                                        Add(GExpr pExpr, std::string pCategory, int pWeight=0U, int pDuplicates=0);
    
    bool                                        FetchSlots(std::string pCategory,
                                                           int pMinimumCount,
                                                           int pMaximumCount,
                                                           GExpr pRequiredExpr,
                                                           std::vector<GStatementSlot> &pResult,
                                                           GScopeState &pScopeStateLocal,
                                                           GScopeState &pScopeStateGlobal,
                                                           std::string &pError);
    
    // this can make a copy of pScopeStateLocal to update, but it should not update the pScopeStateLocal which was passed in
    // we must include the expression, order random
    bool                                        FetchSlots(std::string pCategory,
                                                           int pCount,
                                                           GExpr pRequiredExpr,
                                                           std::vector<GStatementSlot> &pResult,
                                                           GScopeState &pScopeStateLocal,
                                                           GScopeState &pScopeStateGlobal,
                                                           std::string &pError);
    
    bool                                        FetchSlots(std::string pCategory,
                                                           int pMinimumCount,
                                                           int pMaximumCount,
                                                           GSymbol pRequiredSymbol,
                                                           std::vector<GStatementSlot> &pResult,
                                                           GScopeState &pScopeStateLocal,
                                                           GScopeState &pScopeStateGlobal,
                                                           std::string &pError);
    
    // this can make a copy of pScopeStateLocal to update, but it should not update the pScopeStateLocal which was passed in
    // we must include the symbol, order random
    bool                                        FetchSlots(std::string pCategory,
                                                           int pCount,
                                                           GSymbol pRequiredSymbol,
                                                           std::vector<GStatementSlot> &pResult,
                                                           GScopeState &pScopeStateLocal,
                                                           GScopeState &pScopeStateGlobal,
                                                           
                                                           std::string &pError);
    
    bool                                        FetchSlots(std::string pCategory,
                                                           int pMinimumCount,
                                                           int pMaximumCount,
                                                           std::vector<GStatementSlot> &pResult,
                                                           GScopeState &pScopeStateLocal,
                                                           GScopeState &pScopeStateGlobal,
                                                           std::string &pError);
    
    // this can make a copy of pScopeStateLocal to update, but it should not update the pScopeStateLocal which was passed in
    bool                                        FetchSlots(std::string pCategory,
                                                           int pCount,
                                                           std::vector<GStatementSlot> &pResult,
                                                           GScopeState &pScopeStateLocal,
                                                           GScopeState &pScopeStateGlobal,
                                                           std::string &pError);
    
    int                                         Count(std::string pCategory) const;
    
    std::vector<std::string>                    GetCategories() const;
    
};

struct GStatementRecipe {
    struct GWeightedSlot {
        GStatementSlot                  mSlot;
        int                             mProbability = 0U;
    };

    struct GOpPairRule {
        GStatementSlot                  mSlotA;
        GStatementSlot                  mSlotB;
        GOperType                       mOp = GOperType::kInv;
    };
    
    // We cannot mix and match GExpr::Symbol(a) and a
    std::vector<GWeightedSlot>                  mSlots;
    std::unordered_map<std::string, std::unordered_map<int, int>> mOpWeights;
    std::vector<GOpPairRule>                    mOpPairDisallows;
    
    void                                Clear();
    
    void                                Add(GSymbol pSymbol, int pProbability);
    void                                Add(GExpr pExpr, int pProbability);
    void                                Add(GStatementSlot pSlot, int pProbability);
    
    void                                Add(GSymbol pSymbol);
    void                                Add(GExpr pExpr);
    void                                Add(GStatementSlot pSlot);
    
    
    
    // the higher the weight, the more likely to be chosen. the weights are relative to each-other, not on any fixed scale
    void                                ResetOpWeights(GSymbol pSymbol); // the default, kXor 55, kAdd 45
    void                                ResetOpWeights(GExpr pExpr);
    void                                ResetOpWeights(GStatementSlot pSlot);
    
    void                                ClearOpWeights(GSymbol pSymbol);
    void                                ClearOpWeights(GExpr pExpr);
    void                                ClearOpWeights(GStatementSlot pSlot);
    
    void                                AddExpandable(GSymbol pSymbol,
                                                      int pExpandProbability,
                                                      bool pAllowMultiply,
                                                      int pProbability);
    void                                AddExpandable(GExpr pExpr,
                                                      int pExpandProbability,
                                                      bool pAllowMultiply,
                                                      int pProbability);
    void                                AddExpandable(GStatementSlot pSlot,
                                                      int pExpandProbability,
                                                      bool pAllowMultiply,
                                                      int pProbability);
    
    void                                AddExpandable(GSymbol pSymbol,
                                                      int pExpandProbability,
                                                      bool pAllowMultiply);
    void                                AddExpandable(GExpr pExpr,
                                                      int pExpandProbability,
                                                      bool pAllowMultiply);
    void                                AddExpandable(GStatementSlot pSlot,
                                                      int pExpandProbability,
                                                      bool pAllowMultiply);
    
    void                                AddExpandable(GSymbol pSymbol,
                                                      int pExpandProbability);
    void                                AddExpandable(GExpr pExpr,
                                                      int pExpandProbability);
    void                                AddExpandable(GStatementSlot pSlot,
                                                      int pExpandProbability);
    
    
    
    // This is the incoming operator for this slot when it is appended after the prior slot.
    void                                SetOpWeight(GSymbol pSymbol, GOperType pOp, int pWeight);
    void                                SetOpWeight(GExpr pExpr, GOperType pOp, int pWeight);
    void                                SetOpWeight(GStatementSlot pSlot, GOperType pOp, int pWeight);
    
    
    // the most common use case here is that we want to disallow rotations in some cases.
    // if we configure this in an impossible configuration, the system should log the inconsistency
    void                                SetOpPairDisallow(GSymbol pSymbolA, GSymbol pSymbolB, GOperType pOp);
    void                                SetOpPairDisallow(GSymbol pSymbolA, GExpr pExprB, GOperType pOp);
    void                                SetOpPairDisallow(GSymbol pSymbolA, GStatementSlot pSlotB, GOperType pOp);
    void                                SetOpPairDisallow(GExpr pExprA, GSymbol pSymbolB, GOperType pOp);
    void                                SetOpPairDisallow(GExpr pExprA, GStatementSlot pSlotB, GOperType pOp);
    void                                SetOpPairDisallow(GStatementSlot pSlotA, GStatementSlot pSlotB, GOperType pOp);
    
    
    bool                                Bake(GSymbol pSymbol, GAssignType pAssignType, GStatement &pResult, std::string &pError);
    bool                                Bake(GExpr pExpr, GAssignType pAssignType, GStatement &pResult, std::string &pError);
    
    
    bool                                Bake(GSymbol pSymbol, GStatement &pResult, std::string &pError);
    bool                                Bake(GExpr pExpr, GStatement &pResult, std::string &pError);
    
    
    bool                                BakeMix(GSymbol pSymbol, GStatement &pResult, std::string &pError);
    bool                                BakeMix(GExpr pExpr, GStatement &pResult, std::string &pError);
    

    
};

#endif /* GStatement_hpp */
