//
//  FastMatrix8.cpp
//
//  Created by Helen of Troy on 4/25/26.
//

#include "FastMatrix8.hpp"

FM8::FM8(const std::uint8_t (&pMatrix)[8][8]) {
    for (std::size_t aRowIndex = 0; aRowIndex < 8; ++aRowIndex) {
        memcpy(mDataBase + (aRowIndex << 3), pMatrix[aRowIndex], 8);
    }
}

FM8::FM8(const FM8 &pMatrix) {
    memcpy(mDataBase, pMatrix.mDataBase, kSize);
}

FM8::FM8() {
    
}

/*
inline void FM8::InitRows() {
    for (int i = 0; i < 8; i++) { mData[i] = &mDataBase[i << 3]; }
    for (int i = 0; i < 8; i++) { mTemp[i] = &mTemp[i << 3]; }
}
*/

void FM8::LoadAndReset(const std::uint8_t *pSource) {
    memcpy(mDataBase, pSource, kSize);
}

void FM8::Store(std::uint8_t *pDest, std::uint8_t pUnrollByte) const {

    const std::uint8_t* aOrder = FM8UnrollTableA::kUnrollTable[pUnrollByte];
    memcpy(pDest +  0, mDataBase + (aOrder[0] << 3), 8);
    memcpy(pDest +  8, mDataBase + (aOrder[1] << 3), 8);
    memcpy(pDest + 16, mDataBase + (aOrder[2] << 3), 8);
    memcpy(pDest + 24, mDataBase + (aOrder[3] << 3), 8);
    memcpy(pDest + 32, mDataBase + (aOrder[4] << 3), 8);
    memcpy(pDest + 40, mDataBase + (aOrder[5] << 3), 8);
    memcpy(pDest + 48, mDataBase + (aOrder[6] << 3), 8);
    memcpy(pDest + 56, mDataBase + (aOrder[7] << 3), 8);
}

void FM8::SwapRows(std::size_t pRowA, std::size_t pRowB) {

    std::size_t aRowA = (pRowA & 7) << 3;
    std::size_t aRowB = (pRowB & 7) << 3;

    if (aRowA == aRowB) { return; }

    memcpy(mTemp,          mDataBase + aRowA, 8);
    memcpy(mDataBase + aRowA,  mDataBase + aRowB, 8);
    memcpy(mDataBase + aRowB,  mTemp,         8);
}

void FM8::ReverseRow(std::size_t pRow) {

    std::size_t aBase = (pRow & 7) << 3;
    std::uint8_t aHold;

    aHold = mDataBase[aBase + 0]; mDataBase[aBase + 0] = mDataBase[aBase + 7]; mDataBase[aBase + 7] = aHold;
    aHold = mDataBase[aBase + 1]; mDataBase[aBase + 1] = mDataBase[aBase + 6]; mDataBase[aBase + 6] = aHold;
    aHold = mDataBase[aBase + 2]; mDataBase[aBase + 2] = mDataBase[aBase + 5]; mDataBase[aBase + 5] = aHold;
    aHold = mDataBase[aBase + 3]; mDataBase[aBase + 3] = mDataBase[aBase + 4]; mDataBase[aBase + 4] = aHold;
}

void FM8::RotateRow(std::size_t pRow, std::size_t pAmount) {

    std::size_t aBase   = (pRow & 7) << 3;
    std::size_t aAmount = pAmount & 7;

    if (aAmount == 0) { return; }

    for (std::size_t aColumnIndex = 0; aColumnIndex < 8; ++aColumnIndex) {
        mTemp[aColumnIndex] =
            mDataBase[aBase + ((aColumnIndex + aAmount) & 7)];
    }

    memcpy(mDataBase + aBase, mTemp, 8);
}

void FM8::SwapColumns(std::size_t pColA, std::size_t pColB) {

    std::size_t aColA = pColA & 7;
    std::size_t aColB = pColB & 7;

    if (aColA == aColB) { return; }

    std::uint8_t aHold;

    for (std::size_t aRowIndex = 0; aRowIndex < 8; ++aRowIndex) {

        std::size_t aBase = aRowIndex << 3;

        aHold = mDataBase[aBase + aColA];
        mDataBase[aBase + aColA] = mDataBase[aBase + aColB];
        mDataBase[aBase + aColB] = aHold;
    }
}

void FM8::RotateColumn(std::size_t pCol, std::size_t pAmount) {

    std::size_t aCol    = pCol & 7;
    std::size_t aAmount = pAmount & 7;

    if (aAmount == 0) { return; }

    // gather
    for (std::size_t aRowIndex = 0; aRowIndex < 8; ++aRowIndex) {
        mTemp[aRowIndex] =
            mDataBase[((aRowIndex + 8 - aAmount) & 7) << 3 | aCol];
    }

    // scatter
    for (std::size_t aRowIndex = 0; aRowIndex < 8; ++aRowIndex) {
        mDataBase[(aRowIndex << 3) + aCol] = mTemp[aRowIndex];
    }
}

// 90° clockwise
void FM8::RotateRight() {
    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aDst = mTemp;
    aDst[ 0] = aSrc[56]; aDst[ 1] = aSrc[48]; aDst[ 2] = aSrc[40]; aDst[ 3] = aSrc[32];
    aDst[ 4] = aSrc[24]; aDst[ 5] = aSrc[16]; aDst[ 6] = aSrc[ 8]; aDst[ 7] = aSrc[ 0];
    aDst[ 8] = aSrc[57]; aDst[ 9] = aSrc[49]; aDst[10] = aSrc[41]; aDst[11] = aSrc[33];
    aDst[12] = aSrc[25]; aDst[13] = aSrc[17]; aDst[14] = aSrc[ 9]; aDst[15] = aSrc[ 1];
    aDst[16] = aSrc[58]; aDst[17] = aSrc[50]; aDst[18] = aSrc[42]; aDst[19] = aSrc[34];
    aDst[20] = aSrc[26]; aDst[21] = aSrc[18]; aDst[22] = aSrc[10]; aDst[23] = aSrc[ 2];
    aDst[24] = aSrc[59]; aDst[25] = aSrc[51]; aDst[26] = aSrc[43]; aDst[27] = aSrc[35];
    aDst[28] = aSrc[27]; aDst[29] = aSrc[19]; aDst[30] = aSrc[11]; aDst[31] = aSrc[ 3];
    aDst[32] = aSrc[60]; aDst[33] = aSrc[52]; aDst[34] = aSrc[44]; aDst[35] = aSrc[36];
    aDst[36] = aSrc[28]; aDst[37] = aSrc[20]; aDst[38] = aSrc[12]; aDst[39] = aSrc[ 4];
    aDst[40] = aSrc[61]; aDst[41] = aSrc[53]; aDst[42] = aSrc[45]; aDst[43] = aSrc[37];
    aDst[44] = aSrc[29]; aDst[45] = aSrc[21]; aDst[46] = aSrc[13]; aDst[47] = aSrc[ 5];
    aDst[48] = aSrc[62]; aDst[49] = aSrc[54]; aDst[50] = aSrc[46]; aDst[51] = aSrc[38];
    aDst[52] = aSrc[30]; aDst[53] = aSrc[22]; aDst[54] = aSrc[14]; aDst[55] = aSrc[ 6];
    aDst[56] = aSrc[63]; aDst[57] = aSrc[55]; aDst[58] = aSrc[47]; aDst[59] = aSrc[39];
    aDst[60] = aSrc[31]; aDst[61] = aSrc[23]; aDst[62] = aSrc[15]; aDst[63] = aSrc[ 7];
    memcpy(mDataBase, mTemp, 64);
}

// 90° counter-clockwise
void FM8::RotateLeft() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aDst = mTemp;
    aDst[ 0] = aSrc[ 7]; aDst[ 1] = aSrc[15]; aDst[ 2] = aSrc[23]; aDst[ 3] = aSrc[31];
    aDst[ 4] = aSrc[39]; aDst[ 5] = aSrc[47]; aDst[ 6] = aSrc[55]; aDst[ 7] = aSrc[63];
    aDst[ 8] = aSrc[ 6]; aDst[ 9] = aSrc[14]; aDst[10] = aSrc[22]; aDst[11] = aSrc[30];
    aDst[12] = aSrc[38]; aDst[13] = aSrc[46]; aDst[14] = aSrc[54]; aDst[15] = aSrc[62];
    aDst[16] = aSrc[ 5]; aDst[17] = aSrc[13]; aDst[18] = aSrc[21]; aDst[19] = aSrc[29];
    aDst[20] = aSrc[37]; aDst[21] = aSrc[45]; aDst[22] = aSrc[53]; aDst[23] = aSrc[61];
    aDst[24] = aSrc[ 4]; aDst[25] = aSrc[12]; aDst[26] = aSrc[20]; aDst[27] = aSrc[28];
    aDst[28] = aSrc[36]; aDst[29] = aSrc[44]; aDst[30] = aSrc[52]; aDst[31] = aSrc[60];
    aDst[32] = aSrc[ 3]; aDst[33] = aSrc[11]; aDst[34] = aSrc[19]; aDst[35] = aSrc[27];
    aDst[36] = aSrc[35]; aDst[37] = aSrc[43]; aDst[38] = aSrc[51]; aDst[39] = aSrc[59];
    aDst[40] = aSrc[ 2]; aDst[41] = aSrc[10]; aDst[42] = aSrc[18]; aDst[43] = aSrc[26];
    aDst[44] = aSrc[34]; aDst[45] = aSrc[42]; aDst[46] = aSrc[50]; aDst[47] = aSrc[58];
    aDst[48] = aSrc[ 1]; aDst[49] = aSrc[ 9]; aDst[50] = aSrc[17]; aDst[51] = aSrc[25];
    aDst[52] = aSrc[33]; aDst[53] = aSrc[41]; aDst[54] = aSrc[49]; aDst[55] = aSrc[57];
    aDst[56] = aSrc[ 0]; aDst[57] = aSrc[ 8]; aDst[58] = aSrc[16]; aDst[59] = aSrc[24];
    aDst[60] = aSrc[32]; aDst[61] = aSrc[40]; aDst[62] = aSrc[48]; aDst[63] = aSrc[56];
    memcpy(mDataBase, mTemp, 64);
}

void FM8::RotateRightBlocks() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    // copy whole matrix to temp
    memcpy(aTmp, aSrc, 64);

    // A <- C
    aSrc[ 0] = aTmp[32]; aSrc[ 1] = aTmp[33]; aSrc[ 2] = aTmp[34]; aSrc[ 3] = aTmp[35];
    aSrc[ 8] = aTmp[40]; aSrc[ 9] = aTmp[41]; aSrc[10] = aTmp[42]; aSrc[11] = aTmp[43];
    aSrc[16] = aTmp[48]; aSrc[17] = aTmp[49]; aSrc[18] = aTmp[50]; aSrc[19] = aTmp[51];
    aSrc[24] = aTmp[56]; aSrc[25] = aTmp[57]; aSrc[26] = aTmp[58]; aSrc[27] = aTmp[59];

    // B <- A
    aSrc[ 4] = aTmp[ 0]; aSrc[ 5] = aTmp[ 1]; aSrc[ 6] = aTmp[ 2]; aSrc[ 7] = aTmp[ 3];
    aSrc[12] = aTmp[ 8]; aSrc[13] = aTmp[ 9]; aSrc[14] = aTmp[10]; aSrc[15] = aTmp[11];
    aSrc[20] = aTmp[16]; aSrc[21] = aTmp[17]; aSrc[22] = aTmp[18]; aSrc[23] = aTmp[19];
    aSrc[28] = aTmp[24]; aSrc[29] = aTmp[25]; aSrc[30] = aTmp[26]; aSrc[31] = aTmp[27];

    // D <- B
    aSrc[36] = aTmp[ 4]; aSrc[37] = aTmp[ 5]; aSrc[38] = aTmp[ 6]; aSrc[39] = aTmp[ 7];
    aSrc[44] = aTmp[12]; aSrc[45] = aTmp[13]; aSrc[46] = aTmp[14]; aSrc[47] = aTmp[15];
    aSrc[52] = aTmp[20]; aSrc[53] = aTmp[21]; aSrc[54] = aTmp[22]; aSrc[55] = aTmp[23];
    aSrc[60] = aTmp[28]; aSrc[61] = aTmp[29]; aSrc[62] = aTmp[30]; aSrc[63] = aTmp[31];

    // C <- D
    aSrc[32] = aTmp[36]; aSrc[33] = aTmp[37]; aSrc[34] = aTmp[38]; aSrc[35] = aTmp[39];
    aSrc[40] = aTmp[44]; aSrc[41] = aTmp[45]; aSrc[42] = aTmp[46]; aSrc[43] = aTmp[47];
    aSrc[48] = aTmp[52]; aSrc[49] = aTmp[53]; aSrc[50] = aTmp[54]; aSrc[51] = aTmp[55];
    aSrc[56] = aTmp[60]; aSrc[57] = aTmp[61]; aSrc[58] = aTmp[62]; aSrc[59] = aTmp[63];
}

void FM8::RotateLeftBlocks() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    // copy whole matrix to temp
    memcpy(aTmp, aSrc, 64);

    // A <- B
    aSrc[ 0] = aTmp[ 4]; aSrc[ 1] = aTmp[ 5]; aSrc[ 2] = aTmp[ 6]; aSrc[ 3] = aTmp[ 7];
    aSrc[ 8] = aTmp[12]; aSrc[ 9] = aTmp[13]; aSrc[10] = aTmp[14]; aSrc[11] = aTmp[15];
    aSrc[16] = aTmp[20]; aSrc[17] = aTmp[21]; aSrc[18] = aTmp[22]; aSrc[19] = aTmp[23];
    aSrc[24] = aTmp[28]; aSrc[25] = aTmp[29]; aSrc[26] = aTmp[30]; aSrc[27] = aTmp[31];

    // B <- D
    aSrc[ 4] = aTmp[36]; aSrc[ 5] = aTmp[37]; aSrc[ 6] = aTmp[38]; aSrc[ 7] = aTmp[39];
    aSrc[12] = aTmp[44]; aSrc[13] = aTmp[45]; aSrc[14] = aTmp[46]; aSrc[15] = aTmp[47];
    aSrc[20] = aTmp[52]; aSrc[21] = aTmp[53]; aSrc[22] = aTmp[54]; aSrc[23] = aTmp[55];
    aSrc[28] = aTmp[60]; aSrc[29] = aTmp[61]; aSrc[30] = aTmp[62]; aSrc[31] = aTmp[63];

    // D <- C
    aSrc[36] = aTmp[32]; aSrc[37] = aTmp[33]; aSrc[38] = aTmp[34]; aSrc[39] = aTmp[35];
    aSrc[44] = aTmp[40]; aSrc[45] = aTmp[41]; aSrc[46] = aTmp[42]; aSrc[47] = aTmp[43];
    aSrc[52] = aTmp[48]; aSrc[53] = aTmp[49]; aSrc[54] = aTmp[50]; aSrc[55] = aTmp[51];
    aSrc[60] = aTmp[56]; aSrc[61] = aTmp[57]; aSrc[62] = aTmp[58]; aSrc[63] = aTmp[59];

    // C <- A
    aSrc[32] = aTmp[ 0]; aSrc[33] = aTmp[ 1]; aSrc[34] = aTmp[ 2]; aSrc[35] = aTmp[ 3];
    aSrc[40] = aTmp[ 8]; aSrc[41] = aTmp[ 9]; aSrc[42] = aTmp[10]; aSrc[43] = aTmp[11];
    aSrc[48] = aTmp[16]; aSrc[49] = aTmp[17]; aSrc[50] = aTmp[18]; aSrc[51] = aTmp[19];
    aSrc[56] = aTmp[24]; aSrc[57] = aTmp[25]; aSrc[58] = aTmp[26]; aSrc[59] = aTmp[27];
}

