#pragma once

#include "../music/core.h"
#include <memory>
#include <thread>

#define prompt "> "
#define act_max 7


class input_core {
    std::unique_ptr<music_core> m_core;
    std::thread t;

public:
    input_core(std::unique_ptr<music_core> m_core);
    ~input_core(void);

    void start(void);

private:
    void input_thread(void);
};
