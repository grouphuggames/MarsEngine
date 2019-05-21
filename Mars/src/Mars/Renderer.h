#pragma once
#include "Core.h"
#if 0
#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include "d3dx12.h"
#endif
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "Debug.h"

#define WIN32_CHECK(result) if (result < 0) { MARS_CORE_ERROR(#result, " was not properly executed..."); }


namespace Mars
{
	// OpenGL & Vulkan
	void InitGL()
	{
		
	}

	void InitGLScene()
	{

	}

#if 0
	// DirectX 11 & 12
	struct Viewport
	{
		f32 left;
		f32 top;
		f32 right;
		f32 bottom;
	};

	struct RayGenConstantBuffer
	{
		Viewport viewport;
		Viewport stencil;
	};

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

	// required for dx12 initialization
	constexpr u32 back_buffer_count = 3;

	struct
	{
		IDXGIFactory4* dxgi_factory = nullptr;
		u32 adapter_index;
		DXGI_ADAPTER_DESC1 adapter_description;
		IDXGIAdapter1* adapter = nullptr;
		ID3D12Device* device = nullptr;
		ID3D12CommandQueue* command_queue = nullptr;
		ID3D12DescriptorHeap* rtv_descriptor_heap = nullptr;
		u32 rtv_descriptor_size;
		ID3D12CommandAllocator* command_allocators[back_buffer_count];
		ID3D12GraphicsCommandList* command_list = nullptr;
		u64 fence_values[back_buffer_count];
		HANDLE fence_event;
		u32 back_buffer_index;
		ID3D12Fence* fence = nullptr;
		ID3D12Resource* render_targets[back_buffer_count];
		IDXGISwapChain3* swapchain = nullptr;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissor_rect;
		ID3D12Device5* dxr_device = nullptr;
		ID3D12GraphicsCommandList4* dxr_command_list = nullptr;
		ID3D12RootSignature* raytracing_global_root_signature;
		ID3D12RootSignature* raytracing_local_root_signature;
		RayGenConstantBuffer ray_gen_cb;
	} dx12_data;

	struct Vertex
	{
		Vertex() = default;
		Vertex(f32 x, f32 y, f32 z, f32 r, f32 g, f32 b, f32 a) : position(x, y, z), color(r, g, b, a) {}
	
		vec3 position;
		vec4 color;
	};

	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
	s32 add_hot_test = 0;

	void WaitForGPU()
	{
		u64 fence_value = dx12_data.fence_values[dx12_data.back_buffer_index];
		WIN32_CHECK(dx12_data.command_queue->Signal(dx12_data.fence, fence_value));
		WIN32_CHECK(dx12_data.fence->SetEventOnCompletion(fence_value, dx12_data.fence_event));
		WaitForSingleObjectEx(dx12_data.fence_event, INFINITE, false);
		dx12_data.fence_values[dx12_data.back_buffer_index]++;
	}

