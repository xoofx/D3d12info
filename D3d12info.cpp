#include "pch.h"
#include <iostream>

static const wchar_t* D3D12_SHADER_MIN_PRECISION_SUPPORT_NAMES[] = {
    L"D3D12_SHADER_MIN_PRECISION_SUPPORT_NONE",
    L"D3D12_SHADER_MIN_PRECISION_SUPPORT_10_BIT",
    L"D3D12_SHADER_MIN_PRECISION_SUPPORT_16_BIT",
};
static const uint32_t D3D12_SHADER_MIN_PRECISION_SUPPORT_VALUES[] = {
    D3D12_SHADER_MIN_PRECISION_SUPPORT_NONE,
    D3D12_SHADER_MIN_PRECISION_SUPPORT_10_BIT,
    D3D12_SHADER_MIN_PRECISION_SUPPORT_16_BIT,
};

static const wchar_t* D3D12_TILED_RESOURCES_TIER_NAMES[] = {
    L"D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED",
    L"D3D12_TILED_RESOURCES_TIER_1",
    L"D3D12_TILED_RESOURCES_TIER_2",
    L"D3D12_TILED_RESOURCES_TIER_3",
    //L"D3D12_TILED_RESOURCES_TIER_4", // TODO
};
static const uint32_t D3D12_TILED_RESOURCES_TIER_VALUES[] = {
    D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED,
    D3D12_TILED_RESOURCES_TIER_1,
    D3D12_TILED_RESOURCES_TIER_2,
    D3D12_TILED_RESOURCES_TIER_3,
    //D3D12_TILED_RESOURCES_TIER_4, // TODO
};

static const wchar_t* D3D12_RESOURCE_BINDING_TIER_NAMES[] = {
    L"D3D12_RESOURCE_BINDING_TIER_1",
    L"D3D12_RESOURCE_BINDING_TIER_2",
    L"D3D12_RESOURCE_BINDING_TIER_3",
};
static const uint32_t D3D12_RESOURCE_BINDING_TIER_VALUES[] = {
    D3D12_RESOURCE_BINDING_TIER_1,
    D3D12_RESOURCE_BINDING_TIER_2,
    D3D12_RESOURCE_BINDING_TIER_3,
};

static const wchar_t* D3D12_CONSERVATIVE_RASTERIZATION_TIER_NAMES[] = {
    L"D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED",
    L"D3D12_CONSERVATIVE_RASTERIZATION_TIER_1",
    L"D3D12_CONSERVATIVE_RASTERIZATION_TIER_2",
    L"D3D12_CONSERVATIVE_RASTERIZATION_TIER_3",
};
static const uint32_t D3D12_CONSERVATIVE_RASTERIZATION_TIER_VALUES[] = {
    D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED,
    D3D12_CONSERVATIVE_RASTERIZATION_TIER_1,
    D3D12_CONSERVATIVE_RASTERIZATION_TIER_2,
    D3D12_CONSERVATIVE_RASTERIZATION_TIER_3,
};

static const wchar_t* D3D12_CROSS_NODE_SHARING_TIER_NAMES[] = {
    L"D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED",
    L"D3D12_CROSS_NODE_SHARING_TIER_1_EMULATED",
    L"D3D12_CROSS_NODE_SHARING_TIER_1",
    L"D3D12_CROSS_NODE_SHARING_TIER_2",
    //L"D3D12_CROSS_NODE_SHARING_TIER_3",
};
static const uint32_t D3D12_CROSS_NODE_SHARING_TIER_VALUES[] = {
    D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED,
    D3D12_CROSS_NODE_SHARING_TIER_1_EMULATED,
    D3D12_CROSS_NODE_SHARING_TIER_1,
    D3D12_CROSS_NODE_SHARING_TIER_2,
    //D3D12_CROSS_NODE_SHARING_TIER_3, // TODO
};

static const wchar_t* D3D12_RESOURCE_HEAP_TIER_NAMES[] = {
    L"D3D12_RESOURCE_HEAP_TIER_1",
    L"D3D12_RESOURCE_HEAP_TIER_2",
};
static const uint32_t D3D12_RESOURCE_HEAP_TIER_VALUES[] = {
    D3D12_RESOURCE_HEAP_TIER_1,
    D3D12_RESOURCE_HEAP_TIER_2,
};

