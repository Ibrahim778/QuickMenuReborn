cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE "$ENV{SCE_PSP2_SDK_DIR}/host_tools/build/cmake/psp2-snc-toolchain.cmake")

project(QuickMenuReborn)

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

file(GLOB qm_reborn_includes
    "${CMAKE_SOURCE_DIR}/src/quickmenureborn/*.h"
)
include(VitaDevelopmentSuite)

set(CMAKE_C_STANDARD 99)

enable_language(EMD)


add_compile_options(
  -Xdiag=2 -Xquit=2
)

add_link_options(
  --prx-no-runtime-support
)

link_directories(
  ${VDSUITE_LIBRARY_DIRECTORIES}
)

add_library(${PROJECT_NAME} MODULE
    ${user_cppfiles}
    ${user_cfiles}
    ${qm_reborn_includes}
    ${user_hfiles}
    exports.emd
)

include_directories(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}
    ${VDSUITE_USER_INCLUDE_DIRECTORIES}
    ${VDSUITE_STANDARD_INCLUDE_DIRECTORIES}
)

target_link_libraries(${PROJECT_NAME}
    SceAppMgrUser_stub
    SceKernel_stub
    ${pafStubs}
    ${appMgrStubs}
)



install(DIRECTORY ${CMAKE_BINARY_DIR}/user_stubs/
  DESTINATION lib
  FILES_MATCHING PATTERN "*.a"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/quickmenureborn/
  DESTINATION include/quickmenureborn
  FILES_MATCHING PATTERN "*.h"
)