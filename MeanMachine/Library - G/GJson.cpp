//
//  GJson.cpp
//  MeanMachine
//

#include "GJson.hpp"

#include <cctype>
#include <cstdlib>
#include <sstream>
#include <string_view>

namespace MeanMachine_json {
namespace {

class Parser {
public:
    explicit Parser(std::string_view pText)
    : mText(pText) {}

    std::optional<JsonValue> ParseValue(std::string *pError) {
        SkipWhitespace();
        if (AtEnd()) {
            return Fail("unexpected end of JSON", pError);
        }

        const char aCh = Peek();
        if (aCh == 'n') { return ParseNull(pError); }
        if (aCh == 't' || aCh == 'f') { return ParseBool(pError); }
        if (aCh == '"') { return ParseString(pError); }
        if (aCh == '[') { return ParseArray(pError); }
        if (aCh == '{') { return ParseObject(pError); }
        if (aCh == '-' || std::isdigit(static_cast<unsigned char>(aCh)) != 0) {
            return ParseNumber(pError);
        }
        return Fail("unexpected token in JSON", pError);
    }

    bool Finished() {
        SkipWhitespace();
        return AtEnd();
    }

private:
    std::string_view mText;
    std::size_t mCursor = 0U;

    bool AtEnd() const {
        return mCursor >= mText.size();
    }

    char Peek() const {
        return AtEnd() ? '\0' : mText[mCursor];
    }

    char Take() {
        return AtEnd() ? '\0' : mText[mCursor++];
    }

    void SkipWhitespace() {
        while (!AtEnd() && std::isspace(static_cast<unsigned char>(Peek())) != 0) {
            ++mCursor;
        }
    }

    std::optional<JsonValue> Fail(const std::string &pMessage,
                                  std::string *pError) {
        if (pError != NULL) {
            *pError = pMessage;
        }
        return std::nullopt;
    }

    std::optional<JsonValue> ParseNull(std::string *pError) {
        if (mText.substr(mCursor, 4U) != "null") {
            return Fail("invalid null literal", pError);
        }
        mCursor += 4U;
        return JsonValue::Null();
    }

    std::optional<JsonValue> ParseBool(std::string *pError) {
        if (mText.substr(mCursor, 4U) == "true") {
            mCursor += 4U;
            return JsonValue::Bool(true);
        }
        if (mText.substr(mCursor, 5U) == "false") {
            mCursor += 5U;
            return JsonValue::Bool(false);
        }
        return Fail("invalid bool literal", pError);
    }

    std::optional<JsonValue> ParseString(std::string *pError) {
        if (Take() != '"') {
            return Fail("expected string quote", pError);
        }

        std::string aValue;
        while (!AtEnd()) {
            const char aCh = Take();
            if (aCh == '"') {
                return JsonValue::String(std::move(aValue));
            }
            if (aCh != '\\') {
                aValue.push_back(aCh);
                continue;
            }

            if (AtEnd()) {
                return Fail("unterminated escape", pError);
            }
            const char aEscaped = Take();
            switch (aEscaped) {
                case '"': aValue.push_back('"'); break;
                case '\\': aValue.push_back('\\'); break;
                case '/': aValue.push_back('/'); break;
                case 'b': aValue.push_back('\b'); break;
                case 'f': aValue.push_back('\f'); break;
                case 'n': aValue.push_back('\n'); break;
                case 'r': aValue.push_back('\r'); break;
                case 't': aValue.push_back('\t'); break;
                default: return Fail("unsupported escape sequence", pError);
            }
        }

        return Fail("unterminated string", pError);
    }

