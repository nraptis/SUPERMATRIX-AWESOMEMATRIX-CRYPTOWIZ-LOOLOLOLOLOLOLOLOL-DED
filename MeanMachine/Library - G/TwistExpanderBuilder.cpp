//
//  TwistExpanderBuilder.cpp
//  MeanMachine
//

#include "GTwistExpander.hpp"

#include "FileIO.hpp"
#include "GJson.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

using MeanMachine_json::JsonValue;

namespace {

void SetError(std::string *pError,
              const std::string &pMessage) {
    if (pError != nullptr) {
        *pError = pMessage;
    }
}

bool SaveTextFile(const std::string &pPath,
                  const std::string &pText,
                  std::string *pError) {
    const std::vector<std::uint8_t> aData(pText.begin(), pText.end());
    if (!FileIO::Save(pPath, aData)) {
        SetError(pError, "Failed to save file: " + pPath);
        return false;
    }
    return true;
}

std::string SanitizeIdentifier(const std::string &pText,
                               const std::string &pFallback) {
    std::string aResult;
    aResult.reserve(pText.size());

    for (const char aChar : pText) {
        if (std::isalnum(static_cast<unsigned char>(aChar)) != 0) {
            aResult.push_back(aChar);
        } else {
            aResult.push_back('_');
        }
    }

    while (!aResult.empty() && aResult.front() == '_') {
        aResult.erase(aResult.begin());
    }

    if (aResult.empty()) {
        aResult = pFallback;
    }

    if (!aResult.empty() && std::isdigit(static_cast<unsigned char>(aResult.front())) != 0) {
        aResult.insert(aResult.begin(), '_');
    }

    return aResult;
}

bool HasJsonExtension(const std::string &pPath) {
    std::string aExtension = std::filesystem::path(pPath).extension().string();
    std::transform(aExtension.begin(), aExtension.end(), aExtension.begin(),
                   [](const unsigned char pChar) {
                       return static_cast<char>(std::tolower(pChar));
                   });
    return aExtension == ".json";
}

std::string ResolveOutputPathFromProjectRoot(const std::string &pPath) {
    if (pPath.empty()) {
        return FileIO::ProjectRoot();
    }

    const std::filesystem::path aInputPath(pPath);
    if (aInputPath.is_absolute()) {
        return aInputPath.lexically_normal().generic_string();
    }

    const std::filesystem::path aProjectRoot(FileIO::ProjectRoot());
    return (aProjectRoot / aInputPath).lexically_normal().generic_string();
}

std::string ResolveJsonOutputPath(const std::string &pRootPath,
                                  const std::string &pNameBase) {
    if (HasJsonExtension(pRootPath)) {
        return ResolveOutputPathFromProjectRoot(pRootPath);
    }

    const std::string aFileName = pNameBase + ".json";
    return ResolveOutputPathFromProjectRoot(FileIO::Join(pRootPath, aFileName));
}

std::string IndentBlock(const std::string &pText,
                        const int pIndentLevel) {
    const std::string aIndent(static_cast<std::size_t>(pIndentLevel * 4), ' ');

    std::istringstream aSource(pText);
    std::ostringstream aResult;
    std::string aLine;
    bool aFirstLine = true;

    while (std::getline(aSource, aLine)) {
        if (!aFirstLine) {
            aResult << '\n';
        }
        aFirstLine = false;
        aResult << aIndent << aLine;
    }

    return aResult.str();
}

template <typename T>
void AppendUniqueValue(std::vector<T> *pList,
                       const T &pValue) {
    if (pList == nullptr) {
        return;
    }
    for (const T &aValue : *pList) {
        if (aValue == pValue) {
            return;
        }
    }
    pList->push_back(pValue);
}

bool ContainsText(const std::vector<std::string> &pList,
                  const std::string &pValue) {
    for (const std::string &aValue : pList) {
        if (aValue == pValue) {
            return true;
        }
    }
    return false;
}

bool EndsWithText(const std::string &pText,
                  const std::string &pSuffix) {
    if (pSuffix.size() > pText.size()) {
        return false;
    }
    return pText.compare(pText.size() - pSuffix.size(), pSuffix.size(), pSuffix) == 0;
}

bool StartsWithText(const std::string &pText,
                    const std::string &pPrefix) {
    if (pPrefix.size() > pText.size()) {
        return false;
    }
    return pText.compare(0U, pPrefix.size(), pPrefix) == 0;
}

std::string ScalarCppTypeForName(const std::string &pName) {
    if (StartsWithText(pName, "aOracle")) {
        return "std::size_t";
    }
    if (EndsWithText(pName, "Key")) {
        return "std::uint16_t";
    }
    return "std::uint8_t";
}

std::string TrimText(const std::string &pText) {
    if (pText.empty()) {
        return "";
    }

    std::size_t aBegin = 0U;
    while ((aBegin < pText.size()) && std::isspace(static_cast<unsigned char>(pText[aBegin])) != 0) {
        ++aBegin;
    }

    std::size_t aEnd = pText.size();
    while ((aEnd > aBegin) && std::isspace(static_cast<unsigned char>(pText[aEnd - 1U])) != 0) {
        --aEnd;
    }

    return pText.substr(aBegin, aEnd - aBegin);
}

std::string DeclaredIdentifierFromLine(const std::string &pLine) {
    std::string aText = TrimText(pLine);
    if (aText.empty()) {
        return "";
    }
    if (aText.back() == ';') {
        aText.pop_back();
    }

    const std::size_t aEquals = aText.find('=');
    if (aEquals == std::string::npos) {
        return "";
    }

    const std::string aLeft = TrimText(aText.substr(0U, aEquals));
    if (aLeft.empty()) {
        return "";
    }
    if ((aLeft.find(' ') == std::string::npos) && (aLeft.find('\t') == std::string::npos)) {
        return "";
    }
    if ((aLeft.find('(') != std::string::npos) || (aLeft.find(')') != std::string::npos)) {
        return "";
    }

    std::size_t aEnd = aLeft.size();
    while ((aEnd > 0U) && std::isspace(static_cast<unsigned char>(aLeft[aEnd - 1U])) != 0) {
        --aEnd;
    }

    std::size_t aStart = aEnd;
    while ((aStart > 0U) &&
           ((std::isalnum(static_cast<unsigned char>(aLeft[aStart - 1U])) != 0) ||
            (aLeft[aStart - 1U] == '_'))) {
        --aStart;
    }

    if (aStart == aEnd) {
        return "";
    }
    return aLeft.substr(aStart, aEnd - aStart);
}

std::string NormalizeLegacyByteTypeLine(const std::string &pLine) {
    std::string aLine = pLine;

    auto ReplaceAll = [&aLine](const std::string &pFrom, const std::string &pTo) {
        std::size_t aPos = 0U;
        while ((aPos = aLine.find(pFrom, aPos)) != std::string::npos) {
            aLine.replace(aPos, pFrom.size(), pTo);
            aPos += pTo.size();
        }
    };

    ReplaceAll("unsigned char *", "std::uint8_t *");
    ReplaceAll("unsigned char*", "std::uint8_t*");
    ReplaceAll("unsigned char ", "std::uint8_t ");

    return aLine;
}

std::vector<TwistWorkSpaceSlot> FixedWorkspaceSlotOrder() {
    return {
        TwistWorkSpaceSlot::kSource,
        TwistWorkSpaceSlot::kDest,

        TwistWorkSpaceSlot::kSaltA,
        TwistWorkSpaceSlot::kSaltB,
        TwistWorkSpaceSlot::kSaltC,
        TwistWorkSpaceSlot::kSaltD,

        TwistWorkSpaceSlot::kSBoxA,
        TwistWorkSpaceSlot::kSBoxB,
        TwistWorkSpaceSlot::kSBoxC,
        TwistWorkSpaceSlot::kSBoxD,

        TwistWorkSpaceSlot::kSeedExpansionLaneA,
        TwistWorkSpaceSlot::kSeedExpansionLaneB,
        TwistWorkSpaceSlot::kSeedExpansionLaneC,
        TwistWorkSpaceSlot::kSeedExpansionLaneD,

        TwistWorkSpaceSlot::kWorkLaneA,
        TwistWorkSpaceSlot::kWorkLaneB,
        TwistWorkSpaceSlot::kWorkLaneC,
        TwistWorkSpaceSlot::kWorkLaneD,

        TwistWorkSpaceSlot::kOperationLaneA,
        TwistWorkSpaceSlot::kOperationLaneB,

        TwistWorkSpaceSlot::kMaskLaneA,
        TwistWorkSpaceSlot::kMaskLaneB,

        TwistWorkSpaceSlot::kKeyBoxUnrolledA,
        TwistWorkSpaceSlot::kKeyBoxUnrolledB,
        TwistWorkSpaceSlot::kKeyRowReadA,
        TwistWorkSpaceSlot::kKeyRowReadB,
        TwistWorkSpaceSlot::kKeyRowWriteA,
        TwistWorkSpaceSlot::kKeyRowWriteB,

        TwistWorkSpaceSlot::kMaskBoxUnrolledA,
        TwistWorkSpaceSlot::kMaskBoxUnrolledB,
        TwistWorkSpaceSlot::kMaskRowReadA,
        TwistWorkSpaceSlot::kMaskRowReadB,
        TwistWorkSpaceSlot::kMaskRowWriteA,
        TwistWorkSpaceSlot::kMaskRowWriteB
    };
}

std::string WorkspaceAliasDeclaration(const TwistWorkSpaceSlot pSlot) {
    const std::string aAlias = BufAliasName(pSlot);
    const std::string aPrefix = "[[maybe_unused]] std::uint8_t *" + aAlias + " = ";
    switch (pSlot) {
        case TwistWorkSpaceSlot::kSource:
        case TwistWorkSpaceSlot::kDest:
            return aPrefix +
                   "TwistWorkSpace::GetBuffer(pWorkspace, static_cast<TwistWorkSpaceSlot>(" +
                   std::to_string(static_cast<int>(pSlot)) + "));";

        case TwistWorkSpaceSlot::kSaltA: return aPrefix + "pWorkspace->mSaltA;";
        case TwistWorkSpaceSlot::kSaltB: return aPrefix + "pWorkspace->mSaltB;";
        case TwistWorkSpaceSlot::kSaltC: return aPrefix + "pWorkspace->mSaltC;";
        case TwistWorkSpaceSlot::kSaltD: return aPrefix + "pWorkspace->mSaltD;";

        case TwistWorkSpaceSlot::kSBoxA: return aPrefix + "pWorkspace->mSBoxA;";
        case TwistWorkSpaceSlot::kSBoxB: return aPrefix + "pWorkspace->mSBoxB;";
        case TwistWorkSpaceSlot::kSBoxC: return aPrefix + "pWorkspace->mSBoxC;";
        case TwistWorkSpaceSlot::kSBoxD: return aPrefix + "pWorkspace->mSBoxD;";

        case TwistWorkSpaceSlot::kSeedExpansionLaneA: return aPrefix + "pWorkspace->mExpandLaneA;";
        case TwistWorkSpaceSlot::kSeedExpansionLaneB: return aPrefix + "pWorkspace->mExpandLaneB;";
        case TwistWorkSpaceSlot::kSeedExpansionLaneC: return aPrefix + "pWorkspace->mExpandLaneC;";
        case TwistWorkSpaceSlot::kSeedExpansionLaneD: return aPrefix + "pWorkspace->mExpandLaneD;";

        case TwistWorkSpaceSlot::kWorkLaneA: return aPrefix + "pWorkspace->mWorkLaneA;";
        case TwistWorkSpaceSlot::kWorkLaneB: return aPrefix + "pWorkspace->mWorkLaneB;";
        case TwistWorkSpaceSlot::kWorkLaneC: return aPrefix + "pWorkspace->mWorkLaneC;";
        case TwistWorkSpaceSlot::kWorkLaneD: return aPrefix + "pWorkspace->mWorkLaneD;";

        case TwistWorkSpaceSlot::kOperationLaneA: return aPrefix + "pWorkspace->mOperationLaneA;";
        case TwistWorkSpaceSlot::kOperationLaneB: return aPrefix + "pWorkspace->mOperationLaneB;";

        case TwistWorkSpaceSlot::kMaskLaneA: return aPrefix + "pWorkspace->mMaskLaneA;";
        case TwistWorkSpaceSlot::kMaskLaneB: return aPrefix + "pWorkspace->mMaskLaneB;";

        case TwistWorkSpaceSlot::kKeyBoxUnrolledA: return aPrefix + "&(pWorkspace->mKeyBoxA[0][0]);";
        case TwistWorkSpaceSlot::kKeyBoxUnrolledB: return aPrefix + "&(pWorkspace->mKeyBoxB[0][0]);";
        case TwistWorkSpaceSlot::kKeyRowReadA: return aPrefix + "&(pWorkspace->mKeyBoxA[H_KEY_A - 1][0]);";
        case TwistWorkSpaceSlot::kKeyRowReadB: return aPrefix + "&(pWorkspace->mKeyBoxB[H_KEY_B - 1][0]);";
        case TwistWorkSpaceSlot::kKeyRowWriteA: return aPrefix + "&(pWorkspace->mKeyBoxA[0][0]);";
        case TwistWorkSpaceSlot::kKeyRowWriteB: return aPrefix + "&(pWorkspace->mKeyBoxB[0][0]);";

        case TwistWorkSpaceSlot::kMaskBoxUnrolledA: return aPrefix + "&(pWorkspace->mMaskBoxA[0][0]);";
        case TwistWorkSpaceSlot::kMaskBoxUnrolledB: return aPrefix + "&(pWorkspace->mMaskBoxB[0][0]);";
        case TwistWorkSpaceSlot::kMaskRowReadA: return aPrefix + "&(pWorkspace->mMaskBoxA[H_MASK_A - 1][0]);";
        case TwistWorkSpaceSlot::kMaskRowReadB: return aPrefix + "&(pWorkspace->mMaskBoxB[H_MASK_B - 1][0]);";
        case TwistWorkSpaceSlot::kMaskRowWriteA: return aPrefix + "&(pWorkspace->mMaskBoxA[0][0]);";
        case TwistWorkSpaceSlot::kMaskRowWriteB: return aPrefix + "&(pWorkspace->mMaskBoxB[0][0]);";

        default:
            return aPrefix +
                   "TwistWorkSpace::GetBuffer(pWorkspace, static_cast<TwistWorkSpaceSlot>(" +
                   std::to_string(static_cast<int>(pSlot)) + "));";
    }
}

bool ParseBatchJson(const std::string &pBatchJson,
                    GBatch *pBatch,
                    std::string *pError) {
    if (pBatch == nullptr) {
        SetError(pError, "Batch output was null.");
        return false;
    }

    if (!GBatch::FromJson(pBatchJson, pBatch, pError)) {
        if ((pError != nullptr) && pError->empty()) {
            *pError = "Failed to parse branch batch JSON.";
        }
        return false;
    }

    return true;
}

bool AppendBranchBody(const TwistProgramBranch &pBranch,
                      std::ostringstream *pStream,
                      std::string *pError) {
    if (pStream == nullptr) {
        SetError(pError, "Branch output stream was null.");
        return false;
    }

    std::vector<GBatch> aBatches;
    for (const std::string &aBatchJson : pBranch.GetBatchJsonText()) {
        GBatch aBatch;
        if (!ParseBatchJson(aBatchJson, &aBatch, pError)) {
            return false;
        }
        aBatches.push_back(aBatch);
    }

    std::vector<std::string> aDeclaredNames;
    for (const std::string &aLine : pBranch.GetStringLines()) {
        const std::string aNormalizedLine = NormalizeLegacyByteTypeLine(aLine);
        const std::string aDeclaredName = DeclaredIdentifierFromLine(aNormalizedLine);
        if (!aDeclaredName.empty()) {
            AppendUniqueValue(&aDeclaredNames, aDeclaredName);
        }
    }

    std::vector<std::string> aLoopVariables;
    std::vector<std::string> aScalarVariables;
    for (const GBatch &aBatch : aBatches) {
        for (const GLoop &aLoop : aBatch.mLoops) {
            if (!aLoop.mLoopVariableName.empty()) {
                AppendUniqueValue(&aLoopVariables, aLoop.mLoopVariableName);
            }
        }
        for (const std::string &aName : aBatch.CollectVariableNames()) {
            AppendUniqueValue(&aScalarVariables, aName);
        }
    }

    aScalarVariables.erase(
        std::remove_if(aScalarVariables.begin(),
                       aScalarVariables.end(),
                       [&aLoopVariables, &aDeclaredNames](const std::string &pName) {
                           return ContainsText(aLoopVariables, pName) || ContainsText(aDeclaredNames, pName);
                       }),
        aScalarVariables.end());

    bool aWroteDeclaration = false;
    const std::vector<TwistWorkSpaceSlot> aAllSlots = FixedWorkspaceSlotOrder();
    for (TwistWorkSpaceSlot aSlot : aAllSlots) {
        const std::string aAliasName = BufAliasName(aSlot);
        if (ContainsText(aDeclaredNames, aAliasName)) {
            continue;
        }
        *pStream << "    " << WorkspaceAliasDeclaration(aSlot) << '\n';
        aWroteDeclaration = true;
    }

    for (const std::string &aVariable : aScalarVariables) {
        *pStream << "    " << ScalarCppTypeForName(aVariable) << " " << aVariable << " = 0;\n";
        aWroteDeclaration = true;
    }

    if (aWroteDeclaration) {
        *pStream << '\n';
    }

    for (const std::string &aLine : pBranch.GetStringLines()) {
        *pStream << "    " << NormalizeLegacyByteTypeLine(aLine) << '\n';
    }
    if (!pBranch.GetStringLines().empty()) {
        *pStream << '\n';
    }

    for (const GBatch &aBatch : aBatches) {
        const std::string aScopeBlock = aBatch.BuildCppScopeBlock(pError, false);
        if (aScopeBlock.empty()) {
            if ((pError != nullptr) && pError->empty()) {
                *pError = "Batch scope-block export returned empty text.";
            }
            return false;
        }

        *pStream << IndentBlock(aScopeBlock, 1) << '\n';
    }

    return true;
}

JsonValue BranchToJsonValue(const TwistProgramBranch &pBranch,
                            std::string *pError) {
    JsonValue::Array aLines;
    for (const std::string &aLine : pBranch.GetStringLines()) {
        aLines.push_back(JsonValue::String(NormalizeLegacyByteTypeLine(aLine)));
    }

    JsonValue::Array aBatches;
    for (const std::string &aBatchJson : pBranch.GetBatchJsonText()) {
        auto aParsed = JsonValue::Parse(aBatchJson, pError);
        if (!aParsed.has_value() || !aParsed->is_object()) {
            if ((pError != nullptr) && pError->empty()) {
                *pError = "Failed to parse branch batch JSON for program export.";
            }
            return JsonValue::ObjectValue({});
        }
        aBatches.push_back(*aParsed);
    }

    JsonValue::Object aObject;
    aObject["string_lines"] = JsonValue::ArrayValue(std::move(aLines));
    aObject["batches"] = JsonValue::ArrayValue(std::move(aBatches));
    return JsonValue::ObjectValue(std::move(aObject));
}

std::string BytesToCppArray(const std::vector<std::uint8_t> &pBytes,
                            const std::string &pName,
                            const std::string &pLenToken) {
    std::ostringstream aOut;
    aOut << "const std::uint8_t " << pName << "[" << pLenToken << "] = {\n";

    for (std::size_t i = 0; i < pBytes.size(); ++i) {
        if ((i % 16U) == 0U) {
            aOut << "    ";
        }

        char aHex[8];
        std::snprintf(aHex, sizeof(aHex), "0x%02X", pBytes[i]);
        aOut << aHex;

        if (i + 1U < pBytes.size()) {
            aOut << ", ";
        }

        if ((i % 16U) == 15U || (i + 1U == pBytes.size())) {
            aOut << '\n';
        }
    }

    aOut << "};\n";
    return aOut.str();
}

JsonValue::Array BytesToJsonArray(const std::vector<std::uint8_t> &pBytes) {
    JsonValue::Array aOut;
    aOut.reserve(pBytes.size());
    for (std::uint8_t aByte : pBytes) {
        aOut.push_back(JsonValue::Number(static_cast<double>(aByte)));
    }
    return aOut;
}

} // namespace

