#pragma once

#include <cstdint>
#include <cstddef>

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using usize = std::uintptr_t;

namespace kfh {

using ::u8; 
using ::u16; 
using ::u32; 
using ::u64;

using ::i8; 
using ::i16;
using ::i32; 
using ::i64; 

using ::usize; 

// INFO:
// From what ik, `uintptr_t` and `size_t` will be 8 bytes, 
// atleast on x86_64. and most other archs ik. 
// If it is not, let's just not compile the code.
static_assert(sizeof(usize) == sizeof(std::size_t), "This is a weird case. Please open a github issue with info of which architecture you are compiling.");

};