    std::optional<JsonValue> ParseNumber(std::string *pError) {
        const std::size_t aStart = mCursor;
        if (Peek() == '-') {
            ++mCursor;
        }
        while (!AtEnd() && std::isdigit(static_cast<unsigned char>(Peek())) != 0) {
            ++mCursor;
        }
        if (!AtEnd() && Peek() == '.') {
            ++mCursor;
            while (!AtEnd() && std::isdigit(static_cast<unsigned char>(Peek())) != 0) {
                ++mCursor;
            }
        }
        if (!AtEnd() && (Peek() == 'e' || Peek() == 'E')) {
            ++mCursor;
            if (!AtEnd() && (Peek() == '+' || Peek() == '-')) {
                ++mCursor;
            }
            while (!AtEnd() && std::isdigit(static_cast<unsigned char>(Peek())) != 0) {
                ++mCursor;
            }
        }

        const std::string aToken(mText.substr(aStart, mCursor - aStart));
        char *aEndPtr = NULL;
        const double aValue = std::strtod(aToken.c_str(), &aEndPtr);
        if ((aEndPtr == NULL) || (*aEndPtr != '\0')) {
            return Fail("invalid number", pError);
        }
        return JsonValue::Number(aValue);
    }

    std::optional<JsonValue> ParseArray(std::string *pError) {
        if (Take() != '[') {
            return Fail("expected '['", pError);
        }

        JsonValue::Array aValues;
        SkipWhitespace();
        if (!AtEnd() && Peek() == ']') {
            Take();
            return JsonValue::ArrayValue(std::move(aValues));
        }

        while (true) {
            auto aValue = ParseValue(pError);
            if (!aValue.has_value()) {
                return std::nullopt;
            }
            aValues.push_back(std::move(*aValue));
            SkipWhitespace();
            if (AtEnd()) {
                return Fail("unterminated array", pError);
            }
            const char aSeparator = Take();
            if (aSeparator == ']') {
                break;
            }
            if (aSeparator != ',') {
                return Fail("expected ',' or ']'", pError);
            }
            SkipWhitespace();
        }
        return JsonValue::ArrayValue(std::move(aValues));
    }

