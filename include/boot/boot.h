#pragma once
#include <kfh/stdint.h>

struct st_tag {
    kfh::u64 ident;
    st_tag* next;
};

/// System info passed by the bootloader to the kernel.
/// Currently, Adheres to stivale2's stivale2_struct
///
/// TODO:
//  Make it generic so can be used for other archs.
struct boot_info {
    char bootloader_brand[64];
    char bootloader_version[64];
    st_tag* tags;

    /// Get a tag by id.
    //
    /// Returns:
    /// Pointer to `T` if it exists, else nullptr.
    template <typename T>
    T* get_tag(kfh::usize ID)
    {
        st_tag* tmp = tags;
        while (tmp != nullptr) {
            if (tmp->ident == ID) {
                return static_cast<T*>((void*)tmp);
            }
            tmp = tmp->next;
        }
        return nullptr;
    }
};

struct term_t {
    using term_func = void (*)(const char*, kfh::usize);
    st_tag tag;
    kfh::u32 flags;
    kfh::u16 cols;
    kfh::u16 rows;
    term_func term_write;
    kfh::u64 max_length;
};
