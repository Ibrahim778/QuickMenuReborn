cmake_minimum_required(VERSION 2.8.12)

include($ENV{VITASDK}/common.cmake)
#User Plugin
project(QuickMenuReborn)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -nostdlib -Wno-write-strings")

file(GLOB pafStubs
    "${CMAKE_SOURCE_DIR}/paf_stubs/build/*.a"
)

file(GLOB appMgrStubs
    "${CMAKE_SOURCE_DIR}/appmgr_stubs/build/*.a"
)

file(GLOB user_cppfiles
    "${CMAKE_SOURCE_DIR}/src/user/*.cpp"
)

file(GLOB user_cfiles
    "${CMAKE_SOURCE_DIR}/src/user/*.c"
)

file(GLOB user_hfiles
    "${CMAKE_SOURCE_DIR}/src/user/*.h"
)


add_executable(${PROJECT_NAME}
    ${user_cppfiles}
    ${user_cfiles}
    ${user_hfiles}
)

target_link_libraries(${PROJECT_NAME}
    taihen_stub
    ScePaf_stub 
    ${pafStubs}
    ${appMgrStubs}
    SceAppMgr_stub
    SceLibKernel_stub
    SceKernelThreadMgr_stub
)

vita_create_self(${PROJECT_NAME}.suprx ${PROJECT_NAME} CONFIG exports.yml UNSAFE)
vita_create_stubs(user_stubs ${PROJECT_NAME} exports.yml)

install(DIRECTORY ${CMAKE_BINARY_DIR}/user_stubs/
  DESTINATION lib
  FILES_MATCHING PATTERN "*.a"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/user/quickmenureborn/
  DESTINATION include/quickmenureborn
  FILES_MATCHING PATTERN "*.h"
)