void FM8::RotateRightQuarterA() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[ 0] = aSrc[24]; aTmp[ 1] = aSrc[16]; aTmp[ 2] = aSrc[ 8]; aTmp[ 3] = aSrc[ 0];
    aTmp[ 8] = aSrc[25]; aTmp[ 9] = aSrc[17]; aTmp[10] = aSrc[ 9]; aTmp[11] = aSrc[ 1];
    aTmp[16] = aSrc[26]; aTmp[17] = aSrc[18]; aTmp[18] = aSrc[10]; aTmp[19] = aSrc[ 2];
    aTmp[24] = aSrc[27]; aTmp[25] = aSrc[19]; aTmp[26] = aSrc[11]; aTmp[27] = aSrc[ 3];

    aSrc[ 0] = aTmp[ 0]; aSrc[ 1] = aTmp[ 1]; aSrc[ 2] = aTmp[ 2]; aSrc[ 3] = aTmp[ 3];
    aSrc[ 8] = aTmp[ 8]; aSrc[ 9] = aTmp[ 9]; aSrc[10] = aTmp[10]; aSrc[11] = aTmp[11];
    aSrc[16] = aTmp[16]; aSrc[17] = aTmp[17]; aSrc[18] = aTmp[18]; aSrc[19] = aTmp[19];
    aSrc[24] = aTmp[24]; aSrc[25] = aTmp[25]; aSrc[26] = aTmp[26]; aSrc[27] = aTmp[27];
}

void FM8::RotateRightQuarterB() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[ 4] = aSrc[28]; aTmp[ 5] = aSrc[20]; aTmp[ 6] = aSrc[12]; aTmp[ 7] = aSrc[ 4];
    aTmp[12] = aSrc[29]; aTmp[13] = aSrc[21]; aTmp[14] = aSrc[13]; aTmp[15] = aSrc[ 5];
    aTmp[20] = aSrc[30]; aTmp[21] = aSrc[22]; aTmp[22] = aSrc[14]; aTmp[23] = aSrc[ 6];
    aTmp[28] = aSrc[31]; aTmp[29] = aSrc[23]; aTmp[30] = aSrc[15]; aTmp[31] = aSrc[ 7];

    aSrc[ 4] = aTmp[ 4]; aSrc[ 5] = aTmp[ 5]; aSrc[ 6] = aTmp[ 6]; aSrc[ 7] = aTmp[ 7];
    aSrc[12] = aTmp[12]; aSrc[13] = aTmp[13]; aSrc[14] = aTmp[14]; aSrc[15] = aTmp[15];
    aSrc[20] = aTmp[20]; aSrc[21] = aTmp[21]; aSrc[22] = aTmp[22]; aSrc[23] = aTmp[23];
    aSrc[28] = aTmp[28]; aSrc[29] = aTmp[29]; aSrc[30] = aTmp[30]; aSrc[31] = aTmp[31];
}

void FM8::RotateRightQuarterC() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[32] = aSrc[56]; aTmp[33] = aSrc[48]; aTmp[34] = aSrc[40]; aTmp[35] = aSrc[32];
    aTmp[40] = aSrc[57]; aTmp[41] = aSrc[49]; aTmp[42] = aSrc[41]; aTmp[43] = aSrc[33];
    aTmp[48] = aSrc[58]; aTmp[49] = aSrc[50]; aTmp[50] = aSrc[42]; aTmp[51] = aSrc[34];
    aTmp[56] = aSrc[59]; aTmp[57] = aSrc[51]; aTmp[58] = aSrc[43]; aTmp[59] = aSrc[35];

    aSrc[32] = aTmp[32]; aSrc[33] = aTmp[33]; aSrc[34] = aTmp[34]; aSrc[35] = aTmp[35];
    aSrc[40] = aTmp[40]; aSrc[41] = aTmp[41]; aSrc[42] = aTmp[42]; aSrc[43] = aTmp[43];
    aSrc[48] = aTmp[48]; aSrc[49] = aTmp[49]; aSrc[50] = aTmp[50]; aSrc[51] = aTmp[51];
    aSrc[56] = aTmp[56]; aSrc[57] = aTmp[57]; aSrc[58] = aTmp[58]; aSrc[59] = aTmp[59];
}

void FM8::RotateRightQuarterD() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[36] = aSrc[60]; aTmp[37] = aSrc[52]; aTmp[38] = aSrc[44]; aTmp[39] = aSrc[36];
    aTmp[44] = aSrc[61]; aTmp[45] = aSrc[53]; aTmp[46] = aSrc[45]; aTmp[47] = aSrc[37];
    aTmp[52] = aSrc[62]; aTmp[53] = aSrc[54]; aTmp[54] = aSrc[46]; aTmp[55] = aSrc[38];
    aTmp[60] = aSrc[63]; aTmp[61] = aSrc[55]; aTmp[62] = aSrc[47]; aTmp[63] = aSrc[39];

    aSrc[36] = aTmp[36]; aSrc[37] = aTmp[37]; aSrc[38] = aTmp[38]; aSrc[39] = aTmp[39];
    aSrc[44] = aTmp[44]; aSrc[45] = aTmp[45]; aSrc[46] = aTmp[46]; aSrc[47] = aTmp[47];
    aSrc[52] = aTmp[52]; aSrc[53] = aTmp[53]; aSrc[54] = aTmp[54]; aSrc[55] = aTmp[55];
    aSrc[60] = aTmp[60]; aSrc[61] = aTmp[61]; aSrc[62] = aTmp[62]; aSrc[63] = aTmp[63];
}

void FM8::RotateRightEachQuarter() {
    RotateRightQuarterA();
    RotateRightQuarterB();
    RotateRightQuarterC();
    RotateRightQuarterD();
}

void FM8::RotateLeftQuarterA() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[ 0] = aSrc[ 3]; aTmp[ 1] = aSrc[11]; aTmp[ 2] = aSrc[19]; aTmp[ 3] = aSrc[27];
    aTmp[ 8] = aSrc[ 2]; aTmp[ 9] = aSrc[10]; aTmp[10] = aSrc[18]; aTmp[11] = aSrc[26];
    aTmp[16] = aSrc[ 1]; aTmp[17] = aSrc[ 9]; aTmp[18] = aSrc[17]; aTmp[19] = aSrc[25];
    aTmp[24] = aSrc[ 0]; aTmp[25] = aSrc[ 8]; aTmp[26] = aSrc[16]; aTmp[27] = aSrc[24];

    aSrc[ 0] = aTmp[ 0]; aSrc[ 1] = aTmp[ 1]; aSrc[ 2] = aTmp[ 2]; aSrc[ 3] = aTmp[ 3];
    aSrc[ 8] = aTmp[ 8]; aSrc[ 9] = aTmp[ 9]; aSrc[10] = aTmp[10]; aSrc[11] = aTmp[11];
    aSrc[16] = aTmp[16]; aSrc[17] = aTmp[17]; aSrc[18] = aTmp[18]; aSrc[19] = aTmp[19];
    aSrc[24] = aTmp[24]; aSrc[25] = aTmp[25]; aSrc[26] = aTmp[26]; aSrc[27] = aTmp[27];
}

void FM8::RotateLeftQuarterB() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[ 4] = aSrc[ 7]; aTmp[ 5] = aSrc[15]; aTmp[ 6] = aSrc[23]; aTmp[ 7] = aSrc[31];
    aTmp[12] = aSrc[ 6]; aTmp[13] = aSrc[14]; aTmp[14] = aSrc[22]; aTmp[15] = aSrc[30];
    aTmp[20] = aSrc[ 5]; aTmp[21] = aSrc[13]; aTmp[22] = aSrc[21]; aTmp[23] = aSrc[29];
    aTmp[28] = aSrc[ 4]; aTmp[29] = aSrc[12]; aTmp[30] = aSrc[20]; aTmp[31] = aSrc[28];

    aSrc[ 4] = aTmp[ 4]; aSrc[ 5] = aTmp[ 5]; aSrc[ 6] = aTmp[ 6]; aSrc[ 7] = aTmp[ 7];
    aSrc[12] = aTmp[12]; aSrc[13] = aTmp[13]; aSrc[14] = aTmp[14]; aSrc[15] = aTmp[15];
    aSrc[20] = aTmp[20]; aSrc[21] = aTmp[21]; aSrc[22] = aTmp[22]; aSrc[23] = aTmp[23];
    aSrc[28] = aTmp[28]; aSrc[29] = aTmp[29]; aSrc[30] = aTmp[30]; aSrc[31] = aTmp[31];
}

void FM8::RotateLeftQuarterC() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[32] = aSrc[35]; aTmp[33] = aSrc[43]; aTmp[34] = aSrc[51]; aTmp[35] = aSrc[59];
    aTmp[40] = aSrc[34]; aTmp[41] = aSrc[42]; aTmp[42] = aSrc[50]; aTmp[43] = aSrc[58];
    aTmp[48] = aSrc[33]; aTmp[49] = aSrc[41]; aTmp[50] = aSrc[49]; aTmp[51] = aSrc[57];
    aTmp[56] = aSrc[32]; aTmp[57] = aSrc[40]; aTmp[58] = aSrc[48]; aTmp[59] = aSrc[56];

    aSrc[32] = aTmp[32]; aSrc[33] = aTmp[33]; aSrc[34] = aTmp[34]; aSrc[35] = aTmp[35];
    aSrc[40] = aTmp[40]; aSrc[41] = aTmp[41]; aSrc[42] = aTmp[42]; aSrc[43] = aTmp[43];
    aSrc[48] = aTmp[48]; aSrc[49] = aTmp[49]; aSrc[50] = aTmp[50]; aSrc[51] = aTmp[51];
    aSrc[56] = aTmp[56]; aSrc[57] = aTmp[57]; aSrc[58] = aTmp[58]; aSrc[59] = aTmp[59];
}

void FM8::RotateLeftQuarterD() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[36] = aSrc[39]; aTmp[37] = aSrc[47]; aTmp[38] = aSrc[55]; aTmp[39] = aSrc[63];
    aTmp[44] = aSrc[38]; aTmp[45] = aSrc[46]; aTmp[46] = aSrc[54]; aTmp[47] = aSrc[62];
    aTmp[52] = aSrc[37]; aTmp[53] = aSrc[45]; aTmp[54] = aSrc[53]; aTmp[55] = aSrc[61];
    aTmp[60] = aSrc[36]; aTmp[61] = aSrc[44]; aTmp[62] = aSrc[52]; aTmp[63] = aSrc[60];

    aSrc[36] = aTmp[36]; aSrc[37] = aTmp[37]; aSrc[38] = aTmp[38]; aSrc[39] = aTmp[39];
    aSrc[44] = aTmp[44]; aSrc[45] = aTmp[45]; aSrc[46] = aTmp[46]; aSrc[47] = aTmp[47];
    aSrc[52] = aTmp[52]; aSrc[53] = aTmp[53]; aSrc[54] = aTmp[54]; aSrc[55] = aTmp[55];
    aSrc[60] = aTmp[60]; aSrc[61] = aTmp[61]; aSrc[62] = aTmp[62]; aSrc[63] = aTmp[63];
}

void FM8::RotateLeftEachQuarter() {
    RotateLeftQuarterA();
    RotateLeftQuarterB();
    RotateLeftQuarterC();
    RotateLeftQuarterD();
}

// main diagonal flip (same as transpose)
void FM8::TransposeMainDiagonal() {
    std::uint8_t aHold;
    aHold = mDataBase[ 1]; mDataBase[ 1] = mDataBase[ 8]; mDataBase[ 8] = aHold;
    aHold = mDataBase[ 2]; mDataBase[ 2] = mDataBase[16]; mDataBase[16] = aHold;
    aHold = mDataBase[ 3]; mDataBase[ 3] = mDataBase[24]; mDataBase[24] = aHold;
    aHold = mDataBase[ 4]; mDataBase[ 4] = mDataBase[32]; mDataBase[32] = aHold;
    aHold = mDataBase[ 5]; mDataBase[ 5] = mDataBase[40]; mDataBase[40] = aHold;
    aHold = mDataBase[ 6]; mDataBase[ 6] = mDataBase[48]; mDataBase[48] = aHold;
    aHold = mDataBase[ 7]; mDataBase[ 7] = mDataBase[56]; mDataBase[56] = aHold;
    aHold = mDataBase[10]; mDataBase[10] = mDataBase[17]; mDataBase[17] = aHold;
    aHold = mDataBase[11]; mDataBase[11] = mDataBase[25]; mDataBase[25] = aHold;
    aHold = mDataBase[12]; mDataBase[12] = mDataBase[33]; mDataBase[33] = aHold;
    aHold = mDataBase[13]; mDataBase[13] = mDataBase[41]; mDataBase[41] = aHold;
    aHold = mDataBase[14]; mDataBase[14] = mDataBase[49]; mDataBase[49] = aHold;
    aHold = mDataBase[15]; mDataBase[15] = mDataBase[57]; mDataBase[57] = aHold;
    aHold = mDataBase[19]; mDataBase[19] = mDataBase[26]; mDataBase[26] = aHold;
    aHold = mDataBase[20]; mDataBase[20] = mDataBase[34]; mDataBase[34] = aHold;
    aHold = mDataBase[21]; mDataBase[21] = mDataBase[42]; mDataBase[42] = aHold;
    aHold = mDataBase[22]; mDataBase[22] = mDataBase[50]; mDataBase[50] = aHold;
    aHold = mDataBase[23]; mDataBase[23] = mDataBase[58]; mDataBase[58] = aHold;
    aHold = mDataBase[28]; mDataBase[28] = mDataBase[35]; mDataBase[35] = aHold;
    aHold = mDataBase[29]; mDataBase[29] = mDataBase[43]; mDataBase[43] = aHold;
    aHold = mDataBase[30]; mDataBase[30] = mDataBase[51]; mDataBase[51] = aHold;
    aHold = mDataBase[31]; mDataBase[31] = mDataBase[59]; mDataBase[59] = aHold;
    aHold = mDataBase[37]; mDataBase[37] = mDataBase[44]; mDataBase[44] = aHold;
    aHold = mDataBase[38]; mDataBase[38] = mDataBase[52]; mDataBase[52] = aHold;
    aHold = mDataBase[39]; mDataBase[39] = mDataBase[60]; mDataBase[60] = aHold;
    aHold = mDataBase[46]; mDataBase[46] = mDataBase[53]; mDataBase[53] = aHold;
    aHold = mDataBase[47]; mDataBase[47] = mDataBase[61]; mDataBase[61] = aHold;
    aHold = mDataBase[55]; mDataBase[55] = mDataBase[62]; mDataBase[62] = aHold;
}

void FM8::TransposeMainDiagonalQuarterA() {

    std::uint8_t aHold;

    aHold = mDataBase[ 1]; mDataBase[ 1] = mDataBase[ 8]; mDataBase[ 8] = aHold;
    aHold = mDataBase[ 2]; mDataBase[ 2] = mDataBase[16]; mDataBase[16] = aHold;
    aHold = mDataBase[ 3]; mDataBase[ 3] = mDataBase[24]; mDataBase[24] = aHold;

    aHold = mDataBase[10]; mDataBase[10] = mDataBase[17]; mDataBase[17] = aHold;
    aHold = mDataBase[11]; mDataBase[11] = mDataBase[25]; mDataBase[25] = aHold;

    aHold = mDataBase[19]; mDataBase[19] = mDataBase[26]; mDataBase[26] = aHold;
}

void FM8::TransposeMainDiagonalQuarterB() {

    std::uint8_t aHold;

    aHold = mDataBase[ 5]; mDataBase[ 5] = mDataBase[12]; mDataBase[12] = aHold;
    aHold = mDataBase[ 6]; mDataBase[ 6] = mDataBase[20]; mDataBase[20] = aHold;
    aHold = mDataBase[ 7]; mDataBase[ 7] = mDataBase[28]; mDataBase[28] = aHold;

    aHold = mDataBase[14]; mDataBase[14] = mDataBase[21]; mDataBase[21] = aHold;
    aHold = mDataBase[15]; mDataBase[15] = mDataBase[29]; mDataBase[29] = aHold;

    aHold = mDataBase[23]; mDataBase[23] = mDataBase[30]; mDataBase[30] = aHold;
}

