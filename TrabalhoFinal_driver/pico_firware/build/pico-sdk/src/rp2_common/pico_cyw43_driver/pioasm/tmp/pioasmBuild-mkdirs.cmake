# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/monica/pico-sdk/tools/pioasm"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/pioasm"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/pioasm-install"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
