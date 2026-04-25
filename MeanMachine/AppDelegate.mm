//
//  AppDelegate.m
//  MeanMachine
//
//  Created by John Snow on 4/20/26.
//

#import "AppDelegate.h"

#include "FileIO.hpp"
#include "SBoxTables.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_set>


#include "TwistWorkSpace.hpp"
#include "TwistSeeder.hpp"
#include "GStatementRecipe.hpp"
#include "TwistFunctional.hpp"
#include "GTermExpander.hpp"
#include "GSeedWorkLaneRecipe.hpp"
#include "FastMatrix8.hpp"
#include "Random.hpp"
#include "FastMatrixUnroll.hpp"


std::vector<std::vector<int>> GenerateAllPermutations8() {
    std::vector<std::vector<int>> aPermutations;
    
    std::vector<int> aCurrent = {0,1,2,3,4,5,6,7};
    
    do {
        aPermutations.push_back(aCurrent);
    } while (std::next_permutation(aCurrent.begin(), aCurrent.end()));
    
    return aPermutations;
}

struct ArrHash {
    std::size_t operator()(const std::array<uint8_t,8>& a) const {
        std::size_t h = 0;
        for (auto v : a) h = h * 31 + v;
        return h;
    }
};

std::vector<std::vector<int>> GeneratePermutations8_ExcludingUnrollTable() {

    // 1. Build exclusion set
    std::unordered_set<std::array<uint8_t,8>, ArrHash> aExclude;

    for (std::size_t i = 0; i < 256; ++i) {
        std::array<uint8_t,8> row;
        for (int j = 0; j < 8; ++j) {
            row[j] = FM8UnrollTableA::kUnrollTable[i][j];
        }
        aExclude.insert(row);
    }
    
    for (std::size_t i = 0; i < 256; ++i) {
        std::array<uint8_t,8> row;
        for (int j = 0; j < 8; ++j) {
            row[j] = FM8UnrollTableB::kUnrollTable[i][j];
        }
        aExclude.insert(row);
    }
    
    for (std::size_t i = 0; i < 256; ++i) {
        std::array<uint8_t,8> row;
        for (int j = 0; j < 8; ++j) {
            row[j] = FM8UnrollTableC::kUnrollTable[i][j];
        }
        aExclude.insert(row);
    }
    
    for (std::size_t i = 0; i < 256; ++i) {
        std::array<uint8_t,8> row;
        for (int j = 0; j < 8; ++j) {
            row[j] = FM8UnrollTableD::kUnrollTable[i][j];
        }
        aExclude.insert(row);
    }
    
    printf("this many %d\n", aExclude.size());

    // 2. Generate permutations
    std::vector<std::vector<int>> aPermutations;
    std::vector<int> aCurrent = {0,1,2,3,4,5,6,7};

    do {
        std::array<uint8_t,8> key;
        for (int i = 0; i < 8; ++i) {
            key[i] = static_cast<uint8_t>(aCurrent[i]);
        }

        // 3. Keep only if NOT in exclusion set
        if (!aExclude.contains(key)) {
            aPermutations.push_back(aCurrent);
        }

    } while (std::next_permutation(aCurrent.begin(), aCurrent.end()));

    return aPermutations;
}

int ComputeMatches(const std::vector<int>& a, const std::vector<int>& b) {
    int aMatches = 0;
    for (int i = 0; i < 8; i++) {
        if (a[i] == b[i]) {
            aMatches++;
        }
    }
    return aMatches;
}

int ComputeWrapDistance(const std::vector<int>& a, const std::vector<int>& b) {
    int aTotal = 0;

    for (int i = 0; i < 8; i++) {
        if (a[i] != b[i]) {
            int d = std::abs(a[i] - b[i]);
            d = std::min(d, 8 - d);
            aTotal += d;
        }
    }

    return aTotal;
}