void FM8::TransposeMainDiagonalQuarterC() {

    std::uint8_t aHold;

    aHold = mDataBase[33]; mDataBase[33] = mDataBase[40]; mDataBase[40] = aHold;
    aHold = mDataBase[34]; mDataBase[34] = mDataBase[48]; mDataBase[48] = aHold;
    aHold = mDataBase[35]; mDataBase[35] = mDataBase[56]; mDataBase[56] = aHold;

    aHold = mDataBase[42]; mDataBase[42] = mDataBase[49]; mDataBase[49] = aHold;
    aHold = mDataBase[43]; mDataBase[43] = mDataBase[57]; mDataBase[57] = aHold;

    aHold = mDataBase[51]; mDataBase[51] = mDataBase[58]; mDataBase[58] = aHold;
}

void FM8::TransposeMainDiagonalQuarterD() {

    std::uint8_t aHold;

    aHold = mDataBase[37]; mDataBase[37] = mDataBase[44]; mDataBase[44] = aHold;
    aHold = mDataBase[38]; mDataBase[38] = mDataBase[52]; mDataBase[52] = aHold;
    aHold = mDataBase[39]; mDataBase[39] = mDataBase[60]; mDataBase[60] = aHold;

    aHold = mDataBase[46]; mDataBase[46] = mDataBase[53]; mDataBase[53] = aHold;
    aHold = mDataBase[47]; mDataBase[47] = mDataBase[61]; mDataBase[61] = aHold;

    aHold = mDataBase[55]; mDataBase[55] = mDataBase[62]; mDataBase[62] = aHold;
}

void FM8::TransposeMainDiagonalEachQuarter() {
    TransposeMainDiagonalQuarterA();
    TransposeMainDiagonalQuarterB();
    TransposeMainDiagonalQuarterC();
    TransposeMainDiagonalQuarterD();
}


// anti-diagonal flip
void FM8::TransposeAntiDiagonal() {

    std::uint8_t aHold;
    aHold = mDataBase[ 0]; mDataBase[ 0] = mDataBase[63]; mDataBase[63] = aHold;
    aHold = mDataBase[ 1]; mDataBase[ 1] = mDataBase[55]; mDataBase[55] = aHold;
    aHold = mDataBase[ 2]; mDataBase[ 2] = mDataBase[47]; mDataBase[47] = aHold;
    aHold = mDataBase[ 3]; mDataBase[ 3] = mDataBase[39]; mDataBase[39] = aHold;
    aHold = mDataBase[ 4]; mDataBase[ 4] = mDataBase[31]; mDataBase[31] = aHold;
    aHold = mDataBase[ 5]; mDataBase[ 5] = mDataBase[23]; mDataBase[23] = aHold;
    aHold = mDataBase[ 6]; mDataBase[ 6] = mDataBase[15]; mDataBase[15] = aHold;
    aHold = mDataBase[ 8]; mDataBase[ 8] = mDataBase[62]; mDataBase[62] = aHold;
    aHold = mDataBase[ 9]; mDataBase[ 9] = mDataBase[54]; mDataBase[54] = aHold;
    aHold = mDataBase[10]; mDataBase[10] = mDataBase[46]; mDataBase[46] = aHold;
    aHold = mDataBase[11]; mDataBase[11] = mDataBase[38]; mDataBase[38] = aHold;
    aHold = mDataBase[12]; mDataBase[12] = mDataBase[30]; mDataBase[30] = aHold;
    aHold = mDataBase[13]; mDataBase[13] = mDataBase[22]; mDataBase[22] = aHold;
    aHold = mDataBase[16]; mDataBase[16] = mDataBase[61]; mDataBase[61] = aHold;
    aHold = mDataBase[17]; mDataBase[17] = mDataBase[53]; mDataBase[53] = aHold;
    aHold = mDataBase[18]; mDataBase[18] = mDataBase[45]; mDataBase[45] = aHold;
    aHold = mDataBase[19]; mDataBase[19] = mDataBase[37]; mDataBase[37] = aHold;
    aHold = mDataBase[20]; mDataBase[20] = mDataBase[29]; mDataBase[29] = aHold;
    aHold = mDataBase[24]; mDataBase[24] = mDataBase[60]; mDataBase[60] = aHold;
    aHold = mDataBase[25]; mDataBase[25] = mDataBase[52]; mDataBase[52] = aHold;
    aHold = mDataBase[26]; mDataBase[26] = mDataBase[44]; mDataBase[44] = aHold;
    aHold = mDataBase[27]; mDataBase[27] = mDataBase[36]; mDataBase[36] = aHold;
    aHold = mDataBase[32]; mDataBase[32] = mDataBase[59]; mDataBase[59] = aHold;
    aHold = mDataBase[33]; mDataBase[33] = mDataBase[51]; mDataBase[51] = aHold;
    aHold = mDataBase[34]; mDataBase[34] = mDataBase[43]; mDataBase[43] = aHold;
    aHold = mDataBase[40]; mDataBase[40] = mDataBase[58]; mDataBase[58] = aHold;
    aHold = mDataBase[41]; mDataBase[41] = mDataBase[50]; mDataBase[50] = aHold;
    aHold = mDataBase[48]; mDataBase[48] = mDataBase[57]; mDataBase[57] = aHold;
}

void FM8::TransposeAntiDiagonalQuarterA() {

    std::uint8_t aHold;

    aHold = mDataBase[ 0]; mDataBase[ 0] = mDataBase[27]; mDataBase[27] = aHold;
    aHold = mDataBase[ 1]; mDataBase[ 1] = mDataBase[19]; mDataBase[19] = aHold;
    aHold = mDataBase[ 2]; mDataBase[ 2] = mDataBase[11]; mDataBase[11] = aHold;

    aHold = mDataBase[ 8]; mDataBase[ 8] = mDataBase[26]; mDataBase[26] = aHold;
    aHold = mDataBase[ 9]; mDataBase[ 9] = mDataBase[18]; mDataBase[18] = aHold;

    aHold = mDataBase[16]; mDataBase[16] = mDataBase[25]; mDataBase[25] = aHold;
}

void FM8::TransposeAntiDiagonalQuarterB() {

    std::uint8_t aHold;

    aHold = mDataBase[ 4]; mDataBase[ 4] = mDataBase[31]; mDataBase[31] = aHold;
    aHold = mDataBase[ 5]; mDataBase[ 5] = mDataBase[23]; mDataBase[23] = aHold;
    aHold = mDataBase[ 6]; mDataBase[ 6] = mDataBase[15]; mDataBase[15] = aHold;

    aHold = mDataBase[12]; mDataBase[12] = mDataBase[30]; mDataBase[30] = aHold;
    aHold = mDataBase[13]; mDataBase[13] = mDataBase[22]; mDataBase[22] = aHold;

    aHold = mDataBase[20]; mDataBase[20] = mDataBase[29]; mDataBase[29] = aHold;
}

void FM8::TransposeAntiDiagonalQuarterC() {

    std::uint8_t aHold;

    aHold = mDataBase[32]; mDataBase[32] = mDataBase[59]; mDataBase[59] = aHold;
    aHold = mDataBase[33]; mDataBase[33] = mDataBase[51]; mDataBase[51] = aHold;
    aHold = mDataBase[34]; mDataBase[34] = mDataBase[43]; mDataBase[43] = aHold;

    aHold = mDataBase[40]; mDataBase[40] = mDataBase[58]; mDataBase[58] = aHold;
    aHold = mDataBase[41]; mDataBase[41] = mDataBase[50]; mDataBase[50] = aHold;

    aHold = mDataBase[48]; mDataBase[48] = mDataBase[57]; mDataBase[57] = aHold;
}

void FM8::TransposeAntiDiagonalQuarterD() {

    std::uint8_t aHold;

    aHold = mDataBase[36]; mDataBase[36] = mDataBase[63]; mDataBase[63] = aHold;
    aHold = mDataBase[37]; mDataBase[37] = mDataBase[55]; mDataBase[55] = aHold;
    aHold = mDataBase[38]; mDataBase[38] = mDataBase[47]; mDataBase[47] = aHold;

    aHold = mDataBase[44]; mDataBase[44] = mDataBase[62]; mDataBase[62] = aHold;
    aHold = mDataBase[45]; mDataBase[45] = mDataBase[54]; mDataBase[54] = aHold;

    aHold = mDataBase[52]; mDataBase[52] = mDataBase[61]; mDataBase[61] = aHold;
}

void FM8::TransposeAntiDiagonalEachQuarter() {
    TransposeAntiDiagonalQuarterA();
    TransposeAntiDiagonalQuarterB();
    TransposeAntiDiagonalQuarterC();
    TransposeAntiDiagonalQuarterD();
}

void FM8::FlipHorizontal() {

    std::uint8_t aHold;

    // row 0
    aHold = mDataBase[ 0]; mDataBase[ 0] = mDataBase[ 7]; mDataBase[ 7] = aHold;
    aHold = mDataBase[ 1]; mDataBase[ 1] = mDataBase[ 6]; mDataBase[ 6] = aHold;
    aHold = mDataBase[ 2]; mDataBase[ 2] = mDataBase[ 5]; mDataBase[ 5] = aHold;
    aHold = mDataBase[ 3]; mDataBase[ 3] = mDataBase[ 4]; mDataBase[ 4] = aHold;

    // row 1
    aHold = mDataBase[ 8]; mDataBase[ 8] = mDataBase[15]; mDataBase[15] = aHold;
    aHold = mDataBase[ 9]; mDataBase[ 9] = mDataBase[14]; mDataBase[14] = aHold;
    aHold = mDataBase[10]; mDataBase[10] = mDataBase[13]; mDataBase[13] = aHold;
    aHold = mDataBase[11]; mDataBase[11] = mDataBase[12]; mDataBase[12] = aHold;

    // row 2
    aHold = mDataBase[16]; mDataBase[16] = mDataBase[23]; mDataBase[23] = aHold;
    aHold = mDataBase[17]; mDataBase[17] = mDataBase[22]; mDataBase[22] = aHold;
    aHold = mDataBase[18]; mDataBase[18] = mDataBase[21]; mDataBase[21] = aHold;
    aHold = mDataBase[19]; mDataBase[19] = mDataBase[20]; mDataBase[20] = aHold;

    // row 3
    aHold = mDataBase[24]; mDataBase[24] = mDataBase[31]; mDataBase[31] = aHold;
    aHold = mDataBase[25]; mDataBase[25] = mDataBase[30]; mDataBase[30] = aHold;
    aHold = mDataBase[26]; mDataBase[26] = mDataBase[29]; mDataBase[29] = aHold;
    aHold = mDataBase[27]; mDataBase[27] = mDataBase[28]; mDataBase[28] = aHold;

    // row 4
    aHold = mDataBase[32]; mDataBase[32] = mDataBase[39]; mDataBase[39] = aHold;
    aHold = mDataBase[33]; mDataBase[33] = mDataBase[38]; mDataBase[38] = aHold;
    aHold = mDataBase[34]; mDataBase[34] = mDataBase[37]; mDataBase[37] = aHold;
    aHold = mDataBase[35]; mDataBase[35] = mDataBase[36]; mDataBase[36] = aHold;

    // row 5
    aHold = mDataBase[40]; mDataBase[40] = mDataBase[47]; mDataBase[47] = aHold;
    aHold = mDataBase[41]; mDataBase[41] = mDataBase[46]; mDataBase[46] = aHold;
    aHold = mDataBase[42]; mDataBase[42] = mDataBase[45]; mDataBase[45] = aHold;
    aHold = mDataBase[43]; mDataBase[43] = mDataBase[44]; mDataBase[44] = aHold;

    // row 6
    aHold = mDataBase[48]; mDataBase[48] = mDataBase[55]; mDataBase[55] = aHold;
    aHold = mDataBase[49]; mDataBase[49] = mDataBase[54]; mDataBase[54] = aHold;
    aHold = mDataBase[50]; mDataBase[50] = mDataBase[53]; mDataBase[53] = aHold;
    aHold = mDataBase[51]; mDataBase[51] = mDataBase[52]; mDataBase[52] = aHold;

    // row 7
    aHold = mDataBase[56]; mDataBase[56] = mDataBase[63]; mDataBase[63] = aHold;
    aHold = mDataBase[57]; mDataBase[57] = mDataBase[62]; mDataBase[62] = aHold;
    aHold = mDataBase[58]; mDataBase[58] = mDataBase[61]; mDataBase[61] = aHold;
    aHold = mDataBase[59]; mDataBase[59] = mDataBase[60]; mDataBase[60] = aHold;
}

void FM8::FlipVertical() {

    std::uint8_t aHold;

    // row 0 ↔ row 7
    aHold = mDataBase[ 0]; mDataBase[ 0] = mDataBase[56]; mDataBase[56] = aHold;
    aHold = mDataBase[ 1]; mDataBase[ 1] = mDataBase[57]; mDataBase[57] = aHold;
    aHold = mDataBase[ 2]; mDataBase[ 2] = mDataBase[58]; mDataBase[58] = aHold;
    aHold = mDataBase[ 3]; mDataBase[ 3] = mDataBase[59]; mDataBase[59] = aHold;
    aHold = mDataBase[ 4]; mDataBase[ 4] = mDataBase[60]; mDataBase[60] = aHold;
    aHold = mDataBase[ 5]; mDataBase[ 5] = mDataBase[61]; mDataBase[61] = aHold;
    aHold = mDataBase[ 6]; mDataBase[ 6] = mDataBase[62]; mDataBase[62] = aHold;
    aHold = mDataBase[ 7]; mDataBase[ 7] = mDataBase[63]; mDataBase[63] = aHold;

    // row 1 ↔ row 6
    aHold = mDataBase[ 8]; mDataBase[ 8] = mDataBase[48]; mDataBase[48] = aHold;
    aHold = mDataBase[ 9]; mDataBase[ 9] = mDataBase[49]; mDataBase[49] = aHold;
    aHold = mDataBase[10]; mDataBase[10] = mDataBase[50]; mDataBase[50] = aHold;
    aHold = mDataBase[11]; mDataBase[11] = mDataBase[51]; mDataBase[51] = aHold;
    aHold = mDataBase[12]; mDataBase[12] = mDataBase[52]; mDataBase[52] = aHold;
    aHold = mDataBase[13]; mDataBase[13] = mDataBase[53]; mDataBase[53] = aHold;
    aHold = mDataBase[14]; mDataBase[14] = mDataBase[54]; mDataBase[54] = aHold;
    aHold = mDataBase[15]; mDataBase[15] = mDataBase[55]; mDataBase[55] = aHold;

    // row 2 ↔ row 5
    aHold = mDataBase[16]; mDataBase[16] = mDataBase[40]; mDataBase[40] = aHold;
    aHold = mDataBase[17]; mDataBase[17] = mDataBase[41]; mDataBase[41] = aHold;
    aHold = mDataBase[18]; mDataBase[18] = mDataBase[42]; mDataBase[42] = aHold;
    aHold = mDataBase[19]; mDataBase[19] = mDataBase[43]; mDataBase[43] = aHold;
    aHold = mDataBase[20]; mDataBase[20] = mDataBase[44]; mDataBase[44] = aHold;
    aHold = mDataBase[21]; mDataBase[21] = mDataBase[45]; mDataBase[45] = aHold;
    aHold = mDataBase[22]; mDataBase[22] = mDataBase[46]; mDataBase[46] = aHold;
    aHold = mDataBase[23]; mDataBase[23] = mDataBase[47]; mDataBase[47] = aHold;

    // row 3 ↔ row 4
    aHold = mDataBase[24]; mDataBase[24] = mDataBase[32]; mDataBase[32] = aHold;
    aHold = mDataBase[25]; mDataBase[25] = mDataBase[33]; mDataBase[33] = aHold;
    aHold = mDataBase[26]; mDataBase[26] = mDataBase[34]; mDataBase[34] = aHold;
    aHold = mDataBase[27]; mDataBase[27] = mDataBase[35]; mDataBase[35] = aHold;
    aHold = mDataBase[28]; mDataBase[28] = mDataBase[36]; mDataBase[36] = aHold;
    aHold = mDataBase[29]; mDataBase[29] = mDataBase[37]; mDataBase[37] = aHold;
    aHold = mDataBase[30]; mDataBase[30] = mDataBase[38]; mDataBase[38] = aHold;
    aHold = mDataBase[31]; mDataBase[31] = mDataBase[39]; mDataBase[39] = aHold;
}

