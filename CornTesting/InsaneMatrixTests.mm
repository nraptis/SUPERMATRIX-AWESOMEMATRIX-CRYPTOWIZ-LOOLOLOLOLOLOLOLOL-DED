//
//  InsaneMatrixTests.m
//  CornTesting
//
//  Created by John Snow on 4/25/26.
//

#import <Foundation/Foundation.h>
#import <XCTest/XCTest.h>
#include "FastMatrix8.hpp"

FM8 u() {
    FM8 aResult;
    int aNumber = 0;
    for (int i=0; i<8; i++) {
        for (int n=0; n<8; n++) {
            aResult.mDataBase[(i << 3) + n] = aNumber;
            aNumber++;
        }
    }
    return aResult;
}

@interface InsaneMatrixTests : XCTestCase

@end

@implementation InsaneMatrixTests


- (void)testRotateRight {
    
    FM8 aMatrix = u();
    aMatrix.RotateRight();
    
    FM8 aExpected = {{
        {56,48,40,32,24,16, 8, 0},
        {57,49,41,33,25,17, 9, 1},
        {58,50,42,34,26,18,10, 2},
        {59,51,43,35,27,19,11, 3},
        {60,52,44,36,28,20,12, 4},
        {61,53,45,37,29,21,13, 5},
        {62,54,46,38,30,22,14, 6},
        {63,55,47,39,31,23,15, 7}
    }};
    
    if (aMatrix != aExpected) {
        XCTFail("{insane matrix test} rotate right did not work.");
    }
}

- (void)testRotateLeft {
    
    FM8 aMatrix = u();
    aMatrix.RotateLeft();
    
    FM8 aExpected = {{
        { 7,15,23,31,39,47,55,63},
        { 6,14,22,30,38,46,54,62},
        { 5,13,21,29,37,45,53,61},
        { 4,12,20,28,36,44,52,60},
        { 3,11,19,27,35,43,51,59},
        { 2,10,18,26,34,42,50,58},
        { 1, 9,17,25,33,41,49,57},
        { 0, 8,16,24,32,40,48,56}
    }};
    
    if (aMatrix != aExpected) {
        XCTFail("{insane matrix test} rotate left did not work.");
    }
}

- (void)testRotateRightBlocks {

    FM8 aMatrix = u();
    aMatrix.RotateRightBlocks();

    FM8 aExpected = {{
        {32,33,34,35, 0, 1, 2, 3},
        {40,41,42,43, 8, 9,10,11},
        {48,49,50,51,16,17,18,19},
        {56,57,58,59,24,25,26,27},

        {36,37,38,39, 4, 5, 6, 7},
        {44,45,46,47,12,13,14,15},
        {52,53,54,55,20,21,22,23},
        {60,61,62,63,28,29,30,31}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightBlocks failed");
    }
}

- (void)testRotateLeftBlocks {

    FM8 aMatrix = u();
    aMatrix.RotateLeftBlocks();

    FM8 aExpected = {{
        { 4, 5, 6, 7,36,37,38,39},
        {12,13,14,15,44,45,46,47},
        {20,21,22,23,52,53,54,55},
        {28,29,30,31,60,61,62,63},

        { 0, 1, 2, 3,32,33,34,35},
        { 8, 9,10,11,40,41,42,43},
        {16,17,18,19,48,49,50,51},
        {24,25,26,27,56,57,58,59}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftBlocks failed");
    }
}

- (void)testRotateRightQuarterA {

    FM8 aMatrix = u();
    aMatrix.RotateRightQuarterA();

    FM8 aExpected = {{

        {24,16, 8, 0, 4, 5, 6, 7},
        {25,17, 9, 1,12,13,14,15},
        {26,18,10, 2,20,21,22,23},
        {27,19,11, 3,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightQuarterA failed");
    }
}

- (void)testRotateRightQuarterB {

    FM8 aMatrix = u();
    aMatrix.RotateRightQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3,28,20,12, 4},
        { 8, 9,10,11,29,21,13, 5},
        {16,17,18,19,30,22,14, 6},
        {24,25,26,27,31,23,15, 7},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightQuarterB failed");
    }
}

- (void)testRotateRightQuarterC {

    FM8 aMatrix = u();
    aMatrix.RotateRightQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {56,48,40,32,36,37,38,39},
        {57,49,41,33,44,45,46,47},
        {58,50,42,34,52,53,54,55},
        {59,51,43,35,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightQuarterC failed");
    }
}

- (void)testRotateRightQuarterD {

    FM8 aMatrix = u();
    aMatrix.RotateRightQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,60,52,44,36},
        {40,41,42,43,61,53,45,37},
        {48,49,50,51,62,54,46,38},
        {56,57,58,59,63,55,47,39}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightQuarterD failed");
    }
}

- (void)testRotateLeftQuarterA {

    FM8 aMatrix = u();
    aMatrix.RotateLeftQuarterA();

    FM8 aExpected = {{

        { 3,11,19,27, 4, 5, 6, 7},
        { 2,10,18,26,12,13,14,15},
        { 1, 9,17,25,20,21,22,23},
        { 0, 8,16,24,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftQuarterA failed");
    }
}

- (void)testRotateLeftQuarterB {

    FM8 aMatrix = u();
    aMatrix.RotateLeftQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 7,15,23,31},
        { 8, 9,10,11, 6,14,22,30},
        {16,17,18,19, 5,13,21,29},
        {24,25,26,27, 4,12,20,28},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftQuarterB failed");
    }
}

