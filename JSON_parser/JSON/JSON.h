#ifndef JSON_PARSER_JSON_H
#define JSON_PARSER_JSON_H

#include <string>
#include <vector>
#include <map>

#include "types.h"

namespace JSON {

    class JsonElement {
    public:
        JsonElement() = default;

        virtual ~JsonElement() = default;

    protected:

    };

    class IntElem : public JsonElement{
    public:
        IntElem() = default;

        IntElem(long long new_value) : value(new_value) {}

    private:
        long long value = 0;

        static std::unique_ptr<Type::IntType> type;

    };

    class StringElem : public JsonElement{
    public:
        StringElem() = default;

        StringElem(std::string&& new_name) : name(std::move(new_name)) {}

    private:
        std::string name;

        static std::unique_ptr<Type::StringType> type;

    };

    class MapElem : public JsonElement{
    public:
        MapElem() = default;

        void addNewEntry(const std::string&, std::unique_ptr<JsonElement>&&);

    private:
        std::map<std::string,std::unique_ptr<JsonElement>> json_map;

        static std::unique_ptr<Type::MapType> type;

    };

    class ArrayElem : public JsonElement{
    public:
        ArrayElem() = default;

        void addNewEntry(std::unique_ptr<JsonElement>&&);

    private:
        std::vector<std::unique_ptr<JsonElement>> json_vector;

        static std::unique_ptr<Type::ArrayType> type;

    };

}

#endif //JSON_PARSER_JSON_H
