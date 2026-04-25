//
//  GTwistTwister.cpp
//  MeanMachine
//

#include "GTwistTwister.hpp"

GTwistTwister::GTwistTwister()
: GTwistExpander() {
}

GTwistTwister::~GTwistTwister() {
}

bool GTwistTwister::LoadProjectRoot(const std::string &pJsonPath,
                                    std::string *pError) {
    return LoadJSONProjectRoot(pJsonPath, pError);
}
