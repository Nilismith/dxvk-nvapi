#include "nvapi_adapter.h"

namespace dxvk {
    NvapiAdapter::NvapiAdapter() {}

    NvapiAdapter::~NvapiAdapter() {}

    bool NvapiAdapter::Initialize(Com<IDXGIAdapter> dxgiAdapter) {
        Com<IDXGIVkInteropAdapter> dxgiVkInteropAdapter;
        if (FAILED(dxgiAdapter->QueryInterface(IID_PPV_ARGS(&dxgiVkInteropAdapter))))
            return false;

        VkInstance vkInstance = VK_NULL_HANDLE;
        dxgiVkInteropAdapter->GetVulkanHandles(&vkInstance, &m_vkDevice);

        vkGetPhysicalDeviceProperties(m_vkDevice, &m_deviceProperties);

        // TODO: Support other vendors. Currently we depend on a NVIDIA GPU, though we don't do any NVIDIA specific stuff.
        if (m_deviceProperties.vendorID != 0x10de)
            return false; // No Nvidia card

        m_vkDriverVersion = VK_MAKE_VERSION(
            VK_VERSION_MAJOR(m_deviceProperties.driverVersion),
            VK_VERSION_MINOR(m_deviceProperties.driverVersion >> 0) >> 2,
            VK_VERSION_PATCH(m_deviceProperties.driverVersion >> 2) >> 4);

        std::cerr << "NvAPI Device: " << m_deviceProperties.deviceName << " ("<< std::dec << VK_VERSION_MAJOR(m_vkDriverVersion) << "." << VK_VERSION_MINOR(m_vkDriverVersion) << "." << VK_VERSION_PATCH(m_vkDriverVersion) << ")" << std::endl;
        return true;
    }

    std::string NvapiAdapter::GetDeviceName() {
        return std::string(m_deviceProperties.deviceName);
    }

    u_int NvapiAdapter::GetDriverVersion() {
        // Windows releases can only ever have a two digit minor version
        // and does not have a patch number
        return VK_VERSION_MAJOR(m_vkDriverVersion) * 100 +
            std::min(VK_VERSION_MINOR(m_vkDriverVersion), (u_int)99); 
    }

    u_int NvapiAdapter::GetDeviceId() {
        return m_deviceProperties.deviceID;
    }

    u_int NvapiAdapter::GetGpuType() {
        // The enum values for discrete and unknown GPU are the same for Vulkan and NvAPI.
        VkPhysicalDeviceType vkDeviceType = m_deviceProperties.deviceType;
        if (vkDeviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU || vkDeviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            return vkDeviceType;
        
        return VK_PHYSICAL_DEVICE_TYPE_OTHER;
    }
}
