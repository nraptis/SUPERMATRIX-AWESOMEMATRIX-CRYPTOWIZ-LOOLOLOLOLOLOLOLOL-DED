//
//  TwistRunner.cpp
//  Gorgon
//
//  Created by Sonic The Hedge Whore on 4/17/26.
//

#include "TwistRunner.hpp"

#include <cstdio>
#include <cstring>


TwistRunner::TwistRunner() {
}

TwistRunner::~TwistRunner() {
    
}


void TwistRunner::RunSeed(TwistSeeder *pSeeder,
                          std::uint8_t *pPassword,
                          int pPasswordLength) {
    UnrollPassword(pPassword, pPasswordLength);
    
    pSeeder->Seed(&mWorkSpace, mSource);
    
}

bool TwistRunner::RunSeedExpect(TwistSeeder *pSeeder,
                                std::uint8_t *pPassword,
                                int pPasswordLength,
                                TwistWorkSpace *pExpectation) {
    RunSeed(pSeeder, pPassword, pPasswordLength);
    
    for (int aIndex=0; aIndex<S_BLOCK; aIndex++) {
        if (mWorkSpace.mWorkLaneA[aIndex] != pExpectation->mWorkLaneA[aIndex]) {
            printf("result mismatch on work lane a\n");
            return false;
        }
        if (mWorkSpace.mWorkLaneB[aIndex] != pExpectation->mWorkLaneB[aIndex]) {
            printf("result mismatch on work lane b\n");
            return false;
        }
    }
    
    return true;
}


void TwistRunner::UnrollPassword(std::uint8_t *pPassword,
                                 int pPasswordLength) {
    
    memset(mSource, 0, S_BLOCK);
    
    if (pPasswordLength <= 0) {
        return;
    }
    
    int aSeedIndex = 0;
    int aPasswordIndex = 0;
    while (aSeedIndex < S_BLOCK) {
        
        while ((aSeedIndex < S_BLOCK) && (aPasswordIndex < pPasswordLength)) {
            mSource[aSeedIndex] = pPassword[aPasswordIndex];
            ++aSeedIndex;
            ++aPasswordIndex;
        }
        if (aSeedIndex < S_BLOCK) {
            mSource[aSeedIndex] = 0;
            ++aSeedIndex;
        }
        if (aSeedIndex < S_BLOCK) {
            mSource[aSeedIndex] = 0;
            ++aSeedIndex;
        }
        aPasswordIndex = 0;
    }
    
}
