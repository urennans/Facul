# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/_deps/picotool-src"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/_deps/picotool-build"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/_deps"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/picotool/tmp"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/picotool/src/picotoolBuild-stamp"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/picotool/src"
  "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/monica/trabalhos/TrabalhoFinal_driver/pico_firware/build/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