	void ResizeWindowDX12()
	{
		WaitForGPU();

		for (u32 i = 0; i < back_buffer_count; i++)
		{
			if (dx12_data.render_targets[i] != nullptr)
			{
				dx12_data.render_targets[i]->Release();
				dx12_data.render_targets[i] = nullptr;
			}

			dx12_data.fence_values[i] = dx12_data.fence_values[dx12_data.back_buffer_index];
		}

		if (dx12_data.swapchain)
		{
			WIN32_CHECK(dx12_data.swapchain->ResizeBuffers(back_buffer_count, game_state.width, game_state.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {};
			swapchain_desc.Width = game_state.width;
			swapchain_desc.Height = game_state.height;
			swapchain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapchain_desc.BufferCount = back_buffer_count;
			swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchain_desc.SampleDesc.Count = 1;
			swapchain_desc.Scaling = DXGI_SCALING_STRETCH;
			swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapchain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

			IDXGISwapChain1* swapchain = nullptr;
			WIN32_CHECK(dx12_data.dxgi_factory->CreateSwapChainForHwnd(dx12_data.command_queue, game_state.hwnd, &swapchain_desc, nullptr, nullptr, &swapchain));
			dx12_data.swapchain = (IDXGISwapChain3*)swapchain;

			for (u32 i = 0; i < back_buffer_count; i++)
			{
				WIN32_CHECK(dx12_data.swapchain->GetBuffer(i, IID_PPV_ARGS(&dx12_data.render_targets[i])));
				wchar_t name[25] = {};
				swprintf_s(name, L"Render target %u", i);
				dx12_data.render_targets[i]->SetName(name);

				D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
				rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

				CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_descriptor(dx12_data.rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart(), i, dx12_data.rtv_descriptor_size);
				dx12_data.device->CreateRenderTargetView(dx12_data.render_targets[i], &rtv_desc, rtv_descriptor);
			}

			dx12_data.back_buffer_index = dx12_data.swapchain->GetCurrentBackBufferIndex();

			dx12_data.viewport.TopLeftX = 0.f;
			dx12_data.viewport.TopLeftY = 0.f;
			dx12_data.viewport.Width = (f32)game_state.width;
			dx12_data.viewport.Height = (f32)game_state.height;
			dx12_data.viewport.MinDepth = D3D12_MIN_DEPTH;
			dx12_data.viewport.MaxDepth = D3D12_MAX_DEPTH;

			dx12_data.scissor_rect.left = 0;
			dx12_data.scissor_rect.top = 0;
			dx12_data.scissor_rect.right = game_state.width;
			dx12_data.scissor_rect.bottom = game_state.height;
		}
	}

	void InitDX12()
	{
		f32 aspect_ratio = ((f32)game_state.width / (f32)game_state.height);
		dx12_data.ray_gen_cb.viewport = { -1.f, -1.f, 1.f, 1.f };
		dx12_data.ray_gen_cb.stencil = { -1.f + 0.1f / aspect_ratio, -1.f + 0.1f, 1.f - 0.1f / aspect_ratio, 1.f - 0.1f };

		// create dxgi factory
		u32 dxgi_factory_flags = 0;

#ifdef _DEBUG
		ID3D12Debug* debug_controller = nullptr;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
		{
			debug_controller->EnableDebugLayer();
			dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif

		WIN32_CHECK(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&dx12_data.dxgi_factory)));

		// find appropriate hardware adapter
		IDXGIAdapter1* tmp_adapter = nullptr;
		for (u32 adapter_index = 0; dx12_data.dxgi_factory->EnumAdapters1(adapter_index, &tmp_adapter) != DXGI_ERROR_NOT_FOUND; ++adapter_index)
		{
			DXGI_ADAPTER_DESC1 desc;
			WIN32_CHECK(tmp_adapter->GetDesc1(&desc));

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

			WIN32_CHECK(D3D12CreateDevice(tmp_adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr));
			dx12_data.adapter_index = adapter_index;
			dx12_data.adapter_description = desc;

			dx12_data.adapter = tmp_adapter;
		}

		// create device with dxr capabilities
		ID3D12Device* tmp_device = nullptr;
		D3D12_FEATURE_DATA_D3D12_OPTIONS5 feature_support_data = {};

		WIN32_CHECK(D3D12CreateDevice(dx12_data.adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&tmp_device)));
		WIN32_CHECK(tmp_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &feature_support_data, sizeof(feature_support_data)));
		if (feature_support_data.RaytracingTier != D3D12_RAYTRACING_TIER_1_0) { MARS_CORE_ERROR("This physical device does not support DXR..."); }

