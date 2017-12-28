# Multithreading Objects
> Defined in: `lang/thread.hpp`


----------

Lang contains two thread objects: `safe_thread` and `mutating_thread` which wrap around the `pthread` library to provide a convenient way to create threads that capture variables from their surroundings.

As their names suggest, variables captured by `safe_thread` are readonly, where as variables captured by `mutating_thread` are writable.

> Because `mutating_thread` can modify its captured variables, there is a possibility that errors may occur if multiple threads are trying to modify values at the same time. As such, use of `safe_thread` is recommended.

----------

### Declarations
#### Threads
```c++
// namespace lang

template<class... CaptureObjects>
using safe_thread = __1::__thread_t<weak_capture, CaptureObjects...>;

template<class... CaptureObjects>
using mutating_thread = __1::__thread_t<mutable_capture, CaptureObjects...>;
```

#### Capture Types
```c++
// namespace lang
// defined in lang/memory.hpp, included by lang/thread.hpp

template<class T>
using weak_capture = __1::__ref_weak_t<T>;

template<class T>
using mutable_capture = __1::__ref_mutable_t<T>;
```

Capture types are essentially abstractions of pointers with `weak_capture<T>` corresponding to `const T*` and `mutable_capture<T>` simply being `T*`. These objects are **not** smart pointers as they will not destroy or deallocate whatever they point to.

Each object overloads `operator*` and `operator->` to provide basic pointer functionality. A `get()` method is also available, which returns the underlying pointer.

----------
## Thread Object Specifications
### Constructors

`__1::__thread_t`, the generic class that both thread types specialize, has a trivial default and copy constructor along with one explicit constructor as seen below.

```c++
template<template<class> class CaptureType, class... CaptureObjects>
explicit __thread_t(void(*func)(CaptureType<CaptureObjects>...));
```
This constructor takes a lambda or function pointer with a return type of `void` and a given parameter set wrapped in a specific capture type. The `CaptureType` depends on the thread type: with `safe_thread` having `weak_capture` and `mutating_thread` having `mutable_capture`. This function what will be called when creating the thread. There is no need to use `pthread_exit` at the end of the function as the thread object will take care of that.

The parameters for the function correspond to the captured variables which are retrieved using the `capture()` method (see below). As each variable is wrapped in a capture object, they must be dereferenced using `operator*` or `operator->` just like a pointer. `weak_capture<T>` will dereference to `const T&`, where as, `mutable_capture` will dereference to `T&`.

The following would be a valid use of this constructor:

```c++
void foo(lang::weak_capture<int> x, lang::weak_capture<double> d) {}
lang::safe_thread<int, double> thread{foo};
```

An `auto` lambda may also be used like so:

```c++
lang::safe_thread<int, double> thread{[](auto x, auto d){});
```

----------


### Methods

`void set_func(void(*func)(CaptureType<CaptureObjects>...);`

Does the same job as the explicit constructor in defining the function to be called when the thread is started.

----------

`void capture(CaptureObjects *... ptrs);`

Captures surrounding variables as pointers to be used in the thread. 

Parameter pointers must be pointers to lvalues that will exist for the duration of the thread execution time. This means that they should be pointers to static variables or objects on the free store. Passing pointers to variables on the stack or rvalues will result in undefined behaviour.

----------

`void start();`

Creates the underlying thread object which starts running the thread.

> This method should only be called once per thread.

----------

`void join();`

Joins the calling thread and the thread object. The calling thread will wait until the object's thread has finished before advancing.

Provides the same functionality as `pthread_join`.

----------

`void kill();`

Kills the objects thread, forcing it to stop executing. Use of this method is not recommended unless it is absolutely necessary.

Provides the same functionality as `pthread_kill`.

----------

### Non-Member Functions

```c++
template<class... CaptureObjects>
weak_thread<CaptureObjects...> create_thread
	(typename weak_thread<CaptureObjects...>::lambda func, CaptureObjects *... params);
```

```c++
template<class... CaptureObjects>
mutating_thread<CaptureObjects...> create_mutating_thread
	(typename mutating_thread<CaptureObjects...>::lambda func, CaptureObjects *... params);
```

Creates a thread using a lambda/function pointer and a set of capture objects and returns said thread.

This is a helper function that can create a thread without having to explicitly specify the template parameters. When used with an `auto` lambda (see example below) it allows for easy, simplistic thread creation.

----------

### Example

A simple example of  `mutating_thread` which captures 3 integer values, summing up the first two and storing the value in the third.

```c++
#include <lang/thread.hpp>

int main() {
	int x = 1;
	int y = 2;
	int z = 0;
	
	// Create the thread
	auto thread = lang::create_mutating_thread([](auto _x, auto _y, auto _z){
		*_z = *_x + *_y;
	}, &x, &y, &z);
	
	thread.start();
	thread.join();
	printf("%d + %d = %d", x, y, z); // prints 1 + 2 = 3
}
```

Without the `auto` declarations, the values types would be like so:

 - `thread` = `lang::mutating_thread<int, int, int>`
 - `_x, _y, _z` = `lang::mutable_capture<int>`

Notice how each capture object (`_x`, etc...) must be dereferenced before anything can be done to it and how pointers to the variables `x, y, z` are passed into the function as pointers to be captured. The order in which they are passed corresponds to the parameters in the lambda expression.

> Written with [StackEdit](https://stackedit.io/).
