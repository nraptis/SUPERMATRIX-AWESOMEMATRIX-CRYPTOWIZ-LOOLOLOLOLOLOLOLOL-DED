//
//  Random.hpp
//  HomeGrownTests
//
//  Created by Lucky Squirrel on 4/3/26.
//

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

class Random {
public:
    
    static void                         Seed(int pSeed);
    
    static int                          Get();
    static int                          Get(int pMax);
    static int                          Get(int pMin, int pMax);
    
    static float                        GetFloat();
    static float                        GetFloat(float pMax);
    
    static float                        GetFloat(float pMin, float pMax);
    
    static bool                         Bool();
    static bool                         Chance(std::uint8_t pProbability);
    

    template <typename T>
    static T                            Choice(const std::vector<T> &pItems) {
        if (pItems.empty()) {
            return T();
        }
        const std::size_t aIndex = static_cast<std::size_t>(Get(static_cast<int>(pItems.size())));
        return pItems[aIndex];
    }

    template <typename T>
    static void                         Shuffle(std::vector<T> *pItems) {
        if ((pItems == nullptr) || (pItems->size() < 2U)) {
            return;
        }
        for (std::size_t i = 1U; i < pItems->size(); ++i) {
            const std::size_t aSwapIndex = static_cast<std::size_t>(Get(static_cast<int>(i + 1U)));
            std::swap((*pItems)[i], (*pItems)[aSwapIndex]);
        }
    }
    
    template <typename T>
    static void                         Shuffle(T *pItems, int pLength) {
        if ((pItems == nullptr) || (pLength < 2U)) {
            return;
        }
        for (std::size_t i = 1U; i < pLength; ++i) {
            const std::size_t aSwapIndex = static_cast<std::size_t>(Get(static_cast<int>(i + 1U)));
            std::swap((*pItems)[i], (*pItems)[aSwapIndex]);
        }
    }
    
};

#endif /* Random_hpp */