void FM8::FlipHorizontalQuarterA() {

    std::uint8_t aHold;

    aHold = mDataBase[ 0]; mDataBase[ 0] = mDataBase[ 3]; mDataBase[ 3] = aHold;
    aHold = mDataBase[ 1]; mDataBase[ 1] = mDataBase[ 2]; mDataBase[ 2] = aHold;

    aHold = mDataBase[ 8]; mDataBase[ 8] = mDataBase[11]; mDataBase[11] = aHold;
    aHold = mDataBase[ 9]; mDataBase[ 9] = mDataBase[10]; mDataBase[10] = aHold;

    aHold = mDataBase[16]; mDataBase[16] = mDataBase[19]; mDataBase[19] = aHold;
    aHold = mDataBase[17]; mDataBase[17] = mDataBase[18]; mDataBase[18] = aHold;

    aHold = mDataBase[24]; mDataBase[24] = mDataBase[27]; mDataBase[27] = aHold;
    aHold = mDataBase[25]; mDataBase[25] = mDataBase[26]; mDataBase[26] = aHold;
}

void FM8::FlipHorizontalQuarterB() {

    std::uint8_t aHold;

    aHold = mDataBase[ 4]; mDataBase[ 4] = mDataBase[ 7]; mDataBase[ 7] = aHold;
    aHold = mDataBase[ 5]; mDataBase[ 5] = mDataBase[ 6]; mDataBase[ 6] = aHold;

    aHold = mDataBase[12]; mDataBase[12] = mDataBase[15]; mDataBase[15] = aHold;
    aHold = mDataBase[13]; mDataBase[13] = mDataBase[14]; mDataBase[14] = aHold;

    aHold = mDataBase[20]; mDataBase[20] = mDataBase[23]; mDataBase[23] = aHold;
    aHold = mDataBase[21]; mDataBase[21] = mDataBase[22]; mDataBase[22] = aHold;

    aHold = mDataBase[28]; mDataBase[28] = mDataBase[31]; mDataBase[31] = aHold;
    aHold = mDataBase[29]; mDataBase[29] = mDataBase[30]; mDataBase[30] = aHold;
}

void FM8::FlipHorizontalQuarterC() {

    std::uint8_t aHold;

    aHold = mDataBase[32]; mDataBase[32] = mDataBase[35]; mDataBase[35] = aHold;
    aHold = mDataBase[33]; mDataBase[33] = mDataBase[34]; mDataBase[34] = aHold;

    aHold = mDataBase[40]; mDataBase[40] = mDataBase[43]; mDataBase[43] = aHold;
    aHold = mDataBase[41]; mDataBase[41] = mDataBase[42]; mDataBase[42] = aHold;

    aHold = mDataBase[48]; mDataBase[48] = mDataBase[51]; mDataBase[51] = aHold;
    aHold = mDataBase[49]; mDataBase[49] = mDataBase[50]; mDataBase[50] = aHold;

    aHold = mDataBase[56]; mDataBase[56] = mDataBase[59]; mDataBase[59] = aHold;
    aHold = mDataBase[57]; mDataBase[57] = mDataBase[58]; mDataBase[58] = aHold;
}

void FM8::FlipHorizontalQuarterD() {

    std::uint8_t aHold;

    aHold = mDataBase[36]; mDataBase[36] = mDataBase[39]; mDataBase[39] = aHold;
    aHold = mDataBase[37]; mDataBase[37] = mDataBase[38]; mDataBase[38] = aHold;

    aHold = mDataBase[44]; mDataBase[44] = mDataBase[47]; mDataBase[47] = aHold;
    aHold = mDataBase[45]; mDataBase[45] = mDataBase[46]; mDataBase[46] = aHold;

    aHold = mDataBase[52]; mDataBase[52] = mDataBase[55]; mDataBase[55] = aHold;
    aHold = mDataBase[53]; mDataBase[53] = mDataBase[54]; mDataBase[54] = aHold;

    aHold = mDataBase[60]; mDataBase[60] = mDataBase[63]; mDataBase[63] = aHold;
    aHold = mDataBase[61]; mDataBase[61] = mDataBase[62]; mDataBase[62] = aHold;
}

void FM8::FlipHorizontalEachQuarter() {
    FlipHorizontalQuarterA();
    FlipHorizontalQuarterB();
    FlipHorizontalQuarterC();
    FlipHorizontalQuarterD();
}

void FM8::FlipVerticalQuarterA() {

    std::uint8_t aHold;

    aHold = mDataBase[ 0]; mDataBase[ 0] = mDataBase[24]; mDataBase[24] = aHold;
    aHold = mDataBase[ 1]; mDataBase[ 1] = mDataBase[25]; mDataBase[25] = aHold;
    aHold = mDataBase[ 2]; mDataBase[ 2] = mDataBase[26]; mDataBase[26] = aHold;
    aHold = mDataBase[ 3]; mDataBase[ 3] = mDataBase[27]; mDataBase[27] = aHold;

    aHold = mDataBase[ 8]; mDataBase[ 8] = mDataBase[16]; mDataBase[16] = aHold;
    aHold = mDataBase[ 9]; mDataBase[ 9] = mDataBase[17]; mDataBase[17] = aHold;
    aHold = mDataBase[10]; mDataBase[10] = mDataBase[18]; mDataBase[18] = aHold;
    aHold = mDataBase[11]; mDataBase[11] = mDataBase[19]; mDataBase[19] = aHold;
}

void FM8::FlipVerticalQuarterB() {

    std::uint8_t aHold;

    aHold = mDataBase[ 4]; mDataBase[ 4] = mDataBase[28]; mDataBase[28] = aHold;
    aHold = mDataBase[ 5]; mDataBase[ 5] = mDataBase[29]; mDataBase[29] = aHold;
    aHold = mDataBase[ 6]; mDataBase[ 6] = mDataBase[30]; mDataBase[30] = aHold;
    aHold = mDataBase[ 7]; mDataBase[ 7] = mDataBase[31]; mDataBase[31] = aHold;

    aHold = mDataBase[12]; mDataBase[12] = mDataBase[20]; mDataBase[20] = aHold;
    aHold = mDataBase[13]; mDataBase[13] = mDataBase[21]; mDataBase[21] = aHold;
    aHold = mDataBase[14]; mDataBase[14] = mDataBase[22]; mDataBase[22] = aHold;
    aHold = mDataBase[15]; mDataBase[15] = mDataBase[23]; mDataBase[23] = aHold;
}

void FM8::FlipVerticalQuarterC() {

    std::uint8_t aHold;

    aHold = mDataBase[32]; mDataBase[32] = mDataBase[56]; mDataBase[56] = aHold;
    aHold = mDataBase[33]; mDataBase[33] = mDataBase[57]; mDataBase[57] = aHold;
    aHold = mDataBase[34]; mDataBase[34] = mDataBase[58]; mDataBase[58] = aHold;
    aHold = mDataBase[35]; mDataBase[35] = mDataBase[59]; mDataBase[59] = aHold;

    aHold = mDataBase[40]; mDataBase[40] = mDataBase[48]; mDataBase[48] = aHold;
    aHold = mDataBase[41]; mDataBase[41] = mDataBase[49]; mDataBase[49] = aHold;
    aHold = mDataBase[42]; mDataBase[42] = mDataBase[50]; mDataBase[50] = aHold;
    aHold = mDataBase[43]; mDataBase[43] = mDataBase[51]; mDataBase[51] = aHold;
}

void FM8::FlipVerticalQuarterD() {

    std::uint8_t aHold;

    aHold = mDataBase[36]; mDataBase[36] = mDataBase[60]; mDataBase[60] = aHold;
    aHold = mDataBase[37]; mDataBase[37] = mDataBase[61]; mDataBase[61] = aHold;
    aHold = mDataBase[38]; mDataBase[38] = mDataBase[62]; mDataBase[62] = aHold;
    aHold = mDataBase[39]; mDataBase[39] = mDataBase[63]; mDataBase[63] = aHold;

    aHold = mDataBase[44]; mDataBase[44] = mDataBase[52]; mDataBase[52] = aHold;
    aHold = mDataBase[45]; mDataBase[45] = mDataBase[53]; mDataBase[53] = aHold;
    aHold = mDataBase[46]; mDataBase[46] = mDataBase[54]; mDataBase[54] = aHold;
    aHold = mDataBase[47]; mDataBase[47] = mDataBase[55]; mDataBase[55] = aHold;
}

void FM8::FlipVerticalEachQuarter() {

    FlipVerticalQuarterA();
    FlipVerticalQuarterB();
    FlipVerticalQuarterC();
    FlipVerticalQuarterD();
    
}

void FM8::FlipHorizontalBlocks() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    memcpy(aTmp, aSrc, 64);

    // A <- B
    aSrc[ 0] = aTmp[ 4]; aSrc[ 1] = aTmp[ 5]; aSrc[ 2] = aTmp[ 6]; aSrc[ 3] = aTmp[ 7];
    aSrc[ 8] = aTmp[12]; aSrc[ 9] = aTmp[13]; aSrc[10] = aTmp[14]; aSrc[11] = aTmp[15];
    aSrc[16] = aTmp[20]; aSrc[17] = aTmp[21]; aSrc[18] = aTmp[22]; aSrc[19] = aTmp[23];
    aSrc[24] = aTmp[28]; aSrc[25] = aTmp[29]; aSrc[26] = aTmp[30]; aSrc[27] = aTmp[31];

    // B <- A
    aSrc[ 4] = aTmp[ 0]; aSrc[ 5] = aTmp[ 1]; aSrc[ 6] = aTmp[ 2]; aSrc[ 7] = aTmp[ 3];
    aSrc[12] = aTmp[ 8]; aSrc[13] = aTmp[ 9]; aSrc[14] = aTmp[10]; aSrc[15] = aTmp[11];
    aSrc[20] = aTmp[16]; aSrc[21] = aTmp[17]; aSrc[22] = aTmp[18]; aSrc[23] = aTmp[19];
    aSrc[28] = aTmp[24]; aSrc[29] = aTmp[25]; aSrc[30] = aTmp[26]; aSrc[31] = aTmp[27];

    // C <- D
    aSrc[32] = aTmp[36]; aSrc[33] = aTmp[37]; aSrc[34] = aTmp[38]; aSrc[35] = aTmp[39];
    aSrc[40] = aTmp[44]; aSrc[41] = aTmp[45]; aSrc[42] = aTmp[46]; aSrc[43] = aTmp[47];
    aSrc[48] = aTmp[52]; aSrc[49] = aTmp[53]; aSrc[50] = aTmp[54]; aSrc[51] = aTmp[55];
    aSrc[56] = aTmp[60]; aSrc[57] = aTmp[61]; aSrc[58] = aTmp[62]; aSrc[59] = aTmp[63];

    // D <- C
    aSrc[36] = aTmp[32]; aSrc[37] = aTmp[33]; aSrc[38] = aTmp[34]; aSrc[39] = aTmp[35];
    aSrc[44] = aTmp[40]; aSrc[45] = aTmp[41]; aSrc[46] = aTmp[42]; aSrc[47] = aTmp[43];
    aSrc[52] = aTmp[48]; aSrc[53] = aTmp[49]; aSrc[54] = aTmp[50]; aSrc[55] = aTmp[51];
    aSrc[60] = aTmp[56]; aSrc[61] = aTmp[57]; aSrc[62] = aTmp[58]; aSrc[63] = aTmp[59];
}

void FM8::FlipVerticalBlocks() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    memcpy(aTmp, aSrc, 64);

    // A <- C
    aSrc[ 0] = aTmp[32]; aSrc[ 1] = aTmp[33]; aSrc[ 2] = aTmp[34]; aSrc[ 3] = aTmp[35];
    aSrc[ 8] = aTmp[40]; aSrc[ 9] = aTmp[41]; aSrc[10] = aTmp[42]; aSrc[11] = aTmp[43];
    aSrc[16] = aTmp[48]; aSrc[17] = aTmp[49]; aSrc[18] = aTmp[50]; aSrc[19] = aTmp[51];
    aSrc[24] = aTmp[56]; aSrc[25] = aTmp[57]; aSrc[26] = aTmp[58]; aSrc[27] = aTmp[59];

    // B <- D
    aSrc[ 4] = aTmp[36]; aSrc[ 5] = aTmp[37]; aSrc[ 6] = aTmp[38]; aSrc[ 7] = aTmp[39];
    aSrc[12] = aTmp[44]; aSrc[13] = aTmp[45]; aSrc[14] = aTmp[46]; aSrc[15] = aTmp[47];
    aSrc[20] = aTmp[52]; aSrc[21] = aTmp[53]; aSrc[22] = aTmp[54]; aSrc[23] = aTmp[55];
    aSrc[28] = aTmp[60]; aSrc[29] = aTmp[61]; aSrc[30] = aTmp[62]; aSrc[31] = aTmp[63];

    // C <- A
    aSrc[32] = aTmp[ 0]; aSrc[33] = aTmp[ 1]; aSrc[34] = aTmp[ 2]; aSrc[35] = aTmp[ 3];
    aSrc[40] = aTmp[ 8]; aSrc[41] = aTmp[ 9]; aSrc[42] = aTmp[10]; aSrc[43] = aTmp[11];
    aSrc[48] = aTmp[16]; aSrc[49] = aTmp[17]; aSrc[50] = aTmp[18]; aSrc[51] = aTmp[19];
    aSrc[56] = aTmp[24]; aSrc[57] = aTmp[25]; aSrc[58] = aTmp[26]; aSrc[59] = aTmp[27];

    // D <- B
    aSrc[36] = aTmp[ 4]; aSrc[37] = aTmp[ 5]; aSrc[38] = aTmp[ 6]; aSrc[39] = aTmp[ 7];
    aSrc[44] = aTmp[12]; aSrc[45] = aTmp[13]; aSrc[46] = aTmp[14]; aSrc[47] = aTmp[15];
    aSrc[52] = aTmp[20]; aSrc[53] = aTmp[21]; aSrc[54] = aTmp[22]; aSrc[55] = aTmp[23];
    aSrc[60] = aTmp[28]; aSrc[61] = aTmp[29]; aSrc[62] = aTmp[30]; aSrc[63] = aTmp[31];
}

void FM8::TransposeMainDiagonalBlocks() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    memcpy(aTmp, aSrc, 64);

    // B <- C
    aSrc[ 4] = aTmp[32]; aSrc[ 5] = aTmp[33]; aSrc[ 6] = aTmp[34]; aSrc[ 7] = aTmp[35];
    aSrc[12] = aTmp[40]; aSrc[13] = aTmp[41]; aSrc[14] = aTmp[42]; aSrc[15] = aTmp[43];
    aSrc[20] = aTmp[48]; aSrc[21] = aTmp[49]; aSrc[22] = aTmp[50]; aSrc[23] = aTmp[51];
    aSrc[28] = aTmp[56]; aSrc[29] = aTmp[57]; aSrc[30] = aTmp[58]; aSrc[31] = aTmp[59];

    // C <- B
    aSrc[32] = aTmp[ 4]; aSrc[33] = aTmp[ 5]; aSrc[34] = aTmp[ 6]; aSrc[35] = aTmp[ 7];
    aSrc[40] = aTmp[12]; aSrc[41] = aTmp[13]; aSrc[42] = aTmp[14]; aSrc[43] = aTmp[15];
    aSrc[48] = aTmp[20]; aSrc[49] = aTmp[21]; aSrc[50] = aTmp[22]; aSrc[51] = aTmp[23];
    aSrc[56] = aTmp[28]; aSrc[57] = aTmp[29]; aSrc[58] = aTmp[30]; aSrc[59] = aTmp[31];

    // A and D unchanged
}

