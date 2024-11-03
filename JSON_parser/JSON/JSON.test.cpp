#include <catch.hpp>

#include <vector>

#include "JSON.h"

using namespace JSON;

SCENARIO("JSON integer") {
    for (auto i = 0; i < 1000; ++i) {
        auto tmp = rand();
        IntElem integer(tmp);
        REQUIRE(integer.getValue() == tmp);
    }
}