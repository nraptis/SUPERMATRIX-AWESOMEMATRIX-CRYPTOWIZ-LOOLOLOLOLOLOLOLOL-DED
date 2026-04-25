#include "TwistTwister.hpp"

#include <cstdio>

TwistTwister::TwistTwister(TwistSeeder *pSeeder,
                           TwistExpander *pExpander) {
    (void)pSeeder;
    (void)pExpander;
}

TwistTwister::TwistTwister() {
}

TwistTwister::~TwistTwister() {
}

void TwistTwister::TwistBlock(TwistWorkSpace *pWorkspace,
                              std::uint8_t *pDestination) {
    (void)pWorkspace;
    (void)pDestination;
    std::printf("fatal: TwistTwister::TwistBlock is legacy dead code.\n");
}
