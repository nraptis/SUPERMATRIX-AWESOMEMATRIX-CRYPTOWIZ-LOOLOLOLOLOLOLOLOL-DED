#ifndef BREAD_SRC_EXPANSION_KEY_EXPANSION_PASSWORD_EXPANDER_HPP_
#define BREAD_SRC_EXPANSION_KEY_EXPANSION_PASSWORD_EXPANDER_HPP_

#include "TwistWorkSpace.hpp"
#include "TwistExpander.hpp"
#include "TwistSeeder.hpp"

class TwistTwister {
public:
    TwistTwister(TwistSeeder *pSeeder, TwistExpander *pExpander);
    TwistTwister();
    virtual ~TwistTwister();
    
    // The one and only function on TwistTwister
    virtual void                        TwistBlock(TwistWorkSpace *pWorkspace,
                                                   std::uint8_t *pDestination);
    
};

#endif  // BREAD_SRC_EXPANSION_KEY_EXPANSION_PASSWORD_EXPANDER_HPP_
