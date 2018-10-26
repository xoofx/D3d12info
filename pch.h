#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <atlbase.h> // for CComPtr

#include <vector>
#include <string>

#include <cstdint>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#define CHECK_HR(expr)   do { HRESULT hr__ = (expr); if(FAILED(hr__)) assert(0 && #expr); } while(false)

#define SAFE_RELEASE(x)   do { if(x) { (x)->Release(); (x) = nullptr; } } while(false)
