cmake_minimum_required(VERSION 3.9)
project(lang_lib)

set(CMAKE_CXX_STANDARD 17)

add_library(lang_lib
        lang/io/ansi_t.hpp
        lang/any.hpp
        lang/attribute.hpp
        lang/foundation/__base.hpp
        lang/foundation/__dym_array.hpp
        lang/matrix.hpp
        lang/optional.hpp
        lang/point.hpp
        lang/tsafe_attr.hpp
        lang/vector.hpp
        lang/src/lang_src.cpp
        lang/tracer.hpp
        lang/memory_image.hpp
        lang/array.hpp
        lang/queue.hpp
        lang/stack.hpp
        lang/random.hpp
        lang/io.hpp
        lang/iterator.hpp
        lang/foundation/__conditional_keyword.hpp
        lang/src/tracer.tcc
        lang/cluster.hpp
        lang/io/basic_io.hpp
        lang/io/basic_io.tcc
        lang/io/key_logger.hpp
        lang/io/key_logger.tcc
        lang/io/ioctl.hpp
        lang/io/ioctl.tcc
        lang/temporal.hpp
        lang/memory.hpp
        lang/foundation/__ref_types.hpp
        lang/foundation/__basic_capture_set.hpp)

add_executable(test_exe test.cpp)
target_link_libraries(test_exe lang_lib)
