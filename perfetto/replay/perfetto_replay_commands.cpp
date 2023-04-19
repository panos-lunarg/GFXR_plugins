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

#include "perfetto_replay_commands.h"
#include "util/logging.h"
#include "../perfetto_tracing_categories.h"
#include "util/defines.h"

#include <unordered_map>
#include <string>
#include <sstream>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(plugins)
GFXRECON_BEGIN_NAMESPACE(replay)
GFXRECON_BEGIN_NAMESPACE(plugin_perfetto)

static std::unordered_map<uint64_t, VulkanObjectInfo> vulkan_objects;

static void InitializePerfetto()
{
    static bool initialized = false;

    if (!initialized)
    {
        // Initialize perfetto
        perfetto::TracingInitArgs args;
        args.backends |= perfetto::kInProcessBackend;
        args.backends |= perfetto::kSystemBackend;
        perfetto::Tracing::Initialize(args);
        perfetto::TrackEvent::Register();

        initialized = true;
    }
}

void PreProcess_CreateInstance(const ApiCallInfo&           call_info,
                               VkResult                     returnValue,
                               const VkInstanceCreateInfo*  pCreateInfo,
                               const VkAllocationCallbacks* pAllocator,
                               VkInstance*                  pInstance)
{
    InitializePerfetto();

    const uint64_t    command_index = call_info.index;
    const std::string submit_name   = "vkCreateInstance: " + std::to_string(command_index);
    TRACE_EVENT_INSTANT("GFXR", perfetto::DynamicString{ submit_name.c_str() }, "Command ID:", command_index);
}

void PreProcess_QueueSubmit(const ApiCallInfo&  call_info,
                            VkResult            returnValue,
                            VkQueue             queue,
                            uint32_t            submitCount,
                            const VkSubmitInfo* pSubmits,
                            VkFence             fence)
{
    if (pSubmits)
    {
        TRACE_EVENT_INSTANT("GFXR", "vkQueueSubmit", [&](perfetto::EventContext ctx) {
            ctx.AddDebugAnnotation("vkQueueSubmit:", call_info.index);

            uint32_t cmd_buf_count = 0;
            for (uint32_t s = 0; s < submitCount; ++s)
            {
                for (uint32_t c = 0; c < pSubmits[s].commandBufferCount; ++c)
                {
                    const auto cmd_buf_info = vulkan_objects.find(
                        GFXRECON_PTR_TO_UINT<VkCommandBuffer, uint64_t>(pSubmits[s].pCommandBuffers[c]));

                    if (cmd_buf_info != vulkan_objects.end())
                    {
                        std::stringstream id;
                        id << "0x" << std::hex << cmd_buf_info->second.capture_handle
                           << " (id: " << cmd_buf_info->second.capture_id << ")";
                        ctx.AddDebugAnnotation(
                            perfetto::DynamicString{ "vkCommandBuffer: " + std::to_string(cmd_buf_count++) },
                            perfetto::DynamicString{ id.str() });
                    }
                }
            }
        });
    }
    else
    {
        TRACE_EVENT_INSTANT("GFXR", "vkQueueSubmit (empty)", [&](perfetto::EventContext ctx) {});
    }
}

void PreProcess_QueuePresent(const ApiCallInfo&      call_info,
                             VkResult                returnValue,
                             VkQueue                 queue,
                             const VkPresentInfoKHR* pPresentInfo)
{
    const uint64_t    command_index = call_info.index;
    const std::string submit_name   = "vkQueuePresent: " + std::to_string(command_index);
    TRACE_EVENT_INSTANT("GFXR", perfetto::DynamicString{ submit_name.c_str() }, "Command ID:", command_index);
}

void Process_AddStateInformation(const plugins::replay::StateInformation* info)
{
    assert(info);

    switch (info->info_type)
    {
        case kObjectInfo:
            vulkan_objects[info->object_info.replay_handle] = info->object_info;
            break;

        default:
            break;
    };
}

GFXRECON_END_NAMESPACE(plugin_perfetto)
GFXRECON_END_NAMESPACE(replay)
GFXRECON_END_NAMESPACE(plugins)
GFXRECON_END_NAMESPACE(gfxrecon)
