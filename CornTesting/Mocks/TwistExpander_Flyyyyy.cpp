#include "TwistExpander_Flyyyyy.hpp"
#include "TwistFunctional.hpp"

#include <cstring>

const std::uint8_t TwistExpander_Flyyyyy::kSBoxA[S_SBOX] = {
    0x31, 0x9A, 0xC7, 0x96, 0x72, 0x0D, 0x60, 0xEE, 0xBC, 0x00, 0x5C, 0x7A, 0x23, 0x38, 0x8E, 0xEB, 
    0xDE, 0x95, 0xBF, 0xB6, 0x4E, 0xD0, 0x57, 0x3F, 0xAE, 0x59, 0x9E, 0xE3, 0x12, 0xBE, 0x86, 0xAF, 
    0x44, 0x42, 0x22, 0x07, 0x9C, 0x8C, 0x33, 0xFE, 0xD1, 0x92, 0xE5, 0x13, 0xE7, 0x08, 0x90, 0xF6, 
    0x41, 0xA3, 0x46, 0xCE, 0x8B, 0x63, 0x6D, 0x32, 0x20, 0xFA, 0xD8, 0xE9, 0xF9, 0x2B, 0x05, 0x8A, 
    0x3C, 0xB9, 0x76, 0xC6, 0xEA, 0x4C, 0xB1, 0xD3, 0x8D, 0xE1, 0x14, 0x29, 0x37, 0xC5, 0x17, 0xB5, 
    0xA1, 0x18, 0x2C, 0xD9, 0x27, 0x6E, 0x64, 0x9D, 0x21, 0xF2, 0x54, 0xAC, 0x2A, 0x0A, 0xFC, 0x9F, 
    0x34, 0x94, 0xA2, 0x62, 0x3A, 0x26, 0xDD, 0x99, 0x1A, 0x2F, 0x61, 0xFB, 0xC0, 0xED, 0x73, 0xEF, 
    0xEC, 0xB2, 0x5B, 0xE8, 0x0E, 0x3E, 0x80, 0x7E, 0x19, 0x68, 0x45, 0x0B, 0xB7, 0x0F, 0x5E, 0x79, 
    0xD2, 0x7D, 0xD6, 0xF5, 0xF0, 0x4F, 0x36, 0xBB, 0xC2, 0xDF, 0xD7, 0xC1, 0x3D, 0xBD, 0xA7, 0xAA, 
    0x50, 0xF4, 0x6B, 0x65, 0x6A, 0x56, 0x43, 0xE4, 0x7B, 0xB8, 0x74, 0xDA, 0x28, 0xDC, 0x71, 0x69, 
    0xA4, 0xF1, 0xCD, 0x5D, 0x4B, 0x0C, 0x4A, 0x91, 0xE2, 0x55, 0x82, 0x1D, 0x6F, 0x02, 0xC9, 0xDB, 
    0xA8, 0x93, 0xB0, 0x2D, 0xA5, 0x75, 0x47, 0xC3, 0x01, 0xE0, 0x52, 0xD5, 0x11, 0xBA, 0xCF, 0x10, 
    0x39, 0xF7, 0x66, 0x3B, 0xE6, 0xF3, 0x25, 0x8F, 0x98, 0x49, 0xA6, 0x87, 0x06, 0x35, 0x85, 0xA9, 
    0xCB, 0xA0, 0x09, 0x70, 0x67, 0x4D, 0x1F, 0x30, 0x7C, 0xFD, 0xCC, 0x58, 0xF8, 0x83, 0xCA, 0x5F, 
    0x88, 0x03, 0x7F, 0x9B, 0x40, 0x24, 0xC4, 0x2E, 0x1E, 0xAB, 0xD4, 0xB4, 0xB3, 0x81, 0x1B, 0x04, 
    0x89, 0xFF, 0xC8, 0x51, 0x78, 0x84, 0x16, 0x6C, 0x77, 0x53, 0x5A, 0x1C, 0x48, 0xAD, 0x15, 0x97
};

