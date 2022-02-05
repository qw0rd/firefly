#include <boot/stivale2.h>
#include <boot/boot.h>
#include <cstdint>
#include <kfh/stdint.h>

constexpr usize STACK_SIZE = 4 * 1024;

// 16 KiB stack for our kernel
static u8 STACK[STACK_SIZE];

using st_hdr = stivale::stivale2_header;
using st_tag_fb = stivale::stivale2_header_tag_framebuffer;
using st_tag_term = stivale::stivale2_header_tag_terminal;
using st_info = stivale::stivale2_struct;
using st_term = stivale::stivale2_struct_tag_terminal;

static st_tag_term _TERM = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0
    },
    .flags = 0
};


static st_tag_fb _FB = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (usize)&_TERM
    },
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0,
};


[[gnu::section(".stivale2hdr")]] static st_hdr hdr{
    .entry_point = 0,
    .stack = (usize)&STACK[STACK_SIZE - 1],
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    .tags = (usize)&_FB
};


usize strlen(const char* s)
{
    usize len{};
    while (*s++)
        len++;
    return len;
}


extern "C" [[noreturn]] void _start(boot_info& info)
{
    auto* term = info.get_tag<term_t>(STIVALE2_STRUCT_TAG_TERMINAL_ID);
    if (!term) {
        while (1){ 
            asm("mov $0xbeef, %rax");
        }
    }

    const char* msg = "Slow dancing in a burning room";
    term->term_write(msg, strlen(msg));

    while (1) {
        asm("mov $0x1234, %rax");
    }
}
