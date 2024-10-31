#include "JSON.h"

std::unique_ptr<Type::IntType> JSON::IntElem::type = std::make_unique<Type::IntType>();

std::unique_ptr<Type::StringType> JSON::StringElem::type = std::make_unique<Type::StringType>();

void JSON::MapElem::addNewEntry(const std::string &name, std::unique_ptr<JsonElement> &&element) {
    json_map[name] = std::move(element);
}

std::unique_ptr<JSON::JsonElement> *JSON::MapElem::get_item(const std::string &name) {
    if (json_map.find(name) == json_map.end())
        return nullptr;
    return &json_map.find(name)->second;
}

std::unique_ptr<Type::MapType> JSON::MapElem::type = std::make_unique<Type::MapType>();

void JSON::ArrayElem::addNewEntry(std::unique_ptr<JsonElement> &&element) {
    json_vector.emplace_back(std::move(element));
}

std::unique_ptr<Type::ArrayType> JSON::ArrayElem::type = std::make_unique<Type::ArrayType>();