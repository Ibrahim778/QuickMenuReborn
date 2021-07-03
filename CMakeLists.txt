cmake_minimum_required(VERSION 2.8.12)

include($ENV{VITASDK}/common.cmake)

project(QuickMenuReborn)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -nostdlib -Wno-write-strings")

file(GLOB pafStubs
    "${CMAKE_SOURCE_DIR}/paf_stubs/build/*.a"
)

file(GLOB appMgrStubs
    "${CMAKE_SOURCE_DIR}/appmgr_stubs/build/*.a"
)

file(GLOB cppfiles
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

file(GLOB cfiles
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

file(GLOB hfiles
    "${CMAKE_SOURCE_DIR}/src/*.h"
)


add_executable(${PROJECT_NAME}
    ${cppfiles}
    ${cfiles}
    ${hfiles}
)

target_link_libraries(${PROJECT_NAME}
    taihen_stub
    ScePaf_stub 
    ${pafStubs}
    ${appMgrStubs}
    SceLibKernel_stub
    SceKernelThreadMgr_stub
)

vita_create_self(${PROJECT_NAME}.suprx ${PROJECT_NAME} CONFIG exports.yml UNSAFE)
vita_create_stubs(stubs ${PROJECT_NAME} exports.yml)

install(DIRECTORY ${CMAKE_BINARY_DIR}/stubs/
  DESTINATION lib
  FILES_MATCHING PATTERN "*.a"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/quickmenureborn/
  DESTINATION include/quickmenureborn
  FILES_MATCHING PATTERN "*.h"
)