const std::uint8_t TwistExpander_Flyyyyy::kSBoxB[S_SBOX] = {
    0x01, 0xCE, 0x74, 0x94, 0x61, 0x8F, 0xE1, 0x6C, 0xEE, 0x47, 0x6F, 0x4F, 0x81, 0xBD, 0xCA, 0x8A, 
    0xB8, 0xF6, 0x8C, 0x30, 0xEF, 0x12, 0x23, 0xAB, 0x6E, 0x89, 0xBA, 0xA4, 0xC7, 0x1E, 0xE4, 0xFC, 
    0x31, 0xB5, 0x13, 0x51, 0x4A, 0x3A, 0xF1, 0x1C, 0x80, 0xE0, 0x20, 0x55, 0xD0, 0x69, 0x10, 0x32, 
    0xD7, 0xA6, 0xC1, 0xC8, 0x2C, 0x83, 0x29, 0x63, 0xE3, 0x68, 0x3C, 0x7A, 0x0A, 0xAF, 0xA1, 0xBE, 
    0xC9, 0x08, 0x15, 0xE6, 0x18, 0x1F, 0x26, 0x70, 0xA8, 0x54, 0x43, 0x5F, 0x85, 0x84, 0x21, 0x52, 
    0x56, 0x19, 0xB9, 0xAE, 0xF5, 0x4B, 0x95, 0xD8, 0x71, 0x46, 0x41, 0x9E, 0x3D, 0x99, 0xEC, 0x28, 
    0xE7, 0x64, 0x34, 0x11, 0x4D, 0x67, 0xDA, 0xAC, 0xE9, 0xDB, 0x73, 0xFA, 0x62, 0x0E, 0xF3, 0xFE, 
    0x9C, 0x44, 0x79, 0xE8, 0xED, 0x39, 0x60, 0x3B, 0x8B, 0xCF, 0xA3, 0x0B, 0xA2, 0x7F, 0x9F, 0x2D, 
    0xE2, 0x59, 0x96, 0x50, 0xB6, 0x7D, 0x17, 0xF8, 0x92, 0x2A, 0x04, 0xF0, 0x5B, 0x7C, 0xD2, 0x1A, 
    0x35, 0xC2, 0xAD, 0x07, 0x3F, 0xB1, 0x27, 0xB2, 0xDD, 0x9A, 0xE5, 0xCC, 0xCD, 0x48, 0x03, 0x5D, 
    0xB0, 0x24, 0xAA, 0xFD, 0x09, 0x1D, 0x9B, 0x82, 0x36, 0xD3, 0x90, 0xFF, 0xD9, 0xB7, 0xDE, 0x5E, 
    0xEA, 0x72, 0xB4, 0x2B, 0x22, 0x77, 0x53, 0x00, 0xD5, 0xC6, 0xC5, 0xF4, 0xA5, 0x6A, 0x5A, 0x37, 
    0x2F, 0xA0, 0x65, 0xDF, 0x42, 0xD4, 0x05, 0x58, 0x3E, 0xF9, 0x40, 0x76, 0xC3, 0x75, 0x86, 0x78, 
    0x2E, 0xC4, 0xFB, 0x91, 0xF7, 0x8D, 0x0F, 0x93, 0xCB, 0x7E, 0x38, 0x1B, 0x98, 0x02, 0xBC, 0x0D, 
    0x4E, 0x25, 0xA9, 0x5C, 0x45, 0x97, 0x16, 0x49, 0x9D, 0x88, 0x66, 0xC0, 0xD6, 0xF2, 0x7B, 0x33, 
    0xDC, 0x8E, 0x4C, 0xA7, 0xBF, 0x14, 0xB3, 0xBB, 0x87, 0x57, 0xEB, 0x6D, 0x6B, 0x0C, 0xD1, 0x06
};

const std::uint8_t TwistExpander_Flyyyyy::kSBoxC[S_SBOX] = {
    0xBB, 0x5C, 0x68, 0x28, 0x90, 0x99, 0xFB, 0x06, 0x1E, 0x4B, 0xD9, 0x9B, 0xD0, 0x16, 0xB6, 0xDF, 
    0x50, 0x70, 0x46, 0xEB, 0xB2, 0xDA, 0x8A, 0x31, 0x75, 0x00, 0x23, 0xD4, 0x20, 0xE0, 0xBD, 0xD3, 
    0x47, 0xC6, 0x76, 0x6C, 0xDD, 0x48, 0x45, 0x93, 0x7C, 0x57, 0x55, 0x86, 0xAB, 0x40, 0xA9, 0x98, 
    0x9E, 0xA7, 0xB9, 0xC5, 0x6F, 0xA3, 0x29, 0xE3, 0x88, 0xEC, 0xD1, 0x21, 0xB4, 0xDB, 0x92, 0xC9, 
    0x69, 0xC7, 0xEF, 0xCE, 0x79, 0x4C, 0xCC, 0x82, 0xEE, 0x2A, 0xA1, 0x25, 0x3A, 0x96, 0xF9, 0xB3, 
    0x59, 0xD5, 0xFC, 0x77, 0xAF, 0x71, 0x84, 0x7B, 0x2E, 0xE7, 0xD2, 0x8B, 0x7D, 0xBE, 0x6D, 0x85, 
    0x62, 0xD6, 0x01, 0x05, 0xE8, 0x09, 0x08, 0x04, 0x2B, 0xA4, 0x5D, 0x4A, 0x4F, 0x5E, 0x36, 0xA0, 
    0xF8, 0x94, 0xFE, 0x87, 0xC1, 0x97, 0x3C, 0xE4, 0x73, 0xF0, 0xE1, 0x18, 0x4D, 0x67, 0x27, 0xC3, 
    0x24, 0xBC, 0x5B, 0xC0, 0x19, 0x14, 0x9C, 0x39, 0xB1, 0xF6, 0xFD, 0xE9, 0xCF, 0xB8, 0xD8, 0x0A, 
    0xAD, 0x66, 0xA8, 0x22, 0x0E, 0x2C, 0x60, 0xF3, 0x3D, 0x61, 0x11, 0x2F, 0x83, 0xAE, 0xC8, 0x56, 
    0x80, 0xBF, 0x9D, 0x7F, 0x6B, 0x3F, 0xCD, 0x0F, 0x72, 0x74, 0xC2, 0x0C, 0xD7, 0xB5, 0xE2, 0x89, 
    0xF4, 0xF1, 0x6A, 0x5A, 0x26, 0xB7, 0x1F, 0x17, 0xED, 0x8C, 0x53, 0x03, 0x78, 0x9F, 0x63, 0xDE, 
    0xB0, 0x3E, 0x7A, 0x4E, 0x0D, 0x41, 0x51, 0x10, 0xA2, 0x95, 0x7E, 0x1B, 0xCA, 0xC4, 0xE5, 0x52, 
    0x1C, 0xFF, 0xE6, 0x6E, 0xDC, 0xEA, 0xF2, 0x1D, 0x1A, 0xCB, 0x3B, 0xA6, 0x12, 0x64, 0xBA, 0x81, 
    0x37, 0x13, 0x42, 0xFA, 0x38, 0xF7, 0x30, 0x02, 0x54, 0xAC, 0xA5, 0x15, 0x32, 0x9A, 0x8D, 0x34, 
    0x91, 0x35, 0x44, 0x0B, 0x65, 0x43, 0x5F, 0x8F, 0x49, 0xF5, 0x58, 0xAA, 0x33, 0x2D, 0x07, 0x8E
};

