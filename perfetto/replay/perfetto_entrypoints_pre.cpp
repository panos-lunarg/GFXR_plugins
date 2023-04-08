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

#include "includes/replay/plugins_entrypoints_pre.h"
#include "perfetto_replay_commands.h"
#include "format/api_call_id.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(plugins)
GFXRECON_BEGIN_NAMESPACE(replay)
GFXRECON_BEGIN_NAMESPACE(plugin_perfetto)

extern "C"
{
    VKAPI_ATTR void VKAPI_CALL CreateInstance_PreCall(const ApiCallInfo&           call_info,
                                                      VkResult                     returnValue,
                                                      const VkInstanceCreateInfo*  pCreateInfo,
                                                      const VkAllocationCallbacks* pAllocator,
                                                      VkInstance*                  pInstance)
    {
        PerfettoReplayPreCall<format::ApiCallId::ApiCall_vkCreateInstance>::Dispatch(
            call_info, returnValue, pCreateInfo, pAllocator, pInstance);
    }

    VKAPI_ATTR void VKAPI_CALL QueueSubmit_PreCall(const ApiCallInfo&  call_info,
                                                   VkResult            returnValue,
                                                   VkQueue             queue,
                                                   uint32_t            submitCount,
                                                   const VkSubmitInfo* pSubmits,
                                                   VkFence             fence)
    {
        PerfettoReplayPreCall<format::ApiCallId::ApiCall_vkQueueSubmit>::Dispatch(
            call_info, returnValue, queue, submitCount, pSubmits, fence);
    }

    VKAPI_ATTR void VKAPI_CALL QueuePresentKHR_PreCall(const ApiCallInfo&      call_info,
                                                       VkResult                returnValue,
                                                       VkQueue                 queue,
                                                       const VkPresentInfoKHR* pPresentInfo)
    {
        PerfettoReplayPreCall<format::ApiCallId::ApiCall_vkQueuePresentKHR>::Dispatch(
            call_info, returnValue, queue, pPresentInfo);
    }
} // extern "C"

GFXRECON_END_NAMESPACE(plugin_perfetto)
GFXRECON_END_NAMESPACE(replay)
GFXRECON_END_NAMESPACE(plugins)
GFXRECON_END_NAMESPACE(gfxrecon)
