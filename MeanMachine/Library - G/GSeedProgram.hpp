//
//  GSeedProgram.hpp
//  MeanMachine
//

#ifndef GSeedProgram_hpp
#define GSeedProgram_hpp

#include "GExpr.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct GStatement;
struct GLoop;

struct GScopeRules {
    std::unordered_map<std::string, int>        mReadPreferredMinimum;
    std::unordered_map<std::string, int>        mReadPreferredMaximum;

    void                                        SetReadPreferredMinimum(GSymbol pSymbol, int pCount);
    void                                        SetReadPreferredMaximum(GSymbol pSymbol, int pCount);

    int                                         GetReadPreferredMinimum(GSymbol pSymbol) const;
    int                                         GetReadPreferredMaximum(GSymbol pSymbol) const;
};


struct GScopeState {
    std::unordered_map<std::string, int>        mReadCounts;

    void                                        Clear();

    void                                        Consume(GScopeState pScopeState);
    void                                        Consume(GLoop pLoop);
    void                                        Consume(GStatement pStatement);
    void                                        Consume(GSymbol pSymbol);
    void                                        Consume(GExpr pExpr);

    void                                        RegisterRead(GSymbol pSymbol);
    void                                        RegisterRead(GExpr pExpr);

    int                                         GetReadCount(GSymbol pSymbol, int pCount = 0) const;
    int                                         GetReadCount(GExpr pExpr, int pCount = 0) const;
};

struct GTarget {
    GSymbol                                     mSymbol;
    std::shared_ptr<GExpr>                      mIndex;
    
    GTarget();
    
    static GTarget                              Symbol(const GSymbol &pSymbol);
    static GTarget                              Write(const GSymbol &pSymbol,
                                                      const GExpr &pIndex);
    
    void                                        Set(const GTarget &pOther);
    void                                        Invalidate();
    
    bool                                        IsInvalid() const;
    bool                                        HasIndex() const;
    bool                                        IsVar() const;
    bool                                        IsBuf() const;
    
    bool                                        HasSymbol(GSymbol pSymbol) const;
    bool                                        HasOper(GOperType pOper) const;
    
};

enum class GAssignType : std::uint8_t {
    kInvalid = 0,
    kSet = 1,
    kAddAssign = 2,
    kXorAssign = 3
};

struct GStatement {
    GTarget                             mTarget;
    GAssignType                         mAssignType;
    GExpr                               mExpression;

    GStatement();

    static GStatement                   Assign(const GTarget &pTarget,
                                            const GExpr &pExpression);
    static GStatement                   AddAssign(const GTarget &pTarget,
                                               const GExpr &pExpression);
    static GStatement                   XorAssign(const GTarget &pTarget,
                                               const GExpr &pExpression);

    void                                Set(const GStatement &pOther);
    void                                Invalidate();
    bool                                IsInvalid() const;
    
    GSymbol                             GetAssignSymbol(int pIndex) const;
    bool                                HasAssignSymbol(int pIndex) const;
    
    GOperType                           GetAssignOper(int pIndex) const;
    bool                                HasAssignOper(int pIndex) const;
    
    GSymbol                             GetExprSymbol(int pIndex) const;
    bool                                HasExprSymbol(int pIndex) const;
    
    GOperType                           GetExprOper(int pIndex) const;
    bool                                HasExprOper(int pIndex) const;
    
    bool                                HasAssignSymbol(GSymbol pSymbol) const;
    bool                                HasAssignOper(GOperType pOper) const;
    
    bool                                HasExprSymbol(GSymbol pSymbol) const;
    bool                                HasExprOper(GOperType pOper) const;
    
    
    
};

struct GLoop {
    GSymbol                             mLoopVariable;
    std::string                         mLoopVariableName;
    int                                 mLoopBegin;
    std::string                         mLoopEndText;
    int                                 mLoopStep;
    std::vector<GStatement>             mInitializationStatements;
    std::vector<GStatement>             mBodyStatements;
    
    GLoop();
    
    bool                                IsInvalid() const;

    void                                AddBody(GStatement &pStatemet);
    void                                AddInitialization(GStatement &pStatemet);
    
};

struct GBatch {
    std::string                         mName;
    std::vector<GLoop>                  mLoops;
    GScopeState                         mScopeStateGlobal;
    GScopeState                         mScopeStateLocal;

    GBatch();

    bool                                IsInvalid() const;
    void                                CommitLoop(const GLoop &pLoop);

    int                                 CountReads(TwistWorkSpaceSlot pSlot) const;
    int                                 CountWrites(TwistWorkSpaceSlot pSlot) const;
    int                                 CountLoopsReading(TwistWorkSpaceSlot pSlot) const;
    int                                 CountLoopsWriting(TwistWorkSpaceSlot pSlot) const;

    std::vector<std::string>            CollectVariableNames() const;
    std::vector<TwistWorkSpaceSlot>     CollectReferencedSlots() const;

    std::string                         ToPrettyString() const;
    std::string                         ToJson(std::string *pError = nullptr) const;
    static bool                         FromJson(const std::string &pJsonText,
                                                 GBatch *pBatch,
                                                 std::string *pError = nullptr);

    std::string                         BuildCpp(const std::string &pFunctionName = "GeneratedTwistSeed",
                                                 std::string *pError = nullptr,
                                                 bool pEmitDeclarations = true) const;
    std::string                         BuildCppScopeBlock(std::string *pError = nullptr,
                                                           bool pEmitDeclarations = true) const;

    bool                                Execute(TwistWorkSpace *pWorkspace,
                                                std::string *pError = nullptr) const;
    bool                                ExecuteWithVariables(TwistWorkSpace *pWorkspace,
                                                             std::unordered_map<std::string, int> *pVariables,
                                                             std::string *pError = nullptr) const;
};

#endif /* GSeedProgram_hpp */