void TwistProgramBranch::AddBatch(const GBatch &pBatch) {
    std::string aError;
    const std::string aBatchJson = pBatch.ToJson(&aError);
    if (aBatchJson.empty()) {
        std::printf("fatal: TwistProgramBranch::AddBatch failed: %s\n", aError.c_str());
        return;
    }

    mBatchJsonText.push_back(aBatchJson);
}

void TwistProgramBranch::AddLine(const std::string &pLine) {
    if (pLine.empty()) {
        return;
    }
    mStringLines.push_back(pLine);
}

void TwistProgramBranch::AddWorkspaceAliasLine(const std::string &pAliasName,
                                               const std::string &pWorkspaceFieldName) {
    if (pAliasName.empty() || pWorkspaceFieldName.empty()) {
        return;
    }

    AddLine("std::uint8_t *" + pAliasName + " = pWorkspace->" + pWorkspaceFieldName + ";");
}

void TwistProgramBranch::AddAssignByteLine(const std::string &pName,
                                           std::uint8_t pValue) {
    if (pName.empty()) {
        return;
    }

    char aHex[8];
    std::snprintf(aHex, sizeof(aHex), "0x%02X", pValue);
    AddLine("std::uint8_t " + pName + " = " + std::string(aHex) + ";");
}

void TwistProgramBranch::Clear() {
    mBatchJsonText.clear();
    mStringLines.clear();
}

