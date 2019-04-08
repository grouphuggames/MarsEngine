#pragma once
#include "Core.h"
#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include "vec3.h"


namespace Mars
{
	f32 red = 0.f;
	f32 green = 0.f;
	f32 blue = 0.f;
	s32 red_mod = 1;
	s32 green_mod = 1;
	s32 blue_mod = 1;

	struct
	{
		IDXGISwapChain* swap_chain = nullptr;
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* device_context = nullptr;
		ID3D11RenderTargetView* render_target_view = nullptr;

		// take out later
		ID3D11Buffer* triangle_vert_buffer;
		ID3D11VertexShader* vs;
		ID3D11PixelShader* ps;
		ID3D10Blob* vs_buffer;
		ID3D10Blob* ps_buffer;
		ID3D11InputLayout* vert_layout;
	} dx11_data;

	struct Vertex
	{
		Vertex() = default;
		Vertex(f32 x, f32 y, f32 z) : position(x, y, z) {}

		vec3 position;
	};

	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	void InitDX12()
	{
		HRESULT hr;
		IDXGIAdapter1* adapter = nullptr;
		ID3D12Device5* device = nullptr;
		hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device5), (void**)&device);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create DX12 device");
			return;
		}

		D3D12_FEATURE_DATA_D3D12_OPTIONS5 device_capabilities = {};
		hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &device_capabilities, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5));
		if (FAILED(hr) || device_capabilities.RaytracingTier < D3D12_RAYTRACING_TIER_1_0)
		{
			MARS_CORE_ERROR("Device or driver does not support DirectX Raytracing...");
			return;
		}
	}

	void InitDX11()
	{
		HRESULT hr;

		DXGI_MODE_DESC buffer_descriptor = {};
		buffer_descriptor.Width = game_state.width;
		buffer_descriptor.Height = game_state.height;
		buffer_descriptor.RefreshRate.Numerator = 60;
		buffer_descriptor.RefreshRate.Denominator = 1;
		buffer_descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		DXGI_SWAP_CHAIN_DESC swap_chain_descriptor = {};
		swap_chain_descriptor.BufferCount = 1;		// double buffering
		swap_chain_descriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_descriptor.OutputWindow = game_state.hwnd;
		swap_chain_descriptor.SampleDesc.Count = 1;		//msaa
		swap_chain_descriptor.Windowed = true;

		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_descriptor, &dx11_data.swap_chain, &dx11_data.device, nullptr, &dx11_data.device_context);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create DX11 device and/or swapchain");
			return;
		}

		ID3D11Texture2D* back_buffer;
		hr = dx11_data.swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to get DX11 back buffer");
			return;
		}

		hr = dx11_data.device->CreateRenderTargetView(back_buffer, nullptr, &dx11_data.render_target_view);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create render target view");
			return;
		}
		back_buffer->Release();
		dx11_data.device_context->OMSetRenderTargets(1, &dx11_data.render_target_view, nullptr);
	}

	void InitScene()
	{
		HRESULT hr;

		hr = D3DCompileFromFile(L"..\\..\\Mars\\res\\Test.shader", nullptr, nullptr, "VS", "vs_4_0", 0, 0, &dx11_data.vs_buffer, nullptr);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to compile vertex shader...");
		}

		hr = D3DCompileFromFile(L"..\\..\\Mars\\res\\Test.shader", nullptr, nullptr, "PS", "ps_4_0", 0, 0, &dx11_data.ps_buffer, nullptr);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to compile pixel shader...");
		}

		hr = dx11_data.device->CreateVertexShader(dx11_data.vs_buffer->GetBufferPointer(), dx11_data.vs_buffer->GetBufferSize(), nullptr, &dx11_data.vs);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create vertex shader...");
		}

		hr = dx11_data.device->CreatePixelShader(dx11_data.ps_buffer->GetBufferPointer(), dx11_data.ps_buffer->GetBufferSize(), nullptr, &dx11_data.ps);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create pixel shader...");
		}

		dx11_data.device_context->VSSetShader(dx11_data.vs, nullptr, 0);
		dx11_data.device_context->PSSetShader(dx11_data.ps, nullptr, 0);

		Vertex v[] = 
		{
			Vertex(0.f, 0.5f, 0.5f),
			Vertex(0.5f, -0.5f, 0.5f),
			Vertex(-0.5f, -0.5f, 0.5f)
		};
		
		D3D11_BUFFER_DESC vertex_buffer_desc = {};
		vertex_buffer_desc.ByteWidth = 3 * sizeof(Vertex);
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertex_buffer_data = {};
		vertex_buffer_data.pSysMem = v;
		hr = dx11_data.device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &dx11_data.triangle_vert_buffer);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create vertex buffer");
		}

		u32 stride = sizeof(Vertex);
		u32 offset = 0;
		dx11_data.device_context->IASetVertexBuffers(0, 1, &dx11_data.triangle_vert_buffer, &stride, &offset);
		
		dx11_data.device->CreateInputLayout(layout, ARRAYSIZE(layout), dx11_data.vs_buffer->GetBufferPointer(), dx11_data.vs_buffer->GetBufferSize(), &dx11_data.vert_layout);
		dx11_data.device_context->IASetInputLayout(dx11_data.vert_layout);
		dx11_data.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D11_VIEWPORT viewport = {};
		viewport.Width = game_state.width;
		viewport.Height = game_state.height;
		dx11_data.device_context->RSSetViewports(1, &viewport);
	}

	void UpdateRenderer()
	{
		red += red_mod * 0.00005f;
		green += green_mod * 0.00002f;
		blue += blue_mod * 0.00001f;

		if (red >= 1.f || red <= 0.f)
		{
			red_mod *= -1;
		}
		if (green >= 1.f || green <= 0.f)
		{
			green_mod *= -1;
		}
		if (blue >= 1.f || blue <= 0.f)
		{
			blue_mod *= -1;
		}
	}

	void Draw()
	{
		float bg_color[4] = { red, green, blue, 1.f };
		dx11_data.device_context->ClearRenderTargetView(dx11_data.render_target_view, bg_color);
		dx11_data.device_context->Draw(3, 0);
		dx11_data.swap_chain->Present(0, 0);
	}

	void TerminateDX11()
	{
		dx11_data.swap_chain->Release();
		dx11_data.device->Release();
		dx11_data.device_context->Release();

		return;
	}
}
