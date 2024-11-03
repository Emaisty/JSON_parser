#include <catch.hpp>

#include <list>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>

#include "parser.h"

using namespace JSON;

SCENARIO("Parser fails on wrong file") {
    Parser parser;
    REQUIRE(parser.setFileInput("test_not_exists_114567.jij") == false);
}

SCENARIO("Parser can open a file") {
    Parser parser;
    std::ofstream outfile("test_file.txt");
    outfile.close();
    REQUIRE(parser.setFileInput("test_file.txt") == true);
}

SCENARIO("Parser. Correct cases") {
    std::list<std::string> tests = {
            "{}",
            "[1,3]\n",
            "[1,2,3,\"a\",\"b\",\"g\"]\n",
            "{\"a\" : \"b\",\"c\" : \"d\",\"r\" : \"b\"}",
            "{\"a\" : [\"a1\",1,4,[1,3,{\"a\":6}]],\"c\" : {\"a\":{\"a\": 5}},\"r\" : 1}",
    };


    for (auto &i: tests) {
        Parser parser;
        std::ofstream outfile("test_input.txt");
        outfile.clear();
        outfile << i;
        outfile.close();
        REQUIRE(parser.setFileInput("test_input.txt") == true);
        REQUIRE_NOTHROW(parser.parse());
    }
}