# lang-lib
A collection of objects and functions which extend the C++ standard library. 

There is no real direct purpose for this library. It's just a collection of experimental code that may or may not be useful later on.

The library is designed so that, if using C++17, it is not necessary to link with a compiled archive.
This is mainly for simplicity's sake as I don't wish to worry about linking with external libraries when creating short files.

To force all methods, functions, and variables inline (C++17 only) define the following macro at the top of any source file using which is including the library headers.

```c++
#define __LANG_FORCE_INLINE
```

## Language Specifications
* C++17 with C++14 support
