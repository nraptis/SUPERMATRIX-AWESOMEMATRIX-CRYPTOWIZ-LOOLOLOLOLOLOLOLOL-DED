//
//  GTwistExpander.hpp
//  MeanMachine
//

#ifndef GGTwistExpander_hpp
#define GGTwistExpander_hpp

#include "GSeedProgram.hpp"
#include "TwistExpander.hpp"

#include <cstdint>
#include <string>
#include <vector>

class TwistProgramBranch {
public:
    void                                AddBatch(const GBatch &pBatch);
    void                                AddLine(const std::string &pLine);
    void                                AddWorkspaceAliasLine(const std::string &pAliasName,
                                                              const std::string &pWorkspaceFieldName);
    void                                AddAssignByteLine(const std::string &pName,
                                                          std::uint8_t pValue);
    void                                Clear();
    const std::vector<std::string>&     GetBatchJsonText() const;
    const std::vector<std::string>&     GetStringLines() const;

private:
    std::vector<std::string>            mBatchJsonText;
    std::vector<std::string>            mStringLines;

    friend class GTwistExpander;
};

class GTwistExpander : public TwistExpander {
public:
    GTwistExpander();
    ~GTwistExpander() override;

    void                                Seed(TwistWorkSpace *pWorkspace,
                                             std::uint8_t *pSource,
                                             std::uint8_t *pPassword,
                                             unsigned int pPasswordByteLength) override;
    void                                TwistBlock(TwistWorkSpace *pWorkspace,
                                                   std::uint8_t *pSource,
                                                   std::uint8_t *pDestination) override;

    bool                                ExportCPPProjectRoot(const std::string &pRootPath,
                                                             std::string *pError = nullptr) const;
    bool                                ExportJSONProjectRoot(const std::string &pRootPath,
                                                              std::string *pError = nullptr) const;
    bool                                LoadJSONProjectRoot(const std::string &pJsonPath,
                                                            std::string *pError = nullptr);

    void                                RefreshTablePointers();

    std::string                         mNameBase;
    TwistProgramBranch                  mSeeder; // Seed branch
    TwistProgramBranch                  mTwister; // Twist branch

    unsigned char                       mInitialValue_Carry;
    unsigned char                       mInitialValue_Value;
    unsigned char                       mInitialValue_Permute;

    std::vector<std::uint8_t>           _mSBoxA;
    std::vector<std::uint8_t>           _mSBoxB;
    std::vector<std::uint8_t>           _mSBoxC;
    std::vector<std::uint8_t>           _mSBoxD;

    std::vector<std::uint8_t>           _mSaltA;
    std::vector<std::uint8_t>           _mSaltB;
    std::vector<std::uint8_t>           _mSaltC;
    std::vector<std::uint8_t>           _mSaltD;
};

#endif /* GGTwistExpander_hpp */