const std::uint8_t TwistExpander_Flyyyyy::kSBoxD[S_SBOX] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const std::uint8_t TwistExpander_Flyyyyy::kSaltA[S_SALT] = {
    0xA5, 0xF4, 0x59, 0x9D, 0xE8, 0x16, 0x61, 0xC3, 0xCB, 0x7E, 0xD2, 0xED, 0x2C, 0xA1, 0xDC, 0xAE, 
    0x19, 0x51, 0x07, 0x03, 0x5B, 0xBD, 0x1B, 0x01, 0x42, 0xBF, 0x98, 0xAB, 0xF7, 0x06, 0xD9, 0x43, 
    0x93, 0x32, 0x2D, 0xDE, 0x55, 0x11, 0x68, 0x87, 0xBC, 0xF1, 0x0A, 0xF6, 0xC7, 0x7B, 0x3C, 0x82, 
    0xFE, 0x2A, 0x5E, 0x5D, 0xB1, 0xAD, 0x09, 0x69, 0xE0, 0xEB, 0xB8, 0x63, 0x27, 0x29, 0x13, 0xB0, 
    0xCD, 0xA6, 0x84, 0x58, 0xAF, 0x96, 0xA3, 0x71, 0x10, 0x66, 0x56, 0xE4, 0x04, 0x94, 0x9A, 0xCF, 
    0xE7, 0x3F, 0x89, 0xB9, 0x40, 0xC5, 0x0D, 0x54, 0xE1, 0xEE, 0xD7, 0x8F, 0x28, 0xB6, 0x4A, 0x99, 
    0xC8, 0x50, 0x2B, 0xAC, 0x6C, 0x41, 0xD5, 0x38, 0xF2, 0x45, 0x60, 0xEA, 0xF9, 0x0F, 0xE9, 0xC2, 
    0x0E, 0x6F, 0x72, 0x62, 0xDA, 0x00, 0x78, 0x81, 0x3E, 0x64, 0x92, 0xB7, 0x02, 0xDB, 0x1F, 0x21
};

const std::uint8_t TwistExpander_Flyyyyy::kSaltB[S_SALT] = {
    0x55, 0xBA, 0xED, 0x5B, 0xF9, 0x64, 0xE3, 0xF8, 0xC0, 0x89, 0xD2, 0xA8, 0x4F, 0x08, 0x92, 0x44, 
    0x28, 0xD5, 0x4E, 0x13, 0xC8, 0x4B, 0x0D, 0x36, 0xDA, 0x6E, 0x0A, 0x15, 0x93, 0x42, 0xC3, 0xD6, 
    0x1B, 0x29, 0x43, 0xB7, 0x88, 0x10, 0xD7, 0x60, 0x47, 0xEB, 0x27, 0x9F, 0xA5, 0xD8, 0x69, 0x31, 
    0xA7, 0x37, 0x99, 0xB0, 0x2E, 0x6D, 0x0E, 0xDB, 0xC1, 0xD0, 0x1A, 0xC6, 0x5E, 0x1E, 0x35, 0x22, 
    0xB8, 0x7C, 0x49, 0xE1, 0x48, 0x4A, 0x2D, 0xC5, 0x1C, 0x97, 0xA1, 0x9C, 0x67, 0x6F, 0x2F, 0x9D, 
    0xB5, 0x40, 0x20, 0x16, 0xFF, 0x80, 0x53, 0x84, 0xCD, 0x81, 0x83, 0x58, 0x12, 0x5A, 0xE2, 0x06, 
    0xE9, 0xD9, 0x3B, 0x61, 0x8A, 0xF3, 0xA6, 0x34, 0xC2, 0xAE, 0xEF, 0xDC, 0xD3, 0x76, 0xF5, 0xF4, 
    0x7A, 0xA3, 0xEC, 0xCA, 0xEA, 0x3A, 0xF1, 0x18, 0x4C, 0xB2, 0x17, 0x56, 0x1F, 0xE6, 0x50, 0x26
};

const std::uint8_t TwistExpander_Flyyyyy::kSaltC[S_SALT] = {
    0xC3, 0x78, 0x80, 0xAB, 0xBC, 0x42, 0xE8, 0xA5, 0xE1, 0x20, 0x69, 0xF0, 0x97, 0x3E, 0x6D, 0x8B, 
    0xF7, 0xC4, 0x8E, 0x73, 0x3D, 0xFA, 0x4A, 0xCE, 0xB5, 0x47, 0xE7, 0xC7, 0x74, 0x3F, 0x21, 0x04, 
    0xAF, 0xEE, 0x26, 0xD0, 0x39, 0xB3, 0x1D, 0x2F, 0x4B, 0x34, 0x86, 0xC5, 0x58, 0x6C, 0xEA, 0x63, 
    0x81, 0xD7, 0x71, 0x7D, 0x43, 0x87, 0x8A, 0xAC, 0xF8, 0xB0, 0xB6, 0x55, 0xDF, 0xDB, 0x0E, 0xF2, 
    0xA1, 0xCF, 0x23, 0x31, 0x3A, 0xE3, 0x83, 0x95, 0x02, 0x19, 0x35, 0x05, 0x82, 0x5E, 0x5A, 0x1C, 
    0x09, 0xE6, 0x2B, 0x07, 0x08, 0xE5, 0x77, 0x88, 0x49, 0xFC, 0x25, 0x6B, 0x6A, 0xB2, 0xF1, 0x56, 
    0xED, 0x75, 0x66, 0x36, 0xE0, 0xB9, 0x98, 0x17, 0x38, 0x44, 0x59, 0x01, 0x70, 0xCA, 0x0D, 0x14, 
    0x7B, 0xEC, 0x99, 0xE4, 0x2D, 0x7F, 0x60, 0x6F, 0x57, 0xA4, 0x1E, 0x03, 0xC2, 0x9C, 0xB7, 0x9F
};

