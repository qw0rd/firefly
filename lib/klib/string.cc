
#include <klib/string.h>


[[noreturn]] void hang(usize f);

extern "C" void* memcpy(void* dest, const void* src, usize n)
{
    u8* _dest = static_cast<u8*>(dest);
    const u8* _src = static_cast<const u8*>(src);
    
    while (n--)
        *_dest++ = *_src++;

    return dest;
}


extern "C" usize strlen(const char *s)
{
    usize len{};
    while (*s++)
        len++;

    return len;
}