- (void)testRotateLeftQuarterC {

    FM8 aMatrix = u();
    aMatrix.RotateLeftQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {35,43,51,59,36,37,38,39},
        {34,42,50,58,44,45,46,47},
        {33,41,49,57,52,53,54,55},
        {32,40,48,56,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftQuarterC failed");
    }
}

- (void)testRotateLeftQuarterD {

    FM8 aMatrix = u();
    aMatrix.RotateLeftQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,39,47,55,63},
        {40,41,42,43,38,46,54,62},
        {48,49,50,51,37,45,53,61},
        {56,57,58,59,36,44,52,60}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftQuarterD failed");
    }
}

- (void)testRotateRightEachQuarter {

    FM8 aMatrix = u();
    aMatrix.RotateRightEachQuarter();

    FM8 aExpected = {{

        // top-left block rotated
        {24,16, 8, 0, 28,20,12, 4},
        {25,17, 9, 1, 29,21,13, 5},
        {26,18,10, 2, 30,22,14, 6},
        {27,19,11, 3, 31,23,15, 7},

        // bottom blocks rotated
        {56,48,40,32, 60,52,44,36},
        {57,49,41,33, 61,53,45,37},
        {58,50,42,34, 62,54,46,38},
        {59,51,43,35, 63,55,47,39}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightEachQuarter failed");
    }
}

- (void)testRotateLeftEachQuarter {

    FM8 aMatrix = u();
    aMatrix.RotateLeftEachQuarter();

    FM8 aExpected = {{

        // top-left block rotated
        { 3,11,19,27,  7,15,23,31},
        { 2,10,18,26,  6,14,22,30},
        { 1, 9,17,25,  5,13,21,29},
        { 0, 8,16,24,  4,12,20,28},

        // bottom blocks rotated
        {35,43,51,59, 39,47,55,63},
        {34,42,50,58, 38,46,54,62},
        {33,41,49,57, 37,45,53,61},
        {32,40,48,56, 36,44,52,60}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftEachQuarter failed");
    }
}

- (void)testSwapRows {

    FM8 aMatrix = u();
    aMatrix.SwapRows(0, 7);

    FM8 aExpected = {{
        {56,57,58,59,60,61,62,63},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},
        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        { 0, 1, 2, 3, 4, 5, 6, 7}
    }};

    if (aMatrix != aExpected) {
        XCTFail("SwapRows first/last failed");
    }
}

- (void)testReverseRow {

    FM8 aMatrix = u();
    aMatrix.ReverseRow(0);

    FM8 aExpected = {{
        {7,6,5,4,3,2,1,0},
        {8,9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},
        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("ReverseRow failed");
    }
}

- (void)testRotateRow {

    FM8 aMatrix = u();
    aMatrix.RotateRow(0, 1);

    FM8 aExpected = {{
        {1,2,3,4,5,6,7,0},
        {8,9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},
        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRow failed");
    }
}

- (void)testSwapColumns {

    FM8 aMatrix = u();
    aMatrix.SwapColumns(0, 7);

    FM8 aExpected = {{
        {7,1,2,3,4,5,6,0},
        {15,9,10,11,12,13,14,8},
        {23,17,18,19,20,21,22,16},
        {31,25,26,27,28,29,30,24},
        {39,33,34,35,36,37,38,32},
        {47,41,42,43,44,45,46,40},
        {55,49,50,51,52,53,54,48},
        {63,57,58,59,60,61,62,56}
    }};

    if (aMatrix != aExpected) {
        XCTFail("SwapColumns failed");
    }
}

- (void)testRotateColumn {

    FM8 aMatrix = u();
    aMatrix.RotateColumn(0, 1);

    FM8 aExpected = {{
        {56,1,2,3,4,5,6,7},
        { 0,9,10,11,12,13,14,15},
        { 8,17,18,19,20,21,22,23},
        {16,25,26,27,28,29,30,31},
        {24,33,34,35,36,37,38,39},
        {32,41,42,43,44,45,46,47},
        {40,49,50,51,52,53,54,55},
        {48,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateColumn failed");
    }
}

- (void)testTransposeMainDiagonal {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonal();

    FM8 aExpected = {{
        {0,8,16,24,32,40,48,56},
        {1,9,17,25,33,41,49,57},
        {2,10,18,26,34,42,50,58},
        {3,11,19,27,35,43,51,59},
        {4,12,20,28,36,44,52,60},
        {5,13,21,29,37,45,53,61},
        {6,14,22,30,38,46,54,62},
        {7,15,23,31,39,47,55,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonal failed");
    }
}

- (void)testTransposeAntiDiagonal {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonal();

    FM8 aExpected = {{
        {63,55,47,39,31,23,15,7},
        {62,54,46,38,30,22,14,6},
        {61,53,45,37,29,21,13,5},
        {60,52,44,36,28,20,12,4},
        {59,51,43,35,27,19,11,3},
        {58,50,42,34,26,18,10,2},
        {57,49,41,33,25,17,9,1},
        {56,48,40,32,24,16,8,0}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonal failed");
    }
}

- (void)testFlipHorizontal {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontal();

    FM8 aExpected = {{
        {7,6,5,4,3,2,1,0},
        {15,14,13,12,11,10,9,8},
        {23,22,21,20,19,18,17,16},
        {31,30,29,28,27,26,25,24},
        {39,38,37,36,35,34,33,32},
        {47,46,45,44,43,42,41,40},
        {55,54,53,52,51,50,49,48},
        {63,62,61,60,59,58,57,56}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontal failed");
    }
}

- (void)testFlipVertical {

    FM8 aMatrix = u();
    aMatrix.FlipVertical();

    FM8 aExpected = {{
        {56,57,58,59,60,61,62,63},
        {48,49,50,51,52,53,54,55},
        {40,41,42,43,44,45,46,47},
        {32,33,34,35,36,37,38,39},
        {24,25,26,27,28,29,30,31},
        {16,17,18,19,20,21,22,23},
        { 8, 9,10,11,12,13,14,15},
        { 0, 1, 2, 3, 4, 5, 6, 7}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVertical failed");
    }
}

- (void)testTransposeMainDiagonalQuarterA {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalQuarterA();

    FM8 aExpected = {{

        { 0, 8,16,24, 4, 5, 6, 7},
        { 1, 9,17,25,12,13,14,15},
        { 2,10,18,26,20,21,22,23},
        { 3,11,19,27,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalQuarterA failed");
    }
}

- (void)testTransposeMainDiagonalQuarterB {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4,12,20,28},
        { 8, 9,10,11, 5,13,21,29},
        {16,17,18,19, 6,14,22,30},
        {24,25,26,27, 7,15,23,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalQuarterB failed");
    }
}

- (void)testTransposeMainDiagonalQuarterC {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,40,48,56,36,37,38,39},
        {33,41,49,57,44,45,46,47},
        {34,42,50,58,52,53,54,55},
        {35,43,51,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalQuarterC failed");
    }
}

- (void)testTransposeMainDiagonalQuarterD {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,36,44,52,60},
        {40,41,42,43,37,45,53,61},
        {48,49,50,51,38,46,54,62},
        {56,57,58,59,39,47,55,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalQuarterD failed");
    }
}

- (void)testTransposeMainDiagonalEachQuarter {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalEachQuarter();

    FM8 aExpected = {{

        { 0, 8,16,24, 4,12,20,28},
        { 1, 9,17,25, 5,13,21,29},
        { 2,10,18,26, 6,14,22,30},
        { 3,11,19,27, 7,15,23,31},

        {32,40,48,56,36,44,52,60},
        {33,41,49,57,37,45,53,61},
        {34,42,50,58,38,46,54,62},
        {35,43,51,59,39,47,55,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalEachQuarter failed");
    }
}


- (void)testTransposeAntiDiagonalQuarterA {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalQuarterA();

    FM8 aExpected = {{

        {27,19,11, 3, 4, 5, 6, 7},
        {26,18,10, 2,12,13,14,15},
        {25,17, 9, 1,20,21,22,23},
        {24,16, 8, 0,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalQuarterA failed");
    }
}

- (void)testTransposeAntiDiagonalQuarterB {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3,31,23,15, 7},
        { 8, 9,10,11,30,22,14, 6},
        {16,17,18,19,29,21,13, 5},
        {24,25,26,27,28,20,12, 4},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalQuarterB failed");
    }
}

- (void)testTransposeAntiDiagonalQuarterC {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {59,51,43,35,36,37,38,39},
        {58,50,42,34,44,45,46,47},
        {57,49,41,33,52,53,54,55},
        {56,48,40,32,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalQuarterC failed");
    }
}

- (void)testTransposeAntiDiagonalQuarterD {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,63,55,47,39},
        {40,41,42,43,62,54,46,38},
        {48,49,50,51,61,53,45,37},
        {56,57,58,59,60,52,44,36}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalQuarterD failed");
    }
}

- (void)testTransposeAntiDiagonalEachQuarter {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalEachQuarter();

    FM8 aExpected = {{

        {27,19,11, 3,31,23,15, 7},
        {26,18,10, 2,30,22,14, 6},
        {25,17, 9, 1,29,21,13, 5},
        {24,16, 8, 0,28,20,12, 4},

        {59,51,43,35,63,55,47,39},
        {58,50,42,34,62,54,46,38},
        {57,49,41,33,61,53,45,37},
        {56,48,40,32,60,52,44,36}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalEachQuarter failed");
    }
}

- (void)testFlipHorizontalQuarterA {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalQuarterA();

    FM8 aExpected = {{

        { 3, 2, 1, 0, 4, 5, 6, 7},
        {11,10, 9, 8,12,13,14,15},
        {19,18,17,16,20,21,22,23},
        {27,26,25,24,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalQuarterA failed");
    }
}

- (void)testFlipHorizontalQuarterB {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 7, 6, 5, 4},
        { 8, 9,10,11,15,14,13,12},
        {16,17,18,19,23,22,21,20},
        {24,25,26,27,31,30,29,28},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalQuarterB failed");
    }
}

- (void)testFlipHorizontalQuarterC {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {35,34,33,32,36,37,38,39},
        {43,42,41,40,44,45,46,47},
        {51,50,49,48,52,53,54,55},
        {59,58,57,56,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalQuarterC failed");
    }
}

- (void)testFlipHorizontalQuarterD {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,39,38,37,36},
        {40,41,42,43,47,46,45,44},
        {48,49,50,51,55,54,53,52},
        {56,57,58,59,63,62,61,60}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalQuarterD failed");
    }
}

- (void)testFlipHorizontalEachQuarter {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalEachQuarter();

    FM8 aExpected = {{

        { 3, 2, 1, 0, 7, 6, 5, 4},
        {11,10, 9, 8,15,14,13,12},
        {19,18,17,16,23,22,21,20},
        {27,26,25,24,31,30,29,28},

        {35,34,33,32,39,38,37,36},
        {43,42,41,40,47,46,45,44},
        {51,50,49,48,55,54,53,52},
        {59,58,57,56,63,62,61,60}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalEachQuarter failed");
    }
}

- (void)testFlipVerticalQuarterA {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalQuarterA();

    FM8 aExpected = {{

        {24,25,26,27, 4, 5, 6, 7},
        {16,17,18,19,12,13,14,15},
        { 8, 9,10,11,20,21,22,23},
        { 0, 1, 2, 3,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalQuarterA failed");
    }
}

- (void)testFlipVerticalQuarterB {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3,28,29,30,31},
        { 8, 9,10,11,20,21,22,23},
        {16,17,18,19,12,13,14,15},
        {24,25,26,27, 4, 5, 6, 7},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalQuarterB failed");
    }
}

- (void)testFlipVerticalQuarterC {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {56,57,58,59,36,37,38,39},
        {48,49,50,51,44,45,46,47},
        {40,41,42,43,52,53,54,55},
        {32,33,34,35,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalQuarterC failed");
    }
}

- (void)testFlipVerticalQuarterD {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,60,61,62,63},
        {40,41,42,43,52,53,54,55},
        {48,49,50,51,44,45,46,47},
        {56,57,58,59,36,37,38,39}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalQuarterD failed");
    }
}

- (void)testFlipVerticalEachQuarter {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalEachQuarter();

    FM8 aExpected = {{

        {24,25,26,27,28,29,30,31},
        {16,17,18,19,20,21,22,23},
        { 8, 9,10,11,12,13,14,15},
        { 0, 1, 2, 3, 4, 5, 6, 7},

        {56,57,58,59,60,61,62,63},
        {48,49,50,51,52,53,54,55},
        {40,41,42,43,44,45,46,47},
        {32,33,34,35,36,37,38,39}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalEachQuarter failed");
    }
}

- (void)testFlipHorizontalBlocks {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalBlocks();

    FM8 aExpected = {{

        { 4, 5, 6, 7, 0, 1, 2, 3},
        {12,13,14,15, 8, 9,10,11},
        {20,21,22,23,16,17,18,19},
        {28,29,30,31,24,25,26,27},

        {36,37,38,39,32,33,34,35},
        {44,45,46,47,40,41,42,43},
        {52,53,54,55,48,49,50,51},
        {60,61,62,63,56,57,58,59}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalBlocks failed");
    }
}

- (void)testFlipVerticalBlocks {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalBlocks();

    FM8 aExpected = {{

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63},

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalBlocks failed");
    }
}

- (void)testTransposeMainDiagonalBlocks {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalBlocks();

    FM8 aExpected = {{

        { 0, 1, 2, 3,32,33,34,35},
        { 8, 9,10,11,40,41,42,43},
        {16,17,18,19,48,49,50,51},
        {24,25,26,27,56,57,58,59},

        { 4, 5, 6, 7,36,37,38,39},
        {12,13,14,15,44,45,46,47},
        {20,21,22,23,52,53,54,55},
        {28,29,30,31,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalBlocks failed");
    }
}

- (void)testTransposeAntiDiagonalBlocks {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalBlocks();

    FM8 aExpected = {{

        {36,37,38,39, 4, 5, 6, 7},
        {44,45,46,47,12,13,14,15},
        {52,53,54,55,20,21,22,23},
        {60,61,62,63,28,29,30,31},

        {32,33,34,35, 0, 1, 2, 3},
        {40,41,42,43, 8, 9,10,11},
        {48,49,50,51,16,17,18,19},
        {56,57,58,59,24,25,26,27}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalBlocks failed");
    }
}

- (void)testRotateRightEachSixteenthQuarterA {

    FM8 aMatrix = u();
    aMatrix.RotateRightEachSixteenthQuarterA();

    FM8 aExpected = {{

        { 8, 0,10, 2, 4, 5, 6, 7},
        { 9, 1,11, 3,12,13,14,15},
        {24,16,26,18,20,21,22,23},
        {25,17,27,19,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightEachSixteenthQuarterA failed");
    }
}

- (void)testRotateRightEachSixteenthQuarterB {

    FM8 aMatrix = u();
    aMatrix.RotateRightEachSixteenthQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3,12, 4,14, 6},
        { 8, 9,10,11,13, 5,15, 7},
        {16,17,18,19,28,20,30,22},
        {24,25,26,27,29,21,31,23},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightEachSixteenthQuarterB failed");
    }
}

- (void)testRotateRightEachSixteenthQuarterC {

    FM8 aMatrix = u();
    aMatrix.RotateRightEachSixteenthQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {40,32,42,34,36,37,38,39},
        {41,33,43,35,44,45,46,47},
        {56,48,58,50,52,53,54,55},
        {57,49,59,51,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightEachSixteenthQuarterC failed");
    }
}

- (void)testRotateRightEachSixteenthQuarterD {

    FM8 aMatrix = u();
    aMatrix.RotateRightEachSixteenthQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,44,36,46,38},
        {40,41,42,43,45,37,47,39},
        {48,49,50,51,60,52,62,54},
        {56,57,58,59,61,53,63,55}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightEachSixteenthQuarterD failed");
    }
}

- (void)testRotateRightEachSixteenthEachQuarter {

    FM8 aMatrix = u();
    aMatrix.RotateRightEachSixteenthEachQuarter();

    FM8 aExpected = {{

        // Top half
        { 8, 0,10, 2,12, 4,14, 6},
        { 9, 1,11, 3,13, 5,15, 7},
        {24,16,26,18,28,20,30,22},
        {25,17,27,19,29,21,31,23},

        // Bottom half
        {40,32,42,34,44,36,46,38},
        {41,33,43,35,45,37,47,39},
        {56,48,58,50,60,52,62,54},
        {57,49,59,51,61,53,63,55}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightEachSixteenthEachQuarter failed");
    }
}

- (void)testRotateLeftEachSixteenthQuarterA {

    FM8 aMatrix = u();
    aMatrix.RotateLeftEachSixteenthQuarterA();

    FM8 aExpected = {{

        { 1, 9, 3,11, 4, 5, 6, 7},
        { 0, 8, 2,10,12,13,14,15},
        {17,25,19,27,20,21,22,23},
        {16,24,18,26,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftEachSixteenthQuarterA failed");
    }
}

- (void)testRotateLeftEachSixteenthQuarterB {

    FM8 aMatrix = u();
    aMatrix.RotateLeftEachSixteenthQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 5,13, 7,15},
        { 8, 9,10,11, 4,12, 6,14},
        {16,17,18,19,21,29,23,31},
        {24,25,26,27,20,28,22,30},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftEachSixteenthQuarterB failed");
    }
}

- (void)testRotateLeftEachSixteenthQuarterC {

    FM8 aMatrix = u();
    aMatrix.RotateLeftEachSixteenthQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {33,41,35,43,36,37,38,39},
        {32,40,34,42,44,45,46,47},
        {49,57,51,59,52,53,54,55},
        {48,56,50,58,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftEachSixteenthQuarterC failed");
    }
}

- (void)testRotateLeftEachSixteenthEachQuarter {

    FM8 aMatrix = u();
    aMatrix.RotateLeftEachSixteenthEachQuarter();

    FM8 aExpected = {{

        { 1, 9, 3,11, 5,13, 7,15},
        { 0, 8, 2,10, 4,12, 6,14},
        {17,25,19,27,21,29,23,31},
        {16,24,18,26,20,28,22,30},

        {33,41,35,43,37,45,39,47},
        {32,40,34,42,36,44,38,46},
        {49,57,51,59,53,61,55,63},
        {48,56,50,58,52,60,54,62}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftEachSixteenthEachQuarter failed");
    }
}

- (void)testFlipHorizontalEachSixteenthQuarterA {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalEachSixteenthQuarterA();

    FM8 aExpected = {{

        { 1, 0, 3, 2, 4, 5, 6, 7},
        { 9, 8,11,10,12,13,14,15},
        {17,16,19,18,20,21,22,23},
        {25,24,27,26,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalEachSixteenthQuarterA failed");
    }
}

- (void)testFlipHorizontalEachSixteenthQuarterB {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalEachSixteenthQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 5, 4, 7, 6},
        { 8, 9,10,11,13,12,15,14},
        {16,17,18,19,21,20,23,22},
        {24,25,26,27,29,28,31,30},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalEachSixteenthQuarterB failed");
    }
}

- (void)testFlipHorizontalEachSixteenthQuarterC {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalEachSixteenthQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {33,32,35,34,36,37,38,39},
        {41,40,43,42,44,45,46,47},
        {49,48,51,50,52,53,54,55},
        {57,56,59,58,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalEachSixteenthQuarterC failed");
    }
}

- (void)testFlipHorizontalEachSixteenthQuarterD {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalEachSixteenthQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,37,36,39,38},
        {40,41,42,43,45,44,47,46},
        {48,49,50,51,53,52,55,54},
        {56,57,58,59,61,60,63,62}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalEachSixteenthQuarterD failed");
    }
}

- (void)testFlipHorizontalEachSixteenthEachQuarter {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalEachSixteenthEachQuarter();

    FM8 aExpected = {{

        { 1, 0, 3, 2, 5, 4, 7, 6},
        { 9, 8,11,10,13,12,15,14},
        {17,16,19,18,21,20,23,22},
        {25,24,27,26,29,28,31,30},

        {33,32,35,34,37,36,39,38},
        {41,40,43,42,45,44,47,46},
        {49,48,51,50,53,52,55,54},
        {57,56,59,58,61,60,63,62}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalEachSixteenthEachQuarter failed");
    }
}

- (void)testFlipVerticalEachSixteenthQuarterA {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalEachSixteenthQuarterA();

    FM8 aExpected = {{

        { 8, 9,10,11, 4, 5, 6, 7},
        { 0, 1, 2, 3,12,13,14,15},
        {24,25,26,27,20,21,22,23},
        {16,17,18,19,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalEachSixteenthQuarterA failed");
    }
}

- (void)testFlipVerticalEachSixteenthQuarterB {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalEachSixteenthQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3,12,13,14,15},
        { 8, 9,10,11, 4, 5, 6, 7},
        {16,17,18,19,28,29,30,31},
        {24,25,26,27,20,21,22,23},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalEachSixteenthQuarterB failed");
    }
}

- (void)testFlipVerticalEachSixteenthQuarterC {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalEachSixteenthQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {40,41,42,43,36,37,38,39},
        {32,33,34,35,44,45,46,47},
        {56,57,58,59,52,53,54,55},
        {48,49,50,51,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalEachSixteenthQuarterC failed");
    }
}

- (void)testFlipVerticalEachSixteenthQuarterD {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalEachSixteenthQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,44,45,46,47},
        {40,41,42,43,36,37,38,39},
        {48,49,50,51,60,61,62,63},
        {56,57,58,59,52,53,54,55}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalEachSixteenthQuarterD failed");
    }
}

- (void)testFlipVerticalEachSixteenthEachQuarter {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalEachSixteenthEachQuarter();

    FM8 aExpected = {{

        { 8, 9,10,11,12,13,14,15},
        { 0, 1, 2, 3, 4, 5, 6, 7},
        {24,25,26,27,28,29,30,31},
        {16,17,18,19,20,21,22,23},

        {40,41,42,43,44,45,46,47},
        {32,33,34,35,36,37,38,39},
        {56,57,58,59,60,61,62,63},
        {48,49,50,51,52,53,54,55}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalEachSixteenthEachQuarter failed");
    }
}

- (void)testTransposeMainDiagonalEachSixteenthQuarterA {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalEachSixteenthQuarterA();

    FM8 aExpected = {{

        { 0, 8, 2,10, 4, 5, 6, 7},
        { 1, 9, 3,11,12,13,14,15},
        {16,24,18,26,20,21,22,23},
        {17,25,19,27,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalEachSixteenthQuarterA failed");
    }
}

- (void)testTransposeMainDiagonalEachSixteenthQuarterB {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalEachSixteenthQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4,12, 6,14},
        { 8, 9,10,11, 5,13, 7,15},
        {16,17,18,19,20,28,22,30},
        {24,25,26,27,21,29,23,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalEachSixteenthQuarterB failed");
    }
}

- (void)testTransposeMainDiagonalEachSixteenthQuarterC {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalEachSixteenthQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,40,34,42,36,37,38,39},
        {33,41,35,43,44,45,46,47},
        {48,56,50,58,52,53,54,55},
        {49,57,51,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalEachSixteenthQuarterC failed");
    }
}

- (void)testTransposeMainDiagonalEachSixteenthEachQuarter {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalEachSixteenthEachQuarter();

    FM8 aExpected = {{

        { 0, 8, 2,10, 4,12, 6,14},
        { 1, 9, 3,11, 5,13, 7,15},
        {16,24,18,26,20,28,22,30},
        {17,25,19,27,21,29,23,31},

        {32,40,34,42,36,44,38,46},
        {33,41,35,43,37,45,39,47},
        {48,56,50,58,52,60,54,62},
        {49,57,51,59,53,61,55,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalEachSixteenthEachQuarter failed");
    }
}

- (void)testTransposeMainDiagonalEachSixteenthQuarterD {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalEachSixteenthQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,36,44,38,46},
        {40,41,42,43,37,45,39,47},
        {48,49,50,51,52,60,54,62},
        {56,57,58,59,53,61,55,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalEachSixteenthQuarterD failed");
    }
}

- (void)testTransposeAntiDiagonalEachSixteenthQuarterA {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalEachSixteenthQuarterA();

    FM8 aExpected = {{

        { 9, 1,11, 3, 4, 5, 6, 7},
        { 8, 0,10, 2,12,13,14,15},
        {25,17,27,19,20,21,22,23},
        {24,16,26,18,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalEachSixteenthQuarterA failed");
    }
}

- (void)testTransposeAntiDiagonalEachSixteenthQuarterB {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalEachSixteenthQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3,13, 5,15, 7},
        { 8, 9,10,11,12, 4,14, 6},
        {16,17,18,19,29,21,31,23},
        {24,25,26,27,28,20,30,22},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalEachSixteenthQuarterB failed");
    }
}

- (void)testTransposeAntiDiagonalEachSixteenthQuarterC {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalEachSixteenthQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {41,33,43,35,36,37,38,39},
        {40,32,42,34,44,45,46,47},
        {57,49,59,51,52,53,54,55},
        {56,48,58,50,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalEachSixteenthQuarterC failed");
    }
}

- (void)testTransposeAntiDiagonalEachSixteenthQuarterD {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalEachSixteenthQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,45,37,47,39},
        {40,41,42,43,44,36,46,38},
        {48,49,50,51,61,53,63,55},
        {56,57,58,59,60,52,62,54}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalEachSixteenthQuarterD failed");
    }
}

- (void)testTransposeAntiDiagonalEachSixteenthEachQuarter {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalEachSixteenthEachQuarter();

    FM8 aExpected = {{

        { 9, 1,11, 3,13, 5,15, 7},
        { 8, 0,10, 2,12, 4,14, 6},
        {25,17,27,19,29,21,31,23},
        {24,16,26,18,28,20,30,22},

        {41,33,43,35,45,37,47,39},
        {40,32,42,34,44,36,46,38},
        {57,49,59,51,61,53,63,55},
        {56,48,58,50,60,52,62,54}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalEachSixteenthEachQuarter failed");
    }
}

- (void)testRotateRightSixteenthsQuarterA {

    FM8 aMatrix = u();
    aMatrix.RotateRightSixteenthsQuarterA();

    FM8 aExpected = {{

        {16,17, 0, 1, 4, 5, 6, 7},
        {24,25, 8, 9,12,13,14,15},
        {18,19, 2, 3,20,21,22,23},
        {26,27,10,11,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightSixteenthsQuarterA failed");
    }
}

- (void)testRotateRightSixteenthsQuarterB {

    FM8 aMatrix = u();
    aMatrix.RotateRightSixteenthsQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3,20,21, 4, 5},
        { 8, 9,10,11,28,29,12,13},
        {16,17,18,19,22,23, 6, 7},
        {24,25,26,27,30,31,14,15},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightSixteenthsQuarterB failed");
    }
}

- (void)testRotateRightSixteenthsQuarterC {

    FM8 aMatrix = u();
    aMatrix.RotateRightSixteenthsQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {48,49,32,33,36,37,38,39},
        {56,57,40,41,44,45,46,47},
        {50,51,34,35,52,53,54,55},
        {58,59,42,43,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightSixteenthsQuarterC failed");
    }
}

- (void)testRotateRightSixteenthsQuarterD {

    FM8 aMatrix = u();
    aMatrix.RotateRightSixteenthsQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,52,53,36,37},
        {40,41,42,43,60,61,44,45},
        {48,49,50,51,54,55,38,39},
        {56,57,58,59,62,63,46,47}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateRightSixteenthsQuarterD failed");
    }
}

- (void)testRotateLeftSixteenthsQuarterA {

    FM8 aMatrix = u();
    aMatrix.RotateLeftSixteenthsQuarterA();

    FM8 aExpected = {{

        { 2, 3,18,19, 4, 5, 6, 7},
        {10,11,26,27,12,13,14,15},
        { 0, 1,16,17,20,21,22,23},
        { 8, 9,24,25,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftSixteenthsQuarterA failed");
    }
}

- (void)testRotateLeftSixteenthsQuarterB {

    FM8 aMatrix = u();
    aMatrix.RotateLeftSixteenthsQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 6, 7,22,23},
        { 8, 9,10,11,14,15,30,31},
        {16,17,18,19, 4, 5,20,21},
        {24,25,26,27,12,13,28,29},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftSixteenthsQuarterB failed");
    }
}

- (void)testRotateLeftSixteenthsQuarterC {

    FM8 aMatrix = u();
    aMatrix.RotateLeftSixteenthsQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {34,35,50,51,36,37,38,39},
        {42,43,58,59,44,45,46,47},
        {32,33,48,49,52,53,54,55},
        {40,41,56,57,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftSixteenthsQuarterC failed");
    }
}

- (void)testRotateLeftSixteenthsQuarterD {

    FM8 aMatrix = u();
    aMatrix.RotateLeftSixteenthsQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,38,39,54,55},
        {40,41,42,43,46,47,62,63},
        {48,49,50,51,36,37,52,53},
        {56,57,58,59,44,45,60,61}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftSixteenthsQuarterD failed");
    }
}

- (void)testRotateLeftSixteenthsEachQuarter {

    FM8 aMatrix = u();
    aMatrix.RotateLeftSixteenthsEachQuarter();

    FM8 aExpected = {{

        { 2, 3,18,19, 6, 7,22,23},
        {10,11,26,27,14,15,30,31},
        { 0, 1,16,17, 4, 5,20,21},
        { 8, 9,24,25,12,13,28,29},

        {34,35,50,51,38,39,54,55},
        {42,43,58,59,46,47,62,63},
        {32,33,48,49,36,37,52,53},
        {40,41,56,57,44,45,60,61}
    }};

    if (aMatrix != aExpected) {
        XCTFail("RotateLeftSixteenthsEachQuarter failed");
    }
}

- (void)testTransposeMainDiagonalSixteenthsQuarterA {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalSixteenthsQuarterA();

    FM8 aExpected = {{

        { 0, 1,16,17, 4, 5, 6, 7},
        { 8, 9,24,25,12,13,14,15},
        { 2, 3,18,19,20,21,22,23},
        {10,11,26,27,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalSixteenthsQuarterA failed");
    }
}

- (void)testTransposeMainDiagonalSixteenthsQuarterB {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalSixteenthsQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5,20,21},
        { 8, 9,10,11,12,13,28,29},
        {16,17,18,19, 6, 7,22,23},
        {24,25,26,27,14,15,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalSixteenthsQuarterB failed");
    }
}

- (void)testTransposeMainDiagonalSixteenthsQuarterC {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalSixteenthsQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,48,49,36,37,38,39},
        {40,41,56,57,44,45,46,47},
        {34,35,50,51,52,53,54,55},
        {42,43,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalSixteenthsQuarterC failed");
    }
}

- (void)testTransposeMainDiagonalSixteenthsQuarterD {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalSixteenthsQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,36,37,52,53},
        {40,41,42,43,44,45,60,61},
        {48,49,50,51,38,39,54,55},
        {56,57,58,59,46,47,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalSixteenthsQuarterD failed");
    }
}

- (void)testTransposeMainDiagonalSixteenthsEachQuarter {

    FM8 aMatrix = u();
    aMatrix.TransposeMainDiagonalSixteenthsEachQuarter();

    FM8 aExpected = {{

        { 0, 1,16,17, 4, 5,20,21},
        { 8, 9,24,25,12,13,28,29},
        { 2, 3,18,19, 6, 7,22,23},
        {10,11,26,27,14,15,30,31},

        {32,33,48,49,36,37,52,53},
        {40,41,56,57,44,45,60,61},
        {34,35,50,51,38,39,54,55},
        {42,43,58,59,46,47,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeMainDiagonalSixteenthsEachQuarter failed");
    }
}

- (void)testTransposeAntiDiagonalSixteenthsQuarterA {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalSixteenthsQuarterA();

    FM8 aExpected = {{

        {18,19, 2, 3, 4, 5, 6, 7},
        {26,27,10,11,12,13,14,15},
        {16,17, 0, 1,20,21,22,23},
        {24,25, 8, 9,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalSixteenthsQuarterA failed");
    }
}

- (void)testTransposeAntiDiagonalSixteenthsQuarterB {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalSixteenthsQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3,22,23, 6, 7},
        { 8, 9,10,11,30,31,14,15},
        {16,17,18,19,20,21, 4, 5},
        {24,25,26,27,28,29,12,13},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalSixteenthsQuarterB failed");
    }
}

- (void)testTransposeAntiDiagonalSixteenthsQuarterC {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalSixteenthsQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {50,51,34,35,36,37,38,39},
        {58,59,42,43,44,45,46,47},
        {48,49,32,33,52,53,54,55},
        {56,57,40,41,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalSixteenthsQuarterC failed");
    }
}

- (void)testTransposeAntiDiagonalSixteenthsQuarterD {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalSixteenthsQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,54,55,38,39},
        {40,41,42,43,62,63,46,47},
        {48,49,50,51,52,53,36,37},
        {56,57,58,59,60,61,44,45}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalSixteenthsQuarterD failed");
    }
}

- (void)testTransposeAntiDiagonalSixteenthsEachQuarter {

    FM8 aMatrix = u();
    aMatrix.TransposeAntiDiagonalSixteenthsEachQuarter();

    FM8 aExpected = {{

        {18,19, 2, 3,22,23, 6, 7},
        {26,27,10,11,30,31,14,15},
        {16,17, 0, 1,20,21, 4, 5},
        {24,25, 8, 9,28,29,12,13},

        {50,51,34,35,54,55,38,39},
        {58,59,42,43,62,63,46,47},
        {48,49,32,33,52,53,36,37},
        {56,57,40,41,60,61,44,45}
    }};

    if (aMatrix != aExpected) {
        XCTFail("TransposeAntiDiagonalSixteenthsEachQuarter failed");
    }
}

- (void)testFlipHorizontalSixteenthsQuarterA {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalSixteenthsQuarterA();

    FM8 aExpected = {{

        { 2, 3, 0, 1, 4, 5, 6, 7},
        {10,11, 8, 9,12,13,14,15},
        {18,19,16,17,20,21,22,23},
        {26,27,24,25,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalSixteenthsQuarterA failed");
    }
}

- (void)testFlipHorizontalSixteenthsQuarterB {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalSixteenthsQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 6, 7, 4, 5},
        { 8, 9,10,11,14,15,12,13},
        {16,17,18,19,22,23,20,21},
        {24,25,26,27,30,31,28,29},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalSixteenthsQuarterB failed");
    }
}

- (void)testFlipHorizontalSixteenthsQuarterC {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalSixteenthsQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {34,35,32,33,36,37,38,39},
        {42,43,40,41,44,45,46,47},
        {50,51,48,49,52,53,54,55},
        {58,59,56,57,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalSixteenthsQuarterC failed");
    }
}

- (void)testFlipHorizontalSixteenthsQuarterD {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalSixteenthsQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,38,39,36,37},
        {40,41,42,43,46,47,44,45},
        {48,49,50,51,54,55,52,53},
        {56,57,58,59,62,63,60,61}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalSixteenthsQuarterD failed");
    }
}

- (void)testFlipHorizontalSixteenthsEachQuarter {

    FM8 aMatrix = u();
    aMatrix.FlipHorizontalSixteenthsEachQuarter();

    FM8 aExpected = {{

        { 2, 3, 0, 1, 6, 7, 4, 5},
        {10,11, 8, 9,14,15,12,13},
        {18,19,16,17,22,23,20,21},
        {26,27,24,25,30,31,28,29},

        {34,35,32,33,38,39,36,37},
        {42,43,40,41,46,47,44,45},
        {50,51,48,49,54,55,52,53},
        {58,59,56,57,62,63,60,61}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipHorizontalSixteenthsEachQuarter failed");
    }
}

- (void)testFlipVerticalSixteenthsQuarterA {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalSixteenthsQuarterA();

    FM8 aExpected = {{

        {16,17,18,19, 4, 5, 6, 7},
        {24,25,26,27,12,13,14,15},
        { 0, 1, 2, 3,20,21,22,23},
        { 8, 9,10,11,28,29,30,31},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalSixteenthsQuarterA failed");
    }
}

- (void)testFlipVerticalSixteenthsQuarterB {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalSixteenthsQuarterB();

    FM8 aExpected = {{

        { 0, 1, 2, 3,20,21,22,23},
        { 8, 9,10,11,28,29,30,31},
        {16,17,18,19, 4, 5, 6, 7},
        {24,25,26,27,12,13,14,15},

        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47},
        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalSixteenthsQuarterB failed");
    }
}

- (void)testFlipVerticalSixteenthsQuarterC {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalSixteenthsQuarterC();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {48,49,50,51,36,37,38,39},
        {56,57,58,59,44,45,46,47},
        {32,33,34,35,52,53,54,55},
        {40,41,42,43,60,61,62,63}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalSixteenthsQuarterC failed");
    }
}

- (void)testFlipVerticalSixteenthsQuarterD {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalSixteenthsQuarterD();

    FM8 aExpected = {{

        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},
        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},

        {32,33,34,35,52,53,54,55},
        {40,41,42,43,60,61,62,63},
        {48,49,50,51,36,37,38,39},
        {56,57,58,59,44,45,46,47}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalSixteenthsQuarterD failed");
    }
}

- (void)testFlipVerticalSixteenthsEachQuarter {

    FM8 aMatrix = u();
    aMatrix.FlipVerticalSixteenthsEachQuarter();

    FM8 aExpected = {{

        {16,17,18,19,20,21,22,23},
        {24,25,26,27,28,29,30,31},
        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9,10,11,12,13,14,15},

        {48,49,50,51,52,53,54,55},
        {56,57,58,59,60,61,62,63},
        {32,33,34,35,36,37,38,39},
        {40,41,42,43,44,45,46,47}
    }};

    if (aMatrix != aExpected) {
        XCTFail("FlipVerticalSixteenthsEachQuarter failed");
    }
}

@end
