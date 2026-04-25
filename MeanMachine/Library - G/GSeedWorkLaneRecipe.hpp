//
//  GSeedWorkLaneRecipe.hpp
//  MeanMachine
//
//  Created by John Snow on 4/22/26.
//

#ifndef GSeedWorkLaneRecipe_hpp
#define GSeedWorkLaneRecipe_hpp

#include "GSeedProgram.hpp"
//#include "TwistSeedBuilder.hpp"
#include "GSeedProgram.hpp"
#include "GSymbol.hpp"
#include "GStatementRecipe.hpp"
#include "GQuick.hpp"

#include "Random.hpp"
#include <vector>
#include <string>

#define SEED_WORK_LANE_COUNT 4

#define SEED_WORK_LANE_CARRY_MIN 1
#define SEED_WORK_LANE_CARRY_MAX 3

#define SEED_WORK_LANE_PERMUTE_MIN 1
#define SEED_WORK_LANE_PERMUTE_MAX 3

// Pass1:
// (       src ) => w_a
// (src? + w_a ) => w_b
// (w_a? + w_b ) => w_c
// (w_b? + w_c ) => w_d

// Pass2:
// (w_c? + w_d ) => w_a
// (w_d? + w_a ) => w_b
// (w_a? + w_b ) => w_c
// (w_b? + w_c ) => w_d

// Pass1:
//
// SBoxes:
// L1: must A, not D  → {A,B,C}
// L2: must B, not A  → {B,C,D}
// L3: must C, not B  → {C,D,A}
// L4: must D, not C  → {D,A,B}
//
// Salts:
// L1: must C, not A  → {C,B,D}
// L2: must D, not B  → {D,C,A}
// L3: must A, not C  → {A,D,B}
// L4: must B, not D  → {B,A,C}

// Pass2:
//
// SBoxes:
// L1: must C, not A  → {C,B,D}
// L2: must D, not B  → {D,C,A}
// L3: must A, not C  → {A,D,B}
// L4: must B, not D  → {B,A,C}
//
// Salts:
// L1: must B, not C  → {B,A,D}
// L2: must C, not D  → {C,A,B}
// L3: must D, not B  → {D,C,A}
// L4: must A, not C  → {A,B,D}

enum class LoopPermuteAndCarryOrdering : std::uint8_t {
    kValueCarryPermute = 0,
    kValuePermuteCarry = 1,
};

enum class LoopKeyOrdering : std::uint8_t {
    kBeforeSBox = 0,
    kAfterSBox = 1,
};

enum class LoopSBoxBehavior : std::uint8_t {
    kInv = 0,
    
    /*
     aCarry += (aValue * 3);
     aCarry ^= s_box[aLoopIndex];
     */
    /*
     aPermute += (aValue - 9)
     aPermute += s_box[aLoopIndex];
     */
    kMixWithLoopIndex = 1,
    
    /*
     aCarry += (aValue + 11);
     aCarry = s_box[aCarry];
     */
    /*
     aPermute -= (aValue * 11);
     aPermute = s_box[aPermute];
     */
    kPassSelfThrough = 2,
    
};

enum class LoopSaltBehavior : std::uint8_t {
    kInv = 0,
    kLoopIndex = 1,
    kSourceA = 2,
    kSourceB = 3,
    kValue = 4,
    kCarry = 5
};

enum class LoopSecondSourceMode : std::uint8_t {
    kInv = 0, // not enabled
    kValue = 1,
    kCarry = 2,
    kPermute = 3
};

struct GSeedWorkLaneRecipeLoopContract {
    
    
    LoopSaltBehavior                    mValueSaltBehavior;
    LoopKeyOrdering                     mValueKeyOrdering_Carry;
    LoopKeyOrdering                     mValueKeyOrdering_SourceB;
    LoopKeyOrdering                     mValueKeyOrdering_Salt;
    
    bool                                mCarryEnabled;
    LoopSaltBehavior                    mCarrySaltBehavior;
    LoopSBoxBehavior                    mCarrySBoxBehavior;
    LoopKeyOrdering                     mCarryKeyOrdering_Value;
    LoopKeyOrdering                     mCarryKeyOrdering_SourceB;
    LoopKeyOrdering                     mCarryKeyOrdering_Salt;
    
    // We will get rid of the option for second s-box
    bool                                mPermuteEnabled;
    LoopSaltBehavior                    mPermuteSaltBehavior;
    LoopSBoxBehavior                    mPermuteSBoxBehavior;
    LoopKeyOrdering                     mPermuteKeyOrdering_Carry;
    LoopKeyOrdering                     mPermuteKeyOrdering_Value;
    LoopKeyOrdering                     mPermuteKeyOrdering_Salt;
    
    bool                                mSecondSourceEnabled;
    LoopSecondSourceMode                mSecondSourceMode;
    
    GSymbol                             mValueSalt;
    GSymbol                             mValueSBox;
    
    GSymbol                             mCarrySalt;
    GSymbol                             mCarrySBox;
    
    GSymbol                             mPermuteSalt;
    GSymbol                             mPermuteSBox;
    
    GSymbol                             mSourceA;
    GSymbol                             mSourceB;
    
    GSymbol                             mDest;
    
    
    
    
};

struct GSeedWorkLaneRecipeWorkSpace {
public:
    GSeedWorkLaneRecipeWorkSpace();
    
    // *THIS* needed
    bool                                Plan(bool pSecondPass, std::string &pErrorString);
    bool                                Bake(std::vector<GLoop> &pResult, std::string &pErrorString);
    bool                                BakeAlt(std::vector<GLoop> &pResult, std::string &pErrorString);
    
    GSeedWorkLaneRecipeLoopContract     mLoopContracts[SEED_WORK_LANE_COUNT];
    
    GSymbol                             mLoopIndex;
    
    GSymbol                             mSource;
    GSymbol                             mSourceByteA;
    GSymbol                             mSourceByteB;
    
    GSymbol                             mSaltByte;
    
    GSymbol                             mValue;
    GSymbol                             mValueKey;
    
    GSymbol                             mPermute;
    GSymbol                             mPermuteKey;
    
    GSymbol                             mCarry;
    GSymbol                             mCarryKey;
    
    
    GSymbol                             mOracleSourceA;
    GSymbol                             mOracleSourceB;
    
    GSymbol                             mOracleValueSalt;
    GSymbol                             mOracleCarrySalt;
    GSymbol                             mOraclePermuteSalt;
    
    GSymbol                             mOracleValueSBox;
    GSymbol                             mOracleCarrySBox;
    GSymbol                             mOraclePermuteSBox;
    
    
    
    GSymbol                             mSBoxA;
    GSymbol                             mSBoxB;
    GSymbol                             mSBoxC;
    GSymbol                             mSBoxD;
    
    GSymbol                             mSaltA;
    GSymbol                             mSaltB;
    GSymbol                             mSaltC;
    GSymbol                             mSaltD;
    
    GSymbol                             mWorkerA;
    GSymbol                             mWorkerB;
    GSymbol                             mWorkerC;
    GSymbol                             mWorkerD;
    
};

#endif /* GSeedWorkLaneRecipe_hpp */
