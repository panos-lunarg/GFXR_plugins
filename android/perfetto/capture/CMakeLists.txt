###############################################################################
# Copyright (c) 2018-2022 LunarG, Inc.
# Copyright (c) 2019-2022 Advanced Micro Devices, Inc.
# All rights reserved
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#
# Author: LunarG Team
# Author: AMD Developer Tools Team
# Description: CMake script for framework util target
###############################################################################

cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

get_filename_component(GFXRECON_SOURCE_DIR ../../../../.. ABSOLUTE)

include(../../cmake-config/PlatformConfig.cmake)
add_subdirectory(${GFXRECON_SOURCE_DIR}/android/framework/util ${GFXRECON_SOURCE_DIR}/android/framework/util/build)

add_library(perfetto STATIC ${GFXRECON_SOURCE_DIR}/plugins/GFXR_plugins/external/perfetto/sdk/perfetto.cc)
target_link_libraries(perfetto log)

add_library(gfxrecon_perfetto_capture_plugin SHARED "")

target_sources(gfxrecon_perfetto_capture_plugin
               PRIVATE
                    ${GFXRECON_SOURCE_DIR}/plugins/GFXR_plugins/perfetto/capture/perfetto_capture_commands.cpp
                    ${GFXRECON_SOURCE_DIR}/plugins/GFXR_plugins/perfetto/capture/perfetto_capture_commands.h
                    ${GFXRECON_SOURCE_DIR}/plugins/GFXR_plugins/perfetto/capture/perfetto_entrypoints_post.cpp
                    ${GFXRECON_SOURCE_DIR}/plugins/GFXR_plugins/perfetto/capture/perfetto_entrypoints_pre.cpp
                    ${GFXRECON_SOURCE_DIR}/plugins/GFXR_plugins/perfetto/perfetto_tracing_categories.h
                    ${GFXRECON_SOURCE_DIR}/plugins/GFXR_plugins/perfetto/perfetto_tracing_categories.cpp
                    ${GFXRECON_SOURCE_DIR}/plugins/includes/capture/plugins_entrypoints_post.h
                    ${GFXRECON_SOURCE_DIR}/plugins/includes/capture/plugins_entrypoints_pre.h
)

target_compile_definitions(gfxrecon_perfetto_capture_plugin PRIVATE VK_NO_PROTOTYPES VK_ENABLE_BETA_EXTENSIONS)

target_include_directories(gfxrecon_perfetto_capture_plugin
                           PUBLIC
                               ${GFXRECON_SOURCE_DIR}/plugins
                               ${GFXRECON_SOURCE_DIR}/plugins/GFXR_plugins/external/perfetto/sdk
                               ${GFXRECON_SOURCE_DIR}/external/Vulkan-Headers/include
                               ${GFXRECON_SOURCE_DIR}/framework)

target_link_libraries(gfxrecon_perfetto_capture_plugin gfxrecon_util perfetto)
