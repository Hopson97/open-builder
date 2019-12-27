#pragma once

#define NON_COPYABLE(Class)                                                    \
    Class &operator=(const Class &obj) = delete;                               \
    Class(const Class &obj) = delete;

#define NON_MOVEABLE(Class)                                                    \
    Class &operator=(Class &&obj) = delete;                                    \
    Class(Class &&obj) = delete;

// Just some macros to assist with making classes noncopyable etc