const std::vector<std::string>& TwistProgramBranch::GetBatchJsonText() const {
    return mBatchJsonText;
}

const std::vector<std::string>& TwistProgramBranch::GetStringLines() const {
    return mStringLines;
}

bool GTwistExpander::ExportCPPProjectRoot(const std::string &pRootPath,
                                          std::string *pError) const {
    const std::string aBaseInput = mNameBase.empty() ? "Generated" : mNameBase;
    const std::string aBaseName = SanitizeIdentifier(aBaseInput, "Generated");
    const std::string aClassName = "TwistExpander_" + aBaseName;

    const std::string aRootInput = pRootPath.empty() ? "generated/cpp" : pRootPath;
    const std::string aRoot = ResolveOutputPathFromProjectRoot(aRootInput);

    const std::string aHeaderPath = FileIO::Join(aRoot, aClassName + ".hpp");
    const std::string aCppPath = FileIO::Join(aRoot, aClassName + ".cpp");

    GTwistExpander aSnapshot = *this;
    aSnapshot.RefreshTablePointers();

    std::ostringstream aHeader;
    aHeader << "#pragma once\n"
            << "\n"
            << "#include \"TwistExpander.hpp\"\n"
            << "\n"
            << "class " << aClassName << " : public TwistExpander {\n"
            << "public:\n"
            << "    " << aClassName << "();\n"
            << "    ~" << aClassName << "() override = default;\n"
            << "\n"
            << "    void Seed(TwistWorkSpace *pWorkspace,\n"
            << "              std::uint8_t *pSource,\n"
            << "              std::uint8_t *pPassword,\n"
            << "              unsigned int pPasswordByteLength) override;\n"
            << "    void TwistBlock(TwistWorkSpace *pWorkspace,\n"
            << "                    std::uint8_t *pSource,\n"
            << "                    std::uint8_t *pDestination) override;\n"
            << "\n"
            << "private:\n"
            << "    static const std::uint8_t kSBoxA[S_SBOX];\n"
            << "    static const std::uint8_t kSBoxB[S_SBOX];\n"
            << "    static const std::uint8_t kSBoxC[S_SBOX];\n"
            << "    static const std::uint8_t kSBoxD[S_SBOX];\n"
            << "    static const std::uint8_t kSaltA[S_SALT];\n"
            << "    static const std::uint8_t kSaltB[S_SALT];\n"
            << "    static const std::uint8_t kSaltC[S_SALT];\n"
            << "    static const std::uint8_t kSaltD[S_SALT];\n"
            << "};\n";

    std::ostringstream aCpp;
    aCpp << "#include \"" << aClassName << ".hpp\"\n"
         << "#include \"TwistFunctional.hpp\"\n"
         << "\n"
         << "#include <cstring>\n"
         << "\n"
         << BytesToCppArray(aSnapshot._mSBoxA, aClassName + "::kSBoxA", "S_SBOX")
         << '\n'
         << BytesToCppArray(aSnapshot._mSBoxB, aClassName + "::kSBoxB", "S_SBOX")
         << '\n'
         << BytesToCppArray(aSnapshot._mSBoxC, aClassName + "::kSBoxC", "S_SBOX")
         << '\n'
         << BytesToCppArray(aSnapshot._mSBoxD, aClassName + "::kSBoxD", "S_SBOX")
         << '\n'
         << BytesToCppArray(aSnapshot._mSaltA, aClassName + "::kSaltA", "S_SALT")
         << '\n'
         << BytesToCppArray(aSnapshot._mSaltB, aClassName + "::kSaltB", "S_SALT")
         << '\n'
         << BytesToCppArray(aSnapshot._mSaltC, aClassName + "::kSaltC", "S_SALT")
         << '\n'
         << BytesToCppArray(aSnapshot._mSaltD, aClassName + "::kSaltD", "S_SALT")
         << '\n'
         << aClassName << "::" << aClassName << "()\n"
         << ": TwistExpander() {\n"
         << "    mSBoxA = const_cast<std::uint8_t*>(kSBoxA);\n"
         << "    mSBoxB = const_cast<std::uint8_t*>(kSBoxB);\n"
         << "    mSBoxC = const_cast<std::uint8_t*>(kSBoxC);\n"
         << "    mSBoxD = const_cast<std::uint8_t*>(kSBoxD);\n"
         << "    mSaltA = const_cast<std::uint8_t*>(kSaltA);\n"
         << "    mSaltB = const_cast<std::uint8_t*>(kSaltB);\n"
         << "    mSaltC = const_cast<std::uint8_t*>(kSaltC);\n"
         << "    mSaltD = const_cast<std::uint8_t*>(kSaltD);\n"
         << "}\n"
         << "\n"
         << "void " << aClassName << "::Seed(TwistWorkSpace *pWorkspace,\n"
         << "                                 std::uint8_t *pSource,\n"
         << "                                 std::uint8_t *pPassword,\n"
         << "                                 unsigned int pPasswordByteLength) {\n"
         << "    TwistExpander::Seed(pWorkspace, pSource, pPassword, pPasswordByteLength);\n"
         << "    if (pWorkspace == nullptr) { return; }\n";
    if (!AppendBranchBody(aSnapshot.mSeeder, &aCpp, pError)) {
        return false;
    }
    aCpp << "}\n"
         << "\n"
         << "void " << aClassName << "::TwistBlock(TwistWorkSpace *pWorkspace,\n"
         << "                                       std::uint8_t *pSource,\n"
         << "                                       std::uint8_t *pDestination) {\n"
         << "    TwistExpander::TwistBlock(pWorkspace, pSource, pDestination);\n"
         << "    if ((pWorkspace == nullptr) || (pDestination == nullptr)) { return; }\n";
    if (!AppendBranchBody(aSnapshot.mTwister, &aCpp, pError)) {
        return false;
    }
    aCpp << "    std::memcpy(pDestination, pWorkspace->mWorkLaneD, S_BLOCK);\n"
         << "}\n";

    if (!SaveTextFile(aHeaderPath, aHeader.str(), pError)) {
        return false;
    }
    if (!SaveTextFile(aCppPath, aCpp.str(), pError)) {
        return false;
    }

    return true;
}

