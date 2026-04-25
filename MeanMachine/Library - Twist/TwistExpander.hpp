//
//  TwistExpander.hpp
//  MeanMachine
//

#ifndef TwistExpander_hpp
#define TwistExpander_hpp

#include "TwistWorkSpace.hpp"

#include <cstdint>
#include <string>
#include <vector>

class TwistExpander {
public:
    
    TwistExpander();
    
    virtual ~TwistExpander();

    /*
     
     this is not needed.
     
    virtual void                        SeedPrepare(TwistWorkSpace *pWorkspace,
                                                    std::uint8_t *pSource,
                                                    std::uint8_t *pPassword,
                                                    unsigned int pPasswordByteLength);
    */
    
    virtual void                        Seed(TwistWorkSpace *pWorkspace,
                                             std::uint8_t *pSource,
                                             std::uint8_t *pPassword,
                                             unsigned int pPasswordByteLength);

    virtual void                        TwistBlock(TwistWorkSpace *pWorkspace,
                                                   std::uint8_t *pSource,
                                                   std::uint8_t *pDestination);

    // this is not virtual, it calls TwistBlock on every block
    void                                Twist(TwistWorkSpace *pWorkspace,
                                              std::uint8_t *pSource,
                                              std::uint8_t *pDestination,
                                              unsigned int pDestinationByteLength);
    
    
protected:
    
    TwistWorkSpace                      *mWorkspace;
    std::uint8_t                        *mSource;
    
    // The subclasses are responsible for assigning these.
    std::uint8_t                        *mSaltA;
    std::uint8_t                        *mSaltB;
    std::uint8_t                        *mSaltC;
    std::uint8_t                        *mSaltD;
    
    std::uint8_t                        *mSBoxA;
    std::uint8_t                        *mSBoxB;
    std::uint8_t                        *mSBoxC;
    std::uint8_t                        *mSBoxD;
    
    /*
    std::uint8_t                        mSaltA[S_SALT];
    std::uint8_t                        mSaltB[S_SALT];
    std::uint8_t                        mSaltC[S_SALT];
    std::uint8_t                        mSaltD[S_SALT];
    
    std::uint8_t                        mSBoxA[S_SBOX];
    std::uint8_t                        mSBoxB[S_SBOX];
    std::uint8_t                        mSBoxC[S_SBOX];
    std::uint8_t                        mSBoxD[S_SBOX];
    */
};

#endif /* TwistExpander_hpp */
