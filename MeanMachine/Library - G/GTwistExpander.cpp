//
//  GTwistExpander.cpp
//  MeanMachine
//

#include "GTwistExpander.hpp"

#include "FileIO.hpp"
#include "GJson.hpp"
#include "Random.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <filesystem>

using MeanMachine_json::JsonValue;

void TwistWorkSpaceBindExternalBuffers(std::uint8_t *pSource,
                                       std::uint8_t *pDest);

namespace {

void SetError(std::string *pError,
              const std::string &pMessage) {
    if (pError != nullptr) {
        *pError = pMessage;
    }
}

std::string ResolveInputPath(const std::string &pPath) {
    if (pPath.empty()) {
        return "";
    }

    const std::filesystem::path aInputPath(pPath);
    if (aInputPath.is_absolute()) {
        return aInputPath.lexically_normal().generic_string();
    }

    const std::filesystem::path aProjectRoot(FileIO::ProjectRoot());
    return (aProjectRoot / aInputPath).lexically_normal().generic_string();
}

bool LoadTextFile(const std::string &pPath,
                  std::string *pText,
                  std::string *pError) {
    if (pText == nullptr) {
        SetError(pError, "output text pointer was null.");
        return false;
    }

    std::vector<std::uint8_t> aBytes;
    if (!FileIO::Load(pPath, aBytes)) {
        SetError(pError, "failed to read JSON file: " + pPath);
        return false;
    }

    pText->assign(aBytes.begin(), aBytes.end());
    return true;
}

void EnsureTableSize(std::vector<std::uint8_t> *pTable,
                     const int pLength,
                     const bool pIdentityFallback) {
    if (pTable == nullptr) {
        return;
    }

    if (pTable->size() > static_cast<std::size_t>(pLength)) {
        pTable->resize(static_cast<std::size_t>(pLength));
    }

    while (pTable->size() < static_cast<std::size_t>(pLength)) {
        std::uint8_t aValue = 0U;
        if (pIdentityFallback) {
            aValue = static_cast<std::uint8_t>(pTable->size() & 0xFFU);
        }
        pTable->push_back(aValue);
    }
}

bool ParseByteArray(const JsonValue *pArrayValue,
                    std::vector<std::uint8_t> *pOut,
                    std::string *pError) {
    if (pOut == nullptr) {
        SetError(pError, "byte-array output argument was null.");
        return false;
    }
    if (pArrayValue == nullptr) {
        pOut->clear();
        return true;
    }
    if (!pArrayValue->is_array()) {
        SetError(pError, "expected byte table to be an array.");
        return false;
    }

    std::vector<std::uint8_t> aOut;
    aOut.reserve(pArrayValue->as_array().size());
    for (const JsonValue &aValue : pArrayValue->as_array()) {
        int aByte = -1;
        if (aValue.is_number()) {
            aByte = static_cast<int>(aValue.as_number());
        } else if (aValue.is_string()) {
            try {
                aByte = std::stoi(aValue.as_string());
            } catch (...) {
                SetError(pError, "byte table had invalid string value.");
                return false;
            }
        } else {
            SetError(pError, "byte table had non-numeric entry.");
            return false;
        }

        if ((aByte < 0) || (aByte > 255)) {
            SetError(pError, "byte table entry outside [0,255].");
            return false;
        }
        aOut.push_back(static_cast<std::uint8_t>(aByte));
    }

    *pOut = std::move(aOut);
    return true;
}

bool ParseBranch(const JsonValue &pRoot,
                 const std::string &pBranchName,
                 TwistProgramBranch *pBranch,
                 std::string *pError) {
    if (pBranch == nullptr) {
        SetError(pError, "branch output was null.");
        return false;
    }

    const JsonValue *aBranchValue = pRoot.find(pBranchName);
    if ((aBranchValue == nullptr) || !aBranchValue->is_object()) {
        return false;
    }

    pBranch->Clear();

    const JsonValue *aBatchesValue = aBranchValue->find("batches");
    if ((aBatchesValue != nullptr) && aBatchesValue->is_array()) {
        for (const JsonValue &aBatchValue : aBatchesValue->as_array()) {
            if (!aBatchValue.is_object()) {
                SetError(pError, "branch '" + pBranchName + "' has non-object batch entry.");
                return false;
            }

            GBatch aBatch;
            std::string aBatchError;
            if (!GBatch::FromJson(aBatchValue.Serialize(), &aBatch, &aBatchError)) {
                SetError(pError, "failed to parse branch batch JSON: " + aBatchError);
                return false;
            }
            pBranch->AddBatch(aBatch);
        }
    }

    const JsonValue *aStringLinesValue = aBranchValue->find("string_lines");
    if ((aStringLinesValue != nullptr) && aStringLinesValue->is_array()) {
        for (const JsonValue &aLineValue : aStringLinesValue->as_array()) {
            if (!aLineValue.is_string()) {
                SetError(pError, "branch '" + pBranchName + "' has non-string line entry.");
                return false;
            }
            pBranch->AddLine(aLineValue.as_string());
        }
    }

    return true;
}

bool ParseTables(const JsonValue &pRoot,
                 GTwistExpander *pExpander,
                 std::string *pError) {
    if (pExpander == nullptr) {
        SetError(pError, "expander output was null.");
        return false;
    }

    const JsonValue *aTables = pRoot.find("tables");
    if ((aTables == nullptr) || !aTables->is_object()) {
        return true;
    }

    if (!ParseByteArray(aTables->find("sbox_a"), &pExpander->_mSBoxA, pError)) { return false; }
    if (!ParseByteArray(aTables->find("sbox_b"), &pExpander->_mSBoxB, pError)) { return false; }
    if (!ParseByteArray(aTables->find("sbox_c"), &pExpander->_mSBoxC, pError)) { return false; }
    if (!ParseByteArray(aTables->find("sbox_d"), &pExpander->_mSBoxD, pError)) { return false; }

    if (!ParseByteArray(aTables->find("salt_a"), &pExpander->_mSaltA, pError)) { return false; }
    if (!ParseByteArray(aTables->find("salt_b"), &pExpander->_mSaltB, pError)) { return false; }
    if (!ParseByteArray(aTables->find("salt_c"), &pExpander->_mSaltC, pError)) { return false; }
    if (!ParseByteArray(aTables->find("salt_d"), &pExpander->_mSaltD, pError)) { return false; }

    return true;
}

bool ExecuteBatchJsonText(const std::vector<std::string> &pBatchJsonText,
                          TwistWorkSpace *pWorkspace,
                          std::unordered_map<std::string, int> *pVariables,
                          std::string *pError) {
    if (pVariables == nullptr) {
        SetError(pError, "batch variable map was null.");
        return false;
    }

    for (const std::string &aBatchJson : pBatchJsonText) {
        GBatch aBatch;
        if (!GBatch::FromJson(aBatchJson, &aBatch, pError)) {
            if ((pError != nullptr) && pError->empty()) {
                *pError = "failed to parse batch JSON.";
            }
            return false;
        }

        if (!aBatch.ExecuteWithVariables(pWorkspace, pVariables, pError)) {
            if ((pError != nullptr) && pError->empty()) {
                *pError = "batch execution failed.";
            }
            return false;
        }
    }
    return true;
}

std::string TrimCopy(const std::string &pText) {
    std::size_t aStart = 0U;
    while ((aStart < pText.size()) && std::isspace(static_cast<unsigned char>(pText[aStart]))) {
        aStart += 1U;
    }

    std::size_t aEnd = pText.size();
    while ((aEnd > aStart) && std::isspace(static_cast<unsigned char>(pText[aEnd - 1U]))) {
        aEnd -= 1U;
    }
    return pText.substr(aStart, aEnd - aStart);
}

bool IsIdentifier(const std::string &pName) {
    if (pName.empty()) {
        return false;
    }
    const unsigned char aFirst = static_cast<unsigned char>(pName[0]);
    if (!(std::isalpha(aFirst) || (aFirst == '_'))) {
        return false;
    }
    for (const unsigned char aCh : pName) {
        if (!(std::isalnum(aCh) || (aCh == '_'))) {
            return false;
        }
    }
    return true;
}

void StripOuterParens(std::string *pText) {
    if (pText == nullptr) {
        return;
    }
    while (pText->size() >= 2U && pText->front() == '(' && pText->back() == ')') {
        *pText = TrimCopy(pText->substr(1U, pText->size() - 2U));
    }
}

bool ParseIntLiteral(std::string pValueText,
                     int *pValueOut) {
    if (pValueOut == nullptr) {
        return false;
    }

    pValueText = TrimCopy(pValueText);
    if (pValueText.empty()) {
        return false;
    }

    StripOuterParens(&pValueText);

    while (!pValueText.empty()) {
        const char aLast = pValueText.back();
        if (aLast == 'u' || aLast == 'U' || aLast == 'l' || aLast == 'L') {
            pValueText.pop_back();
            pValueText = TrimCopy(pValueText);
            continue;
        }
        break;
    }

    if (pValueText.empty()) {
        return false;
    }

    const char *aStart = pValueText.c_str();
    char *aEnd = nullptr;
    const long aValue = std::strtol(aStart, &aEnd, 0);
    if ((aEnd == nullptr) || (*aEnd != '\0')) {
        return false;
    }
    *pValueOut = static_cast<int>(aValue);
    return true;
}

void ApplyBranchStringLines(const std::vector<std::string> &pLines,
                            std::unordered_map<std::string, int> *pVariables) {
    if (pVariables == nullptr) {
        return;
    }

    for (const std::string &aRawLine : pLines) {
        std::string aLine = aRawLine;
        const std::size_t aComment = aLine.find("//");
        if (aComment != std::string::npos) {
            aLine = aLine.substr(0U, aComment);
        }
        aLine = TrimCopy(aLine);
        if (aLine.empty()) {
            continue;
        }

        if (!aLine.empty() && aLine.back() == ';') {
            aLine.pop_back();
            aLine = TrimCopy(aLine);
        }
        if (aLine.empty()) {
            continue;
        }

        const std::size_t aEqual = aLine.find('=');
        if (aEqual == std::string::npos) {
            continue;
        }

        std::string aLeft = TrimCopy(aLine.substr(0U, aEqual));
        std::string aRight = TrimCopy(aLine.substr(aEqual + 1U));
        if (aLeft.empty() || aRight.empty()) {
            continue;
        }

        if ((aLeft.find('*') != std::string::npos) ||
            (aLeft.find('[') != std::string::npos) ||
            (aLeft.find('(') != std::string::npos) ||
            (aLeft.find(')') != std::string::npos)) {
            continue;
        }

        const std::size_t aLastSpace = aLeft.find_last_of(" \t");
        const std::string aName = (aLastSpace == std::string::npos)
            ? aLeft
            : TrimCopy(aLeft.substr(aLastSpace + 1U));
        if (!IsIdentifier(aName)) {
            continue;
        }

        int aValue = 0;
        if (!ParseIntLiteral(aRight, &aValue)) {
            continue;
        }

        (*pVariables)[aName] = aValue;
    }
}

bool ExecuteBranch(const TwistProgramBranch &pBranch,
                   TwistWorkSpace *pWorkspace,
                   std::string *pError) {
    std::unordered_map<std::string, int> aVariables;
    ApplyBranchStringLines(pBranch.GetStringLines(), &aVariables);
    return ExecuteBatchJsonText(pBranch.GetBatchJsonText(), pWorkspace, &aVariables, pError);
}

} // namespace

