# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/santiago/Research/microfuzz/toolchains/esp-idf/components/bootloader/subproject"
  "/home/santiago/Research/microfuzz/on_board/ESP32/build/bootloader"
  "/home/santiago/Research/microfuzz/on_board/ESP32/build/bootloader-prefix"
  "/home/santiago/Research/microfuzz/on_board/ESP32/build/bootloader-prefix/tmp"
  "/home/santiago/Research/microfuzz/on_board/ESP32/build/bootloader-prefix/src/bootloader-stamp"
  "/home/santiago/Research/microfuzz/on_board/ESP32/build/bootloader-prefix/src"
  "/home/santiago/Research/microfuzz/on_board/ESP32/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/santiago/Research/microfuzz/on_board/ESP32/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/santiago/Research/microfuzz/on_board/ESP32/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