int ComputeCycleScore(const std::vector<int>& perm) {
    bool visited[8] = {false};
    int cycleCount = 0;
    int largestCycle = 0;
    int fixedPoints = 0;

    for (int i = 0; i < 8; i++) {
        if (perm[i] == i) fixedPoints++;
    }

    for (int i = 0; i < 8; i++) {
        if (!visited[i]) {
            int length = 0;
            int j = i;

            while (!visited[j]) {
                visited[j] = true;
                j = perm[j];
                length++;
            }

            cycleCount++;
            if (length > largestCycle) {
                largestCycle = length;
            }
        }
    }

    return largestCycle * 10 - cycleCount * 2 - fixedPoints * 3;
}

// helper for hashing vector<int>
struct VecHash {
    std::size_t operator()(const std::vector<int>& v) const {
        std::size_t h = 0;
        for (int x : v) {
            h = h * 31 + x;
        }
        return h;
    }
};

void BuildBest256() {

    std::vector<std::vector<int>> aPerms = GeneratePermutations8_ExcludingUnrollTable();

        //Random::Shuffle(aPerms); // your shuffle

        std::vector<std::vector<int>> aChosen;
        aChosen.reserve(256);

        std::unordered_set<std::vector<int>, VecHash> aUsed;

        // seed
        //aChosen.push_back({0,1,2,3,4,5,6,7});
        //aUsed.insert(aChosen[0]);

    
    while (aChosen.size() < 256) {

        int aBestIndex = -1;

        int aBestWorstMatch = 999;
        int aBestCycleScore = -999;
        int aBestDistanceScore = -1;

        for (int aPermIndex = 0; aPermIndex < (int)aPerms.size(); aPermIndex++) {

            const auto& aCandidate = aPerms[aPermIndex];

            if (aUsed.find(aCandidate) != aUsed.end()) {
                continue;
            }

            // --- compute worst match ---
            int aWorstMatch = 0;

            for (const auto& aExisting : aChosen) {
                int aMatches = ComputeMatches(aCandidate, aExisting);
                if (aMatches > aWorstMatch) {
                    aWorstMatch = aMatches;
                }
            }

            // --- primary: minimize worst match ---
            if (aWorstMatch > aBestWorstMatch) {
                continue;
            }

            // --- compute cycle score ---
            int aCycleScore = ComputeCycleScore(aCandidate);

            // --- compute distance ---
            int aTotalDistance = 0;
            for (const auto& aExisting : aChosen) {
                aTotalDistance += ComputeWrapDistance(aCandidate, aExisting);
            }

            // --- selection logic ---
            bool aTake = false;

            if (aWorstMatch < aBestWorstMatch) {
                aTake = true;
            }
            else if (aWorstMatch == aBestWorstMatch) {

                if (aCycleScore > aBestCycleScore) {
                    aTake = true;
                }
                else if (aCycleScore == aBestCycleScore) {

                    if (aTotalDistance > aBestDistanceScore) {
                        aTake = true;
                    }
                }
            }

            if (aTake) {
                aBestWorstMatch = aWorstMatch;
                aBestCycleScore = aCycleScore;
                aBestDistanceScore = aTotalDistance;
                aBestIndex = aPermIndex;
            }
        }

        if (aBestIndex == -1) {
            break;
        }

        aChosen.push_back(aPerms[aBestIndex]);
        aUsed.insert(aPerms[aBestIndex]);

        printf("Picked %zu (match=%d, cycle=%d, dist=%d)\n",
            aChosen.size(),
            aBestWorstMatch,
            aBestCycleScore,
            aBestDistanceScore);
    }
    // Emit a large declaration statement for header file. 4 per row.
    // blah = [[0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7],
    //          [0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7],
    printf("\n=== COPY THIS ===\n\n");
    printf("static const uint8_t kUnrollTable[256][8] = {\n");

    for (int i = 0; i < 256; i++) {
        printf("    {");
        for (int j = 0; j < 8; j++) {
            printf("%d", aChosen[i][j]);
            if (j != 7) printf(", ");
        }
        printf("}%s\n", (i == 255) ? "" : ",");
    }

    printf("};\n");
    
}

