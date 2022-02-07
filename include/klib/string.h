#pragma once
#include <kfh/stdint.h>

extern "C" {
    void* memcpy(void* dest, const void* str, usize n);

    usize strlen(const char *s);
}


