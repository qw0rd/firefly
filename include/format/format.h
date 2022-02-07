#pragma once
#include <kfh/stdint.h>
#include <klib/string.h>
#include <concepts>

struct FormatResult {
    /// Pointer to a null terminated string
    const char* _ptr;
    /// The length of the string.
    usize len;
};

enum class NumFormat {
    BIN = 2,
    OCT = 8,
    DEC = 10,
    HEX = 16
};



[[noreturn]] void hang(usize f);

class Formatter;

class TypeFormatter {
    // reference  to the current formatter.
    Formatter& fmt;

public:
    TypeFormatter(Formatter& _f) : fmt{_f} {}

    template <typename T, typename... Args>
    void format(T arg, Args... args) 
    {
        fmt._fmt(arg, args...);
    }
};

template <typename T>
concept Formattable = requires(T obj, TypeFormatter& fmt) {
        obj.format(fmt);
}; 

/*
   Currently, The `Formatter` API works but should seriously consider rewriting it after setting up the inital OS stuff.
    TODO: MAKE THE API NOT SUCK
 */



class Formatter {
    // the maximum size of the buffer.
    static constexpr usize BUF_LEN = 1024;

    // The formatted string will be stored in this buffer.
    // The size of the buffer should be known at compile time.
    char buf[BUF_LEN]{};
    
    inline static char num_buf[65]{};

    // the current index into the buffer.
    usize cursor{};

    NumFormat num_f{NumFormat::DEC};

    static constexpr char* letters = "0123456789abcdef";


    void _fmt(const char* str)
    {
        auto len = strlen(str);
        memcpy(&buf[cursor], str, len);
        cursor += len;
    }

    void _fmt(NumFormat num_fmt)
    {
        num_f = num_fmt;
    }

    void _fmt(const Formattable auto& val)
    {
        TypeFormatter t{*this};
        val.format(t);
    }

    template <typename T> requires std::is_pointer_v<T>
    void _fmt(T a)
    {
        auto cpy = num_f;
        num_f = NumFormat::HEX;
        _fmt((usize)a); 
        num_f = cpy;
    }

    void _fmt(const std::integral auto num)
    {
        // set up our buffer
        auto cur = 64;
        usize num_len{};
        num_buf[cur--] = 0;

        auto tmp = num;
        while (tmp > 0) {
            auto base = static_cast<int>(num_f);
            auto current = tmp % base;
            char c = letters[current % static_cast<int>(base)];
            num_buf[cur--] = c;
            tmp /= base;
            num_len += 1;
        }
    
       memcpy(&buf[cursor], &num_buf[cur + 1], num_len);
       cursor += num_len;
    }


    template <typename T, typename... Args>
    void _fmt(T arg1, Args... args) 
    {
        _fmt(arg1);
        _fmt(args...);
    }

public:
    constexpr Formatter() = default;

    // delete the copy constructor and assignment
    Formatter(const Formatter& fmt) = delete;
    Formatter& operator=(const Formatter& rhs) = delete;

    // delete the move constructor and assignment
    Formatter(const Formatter&& fmt) = delete;
    Formatter& operator=(Formatter&& rhs) = delete;

    template <typename T, typename... Args>
    FormatResult format(T arg1, Args... args)
    {
        _fmt(arg1, args...);
        buf[cursor] = 0;
        auto len = cursor;
        cursor = 0;

        return {
            buf,
            len
        };
    }

    template <typename T, typename... Args>
    void tfmt(T val, Args... a)
    {
        _fmt(val);
        _fmt(a...);

    }

    friend TypeFormatter;
};

