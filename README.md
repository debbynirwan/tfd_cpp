# tfd_cpp
Total-order Forward Decomposition Algorithm: an HTN Planner library.

This library provides the TFD algorithm - an HTN Planner so that developers can focus on writing Planning Domain and Planning Problem for their applications.

## Book and Python Implementation
The implementation of this algorithm is based on Total-order STN Planning in "Automated Planning: Theory & Practice" [book](https://www.amazon.sg/Automated-Planning-Practice-Malik-Ghallab/dp/1558608567).

This library is a C++17 version of [Pyhop](https://bitbucket.org/dananau/pyhop/src/master/) implementation.

# Dependencies and Installation
## Dependencies
To build the library you will need to install the dependencies below:
* Compiler that supports C++17
* CMake
* Boost (for logging library)
* GTest (for building unit tests)
## Installation
To compile and install the library, from the root directory, execute:

     mkdir build; cd build
     cmake ..
     make
     sudo make install

# Examples
In the [examples](examples) folder you can see one example that uses the library. The application that uses this library must implement the following:
* Planning Domain which has all methods and operators
* Planning Problem which has the initial state of the world and the task to be solved

To execute the example after building it, you can simply execute:

    ./examples/simple_travel

## Write your own Domain and Problem
You can follow the examples to write your own planning domain and problem.

# Documentation
If you're interested in understanding the concepts and algorithm you can read the blog post here(to be updated).

# Issues
Please report issues if you found bugs or raise a Pull Request.
