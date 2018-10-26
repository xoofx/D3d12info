#include "pch.h"
#include <iostream>

static const wchar_t* D3D12_SHADER_MIN_PRECISION_SUPPORT_NAMES[] = {
    L"D3D12_SHADER_MIN_PRECISION_SUPPORT_NONE",
    L"D3D12_SHADER_MIN_PRECISION_SUPPORT_10_BIT",
    L"D3D12_SHADER_MIN_PRECISION_SUPPORT_16_BIT",
};
static const wchar_t* D3D12_TILED_RESOURCES_TIER_NAMES[] = {
    L"D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED",
    L"D3D12_TILED_RESOURCES_TIER_1",
    L"D3D12_TILED_RESOURCES_TIER_2",
    L"D3D12_TILED_RESOURCES_TIER_3",
    L"D3D12_TILED_RESOURCES_TIER_4",
};
static const wchar_t* D3D12_RESOURCE_BINDING_TIER_NAMES[] = {
    L"D3D12_RESOURCE_BINDING_TIER_1",
    L"D3D12_RESOURCE_BINDING_TIER_2",
    L"D3D12_RESOURCE_BINDING_TIER_3",
};
static const wchar_t* D3D12_CONSERVATIVE_RASTERIZATION_TIER_NAMES[] = {
    L"D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED",
    L"D3D12_CONSERVATIVE_RASTERIZATION_TIER_1",
    L"D3D12_CONSERVATIVE_RASTERIZATION_TIER_2",
    L"D3D12_CONSERVATIVE_RASTERIZATION_TIER_3",
};
static const wchar_t* D3D12_CROSS_NODE_SHARING_TIER_NAMES[] = {
    L"D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED",
    L"D3D12_CROSS_NODE_SHARING_TIER_1_EMULATED",
    L"D3D12_CROSS_NODE_SHARING_TIER_1",
    L"D3D12_CROSS_NODE_SHARING_TIER_2",
    L"D3D12_CROSS_NODE_SHARING_TIER_3",
};
static const wchar_t* D3D12_RESOURCE_HEAP_TIER_NAMES[] = {
    L"D3D12_RESOURCE_HEAP_TIER_1",
    L"D3D12_RESOURCE_HEAP_TIER_2",
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

static void PrintEnum(const wchar_t* name, uint32_t value, const wchar_t* const* names, size_t nameCount)
{
    PrintIndent();
    wprintf(L"%s = 0x%X", name, value);
    if(value < nameCount)
    {
        wprintf(L" (%s)", names[value]);
    }
    wprintf(L"\n");
}

static void Print_D3D12_FEATURE_DATA_D3D12_OPTIONS(const D3D12_FEATURE_DATA_D3D12_OPTIONS& options)
{
    Print_BOOL(  L"DoublePrecisionFloatShaderOps       ", options.DoublePrecisionFloatShaderOps);
    Print_BOOL(  L"OutputMergerLogicOp                 ", options.OutputMergerLogicOp);
    PrintEnum(   L"MinPrecisionSupport                 ", options.MinPrecisionSupport, D3D12_SHADER_MIN_PRECISION_SUPPORT_NAMES, _countof(D3D12_SHADER_MIN_PRECISION_SUPPORT_NAMES));
    PrintEnum(   L"TiledResourcesTier                  ", options.TiledResourcesTier, D3D12_TILED_RESOURCES_TIER_NAMES, _countof(D3D12_TILED_RESOURCES_TIER_NAMES));
    PrintEnum(   L"ResourceBindingTier                 ", options.ResourceBindingTier, D3D12_RESOURCE_BINDING_TIER_NAMES, _countof(D3D12_RESOURCE_BINDING_TIER_NAMES));
    Print_BOOL(  L"PSSpecifiedStencilRefSupported      ", options.PSSpecifiedStencilRefSupported);
    Print_BOOL(  L"TypedUAVLoadAdditionalFormats       ", options.TypedUAVLoadAdditionalFormats);
    Print_BOOL(  L"ROVsSupported                       ", options.ROVsSupported);
    PrintEnum(   L"ConservativeRasterizationTier       ", options.ConservativeRasterizationTier, D3D12_CONSERVATIVE_RASTERIZATION_TIER_NAMES, _countof(D3D12_CONSERVATIVE_RASTERIZATION_TIER_NAMES));
    Print_uint32(L"MaxGPUVirtualAddressBitsPerResource ", options.MaxGPUVirtualAddressBitsPerResource);
    Print_BOOL(  L"StandardSwizzle64KBSupported        ", options.StandardSwizzle64KBSupported);
    PrintEnum(   L"CrossNodeSharingTier                ", options.CrossNodeSharingTier, D3D12_CROSS_NODE_SHARING_TIER_NAMES, _countof(D3D12_CROSS_NODE_SHARING_TIER_NAMES));
    Print_BOOL(  L"CrossAdapterRowMajorTextureSupported", options.CrossAdapterRowMajorTextureSupported);
    Print_BOOL(  L"VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation", options.VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation);
    PrintEnum(   L"ResourceHeapTier                    ", options.ResourceHeapTier, D3D12_RESOURCE_HEAP_TIER_NAMES, _countof(D3D12_RESOURCE_HEAP_TIER_NAMES));
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

        wprintf(L"  Description           = %s\n", desc.Description);
        wprintf(L"  VendorId              = 0x%X\n", desc.VendorId);
        wprintf(L"  DeviceId              = 0x%X\n", desc.DeviceId);
        wprintf(L"  SubSysId              = 0x%X\n", desc.SubSysId);
        wprintf(L"  Revision              = 0x%X\n", desc.Revision);
        wprintf(L"  DedicatedVideoMemory  = %zu\n", desc.DedicatedVideoMemory);
        wprintf(L"  DedicatedSystemMemory = %zu\n", desc.DedicatedSystemMemory);
        wprintf(L"  SharedSystemMemory    = %zu\n", desc.SharedSystemMemory);
        wprintf(L"  AdapterLuid           = %08X-%08X\n", (uint32_t)desc.AdapterLuid.HighPart, (uint32_t)desc.AdapterLuid.LowPart);
        wprintf(L"  Flags                 = 0x%X\n", desc.Flags);
        if((desc.Flags & DXGI_ADAPTER_FLAG_REMOTE) != 0)
        {
            wprintf(L"    DXGI_ADAPTER_FLAG_REMOTE\n");
        }
        if((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
        {
            wprintf(L"    DXGI_ADAPTER_FLAG_SOFTWARE\n");
        }
        if(desc.Flags == 0)
        {
            wprintf(L"    DXGI_ADAPTER_FLAG_NONE\n");
        }

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
