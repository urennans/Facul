cmake_minimum_required(VERSION 3.13)

# Inclui o SDK do Pico
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_sensor_usb C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o executável
add_executable(pico_sensor_usb
    pico_sensor_usb.c
   # usb_descriptors.c
)

# Vincula bibliotecas necessárias
target_link_libraries(pico_sensor_usb 
    pico_stdlib
    hardware_adc
    hardware_pwm
    hardware_gpio
    pico_multicore
)

# Habilita saída USB
pico_enable_stdio_usb(pico_sensor_usb 1)
pico_enable_stdio_uart(pico_sensor_usb 0)

# Gera arquivos de saída (.uf2, .hex, etc.)
pico_add_extra_outputs(pico_sensor_usb)

# Mensagem informativa
message("Projeto pico_sensor_usb configurado com sucesso!")