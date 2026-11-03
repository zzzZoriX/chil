#pragma once

#include "colors.h"
#include <exception>
#include <string>
#include <iostream>


class warn: std::exception {
    std::string message;

public:
    warn(const std::string& warn)
    :   message(warn) {}

    const char* what(void) const noexcept override {
        return message.c_str();
    }

    static void fear(warn&& w) {
        std::cout << yellow_b_fg << w.what() << reset_fg << std::endl;
    }
};