void FM8::TransposeAntiDiagonalBlocks() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    memcpy(aTmp, aSrc, 64);

    // A <- D
    aSrc[ 0] = aTmp[36]; aSrc[ 1] = aTmp[37]; aSrc[ 2] = aTmp[38]; aSrc[ 3] = aTmp[39];
    aSrc[ 8] = aTmp[44]; aSrc[ 9] = aTmp[45]; aSrc[10] = aTmp[46]; aSrc[11] = aTmp[47];
    aSrc[16] = aTmp[52]; aSrc[17] = aTmp[53]; aSrc[18] = aTmp[54]; aSrc[19] = aTmp[55];
    aSrc[24] = aTmp[60]; aSrc[25] = aTmp[61]; aSrc[26] = aTmp[62]; aSrc[27] = aTmp[63];

    // D <- A
    aSrc[36] = aTmp[ 0]; aSrc[37] = aTmp[ 1]; aSrc[38] = aTmp[ 2]; aSrc[39] = aTmp[ 3];
    aSrc[44] = aTmp[ 8]; aSrc[45] = aTmp[ 9]; aSrc[46] = aTmp[10]; aSrc[47] = aTmp[11];
    aSrc[52] = aTmp[16]; aSrc[53] = aTmp[17]; aSrc[54] = aTmp[18]; aSrc[55] = aTmp[19];
    aSrc[60] = aTmp[24]; aSrc[61] = aTmp[25]; aSrc[62] = aTmp[26]; aSrc[63] = aTmp[27];

    // B and C unchanged
}

void FM8::RotateRightEachSixteenthQuarterA() {

    std::uint8_t aHold;

    // --- block (0,0) ---
    aHold = mDataBase[ 0];
    mDataBase[ 0] = mDataBase[ 8];
    mDataBase[ 8] = mDataBase[ 9];
    mDataBase[ 9] = mDataBase[ 1];
    mDataBase[ 1] = aHold;

    // --- block (0,2) ---
    aHold = mDataBase[ 2];
    mDataBase[ 2] = mDataBase[10];
    mDataBase[10] = mDataBase[11];
    mDataBase[11] = mDataBase[ 3];
    mDataBase[ 3] = aHold;

    // --- block (2,0) ---
    aHold = mDataBase[16];
    mDataBase[16] = mDataBase[24];
    mDataBase[24] = mDataBase[25];
    mDataBase[25] = mDataBase[17];
    mDataBase[17] = aHold;

    // --- block (2,2) ---
    aHold = mDataBase[18];
    mDataBase[18] = mDataBase[26];
    mDataBase[26] = mDataBase[27];
    mDataBase[27] = mDataBase[19];
    mDataBase[19] = aHold;
}

void FM8::RotateRightEachSixteenthQuarterB() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[ 4];
    mDataBase[ 4] = mDataBase[12];
    mDataBase[12] = mDataBase[13];
    mDataBase[13] = mDataBase[ 5];
    mDataBase[ 5] = aHold;

    // (0,2)
    aHold = mDataBase[ 6];
    mDataBase[ 6] = mDataBase[14];
    mDataBase[14] = mDataBase[15];
    mDataBase[15] = mDataBase[ 7];
    mDataBase[ 7] = aHold;

    // (2,0)
    aHold = mDataBase[20];
    mDataBase[20] = mDataBase[28];
    mDataBase[28] = mDataBase[29];
    mDataBase[29] = mDataBase[21];
    mDataBase[21] = aHold;

    // (2,2)
    aHold = mDataBase[22];
    mDataBase[22] = mDataBase[30];
    mDataBase[30] = mDataBase[31];
    mDataBase[31] = mDataBase[23];
    mDataBase[23] = aHold;
}

void FM8::RotateRightEachSixteenthQuarterC() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[32];
    mDataBase[32] = mDataBase[40];
    mDataBase[40] = mDataBase[41];
    mDataBase[41] = mDataBase[33];
    mDataBase[33] = aHold;

    // (0,2)
    aHold = mDataBase[34];
    mDataBase[34] = mDataBase[42];
    mDataBase[42] = mDataBase[43];
    mDataBase[43] = mDataBase[35];
    mDataBase[35] = aHold;

    // (2,0)
    aHold = mDataBase[48];
    mDataBase[48] = mDataBase[56];
    mDataBase[56] = mDataBase[57];
    mDataBase[57] = mDataBase[49];
    mDataBase[49] = aHold;

    // (2,2)
    aHold = mDataBase[50];
    mDataBase[50] = mDataBase[58];
    mDataBase[58] = mDataBase[59];
    mDataBase[59] = mDataBase[51];
    mDataBase[51] = aHold;
}

void FM8::RotateRightEachSixteenthQuarterD() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[36];
    mDataBase[36] = mDataBase[44];
    mDataBase[44] = mDataBase[45];
    mDataBase[45] = mDataBase[37];
    mDataBase[37] = aHold;

    // (0,2)
    aHold = mDataBase[38];
    mDataBase[38] = mDataBase[46];
    mDataBase[46] = mDataBase[47];
    mDataBase[47] = mDataBase[39];
    mDataBase[39] = aHold;

    // (2,0)
    aHold = mDataBase[52];
    mDataBase[52] = mDataBase[60];
    mDataBase[60] = mDataBase[61];
    mDataBase[61] = mDataBase[53];
    mDataBase[53] = aHold;

    // (2,2)
    aHold = mDataBase[54];
    mDataBase[54] = mDataBase[62];
    mDataBase[62] = mDataBase[63];
    mDataBase[63] = mDataBase[55];
    mDataBase[55] = aHold;
}

void FM8::RotateRightEachSixteenthEachQuarter() {
    RotateRightEachSixteenthQuarterA();
    RotateRightEachSixteenthQuarterB();
    RotateRightEachSixteenthQuarterC();
    RotateRightEachSixteenthQuarterD();
}

void FM8::RotateLeftEachSixteenthQuarterA() {

    std::uint8_t aHold;

    aHold = mDataBase[0];
    mDataBase[0] = mDataBase[1];
    mDataBase[1] = mDataBase[9];
    mDataBase[9] = mDataBase[8];
    mDataBase[8] = aHold;

    aHold = mDataBase[2];
    mDataBase[2] = mDataBase[3];
    mDataBase[3] = mDataBase[11];
    mDataBase[11] = mDataBase[10];
    mDataBase[10] = aHold;

    aHold = mDataBase[16];
    mDataBase[16] = mDataBase[17];
    mDataBase[17] = mDataBase[25];
    mDataBase[25] = mDataBase[24];
    mDataBase[24] = aHold;

    aHold = mDataBase[18];
    mDataBase[18] = mDataBase[19];
    mDataBase[19] = mDataBase[27];
    mDataBase[27] = mDataBase[26];
    mDataBase[26] = aHold;
}

void FM8::RotateLeftEachSixteenthQuarterB() {

    std::uint8_t aHold;

    aHold = mDataBase[4];
    mDataBase[4] = mDataBase[5];
    mDataBase[5] = mDataBase[13];
    mDataBase[13] = mDataBase[12];
    mDataBase[12] = aHold;

    aHold = mDataBase[6];
    mDataBase[6] = mDataBase[7];
    mDataBase[7] = mDataBase[15];
    mDataBase[15] = mDataBase[14];
    mDataBase[14] = aHold;

    aHold = mDataBase[20];
    mDataBase[20] = mDataBase[21];
    mDataBase[21] = mDataBase[29];
    mDataBase[29] = mDataBase[28];
    mDataBase[28] = aHold;

    aHold = mDataBase[22];
    mDataBase[22] = mDataBase[23];
    mDataBase[23] = mDataBase[31];
    mDataBase[31] = mDataBase[30];
    mDataBase[30] = aHold;
}

void FM8::RotateLeftEachSixteenthQuarterC() {

    std::uint8_t aHold;

    aHold = mDataBase[32];
    mDataBase[32] = mDataBase[33];
    mDataBase[33] = mDataBase[41];
    mDataBase[41] = mDataBase[40];
    mDataBase[40] = aHold;

    aHold = mDataBase[34];
    mDataBase[34] = mDataBase[35];
    mDataBase[35] = mDataBase[43];
    mDataBase[43] = mDataBase[42];
    mDataBase[42] = aHold;

    aHold = mDataBase[48];
    mDataBase[48] = mDataBase[49];
    mDataBase[49] = mDataBase[57];
    mDataBase[57] = mDataBase[56];
    mDataBase[56] = aHold;

    aHold = mDataBase[50];
    mDataBase[50] = mDataBase[51];
    mDataBase[51] = mDataBase[59];
    mDataBase[59] = mDataBase[58];
    mDataBase[58] = aHold;
}

void FM8::RotateLeftEachSixteenthQuarterD() {

    std::uint8_t aHold;

    aHold = mDataBase[36];
    mDataBase[36] = mDataBase[37];
    mDataBase[37] = mDataBase[45];
    mDataBase[45] = mDataBase[44];
    mDataBase[44] = aHold;

    aHold = mDataBase[38];
    mDataBase[38] = mDataBase[39];
    mDataBase[39] = mDataBase[47];
    mDataBase[47] = mDataBase[46];
    mDataBase[46] = aHold;

    aHold = mDataBase[52];
    mDataBase[52] = mDataBase[53];
    mDataBase[53] = mDataBase[61];
    mDataBase[61] = mDataBase[60];
    mDataBase[60] = aHold;

    aHold = mDataBase[54];
    mDataBase[54] = mDataBase[55];
    mDataBase[55] = mDataBase[63];
    mDataBase[63] = mDataBase[62];
    mDataBase[62] = aHold;
}

void FM8::RotateLeftEachSixteenthEachQuarter() {
    RotateLeftEachSixteenthQuarterA();
    RotateLeftEachSixteenthQuarterB();
    RotateLeftEachSixteenthQuarterC();
    RotateLeftEachSixteenthQuarterD();
}


void FM8::FlipHorizontalEachSixteenthQuarterA() {

    std::uint8_t aHold;

    // block (0,0)
    aHold = mDataBase[0];  mDataBase[0] = mDataBase[1];  mDataBase[1] = aHold;
    aHold = mDataBase[8];  mDataBase[8] = mDataBase[9];  mDataBase[9] = aHold;

    // block (0,2)
    aHold = mDataBase[2];  mDataBase[2] = mDataBase[3];  mDataBase[3] = aHold;
    aHold = mDataBase[10]; mDataBase[10] = mDataBase[11]; mDataBase[11] = aHold;

    // block (2,0)
    aHold = mDataBase[16]; mDataBase[16] = mDataBase[17]; mDataBase[17] = aHold;
    aHold = mDataBase[24]; mDataBase[24] = mDataBase[25]; mDataBase[25] = aHold;

    // block (2,2)
    aHold = mDataBase[18]; mDataBase[18] = mDataBase[19]; mDataBase[19] = aHold;
    aHold = mDataBase[26]; mDataBase[26] = mDataBase[27]; mDataBase[27] = aHold;
}

void FM8::FlipHorizontalEachSixteenthQuarterB() {

    std::uint8_t aHold;

    // block (0,0)
    aHold = mDataBase[4];  mDataBase[4] = mDataBase[5];  mDataBase[5] = aHold;
    aHold = mDataBase[12]; mDataBase[12] = mDataBase[13]; mDataBase[13] = aHold;

    // block (0,2)
    aHold = mDataBase[6];  mDataBase[6] = mDataBase[7];  mDataBase[7] = aHold;
    aHold = mDataBase[14]; mDataBase[14] = mDataBase[15]; mDataBase[15] = aHold;

    // block (2,0)
    aHold = mDataBase[20]; mDataBase[20] = mDataBase[21]; mDataBase[21] = aHold;
    aHold = mDataBase[28]; mDataBase[28] = mDataBase[29]; mDataBase[29] = aHold;

    // block (2,2)
    aHold = mDataBase[22]; mDataBase[22] = mDataBase[23]; mDataBase[23] = aHold;
    aHold = mDataBase[30]; mDataBase[30] = mDataBase[31]; mDataBase[31] = aHold;
}

void FM8::FlipHorizontalEachSixteenthQuarterC() {

    std::uint8_t aHold;

    // block (0,0)
    aHold = mDataBase[32]; mDataBase[32] = mDataBase[33]; mDataBase[33] = aHold;
    aHold = mDataBase[40]; mDataBase[40] = mDataBase[41]; mDataBase[41] = aHold;

    // block (0,2)
    aHold = mDataBase[34]; mDataBase[34] = mDataBase[35]; mDataBase[35] = aHold;
    aHold = mDataBase[42]; mDataBase[42] = mDataBase[43]; mDataBase[43] = aHold;

    // block (2,0)
    aHold = mDataBase[48]; mDataBase[48] = mDataBase[49]; mDataBase[49] = aHold;
    aHold = mDataBase[56]; mDataBase[56] = mDataBase[57]; mDataBase[57] = aHold;

    // block (2,2)
    aHold = mDataBase[50]; mDataBase[50] = mDataBase[51]; mDataBase[51] = aHold;
    aHold = mDataBase[58]; mDataBase[58] = mDataBase[59]; mDataBase[59] = aHold;
}

void FM8::FlipHorizontalEachSixteenthQuarterD() {

    std::uint8_t aHold;

    // block (0,0)
    aHold = mDataBase[36]; mDataBase[36] = mDataBase[37]; mDataBase[37] = aHold;
    aHold = mDataBase[44]; mDataBase[44] = mDataBase[45]; mDataBase[45] = aHold;

    // block (0,2)
    aHold = mDataBase[38]; mDataBase[38] = mDataBase[39]; mDataBase[39] = aHold;
    aHold = mDataBase[46]; mDataBase[46] = mDataBase[47]; mDataBase[47] = aHold;

    // block (2,0)
    aHold = mDataBase[52]; mDataBase[52] = mDataBase[53]; mDataBase[53] = aHold;
    aHold = mDataBase[60]; mDataBase[60] = mDataBase[61]; mDataBase[61] = aHold;

    // block (2,2)
    aHold = mDataBase[54]; mDataBase[54] = mDataBase[55]; mDataBase[55] = aHold;
    aHold = mDataBase[62]; mDataBase[62] = mDataBase[63]; mDataBase[63] = aHold;
}

void FM8::FlipHorizontalEachSixteenthEachQuarter() {

    FlipHorizontalEachSixteenthQuarterA();
    FlipHorizontalEachSixteenthQuarterB();
    FlipHorizontalEachSixteenthQuarterC();
    FlipHorizontalEachSixteenthQuarterD();
}

void FM8::FlipVerticalEachSixteenthQuarterA() {

    std::uint8_t aHold;

    // block (0,0)
    aHold = mDataBase[0];  mDataBase[0] = mDataBase[8];  mDataBase[8] = aHold;
    aHold = mDataBase[1];  mDataBase[1] = mDataBase[9];  mDataBase[9] = aHold;

    // block (0,2)
    aHold = mDataBase[2];  mDataBase[2] = mDataBase[10]; mDataBase[10] = aHold;
    aHold = mDataBase[3];  mDataBase[3] = mDataBase[11]; mDataBase[11] = aHold;

    // block (2,0)
    aHold = mDataBase[16]; mDataBase[16] = mDataBase[24]; mDataBase[24] = aHold;
    aHold = mDataBase[17]; mDataBase[17] = mDataBase[25]; mDataBase[25] = aHold;

    // block (2,2)
    aHold = mDataBase[18]; mDataBase[18] = mDataBase[26]; mDataBase[26] = aHold;
    aHold = mDataBase[19]; mDataBase[19] = mDataBase[27]; mDataBase[27] = aHold;
}

