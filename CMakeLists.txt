cmake_minimum_required(VERSION 3.28)

project(freertos LANGUAGES ASM C CXX VERSION 1.0)

# Include the ARM toolchain
include(gcc-arm-none-eabi.cmake)

file(GLOB_RECURSE FREERTOS_SOURCES 
    ${CMAKE_SOURCE_DIR}/FreeRTOS/*.c
)

message(STATUS ${FREERTOS_SOURCES})


# Create the main executable target
add_executable(${PROJECT_NAME} ${SOURCES} ${FREERTOS_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/cmsis/inc
    ${CMAKE_SOURCE_DIR}/cmsis/core
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/FreeRTOS/include
)


include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/FreeRTOS/portable/GCC/ARM_CM4F  )
include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/FreeRTOS/include)


target_compile_definitions(${PROJECT_NAME} PUBLIC STM32F411xE)

# Set compiler options (e.g., enable debugging)
target_compile_options(${PROJECT_NAME} PUBLIC -g)

# Set linker options
target_link_options(${PROJECT_NAME} PUBLIC 
    -T${LINKER_SCRIPT}
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
    -Wl,--gc-sections
)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
)

add_custom_target(flash 
    COMMAND st-flash write ${PROJECT_NAME}.bin 0x08000000
    COMMAND st-flash reset
)