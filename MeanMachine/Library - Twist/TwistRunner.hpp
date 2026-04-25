//
//  TwistRunner.hpp
//  Gorgon
//
//  Created by Sonic The Hedge Whore on 4/17/26.
//

#ifndef TwistRunner_hpp
#define TwistRunner_hpp

#include "TwistSeeder.hpp"
#include "TwistWorkSpace.hpp"

class TwistRunner {
    
public:
    TwistRunner();
    ~TwistRunner();
    
    
    
    std::uint8_t                            mSource[S_BLOCK];
    std::uint8_t                            mDest[S_BLOCK];
    
    TwistWorkSpace                          mWorkSpace;
    
    
    void                                    RunSeed(TwistSeeder *pSeeder,
                                                    std::uint8_t *pPassword,
                                                    int pPasswordLength);
    
    
    bool                                    RunSeedExpect(TwistSeeder *pSeeder,
                                                          std::uint8_t *pPassword,
                                                          int pPasswordLength,
                                                          TwistWorkSpace *pExpectation);
    
    
    void                                    UnrollPassword(std::uint8_t *pPassword,
                                                    int pPasswordLength);
    
    
    
    
};

#endif /* TwistRunner_hpp */