/*
namespace {

constexpr std::size_t kSBoxLength = 256U;
constexpr std::size_t kMaxExportBoxes = 256U;

std::string MakeBoxSuffix(const std::size_t pIndex) {
    if (pIndex < 26U) {
        return std::string(1, static_cast<char>('A' + static_cast<int>(pIndex)));
    }
    return std::to_string(pIndex + 1U);
}

bool IsPermutationBox(const std::vector<std::uint8_t> &pBox) {
    if (pBox.size() != kSBoxLength) {
        return false;
    }

    std::array<bool, kSBoxLength> aSeen = {};
    for (const std::uint8_t aValue : pBox) {
        if (aSeen[aValue]) {
            return false;
        }
        aSeen[aValue] = true;
    }
    return true;
}

std::vector<std::vector<std::uint8_t>> LoadExportBoxes() {
    
    const std::string aDirectory = FileIO::ProjectRoot("Assets/data_s_box");
    std::vector<std::string> aPaths = FileIO::GetAllFiles(aDirectory);
    std::sort(aPaths.begin(), aPaths.end());
    
    auto aBoxed = SBoxTables::Get();
    

    std::vector<std::vector<std::uint8_t>> aBoxes;
    for (const std::string &aPath : aPaths) {
        std::vector<std::uint8_t> aFileData;
        if (!FileIO::Load(aPath, aFileData)) {
            continue;
        }

        for (std::size_t aOffset = 0U; (aOffset + kSBoxLength) <= aFileData.size(); aOffset += kSBoxLength) {
            std::vector<std::uint8_t> aBox(aFileData.begin() + static_cast<long>(aOffset),
                                           aFileData.begin() + static_cast<long>(aOffset + kSBoxLength));
            if (!IsPermutationBox(aBox)) {
                continue;
            }
            aBoxes.push_back(aBox);
            if (aBoxes.size() >= kMaxExportBoxes) {
                return aBoxes;
            }
        }
    }
    return aBoxes;
}

std::vector<std::uint8_t> BuildInverseBox(const std::vector<std::uint8_t> &pForward) {
    std::vector<std::uint8_t> aReverse(kSBoxLength, 0U);
    for (std::size_t i = 0U; i < pForward.size() && i < kSBoxLength; ++i) {
        aReverse[pForward[i]] = static_cast<std::uint8_t>(i);
    }
    return aReverse;
}

void AppendBoxCode(std::string *pDest,
                   const std::string &pName,
                   const std::vector<std::uint8_t> &pBox) {
    if (pDest == NULL) {
        return;
    }

    *pDest += "unsigned char ";
    *pDest += pName;
    *pDest += "[256] = {\n";

    for (std::size_t i = 0U; i < pBox.size(); ++i) {
        if ((i % 16U) == 0U) {
            *pDest += "    ";
        }

        char aBuffer[8];
        std::snprintf(aBuffer, sizeof(aBuffer), "0x%02X", pBox[i]);
        *pDest += aBuffer;

        if (i + 1U != pBox.size()) {
            *pDest += ", ";
        }

        if ((i % 16U) == 15U) {
            *pDest += "\n";
        }
    }

    if ((pBox.size() % 16U) != 0U) {
        *pDest += "\n";
    }
    *pDest += "};\n\n";
}

std::string BuildExportCode(const std::vector<std::vector<std::uint8_t>> &pBoxes) {
    std::string aCode;
    aCode += "// Generated by MeanMachine/AppDelegate.mm\n";
    aCode += "// First 10 permutation S-boxes from Assets/data_s_box with inverses.\n\n";

    for (std::size_t i = 0U; i < pBoxes.size(); ++i) {
        const std::string aSuffix = MakeBoxSuffix(i);
        const std::vector<std::uint8_t> aReverse = BuildInverseBox(pBoxes[i]);
        AppendBoxCode(&aCode, "mForwardBox" + aSuffix, pBoxes[i]);
        AppendBoxCode(&aCode, "mReverseBox" + aSuffix, aReverse);
    }

    return aCode;
}

void ExportSBoxesAsHeader() {
    const std::vector<std::vector<std::uint8_t>> aBoxes = LoadExportBoxes();
    if (aBoxes.empty()) {
        std::printf("S-box export skipped: no permutation boxes found in Assets/data_s_box.\n");
        return;
    }

    const std::string aCode = BuildExportCode(aBoxes);
    const std::string aOutputPath = FileIO::ProjectRoot("generated/exported_s_boxes.hpp");
    std::vector<std::uint8_t> aBytes(aCode.begin(), aCode.end());
    if (!FileIO::Save(aOutputPath, aBytes)) {
        std::printf("S-box export failed: %s\n", aOutputPath.c_str());
        return;
    }

    std::printf("Exported %zu S-box pairs to %s\n", aBoxes.size(), aOutputPath.c_str());
}

} // namespace
*/

