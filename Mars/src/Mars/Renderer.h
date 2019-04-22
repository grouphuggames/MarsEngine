#pragma once
#include "Core.h"
#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include "d3dx12.h"
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

	// required for dx12 initialization
	constexpr u32 frame_count = 2;

	struct
	{
		u32 frame_index;
		CD3DX12_VIEWPORT viewport;
		CD3DX12_RECT scissor_rect;
		u32 rtv_descriptor_size;
		ID3D12Device5* rt_device = nullptr;
		ID3D12CommandQueue* command_queue = nullptr;
		IDXGISwapChain3* swap_chain = nullptr;
		ID3D12DescriptorHeap* rtv_heap = nullptr;
		ID3D12Resource* render_targets[frame_count];
		ID3D12CommandAllocator* command_allocator = nullptr;
		ID3D12RootSignature* root_signature = nullptr;
		ID3D12PipelineState* pipeline_state = nullptr;
		ID3D12GraphicsCommandList* command_list = nullptr;
		ID3D12Resource* vertex_buffer = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
		ID3D12Fence* fence = nullptr;
		u64 fence_value;
		HANDLE fence_event;
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

	void WaitForPreviousFrame()
	{
		HRESULT hr;
		const u64 fence = dx12_data.fence_value;

		hr = dx12_data.command_queue->Signal(dx12_data.fence, fence);
		dx12_data.fence_value++;

		if (dx12_data.fence->GetCompletedValue() < fence)
		{
			hr = dx12_data.fence->SetEventOnCompletion(fence, dx12_data.fence_event);
			if (FAILED(hr))
			{
				MARS_CORE_ERROR("Failed to set dx12 fence event...");
				return;
			}

			WaitForSingleObject(dx12_data.fence_event, INFINITE);
		}

		dx12_data.frame_index = dx12_data.swap_chain->GetCurrentBackBufferIndex();
	}

	void InitDX12()
	{
		HRESULT hr;

		dx12_data.frame_index = 0;
		dx12_data.viewport = CD3DX12_VIEWPORT(0.f, 0.f, (f32)game_state.width, (f32)game_state.height);
		dx12_data.scissor_rect = CD3DX12_RECT(0, 0, (LONG)game_state.width, (LONG)game_state.height);
		dx12_data.rtv_descriptor_size = 0;

#ifdef _DEBUG
		u32 dxgi_factory_flags = 0;
		ID3D12Debug* debug_controller = nullptr;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
		{
			debug_controller->EnableDebugLayer();
			dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif

		IDXGIFactory4* factory;
		hr = CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&factory));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create DXGI factory...");
			return;
		}

		IDXGIAdapter1* hardware_adapter = nullptr;
		
		IDXGIAdapter1* adapter = nullptr;
		for (u32 adapter_index = 0; factory->EnumAdapters1(adapter_index, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapter_index)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

			hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr);
			if (SUCCEEDED(hr))
			{
				break;
			}
		}

		hardware_adapter = adapter;
		hr = D3D12CreateDevice(hardware_adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&dx12_data.rt_device));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("DX12 device creation failed...");
			return;
		}

		D3D12_COMMAND_QUEUE_DESC queue_desc = {};
		queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		hr = dx12_data.rt_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&dx12_data.command_queue));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("DX12 command queue creation failed...");
			return;
		}

		DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
		swap_chain_desc.BufferCount = frame_count;
		swap_chain_desc.Width = game_state.width;
		swap_chain_desc.Height = game_state.height;
		swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swap_chain_desc.SampleDesc.Count = 1;

		IDXGISwapChain1* swap_chain = nullptr;
		hr = factory->CreateSwapChainForHwnd(dx12_data.command_queue, game_state.hwnd, &swap_chain_desc, nullptr, nullptr, &swap_chain);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("DX12 swapchain creation failed...");
			return;
		}

		hr = factory->MakeWindowAssociation(game_state.hwnd, DXGI_MWA_NO_ALT_ENTER);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("DX12 window association failed...");
			return;
		}

		dx12_data.swap_chain = (IDXGISwapChain3*)swap_chain;		// this is not how it's done in the sample...might be a point of failure
		dx12_data.frame_index = dx12_data.swap_chain->GetCurrentBackBufferIndex();

		D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
		rtv_heap_desc.NumDescriptors = frame_count;
		rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		hr = dx12_data.rt_device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&dx12_data.rtv_heap));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("DX12 descriptor heap creation failed...");
			return;
		}

		dx12_data.rtv_descriptor_size = dx12_data.rt_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(dx12_data.rtv_heap->GetCPUDescriptorHandleForHeapStart());
		for (u32 n = 0; n < frame_count; n++)
		{
			hr = dx12_data.swap_chain->GetBuffer(n, IID_PPV_ARGS(&dx12_data.render_targets[n]));
			if (FAILED(hr))
			{
				MARS_CORE_ERROR("DX12 swapchain buffer #", n, " creation failed...");
				return;
			}

			dx12_data.rt_device->CreateRenderTargetView(dx12_data.render_targets[n], nullptr, rtv_handle);
			rtv_handle.Offset(1, dx12_data.rtv_descriptor_size);
		}

		dx12_data.rt_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&dx12_data.command_allocator));
	}

	void InitDX12Scene()
	{
		HRESULT hr;

		CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc = {};
		root_signature_desc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ID3DBlob* signature = nullptr;
		ID3DBlob* error = nullptr;

		hr = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to serialize DX12 root signature...");
			return;
		}

		hr = dx12_data.rt_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&dx12_data.root_signature));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create DX12 root signature...");
			return;
		}

		ID3DBlob* vertex_shader;
		ID3DBlob* pixel_shader;

