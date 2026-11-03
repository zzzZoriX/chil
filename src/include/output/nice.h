#pragma once

#include "colors.h"
#include <exception>
#include <string>
#include <iostream>


class chil: std::exception {
    std::string message;

public:
    chil(const std::string& nice)
    :   message(nice) {}

    const char* what(void) const noexcept override {
        return message.c_str();
    }

    static void nice(chil&& n) {
        std::cout << green_b_fg << n.what() << reset_fg << std::endl;
    }
};
