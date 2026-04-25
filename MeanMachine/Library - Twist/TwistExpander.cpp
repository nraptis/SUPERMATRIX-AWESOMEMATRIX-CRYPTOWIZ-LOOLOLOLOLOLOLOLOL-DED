//
//  TwistExpander.cpp
//  MeanMachine
//

#include "TwistExpander.hpp"

#include <cstdio>
#include <cstring>

void TwistWorkSpaceBindExternalBuffers(std::uint8_t *pSource,
                                       std::uint8_t *pDest);

namespace {

const std::uint8_t *DefaultSBox() {
    static std::uint8_t sIdentity[S_SBOX];
    static bool sDidInit = false;
    if (!sDidInit) {
        for (int i = 0; i < S_SBOX; ++i) {
            sIdentity[i] = static_cast<std::uint8_t>(i & 0xFF);
        }
        sDidInit = true;
    }
    return sIdentity;
}

const std::uint8_t *DefaultSalt() {
    static std::uint8_t sZero[S_SALT] = {0};
    return sZero;
}

void UnrollPasswordToSource(std::uint8_t *pSource,
                            std::uint8_t *pPassword,
                            const unsigned int pPasswordByteLength) {
    if (pSource == nullptr) {
        return;
    }

    std::memset(pSource, 0, static_cast<std::size_t>(S_BLOCK));
    if ((pPassword == nullptr) || (pPasswordByteLength == 0U)) {
        return;
    }

    unsigned int aInitialCopy = pPasswordByteLength;
    if (aInitialCopy > S_BLOCK) {
        aInitialCopy = S_BLOCK;
    }

    std::memcpy(pSource, pPassword, static_cast<std::size_t>(aInitialCopy));
    if (aInitialCopy < S_BLOCK) {
        pSource[aInitialCopy++] = 0;
    }
    if (aInitialCopy < S_BLOCK) {
        pSource[aInitialCopy++] = 0;
    }

    unsigned int aFilled = aInitialCopy;
    while (aFilled < S_BLOCK) {
        unsigned int aChunk = aFilled;
        if ((aFilled + aChunk) > S_BLOCK) {
            aChunk = S_BLOCK - aFilled;
        }
        std::memcpy(pSource + aFilled, pSource, static_cast<std::size_t>(aChunk));
        aFilled += aChunk;
    }
}

const std::uint8_t *ResolveTable(const std::uint8_t *pTable,
                                 bool pIsSalt) {
    if (pTable != nullptr) {
        return pTable;
    }
    return pIsSalt ? DefaultSalt() : DefaultSBox();
}

} // namespace

TwistExpander::TwistExpander() {
    mWorkspace = nullptr;
    mSource = nullptr;

    mSaltA = nullptr;
    mSaltB = nullptr;
    mSaltC = nullptr;
    mSaltD = nullptr;

    mSBoxA = nullptr;
    mSBoxB = nullptr;
    mSBoxC = nullptr;
    mSBoxD = nullptr;
}

TwistExpander::~TwistExpander() {
}

void TwistExpander::Seed(TwistWorkSpace *pWorkspace,
                         std::uint8_t *pSource,
                         std::uint8_t *pPassword,
                         unsigned int pPasswordByteLength) {
    if (pWorkspace == nullptr) {
        std::printf("fatal: TwistExpander::Seed requires workspace\n");
        return;
    }
    if (pSource == nullptr) {
        std::printf("fatal: TwistExpander::Seed requires source buffer\n");
        return;
    }

    mWorkspace = pWorkspace;
    mSource = pSource;

    TwistWorkSpaceBindExternalBuffers(pSource, nullptr);
    UnrollPasswordToSource(pSource, pPassword, pPasswordByteLength);

    std::memcpy(pWorkspace->mSaltA, ResolveTable(mSaltA, true), S_SALT);
    std::memcpy(pWorkspace->mSaltB, ResolveTable(mSaltB, true), S_SALT);
    std::memcpy(pWorkspace->mSaltC, ResolveTable(mSaltC, true), S_SALT);
    std::memcpy(pWorkspace->mSaltD, ResolveTable(mSaltD, true), S_SALT);

    std::memcpy(pWorkspace->mSBoxA, ResolveTable(mSBoxA, false), S_SBOX);
    std::memcpy(pWorkspace->mSBoxB, ResolveTable(mSBoxB, false), S_SBOX);
    std::memcpy(pWorkspace->mSBoxC, ResolveTable(mSBoxC, false), S_SBOX);
    std::memcpy(pWorkspace->mSBoxD, ResolveTable(mSBoxD, false), S_SBOX);
}

void TwistExpander::TwistBlock(TwistWorkSpace *pWorkspace,
                               std::uint8_t *pSource,
                               std::uint8_t *pDestination) {
    if ((pWorkspace == nullptr) || (pSource == nullptr) || (pDestination == nullptr)) {
        return;
    }

    TwistWorkSpaceBindExternalBuffers(pSource, pDestination);
    std::memcpy(pWorkspace->mWorkLaneD, pSource, S_BLOCK);
    std::memcpy(pDestination, pWorkspace->mWorkLaneD, S_BLOCK);
}

void TwistExpander::Twist(TwistWorkSpace *pWorkspace,
                          std::uint8_t *pSource,
                          std::uint8_t *pDestination,
                          unsigned int pDestinationByteLength) {
    if ((pWorkspace == nullptr) || (pSource == nullptr) || (pDestination == nullptr)) {
        std::printf("fatal: TwistExpander::Twist requires workspace/source/destination\n");
        return;
    }
    if ((pDestinationByteLength % S_BLOCK) != 0U) {
        std::printf("fatal: TwistExpander::Twist needs a multiple of S_BLOCK\n");
        return;
    }

    for (unsigned int aStartByte = 0U;
         aStartByte < pDestinationByteLength;
         aStartByte += S_BLOCK) {
        TwistWorkSpaceBindExternalBuffers(pSource + aStartByte, pDestination + aStartByte);
        TwistBlock(pWorkspace, pSource + aStartByte, pDestination + aStartByte);
    }
}
