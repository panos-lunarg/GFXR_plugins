/*
** Copyright (c) 2023 Valve Corporation
** Copyright (c) 2023 LunarG, Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
*/

#ifndef GFXRECON_PLUGINS_PERFETTO_VULKAN_REPLAY_COMMANDS_H
#define GFXRECON_PLUGINS_PERFETTO_VULKAN_REPLAY_COMMANDS_H

#include "format/api_call_id.h"
#include "vulkan/vulkan.h"
#include "decode/api_decoder.h"
#include "decode/struct_pointer_decoder.h"
#include "decode/handle_pointer_decoder.h"
#include "generated/generated_vulkan_struct_decoders.h"
#include "generated/generated_vulkan_struct_handle_wrappers.h"
#include "util/defines.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(plugins)
GFXRECON_BEGIN_NAMESPACE(replay)
GFXRECON_BEGIN_NAMESPACE(plugin_perfetto)

using namespace decode;

template <format::ApiCallId Id>
struct PerfettoReplayPreCall
{
    template <typename... Args>
    static void Dispatch(const ApiCallInfo&, VkResult, Args...)
    {}
};

template <format::ApiCallId Id>
struct PerfettoEncoderPostCall
{
    template <typename... Args>
    static void Dispatch(const ApiCallInfo&, VkResult, Args...)
    {}
};

#if !defined(WIN32)

void PreProcess_CreateInstance(const ApiCallInfo&           call_info,
                               VkResult                     returnValue,
                               const VkInstanceCreateInfo*  pCreateInfo,
                               const VkAllocationCallbacks* pAllocator,
                               VkInstance*                  pInstance);

void PreProcess_QueueSubmit(const ApiCallInfo&  call_info,
                            VkResult            returnValue,
                            VkQueue             queue,
                            uint32_t            submitCount,
                            const VkSubmitInfo* pSubmits,
                            VkFence             fence);

void PreProcess_QueuePresent(const ApiCallInfo&      call_info,
                             VkResult                returnValue,
                             VkQueue                 queue,
                             const VkPresentInfoKHR* pPresentInfo);

template <>
struct PerfettoReplayPreCall<format::ApiCallId::ApiCall_vkCreateInstance>
{
    template <typename... Args>
    static void Dispatch(const ApiCallInfo& call_info, VkResult result, Args... args)
    {
        PreProcess_CreateInstance(call_info, result, args...);
    }
};

template <>
struct PerfettoReplayPreCall<format::ApiCallId::ApiCall_vkQueuePresentKHR>
{
    template <typename... Args>
    static void Dispatch(const ApiCallInfo& call_info, Args... args)
    {
        PreProcess_QueuePresent(call_info, args...);
    }
};

template <>
struct PerfettoReplayPreCall<format::ApiCallId::ApiCall_vkQueueSubmit>
{
    template <typename... Args>
    static void Dispatch(const ApiCallInfo& call_info, Args... args)
    {
        PreProcess_QueueSubmit(call_info, args...);
    }
};

#endif // !defined(WIN32)

GFXRECON_END_NAMESPACE(plugin_perfetto)
GFXRECON_END_NAMESPACE(replay)
GFXRECON_END_NAMESPACE(plugins)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_PLUGINS_PERFETTO_VULKAN_REPLAY_COMMANDS_H