GTwistExpander::GTwistExpander()
: TwistExpander() {
    mNameBase = "Generated";

    mInitialValue_Carry = static_cast<unsigned char>(Random::Get(256));
    mInitialValue_Value = static_cast<unsigned char>(Random::Get(256));
    mInitialValue_Permute = static_cast<unsigned char>(Random::Get(256));

    RefreshTablePointers();
}

GTwistExpander::~GTwistExpander() {
}

void GTwistExpander::RefreshTablePointers() {
    EnsureTableSize(&_mSBoxA, S_SBOX, true);
    EnsureTableSize(&_mSBoxB, S_SBOX, true);
    EnsureTableSize(&_mSBoxC, S_SBOX, true);
    EnsureTableSize(&_mSBoxD, S_SBOX, true);

    EnsureTableSize(&_mSaltA, S_SALT, false);
    EnsureTableSize(&_mSaltB, S_SALT, false);
    EnsureTableSize(&_mSaltC, S_SALT, false);
    EnsureTableSize(&_mSaltD, S_SALT, false);

    mSBoxA = _mSBoxA.data();
    mSBoxB = _mSBoxB.data();
    mSBoxC = _mSBoxC.data();
    mSBoxD = _mSBoxD.data();

    mSaltA = _mSaltA.data();
    mSaltB = _mSaltB.data();
    mSaltC = _mSaltC.data();
    mSaltD = _mSaltD.data();
}

