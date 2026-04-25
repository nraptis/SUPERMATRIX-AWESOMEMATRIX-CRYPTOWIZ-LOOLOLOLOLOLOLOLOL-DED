//
//  TwistWrap.hpp
//  Gorgon
//
//  Created by Codex on 4/15/26.
//

#ifndef TwistWrap_hpp
#define TwistWrap_hpp

#include "TwistWorkSpace.hpp"
#include <cstddef>
#include <cstdint>

inline std::uint8_t RotL8(std::uint8_t pValue, std::uint8_t pAmount) {
    std::uint8_t a = static_cast<std::uint8_t>(pAmount & 7U);
    if (a == 0U) { return pValue; }
    return static_cast<std::uint8_t>((pValue << a) | (pValue >> (8U - a)));
}

inline std::uint8_t RotateRight8(std::uint8_t pValue, std::uint8_t pAmount) {
    std::uint8_t a = static_cast<std::uint8_t>(pAmount & 7U);
    if (a == 0U) { return pValue; }
    return static_cast<std::uint8_t>((pValue >> a) | (pValue << (8U - a)));
}

inline int WrapIndex(int pIndex,
                              int pLength) {
    if (pLength <= 0) {
        return 0;
    }
    
    int aWrappedIndex = pIndex % pLength;
    if (aWrappedIndex < 0) {
        aWrappedIndex += pLength;
    }
    return aWrappedIndex;
}

inline std::size_t WrapSizedIndex(int pIndex,
                                  int pLength) {
    return static_cast<std::size_t>(WrapIndex(pIndex, pLength));
}

inline std::size_t WrapBlock(int pIndex) {
    return WrapSizedIndex(pIndex, S_BLOCK);
}

inline std::size_t WrapKeyA(int pIndex) {
    return WrapSizedIndex(pIndex, S_KEY_A);
}

inline std::size_t WrapKeyB(int pIndex) {
    return WrapSizedIndex(pIndex, S_KEY_B);
}

inline std::size_t WrapMaskA(int pIndex) {
    return WrapSizedIndex(pIndex, S_MASK_A);
}

inline std::size_t WrapMaskB(int pIndex) {
    return WrapSizedIndex(pIndex, S_MASK_B);
}

inline std::size_t WrapNoiseA(int pIndex) {
    return WrapSizedIndex(pIndex, S_SBOX);
}

inline std::size_t WrapNoiseB(int pIndex) {
    return WrapSizedIndex(pIndex, S_SBOX);
}

inline std::size_t WrapNoiseC(int pIndex) {
    return WrapSizedIndex(pIndex, S_SBOX);
}

inline std::size_t WrapNoiseD(int pIndex) {
    return WrapSizedIndex(pIndex, S_SBOX);
}

inline std::size_t WrapSaltA(int pIndex) {
    return WrapSizedIndex(pIndex, S_SALT);
}

inline std::size_t WrapSaltB(int pIndex) {
    return WrapSizedIndex(pIndex, S_SALT);
}

inline std::size_t WrapSaltC(int pIndex) {
    return WrapSizedIndex(pIndex, S_SALT);
}

inline std::size_t WrapSaltD(int pIndex) {
    return WrapSizedIndex(pIndex, S_SALT);
}

inline std::uint8_t TwistWrapRead8(const std::uint8_t *pBuffer,
                                   int pIndex,
                                   int pLength) {
    return pBuffer[WrapIndex(pIndex, pLength)];
}

inline std::uint8_t WrapBlock8(const std::uint8_t *pBuffer,
                               int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_BLOCK);
}

inline std::uint8_t WrapKeyA8(const std::uint8_t *pBuffer,
                              int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_KEY_A);
}

inline std::uint8_t WrapKeyB8(const std::uint8_t *pBuffer,
                              int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_KEY_B);
}

inline std::uint8_t WrapMaskA8(const std::uint8_t *pBuffer,
                               int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_MASK_A);
}

inline std::uint8_t WrapMaskB8(const std::uint8_t *pBuffer,
                               int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_MASK_B);
}

inline std::uint8_t WrapNoiseA8(const std::uint8_t *pBuffer,
                                int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_SBOX);
}

inline std::uint8_t WrapNoiseB8(const std::uint8_t *pBuffer,
                                int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_SBOX);
}

inline std::uint8_t WrapNoiseC8(const std::uint8_t *pBuffer,
                                int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_SBOX);
}

inline std::uint8_t WrapNoiseD8(const std::uint8_t *pBuffer,
                                int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_SBOX);
}

inline std::uint8_t WrapSaltA8(const std::uint8_t *pBuffer,
                               int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_SALT);
}

inline std::uint8_t WrapSaltB8(const std::uint8_t *pBuffer,
                               int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_SALT);
}

inline std::uint8_t WrapSaltC8(const std::uint8_t *pBuffer,
                               int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_SALT);
}

inline std::uint8_t WrapSaltD8(const std::uint8_t *pBuffer,
                               int pIndex) {
    return TwistWrapRead8(pBuffer, pIndex, S_SALT);
}

#endif /* TwistWrap_hpp */
