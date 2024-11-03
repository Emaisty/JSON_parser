#include <catch.hpp>

#include <vector>

#include "JSON.h"

using namespace JSON;

SCENARIO("JSON integer") {
    for (auto i = 0; i < 10; ++i) {
        auto tmp = rand();
        IntElem integer(tmp);
        REQUIRE(integer.getValue() == tmp);
    }
}

SCENARIO("JSON map") {
    MapElem map;
    std::vector<JsonElement *> elements;
    for (auto i = 0; i < 10; ++i) {
        auto tmp = rand();
        std::unique_ptr<JsonElement> elem;
        switch (tmp % 4) {
            case 0:
                elem = std::make_unique<IntElem>();
                break;
            case 1:
                elem = std::make_unique<StringElem>();
                break;
            case 2:
                elem = std::make_unique<MapElem>();
                break;
            case 3:
                elem = std::make_unique<ArrayElem>();
                break;
        }
        elements.push_back(elem.get());
        map.addNewEntry(std::to_string(i), std::move(elem));
    }
    for (auto i = 0; i < 10; ++i) {
        auto tmp = rand() % 10;
        REQUIRE(map.getItem(std::to_string(tmp))->get() == elements[tmp]);
    }
}

SCENARIO("JSON array") {
    ArrayElem map;
    std::vector<JsonElement *> elements;
    for (auto i = 0; i < 10; ++i) {
        auto tmp = rand();
        std::unique_ptr<JsonElement> elem;
        switch (tmp % 4) {
            case 0:
                elem = std::make_unique<IntElem>();
                break;
            case 1:
                elem = std::make_unique<StringElem>();
                break;
            case 2:
                elem = std::make_unique<MapElem>();
                break;
            case 3:
                elem = std::make_unique<ArrayElem>();
                break;
        }
        elements.push_back(elem.get());
        map.addNewEntry(std::move(elem));
    }
    for (auto i = 0; i < 10; ++i) {
        auto tmp = rand() % 10;
        REQUIRE(map.getElement(tmp)->get() == elements[tmp]);
    }
}