static const wchar_t* DXGI_ADAPTER_FLAG_NAMES[] = {
   L"DXGI_ADAPTER_FLAG_NONE",
   L"DXGI_ADAPTER_FLAG_REMOTE",
   L"DXGI_ADAPTER_FLAG_SOFTWARE",
};
static const uint32_t DXGI_ADAPTER_FLAG_VALUES[] = {
   DXGI_ADAPTER_FLAG_NONE,
   DXGI_ADAPTER_FLAG_REMOTE,
   DXGI_ADAPTER_FLAG_SOFTWARE,
};


static HINSTANCE g_Instance;
static CComPtr<ID3D12Device> g_Device;
static uint32_t g_Indent;

static void PrintIndent()
{
    wprintf(L"%s", L"                                " + (32 - g_Indent * 2));
}

static void Print_BOOL(const wchar_t* name, BOOL value)
{
    PrintIndent();
    wprintf(L"%s = %s\n", name, value ? L"TRUE" : L"FALSE");
}
static void Print_uint32(const wchar_t* name, uint32_t value)
{
    PrintIndent();
    wprintf(L"%s = %u\n", name, value);
}
static void Print_size(const wchar_t* name, size_t value)
{
    PrintIndent();
    wprintf(L"%s = %zu\n", name, value);
}
static void Print_hex32(const wchar_t* name, uint32_t value)
{
    PrintIndent();
    wprintf(L"%s = 0x%X\n", name, value);
}
static void Print_string(const wchar_t* name, const wchar_t* value)
{
    PrintIndent();
    wprintf(L"%s = %s\n", name, value);
}
static void Print_LUID(const wchar_t* name, LUID value)
{
    PrintIndent();
    wprintf(L"%s = %08X-%08X\n", name, (uint32_t)value.HighPart, (uint32_t)value.LowPart);
}

static void PrintStructBegin(const wchar_t* name)
{
    PrintIndent();
    wprintf(L"%s:\n", name);
    ++g_Indent;
}

static void PrintStructEnd()
{
    --g_Indent;
}

static void PrintEnum(const wchar_t* name, uint32_t value,
    const wchar_t* const* names, const uint32_t* values, size_t valueCount)
{
    PrintIndent();
    for(size_t i = 0; i < valueCount; ++i)
    {
        if(value == values[i])
        {
            wprintf(L"%s = %s (0x%X)\n", name, names[i], value);
            return;
        }
    }
    wprintf(L"%s = 0x%X\n", name, value);
}

static void PrintFlags(const wchar_t* name, uint32_t value,
    const wchar_t* const* flagNames, const uint32_t* flagValues,
    size_t flagCount)
{
    PrintIndent();
    wprintf(L"%s = 0x%X\n", name, value);

    ++g_Indent;
    size_t zeroFlagIndex = SIZE_MAX;
    for(size_t i = 0; i < flagCount; ++i)
    {
        if(flagValues[i] == 0)
        {
            zeroFlagIndex = i;
        }
        else
        {
            if((value & flagValues[i]) != 0)
            {
                PrintIndent();
                wprintf(L"%s\n", flagNames[i]);
            }
        }
    }
    if(value == 0 && zeroFlagIndex != SIZE_MAX)
    {
        PrintIndent();
        wprintf(L"%s\n", flagNames[zeroFlagIndex]);
    }
    --g_Indent;
}