    std::optional<JsonValue> ParseObject(std::string *pError) {
        if (Take() != '{') {
            return Fail("expected '{'", pError);
        }

        JsonValue::Object aFields;
        SkipWhitespace();
        if (!AtEnd() && Peek() == '}') {
            Take();
            return JsonValue::ObjectValue(std::move(aFields));
        }

        while (true) {
            auto aKey = ParseString(pError);
            if (!aKey.has_value() || !aKey->is_string()) {
                return Fail("object key must be a string", pError);
            }
            SkipWhitespace();
            if (AtEnd() || (Take() != ':')) {
                return Fail("expected ':' after object key", pError);
            }
            SkipWhitespace();
            auto aValue = ParseValue(pError);
            if (!aValue.has_value()) {
                return std::nullopt;
            }
            aFields[aKey->as_string()] = std::move(*aValue);
            SkipWhitespace();
            if (AtEnd()) {
                return Fail("unterminated object", pError);
            }
            const char aSeparator = Take();
            if (aSeparator == '}') {
                break;
            }
            if (aSeparator != ',') {
                return Fail("expected ',' or '}'", pError);
            }
            SkipWhitespace();
        }

        return JsonValue::ObjectValue(std::move(aFields));
    }
};

std::string EscapeString(const std::string &pValue) {
    std::string aEscaped;
    aEscaped.reserve(pValue.size() + 8U);
    for (char aCh : pValue) {
        switch (aCh) {
            case '"': aEscaped += "\\\""; break;
            case '\\': aEscaped += "\\\\"; break;
            case '\n': aEscaped += "\\n"; break;
            case '\r': aEscaped += "\\r"; break;
            case '\t': aEscaped += "\\t"; break;
            default: aEscaped.push_back(aCh); break;
        }
    }
    return aEscaped;
}

} // namespace

JsonValue::JsonValue() = default;
JsonValue::JsonValue(bool pValue) : mType(Type::kBool), mBoolValue(pValue) {}
JsonValue::JsonValue(double pValue) : mType(Type::kNumber), mNumberValue(pValue) {}
JsonValue::JsonValue(std::string pValue) : mType(Type::kString), mStringValue(std::move(pValue)) {}
JsonValue::JsonValue(Array pValue) : mType(Type::kArray), mArrayValue(std::move(pValue)) {}
JsonValue::JsonValue(Object pValue) : mType(Type::kObject), mObjectValue(std::move(pValue)) {}

JsonValue JsonValue::Null() { return JsonValue(); }
JsonValue JsonValue::Bool(bool pValue) { return JsonValue(pValue); }
JsonValue JsonValue::Number(double pValue) { return JsonValue(pValue); }
JsonValue JsonValue::String(std::string pValue) { return JsonValue(std::move(pValue)); }
JsonValue JsonValue::ArrayValue(Array pValue) { return JsonValue(std::move(pValue)); }
JsonValue JsonValue::ObjectValue(Object pValue) { return JsonValue(std::move(pValue)); }

bool JsonValue::as_bool(bool pFallback) const {
    return mType == Type::kBool ? mBoolValue : pFallback;
}

double JsonValue::as_number(double pFallback) const {
    return mType == Type::kNumber ? mNumberValue : pFallback;
}

std::string JsonValue::as_string(const std::string &pFallback) const {
    return mType == Type::kString ? mStringValue : pFallback;
}

const JsonValue::Array& JsonValue::as_array() const {
    static const Array kEmpty{};
    return mType == Type::kArray ? mArrayValue : kEmpty;
}

const JsonValue::Object& JsonValue::as_object() const {
    static const Object kEmpty{};
    return mType == Type::kObject ? mObjectValue : kEmpty;
}

JsonValue::Array& JsonValue::as_array_mut() {
    if (mType != Type::kArray) {
        mType = Type::kArray;
        mArrayValue.clear();
    }
    return mArrayValue;
}

JsonValue::Object& JsonValue::as_object_mut() {
    if (mType != Type::kObject) {
        mType = Type::kObject;
        mObjectValue.clear();
    }
    return mObjectValue;
}

const JsonValue* JsonValue::find(const std::string& pKey) const {
    if (mType != Type::kObject) {
        return NULL;
    }
    const auto aIterator = mObjectValue.find(pKey);
    if (aIterator == mObjectValue.end()) {
        return NULL;
    }
    return &(aIterator->second);
}

std::string JsonValue::Serialize() const {
    switch (mType) {
        case Type::kNull:
            return "null";
        case Type::kBool:
            return mBoolValue ? "true" : "false";
        case Type::kNumber: {
            std::ostringstream aStream;
            aStream.precision(17);
            aStream << mNumberValue;
            return aStream.str();
        }
        case Type::kString:
            return std::string("\"") + EscapeString(mStringValue) + "\"";
        case Type::kArray: {
            std::string aResult = "[";
            for (std::size_t aIndex = 0U; aIndex < mArrayValue.size(); ++aIndex) {
                if (aIndex > 0U) {
                    aResult += ",";
                }
                aResult += mArrayValue[aIndex].Serialize();
            }
            aResult += "]";
            return aResult;
        }
        case Type::kObject: {
            std::string aResult = "{";
            bool aFirst = true;
            for (const auto &aPair : mObjectValue) {
                if (!aFirst) {
                    aResult += ",";
                }
                aFirst = false;
                aResult += std::string("\"") + EscapeString(aPair.first) + "\":" + aPair.second.Serialize();
            }
            aResult += "}";
            return aResult;
        }
    }
    return "null";
}

std::optional<JsonValue> JsonValue::Parse(const std::string& pText,
                                          std::string *pError) {
    Parser aParser(pText);
    auto aValue = aParser.ParseValue(pError);
    if (!aValue.has_value()) {
        return std::nullopt;
    }
    if (!aParser.Finished()) {
        if (pError != NULL) {
            *pError = "unexpected trailing JSON content";
        }
        return std::nullopt;
    }
    return aValue;
}

} // namespace MeanMachine_json
