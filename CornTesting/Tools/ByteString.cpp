#include "ByteString.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>
#include <vector>

ByteString::ByteString() {
    mData = nullptr;
    mLength = 0;
    mSize = 0;
}

ByteString::ByteString(const ByteString &pString) {
    mData = nullptr;
    mLength = 0;
    mSize = 0;
    Set(pString);
}

ByteString::ByteString(ByteString &&pString) noexcept {
    mData = pString.mData;
    mLength = pString.mLength;
    mSize = pString.mSize;

    pString.mData = nullptr;
    pString.mLength = 0;
    pString.mSize = 0;
}

ByteString::ByteString(const std::string &pString) {
    mData = nullptr;
    mLength = 0;
    mSize = 0;
    Set(pString);
}

ByteString::ByteString(int pNumber) {
    mData = nullptr;
    mLength = 0;
    mSize = 0;
    ParseInt(pNumber);
}

ByteString::ByteString(float pNumber, int pDecimalCount) {
    mData = nullptr;
    mLength = 0;
    mSize = 0;
    ParseFloat(pNumber, pDecimalCount);
}

ByteString::ByteString(const std::vector<uint8_t> &pCharacters) {
    mData = nullptr;
    mLength = 0;
    mSize = 0;
    Set(pCharacters.data(), (int)pCharacters.size());
}

ByteString::~ByteString() {
    Free();
}

void ByteString::Free() {
    //delete [] mData;
    
    mData = nullptr;
    mLength = 0;
    mSize = 0;
}

void ByteString::Clear() {
    mLength = 0;
}

void ByteString::Size(int pSize) {
    if (pSize <= 0) {
        Free();
        return;
    }

    if (pSize == mSize) {
        return;
    }

    std::uint8_t *aNewData = new std::uint8_t[pSize];
    const int aBytesToCopy = std::min(mLength, pSize);
    if (mData != nullptr && aBytesToCopy > 0) {
        std::memcpy(aNewData, mData, static_cast<std::size_t>(aBytesToCopy));
    }

    //delete [] mData;
    mData = aNewData;
    mSize = pSize;
    mLength = aBytesToCopy;
}

void ByteString::Set(const std::uint8_t *pString, int pLength) {
    if (pString == nullptr || pLength <= 0) {
        mLength = 0;
        return;
    }

    if (pLength > mSize) {
        Size(pLength);
    }

    std::memcpy(mData, pString, static_cast<std::size_t>(pLength));
    mLength = pLength;
}

void ByteString::Set(const std::string &pString) {
    Set(reinterpret_cast<const std::uint8_t *>(pString.data()), static_cast<int>(pString.size()));
}

void ByteString::Set(const ByteString &pString) {
    if (this == &pString) {
        return;
    }

    Set(pString.mData, pString.mLength);
}

void ByteString::Set(std::vector<uint8_t> &pCharacters) {
    if (mSize < pCharacters.size()) {
        Size((int)pCharacters.size());
    }
    
    memcpy(mData, (std::uint8_t *)pCharacters.data(), pCharacters.size());
    mLength = (int)pCharacters.size();
}

int ByteString::Compare(const std::uint8_t *pString, int pLength) const {
    if (pLength <= 0) {
        if (mLength <= 0) {
            return 0;
        }
        return 1;
    }

    if (pString == nullptr) {
        return (mLength <= 0) ? 0 : 1;
    }

    const int aShortest = std::min(mLength, pLength);
    for (int i = 0; i < aShortest; ++i) {
        if (mData[i] < pString[i]) { return -1; }
        if (mData[i] > pString[i]) { return 1; }
    }

    if (mLength < pLength) { return -1; }
    if (mLength > pLength) { return 1; }
    return 0;
}

int ByteString::Compare(const std::string &pString) const {
    return Compare(reinterpret_cast<const std::uint8_t *>(pString.data()), static_cast<int>(pString.size()));
}

int ByteString::Compare(const ByteString &pString) const {
    return Compare(pString.mData, pString.mLength);
}

void ByteString::Append(const std::uint8_t *pString, int pLength) {
    if (pString == nullptr || pLength <= 0) {
        return;
    }

    const int aOldLength = mLength;
    const int aNewLength = aOldLength + pLength;
    if (aNewLength > mSize) {
        Size(aNewLength);
    }

    std::memcpy(&(mData[aOldLength]), pString, static_cast<std::size_t>(pLength));
    mLength = aNewLength;
}

void ByteString::Append(const std::string &pString) {
    Append(reinterpret_cast<const std::uint8_t *>(pString.data()), static_cast<int>(pString.size()));
}

