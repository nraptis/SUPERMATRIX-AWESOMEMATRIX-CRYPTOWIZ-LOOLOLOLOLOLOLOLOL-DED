//
//  FastMatrix8.hpp
//
//  Created by Helen of Troy on 4/25/26.
//

#ifndef FastMatrix8_hpp
#define FastMatrix8_hpp

#include "FastMatrixUnroll.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

enum class FM8UnrollMode : std::uint8_t {
    kInv = 0,
    kRegular = 1
};

enum class FM8FastOp : std::uint8_t {
    kInv = 0,
};

enum class FM8SlowOp : std::uint8_t {
    kInv = 0,
};

// What is needed now:
//

class FM8 final {
public:
    static constexpr std::size_t                kWidth = 8;
    static constexpr std::size_t                kHeight = 8;
    static constexpr std::size_t                kSize = 64;
    
    FM8(const std::uint8_t (&pMatrix)[8][8]);
    FM8(const FM8 &pMatrix);
    FM8();
    
    std::uint8_t                                mDataBase[kSize];
    //std::uint8_t                                *mData[kHeight];
    
    /*
     #    Name    Meaning
     1    RotateRight    whole 8×8 contents
     2    RotateBlocksRight    move 4×4 quarters
     3    RotateRightQuarterA    rotate contents of one 4×4
     4    RotateRightEachQuarter    rotate contents of all 4×4
     5    RotateEachSixteenthRightQuarterA    rotate inside each 2×2 (one quarter)
     6    RotateEachSixteenthRightEachQuarter    rotate inside each 2×2 (all quarters)
     7    RotateSixteenthsRightQuarterA    move 2×2 blocks (one quarter)
     8    RotateSixteenthsRightEachQuarter    move 2×2 blocks (all quarters)
     */
    
    void                                        LoadAndReset(const std::uint8_t *pSource);
    void                                        Store(std::uint8_t *pDest, std::uint8_t pUnrollByte) const;
    
    // Fast Ops:
    void                                        ReverseRow(std::size_t pRow);
    void                                        SwapRows(std::size_t pRowA, std::size_t pRowB);
    void                                        RotateRow(std::size_t pRow, std::size_t pAmount);
    
    // Medium Ops
    void                                        SwapColumns(std::size_t pRowA, std::size_t pRowB);
    void                                        RotateColumn(std::size_t pRowA, std::size_t pRowB);
    
    
    
    void                                        RotateRight();
    void                                        RotateRightBlocks();
    void                                        RotateRightQuarterA();
    void                                        RotateRightQuarterB();
    void                                        RotateRightQuarterC();
    void                                        RotateRightQuarterD();
    void                                        RotateRightEachQuarter();
    
    void                                        RotateRightSixteenthsQuarterA();
    void                                        RotateRightSixteenthsQuarterB();
    void                                        RotateRightSixteenthsQuarterC();
    void                                        RotateRightSixteenthsQuarterD();
    void                                        RotateRightSixteenthsEachQuarter();
    
    void                                        RotateRightEachSixteenthQuarterA();
    void                                        RotateRightEachSixteenthQuarterB();
    void                                        RotateRightEachSixteenthQuarterC();
    void                                        RotateRightEachSixteenthQuarterD();
    void                                        RotateRightEachSixteenthEachQuarter();
    
    
    void                                        RotateLeft();
    void                                        RotateLeftBlocks();
    void                                        RotateLeftQuarterA();
    void                                        RotateLeftQuarterB();
    void                                        RotateLeftQuarterC();
    void                                        RotateLeftQuarterD();
    void                                        RotateLeftEachQuarter();
    
    void                                        RotateLeftSixteenthsQuarterA();
    void                                        RotateLeftSixteenthsQuarterB();
    void                                        RotateLeftSixteenthsQuarterC();
    void                                        RotateLeftSixteenthsQuarterD();
    void                                        RotateLeftSixteenthsEachQuarter();
    
    void                                        RotateLeftEachSixteenthQuarterA();
    void                                        RotateLeftEachSixteenthQuarterB();
    void                                        RotateLeftEachSixteenthQuarterC();
    void                                        RotateLeftEachSixteenthQuarterD();
    void                                        RotateLeftEachSixteenthEachQuarter();
    
    
    
    
    void                                        TransposeMainDiagonal();
    void                                        TransposeMainDiagonalBlocks(); // swaps b and c
    
