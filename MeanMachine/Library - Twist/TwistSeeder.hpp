//
//  TwistSeeder.hpp
//  MeanMachine
//

#ifndef TwistSeeder_hpp
#define TwistSeeder_hpp

#include "TwistWorkSpace.hpp"
#include <cstdint>

class TwistSeeder {
    
    // I am 100% owned by TwistExpander
    
public:
    TwistSeeder();
    virtual ~TwistSeeder();

    virtual void                        Seed(TwistWorkSpace *pWorkspace,
                                             std::uint8_t *pSource);

    void                                SetSBoxes(const std::uint8_t *pA,
                                                  const std::uint8_t *pB,
                                                  const std::uint8_t *pC,
                                                  const std::uint8_t *pD);
    void                                SetSalts(const std::uint8_t *pA,
                                                 const std::uint8_t *pB,
                                                 const std::uint8_t *pC,
                                                 const std::uint8_t *pD);
    void                                CopyTablesToWorkspace(TwistWorkSpace *pWorkspace) const;

protected:
    std::uint8_t                        mSaltA[S_SALT];
    std::uint8_t                        mSaltB[S_SALT];
    std::uint8_t                        mSaltC[S_SALT];
    std::uint8_t                        mSaltD[S_SALT];

    std::uint8_t                        mSBoxA[S_SBOX];
    std::uint8_t                        mSBoxB[S_SBOX];
    std::uint8_t                        mSBoxC[S_SBOX];
    std::uint8_t                        mSBoxD[S_SBOX];
};

#endif /* TwistSeeder_hpp */
