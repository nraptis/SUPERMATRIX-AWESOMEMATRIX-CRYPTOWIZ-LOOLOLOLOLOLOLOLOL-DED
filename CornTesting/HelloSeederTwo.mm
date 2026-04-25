//
//  HelloSeederTwo.m
//  CornTesting
//
//  Created by John Snow on 4/22/26.
//

#import <Foundation/Foundation.h>
#import <XCTest/XCTest.h>
#include <algorithm>
#include <vector>
#include "SBoxTables.hpp"
#include "TwistWorkSpace.hpp"
//#include "TwistSeedBuilder.hpp"
#include "TwistSeeder.hpp"
#include "GStatementRecipe.hpp"
#include "GSymbol.hpp"
#include "GSeedProgram.hpp"

#include "TwistFunctional.hpp"
#include "GTermExpander.hpp"
#include "GSeedWorkLaneRecipe.hpp"
#include "Random.hpp"

@interface HelloSeederTwo : XCTestCase

@end

@implementation HelloSeederTwo

- (void)testRealSeedFlow {
    
    
    
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
    
    
    
    printf("ok, the plan worked\n");
    
    printf("%s\n", aBatch1.ToPrettyString().c_str());
    
    
    printf("ok, the plan worked\n");
    
    printf("%s\n", aBatch2.ToPrettyString().c_str());
    
    printf("ok, the plan worked\n");
    
    
    //GSeedWorkLaneRecipe
    
    // SeederTwo design scratchpad for a 4-lane world.
    //
    // High level:
    // 1. Each lane should still have its own local "personality".
    // 2. Across four lanes, at least two should consume prior lane output.
    // 3. We want the loop index to matter often, because index-driven churn
    //    helps break symmetry when we drop from 8 workers to 4.
    // 4. Every lane should have one strong nonlinear step and one state update.
    //
    // Necessary ingredients for a healthy loop:
    // - A state byte that evolves every round.
    // - A value_key that depends on state and at least one source.
    // - One S-box lookup for strong nonlinearity.
    // - A state update that feeds the output back into the next round.
    //
    // Optional ingredients that should be policy-driven:
    // - Carry use.
    // - Permute / swap / write-to-index-that-is-not-aLoop.
    // - A second update round after the main lookup.
    // - A second S-box, especially when we want index-aware carry refresh.
    //
    // Good 4-lane dependency flow:
    //
    // available_sources = [mSource]
    // scratch_lanes = [mWorkLaneA, mWorkLaneB, mWorkLaneC, mWorkLaneD]
    // shuffle(scratch_lanes)
    //
    // lane_0:
    //   withdraw 1 source from available_sources
    //   build loop with low complexity
    //   append lane_0 output to available_sources
    //
    // lane_1:
    //   withdraw 1 or 2 sources
    //   allow carry or second update
    //   append lane_1 output to available_sources
    //
    // lane_2:
    //   withdraw exactly 2 sources
    //   prefer cross-lane dependency and optional permute
    //   append lane_2 output to available_sources
    //
    // lane_3:
    //   withdraw exactly 2 sources
    //   prefer strongest recipe, usually with carry and/or second update
    //
    // Suggested object model:
    //
    // struct CarryContract {
    //     int require_min = 1;         // lanes that must use carry
    //     int require_max = 3;         // lanes allowed to use carry
    //     int loops_to_consider = 4;   // how many worker lanes
    //     bool require_index_mix = true;
    //     bool require_sbox_refresh = true;
    // };
    //
    // struct PermuteContract {
    //     int require_min = 0;
    //     int require_max = 2;
    //     bool require_index_based_key = true;
    //     bool require_distinct_target = true;
    // };
    //
    // struct LoopFeaturePlan {
    //     int source_count_min = 1;
    //     int source_count_max = 2;
    //     bool use_carry = false;
    //     bool use_permute = false;
    //     bool use_second_update = false;
    //     bool use_second_sbox = false;
    // };
    //
    // struct LaneBuildPlan {
    //     int lane_index = 0;
    //     int dependency_depth = 0;
    //     LoopFeaturePlan features;
    // };
    //
    // The key shift is that contracts should answer:
    // - how many lanes must use a feature?
    // - how many may use it?
    // - what extra rule makes that feature worth it?
    //
    // In other words, not just:
    //   should_carry(loop_index=2)
    //
    // but more like:
    //   carry_plan = carry_contract.Assign(loop_count=4, rng)
    //   use_carry = carry_plan.UseOnLane(2)
    //
    // This keeps the decision global first, local second.
    //
    // Crypto heuristics worth preserving:
    // - If carry is present, refresh it through an S-box or at least with the
    //   loop index. A stale carry is fake complexity.
    // - Index-based access is especially valuable in the carry path:
    //     value = s_box_a[mix]
    //     carry = value + s_box_b[aLoop]
    // - If only one S-box is used, the state update should be nontrivial:
    //     state = twist(value ^ carry) + salt[aLoop]
    // - If two S-boxes are used, make them play different roles:
    //     s_box_a = nonlinear value generation
    //     s_box_b = carry refresh, permute key, or second update
    // - With only 4 lanes, lane reuse must be more aggressive. We probably
    //   want lane_2 and lane_3 to consume prior lane output nearly always.
    //
    // Stronger loop examples:
    //
    // Example A: minimal but respectable
    // - 1 source
    // - 1 s-box
    // - no carry
    // - no permute
    //
    // state = seed_byte;
    // for (int aLoop=0; aLoop<S_BLOCK; aLoop++) {
    //     mix = twist(state) ^ twist(source[aLoop]) ^ salt_a[aLoop];
    //     value = s_box_a[mix];
    //     state = twist(value + salt_b[aLoop]);
    //     lane[aLoop] = value ^ state;
    // }
    //
    // Example B: preferred carry recipe
    // - 1 source
    // - 2 s-boxes
    // - carry
    // - no permute
    //
    // state = 243;
    // carry = 241;
    // for (int aLoop=0; aLoop<S_BLOCK; aLoop++) {
    //     mix = twist(state) ^ source[aLoop] ^ carry ^ salt_a[aLoop];
    //     value = s_box_a[mix];
    //     carry = value + s_box_b[aLoop];
    //     state = twist(value ^ carry ^ salt_b[aLoop]);
    //     lane[aLoop] = value;
    // }
    //
    // Example C: two-source diffusion lane
    // - 2 sources
    // - 1 s-box
    // - optional carry
    // - second update
    //
    // state = 199;
    // carry = 17;
    // for (int aLoop=0; aLoop<S_BLOCK; aLoop++) {
    //     mix = twist(src_a[aLoop]) + twist(src_b[aLoop]);
    //     mix ^= twist(state) ^ salt_a[aLoop];
    //     value = s_box_a[mix];
    //     carry = carry + value + aLoop;
    //     state = twist(value ^ carry);
    //     state = state + s_box_a[twist(src_b[aLoop]) ^ salt_c[aLoop]];
    //     lane[aLoop] = state ^ value;
    // }
    //
    // Example D: permuting write path
    // - 1 or 2 sources
    // - 2 s-boxes
    // - carry optional
    // - permute yes
    //
    // state = 91;
    // carry = 203;
    // for (int aLoop=0; aLoop<S_BLOCK; aLoop++) {
    //     mix = twist(state) ^ src_a[aLoop] ^ salt_a[aLoop];
    //     value = s_box_a[mix];
    //     carry = value + s_box_b[aLoop];
    //     permute_key = twist(value) ^ carry ^ salt_c[aLoop];
    //     permute = s_box_b[permute_key];
    //     state = twist(value + carry + salt_b[aLoop]);
    //     lane[permute] ^= state;
    // }
    //
    // Example E: strong late-lane recipe for only 4 workers
    // - 2 sources, one of them should be prior lane output
    // - 2 s-boxes
    // - carry yes
    // - second update yes
    // - permute optional
    //
    // state = 157;
    // carry = 73;
    // for (int aLoop=0; aLoop<S_BLOCK; aLoop++) {
    //     mix = twist(src_prev[aLoop]) ^ twist(src_root[aLoop]);
    //     mix += twist(state) + salt_a[aLoop];
    //     value = s_box_a[mix];
    //     carry = s_box_b[value ^ aLoop] ^ carry;
    //     state = twist(value + carry + salt_b[aLoop]);
    //     update = s_box_b[twist(state) ^ src_prev[aLoop] ^ salt_c[aLoop]];
    //     state ^= twist(update);
    //     lane[aLoop] = value ^ update ^ carry;
    // }
    //
    // Necessary vs optional:
    //
    // Necessary:
    // - 1 nonlinear lookup per iteration.
    // - State feedback every iteration.
    // - Lane-to-lane dependency growth across the 4 workers.
    // - At least one index-aware ingredient in the stronger lanes.
    //
    // Optional:
    // - Carry in every lane.
    // - Permute in every lane.
    // - Two S-boxes in every lane.
    // - Second update round in every lane.
    //
    // My bias with only 4 lanes:
    // - lane_0: minimal or carry-lite
    // - lane_1: carry
    // - lane_2: carry + second update
    // - lane_3: carry + optional permute + cross-lane source
    //
    // That gives us diversity without making every lane bloated.
    //
    // If we later want "pool-like" selection, the non-expression version
    // probably needs a feature allocator that scores whole loop recipes, not
    // individual symbols. Something like:
    //
    // struct LoopRecipe {
    //     int source_count;
    //     bool uses_carry;
    //     bool uses_permute;
    //     bool uses_second_update;
    //     int sbox_count;
    //     int dependency_depth;
    //     int expected_diffusion_score;
    // };
    //
    // Then the planner can filter recipes by contract first, and only after
    // that randomize within the allowed set.
    //
    // Dropping from 8 lanes to 4 probably means we should be stricter about
    // forcing lane dependency and stronger per-lane updates, not necessarily
    // forcing every optional feature on every lane.
    XCTAssertTrue(true);
    
    
}



@end
