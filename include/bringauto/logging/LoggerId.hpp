#pragma once

struct LoggerId {
    const char id[8]   {0};
    const int constant {0};
    //constexpr LoggerId(char id[8], const int constant) : id(id), constant(constant) {}
};