const std::uint8_t TwistExpander_Flyyyyy::kSaltD[S_SALT] = {
    0xAC, 0x5F, 0x07, 0x01, 0xA5, 0xD9, 0xAA, 0xED, 0x32, 0x94, 0xDF, 0xB2, 0xA3, 0xA8, 0xD3, 0xB7, 
    0xC6, 0xD8, 0xEB, 0xEF, 0xD0, 0x52, 0x11, 0x38, 0x3D, 0x85, 0x16, 0xA0, 0x9B, 0xF8, 0xC4, 0x2E, 
    0x0D, 0x8E, 0xB0, 0x04, 0xDC, 0x19, 0x66, 0x28, 0x41, 0x48, 0x23, 0x88, 0xE9, 0x83, 0x82, 0x3F, 
    0x57, 0x70, 0xC7, 0x03, 0x89, 0x73, 0xE7, 0x75, 0x7A, 0x61, 0x45, 0x7D, 0xB8, 0xB4, 0xCE, 0x9A, 
    0xE1, 0x68, 0xEC, 0x14, 0xA4, 0x1A, 0x7F, 0x8B, 0x0A, 0x6A, 0x18, 0x7E, 0x2F, 0xCD, 0xC1, 0x36, 
    0xBA, 0x46, 0x24, 0x56, 0xEA, 0x3E, 0xE3, 0xCF, 0x69, 0x76, 0xC8, 0xF7, 0xA7, 0x49, 0xE2, 0x05, 
    0xF6, 0xCB, 0x63, 0x60, 0x62, 0xF9, 0x1F, 0x86, 0x8C, 0xFD, 0xB9, 0x72, 0x97, 0x34, 0xB6, 0xFE, 
    0x27, 0xA6, 0x4F, 0x6E, 0x00, 0x2B, 0x47, 0xFB, 0x55, 0xBD, 0x1E, 0x5A, 0xFC, 0x13, 0x15, 0x6B
};

TwistExpander_Flyyyyy::TwistExpander_Flyyyyy()
: TwistExpander() {
    mSBoxA = const_cast<std::uint8_t*>(kSBoxA);
    mSBoxB = const_cast<std::uint8_t*>(kSBoxB);
    mSBoxC = const_cast<std::uint8_t*>(kSBoxC);
    mSBoxD = const_cast<std::uint8_t*>(kSBoxD);
    mSaltA = const_cast<std::uint8_t*>(kSaltA);
    mSaltB = const_cast<std::uint8_t*>(kSaltB);
    mSaltC = const_cast<std::uint8_t*>(kSaltC);
    mSaltD = const_cast<std::uint8_t*>(kSaltD);
}

