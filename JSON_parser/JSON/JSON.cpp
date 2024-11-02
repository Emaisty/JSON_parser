#include "JSON.h"

void JSON::IntElem::print(std::ostream &oss) const {
    oss << value;
}

void JSON::StringElem::print(std::ostream &oss) const {
    oss << name;
}

std::function<std::unique_ptr<JSON::JsonElement> *(std::vector<std::unique_ptr<JSON::JsonElement> *>,
                                                   JSON::Context &)> *
JSON::FuncElem::getBody() {
    return &fBody;
}

void JSON::FuncElem::print(std::ostream &oss) const {
    throw std::invalid_argument("ERROR. Cannot print function");
}

void JSON::MapElem::addNewEntry(const std::string &name, std::unique_ptr<JsonElement> &&element) {
    json_map[name] = std::move(element);
}

std::unique_ptr<JSON::JsonElement> *JSON::MapElem::getItem(const std::string &name) {
    if (json_map.find(name) == json_map.end())
        return nullptr;
    return &json_map.find(name)->second;
}

long long JSON::MapElem::countElements() const {
    return json_map.size();
}

const std::map<std::string, std::unique_ptr<JSON::JsonElement>> *JSON::MapElem::getObject() const {
    return &json_map;
}

void JSON::MapElem::print(std::ostream &oss) const {
    oss << "{ ";
    for (auto i = json_map.begin(); i != json_map.end(); ++i) {
        oss << i->first << " : ";
        i->second->print(oss);
        if (std::distance(i, json_map.end()) != 1)
            oss << ", ";
    }
    oss << "}";
}

void JSON::ArrayElem::addNewEntry(std::unique_ptr<JsonElement> &&element) {
    json_vector.emplace_back(std::move(element));
}

std::unique_ptr<JSON::JsonElement> *JSON::ArrayElem::getElement(long long elem) {
    if (elem < 0 || elem >= json_vector.size())
        return nullptr;
    return &json_vector[elem];
}

long long JSON::ArrayElem::countElements() const {
    return json_vector.size();
}

const std::vector<std::unique_ptr<JSON::JsonElement>> *JSON::ArrayElem::getArray() const {
    return &json_vector;
}

void JSON::ArrayElem::print(std::ostream &oss) const {
    oss << "[ ";
    for (auto i = 0; i < json_vector.size(); ++i) {
        json_vector[i]->print(oss);
        if (json_vector.size() - i != 1)
            oss << ", ";
    }
    oss << "]";
}

JSON::Context::Context() {
    for (auto &i: init_funcs) {
        auto name = i.first;
        functions[name] = std::make_unique<JSON::FuncElem>(std::move(i.first), std::move(i.second));
    }

}

std::unique_ptr<JSON::JsonElement> *JSON::Context::addNewNumberElem(std::unique_ptr<JSON::JsonElement> &&new_elem) {
    numberElements.push_back(std::move(new_elem));
    return &numberElements.back();
}