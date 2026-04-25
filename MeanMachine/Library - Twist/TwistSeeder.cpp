//
//  TwistSeeder.cpp
//  MeanMachine
//

#include "TwistSeeder.hpp"

#include <cstdio>
#include <cstring>

namespace {

void FillIdentity(std::uint8_t *pDest,
                  const std::size_t pLength) {
    for (std::size_t aIndex = 0U; aIndex < pLength; ++aIndex) {
        pDest[aIndex] = static_cast<std::uint8_t>(aIndex & 0xFFU);
    }
}

void CopyBytes(std::uint8_t *pDest,
               const std::uint8_t *pSource,
               const std::size_t pLength) {
    if ((pDest == nullptr) || (pSource == nullptr) || (pLength == 0U)) {
        return;
    }
    std::memcpy(pDest, pSource, pLength);
}

} // namespace

TwistSeeder::TwistSeeder() {
    FillIdentity(mSBoxA, S_SBOX);
    FillIdentity(mSBoxB, S_SBOX);
    FillIdentity(mSBoxC, S_SBOX);
    FillIdentity(mSBoxD, S_SBOX);
    std::memset(mSaltA, 0, S_SALT);
    std::memset(mSaltB, 0, S_SALT);
    std::memset(mSaltC, 0, S_SALT);
    std::memset(mSaltD, 0, S_SALT);
}

TwistSeeder::~TwistSeeder() {
}

void TwistSeeder::Seed(TwistWorkSpace *pWorkspace,
                       std::uint8_t *pSource) {
    if (pWorkspace == nullptr) {
        std::printf("fatal: TwistSeeder::Seed requires a workspace.\n");
        return;
    }

    (void)pSource;
    CopyTablesToWorkspace(pWorkspace);
}

void TwistSeeder::SetSBoxes(const std::uint8_t *pA,
                            const std::uint8_t *pB,
                            const std::uint8_t *pC,
                            const std::uint8_t *pD) {
    CopyBytes(mSBoxA, pA, S_SBOX);
    CopyBytes(mSBoxB, pB, S_SBOX);
    CopyBytes(mSBoxC, pC, S_SBOX);
    CopyBytes(mSBoxD, pD, S_SBOX);
}

void TwistSeeder::SetSalts(const std::uint8_t *pA,
                           const std::uint8_t *pB,
                           const std::uint8_t *pC,
                           const std::uint8_t *pD) {
    CopyBytes(mSaltA, pA, S_SALT);
    CopyBytes(mSaltB, pB, S_SALT);
    CopyBytes(mSaltC, pC, S_SALT);
    CopyBytes(mSaltD, pD, S_SALT);
}

void TwistSeeder::CopyTablesToWorkspace(TwistWorkSpace *pWorkspace) const {
    if (pWorkspace == nullptr) {
        return;
    }

    std::memcpy(pWorkspace->mSBoxA, mSBoxA, S_SBOX);
    std::memcpy(pWorkspace->mSBoxB, mSBoxB, S_SBOX);
    std::memcpy(pWorkspace->mSBoxC, mSBoxC, S_SBOX);
    std::memcpy(pWorkspace->mSBoxD, mSBoxD, S_SBOX);

    std::memcpy(pWorkspace->mSaltA, mSaltA, S_SALT);
    std::memcpy(pWorkspace->mSaltB, mSaltB, S_SALT);
    std::memcpy(pWorkspace->mSaltC, mSaltC, S_SALT);
    std::memcpy(pWorkspace->mSaltD, mSaltD, S_SALT);
}