void ByteString::Append(const ByteString &pString) {
    if (this == &pString) {
        ByteString aCopy(pString);
        Append(aCopy.mData, aCopy.mLength);
        return;
    }

    Append(pString.mData, pString.mLength);
}

void ByteString::Reverse() {
    Reverse(0, mLength - 1);
}

void ByteString::Reverse(int pStartIndex, int pEndIndex) {
    if (pStartIndex < 0 || pEndIndex < 0) {
        return;
    }
    if (pStartIndex >= mLength || pEndIndex >= mLength) {
        return;
    }

    while (pStartIndex < pEndIndex) {
        std::swap(mData[pStartIndex], mData[pEndIndex]);
        ++pStartIndex;
        --pEndIndex;
    }
}

void ByteString::Rotate(int pAmount) {
    if (mLength <= 1) {
        return;
    }

    long long aAmount64 = static_cast<long long>(pAmount) % static_cast<long long>(mLength);
    if (aAmount64 < 0) {
        aAmount64 += mLength;
    }

    const int aAmount = static_cast<int>(aAmount64);
    if (aAmount == 0) {
        return;
    }

    Reverse(0, mLength - 1);
    Reverse(0, aAmount - 1);
    Reverse(aAmount, mLength - 1);
}

void ByteString::Truncate(int pSize) {
    if (pSize <= 0) {
        Free();
        return;
    }
    if (pSize < mLength) {
        mLength = pSize;
    }
}

int ByteString::RotateMinAmount() {
    int i = 0;
    int j = 1;
    int k = 0;
    while ((i < mLength) && (j < mLength) && (k < mLength)) {
        int aIndexIK = (i + k);
        if (aIndexIK >= mLength) { aIndexIK -= mLength; }
        int aIndexJK = (j + k);
        if (aIndexJK >= mLength) { aIndexJK -= mLength; }
        std::uint8_t aCharIK = mData[aIndexIK];
        std::uint8_t aCharJK = mData[aIndexJK];
        if (aCharIK == aCharJK) {
            ++k;
        } else if (aCharIK > aCharJK) {
            i += (k + 1);
            if (i <= j) { i = j + 1; }
            k = 0;
        } else {
            j += (k + 1);
            if (j <= i) { j = i + 1; }
            k = 0;
        }
    }
    return mLength - std::min(i, j);
}

void ByteString::RotateMin() {
    int aAmount = RotateMinAmount();
    Rotate(aAmount);
}

int ByteString::Hash(std::uint8_t *pData, int pLength) {
    int aResult = 5381;
    for (int aIndex=0;aIndex<pLength;aIndex++) {
        int aValue = pData[aIndex];
        aResult = ((aResult << 5) + aResult) ^ aValue;
    }
    return aResult;
}

int ByteString::Hash() {
    return ByteString::Hash(mData, mLength);
}

int ByteString::HashRotationInvariant() {
    int aRotateMinAmount = RotateMinAmount();
    if (aRotateMinAmount == 0) {
        return ByteString::Hash(mData, mLength);
    } else {
        Rotate(aRotateMinAmount);
        int aResult = ByteString::Hash(mData, mLength);
        Rotate(-aRotateMinAmount);
        return aResult;
    }
}

ByteString ByteString::Substring(int pIndex, int pLength) {
    int aStartIndex = pIndex;
    int aEndIndex = pIndex + pLength;

    if (aStartIndex < 0) {
        aEndIndex += aStartIndex;
        aStartIndex = 0;
    }
    if (aEndIndex > mLength) {
        aEndIndex = mLength;
    }

    int aLength = (aEndIndex - aStartIndex);
    ByteString aResult;
    if ((aStartIndex < mLength) && (mLength > 0) && (aLength > 0)) {
        aResult.Size(aLength);
        memcpy(aResult.mData, &(mData[aStartIndex]), aLength);
        aResult.mLength = aLength;
    }
    return aResult;
}

ByteString ByteString::SubstringWrapping(int pIndex, int pLength) {
    int aLength = pLength;
    if (mLength <= 0 || aLength <= 0) {
        return ByteString();
    }

    if (aLength > mLength) {
        aLength = mLength;
    }

    // Normalize index into [0, mLength)
    int aIndex = pIndex % mLength;
    if (aIndex < 0) {
        aIndex += mLength;
    }

    ByteString aResult;
    aResult.Size(aLength);

    int aRemaining = aLength;
    int aWriteOffset = 0;
    int aReadIndex = aIndex;

    while (aRemaining > 0) {
        // How much we can copy before hitting end
        int aChunk = mLength - aReadIndex;
        if (aChunk > aRemaining) {
            aChunk = aRemaining;
        }

        memcpy(aResult.mData + aWriteOffset,
               mData + aReadIndex,
               aChunk);

        aRemaining -= aChunk;
        aWriteOffset += aChunk;

        // Wrap to beginning
        aReadIndex = 0;
    }

    aResult.mLength = aLength;
    return aResult;
}


