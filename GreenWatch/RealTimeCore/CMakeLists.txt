﻿#  Copyright (c) Microsoft Corporation. All rights reserved.
#  Licensed under the MIT License.

cmake_minimum_required (VERSION 3.8)
project (GreenWatch_RealTimeCore C)

# Create executable
add_executable (${PROJECT_NAME}  main.c resources/LPS22HH.c resources/LSM6DSO.c resources/ui_msg.c resources/utilities.c lib/VectorTable.c lib/GPT.c lib/GPIO.c lib/UART.c lib/Print.c lib/I2CMaster.c lib/ADC.c)
target_link_libraries (${PROJECT_NAME})
set_target_properties (${PROJECT_NAME} PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/linker.ld)

azsphere_configure_tools(TOOLS_REVISION "20.04")
azsphere_configure_api(TARGET_API_SET "5+Beta2004")

string(APPEND CMAKE_C_FLAGS " -D DEBUG")

# Add MakeImage post-build command
include ("${AZURE_SPHERE_MAKE_IMAGE_FILE}")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC -L"${CMAKE_SOURCE_DIR}")