void TwistExpander_Flyyyyy::Seed(TwistWorkSpace *pWorkspace,
                                 std::uint8_t *pSource,
                                 std::uint8_t *pPassword,
                                 unsigned int pPasswordByteLength) {
    TwistExpander::Seed(pWorkspace, pSource, pPassword, pPasswordByteLength);
    if (pWorkspace == nullptr) { return; }
    [[maybe_unused]] std::uint8_t *aSource = TwistWorkSpace::GetBuffer(pWorkspace, static_cast<TwistWorkSpaceSlot>(0));
    [[maybe_unused]] std::uint8_t *aDestination = TwistWorkSpace::GetBuffer(pWorkspace, static_cast<TwistWorkSpaceSlot>(1));
    [[maybe_unused]] std::uint8_t *aSaltB = pWorkspace->mSaltB;
    [[maybe_unused]] std::uint8_t *aSaltC = pWorkspace->mSaltC;
    [[maybe_unused]] std::uint8_t *aSaltD = pWorkspace->mSaltD;
    [[maybe_unused]] std::uint8_t *aSBoxA = pWorkspace->mSBoxA;
    [[maybe_unused]] std::uint8_t *aSBoxB = pWorkspace->mSBoxB;
    [[maybe_unused]] std::uint8_t *aSBoxC = pWorkspace->mSBoxC;
    [[maybe_unused]] std::uint8_t *aSBoxD = pWorkspace->mSBoxD;
    [[maybe_unused]] std::uint8_t *aExpandLaneA = pWorkspace->mExpandLaneA;
    [[maybe_unused]] std::uint8_t *aExpandLaneB = pWorkspace->mExpandLaneB;
    [[maybe_unused]] std::uint8_t *aExpandLaneC = pWorkspace->mExpandLaneC;
    [[maybe_unused]] std::uint8_t *aExpandLaneD = pWorkspace->mExpandLaneD;
    [[maybe_unused]] std::uint8_t *aWorkLaneA = pWorkspace->mWorkLaneA;
    [[maybe_unused]] std::uint8_t *aWorkLaneB = pWorkspace->mWorkLaneB;
    [[maybe_unused]] std::uint8_t *aWorkLaneC = pWorkspace->mWorkLaneC;
    [[maybe_unused]] std::uint8_t *aWorkLaneD = pWorkspace->mWorkLaneD;
    [[maybe_unused]] std::uint8_t *aOperationLaneA = pWorkspace->mOperationLaneA;
    [[maybe_unused]] std::uint8_t *aOperationLaneB = pWorkspace->mOperationLaneB;
    [[maybe_unused]] std::uint8_t *aMaskLaneA = pWorkspace->mMaskLaneA;
    [[maybe_unused]] std::uint8_t *aMaskLaneB = pWorkspace->mMaskLaneB;
    [[maybe_unused]] std::uint8_t *aKeyBoxUnrolledA = &(pWorkspace->mKeyBoxA[0][0]);
    [[maybe_unused]] std::uint8_t *aKeyBoxUnrolledB = &(pWorkspace->mKeyBoxB[0][0]);
    [[maybe_unused]] std::uint8_t *aKeyRowReadA = &(pWorkspace->mKeyBoxA[H_KEY_A - 1][0]);
    [[maybe_unused]] std::uint8_t *aKeyRowReadB = &(pWorkspace->mKeyBoxB[H_KEY_B - 1][0]);
    [[maybe_unused]] std::uint8_t *aKeyRowWriteA = &(pWorkspace->mKeyBoxA[0][0]);
    [[maybe_unused]] std::uint8_t *aKeyRowWriteB = &(pWorkspace->mKeyBoxB[0][0]);
    [[maybe_unused]] std::uint8_t *aMaskBoxUnrolledA = &(pWorkspace->mMaskBoxA[0][0]);
    [[maybe_unused]] std::uint8_t *aMaskBoxUnrolledB = &(pWorkspace->mMaskBoxB[0][0]);
    [[maybe_unused]] std::uint8_t *aMaskRowReadA = &(pWorkspace->mMaskBoxA[H_MASK_A - 1][0]);
    [[maybe_unused]] std::uint8_t *aMaskRowReadB = &(pWorkspace->mMaskBoxB[H_MASK_B - 1][0]);
    [[maybe_unused]] std::uint8_t *aMaskRowWriteA = &(pWorkspace->mMaskBoxA[0][0]);
    [[maybe_unused]] std::uint8_t *aMaskRowWriteB = &(pWorkspace->mMaskBoxB[0][0]);
    std::uint8_t aSourceByteA = 0;
    std::size_t aOracleSourceA = 0;
    std::uint8_t aSaltByte = 0;
    std::uint16_t aValueKey = 0;
    std::uint8_t aValue = 0;
    std::uint16_t aPermuteKey = 0;
    std::uint8_t aPermute = 0;
    std::size_t aOraclePermuteSBox = 0;
    std::uint8_t aSourceByteB = 0;
    std::size_t aOracleSourceB = 0;
    std::size_t aOracleCarrySalt = 0;
    std::uint16_t aCarryKey = 0;
    std::size_t aOracleCarrySBox = 0;
    std::size_t aOraclePermuteSalt = 0;
    std::size_t aOracleValueSalt = 0;

    std::uint8_t *aSaltA = pWorkspace->mSaltA;
    std::uint8_t aCarry = 0xDC;

    {
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 2852U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = RotL8(aSource[(aOracleSourceA)], 1U);
                aSaltByte = aSaltC[((aSourceByteA) & 0x7FU)];
                aValueKey = ((((aValue) ^ (aSourceByteA))) ^ (aSaltByte));
                aValue = aSBoxA[((aValueKey) & 0xFFU)];
                aSaltByte = aSaltB[((aValue) & 0x7FU)];
                aPermuteKey = ((RotL8(aValueKey, 4U)) + (17U));
                aOraclePermuteSBox = aLoopIndex + 182U;
                if (aOraclePermuteSBox >= S_SBOX) { aOraclePermuteSBox -= S_SBOX; }
                aOraclePermuteSBox &= 0xFFU;
                aPermute = ((aPermuteKey) ^ (aSBoxC[((aOraclePermuteSBox) & 0xFFU)]));
                aPermute += aSaltByte;
                aValue = RotL8(aValue, aPermute);
                aWorkLaneA[(aLoopIndex)] = aValue;
            }
        
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 2761U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((RotL8(aWorkLaneA[(aOracleSourceA)], 7U)) + (23U));
                aOracleSourceB = aLoopIndex + 2633U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = ((((aSource[(aOracleSourceB)]) * (245U))) - (176U));
                aSaltByte = aSaltD[((aSourceByteB) & 0x7FU)];
                aValueKey = ((((aValue) ^ (aSourceByteA))) ^ (aSourceByteB));
                aValue = aSBoxB[((aValueKey) & 0xFFU)];
                aValue ^= aSaltByte;
                aWorkLaneB[(aLoopIndex)] = aValue;
            }
        
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 988U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = RotL8(aWorkLaneB[(aOracleSourceA)], 6U);
                aOracleSourceB = aLoopIndex + 1424U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = ((RotL8(aWorkLaneA[(aOracleSourceB)], 2U)) + (183U));
                aSaltByte = aSaltA[((aSourceByteB) & 0x7FU)];
                aValueKey = ((aValue) + (aSourceByteA));
                aValue = aSBoxC[((aValueKey) & 0xFFU)];
                aValue += ((aSaltByte) + (aCarry));
                aOracleCarrySalt = aLoopIndex + 46U;
                if (aOracleCarrySalt >= S_SALT) { aOracleCarrySalt -= S_SALT; }
                aOracleCarrySalt &= 0x7FU;
                aSaltByte = aSaltA[((aOracleCarrySalt) & 0x7FU)];
                aCarryKey = aCarry;
                aOracleCarrySBox = aLoopIndex + 102U;
                if (aOracleCarrySBox >= S_SBOX) { aOracleCarrySBox -= S_SBOX; }
                aOracleCarrySBox &= 0xFFU;
                aCarry = ((aCarryKey) ^ (aSBoxD[((aOracleCarrySBox) & 0xFFU)]));
                aCarry ^= ((((aValue) + (aSourceByteB))) + (aSaltByte));
                aSaltByte = aSaltB[((aValue) & 0x7FU)];
                aPermuteKey = ((aValueKey) + (aSaltByte));
                aPermute = aPermuteKey;
                aPermute ^= aCarryKey;
                aValue = RotL8(aValue, aPermute);
                aWorkLaneC[(aLoopIndex)] = aValue;
            }
        
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 4809U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((RotL8(aWorkLaneC[(aOracleSourceA)], 7U)) + (9U));
                aOracleSourceB = aLoopIndex + 5004U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = ((aWorkLaneB[(aOracleSourceB)]) + (106U));
                aSaltByte = aSaltB[((aSourceByteA) & 0x7FU)];
                aValueKey = ((aValue) ^ (aSourceByteA));
                aValue = aSBoxD[((aValueKey) & 0xFFU)];
                aValue ^= aSaltByte;
                aOraclePermuteSalt = aLoopIndex + 36U;
                if (aOraclePermuteSalt >= S_SALT) { aOraclePermuteSalt -= S_SALT; }
                aOraclePermuteSalt &= 0x7FU;
                aSaltByte = aSaltC[((aOraclePermuteSalt) & 0x7FU)];
                aPermuteKey = RotL8(((aSourceByteB) ^ (164U)), 3U);
                aOraclePermuteSBox = aLoopIndex + 243U;
                if (aOraclePermuteSBox >= S_SBOX) { aOraclePermuteSBox -= S_SBOX; }
                aOraclePermuteSBox &= 0xFFU;
                aPermute = ((aPermuteKey) ^ (aSBoxA[((aOraclePermuteSBox) & 0xFFU)]));
                aPermute += ((aValueKey) + (aSaltByte));
                aValue = RotL8(aValue, aPermute);
                aWorkLaneD[(aLoopIndex)] = aValue;
            }
    }
    {
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 2272U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((((aWorkLaneD[(aOracleSourceA)]) * (35U))) ^ (239U));
                aOracleSourceB = aLoopIndex + 7358U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = RotL8(((aWorkLaneC[(aOracleSourceB)]) - (29U)), 3U);
                aSaltByte = aSaltB[((aCarry) & 0x7FU)];
                aValueKey = ((((aValue) + (aSourceByteA))) + (aSaltByte));
                aValue = aSBoxC[((aValueKey) & 0xFFU)];
                aValue ^= ((aCarry) ^ (aSourceByteB));
                aSaltByte = aSaltD[((aSourceByteA) & 0x7FU)];
                aCarryKey = ((aCarry) + (aValue));
                aOracleCarrySBox = aLoopIndex + 101U;
                if (aOracleCarrySBox >= S_SBOX) { aOracleCarrySBox -= S_SBOX; }
                aOracleCarrySBox &= 0xFFU;
                aCarry = ((aCarryKey) ^ (aSBoxD[((aOracleCarrySBox) & 0xFFU)]));
                aCarry ^= aSaltByte;
                aWorkLaneA[(aLoopIndex)] = aValue;
            }
        
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 5855U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((RotL8(aWorkLaneA[(aOracleSourceA)], 2U)) + (156U));
                aOracleSourceB = aLoopIndex + 2434U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = ((((aWorkLaneD[(aOracleSourceB)]) * (11U))) - (106U));
                aSaltByte = aSaltC[((aSourceByteA) & 0x7FU)];
                aValueKey = ((aValue) + (aSourceByteA));
                aValue = aSBoxD[((aValueKey) & 0xFFU)];
                aValue ^= ((((aSaltByte) ^ (aCarry))) + (aSourceByteB));
                aSaltByte = aSaltC[((aValue) & 0x7FU)];
                aCarryKey = ((aCarry) + (aSaltByte));
                aOracleCarrySBox = aLoopIndex + 129U;
                if (aOracleCarrySBox >= S_SBOX) { aOracleCarrySBox -= S_SBOX; }
                aOracleCarrySBox &= 0xFFU;
                aCarry = ((aCarryKey) ^ (aSBoxA[((aOracleCarrySBox) & 0xFFU)]));
                aCarry += aValue;
                aWorkLaneB[(aLoopIndex)] = aValue;
            }
        
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 3593U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((aWorkLaneB[(aOracleSourceA)]) - (230U));
                aOracleSourceB = aLoopIndex + 3936U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = ((RotL8(aWorkLaneA[(aOracleSourceB)], 4U)) + (86U));
                aOracleValueSalt = aLoopIndex + 95U;
                if (aOracleValueSalt >= S_SALT) { aOracleValueSalt -= S_SALT; }
                aOracleValueSalt &= 0x7FU;
                aSaltByte = aSaltD[((aOracleValueSalt) & 0x7FU)];
                aValueKey = ((((aValue) ^ (aSourceByteA))) + (aSourceByteB));
                aValue = aSBoxA[((aValueKey) & 0xFFU)];
                aValue ^= aSaltByte;
                aWorkLaneC[(aLoopIndex)] = aValue;
            }
        
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 7300U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((aWorkLaneC[(aOracleSourceA)]) - (118U));
                aOracleSourceB = aLoopIndex + 1161U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = RotL8(((aWorkLaneB[(aOracleSourceB)]) * (141U)), 3U);
                aOracleValueSalt = aLoopIndex + 18U;
                if (aOracleValueSalt >= S_SALT) { aOracleValueSalt -= S_SALT; }
                aOracleValueSalt &= 0x7FU;
                aSaltByte = aSaltA[((aOracleValueSalt) & 0x7FU)];
                aValueKey = ((aValue) ^ (aSourceByteA));
                aValue = aSBoxB[((aValueKey) & 0xFFU)];
                aValue ^= ((aSaltByte) ^ (aSourceByteB));
                aSaltByte = aSaltB[((aValue) & 0x7FU)];
                aPermuteKey = ((((aValueKey) * (235U))) - (25U));
                aOraclePermuteSBox = aLoopIndex + 2U;
                if (aOraclePermuteSBox >= S_SBOX) { aOraclePermuteSBox -= S_SBOX; }
                aOraclePermuteSBox &= 0xFFU;
                aPermute = ((aPermuteKey) ^ (aSBoxC[((aOraclePermuteSBox) & 0xFFU)]));
                aPermute += aSaltByte;
                aValue = RotL8(aValue, aPermute);
                aWorkLaneD[(aLoopIndex)] = aValue;
            }
    }
}

