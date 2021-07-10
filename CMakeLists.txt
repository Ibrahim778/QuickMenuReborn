cmake_minimum_required(VERSION 3.12)
set(CMAKE_TOOLCHAIN_FILE "$ENV{SCE_PSP2_SDK_DIR}/host_tools/build/cmake/psp2-snc-toolchain.cmake")
project(QuickMenuReborn)
file(GLOB pafStubs
    "${CMAKE_SOURCE_DIR}/vdsuite_paf_stub/*_weak.a"
)
file(GLOB user_cppfiles
    "${CMAKE_SOURCE_DIR}/src/user/*.cpp"
)
file(GLOB user_cfiles
    "${CMAKE_SOURCE_DIR}/src/user/*.c"
)
file(GLOB export_cfiles
    "${CMAKE_SOURCE_DIR}/src/export_handler/*.c"
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

include_directories(${PROJECT_NAME}
  ${CMAKE_SOURCE_DIR}
  ${VDSUITE_USER_INCLUDE_DIRECTORIES}
  ${VDSUITE_STANDARD_INCLUDE_DIRECTORIES}
)

link_directories(
  ${VDSUITE_LIBRARY_DIRECTORIES}
)

#Export Handler

add_library(QuickMenuRebornExports MODULE
  ${export_cfiles}
  ExportHandler.emd
)

target_link_libraries(QuickMenuRebornExports
  SceKernel_stub
  ${pafStubs}
  SceDbg_stub
)

VDSuiteSignElf(QuickMenuRebornExports.suprx QuickMenuRebornExports)

VDSuiteCreateStubs(stubs ExportHandler.emd
  LIBRARIES
    quickMenuRebornExports_stub
)
  
# Main Plugin

add_library(${PROJECT_NAME} MODULE
    ${user_cppfiles}
    ${user_cfiles}
    ${qm_reborn_includes}
    ${user_hfiles}
    QuickMenuReborn.emd
)

target_link_libraries(${PROJECT_NAME}
    SceKernel_stub
    SceThreadmgr_stub
    SceDbg_stub
    SceAppMgrUser_stub
    ${pafStubs}
)

VDSuiteSignElf(${PROJECT_NAME}.suprx ${PROJECT_NAME})

#Sample App
add_library(samples MODULE
  ${qm_reborn_includes}
  src/sample_app/main.c
  QuickMenuRebornSample.emd
)

target_link_libraries(samples
  quickMenuRebornExports_stub
  SceKernel_stub
  SceDbg_stub
)

VDSuiteSignElf(QuickMenuRebornSample.suprx samples)