void GTwistExpander::Seed(TwistWorkSpace *pWorkspace,
                          std::uint8_t *pSource,
                          std::uint8_t *pPassword,
                          unsigned int pPasswordByteLength) {
    RefreshTablePointers();
    TwistExpander::Seed(pWorkspace, pSource, pPassword, pPasswordByteLength);

    if (pWorkspace == nullptr) {
        return;
    }

    TwistWorkSpaceBindExternalBuffers(pSource, nullptr);

    std::string aError;
    if (!ExecuteBranch(mSeeder, pWorkspace, &aError)) {
        std::printf("fatal: GTwistExpander::Seed failed: %s\n", aError.c_str());
    }
}

void GTwistExpander::TwistBlock(TwistWorkSpace *pWorkspace,
                                std::uint8_t *pSource,
                                std::uint8_t *pDestination) {
    if ((pWorkspace == nullptr) || (pSource == nullptr) || (pDestination == nullptr)) {
        return;
    }

    TwistWorkSpaceBindExternalBuffers(pSource, pDestination);
    std::memcpy(pWorkspace->mWorkLaneD, pSource, static_cast<std::size_t>(S_BLOCK));

    std::string aError;
    if (!ExecuteBranch(mTwister, pWorkspace, &aError)) {
        std::printf("fatal: GTwistExpander::TwistBlock failed: %s\n", aError.c_str());
        return;
    }

    std::memcpy(pDestination, pWorkspace->mWorkLaneD, static_cast<std::size_t>(S_BLOCK));
}

