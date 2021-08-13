# Sample App

cmake_minimum_required(VERSION 3.12)
set(CMAKE_TOOLCHAIN_FILE "$ENV{SCE_PSP2_SDK_DIR}/host_tools/build/cmake/psp2-snc-toolchain.cmake")
project(QuickMenuRebornSample)

include(VitaDevelopmentSuite)

set(CMAKE_C_STANDARD 99)
enable_language(EMD)

add_link_options(
  --prx-no-runtime-support
)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${VDSUITE_USER_INCLUDE_DIRECTORIES}
  ${VDSUITE_STANDARD_INCLUDE_DIRECTORIES}
)

link_directories(
  ${VDSUITE_LIBRARY_DIRECTORIES}
)

add_library(samples MODULE
  src/main.c
  QuickMenuRebornSample.emd
)

target_link_libraries(samples
  SceKernel_stub
  QuickMenuReborn_stub_weak
)

VDSuiteSignElf(QuickMenuRebornSample.suprx samples)