		WIN32_CHECK(D3D12CreateDevice(dx12_data.adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&dx12_data.device)));

		// create command queue
		D3D12_COMMAND_QUEUE_DESC queue_desc = {};
		queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		WIN32_CHECK(dx12_data.device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&dx12_data.command_queue)));

		// create descriptor heaps for render target view
		D3D12_DESCRIPTOR_HEAP_DESC rtv_descriptor_heap_desc = {};
		rtv_descriptor_heap_desc.NumDescriptors = back_buffer_count;
		rtv_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		WIN32_CHECK(dx12_data.device->CreateDescriptorHeap(&rtv_descriptor_heap_desc, IID_PPV_ARGS(&dx12_data.rtv_descriptor_heap)));
		dx12_data.rtv_descriptor_size = dx12_data.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// create command allocators
		for (u8 i = 0; i < back_buffer_count; i++)
		{
			WIN32_CHECK(dx12_data.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&dx12_data.command_allocators[i])));
		}

		// create command list for graphics
		WIN32_CHECK(dx12_data.device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, dx12_data.command_allocators[0], nullptr, IID_PPV_ARGS(&dx12_data.command_list)));
		WIN32_CHECK(dx12_data.command_list->Close());

		// create a fence
		WIN32_CHECK(dx12_data.device->CreateFence(dx12_data.fence_values[dx12_data.back_buffer_index], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&dx12_data.fence)));
		dx12_data.fence_values[dx12_data.back_buffer_index]++;

		dx12_data.fence_event = CreateEvent(nullptr, false, false, nullptr);

		ResizeWindowDX12();

		// init the ray tracing pipeline
		WIN32_CHECK(dx12_data.device->QueryInterface(IID_PPV_ARGS(&dx12_data.dxr_device)));
		WIN32_CHECK(dx12_data.command_list->QueryInterface(IID_PPV_ARGS(&dx12_data.dxr_command_list)));

		CD3DX12_DESCRIPTOR_RANGE uav_descriptor;
		uav_descriptor.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

		ID3DBlob* blob;
		ID3DBlob* error;
		
		{
			CD3DX12_ROOT_PARAMETER root_parameters[2];
			root_parameters[0].InitAsDescriptorTable(1, &uav_descriptor);
			root_parameters[1].InitAsShaderResourceView(0);

			CD3DX12_ROOT_SIGNATURE_DESC global_root_signature_desc(ARRAYSIZE(root_parameters), root_parameters);
			WIN32_CHECK(D3D12SerializeRootSignature(&global_root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error));
			WIN32_CHECK(dx12_data.device->CreateRootSignature(1, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&dx12_data.raytracing_global_root_signature)));
		}

		{
			CD3DX12_ROOT_PARAMETER root_parameters[1];
			root_parameters[0].InitAsConstants(((sizeof(dx12_data.ray_gen_cb) - 1) / sizeof(UINT32) + 1), 0, 0);

			CD3DX12_ROOT_SIGNATURE_DESC local_root_signature_desc(ARRAYSIZE(root_parameters), root_parameters);
			local_root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
			WIN32_CHECK(D3D12SerializeRootSignature(&local_root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error));
			WIN32_CHECK(dx12_data.device->CreateRootSignature(1, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&dx12_data.raytracing_local_root_signature)));
		}


	}

	void InitDX12Scene()
	{
	}

	void UpdateDX12Renderer() {}

	void DX12Draw()
	{
	}

	void TerminateDX12()
	{
	}

	void InitDX11()
	{
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

		WIN32_CHECK(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_descriptor, &dx11_data.swap_chain, &dx11_data.device, nullptr, &dx11_data.device_context));

		ID3D11Texture2D* back_buffer = nullptr;
		WIN32_CHECK(dx11_data.swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer));

		WIN32_CHECK(dx11_data.device->CreateRenderTargetView(back_buffer, nullptr, &dx11_data.render_target_view));
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

	void InitDX11Scene()
	{
		WIN32_CHECK(D3DCompileFromFile(L"..\\..\\Mars\\res\\Test.shader", nullptr, nullptr, "VS", "vs_4_0", 0, 0, &dx11_data.vs_buffer, &dx11_data.error_message));
		WIN32_CHECK(D3DCompileFromFile(L"..\\..\\Mars\\res\\Test.shader", nullptr, nullptr, "PS", "ps_4_0", 0, 0, &dx11_data.ps_buffer, nullptr));

		WIN32_CHECK(dx11_data.device->CreateVertexShader(dx11_data.vs_buffer->GetBufferPointer(), dx11_data.vs_buffer->GetBufferSize(), nullptr, &dx11_data.vs));
		WIN32_CHECK(dx11_data.device->CreatePixelShader(dx11_data.ps_buffer->GetBufferPointer(), dx11_data.ps_buffer->GetBufferSize(), nullptr, &dx11_data.ps));

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
		WIN32_CHECK(dx11_data.device->CreateBuffer(&index_buffer_desc, &index_buffer_data, &dx11_data.index_buffer));

		dx11_data.device_context->IASetIndexBuffer(dx11_data.index_buffer, DXGI_FORMAT_R32_UINT, 0);
		
		D3D11_BUFFER_DESC vertex_buffer_desc = {};
		vertex_buffer_desc.ByteWidth = 8 * sizeof(Vertex);
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertex_buffer_data = {};
		vertex_buffer_data.pSysMem = v;
		WIN32_CHECK(dx11_data.device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &dx11_data.vert_buffer));

		u32 stride = sizeof(Vertex);
		u32 offset = 0;
		dx11_data.device_context->IASetVertexBuffers(0, 1, &dx11_data.vert_buffer, &stride, &offset);

		dx11_data.device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), dx11_data.vs_buffer->GetBufferPointer(), dx11_data.vs_buffer->GetBufferSize(), &dx11_data.vert_layout);
		dx11_data.device_context->IASetInputLayout(dx11_data.vert_layout);
		dx11_data.device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D11_BUFFER_DESC cbbd = {};
		cbbd.ByteWidth = sizeof(cb_per_obj);
		cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		WIN32_CHECK(dx11_data.device->CreateBuffer(&cbbd, nullptr, &dx11_data.cb_per_object_buffer));

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

	void UpdateDX11Renderer()
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

	void DX11Draw()
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
#endif
}
