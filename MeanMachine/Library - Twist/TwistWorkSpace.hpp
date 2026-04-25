//
//  TwistWorkSpace.hpp
//  Gorgon
//

#ifndef TwistWorkSpace_hpp
#define TwistWorkSpace_hpp

#include <cstdint>
#include <vector>

#define S_BLOCK 7680 // 3,840 // 1,920

#define S_SBOX 256
#define S_SALT 128

#define W_KEY_A 96
#define H_KEY_A 10
#define S_KEY_A (W_KEY_A * H_KEY_A)

#define W_KEY_B 80
#define H_KEY_B 16
#define S_KEY_B (W_KEY_B * H_KEY_B)

#define W_MASK_A 160
#define H_MASK_A 12
#define S_MASK_A (W_MASK_A * H_MASK_A)

#define W_MASK_B 64
#define H_MASK_B 15
#define S_MASK_B (W_MASK_B * H_MASK_B)

enum class TwistWorkSpaceSlot : std::uint8_t {
    
    kInvalid=255,
    
    kSource=0, // size is N * S_BLOCK. To fetch from here is different, because it includes an offset. So we wrap into [offset...offset+S_BLOCK)
    kDest=1, // size is N * S_BLOCK To write to here is different, because it includes an offset. So we wrap into [offset...offset+S_BLOCK)
    
    kSaltA=10,
    kSaltB=11,
    kSaltC=12,
    kSaltD=13,
    
    kSBoxA=20,
    kSBoxB=21,
    kSBoxC=22,
    kSBoxD=23,
    
    kSeedExpansionLaneA=40,
    kSeedExpansionLaneB=41,
    kSeedExpansionLaneC=42,
    kSeedExpansionLaneD=43,
    
    kWorkLaneA=50,
    kWorkLaneB=51,
    kWorkLaneC=52,
    kWorkLaneD=53,
    
    kOperationLaneA=70,
    kOperationLaneB=71,
    
    kMaskLaneA=80,
    kMaskLaneB=81,
    
    
    // These are confusing.
    // 1.) We "seed" into entire box, e.g. "kKeyBoxUnrolledA" or "kKeyBoxUnrolledB"
    // 2.) We "read", in a twist, from "kKeyRowReadA" or "kKeyRowReadB", which is the 'last row'.
    // 3.) Before writing, we Shift, evicting last row and duplicating first row
    // 4.) We "write", after a twist, from "kKeyRowWriteA" or "kKeyRowWriteB", which is the 'first row'.
    
    kKeyBoxUnrolledA=100, // size is S_KEY_A
    kKeyBoxUnrolledB=101, // size is S_KEY_B
    kKeyRowReadA=110, // size is W_KEY_A
    kKeyRowReadB=111, // size is W_KEY_B
    kKeyRowWriteA=120, // size is S_KEY_A
    kKeyRowWriteB=121, // size is S_KEY_B
    
    kMaskBoxUnrolledA=130, // size is S_MASK_A
    kMaskBoxUnrolledB=131, // size is S_MASK_B
    kMaskRowReadA=140, // size is W_MASK_A
    kMaskRowReadB=141, // size is W_MASK_B
    kMaskRowWriteA=150, // size is S_MASK_A
    kMaskRowWriteB=151, // size is S_MASK_B
};


class TwistWorkSpace {
    
public:
    TwistWorkSpace();
    
    // We stop storing source here.
    // std::uint8_t                            *mSource;
    
    // We stop storing dest here.
    //std::uint8_t                            *mDest;
    
    // We keep these as clones
    std::uint8_t                            mSaltA[S_SALT];
    std::uint8_t                            mSaltB[S_SALT];
    std::uint8_t                            mSaltC[S_SALT];
    std::uint8_t                            mSaltD[S_SALT];
    
    std::uint8_t                            mSBoxA[S_SBOX];
    std::uint8_t                            mSBoxB[S_SBOX];
    std::uint8_t                            mSBoxC[S_SBOX];
    std::uint8_t                            mSBoxD[S_SBOX];
    
    uint8_t                                 mKeyBoxA[H_KEY_A][W_KEY_A];
    uint8_t                                 mKeyBoxB[H_KEY_B][W_KEY_B];
    uint8_t                                 mMaskBoxA[H_MASK_A][W_MASK_A];
    uint8_t                                 mMaskBoxB[H_MASK_B][W_MASK_B];
    
    std::uint8_t                            mExpandLaneA[S_BLOCK];
    std::uint8_t                            mExpandLaneB[S_BLOCK];
    std::uint8_t                            mExpandLaneC[S_BLOCK];
    std::uint8_t                            mExpandLaneD[S_BLOCK];
    
    std::uint8_t                            mWorkLaneA[S_BLOCK];
    std::uint8_t                            mWorkLaneB[S_BLOCK];
    std::uint8_t                            mWorkLaneC[S_BLOCK];
    std::uint8_t                            mWorkLaneD[S_BLOCK];
    
    std::uint8_t                            mOperationLaneA[S_BLOCK];
    std::uint8_t                            mOperationLaneB[S_BLOCK];
    
    std::uint8_t                            mMaskLaneA[S_BLOCK];
    std::uint8_t                            mMaskLaneB[S_BLOCK];
    
    // Rotate stays byte-wide across the workspace helpers.
    static void                             ShiftKeyBoxA(std::uint8_t *pBox); //
    static void                             ShiftKeyBoxB(std::uint8_t *pBox); //
    static void                             ShiftMaskBoxA(std::uint8_t *pBox); //
    static void                             ShiftMaskBoxB(std::uint8_t *pBox); //
    
    static std::uint8_t                     *GetBuffer(TwistWorkSpace *pWorkspace, TwistWorkSpaceSlot pSlot); //
    static int                              GetBufferLength(TwistWorkSpaceSlot pSlot); //

    // Legacy misspellings retained while the rest of the project catches up.
    static std::uint8_t                     *GetBufer(TwistWorkSpace *pWorkspace, TwistWorkSpaceSlot pSlot); //
    static int                              GetBuferLength(TwistWorkSpaceSlot pSlot); //
    
};

#endif /* TwistWorkSpace_hpp */
