# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/211075/esp/esp-idf/components/bootloader/subproject"
  "D:/Personal/Clase_Embebidos/embedded-systems-track/subjects/RTOS2/Demos/01_Task_Creation/build/bootloader"
  "D:/Personal/Clase_Embebidos/embedded-systems-track/subjects/RTOS2/Demos/01_Task_Creation/build/bootloader-prefix"
  "D:/Personal/Clase_Embebidos/embedded-systems-track/subjects/RTOS2/Demos/01_Task_Creation/build/bootloader-prefix/tmp"
  "D:/Personal/Clase_Embebidos/embedded-systems-track/subjects/RTOS2/Demos/01_Task_Creation/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Personal/Clase_Embebidos/embedded-systems-track/subjects/RTOS2/Demos/01_Task_Creation/build/bootloader-prefix/src"
  "D:/Personal/Clase_Embebidos/embedded-systems-track/subjects/RTOS2/Demos/01_Task_Creation/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Personal/Clase_Embebidos/embedded-systems-track/subjects/RTOS2/Demos/01_Task_Creation/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
