#TOOLCHAIN CONFIGURATION

set(CMAKE_TOOLCHAIN_PREFIX arm-none-eabi)
set(CMAKE_ASM_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}-as)
set(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}-g++)
set(CMAKE_OBJCOPY ${CMAKE_TOOLCHAIN_PREFIX}-objcopy)
set(CMAKE_OBJDUMP ${CMAKE_TOOLCHAIN_PREFIX}-objdump)
set(CMAKE_SIZE ${CMAKE_TOOLCHAIN_PREFIX}-size)


#MANAGING NECESSARY FILES
file(GLOB_RECURSE SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.c
    z${CMAKE_SOURCE_DIR}/src/*.s
    z${CMAKE_SOURCE_DIR}/src/*.cpp 

)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F411xE.ld)


#COMPILER AND LINKER FLAGS
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -ffunction-sections -fdata-sections -Wall -Werror")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS} -fno-rtti -fno-tree-loop-distribution)


set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")