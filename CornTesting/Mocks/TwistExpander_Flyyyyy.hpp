#pragma once

#include "TwistExpander.hpp"

class TwistExpander_Flyyyyy : public TwistExpander {
public:
    TwistExpander_Flyyyyy();
    ~TwistExpander_Flyyyyy() override = default;

    void Seed(TwistWorkSpace *pWorkspace,
              std::uint8_t *pSource,
              std::uint8_t *pPassword,
              unsigned int pPasswordByteLength) override;
    void TwistBlock(TwistWorkSpace *pWorkspace,
                    std::uint8_t *pSource,
                    std::uint8_t *pDestination) override;

private:
    static const std::uint8_t kSBoxA[S_SBOX];
    static const std::uint8_t kSBoxB[S_SBOX];
    static const std::uint8_t kSBoxC[S_SBOX];
    static const std::uint8_t kSBoxD[S_SBOX];
    static const std::uint8_t kSaltA[S_SALT];
    static const std::uint8_t kSaltB[S_SALT];
    static const std::uint8_t kSaltC[S_SALT];
    static const std::uint8_t kSaltD[S_SALT];
};