bool GTwistExpander::LoadJSONProjectRoot(const std::string &pJsonPath,
                                         std::string *pError) {
    const std::string aResolvedPath = ResolveInputPath(pJsonPath);
    if (aResolvedPath.empty()) {
        SetError(pError, "JSON path was empty.");
        return false;
    }

    std::string aJsonText;
    if (!LoadTextFile(aResolvedPath, &aJsonText, pError)) {
        return false;
    }

    auto aRoot = JsonValue::Parse(aJsonText, pError);
    if (!aRoot.has_value() || !aRoot->is_object()) {
        if ((pError != nullptr) && pError->empty()) {
            *pError = "program JSON root was invalid.";
        }
        return false;
    }

    if (const JsonValue *aNameBase = aRoot->find("name_base"); (aNameBase != nullptr) && aNameBase->is_string()) {
        mNameBase = aNameBase->as_string();
    }

    bool aDidParseSeed = ParseBranch(*aRoot, "seed", &mSeeder, pError);
    bool aDidParseTwist = ParseBranch(*aRoot, "twist", &mTwister, pError);

    if (!aDidParseSeed) {
        (void)ParseBranch(*aRoot, "seeder", &mSeeder, pError);
    }
    if (!aDidParseTwist) {
        (void)ParseBranch(*aRoot, "twister", &mTwister, pError);
    }

    if (!ParseTables(*aRoot, this, pError)) {
        return false;
    }

    RefreshTablePointers();
    return true;
}