#ifdef _DEBUG
		u32 compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		u32 compile_flags = 0;
#endif

		hr = D3DCompileFromFile(L"..\\..\\Mars\\res\\DX12Test.shader", nullptr, nullptr, "VSMain", "vs_5_0", compile_flags, 0, &vertex_shader, &error);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to compile vertex shader...\n", (char*)error->GetBufferPointer());
			return;
		}

		hr = D3DCompileFromFile(L"..\\..\\Mars\\res\\DX12Test.shader", nullptr, nullptr, "PSMain", "ps_5_0", compile_flags, 0, &pixel_shader, &error);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to compile pixel shader...\n", (char*)error->GetBufferPointer());
			return;
		}

		D3D12_INPUT_ELEMENT_DESC input_element_descs[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(vec3), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
		pso_desc.InputLayout = { input_element_descs, _countof(input_element_descs) };
		pso_desc.pRootSignature = dx12_data.root_signature;
		pso_desc.VS = CD3DX12_SHADER_BYTECODE(vertex_shader);
		pso_desc.PS = CD3DX12_SHADER_BYTECODE(pixel_shader);
		pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pso_desc.DepthStencilState.DepthEnable = false;
		pso_desc.DepthStencilState.StencilEnable = false;
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pso_desc.SampleDesc.Count = 1;
		
		hr = dx12_data.rt_device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&dx12_data.pipeline_state));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create graphics pipeline state...");
			return;
		}

		hr = dx12_data.rt_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, dx12_data.command_allocator, dx12_data.pipeline_state, IID_PPV_ARGS(&dx12_data.command_list));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create DX12 command list...");
			return;
		}

		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to close DX12 command list...");
			return;
		}

		Vertex triangle_verts[] = 
		{
			Vertex(0.f, 0.25f * ((f32)game_state.width / (f32)game_state.height), 0.f, 1.f, 0.f, 0.f, 1.f),
			Vertex(0.25f, -0.25f * ((f32)game_state.width / (f32)game_state.height), 0.f, 0.f, 1.f, 0.f, 1.f),
			Vertex(-0.25f, -0.25f * ((f32)game_state.width / (f32)game_state.height), 0.f, 0.f, 0.f, 1.f, 1.f),
		};

		hr = dx12_data.rt_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeof(triangle_verts)), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&dx12_data.vertex_buffer));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create dx12 vertex buffer...");
			return;
		}

		u8* vertex_data_begin = nullptr;
		CD3DX12_RANGE read_range(0, 0);
		hr = dx12_data.vertex_buffer->Map(0, &read_range, reinterpret_cast<void**>(&vertex_data_begin));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to copy vertex data into vertex buffer...");
			return;
		}

		memcpy(vertex_data_begin, triangle_verts, sizeof(triangle_verts));
		dx12_data.vertex_buffer->Unmap(0, nullptr);

		dx12_data.vertex_buffer_view.BufferLocation = dx12_data.vertex_buffer->GetGPUVirtualAddress();
		dx12_data.vertex_buffer_view.StrideInBytes = sizeof(Vertex);
		dx12_data.vertex_buffer_view.SizeInBytes = sizeof(triangle_verts);

		hr = dx12_data.rt_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&dx12_data.fence));
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to create dx12 fence...");
			return;
		}

		dx12_data.fence_value = 1;

		dx12_data.fence_event = CreateEvent(nullptr, false, false, nullptr);
		if (dx12_data.fence_event == nullptr)
		{
			MARS_CORE_ERROR("Failed to create dx12 fence event...");
			return;
		}

		WaitForPreviousFrame();
	}

	void UpdateDX12Renderer() {}

	void PopulateCommandList()
	{
		HRESULT hr;

		hr = dx12_data.command_allocator->Reset();
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to reset dx12 command allocator...");
			return;
		}

		hr = dx12_data.command_list->Reset(dx12_data.command_allocator, dx12_data.pipeline_state);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to reset dx12 command list...");
			return;
		}

		dx12_data.command_list->SetGraphicsRootSignature(dx12_data.root_signature);
		dx12_data.command_list->RSSetViewports(1, &dx12_data.viewport);
		dx12_data.command_list->RSSetScissorRects(1, &dx12_data.scissor_rect);

		dx12_data.command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx12_data.render_targets[dx12_data.frame_index], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(dx12_data.rtv_heap->GetCPUDescriptorHandleForHeapStart(), dx12_data.frame_index, dx12_data.rtv_descriptor_size);
		dx12_data.command_list->OMSetRenderTargets(1, &rtv_handle, false, nullptr);

		const f32 clear_color[] = { 0.f, 0.f, 0.f, 1.f };
		dx12_data.command_list->ClearRenderTargetView(rtv_handle, clear_color, 0, nullptr);
		dx12_data.command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dx12_data.command_list->IASetVertexBuffers(0, 1, &dx12_data.vertex_buffer_view);
		dx12_data.command_list->DrawInstanced(3, 1, 0, 0);

		dx12_data.command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(dx12_data.render_targets[dx12_data.frame_index], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		hr = dx12_data.command_list->Close();
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to close command list...");
			return;
		}
	}

	void DX12Draw()
	{
		HRESULT hr;
		PopulateCommandList();

		ID3D12CommandList* pp_command_lists[] = { dx12_data.command_list };
		dx12_data.command_queue->ExecuteCommandLists(_countof(pp_command_lists), pp_command_lists);

		hr = dx12_data.swap_chain->Present(1, 0);
		if (FAILED(hr))
		{
			MARS_CORE_ERROR("Failed to present next image in swap chain...");
			return;
		}

		WaitForPreviousFrame();
	}

	void TerminateDX12()
	{
		CloseHandle(dx12_data.fence_event);
		dx12_data.rt_device->Release();
		dx12_data.command_queue->Release();
		dx12_data.swap_chain->Release();
		dx12_data.rtv_heap->Release();
		dx12_data.command_allocator->Release();
		dx12_data.root_signature->Release();
		dx12_data.pipeline_state->Release();
		dx12_data.command_list->Release();
		dx12_data.vertex_buffer->Release();
		dx12_data.fence->Release();

		for (s32 i = 0; i < frame_count; i++)
		{
			dx12_data.render_targets[frame_count]->Release();
		}
	}

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

	void InitDX11Scene()
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
}