static void Print_D3D12_FEATURE_DATA_D3D12_OPTIONS(const D3D12_FEATURE_DATA_D3D12_OPTIONS& options)
{
    Print_BOOL(  L"DoublePrecisionFloatShaderOps       ", options.DoublePrecisionFloatShaderOps);
    Print_BOOL(  L"OutputMergerLogicOp                 ", options.OutputMergerLogicOp);
    PrintEnum(   L"MinPrecisionSupport                 ", options.MinPrecisionSupport, D3D12_SHADER_MIN_PRECISION_SUPPORT_NAMES, D3D12_SHADER_MIN_PRECISION_SUPPORT_VALUES, _countof(D3D12_SHADER_MIN_PRECISION_SUPPORT_VALUES));
    PrintEnum(   L"TiledResourcesTier                  ", options.TiledResourcesTier, D3D12_TILED_RESOURCES_TIER_NAMES, D3D12_TILED_RESOURCES_TIER_VALUES, _countof(D3D12_TILED_RESOURCES_TIER_VALUES));
    PrintEnum(   L"ResourceBindingTier                 ", options.ResourceBindingTier, D3D12_RESOURCE_BINDING_TIER_NAMES, D3D12_RESOURCE_BINDING_TIER_VALUES, _countof(D3D12_RESOURCE_BINDING_TIER_VALUES));
    Print_BOOL(  L"PSSpecifiedStencilRefSupported      ", options.PSSpecifiedStencilRefSupported);
    Print_BOOL(  L"TypedUAVLoadAdditionalFormats       ", options.TypedUAVLoadAdditionalFormats);
    Print_BOOL(  L"ROVsSupported                       ", options.ROVsSupported);
    PrintEnum(   L"ConservativeRasterizationTier       ", options.ConservativeRasterizationTier, D3D12_CONSERVATIVE_RASTERIZATION_TIER_NAMES, D3D12_CONSERVATIVE_RASTERIZATION_TIER_VALUES, _countof(D3D12_CONSERVATIVE_RASTERIZATION_TIER_VALUES));
    Print_uint32(L"MaxGPUVirtualAddressBitsPerResource ", options.MaxGPUVirtualAddressBitsPerResource);
    Print_BOOL(  L"StandardSwizzle64KBSupported        ", options.StandardSwizzle64KBSupported);
    PrintEnum(   L"CrossNodeSharingTier                ", options.CrossNodeSharingTier, D3D12_CROSS_NODE_SHARING_TIER_NAMES, D3D12_CROSS_NODE_SHARING_TIER_VALUES, _countof(D3D12_CROSS_NODE_SHARING_TIER_VALUES));
    Print_BOOL(  L"CrossAdapterRowMajorTextureSupported", options.CrossAdapterRowMajorTextureSupported);
    Print_BOOL(  L"VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation", options.VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation);
    PrintEnum(   L"ResourceHeapTier                    ", options.ResourceHeapTier, D3D12_RESOURCE_HEAP_TIER_NAMES, D3D12_RESOURCE_HEAP_TIER_VALUES, _countof(D3D12_RESOURCE_HEAP_TIER_VALUES));
}

int main()
{
    g_Instance = (HINSTANCE)GetModuleHandle(NULL);

    CoInitialize(NULL);

    wprintf(L"=========\n");
    wprintf(L"D3D12INFO\n");
    wprintf(L"=========\n");
    wprintf(L"\n");

    IDXGIFactory4* dxgiFactory;
    CHECK_HR( CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) );

    wprintf(L"DXGI Adapters:\n");
    wprintf(L"==============\n");
    IDXGIAdapter1* adapter = nullptr;
    UINT adapterIndex = 0;
    while(dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        wprintf(L"Adapter %u:\n", adapterIndex);

        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);
        ++g_Indent;
        Print_string(L"Description          ", desc.Description);
        Print_hex32 (L"VendorId             ", desc.VendorId);
        Print_hex32 (L"DeviceId             ", desc.DeviceId);
        Print_hex32 (L"SubSysId             ", desc.SubSysId);
        Print_hex32 (L"Revision             ", desc.Revision);
        Print_size  (L"DedicatedVideoMemory ", desc.DedicatedVideoMemory);
        Print_size  (L"DedicatedSystemMemory", desc.DedicatedSystemMemory);
        Print_size  (L"SharedSystemMemory   ", desc.SharedSystemMemory);
        Print_LUID  (L"AdapterLuid          ", desc.AdapterLuid);
        PrintFlags  (L"Flags                ", desc.Flags, DXGI_ADAPTER_FLAG_NAMES, DXGI_ADAPTER_FLAG_VALUES, _countof(DXGI_ADAPTER_FLAG_VALUES));
        --g_Indent;

        SAFE_RELEASE(adapter);
        adapterIndex++;
    }

    CHECK_HR( dxgiFactory->EnumAdapters1(0, &adapter) );
    ID3D12Device* device = nullptr;
    CHECK_HR( D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)) );
    g_Device.Attach(device);

    wprintf(L"\n");
    wprintf(L"D3D12_FEATURE_DATA_D3D12_OPTIONS:\n");
    wprintf(L"=================================\n");
    D3D12_FEATURE_DATA_D3D12_OPTIONS options = {};
    CHECK_HR( g_Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options)) );
    Print_D3D12_FEATURE_DATA_D3D12_OPTIONS(options);

    g_Device.Release();

    SAFE_RELEASE(adapter);
    SAFE_RELEASE(dxgiFactory);
}