@interface AppDelegate ()

@property (strong) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    (void)aNotification;
    
    


    // Now print out the exact code to initialize FM8UnrollTableA
    
    BuildBest256();
    return;
    
    
    std::string aErrorString;
    GSeedWorkLaneRecipeWorkSpace aRecipeSpace;
    
    /*
    GBatch aBatch1;
    if (!aRecipeSpace.Plan(false, aErrorString)) {
        printf("{real seeder flow} work lane recipe failed to plan.");
        return;
    }
    
    
   
    if (!aRecipeSpace.Bake(aBatch1.mLoops, aErrorString)) {
        printf("{real seeder flow} work lane recipe failed to plan.");
        return;
    }
    
    GBatch aBatch2;
    if (!aRecipeSpace.Plan(true, aErrorString)) {
        printf("{real seeder flow} work lane recipe failed to plan.");
        return;
    }
    
    
   
    if (!aRecipeSpace.Bake(aBatch2.mLoops, aErrorString)) {
        printf("{real seeder flow} work lane recipe failed to plan.");
        return;
    }
    
    
    
    printf("ok, the plan worked\n");
    
    printf("%s\n", aBatch1.ToPrettyString().c_str());
    
    
    printf("ok, the plan worked\n");
    
    printf("%s\n", aBatch2.ToPrettyString().c_str());
    
    printf("ok, the plan worked\n");
    
    */
    
    
    FM8 amatri;
    amatri.RotateRight();
    //amatri.

    for (int aRowIndex = 0; aRowIndex < 256; aRowIndex++) {

        int aMinMatches = 8;
        int aMaxMatches = 0;
        int aSumMatches = 0;
        int aCount = 0;

        for (int aOtherIndex = 0; aOtherIndex < 256; aOtherIndex++) {
            if (aRowIndex == aOtherIndex) {
                continue;
            }

            int aMatches = 0;

            for (int aElementIndex = 0; aElementIndex < 8; aElementIndex++) {
                if (FM8UnrollTableA::kUnrollTable[aRowIndex][aElementIndex] ==
                    FM8UnrollTableA::kUnrollTable[aOtherIndex][aElementIndex]) {
                    aMatches++;
                }
            }
            
            

            if (aMatches < aMinMatches) {
                aMinMatches = aMatches;
            }

            if (aMatches > aMaxMatches) {
                aMaxMatches = aMatches;
            }

            aSumMatches += aMatches;
            aCount++;
        }

        double aAverageMatches = (double)aSumMatches / (double)aCount;

        printf("Row %3d: min=%d max=%d avg=%.2f\n",
               aRowIndex, aMinMatches, aMaxMatches, aAverageMatches);
    }
    
    printf("adsasd\n\n");
    printf("adsasd\n\n");
    
    
    
    printf("adsasd\n\n");
    
    
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    (void)aNotification;
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}


@end
