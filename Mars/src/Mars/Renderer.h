#pragma once
#include "Core.h"
#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "Debug.h"


namespace Mars
{
	struct
	{
		IDXGISwapChain* swap_chain = nullptr;
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* device_context = nullptr;
		ID3D11RenderTargetView* render_target_view = nullptr;
		ID3D11DepthStencilView* depth_stencil_view = nullptr;
		ID3D11Texture2D* depth_stencil_buffer = nullptr;

		// take out later
		ID3D11Buffer* vert_buffer = nullptr;
		ID3D11Buffer* index_buffer = nullptr;
		ID3D11VertexShader* vs = nullptr;
		ID3D11PixelShader* ps = nullptr;
		ID3D10Blob* vs_buffer = nullptr;
		ID3D10Blob* ps_buffer = nullptr;
		ID3D10Blob* error_message = nullptr;
		ID3D11InputLayout* vert_layout = nullptr;

		// we'll find out where to put this stuff later
		ID3D11Buffer* cb_per_object_buffer = nullptr;
	} dx11_data;

	struct Vertex
	{
		Vertex() = default;
		Vertex(f32 x, f32 y, f32 z, f32 r, f32 g, f32 b, f32 a) : position(x, y, z), color(r, g, b, a) {}

		vec3 position;
		vec4 color;
	};

	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} ,
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(vec3), D3D11_INPUT_PER_VERTEX_DATA, 0 }		// sizeof argument is there as an offset...if we were using XMFLOAT3 for position, offset would only be 12
	};

	mat4 cube1world;

	mat4 rotation;
	mat4 scale;
	mat4 translation;
	f32 rot = 0.01f;

	vec3 cube_position(0.f, 0.f, 4.f);

	mat4 wvp;
	mat4 world;
	mat4 cam_view;
	mat4 cam_projection;

	vec3 cam_position;
	vec3 cam_target;
	vec3 cam_up;

	struct cbPerObject
	{
		mat4 wvp;
	};

	cbPerObject cb_per_obj;

	void InitDX12();

	void InitDX11()
	{
		HRESULT hr;
		ADDHOT(cube_position);

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

		ID3D11Texture2D* back_buffer = nullptr;
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

		D3D11_TEXTURE2D_DESC depth_stencil_desc = {};
		depth_stencil_desc.Width = game_state.width;
		depth_stencil_desc.Height = game_state.height;
		depth_stencil_desc.MipLevels = 1;
		depth_stencil_desc.ArraySize = 1;
		depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_desc.SampleDesc.Count = 1;
		depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		dx11_data.device->CreateTexture2D(&depth_stencil_desc, nullptr, &dx11_data.depth_stencil_buffer);
		dx11_data.device->CreateDepthStencilView(dx11_data.depth_stencil_buffer, nullptr, &dx11_data.depth_stencil_view);

		dx11_data.device_context->OMSetRenderTargets(1, &dx11_data.render_target_view, nullptr);
	}

	void InitScene()
	{
		HRESULT hr;

		hr = D3DCompileFromFile(L"..\\..\\Mars\\res\\Test.shader", nullptr, nullptr, "VS", "vs_4_0", 0, 0, &dx11_data.vs_buffer, &dx11_data.error_message);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to compile vertex shader...\n", (char*)dx11_data.error_message->GetBufferPointer());
			return;
		}

		hr = D3DCompileFromFile(L"..\\..\\Mars\\res\\Test.shader", nullptr, nullptr, "PS", "ps_4_0", 0, 0, &dx11_data.ps_buffer, nullptr);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to compile pixel shader...\n", (char*)dx11_data.error_message->GetBufferPointer());
			return;
		}

		hr = dx11_data.device->CreateVertexShader(dx11_data.vs_buffer->GetBufferPointer(), dx11_data.vs_buffer->GetBufferSize(), nullptr, &dx11_data.vs);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create vertex shader...");
			return;
		}

		hr = dx11_data.device->CreatePixelShader(dx11_data.ps_buffer->GetBufferPointer(), dx11_data.ps_buffer->GetBufferSize(), nullptr, &dx11_data.ps);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create pixel shader...");
			return;
		}

		dx11_data.device_context->VSSetShader(dx11_data.vs, nullptr, 0);
		dx11_data.device_context->PSSetShader(dx11_data.ps, nullptr, 0);

		Vertex v[] = 
		{
			Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
			Vertex(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
			Vertex(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
			Vertex(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
			Vertex(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
			Vertex(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
			Vertex(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
			Vertex(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f)
		};

		DWORD indices[] =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		D3D11_BUFFER_DESC index_buffer_desc = {};
		index_buffer_desc.ByteWidth = 12 * 3 * sizeof(DWORD);
		index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA index_buffer_data = {};
		index_buffer_data.pSysMem = indices;
		hr = dx11_data.device->CreateBuffer(&index_buffer_desc, &index_buffer_data, &dx11_data.index_buffer);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create index buffer");
			return;
		}

		dx11_data.device_context->IASetIndexBuffer(dx11_data.index_buffer, DXGI_FORMAT_R32_UINT, 0);
		
		D3D11_BUFFER_DESC vertex_buffer_desc = {};
		vertex_buffer_desc.ByteWidth = 8 * sizeof(Vertex);
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertex_buffer_data = {};
		vertex_buffer_data.pSysMem = v;
		hr = dx11_data.device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &dx11_data.vert_buffer);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create vertex buffer");
			return;
		}

		u32 stride = sizeof(Vertex);
		u32 offset = 0;
		dx11_data.device_context->IASetVertexBuffers(0, 1, &dx11_data.vert_buffer, &stride, &offset);

		dx11_data.device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), dx11_data.vs_buffer->GetBufferPointer(), dx11_data.vs_buffer->GetBufferSize(), &dx11_data.vert_layout);
		dx11_data.device_context->IASetInputLayout(dx11_data.vert_layout);
		dx11_data.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D11_BUFFER_DESC cbbd = {};
		cbbd.ByteWidth = sizeof(cb_per_obj);
		cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hr = dx11_data.device->CreateBuffer(&cbbd, nullptr, &dx11_data.cb_per_object_buffer);

		cam_position = vec3(0.f, 3.f, -8.f);
		cam_target = vec3();
		cam_up = vec3(0.f, 1.f, 0.f);

		cam_view = mat4::LookAtLH(cam_position, cam_target, cam_up);
		cam_projection = mat4::PerspectiveFovLH(0.4f * PI, (f32)game_state.width / game_state.height, 1.f, 1000.f);
		world = mat4(1.f);
		wvp = world * cam_view * cam_projection;

		cb_per_obj.wvp = mat4::Transpose(wvp);
		dx11_data.device_context->UpdateSubresource(dx11_data.cb_per_object_buffer, 0, nullptr, &cb_per_obj, 0, 0);
		dx11_data.device_context->VSSetConstantBuffers(0, 1, &dx11_data.cb_per_object_buffer);

		D3D11_VIEWPORT viewport = {};
		viewport.Width = (f32)game_state.width;
		viewport.Height = (f32)game_state.height;
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;
		dx11_data.device_context->RSSetViewports(1, &viewport);
	}

	void UpdateRenderer()
	{
		rot += 0.0005f;
		if (rot > 2 * PI)
		{
			rot = 0.f;
		}

		cube1world = mat4(1.f);
		vec3 rot_axis(0.f, 1.f, 0.f);
		rotation = mat4::Rotate(rot_axis, rot);
		translation = mat4::Translate(cube_position);

		cube1world = rotation * translation;
	}

	void Draw()
	{
		f32 bg_color[4] = { 0.f, 0.f, 0.f, 1.f };
		dx11_data.device_context->ClearRenderTargetView(dx11_data.render_target_view, bg_color);
		dx11_data.device_context->ClearDepthStencilView(dx11_data.depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		wvp = cube1world * cam_view * cam_projection;
		cb_per_obj.wvp = mat4::Transpose(wvp);
		dx11_data.device_context->UpdateSubresource(dx11_data.cb_per_object_buffer, 0, nullptr, &cb_per_obj, 0, 0);
		dx11_data.device_context->VSSetConstantBuffers(0, 1, &dx11_data.cb_per_object_buffer);

		dx11_data.device_context->DrawIndexed(36, 0, 0);
		dx11_data.swap_chain->Present(0, 0);
	}

	void TerminateDX11()
	{
		dx11_data.swap_chain->Release();
		dx11_data.device->Release();
		dx11_data.device_context->Release();
		dx11_data.render_target_view->Release();
		dx11_data.vert_buffer->Release();
		dx11_data.index_buffer->Release();
		dx11_data.vs->Release();
		dx11_data.ps->Release();
		dx11_data.vs_buffer->Release();
		dx11_data.ps_buffer->Release();
		dx11_data.vert_layout->Release();
		dx11_data.depth_stencil_view->Release();
		dx11_data.depth_stencil_buffer->Release();
		dx11_data.cb_per_object_buffer->Release();
	}
}
