#pragma once

#include "colors.h"
#include <exception>
#include <string>
#include <format>
#include <iostream>
#include <cstdlib>


class error: std::exception {
    std::string message;

public:
    enum class code {
        atferr = 10,
        sdlerr = 20,
        auderr = 21,
        plterr = 30,
    };

    error()
    :   message("noerr") {}
    error(const std::string& err, const code e_code)
    :   message(std::format("eno-{} > {}", static_cast<int>(e_code), err)) {}

    const char* what(void) const noexcept override {
        return message.c_str();
    }

    static void panic(const error& e) {
        std::cout << red_b_fg << e.what() << reset_fg << std::endl;
    }
};