void FM8::FlipVerticalEachSixteenthQuarterB() {

    std::uint8_t aHold;

    // block (0,0)
    aHold = mDataBase[4];  mDataBase[4] = mDataBase[12]; mDataBase[12] = aHold;
    aHold = mDataBase[5];  mDataBase[5] = mDataBase[13]; mDataBase[13] = aHold;

    // block (0,2)
    aHold = mDataBase[6];  mDataBase[6] = mDataBase[14]; mDataBase[14] = aHold;
    aHold = mDataBase[7];  mDataBase[7] = mDataBase[15]; mDataBase[15] = aHold;

    // block (2,0)
    aHold = mDataBase[20]; mDataBase[20] = mDataBase[28]; mDataBase[28] = aHold;
    aHold = mDataBase[21]; mDataBase[21] = mDataBase[29]; mDataBase[29] = aHold;

    // block (2,2)
    aHold = mDataBase[22]; mDataBase[22] = mDataBase[30]; mDataBase[30] = aHold;
    aHold = mDataBase[23]; mDataBase[23] = mDataBase[31]; mDataBase[31] = aHold;
}

void FM8::FlipVerticalEachSixteenthQuarterC() {

    std::uint8_t aHold;

    // block (0,0)
    aHold = mDataBase[32]; mDataBase[32] = mDataBase[40]; mDataBase[40] = aHold;
    aHold = mDataBase[33]; mDataBase[33] = mDataBase[41]; mDataBase[41] = aHold;

    // block (0,2)
    aHold = mDataBase[34]; mDataBase[34] = mDataBase[42]; mDataBase[42] = aHold;
    aHold = mDataBase[35]; mDataBase[35] = mDataBase[43]; mDataBase[43] = aHold;

    // block (2,0)
    aHold = mDataBase[48]; mDataBase[48] = mDataBase[56]; mDataBase[56] = aHold;
    aHold = mDataBase[49]; mDataBase[49] = mDataBase[57]; mDataBase[57] = aHold;

    // block (2,2)
    aHold = mDataBase[50]; mDataBase[50] = mDataBase[58]; mDataBase[58] = aHold;
    aHold = mDataBase[51]; mDataBase[51] = mDataBase[59]; mDataBase[59] = aHold;
}

void FM8::FlipVerticalEachSixteenthQuarterD() {

    std::uint8_t aHold;

    // block (0,0)
    aHold = mDataBase[36]; mDataBase[36] = mDataBase[44]; mDataBase[44] = aHold;
    aHold = mDataBase[37]; mDataBase[37] = mDataBase[45]; mDataBase[45] = aHold;

    // block (0,2)
    aHold = mDataBase[38]; mDataBase[38] = mDataBase[46]; mDataBase[46] = aHold;
    aHold = mDataBase[39]; mDataBase[39] = mDataBase[47]; mDataBase[47] = aHold;

    // block (2,0)
    aHold = mDataBase[52]; mDataBase[52] = mDataBase[60]; mDataBase[60] = aHold;
    aHold = mDataBase[53]; mDataBase[53] = mDataBase[61]; mDataBase[61] = aHold;

    // block (2,2)
    aHold = mDataBase[54]; mDataBase[54] = mDataBase[62]; mDataBase[62] = aHold;
    aHold = mDataBase[55]; mDataBase[55] = mDataBase[63]; mDataBase[63] = aHold;
}

void FM8::FlipVerticalEachSixteenthEachQuarter() {

    FlipVerticalEachSixteenthQuarterA();
    FlipVerticalEachSixteenthQuarterB();
    FlipVerticalEachSixteenthQuarterC();
    FlipVerticalEachSixteenthQuarterD();
}

void FM8::TransposeMainDiagonalEachSixteenthQuarterA() {

    std::uint8_t aHold;

    aHold = mDataBase[1];  mDataBase[1] = mDataBase[8];  mDataBase[8] = aHold;
    aHold = mDataBase[3];  mDataBase[3] = mDataBase[10]; mDataBase[10] = aHold;
    aHold = mDataBase[17]; mDataBase[17] = mDataBase[24]; mDataBase[24] = aHold;
    aHold = mDataBase[19]; mDataBase[19] = mDataBase[26]; mDataBase[26] = aHold;
}

void FM8::TransposeMainDiagonalEachSixteenthQuarterB() {

    std::uint8_t aHold;

    aHold = mDataBase[5];  mDataBase[5] = mDataBase[12]; mDataBase[12] = aHold;
    aHold = mDataBase[7];  mDataBase[7] = mDataBase[14]; mDataBase[14] = aHold;
    aHold = mDataBase[21]; mDataBase[21] = mDataBase[28]; mDataBase[28] = aHold;
    aHold = mDataBase[23]; mDataBase[23] = mDataBase[30]; mDataBase[30] = aHold;
}

void FM8::TransposeMainDiagonalEachSixteenthQuarterC() {

    std::uint8_t aHold;

    aHold = mDataBase[33]; mDataBase[33] = mDataBase[40]; mDataBase[40] = aHold;
    aHold = mDataBase[35]; mDataBase[35] = mDataBase[42]; mDataBase[42] = aHold;
    aHold = mDataBase[49]; mDataBase[49] = mDataBase[56]; mDataBase[56] = aHold;
    aHold = mDataBase[51]; mDataBase[51] = mDataBase[58]; mDataBase[58] = aHold;
}

void FM8::TransposeMainDiagonalEachSixteenthEachQuarter() {

    TransposeMainDiagonalEachSixteenthQuarterA();
    TransposeMainDiagonalEachSixteenthQuarterB();
    TransposeMainDiagonalEachSixteenthQuarterC();
    TransposeMainDiagonalEachSixteenthQuarterD();
}

void FM8::TransposeMainDiagonalEachSixteenthQuarterD() {

    std::uint8_t aHold;

    aHold = mDataBase[37]; mDataBase[37] = mDataBase[44]; mDataBase[44] = aHold;
    aHold = mDataBase[39]; mDataBase[39] = mDataBase[46]; mDataBase[46] = aHold;
    aHold = mDataBase[53]; mDataBase[53] = mDataBase[60]; mDataBase[60] = aHold;
    aHold = mDataBase[55]; mDataBase[55] = mDataBase[62]; mDataBase[62] = aHold;
}


void FM8::TransposeAntiDiagonalEachSixteenthQuarterA() {

    std::uint8_t aHold;

    aHold = mDataBase[0];  mDataBase[0] = mDataBase[9];  mDataBase[9] = aHold;
    aHold = mDataBase[2];  mDataBase[2] = mDataBase[11]; mDataBase[11] = aHold;
    aHold = mDataBase[16]; mDataBase[16] = mDataBase[25]; mDataBase[25] = aHold;
    aHold = mDataBase[18]; mDataBase[18] = mDataBase[27]; mDataBase[27] = aHold;
}

void FM8::TransposeAntiDiagonalEachSixteenthQuarterB() {

    std::uint8_t aHold;

    aHold = mDataBase[4];  mDataBase[4] = mDataBase[13]; mDataBase[13] = aHold;
    aHold = mDataBase[6];  mDataBase[6] = mDataBase[15]; mDataBase[15] = aHold;
    aHold = mDataBase[20]; mDataBase[20] = mDataBase[29]; mDataBase[29] = aHold;
    aHold = mDataBase[22]; mDataBase[22] = mDataBase[31]; mDataBase[31] = aHold;
}

void FM8::TransposeAntiDiagonalEachSixteenthQuarterC() {

    std::uint8_t aHold;

    aHold = mDataBase[32]; mDataBase[32] = mDataBase[41]; mDataBase[41] = aHold;
    aHold = mDataBase[34]; mDataBase[34] = mDataBase[43]; mDataBase[43] = aHold;
    aHold = mDataBase[48]; mDataBase[48] = mDataBase[57]; mDataBase[57] = aHold;
    aHold = mDataBase[50]; mDataBase[50] = mDataBase[59]; mDataBase[59] = aHold;
}

void FM8::TransposeAntiDiagonalEachSixteenthQuarterD() {

    std::uint8_t aHold;

    aHold = mDataBase[36]; mDataBase[36] = mDataBase[45]; mDataBase[45] = aHold;
    aHold = mDataBase[38]; mDataBase[38] = mDataBase[47]; mDataBase[47] = aHold;
    aHold = mDataBase[52]; mDataBase[52] = mDataBase[61]; mDataBase[61] = aHold;
    aHold = mDataBase[54]; mDataBase[54] = mDataBase[63]; mDataBase[63] = aHold;
}

void FM8::TransposeAntiDiagonalEachSixteenthEachQuarter() {

    TransposeAntiDiagonalEachSixteenthQuarterA();
    TransposeAntiDiagonalEachSixteenthQuarterB();
    TransposeAntiDiagonalEachSixteenthQuarterC();
    TransposeAntiDiagonalEachSixteenthQuarterD();
}

void FM8::RotateRightSixteenthsQuarterA() {

    std::uint8_t aHold;

    // --- position (0,0) within each 2x2 ---
    aHold = mDataBase[0];
    mDataBase[0]  = mDataBase[16];
    mDataBase[16] = mDataBase[18];
    mDataBase[18] = mDataBase[2];
    mDataBase[2]  = aHold;

    // --- position (0,1) ---
    aHold = mDataBase[1];
    mDataBase[1]  = mDataBase[17];
    mDataBase[17] = mDataBase[19];
    mDataBase[19] = mDataBase[3];
    mDataBase[3]  = aHold;

    // --- position (1,0) ---
    aHold = mDataBase[8];
    mDataBase[8]  = mDataBase[24];
    mDataBase[24] = mDataBase[26];
    mDataBase[26] = mDataBase[10];
    mDataBase[10] = aHold;

    // --- position (1,1) ---
    aHold = mDataBase[9];
    mDataBase[9]  = mDataBase[25];
    mDataBase[25] = mDataBase[27];
    mDataBase[27] = mDataBase[11];
    mDataBase[11] = aHold;
}

void FM8::RotateRightSixteenthsQuarterB() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[4];
    mDataBase[4]  = mDataBase[20];
    mDataBase[20] = mDataBase[22];
    mDataBase[22] = mDataBase[6];
    mDataBase[6]  = aHold;

    // (0,1)
    aHold = mDataBase[5];
    mDataBase[5]  = mDataBase[21];
    mDataBase[21] = mDataBase[23];
    mDataBase[23] = mDataBase[7];
    mDataBase[7]  = aHold;

    // (1,0)
    aHold = mDataBase[12];
    mDataBase[12] = mDataBase[28];
    mDataBase[28] = mDataBase[30];
    mDataBase[30] = mDataBase[14];
    mDataBase[14] = aHold;

    // (1,1)
    aHold = mDataBase[13];
    mDataBase[13] = mDataBase[29];
    mDataBase[29] = mDataBase[31];
    mDataBase[31] = mDataBase[15];
    mDataBase[15] = aHold;
}

void FM8::RotateRightSixteenthsQuarterC() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[32];
    mDataBase[32] = mDataBase[48];
    mDataBase[48] = mDataBase[50];
    mDataBase[50] = mDataBase[34];
    mDataBase[34] = aHold;

    // (0,1)
    aHold = mDataBase[33];
    mDataBase[33] = mDataBase[49];
    mDataBase[49] = mDataBase[51];
    mDataBase[51] = mDataBase[35];
    mDataBase[35] = aHold;

    // (1,0)
    aHold = mDataBase[40];
    mDataBase[40] = mDataBase[56];
    mDataBase[56] = mDataBase[58];
    mDataBase[58] = mDataBase[42];
    mDataBase[42] = aHold;

    // (1,1)
    aHold = mDataBase[41];
    mDataBase[41] = mDataBase[57];
    mDataBase[57] = mDataBase[59];
    mDataBase[59] = mDataBase[43];
    mDataBase[43] = aHold;
}

void FM8::RotateRightSixteenthsQuarterD() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[36];
    mDataBase[36] = mDataBase[52];
    mDataBase[52] = mDataBase[54];
    mDataBase[54] = mDataBase[38];
    mDataBase[38] = aHold;

    // (0,1)
    aHold = mDataBase[37];
    mDataBase[37] = mDataBase[53];
    mDataBase[53] = mDataBase[55];
    mDataBase[55] = mDataBase[39];
    mDataBase[39] = aHold;

    // (1,0)
    aHold = mDataBase[44];
    mDataBase[44] = mDataBase[60];
    mDataBase[60] = mDataBase[62];
    mDataBase[62] = mDataBase[46];
    mDataBase[46] = aHold;

    // (1,1)
    aHold = mDataBase[45];
    mDataBase[45] = mDataBase[61];
    mDataBase[61] = mDataBase[63];
    mDataBase[63] = mDataBase[47];
    mDataBase[47] = aHold;
}

void FM8::RotateRightSixteenthsEachQuarter() {
    RotateRightSixteenthsQuarterA();
    RotateRightSixteenthsQuarterB();
    RotateRightSixteenthsQuarterC();
    RotateRightSixteenthsQuarterD();
}

void FM8::RotateLeftSixteenthsQuarterA() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[0];
    mDataBase[0]  = mDataBase[2];
    mDataBase[2]  = mDataBase[18];
    mDataBase[18] = mDataBase[16];
    mDataBase[16] = aHold;

    // (0,1)
    aHold = mDataBase[1];
    mDataBase[1]  = mDataBase[3];
    mDataBase[3]  = mDataBase[19];
    mDataBase[19] = mDataBase[17];
    mDataBase[17] = aHold;

    // (1,0)
    aHold = mDataBase[8];
    mDataBase[8]  = mDataBase[10];
    mDataBase[10] = mDataBase[26];
    mDataBase[26] = mDataBase[24];
    mDataBase[24] = aHold;

    // (1,1)
    aHold = mDataBase[9];
    mDataBase[9]  = mDataBase[11];
    mDataBase[11] = mDataBase[27];
    mDataBase[27] = mDataBase[25];
    mDataBase[25] = aHold;
}

void FM8::RotateLeftSixteenthsQuarterB() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[4];
    mDataBase[4]  = mDataBase[6];
    mDataBase[6]  = mDataBase[22];
    mDataBase[22] = mDataBase[20];
    mDataBase[20] = aHold;

    // (0,1)
    aHold = mDataBase[5];
    mDataBase[5]  = mDataBase[7];
    mDataBase[7]  = mDataBase[23];
    mDataBase[23] = mDataBase[21];
    mDataBase[21] = aHold;

    // (1,0)
    aHold = mDataBase[12];
    mDataBase[12] = mDataBase[14];
    mDataBase[14] = mDataBase[30];
    mDataBase[30] = mDataBase[28];
    mDataBase[28] = aHold;

    // (1,1)
    aHold = mDataBase[13];
    mDataBase[13] = mDataBase[15];
    mDataBase[15] = mDataBase[31];
    mDataBase[31] = mDataBase[29];
    mDataBase[29] = aHold;
}

void FM8::RotateLeftSixteenthsQuarterC() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[32];
    mDataBase[32] = mDataBase[34];
    mDataBase[34] = mDataBase[50];
    mDataBase[50] = mDataBase[48];
    mDataBase[48] = aHold;

    // (0,1)
    aHold = mDataBase[33];
    mDataBase[33] = mDataBase[35];
    mDataBase[35] = mDataBase[51];
    mDataBase[51] = mDataBase[49];
    mDataBase[49] = aHold;

    // (1,0)
    aHold = mDataBase[40];
    mDataBase[40] = mDataBase[42];
    mDataBase[42] = mDataBase[58];
    mDataBase[58] = mDataBase[56];
    mDataBase[56] = aHold;

    // (1,1)
    aHold = mDataBase[41];
    mDataBase[41] = mDataBase[43];
    mDataBase[43] = mDataBase[59];
    mDataBase[59] = mDataBase[57];
    mDataBase[57] = aHold;
}

