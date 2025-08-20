cmake_minimum_required(VERSION 3.16)
# Manually set CMAKE_SYSTEM_NAME to indicate cross compiling
set(CMAKE_SYSTEM_NAME "ARM")
include(FetchContent)
# Setup toolchian
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/build-tool/cmake/stm32_gcc.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#set Driver Path
set(DRIVERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/BSP/Drivers/STM32F1xx_HAL_Driver/Src)
set(DRIVERS_INC ${CMAKE_CURRENT_SOURCE_DIR}/BSP/Drivers/STM32F1xx_HAL_Driver/Inc)
set(RTOS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/FreeRTOS)
set(RTOS_INC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/FreeRTOS/include ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/FreeRTOS/portable/GCC/ARM_CM3) 

file(GLOB SRC_FILES 
        "${DRIVERS_PATH}/stm32f1xx_hal_uart.c"
        "${DRIVERS_PATH}/stm32f1xx_hal_rcc.c"
        "${DRIVERS_PATH}/stm32f1xx_hal_dma.c"
        "${DRIVERS_PATH}/stm32f1xx_hal_cortex.c"
        "${DRIVERS_PATH}/stm32f1xx_hal_tim.c"
        "${DRIVERS_PATH}/stm32f1xx_hal_tim_ex.c"
        
        
)
file(GLOB RTOS_FILES
        "${RTOS_PATH}/portable/GCC/ARM_CM3/port.c"
        "${RTOS_PATH}/portable/MemMang/heap_4.c"
        "${RTOS_PATH}/event_groups.c"
        "${RTOS_PATH}/list.c"
        "${RTOS_PATH}/queue.c"
        "${RTOS_PATH}/stream_buffer.c"
        "${RTOS_PATH}/tasks.c"
        "${RTOS_PATH}/timers.c"

)

# set project
project(stm32f1 C ASM)

# Set build options
set(USE_HAL 1)

# Setup cubemx directory
set(STM32_CUBE_F1_PATH ${CMAKE_CURRENT_SOURCE_DIR}/BSP)

# Get CMSIS package needed for compilation
find_package(CMSIS COMPONENTS STM32F10X REQUIRED)

# Set project sources
set(PROJECT_SOURCES
    ${RTOS_FILES}
    src/main.c
    src/uart.c
    src/stm32f1xx_it.c
    src/gpio.c
    src/stm32f1xx_hal_timebase_tim.c
    src/freertos.c
    ${SRC_FILES}
    
)


# FetchContent_Declare( freertos_kernel
#   GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
#   GIT_TAG        main #Note: Best practice to use specific git-hash or tagged version
# )

# add_library(freertos_config INTERFACE)

# target_include_directories(freertos_config SYSTEM
# INTERFACE
#     include
# )

# target_compile_definitions(freertos_config
#   INTERFACE
#     projCOVERAGE_TEST=0
# )
# target_compile_definitions(freertos_config INTERFACE ${definitions})
# target_compile_options(freertos_config INTERFACE ${options}
#     -mcpu=cortex-m3
#     -mthumb
#     -Wall
#     # Add any other compiler options specific to your project
# )

# FetchContent_MakeAvailable(freertos_kernel)

# set(FREERTOS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/FreeRTOS)

# set( FREERTOS_HEAP "4" CACHE STRING "" FORCE)
# # Select the cross-compile PORT

# set(FREERTOS_PORT "GCC_ARM_CM3" CACHE STRING "" FORCE)



# #add_subdirectory(${FREERTOS_PATH})

# Set project includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${DRIVERS_INC} ${RTOS_INC})

# Include HAL if required else only use CMSIS
if (${USE_HAL})
    message("Building with HAL")
    add_definitions(-DUSE_HAL)
    find_package(HAL COMPONENTS STM32F10X REQUIRED)
    set(LIBRARIES CMSIS::STM32::F103C8 HAL::STM32::F1::GPIO)
else()
    message("Building without HAL")
    set(LIBRARIES CMSIS::STM32::F103C8)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-Map,output.map")

# Set the project executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Link libraries to executable based on HAL or CMSIS
target_link_libraries(${PROJECT_NAME}
 ${LIBRARIES}
 STM32::NoSys
)
# target_link_libraries(${PROJECT_NAME} 
# freertos_config
#  freertos_kernel
# )

# Generated wanted files and printout debug information
stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_binary_file(${PROJECT_NAME})
stm32_generate_hex_file(${PROJECT_NAME})