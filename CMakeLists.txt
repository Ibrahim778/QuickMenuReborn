cmake_minimum_required(VERSION 3.12)
set(CMAKE_TOOLCHAIN_FILE "$ENV{SCE_PSP2_SDK_DIR}/host_tools/build/cmake/psp2-snc-toolchain.cmake")
project(QuickMenuReborn)
include(VitaDevelopmentSuite)

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

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
enable_language(EMD)

# ONLY COMPILE MAIN PLUGIN WITH -O0 or -O1
add_compile_options(
    -D_SCE_TARGET_OS_PSP2=1 -Xdiag=0 -Xquit=2
)
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

# Main Plugin

add_library(${PROJECT_NAME} MODULE
    ${user_cppfiles}
    ${qm_reborn_includes}
    ${user_hfiles}
    ${user_cfiles}
    QuickMenuReborn.emd
)

target_link_libraries(${PROJECT_NAME}
    SceKernel_stub
    SceThreadmgr_stub
    SceDbg_stub
    SceAppMgrUser_stub
    SceRegMgr_stub
    SceDisplay_stub
    taihenUnsafe_stub
	ScePafStdc_stub_weak
    ScePafThread_stub_weak
    ScePafTopLevel_stub_weak
    ScePafWidget_stub_weak
    ScePafCommon_stub_weak
    ScePafGraphics_stub_weak
    ScePafMisc_stub_weak
    ScePafResource_stub_weak
)

VDSuiteSignElf(${PROJECT_NAME}.suprx ${PROJECT_NAME})

VDSuiteCreateStubs(stubs ${PROJECT_NAME}.emd
  LIBRARIES
    QuickMenuReborn_stub
    QuickMenuReborn_stub_weak
)

# Generate VitaSDK stubs
add_custom_command(TARGET QuickMenuReborn POST_BUILD
  COMMAND $ENV{SCE_PSP2_SDK_DIR}/host_tools/build/bin/vdsuite-libgen.exe ${CMAKE_SOURCE_DIR}/QuickMenuReborn.yml ${CMAKE_BINARY_DIR}/vitasdk_stubs --output-kind VitasdkStub
)

add_custom_command(TARGET QuickMenuReborn POST_BUILD
    COMMAND python ${CMAKE_SOURCE_DIR}/resource/cxml/appinfo/appinfo_compiler.py -o ${CMAKE_SOURCE_DIR}/resource/qmr_plugin.rco ${CMAKE_SOURCE_DIR}/resource/src/qmr_plugin.xml
)

add_custom_target(install_vdsuite
    COMMAND xcopy /E /I /H /Y ..\\src\\quickmenureborn\\ %SCE_PSP2_SDK_DIR%\\target\\include\\vdsuite\\user\\quickmenureborn\\
    COMMAND xcopy /E /I /H /Y stubs\\libQuickMenuReborn_stub.a %SCE_PSP2_SDK_DIR%\\target\\lib\\vdsuite\\
    COMMAND xcopy /E /I /H /Y stubs\\libQuickMenuReborn_stub_weak.a %SCE_PSP2_SDK_DIR%\\target\\lib\\vdsuite\\
)

add_subdirectory(sample_plugin)