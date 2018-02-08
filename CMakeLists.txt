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
        lang/list.hpp
        lang/queue.hpp
        lang/stack.hpp
        lang/random.hpp
        lang/io.hpp
        lang/foundation/__conditional_keyword.hpp
        lang/src/tracer.tcc
        lang/cluster.hpp
        lang/io/basic_io.hpp
        lang/io/basic_io.tcc
        lang/io/ioctl.hpp
        lang/io/ioctl.tcc
        lang/temporal.hpp
        lang/memory.hpp
        lang/foundation/__ref_types.hpp
        lang/lang.hpp lang/thread.hpp
        lang/foundation/__thread_t.hpp
        lang/io/keypad.hpp
        lang/io/keypad.tcc
        lang/misc.hpp
        lang/function.hpp
        lang/int_seq.hpp
        lang/subprocess.hpp
        lang/src/subprocess.tcc
        lang/sys.hpp
        lang/src/sys.tcc
        lang/parsing.hpp
        lang/src/parsing.tcc
        lang/iterator.hpp
        lang/operand.hpp lang/seq_types/basic_sequence.hpp lang/memory/object_traits.hpp lang/memory/allocator_traits.hpp lang/memory/heap.hpp lang/seq_types/array.hpp lang/seq_types/sequence.hpp lang/seq_types/list.hpp lang/memory/static_pool.hpp lang/singleton.hpp lang/memory/pool.hpp lang/seq.hpp lang/memory/hybrid.hpp lang/comparison.hpp lang/heap.hpp lang/print.hpp lang/algorithm.hpp lang/priority_queue.hpp)

add_executable(test_exe test.cpp lang/memory/allocator.hpp)
target_link_libraries(test_exe lang_lib sqlite3)
