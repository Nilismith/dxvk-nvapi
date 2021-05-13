#pragma once

#include "../nvapi_private.h"
#include "../util/com_pointer.h"
#include "nvapi_output.h"

#include <set>

namespace dxvk {
    class NvapiAdapter {

    public:
        NvapiAdapter();
        ~NvapiAdapter();

        bool Initialize(Com<IDXGIAdapter>& dxgiAdapter, std::vector<NvapiOutput*>& outputs);
        [[nodiscard]] std::string GetDeviceName() const;
        [[nodiscard]] uint32_t GetDriverVersion() const;
        [[nodiscard]] uint32_t GetDeviceId() const;
        [[nodiscard]] uint32_t GetGpuType() const;
        [[nodiscard]] uint32_t GetBusId() const;
        [[nodiscard]] uint32_t GetVRamSize() const;

    private:
        bool isVkDeviceExtensionSupported(std::string extName);

        VkPhysicalDeviceProperties m_deviceProperties{};
        VkPhysicalDevicePCIBusInfoPropertiesEXT m_devicePciBusProperties{};
        VkPhysicalDeviceMemoryProperties m_memoryProperties{};
        uint32_t m_vkDriverVersion{};
        std::set<std::string> m_deviceExtensions{};
    };
}
