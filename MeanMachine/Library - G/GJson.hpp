//
//  GJson.hpp
//  MeanMachine
//

#ifndef GJson_hpp
#define GJson_hpp

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace MeanMachine_json {

class JsonValue {
public:
    enum class Type {
        kNull,
        kBool,
        kNumber,
        kString,
        kArray,
        kObject,
    };

    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue>;

    JsonValue();
    explicit JsonValue(bool pValue);
    explicit JsonValue(double pValue);
    explicit JsonValue(std::string pValue);
    explicit JsonValue(Array pValue);
    explicit JsonValue(Object pValue);

    static JsonValue Null();
    static JsonValue Bool(bool pValue);
    static JsonValue Number(double pValue);
    static JsonValue String(std::string pValue);
    static JsonValue ArrayValue(Array pValue);
    static JsonValue ObjectValue(Object pValue);

    Type type() const { return mType; }
    bool is_null() const { return mType == Type::kNull; }
    bool is_bool() const { return mType == Type::kBool; }
    bool is_number() const { return mType == Type::kNumber; }
    bool is_string() const { return mType == Type::kString; }
    bool is_array() const { return mType == Type::kArray; }
    bool is_object() const { return mType == Type::kObject; }

    bool as_bool(bool pFallback = false) const;
    double as_number(double pFallback = 0.0) const;
    std::string as_string(const std::string &pFallback = "") const;

    const Array& as_array() const;
    const Object& as_object() const;
    Array& as_array_mut();
    Object& as_object_mut();

    const JsonValue* find(const std::string& pKey) const;

    std::string Serialize() const;
    static std::optional<JsonValue> Parse(const std::string& pText, std::string *pError);

private:
    Type mType = Type::kNull;
    bool mBoolValue = false;
    double mNumberValue = 0.0;
    std::string mStringValue;
    Array mArrayValue;
    Object mObjectValue;
};

} // namespace MeanMachine_json

#endif /* GJson_hpp */