void FM8::RotateLeftSixteenthsQuarterD() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[36];
    mDataBase[36] = mDataBase[38];
    mDataBase[38] = mDataBase[54];
    mDataBase[54] = mDataBase[52];
    mDataBase[52] = aHold;

    // (0,1)
    aHold = mDataBase[37];
    mDataBase[37] = mDataBase[39];
    mDataBase[39] = mDataBase[55];
    mDataBase[55] = mDataBase[53];
    mDataBase[53] = aHold;

    // (1,0)
    aHold = mDataBase[44];
    mDataBase[44] = mDataBase[46];
    mDataBase[46] = mDataBase[62];
    mDataBase[62] = mDataBase[60];
    mDataBase[60] = aHold;

    // (1,1)
    aHold = mDataBase[45];
    mDataBase[45] = mDataBase[47];
    mDataBase[47] = mDataBase[63];
    mDataBase[63] = mDataBase[61];
    mDataBase[61] = aHold;
}

void FM8::RotateLeftSixteenthsEachQuarter() {

    RotateLeftSixteenthsQuarterA();
    RotateLeftSixteenthsQuarterB();
    RotateLeftSixteenthsQuarterC();
    RotateLeftSixteenthsQuarterD();
}

void FM8::TransposeMainDiagonalSixteenthsQuarterA() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[2];
    mDataBase[2]  = mDataBase[16];
    mDataBase[16] = aHold;

    // (0,1)
    aHold = mDataBase[3];
    mDataBase[3]  = mDataBase[17];
    mDataBase[17] = aHold;

    // (1,0)
    aHold = mDataBase[10];
    mDataBase[10] = mDataBase[24];
    mDataBase[24] = aHold;

    // (1,1)
    aHold = mDataBase[11];
    mDataBase[11] = mDataBase[25];
    mDataBase[25] = aHold;
}

void FM8::TransposeMainDiagonalSixteenthsQuarterB() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[6];
    mDataBase[6]  = mDataBase[20];
    mDataBase[20] = aHold;

    // (0,1)
    aHold = mDataBase[7];
    mDataBase[7]  = mDataBase[21];
    mDataBase[21] = aHold;

    // (1,0)
    aHold = mDataBase[14];
    mDataBase[14] = mDataBase[28];
    mDataBase[28] = aHold;

    // (1,1)
    aHold = mDataBase[15];
    mDataBase[15] = mDataBase[29];
    mDataBase[29] = aHold;
}

void FM8::TransposeMainDiagonalSixteenthsQuarterC() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[34];
    mDataBase[34] = mDataBase[48];
    mDataBase[48] = aHold;

    // (0,1)
    aHold = mDataBase[35];
    mDataBase[35] = mDataBase[49];
    mDataBase[49] = aHold;

    // (1,0)
    aHold = mDataBase[42];
    mDataBase[42] = mDataBase[56];
    mDataBase[56] = aHold;

    // (1,1)
    aHold = mDataBase[43];
    mDataBase[43] = mDataBase[57];
    mDataBase[57] = aHold;
}

void FM8::TransposeMainDiagonalSixteenthsQuarterD() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[38];
    mDataBase[38] = mDataBase[52];
    mDataBase[52] = aHold;

    // (0,1)
    aHold = mDataBase[39];
    mDataBase[39] = mDataBase[53];
    mDataBase[53] = aHold;

    // (1,0)
    aHold = mDataBase[46];
    mDataBase[46] = mDataBase[60];
    mDataBase[60] = aHold;

    // (1,1)
    aHold = mDataBase[47];
    mDataBase[47] = mDataBase[61];
    mDataBase[61] = aHold;
}

void FM8::TransposeMainDiagonalSixteenthsEachQuarter() {

    TransposeMainDiagonalSixteenthsQuarterA();
    TransposeMainDiagonalSixteenthsQuarterB();
    TransposeMainDiagonalSixteenthsQuarterC();
    TransposeMainDiagonalSixteenthsQuarterD();
}

void FM8::TransposeAntiDiagonalSixteenthsQuarterA() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[0];
    mDataBase[0]  = mDataBase[18];
    mDataBase[18] = aHold;

    // (0,1)
    aHold = mDataBase[1];
    mDataBase[1]  = mDataBase[19];
    mDataBase[19] = aHold;

    // (1,0)
    aHold = mDataBase[8];
    mDataBase[8]  = mDataBase[26];
    mDataBase[26] = aHold;

    // (1,1)
    aHold = mDataBase[9];
    mDataBase[9]  = mDataBase[27];
    mDataBase[27] = aHold;
}

void FM8::TransposeAntiDiagonalSixteenthsQuarterB() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[4];
    mDataBase[4]  = mDataBase[22];
    mDataBase[22] = aHold;

    // (0,1)
    aHold = mDataBase[5];
    mDataBase[5]  = mDataBase[23];
    mDataBase[23] = aHold;

    // (1,0)
    aHold = mDataBase[12];
    mDataBase[12] = mDataBase[30];
    mDataBase[30] = aHold;

    // (1,1)
    aHold = mDataBase[13];
    mDataBase[13] = mDataBase[31];
    mDataBase[31] = aHold;
}

void FM8::TransposeAntiDiagonalSixteenthsQuarterC() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[32];
    mDataBase[32] = mDataBase[50];
    mDataBase[50] = aHold;

    // (0,1)
    aHold = mDataBase[33];
    mDataBase[33] = mDataBase[51];
    mDataBase[51] = aHold;

    // (1,0)
    aHold = mDataBase[40];
    mDataBase[40] = mDataBase[58];
    mDataBase[58] = aHold;

    // (1,1)
    aHold = mDataBase[41];
    mDataBase[41] = mDataBase[59];
    mDataBase[59] = aHold;
}

void FM8::TransposeAntiDiagonalSixteenthsQuarterD() {

    std::uint8_t aHold;

    // (0,0)
    aHold = mDataBase[36];
    mDataBase[36] = mDataBase[54];
    mDataBase[54] = aHold;

    // (0,1)
    aHold = mDataBase[37];
    mDataBase[37] = mDataBase[55];
    mDataBase[55] = aHold;

    // (1,0)
    aHold = mDataBase[44];
    mDataBase[44] = mDataBase[62];
    mDataBase[62] = aHold;

    // (1,1)
    aHold = mDataBase[45];
    mDataBase[45] = mDataBase[63];
    mDataBase[63] = aHold;
}

void FM8::TransposeAntiDiagonalSixteenthsEachQuarter() {

    TransposeAntiDiagonalSixteenthsQuarterA();
    TransposeAntiDiagonalSixteenthsQuarterB();
    TransposeAntiDiagonalSixteenthsQuarterC();
    TransposeAntiDiagonalSixteenthsQuarterD();
}

void FM8::FlipHorizontalSixteenthsQuarterA() {

    std::uint8_t aHold;

    // a ↔ b
    aHold = mDataBase[0];  mDataBase[0] = mDataBase[2];  mDataBase[2] = aHold;
    aHold = mDataBase[1];  mDataBase[1] = mDataBase[3];  mDataBase[3] = aHold;
    aHold = mDataBase[8];  mDataBase[8] = mDataBase[10]; mDataBase[10] = aHold;
    aHold = mDataBase[9];  mDataBase[9] = mDataBase[11]; mDataBase[11] = aHold;

    // c ↔ d
    aHold = mDataBase[16]; mDataBase[16] = mDataBase[18]; mDataBase[18] = aHold;
    aHold = mDataBase[17]; mDataBase[17] = mDataBase[19]; mDataBase[19] = aHold;
    aHold = mDataBase[24]; mDataBase[24] = mDataBase[26]; mDataBase[26] = aHold;
    aHold = mDataBase[25]; mDataBase[25] = mDataBase[27]; mDataBase[27] = aHold;
}

void FM8::FlipHorizontalSixteenthsQuarterB() {

    std::uint8_t aHold;

    // a ↔ b
    aHold = mDataBase[4];  mDataBase[4] = mDataBase[6];  mDataBase[6] = aHold;
    aHold = mDataBase[5];  mDataBase[5] = mDataBase[7];  mDataBase[7] = aHold;
    aHold = mDataBase[12]; mDataBase[12] = mDataBase[14]; mDataBase[14] = aHold;
    aHold = mDataBase[13]; mDataBase[13] = mDataBase[15]; mDataBase[15] = aHold;

    // c ↔ d
    aHold = mDataBase[20]; mDataBase[20] = mDataBase[22]; mDataBase[22] = aHold;
    aHold = mDataBase[21]; mDataBase[21] = mDataBase[23]; mDataBase[23] = aHold;
    aHold = mDataBase[28]; mDataBase[28] = mDataBase[30]; mDataBase[30] = aHold;
    aHold = mDataBase[29]; mDataBase[29] = mDataBase[31]; mDataBase[31] = aHold;
}

void FM8::FlipHorizontalSixteenthsQuarterC() {

    std::uint8_t aHold;

    // a ↔ b
    aHold = mDataBase[32]; mDataBase[32] = mDataBase[34]; mDataBase[34] = aHold;
    aHold = mDataBase[33]; mDataBase[33] = mDataBase[35]; mDataBase[35] = aHold;
    aHold = mDataBase[40]; mDataBase[40] = mDataBase[42]; mDataBase[42] = aHold;
    aHold = mDataBase[41]; mDataBase[41] = mDataBase[43]; mDataBase[43] = aHold;

    // c ↔ d
    aHold = mDataBase[48]; mDataBase[48] = mDataBase[50]; mDataBase[50] = aHold;
    aHold = mDataBase[49]; mDataBase[49] = mDataBase[51]; mDataBase[51] = aHold;
    aHold = mDataBase[56]; mDataBase[56] = mDataBase[58]; mDataBase[58] = aHold;
    aHold = mDataBase[57]; mDataBase[57] = mDataBase[59]; mDataBase[59] = aHold;
}

void FM8::FlipHorizontalSixteenthsQuarterD() {

    std::uint8_t aHold;

    // a ↔ b
    aHold = mDataBase[36]; mDataBase[36] = mDataBase[38]; mDataBase[38] = aHold;
    aHold = mDataBase[37]; mDataBase[37] = mDataBase[39]; mDataBase[39] = aHold;
    aHold = mDataBase[44]; mDataBase[44] = mDataBase[46]; mDataBase[46] = aHold;
    aHold = mDataBase[45]; mDataBase[45] = mDataBase[47]; mDataBase[47] = aHold;

    // c ↔ d
    aHold = mDataBase[52]; mDataBase[52] = mDataBase[54]; mDataBase[54] = aHold;
    aHold = mDataBase[53]; mDataBase[53] = mDataBase[55]; mDataBase[55] = aHold;
    aHold = mDataBase[60]; mDataBase[60] = mDataBase[62]; mDataBase[62] = aHold;
    aHold = mDataBase[61]; mDataBase[61] = mDataBase[63]; mDataBase[63] = aHold;
}

void FM8::FlipHorizontalSixteenthsEachQuarter() {

    FlipHorizontalSixteenthsQuarterA();
    FlipHorizontalSixteenthsQuarterB();
    FlipHorizontalSixteenthsQuarterC();
    FlipHorizontalSixteenthsQuarterD();
}

void FM8::FlipVerticalSixteenthsQuarterA() {

    std::uint8_t aHold;

    // a ↔ c
    aHold = mDataBase[0];  mDataBase[0] = mDataBase[16]; mDataBase[16] = aHold;
    aHold = mDataBase[1];  mDataBase[1] = mDataBase[17]; mDataBase[17] = aHold;
    aHold = mDataBase[8];  mDataBase[8] = mDataBase[24]; mDataBase[24] = aHold;
    aHold = mDataBase[9];  mDataBase[9] = mDataBase[25]; mDataBase[25] = aHold;

    // b ↔ d
    aHold = mDataBase[2];  mDataBase[2] = mDataBase[18]; mDataBase[18] = aHold;
    aHold = mDataBase[3];  mDataBase[3] = mDataBase[19]; mDataBase[19] = aHold;
    aHold = mDataBase[10]; mDataBase[10] = mDataBase[26]; mDataBase[26] = aHold;
    aHold = mDataBase[11]; mDataBase[11] = mDataBase[27]; mDataBase[27] = aHold;
}

void FM8::FlipVerticalSixteenthsQuarterB() {

    std::uint8_t aHold;

    // a ↔ c
    aHold = mDataBase[4];  mDataBase[4] = mDataBase[20]; mDataBase[20] = aHold;
    aHold = mDataBase[5];  mDataBase[5] = mDataBase[21]; mDataBase[21] = aHold;
    aHold = mDataBase[12]; mDataBase[12] = mDataBase[28]; mDataBase[28] = aHold;
    aHold = mDataBase[13]; mDataBase[13] = mDataBase[29]; mDataBase[29] = aHold;

    // b ↔ d
    aHold = mDataBase[6];  mDataBase[6] = mDataBase[22]; mDataBase[22] = aHold;
    aHold = mDataBase[7];  mDataBase[7] = mDataBase[23]; mDataBase[23] = aHold;
    aHold = mDataBase[14]; mDataBase[14] = mDataBase[30]; mDataBase[30] = aHold;
    aHold = mDataBase[15]; mDataBase[15] = mDataBase[31]; mDataBase[31] = aHold;
}

void FM8::FlipVerticalSixteenthsQuarterC() {

    std::uint8_t aHold;

    // a ↔ c
    aHold = mDataBase[32]; mDataBase[32] = mDataBase[48]; mDataBase[48] = aHold;
    aHold = mDataBase[33]; mDataBase[33] = mDataBase[49]; mDataBase[49] = aHold;
    aHold = mDataBase[40]; mDataBase[40] = mDataBase[56]; mDataBase[56] = aHold;
    aHold = mDataBase[41]; mDataBase[41] = mDataBase[57]; mDataBase[57] = aHold;

    // b ↔ d
    aHold = mDataBase[34]; mDataBase[34] = mDataBase[50]; mDataBase[50] = aHold;
    aHold = mDataBase[35]; mDataBase[35] = mDataBase[51]; mDataBase[51] = aHold;
    aHold = mDataBase[42]; mDataBase[42] = mDataBase[58]; mDataBase[58] = aHold;
    aHold = mDataBase[43]; mDataBase[43] = mDataBase[59]; mDataBase[59] = aHold;
}

void FM8::FlipVerticalSixteenthsQuarterD() {

    std::uint8_t aHold;

    // a ↔ c
    aHold = mDataBase[36]; mDataBase[36] = mDataBase[52]; mDataBase[52] = aHold;
    aHold = mDataBase[37]; mDataBase[37] = mDataBase[53]; mDataBase[53] = aHold;
    aHold = mDataBase[44]; mDataBase[44] = mDataBase[60]; mDataBase[60] = aHold;
    aHold = mDataBase[45]; mDataBase[45] = mDataBase[61]; mDataBase[61] = aHold;

    // b ↔ d
    aHold = mDataBase[38]; mDataBase[38] = mDataBase[54]; mDataBase[54] = aHold;
    aHold = mDataBase[39]; mDataBase[39] = mDataBase[55]; mDataBase[55] = aHold;
    aHold = mDataBase[46]; mDataBase[46] = mDataBase[62]; mDataBase[62] = aHold;
    aHold = mDataBase[47]; mDataBase[47] = mDataBase[63]; mDataBase[63] = aHold;
}

void FM8::FlipVerticalSixteenthsEachQuarter() {

    FlipVerticalSixteenthsQuarterA();
    FlipVerticalSixteenthsQuarterB();
    FlipVerticalSixteenthsQuarterC();
    FlipVerticalSixteenthsQuarterD();
}

