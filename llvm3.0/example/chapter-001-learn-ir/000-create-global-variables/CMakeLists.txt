cmake_minimum_required(VERSION 3.0)

project(create_global_var)

# configure cmake command
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set exec
add_executable(main main.cpp)

# Link pthread
find_package(Threads REQUIRED)
target_link_libraries(main Threads::Threads)

# Find and link LLVM 3.0 using llvm-config
execute_process(COMMAND llvm-config --cxxflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config --libs OUTPUT_VARIABLE LLVM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LLVM_LD_FLAGS}")

target_link_libraries(main ${LLVM_LIBS})

# Link rt
find_library(RT_LIBRARY rt)
if(RT_LIBRARY)
  target_link_libraries(main ${RT_LIBRARY})
endif()

# Link dl
find_library(DL_LIBRARY dl)
if(DL_LIBRARY)
  target_link_libraries(main ${DL_LIBRARY})
endif()