    void                                        TransposeMainDiagonalQuarterA();
    void                                        TransposeMainDiagonalQuarterB();
    void                                        TransposeMainDiagonalQuarterC();
    void                                        TransposeMainDiagonalQuarterD();
    void                                        TransposeMainDiagonalEachQuarter();
    
    void                                        TransposeMainDiagonalSixteenthsQuarterA();
    void                                        TransposeMainDiagonalSixteenthsQuarterB();
    void                                        TransposeMainDiagonalSixteenthsQuarterC();
    void                                        TransposeMainDiagonalSixteenthsQuarterD();
    void                                        TransposeMainDiagonalSixteenthsEachQuarter();
    
    void                                        TransposeMainDiagonalEachSixteenthQuarterA();
    void                                        TransposeMainDiagonalEachSixteenthQuarterB();
    void                                        TransposeMainDiagonalEachSixteenthQuarterC();
    void                                        TransposeMainDiagonalEachSixteenthQuarterD();
    void                                        TransposeMainDiagonalEachSixteenthEachQuarter();
    
    
    
    
    
    void                                        TransposeAntiDiagonal();
    void                                        TransposeAntiDiagonalBlocks(); // swaps a and d
    
    void                                        TransposeAntiDiagonalQuarterA();
    void                                        TransposeAntiDiagonalQuarterB();
    void                                        TransposeAntiDiagonalQuarterC();
    void                                        TransposeAntiDiagonalQuarterD();
    void                                        TransposeAntiDiagonalEachQuarter();
    
    void                                        TransposeAntiDiagonalSixteenthsQuarterA();
    void                                        TransposeAntiDiagonalSixteenthsQuarterB();
    void                                        TransposeAntiDiagonalSixteenthsQuarterC();
    void                                        TransposeAntiDiagonalSixteenthsQuarterD();
    void                                        TransposeAntiDiagonalSixteenthsEachQuarter();
    
    void                                        TransposeAntiDiagonalEachSixteenthQuarterA();
    void                                        TransposeAntiDiagonalEachSixteenthQuarterB();
    void                                        TransposeAntiDiagonalEachSixteenthQuarterC();
    void                                        TransposeAntiDiagonalEachSixteenthQuarterD();
    void                                        TransposeAntiDiagonalEachSixteenthEachQuarter();
    
    
    
    
    
    void                                        FlipHorizontal();
    void                                        FlipHorizontalBlocks();
    
    void                                        FlipHorizontalQuarterA();
    void                                        FlipHorizontalQuarterB();
    void                                        FlipHorizontalQuarterC();
    void                                        FlipHorizontalQuarterD();
    void                                        FlipHorizontalEachQuarter();
    
    void                                        FlipHorizontalSixteenthsQuarterA();
    void                                        FlipHorizontalSixteenthsQuarterB();
    void                                        FlipHorizontalSixteenthsQuarterC();
    void                                        FlipHorizontalSixteenthsQuarterD();
    void                                        FlipHorizontalSixteenthsEachQuarter();
    
    void                                        FlipHorizontalEachSixteenthQuarterA();
    void                                        FlipHorizontalEachSixteenthQuarterB();
    void                                        FlipHorizontalEachSixteenthQuarterC();
    void                                        FlipHorizontalEachSixteenthQuarterD();
    void                                        FlipHorizontalEachSixteenthEachQuarter();
    
    
    
    void                                        FlipVertical();
    void                                        FlipVerticalBlocks();
    
    void                                        FlipVerticalQuarterA();
    void                                        FlipVerticalQuarterB();
    void                                        FlipVerticalQuarterC();
    void                                        FlipVerticalQuarterD();
    void                                        FlipVerticalEachQuarter();
    
    void                                        FlipVerticalSixteenthsQuarterA();
    void                                        FlipVerticalSixteenthsQuarterB();
    void                                        FlipVerticalSixteenthsQuarterC();
    void                                        FlipVerticalSixteenthsQuarterD();
    void                                        FlipVerticalSixteenthsEachQuarter();
    
    void                                        FlipVerticalEachSixteenthQuarterA();
    void                                        FlipVerticalEachSixteenthQuarterB();
    void                                        FlipVerticalEachSixteenthQuarterC();
    void                                        FlipVerticalEachSixteenthQuarterD();
    void                                        FlipVerticalEachSixteenthEachQuarter();
    
    
    
    
    // Testing
    
    bool                                        operator==(const FM8& other) const;
    bool                                        operator!=(const FM8& other) const;
    
    
private:
    
    inline void                                 InitRows();
    
    
    std::uint8_t                                mTemp[kSize];
    
};



#endif /* FastMatrix8_hpp */