void FM8::PinwheelRight() {
    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aDst = mTemp;
    aDst[ 0] = aSrc[32]; aDst[ 1] = aSrc[24]; aDst[ 2] = aSrc[16]; aDst[ 3] = aSrc[ 8];
    aDst[ 4] = aSrc[ 0]; aDst[ 5] = aSrc[ 1]; aDst[ 6] = aSrc[ 2]; aDst[ 7] = aSrc[ 3];
    aDst[ 8] = aSrc[40]; aDst[ 9] = aSrc[12]; aDst[10] = aSrc[13]; aDst[11] = aSrc[14];
    aDst[12] = aSrc[22]; aDst[13] = aSrc[30]; aDst[14] = aSrc[38]; aDst[15] = aSrc[ 4];
    aDst[16] = aSrc[48]; aDst[17] = aSrc[11]; aDst[18] = aSrc[34]; aDst[19] = aSrc[26];
    aDst[20] = aSrc[18]; aDst[21] = aSrc[19]; aDst[22] = aSrc[46]; aDst[23] = aSrc[ 5];
    aDst[24] = aSrc[56]; aDst[25] = aSrc[10]; aDst[26] = aSrc[42]; aDst[27] = aSrc[28];
    aDst[28] = aSrc[36]; aDst[29] = aSrc[20]; aDst[30] = aSrc[54]; aDst[31] = aSrc[ 6];
    aDst[32] = aSrc[57]; aDst[33] = aSrc[ 9]; aDst[34] = aSrc[43]; aDst[35] = aSrc[27];
    aDst[36] = aSrc[35]; aDst[37] = aSrc[21]; aDst[38] = aSrc[53]; aDst[39] = aSrc[ 7];
    aDst[40] = aSrc[58]; aDst[41] = aSrc[17]; aDst[42] = aSrc[44]; aDst[43] = aSrc[45];
    aDst[44] = aSrc[37]; aDst[45] = aSrc[29]; aDst[46] = aSrc[52]; aDst[47] = aSrc[15];
    aDst[48] = aSrc[59]; aDst[49] = aSrc[25]; aDst[50] = aSrc[33]; aDst[51] = aSrc[41];
    aDst[52] = aSrc[49]; aDst[53] = aSrc[50]; aDst[54] = aSrc[51]; aDst[55] = aSrc[23];
    aDst[56] = aSrc[60]; aDst[57] = aSrc[61]; aDst[58] = aSrc[62]; aDst[59] = aSrc[63];
    aDst[60] = aSrc[55]; aDst[61] = aSrc[47]; aDst[62] = aSrc[39]; aDst[63] = aSrc[31];
    memcpy(mDataBase, mTemp, 64);
}

void FM8::PinwheelLeft() {
    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aDst = mTemp;
    aDst[32] = aSrc[ 0]; aDst[24] = aSrc[ 1]; aDst[16] = aSrc[ 2]; aDst[ 8] = aSrc[ 3];
    aDst[ 0] = aSrc[ 4]; aDst[ 1] = aSrc[ 5]; aDst[ 2] = aSrc[ 6]; aDst[ 3] = aSrc[ 7];
    aDst[40] = aSrc[ 8]; aDst[12] = aSrc[ 9]; aDst[13] = aSrc[10]; aDst[14] = aSrc[11];
    aDst[22] = aSrc[12]; aDst[30] = aSrc[13]; aDst[38] = aSrc[14]; aDst[ 4] = aSrc[15];
    aDst[48] = aSrc[16]; aDst[11] = aSrc[17]; aDst[34] = aSrc[18]; aDst[26] = aSrc[19];
    aDst[18] = aSrc[20]; aDst[19] = aSrc[21]; aDst[46] = aSrc[22]; aDst[ 5] = aSrc[23];
    aDst[56] = aSrc[24]; aDst[10] = aSrc[25]; aDst[42] = aSrc[26]; aDst[28] = aSrc[27];
    aDst[36] = aSrc[28]; aDst[20] = aSrc[29]; aDst[54] = aSrc[30]; aDst[ 6] = aSrc[31];
    aDst[57] = aSrc[32]; aDst[ 9] = aSrc[33]; aDst[43] = aSrc[34]; aDst[27] = aSrc[35];
    aDst[35] = aSrc[36]; aDst[21] = aSrc[37]; aDst[53] = aSrc[38]; aDst[ 7] = aSrc[39];
    aDst[58] = aSrc[40]; aDst[17] = aSrc[41]; aDst[44] = aSrc[42]; aDst[45] = aSrc[43];
    aDst[37] = aSrc[44]; aDst[29] = aSrc[45]; aDst[52] = aSrc[46]; aDst[15] = aSrc[47];
    aDst[59] = aSrc[48]; aDst[25] = aSrc[49]; aDst[33] = aSrc[50]; aDst[41] = aSrc[51];
    aDst[49] = aSrc[52]; aDst[50] = aSrc[53]; aDst[51] = aSrc[54]; aDst[23] = aSrc[55];
    aDst[60] = aSrc[56]; aDst[61] = aSrc[57]; aDst[62] = aSrc[58]; aDst[63] = aSrc[59];
    aDst[55] = aSrc[60]; aDst[47] = aSrc[61]; aDst[39] = aSrc[62]; aDst[31] = aSrc[63];
    memcpy(mDataBase, mTemp, 64);
}

void FM8::PinwheelRightQuarterA() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[ 0] = aSrc[16]; aTmp[ 1] = aSrc[ 8]; aTmp[ 2] = aSrc[ 0]; aTmp[ 3] = aSrc[ 1];
    aTmp[ 8] = aSrc[24]; aTmp[ 9] = aSrc[10]; aTmp[10] = aSrc[18]; aTmp[11] = aSrc[ 2];
    aTmp[16] = aSrc[25]; aTmp[17] = aSrc[ 9]; aTmp[18] = aSrc[17]; aTmp[19] = aSrc[ 3];
    aTmp[24] = aSrc[26]; aTmp[25] = aSrc[27]; aTmp[26] = aSrc[19]; aTmp[27] = aSrc[11];

    aSrc[ 0] = aTmp[ 0]; aSrc[ 1] = aTmp[ 1]; aSrc[ 2] = aTmp[ 2]; aSrc[ 3] = aTmp[ 3];
    aSrc[ 8] = aTmp[ 8]; aSrc[ 9] = aTmp[ 9]; aSrc[10] = aTmp[10]; aSrc[11] = aTmp[11];
    aSrc[16] = aTmp[16]; aSrc[17] = aTmp[17]; aSrc[18] = aTmp[18]; aSrc[19] = aTmp[19];
    aSrc[24] = aTmp[24]; aSrc[25] = aTmp[25]; aSrc[26] = aTmp[26]; aSrc[27] = aTmp[27];
}

void FM8::PinwheelRightQuarterB() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[ 4] = aSrc[20]; aTmp[ 5] = aSrc[12]; aTmp[ 6] = aSrc[ 4]; aTmp[ 7] = aSrc[ 5];
    aTmp[12] = aSrc[28]; aTmp[13] = aSrc[14]; aTmp[14] = aSrc[22]; aTmp[15] = aSrc[ 6];
    aTmp[20] = aSrc[29]; aTmp[21] = aSrc[13]; aTmp[22] = aSrc[21]; aTmp[23] = aSrc[ 7];
    aTmp[28] = aSrc[30]; aTmp[29] = aSrc[31]; aTmp[30] = aSrc[23]; aTmp[31] = aSrc[15];

    aSrc[ 4] = aTmp[ 4]; aSrc[ 5] = aTmp[ 5]; aSrc[ 6] = aTmp[ 6]; aSrc[ 7] = aTmp[ 7];
    aSrc[12] = aTmp[12]; aSrc[13] = aTmp[13]; aSrc[14] = aTmp[14]; aSrc[15] = aTmp[15];
    aSrc[20] = aTmp[20]; aSrc[21] = aTmp[21]; aSrc[22] = aTmp[22]; aSrc[23] = aTmp[23];
    aSrc[28] = aTmp[28]; aSrc[29] = aTmp[29]; aSrc[30] = aTmp[30]; aSrc[31] = aTmp[31];
}

void FM8::PinwheelRightQuarterC() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[32] = aSrc[48]; aTmp[33] = aSrc[40]; aTmp[34] = aSrc[32]; aTmp[35] = aSrc[33];
    aTmp[40] = aSrc[56]; aTmp[41] = aSrc[42]; aTmp[42] = aSrc[50]; aTmp[43] = aSrc[34];
    aTmp[48] = aSrc[57]; aTmp[49] = aSrc[41]; aTmp[50] = aSrc[49]; aTmp[51] = aSrc[35];
    aTmp[56] = aSrc[58]; aTmp[57] = aSrc[59]; aTmp[58] = aSrc[51]; aTmp[59] = aSrc[43];

    aSrc[32]=aTmp[32]; aSrc[33]=aTmp[33]; aSrc[34]=aTmp[34]; aSrc[35]=aTmp[35];
    aSrc[40]=aTmp[40]; aSrc[41]=aTmp[41]; aSrc[42]=aTmp[42]; aSrc[43]=aTmp[43];
    aSrc[48]=aTmp[48]; aSrc[49]=aTmp[49]; aSrc[50]=aTmp[50]; aSrc[51]=aTmp[51];
    aSrc[56]=aTmp[56]; aSrc[57]=aTmp[57]; aSrc[58]=aTmp[58]; aSrc[59]=aTmp[59];
}

void FM8::PinwheelRightQuarterD() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[36] = aSrc[52]; aTmp[37] = aSrc[44]; aTmp[38] = aSrc[36]; aTmp[39] = aSrc[37];
    aTmp[44] = aSrc[60]; aTmp[45] = aSrc[46]; aTmp[46] = aSrc[54]; aTmp[47] = aSrc[38];
    aTmp[52] = aSrc[61]; aTmp[53] = aSrc[45]; aTmp[54] = aSrc[53]; aTmp[55] = aSrc[39];
    aTmp[60] = aSrc[62]; aTmp[61] = aSrc[63]; aTmp[62] = aSrc[55]; aTmp[63] = aSrc[47];

    aSrc[36]=aTmp[36]; aSrc[37]=aTmp[37]; aSrc[38]=aTmp[38]; aSrc[39]=aTmp[39];
    aSrc[44]=aTmp[44]; aSrc[45]=aTmp[45]; aSrc[46]=aTmp[46]; aSrc[47]=aTmp[47];
    aSrc[52]=aTmp[52]; aSrc[53]=aTmp[53]; aSrc[54]=aTmp[54]; aSrc[55]=aTmp[55];
    aSrc[60]=aTmp[60]; aSrc[61]=aTmp[61]; aSrc[62]=aTmp[62]; aSrc[63]=aTmp[63];
}

void FM8::PinwheelLeftQuarterA() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[16] = aSrc[ 0]; aTmp[ 8] = aSrc[ 1]; aTmp[ 0] = aSrc[ 2]; aTmp[ 1] = aSrc[ 3];
    aTmp[24] = aSrc[ 8]; aTmp[10] = aSrc[ 9]; aTmp[18] = aSrc[10]; aTmp[ 2] = aSrc[11];
    aTmp[25] = aSrc[16]; aTmp[ 9] = aSrc[17]; aTmp[17] = aSrc[18]; aTmp[ 3] = aSrc[19];
    aTmp[26] = aSrc[24]; aTmp[27] = aSrc[25]; aTmp[19] = aSrc[26]; aTmp[11] = aSrc[27];

    aSrc[ 0] = aTmp[ 0]; aSrc[ 1] = aTmp[ 1]; aSrc[ 2] = aTmp[ 2]; aSrc[ 3] = aTmp[ 3];
    aSrc[ 8] = aTmp[ 8]; aSrc[ 9] = aTmp[ 9]; aSrc[10] = aTmp[10]; aSrc[11] = aTmp[11];
    aSrc[16] = aTmp[16]; aSrc[17] = aTmp[17]; aSrc[18] = aTmp[18]; aSrc[19] = aTmp[19];
    aSrc[24] = aTmp[24]; aSrc[25] = aTmp[25]; aSrc[26] = aTmp[26]; aSrc[27] = aTmp[27];
}

void FM8::PinwheelLeftQuarterB() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[20] = aSrc[ 4]; aTmp[12] = aSrc[ 5]; aTmp[ 4] = aSrc[ 6]; aTmp[ 5] = aSrc[ 7];
    aTmp[28] = aSrc[12]; aTmp[14] = aSrc[13]; aTmp[22] = aSrc[14]; aTmp[ 6] = aSrc[15];
    aTmp[29] = aSrc[20]; aTmp[13] = aSrc[21]; aTmp[21] = aSrc[22]; aTmp[ 7] = aSrc[23];
    aTmp[30] = aSrc[28]; aTmp[31] = aSrc[29]; aTmp[23] = aSrc[30]; aTmp[15] = aSrc[31];

    aSrc[ 4] = aTmp[ 4]; aSrc[ 5] = aTmp[ 5]; aSrc[ 6] = aTmp[ 6]; aSrc[ 7] = aTmp[ 7];
    aSrc[12] = aTmp[12]; aSrc[13] = aTmp[13]; aSrc[14] = aTmp[14]; aSrc[15] = aTmp[15];
    aSrc[20] = aTmp[20]; aSrc[21] = aTmp[21]; aSrc[22] = aTmp[22]; aSrc[23] = aTmp[23];
    aSrc[28] = aTmp[28]; aSrc[29] = aTmp[29]; aSrc[30] = aTmp[30]; aSrc[31] = aTmp[31];
}

void FM8::PinwheelLeftQuarterC() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[48] = aSrc[32]; aTmp[40] = aSrc[33]; aTmp[32] = aSrc[34]; aTmp[33] = aSrc[35];
    aTmp[56] = aSrc[40]; aTmp[42] = aSrc[41]; aTmp[50] = aSrc[42]; aTmp[34] = aSrc[43];
    aTmp[57] = aSrc[48]; aTmp[41] = aSrc[49]; aTmp[49] = aSrc[50]; aTmp[35] = aSrc[51];
    aTmp[58] = aSrc[56]; aTmp[59] = aSrc[57]; aTmp[51] = aSrc[58]; aTmp[43] = aSrc[59];

    aSrc[32]=aTmp[32]; aSrc[33]=aTmp[33]; aSrc[34]=aTmp[34]; aSrc[35]=aTmp[35];
    aSrc[40]=aTmp[40]; aSrc[41]=aTmp[41]; aSrc[42]=aTmp[42]; aSrc[43]=aTmp[43];
    aSrc[48]=aTmp[48]; aSrc[49]=aTmp[49]; aSrc[50]=aTmp[50]; aSrc[51]=aTmp[51];
    aSrc[56]=aTmp[56]; aSrc[57]=aTmp[57]; aSrc[58]=aTmp[58]; aSrc[59]=aTmp[59];
}

void FM8::PinwheelLeftQuarterD() {

    std::uint8_t *aSrc = mDataBase;
    std::uint8_t *aTmp = mTemp;

    aTmp[52] = aSrc[36]; aTmp[44] = aSrc[37]; aTmp[36] = aSrc[38]; aTmp[37] = aSrc[39];
    aTmp[60] = aSrc[44]; aTmp[46] = aSrc[45]; aTmp[54] = aSrc[46]; aTmp[38] = aSrc[47];
    aTmp[61] = aSrc[52]; aTmp[45] = aSrc[53]; aTmp[53] = aSrc[54]; aTmp[39] = aSrc[55];
    aTmp[62] = aSrc[60]; aTmp[63] = aSrc[61]; aTmp[55] = aSrc[62]; aTmp[47] = aSrc[63];

    aSrc[36]=aTmp[36]; aSrc[37]=aTmp[37]; aSrc[38]=aTmp[38]; aSrc[39]=aTmp[39];
    aSrc[44]=aTmp[44]; aSrc[45]=aTmp[45]; aSrc[46]=aTmp[46]; aSrc[47]=aTmp[47];
    aSrc[52]=aTmp[52]; aSrc[53]=aTmp[53]; aSrc[54]=aTmp[54]; aSrc[55]=aTmp[55];
    aSrc[60]=aTmp[60]; aSrc[61]=aTmp[61]; aSrc[62]=aTmp[62]; aSrc[63]=aTmp[63];
}

bool FM8::operator==(const FM8& other) const {
    return std::memcmp(mDataBase, other.mDataBase, kSize) == 0;
}

bool FM8::operator!=(const FM8& other) const {
    return std::memcmp(mDataBase, other.mDataBase, kSize) != 0;
}