void ByteString::ParseInt(int pNumber) {
    char aBuffer[16];
    int aWritePos = 0;

    if (pNumber == 0) {
        aBuffer[0] = '0';
        Set(reinterpret_cast<const std::uint8_t *>(aBuffer), 1);
        return;
    }

    long long aNumber = pNumber;
    if (aNumber < 0) {
        aBuffer[aWritePos++] = '-';
        aNumber = -aNumber;
    }

    long long aScan = aNumber;
    while (aScan > 0) {
        aScan /= 10;
        ++aWritePos;
    }

    const int aTotalLength = aWritePos;
    int aIndex = aTotalLength - 1;
    while (aNumber > 0) {
        aBuffer[aIndex--] = static_cast<char>('0' + (aNumber % 10));
        aNumber /= 10;
    }

    Set(reinterpret_cast<const std::uint8_t *>(aBuffer), aTotalLength);
}

void ByteString::ParseFloat(float pFloat, int pDecimalCount) {
    std::uint8_t aBuffer[64];
    int aWritePos = 0;

    if (pFloat < 0.0f) {
        aBuffer[aWritePos++] = '-';
        pFloat = -pFloat;
    }

    if (pFloat > 100000000.0f) {
        pFloat = 100000000.0f;
    }

    double aValue = static_cast<double>(pFloat);
    double aRounding = 0.5;
    for (int i = 0; i < pDecimalCount; ++i) {
        aRounding /= 10.0;
    }
    aValue += aRounding;

    long long aWholePart = static_cast<long long>(aValue);
    double aFractionPart = aValue - static_cast<double>(aWholePart);

    if (aWholePart == 0) {
        aBuffer[aWritePos++] = '0';
    } else {
        const int aDigitsStart = aWritePos;
        while (aWholePart > 0) {
            aBuffer[aWritePos++] = static_cast<std::uint8_t>('0' + (aWholePart % 10));
            aWholePart /= 10;
        }

        for (int i = 0; (aDigitsStart + i) < (aWritePos - 1 - i); ++i) {
            std::swap(aBuffer[aDigitsStart + i], aBuffer[aWritePos - 1 - i]);
        }
    }

    if (pDecimalCount > 0) {
        aBuffer[aWritePos++] = '.';
        for (int i = 0; i < pDecimalCount; ++i) {
            aFractionPart *= 10.0;
            const int aDigit = static_cast<int>(aFractionPart);
            aBuffer[aWritePos++] = static_cast<std::uint8_t>('0' + aDigit);
            aFractionPart -= aDigit;
        }
    }

    Set(aBuffer, aWritePos);
}

std::string ByteString::ToHexArrayString() const {
    std::string aResult = "{";
    aResult.reserve(static_cast<std::size_t>(mLength) * 4U);

    for (int aByteIndex = 0; aByteIndex < mLength; ++aByteIndex) {
        aResult += std::format("{:02x}", mData[aByteIndex]);
        if (aByteIndex != (mLength - 1)) {
            aResult.append(", ");
        }
    }

    aResult.append("}");
    return aResult;
}

std::string ByteString::ToString() const {
    if (mData == nullptr || mLength <= 0) {
        return std::string();
    }

    return std::string(reinterpret_cast<const char *>(mData), static_cast<std::size_t>(mLength));
}

ByteString ByteString::operator+(const ByteString &pRight) const {
    ByteString aResult(*this);
    aResult.Append(pRight);
    return aResult;
}

ByteString ByteString::operator+(const std::string &pRight) const {
    ByteString aResult(*this);
    aResult.Append(pRight);
    return aResult;
}

ByteString &ByteString::operator = (const ByteString &pString) {
    Set(pString);
    return *this;
}

ByteString &ByteString::operator = (ByteString &&pString) noexcept {
    if (this == &pString) {
        return *this;
    }

    //delete [] mData;
    
    mData = pString.mData;
    mLength = pString.mLength;
    mSize = pString.mSize;

    pString.mData = nullptr;
    pString.mLength = 0;
    pString.mSize = 0;
    return *this;
}

ByteString &ByteString::operator = (const std::string &pString) {
    Set(pString);
    return *this;
}