bool GTwistExpander::ExportJSONProjectRoot(const std::string &pRootPath,
                                           std::string *pError) const {
    GTwistExpander aSnapshot = *this;
    aSnapshot.RefreshTablePointers();

    const std::string aBaseInput = aSnapshot.mNameBase.empty() ? "Generated" : aSnapshot.mNameBase;
    const std::string aBaseName = SanitizeIdentifier(aBaseInput, "Generated");

    JsonValue::Object aRootObject;
    aRootObject["name_base"] = JsonValue::String(aBaseInput);
    aRootObject["seed"] = BranchToJsonValue(aSnapshot.mSeeder, pError);
    if ((pError != nullptr) && !pError->empty()) {
        return false;
    }

    aRootObject["twist"] = BranchToJsonValue(aSnapshot.mTwister, pError);
    if ((pError != nullptr) && !pError->empty()) {
        return false;
    }

    JsonValue::Object aTables;
    aTables["sbox_a"] = JsonValue::ArrayValue(BytesToJsonArray(aSnapshot._mSBoxA));
    aTables["sbox_b"] = JsonValue::ArrayValue(BytesToJsonArray(aSnapshot._mSBoxB));
    aTables["sbox_c"] = JsonValue::ArrayValue(BytesToJsonArray(aSnapshot._mSBoxC));
    aTables["sbox_d"] = JsonValue::ArrayValue(BytesToJsonArray(aSnapshot._mSBoxD));
    aTables["salt_a"] = JsonValue::ArrayValue(BytesToJsonArray(aSnapshot._mSaltA));
    aTables["salt_b"] = JsonValue::ArrayValue(BytesToJsonArray(aSnapshot._mSaltB));
    aTables["salt_c"] = JsonValue::ArrayValue(BytesToJsonArray(aSnapshot._mSaltC));
    aTables["salt_d"] = JsonValue::ArrayValue(BytesToJsonArray(aSnapshot._mSaltD));
    aRootObject["tables"] = JsonValue::ObjectValue(std::move(aTables));

    const std::string aJsonText = JsonValue::ObjectValue(std::move(aRootObject)).Serialize();
    const std::string aRoot = pRootPath.empty() ? "generated/json" : pRootPath;
    const std::string aOutputPath = ResolveJsonOutputPath(aRoot, aBaseName);

    return SaveTextFile(aOutputPath, aJsonText, pError);
}