void TwistExpander_Flyyyyy::TwistBlock(TwistWorkSpace *pWorkspace,
                                       std::uint8_t *pSource,
                                       std::uint8_t *pDestination) {
    TwistExpander::TwistBlock(pWorkspace, pSource, pDestination);
    if ((pWorkspace == nullptr) || (pDestination == nullptr)) { return; }
    [[maybe_unused]] std::uint8_t *aSource = TwistWorkSpace::GetBuffer(pWorkspace, static_cast<TwistWorkSpaceSlot>(0));
    [[maybe_unused]] std::uint8_t *aDestination = TwistWorkSpace::GetBuffer(pWorkspace, static_cast<TwistWorkSpaceSlot>(1));
    [[maybe_unused]] std::uint8_t *aSaltB = pWorkspace->mSaltB;
    [[maybe_unused]] std::uint8_t *aSaltC = pWorkspace->mSaltC;
    [[maybe_unused]] std::uint8_t *aSaltD = pWorkspace->mSaltD;
    [[maybe_unused]] std::uint8_t *aSBoxA = pWorkspace->mSBoxA;
    [[maybe_unused]] std::uint8_t *aSBoxB = pWorkspace->mSBoxB;
    [[maybe_unused]] std::uint8_t *aSBoxC = pWorkspace->mSBoxC;
    [[maybe_unused]] std::uint8_t *aSBoxD = pWorkspace->mSBoxD;
    [[maybe_unused]] std::uint8_t *aExpandLaneA = pWorkspace->mExpandLaneA;
    [[maybe_unused]] std::uint8_t *aExpandLaneB = pWorkspace->mExpandLaneB;
    [[maybe_unused]] std::uint8_t *aExpandLaneC = pWorkspace->mExpandLaneC;
    [[maybe_unused]] std::uint8_t *aExpandLaneD = pWorkspace->mExpandLaneD;
    [[maybe_unused]] std::uint8_t *aWorkLaneA = pWorkspace->mWorkLaneA;
    [[maybe_unused]] std::uint8_t *aWorkLaneB = pWorkspace->mWorkLaneB;
    [[maybe_unused]] std::uint8_t *aWorkLaneC = pWorkspace->mWorkLaneC;
    [[maybe_unused]] std::uint8_t *aWorkLaneD = pWorkspace->mWorkLaneD;
    [[maybe_unused]] std::uint8_t *aOperationLaneA = pWorkspace->mOperationLaneA;
    [[maybe_unused]] std::uint8_t *aOperationLaneB = pWorkspace->mOperationLaneB;
    [[maybe_unused]] std::uint8_t *aMaskLaneA = pWorkspace->mMaskLaneA;
    [[maybe_unused]] std::uint8_t *aMaskLaneB = pWorkspace->mMaskLaneB;
    [[maybe_unused]] std::uint8_t *aKeyBoxUnrolledA = &(pWorkspace->mKeyBoxA[0][0]);
    [[maybe_unused]] std::uint8_t *aKeyBoxUnrolledB = &(pWorkspace->mKeyBoxB[0][0]);
    [[maybe_unused]] std::uint8_t *aKeyRowReadA = &(pWorkspace->mKeyBoxA[H_KEY_A - 1][0]);
    [[maybe_unused]] std::uint8_t *aKeyRowReadB = &(pWorkspace->mKeyBoxB[H_KEY_B - 1][0]);
    [[maybe_unused]] std::uint8_t *aKeyRowWriteA = &(pWorkspace->mKeyBoxA[0][0]);
    [[maybe_unused]] std::uint8_t *aKeyRowWriteB = &(pWorkspace->mKeyBoxB[0][0]);
    [[maybe_unused]] std::uint8_t *aMaskBoxUnrolledA = &(pWorkspace->mMaskBoxA[0][0]);
    [[maybe_unused]] std::uint8_t *aMaskBoxUnrolledB = &(pWorkspace->mMaskBoxB[0][0]);
    [[maybe_unused]] std::uint8_t *aMaskRowReadA = &(pWorkspace->mMaskBoxA[H_MASK_A - 1][0]);
    [[maybe_unused]] std::uint8_t *aMaskRowReadB = &(pWorkspace->mMaskBoxB[H_MASK_B - 1][0]);
    [[maybe_unused]] std::uint8_t *aMaskRowWriteA = &(pWorkspace->mMaskBoxA[0][0]);
    [[maybe_unused]] std::uint8_t *aMaskRowWriteB = &(pWorkspace->mMaskBoxB[0][0]);
    std::uint8_t aSourceByteA = 0;
    std::size_t aOracleSourceA = 0;
    std::uint8_t aSourceByteB = 0;
    std::size_t aOracleSourceB = 0;
    std::uint8_t aSaltByte = 0;
    std::uint16_t aValueKey = 0;
    std::uint8_t aValue = 0;
    std::uint16_t aCarryKey = 0;
    std::size_t aOracleCarrySBox = 0;
    std::size_t aOracleValueSalt = 0;
    std::uint16_t aPermuteKey = 0;
    std::uint8_t aPermute = 0;
    std::size_t aOraclePermuteSBox = 0;

    std::uint8_t *aSaltA = pWorkspace->mSaltA;
    std::uint8_t aCarry = 0xFA;

    {
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 2272U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((((aWorkLaneD[(aOracleSourceA)]) * (35U))) ^ (239U));
                aOracleSourceB = aLoopIndex + 7358U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = RotL8(((aWorkLaneC[(aOracleSourceB)]) - (29U)), 3U);
                aSaltByte = aSaltB[((aCarry) & 0x7FU)];
                aValueKey = ((((aValue) + (aSourceByteA))) + (aSaltByte));
                aValue = aSBoxC[((aValueKey) & 0xFFU)];
                aValue ^= ((aCarry) ^ (aSourceByteB));
                aSaltByte = aSaltD[((aSourceByteA) & 0x7FU)];
                aCarryKey = ((aCarry) + (aValue));
                aOracleCarrySBox = aLoopIndex + 101U;
                if (aOracleCarrySBox >= S_SBOX) { aOracleCarrySBox -= S_SBOX; }
                aOracleCarrySBox &= 0xFFU;
                aCarry = ((aCarryKey) ^ (aSBoxD[((aOracleCarrySBox) & 0xFFU)]));
                aCarry ^= aSaltByte;
                aWorkLaneA[(aLoopIndex)] = aValue;
            }
        
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 5855U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((RotL8(aWorkLaneA[(aOracleSourceA)], 2U)) + (156U));
                aOracleSourceB = aLoopIndex + 2434U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = ((((aWorkLaneD[(aOracleSourceB)]) * (11U))) - (106U));
                aSaltByte = aSaltC[((aSourceByteA) & 0x7FU)];
                aValueKey = ((aValue) + (aSourceByteA));
                aValue = aSBoxD[((aValueKey) & 0xFFU)];
                aValue ^= ((((aSaltByte) ^ (aCarry))) + (aSourceByteB));
                aSaltByte = aSaltC[((aValue) & 0x7FU)];
                aCarryKey = ((aCarry) + (aSaltByte));
                aOracleCarrySBox = aLoopIndex + 129U;
                if (aOracleCarrySBox >= S_SBOX) { aOracleCarrySBox -= S_SBOX; }
                aOracleCarrySBox &= 0xFFU;
                aCarry = ((aCarryKey) ^ (aSBoxA[((aOracleCarrySBox) & 0xFFU)]));
                aCarry += aValue;
                aWorkLaneB[(aLoopIndex)] = aValue;
            }
        
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 3593U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((aWorkLaneB[(aOracleSourceA)]) - (230U));
                aOracleSourceB = aLoopIndex + 3936U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = ((RotL8(aWorkLaneA[(aOracleSourceB)], 4U)) + (86U));
                aOracleValueSalt = aLoopIndex + 95U;
                if (aOracleValueSalt >= S_SALT) { aOracleValueSalt -= S_SALT; }
                aOracleValueSalt &= 0x7FU;
                aSaltByte = aSaltD[((aOracleValueSalt) & 0x7FU)];
                aValueKey = ((((aValue) ^ (aSourceByteA))) + (aSourceByteB));
                aValue = aSBoxA[((aValueKey) & 0xFFU)];
                aValue ^= aSaltByte;
                aWorkLaneC[(aLoopIndex)] = aValue;
            }
        
            for (std::size_t aLoopIndex = 0U; aLoopIndex < static_cast<std::size_t>(S_BLOCK); aLoopIndex += 1U) {
                aOracleSourceA = aLoopIndex + 7300U;
                if (aOracleSourceA >= S_BLOCK) { aOracleSourceA -= S_BLOCK; }
                aSourceByteA = ((aWorkLaneC[(aOracleSourceA)]) - (118U));
                aOracleSourceB = aLoopIndex + 1161U;
                if (aOracleSourceB >= S_BLOCK) { aOracleSourceB -= S_BLOCK; }
                aSourceByteB = RotL8(((aWorkLaneB[(aOracleSourceB)]) * (141U)), 3U);
                aOracleValueSalt = aLoopIndex + 18U;
                if (aOracleValueSalt >= S_SALT) { aOracleValueSalt -= S_SALT; }
                aOracleValueSalt &= 0x7FU;
                aSaltByte = aSaltA[((aOracleValueSalt) & 0x7FU)];
                aValueKey = ((aValue) ^ (aSourceByteA));
                aValue = aSBoxB[((aValueKey) & 0xFFU)];
                aValue ^= ((aSaltByte) ^ (aSourceByteB));
                aSaltByte = aSaltB[((aValue) & 0x7FU)];
                aPermuteKey = ((((aValueKey) * (235U))) - (25U));
                aOraclePermuteSBox = aLoopIndex + 2U;
                if (aOraclePermuteSBox >= S_SBOX) { aOraclePermuteSBox -= S_SBOX; }
                aOraclePermuteSBox &= 0xFFU;
                aPermute = ((aPermuteKey) ^ (aSBoxC[((aOraclePermuteSBox) & 0xFFU)]));
                aPermute += aSaltByte;
                aValue = RotL8(aValue, aPermute);
                aWorkLaneD[(aLoopIndex)] = aValue;
            }
    }
    std::memcpy(pDestination, pWorkspace->mWorkLaneD, S_BLOCK);
}
