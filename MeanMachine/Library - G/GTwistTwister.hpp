//
//  GTwistTwister.hpp
//  MeanMachine
//

#ifndef GTwistTwister_hpp
#define GTwistTwister_hpp

#include "GTwistExpander.hpp"

#include <string>

// Legacy compatibility wrapper while callers migrate to GTwistExpander.
class GTwistTwister : public GTwistExpander {
public:
    GTwistTwister();
    ~GTwistTwister() override;

    bool                                LoadProjectRoot(const std::string &pJsonPath,
                                                        std::string *pError = nullptr);
};

#endif /* GTwistTwister_hpp */
