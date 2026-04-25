#ifndef F_SILLY_STRING_H
#define F_SILLY_STRING_H

#include <cstdint>
#include <string>
#include <vector>




class ByteString {
public:
    ByteString();
    ByteString(const ByteString &pString);
    ByteString(ByteString &&pString) noexcept;
    ByteString(const std::string &pString);
    ByteString(const std::vector<uint8_t> &pCharacters);
    
    
    ByteString(int pNumber);
    ByteString(float pNumber, int pDecimalCount);
    ~ByteString();

    std::uint8_t                    *mData;
    int                             mLength;
    int                             mSize;

    void                            Size(int pSize);

    void                            Free();
    void                            Clear();

    void                            Set(const std::uint8_t *pString, int pLength);
    void                            Set(const std::string &pString);
    void                            Set(const ByteString &pString);
    void                            Set(std::vector<uint8_t> &pCharacters);
    

    int                             Compare(const std::uint8_t *pString, int pLength) const;
    int                             Compare(const std::string &pString) const;
    int                             Compare(const ByteString &pString) const;

    void                            Append(const std::uint8_t *pString, int pLength);
    void                            Append(const std::string &pString);
    void                            Append(const ByteString &pString);

    void                            Truncate(int pSize);

    void                            Reverse();
    void                            Reverse(int pStartIndex, int pEndIndex);
    void                            Rotate(int pAmount);
    
    int                             RotateMinAmount();
    void                            RotateMin();
    
    ByteString                      Substring(int pIndex, int pLength);
    ByteString                      SubstringWrapping(int pIndex, int pLength);
    
    
    inline void                     ParseInt(int pNumber);
    inline void                     ParseFloat(float pFloat, int pDecimalCount);

    std::string                     ToString() const;
    std::string                     ToHexArrayString() const;
    
    static int            Hash(std::uint8_t *pData, int pLength);
    int                   Hash();
    int                   HashRotationInvariant();
    
    inline bool                     operator == (const ByteString &pString) const { return Compare(pString) == 0; }
    inline bool                     operator == (const std::string &pString) const { return Compare(pString) == 0; }

    ByteString                      &operator = (const ByteString &pString);
    ByteString                      &operator = (ByteString &&pString) noexcept;
    ByteString                      &operator = (const std::string &pString);

    ByteString                      operator + (const ByteString &pRight) const;
    ByteString                      operator + (const std::string &pRight) const;
};

#endif
