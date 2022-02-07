#include <boot/stivale2.h>
#include <boot/boot.h>
#include <cstdint>
#include <kfh/stdint.h>
#include <format/format.h>

constexpr usize STACK_SIZE = 4 * 1024;

// 16 KiB stack for our kernel
static u8 STACK[STACK_SIZE];

using st_hdr = stivale::stivale2_header;
using st_tag_fb = stivale::stivale2_header_tag_framebuffer;
using st_tag_term = stivale::stivale2_header_tag_terminal;
using st_info = stivale::stivale2_struct;
using st_term = stivale::stivale2_struct_tag_terminal;
using st_smp = stivale::stivale2_header_tag_smp;

static st_smp _SMP = {
    .tag = {
        .identifier = 0x1ab015085f3273df,
        .next = 0,
    },
    .flags = (1 << 0) | (1 << 1)
};

static st_tag_term _TERM = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = (usize)&_SMP
    },
    .flags = 0,
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


[[gnu::section(".stivale2hdr"), gnu::used]] static st_hdr hdr{
    .entry_point = 0,
    .stack = (usize)&STACK[STACK_SIZE],
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    .tags = (usize)&_FB
};


[[noreturn]] void hang(usize f = 0xdeed)
{
    while (1) {
        asm(R"(
            mov %0, %%rax
            hlt
        )" :: "r"(f));
    }
}

using sti_uart = stivale::stivale2_struct_tag_mmio32_uart;

term_t::term_func _printer = nullptr;

template <typename T, typename... Args>
void print(T arg1, Args... args)
{
    constinit static Formatter f;
    //Formatter f;
    auto [str, len] = f.format(arg1, args...);
    if (_printer) {
        _printer(str, len);
        _printer("\n", 1);
    }
}

using firmware_t = stivale::stivale2_struct_tag_firmware;

extern "C" [[noreturn]] void _start(boot_info& info)
{
    auto* term = info.get_tag<term_t>(STIVALE2_STRUCT_TAG_TERMINAL_ID);
    if (!term) {
        hang(0x2222);
    }

    _printer = term->term_write;
    print("FireflyOS v1");

    // get firmware info i.e wether we got booted by uefi or bios
    auto* firmware = info.get_tag<firmware_t>(0x359d837855e3858c);
    if (!firmware) hang(0x1111);

    const auto* firm_str = firmware->flags ? "BIOS"  : "UEFI";
    print("Firmware: ", firm_str);

    // search for an UART
    auto *uart = info.get_tag<sti_uart>(0xb813f9b8dbc78797);
    if (uart) { 
        print("UART found at: 0x", NumFormat::HEX, uart->addr);
    } else {
        print("No UART found");
    }

    using mem_info = stivale::stivale2_struct_tag_memmap;
    auto* entries = info.get_tag<mem_info>(0x2187f79e8612de07);
    if (!entries) {
        print("No entries found\n");
    } else {
        print("Memory entries found: ", entries->entries);
    }

     using smp_t = stivale::stivale2_struct_tag_smp;
     auto* smp = info.get_tag<smp_t>(0x34d1d96339647025);
     if (smp)  {
        print("CPU core count: ", NumFormat::DEC, smp->cpu_count);
     }
    
     int a = 10;
     print("Addr of a: 0x", &a);

     print("Halting");

    while (1) {
        asm("mov $0x1234, %rax");
    }
}
