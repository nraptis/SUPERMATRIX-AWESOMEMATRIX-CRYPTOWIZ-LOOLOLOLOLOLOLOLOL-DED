//
//  CruelExportTest.m
//  CornTesting
//
//  Created by John Snow on 4/24/26.
//

#import <Foundation/Foundation.h>
#import <XCTest/XCTest.h>
#include <vector>
#include "TwistWorkSpace.hpp"
//#include "TwistSeedBuilder.hpp"
#include "GStatementRecipe.hpp"
#include "TwistFunctional.hpp"
#include "GTermExpander.hpp"
#include "Random.hpp"
#include "TwistExpander_Flyyyyy.hpp"
#include "GTwistTwister.hpp"
#include "GTwistExpander.hpp"
#include "GSeedWorkLaneRecipe.hpp"
#include "TwistExpander.hpp"
#include "SBoxTables.hpp"

@interface CruelExportTest : XCTestCase

@end

@implementation CruelExportTest

- (void)testExportedTwister {
    
    
    /*
     {
        // the pre-step
        std::string aErrorString;
        GSeedWorkLaneRecipeWorkSpace aRecipeSpace;
        
        
        GBatch aBatch1;
        if (!aRecipeSpace.Plan(false, aErrorString)) {
            XCTFail("{real seeder flow} work lane recipe failed to plan.");
            return;
        }
        
        if (!aRecipeSpace.Bake(aBatch1.mLoops, aErrorString)) {
            XCTFail("{real seeder flow} work lane recipe failed to plan.");
            return;
        }
        
        GBatch aBatch2;
        if (!aRecipeSpace.Plan(true, aErrorString)) {
            XCTFail("{real seeder flow} work lane recipe failed to plan.");
            return;
        }
        
        
       
        if (!aRecipeSpace.Bake(aBatch2.mLoops, aErrorString)) {
            XCTFail("{real seeder flow} work lane recipe failed to plan.");
            return;
        }
        
        GTwistExpander aExpanderBuilder;
        
        SBoxTables::CustomInjectFourSalt(&aExpanderBuilder);
        SBoxTables::CustomInjectFourSBox(&aExpanderBuilder);
        
        aExpanderBuilder.mNameBase = "Flyyyyy"; // DemoExpander, DemoSeeder, DemoTwister

        aExpanderBuilder.mSeeder.AddBatch(aBatch1);
        aExpanderBuilder.mSeeder.AddBatch(aBatch2);

        aExpanderBuilder.mTwister.AddBatch(aBatch2); // example, it has the twister and the seeder

        // String-only lines for direct, non-calculated snippets.
        aExpanderBuilder.mSeeder.AddWorkspaceAliasLine("aSaltA", "mSaltA");
        aExpanderBuilder.mTwister.AddWorkspaceAliasLine("aSaltA", "mSaltA");

        // Calculated-value line sample.
        aExpanderBuilder.mSeeder.AddAssignByteLine("aCarry", 0xDCU);
        aExpanderBuilder.mTwister.AddAssignByteLine("aCarry", 0xFAU);

        if (!aExpanderBuilder.ExportCPPProjectRoot("generated/cpp//")) {
            XCTFail("{export test} C++ export failed.");
            return;
        }
        if (!aExpanderBuilder.ExportJSONProjectRoot("Assets/test/pooop.json")) {
            XCTFail("{export test} JSON export failed.");
            return;
        }
        
        
        
    }
    */
    
    
    
    const char *pwd = "a man a plan a canal";
    int ps = (int)strlen(pwd);
    
    // This is exported from something in G, as C++ code we can drop in over DemoExpander
    TwistExpander_Flyyyyy aExpanderA;
    
    // This uses json load;
    GTwistTwister aExpanderB;
    
    std::string aLoadError;
    if (!aExpanderB.LoadProjectRoot("Assets/test/flyy.json", &aLoadError)) {
        XCTFail("%s", aLoadError.c_str());
        return;
    }
    
    unsigned char aSourceA[S_BLOCK];
    unsigned char aSourceB[S_BLOCK];
    
    TwistWorkSpace aWorkSpaceA;
    TwistWorkSpace aWorkSpaceB;
    
    aExpanderA.Seed(&aWorkSpaceA, aSourceA, (std::uint8_t *)pwd, ps);
    aExpanderB.Seed(&aWorkSpaceB, aSourceB, (std::uint8_t *)pwd, ps);

    unsigned char aDestA[S_BLOCK];
    unsigned char aDestB[S_BLOCK];
    aExpanderA.TwistBlock(&aWorkSpaceA, aSourceA, aDestA);
    aExpanderB.TwistBlock(&aWorkSpaceB, aSourceB, aDestB);
    
    for (int i=0;i<S_BLOCK;i++) {
        if (aWorkSpaceA.mWorkLaneA[i] != aWorkSpaceB.mWorkLaneA[i]) {
            XCTFail("{export test} work lane a mismatched.");
            return;
        }
        if (aWorkSpaceA.mWorkLaneB[i] != aWorkSpaceB.mWorkLaneB[i]) {
            XCTFail("{export test} work lane b mismatched.");
            return;
        }
        if (aWorkSpaceA.mWorkLaneC[i] != aWorkSpaceB.mWorkLaneC[i]) {
            XCTFail("{export test} work lane c mismatched.");
            return;
        }
        if (aWorkSpaceA.mWorkLaneD[i] != aWorkSpaceB.mWorkLaneD[i]) {
            XCTFail("{export test} work lane d mismatched.");
            return;
        }
    }
    
    
    
}



@end
