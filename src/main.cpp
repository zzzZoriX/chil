#include "include/output/errors.h"


int main(int argc, char** argv) {
    try {
        if(argc < 2)
            throw error("Too few arguments. Example of correct usage:\n> chil info", error::code::atferr);      

        std::cout << green_fg << "success" << reset_fg << std::endl;
    }
    catch (const error& e) {
        error::panic(e);
    }

    return 0;
}
