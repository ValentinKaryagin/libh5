#include "render_device_d3d11.h"

#include "application_window_windows.h"

#include "texture2d_d3d11.h"
#include "vertex_shader_d3d11.h"
#include "pixel_shader_d3d11.h"
#include "shader_res_d3d11.h"
#include "sprite_font_d3d11.h"

#include "vertex.h"

#include "d3d11_private.h"

namespace h5
{
	render_device_d3d11::render_device_d3d11() {}

	render_device_d3d11::~render_device_d3d11()
	{
		terminate();
	}

	int32_t render_device_d3d11::init(application_window_windows *window, bool is_fullscreen, bool is_vsync_enabled)
	{
		if (!window)
		{
			return ERRLVL_D3D11_NULL_WINDOW;
		}

		uint32_t x = 0, y = 0;
		window->get_canvas_size(&x, &y);

		DXGI_MODE_DESC md{};
		md.Width = x;
		md.Height = y;
		md.RefreshRate.Numerator = 60;
		md.RefreshRate.Denominator = 1;
		md.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		md.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		md.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SWAP_CHAIN_DESC scd{};
		scd.BufferDesc = md;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = (HWND)window->get_win32_handler();
		scd.Windowed = is_fullscreen ? FALSE : TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		//scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// todo: why DXGI_SWAP_EFFECT_DISCARD is legacy?
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

#ifdef _DEBUG
		UINT flags = (UINT)D3D11_CREATE_DEVICE_DEBUG;
#else
		UINT flags = 0;
#endif

		if (FAILED(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			flags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&scd,
			&swap_chain,
			&device,
			nullptr,
			&device_context)))
		{
			return ERRLVL_D3D11_CREATE_DEVICE_AND_SWAPCHAIN;
		}

		ID3D11Texture2D *bb = nullptr;
		if (FAILED(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&bb))))
		{
			return ERRLVL_D3D11_SWAPCHAIN_GET_BUFFER;
		}

		if (FAILED(device->CreateRenderTargetView(bb, nullptr, &rtv)))
		{
			return ERRLVL_D3D11_DEVICE_CREATE_RTV;
		}

		bb->Release();

		device_context->OMSetRenderTargets(1, &rtv, nullptr);

		D3D11_TEXTURE2D_DESC dstd{};
		dstd.Width = x;
		dstd.Height = y;
		dstd.MipLevels = 1;
		dstd.ArraySize = 1;
		dstd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dstd.SampleDesc.Count = 1;
		dstd.SampleDesc.Quality = 0;
		dstd.Usage = D3D11_USAGE_DEFAULT;
		dstd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dstd.CPUAccessFlags = 0;
		dstd.MiscFlags = 0;

		if (FAILED(device->CreateTexture2D(&dstd, nullptr, &ds_buffer)))
		{
			return ERRLVL_D3D11_CREATE_TEXTURE2D;
		}
				
		if (FAILED(device->CreateDepthStencilView(ds_buffer, nullptr, &dsv)))
		{
			return ERRLVL_D3D11_CREATE_DEPTH_STENCIL_VIEW;
		}

		device_context->OMSetRenderTargets(1, &rtv, dsv);

		D3D11_VIEWPORT vp{};
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = static_cast<float>(x);
		vp.Height = static_cast<float>(y);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		device_context->RSSetViewports(1, &vp);		

		device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		is_vsync = is_vsync_enabled;

		D3D11_RASTERIZER_DESC slrs{};
		slrs.FillMode = D3D11_FILL_SOLID;
		slrs.CullMode = D3D11_CULL_BACK;
		slrs.MultisampleEnable = FALSE;
		slrs.AntialiasedLineEnable = FALSE;
		slrs.DepthClipEnable = TRUE;
		if (FAILED(device->CreateRasterizerState(&slrs, &solid_rs)))
		{
			return ERRLVL_D3D11_CREATE_RASTERIZER_STATE;
		}

		D3D11_RASTERIZER_DESC wfrs{};
		wfrs.FillMode = D3D11_FILL_WIREFRAME;
		wfrs.CullMode = D3D11_CULL_NONE;
		wfrs.MultisampleEnable = FALSE;
		wfrs.AntialiasedLineEnable = FALSE;
		wfrs.DepthClipEnable = TRUE;
		if (FAILED(device->CreateRasterizerState(&wfrs, &wireframe_rs)))
		{
			return ERRLVL_D3D11_CREATE_RASTERIZER_STATE;
		}

		device_context->RSSetState(solid_rs);

		CD3D11_DEFAULT sampler_default;
		CD3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(sampler_default);
		D3D11_SAMPLER_DESC sd = sampler_desc;
		if (FAILED(device->CreateSamplerState(&sd, &sampler_state)))
		{
			return ERRLVL_D3D11_CREATE_SAMPLER_STATE;
		}

		device_context->PSSetSamplers(0, 1, &sampler_state);

		sprite_batch = new(std::nothrow) DirectX::SpriteBatch(device_context);
		if (!sprite_batch)
		{
			return ERRLVL_DX_MEMORY_ALLOCATE;
		}

		return 0;
	}

	void render_device_d3d11::terminate()
	{
		D3D11_SAFE_DELETE(sprite_batch);

		D3D11_SAFE_RELEASE(sampler_state);
		D3D11_SAFE_RELEASE(wireframe_rs);
		D3D11_SAFE_RELEASE(solid_rs);
		D3D11_SAFE_RELEASE(dsv);
		D3D11_SAFE_RELEASE(ds_buffer);
		D3D11_SAFE_RELEASE(layout);
		D3D11_SAFE_RELEASE(rtv);
		D3D11_SAFE_RELEASE(swap_chain);
		D3D11_SAFE_RELEASE(device_context);
		D3D11_SAFE_RELEASE(device);
	}

	void render_device_d3d11::enable_vsync(bool is_enabled)
	{
		is_vsync = is_enabled;
	}

	void render_device_d3d11::clear(const colorf_rgba &color)
	{
#ifdef _DEBUG
		objects_to_draw_count = 0;
		draw_calls_count = 0;
#endif

		device_context->ClearRenderTargetView(rtv, reinterpret_cast<const float *>(&color));
		device_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}		

	int32_t render_device_d3d11::present() const
	{
		if (FAILED(swap_chain->Present(is_vsync ? 1 : 0, 0)))
		{
			return ERRLVL_D3D11_PRESENT;
		}

		return 0;
	}

	void render_device_d3d11::draw(size_t vertices_count)
	{
#ifdef _DEBUG
		++objects_to_draw_count;
		++draw_calls_count;
#endif

		device_context->Draw(static_cast<uint32_t>(vertices_count), 0);
	}

	void render_device_d3d11::draw_indexed(size_t indices_count)
	{
#ifdef _DEBUG
		++objects_to_draw_count;
		++draw_calls_count;
#endif

		device_context->DrawIndexed(static_cast<uint32_t>(indices_count), 0, 0);
	}

	void render_device_d3d11::draw_instanced(size_t vertices_count_per_instance, size_t instances_count)
	{
#ifdef _DEBUG
		objects_to_draw_count += instances_count;
		++draw_calls_count;
#endif

		device_context->DrawInstanced(
			static_cast<uint32_t>(vertices_count_per_instance),
			static_cast<uint32_t>(instances_count),
			0,
			0);
	}

	void render_device_d3d11::draw_indexed_instanced(size_t indices_count_per_instance, size_t instances_count)
	{
#ifdef _DEBUG
		objects_to_draw_count += instances_count;
		++draw_calls_count;
#endif

		device_context->DrawIndexedInstanced(
			static_cast<uint32_t>(indices_count_per_instance),
			static_cast<uint32_t>(instances_count),
			0,
			0,
			0);
	}

	void render_device_d3d11::draw_text(const sprite_font_d3d11 *font, const std::wstring &text, const vector2 &coord, const colorf_rgba &color)
	{
		sprite_batch->Begin();
		font->font->DrawString(sprite_batch, text.c_str(), coord, DirectX::FXMVECTOR{ { color.r, color.g, color.b, color.a } });
		sprite_batch->End();
	}

	void render_device_d3d11::draw_text(const sprite_font_d3d11 *font, const std::string &text, const vector2 &coord, const colorf_rgba &color)
	{
		sprite_batch->Begin();
		font->font->DrawString(sprite_batch, text.c_str(), coord, DirectX::FXMVECTOR{ { color.r, color.g, color.b, color.a } });
		sprite_batch->End();
	}

	int32_t render_device_d3d11::create_buffer(render_buffer_d3d11::type type, void *data, size_t elements_count, size_t element_size, render_buffer_d3d11 *out) const
	{
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = static_cast<uint32_t>(elements_count * element_size);
		bd.BindFlags =
			type == render_buffer_d3d11::type::vertex ? D3D11_BIND_VERTEX_BUFFER :
			type == render_buffer_d3d11::type::index ? D3D11_BIND_INDEX_BUFFER :
			type == render_buffer_d3d11::type::constant ? D3D11_BIND_CONSTANT_BUFFER :
			type == render_buffer_d3d11::type::structured_buffer ? D3D11_BIND_SHADER_RESOURCE :
			0;
		bd.MiscFlags = type == render_buffer_d3d11::type::structured_buffer ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : 0;
		bd.StructureByteStride = static_cast<uint32_t>(element_size);

		D3D11_SUBRESOURCE_DATA srd{};
		srd.pSysMem = data;

		if (FAILED(device->CreateBuffer(&bd, &srd, &out->buffer)))
		{
			return ERRLVL_D3D11_CREATE_BUFFER;
		}

		return 0;
	}

	int32_t render_device_d3d11::create_shader_res(render_buffer_d3d11 *buffer, size_t elements_count, shader_res_d3d11 *out) const
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvd.BufferEx.NumElements = static_cast<uint32_t>(elements_count);
		if (FAILED(device->CreateShaderResourceView(buffer->buffer, &srvd, &out->shader_res)))
		{
			return ERRLVL_D3D11_CREATE_SHADER_RESOURCE_VIEW;
		}

		return 0;
	}

	int32_t render_device_d3d11::create_shader_res(texture2d_d3d11 *textures, shader_res_d3d11 *out) const
	{
		if (FAILED(DirectX::CreateShaderResourceView(
			device,
			textures->image->GetImages(),
			textures->image->GetImageCount(),
			textures->image->GetMetadata(),
			&out->shader_res)))
		{
			return ERRLVL_DX_CREATE_SHADER_RESOURCE_VIEW;
		}

		return 0;
	}

	int32_t render_device_d3d11::create_vertex_shader(vertex_shader_d3d11 *out)
	{
		if (FAILED(device->CreateVertexShader(out->buffer->GetBufferPointer(), out->buffer->GetBufferSize(), nullptr, &out->shader)))
		{
			return ERRLVL_D3D11_CREATE_VERTEX_SHADER;
		}

		return 0;
	}

	int32_t render_device_d3d11::create_pixel_shader(pixel_shader_d3d11 *out)
	{
		if (FAILED(device->CreatePixelShader(out->buffer->GetBufferPointer(), out->buffer->GetBufferSize(), nullptr, &out->shader)))
		{
			return ERRLVL_D3D11_CREATE_PIXEL_SHADER;
		}

		return 0;
	}

	int32_t render_device_d3d11::create_sprite_font(const std::wstring &filename, sprite_font_d3d11 *out)
	{
		out->font = new (std::nothrow) DirectX::SpriteFont(device, filename.c_str());
		if (!out->font)
		{
			return ERRLVL_D3D11_CREATE_SPRITEFONT;
			//return ERRLVL_DX_MEMORY_ALLOCATE;
		}

		return 0;
	}

	void render_device_d3d11::set_vertex_buffer(render_buffer_d3d11 *buffer)
	{
		const uint32_t vertex_stride = sizeof(vertex);
		const uint32_t offset = 0;
		device_context->IASetVertexBuffers(0, 1, &buffer->buffer, &vertex_stride, &offset);
	}

	void render_device_d3d11::set_index_buffer(render_buffer_d3d11 *buffer)
	{
		device_context->IASetIndexBuffer(buffer->buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void render_device_d3d11::set_vertex_shader(vertex_shader_d3d11 *shader)
	{
		device_context->VSSetShader(shader->shader, nullptr, 0);

		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },			
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		D3D11_SAFE_RELEASE(layout);

		if (FAILED(device->CreateInputLayout(ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), shader->buffer->GetBufferPointer(), shader->buffer->GetBufferSize(), &layout)))
		{
			DEBUG_PRINT_FILELINE();
			//ERRLVL_D3D11_CREATE_INPUT_LAYOUT
		}

		device_context->IASetInputLayout(layout);
	}

	void render_device_d3d11::set_pixel_shader(pixel_shader_d3d11 *shader)
	{
		device_context->PSSetShader(shader->shader, nullptr, 0);
	}	

	void render_device_d3d11::update_constant_buffer(render_buffer_d3d11 *buffer, void *data, size_t index) const
	{
		device_context->UpdateSubresource(buffer->buffer, static_cast<uint32_t>(index), nullptr, data, 0, 0);
	}

	void render_device_d3d11::set_vertex_shader_constant_buffer(render_buffer_d3d11 *buffer, size_t index)
	{
		device_context->VSSetConstantBuffers(static_cast<uint32_t>(index), 1, &buffer->buffer);
	}

	void render_device_d3d11::set_pixel_shader_constant_buffer(render_buffer_d3d11 *buffer, size_t index)
	{
		device_context->PSSetConstantBuffers(static_cast<uint32_t>(index), 1, &buffer->buffer);
	}

	void render_device_d3d11::set_vertex_shader_res(shader_res_d3d11 *res, size_t index)
	{
		device_context->VSSetShaderResources(static_cast<uint32_t>(index), 1, &res->shader_res);
	}

	void render_device_d3d11::set_pixel_shader_res(shader_res_d3d11 *res, size_t index)
	{
		device_context->PSSetShaderResources(static_cast<uint32_t>(index), 1, &res->shader_res);
	}

	void render_device_d3d11::set_render_state(state rs)
	{
		device_context->RSSetState(rs == state::solid ? solid_rs : wireframe_rs);
	}

	int32_t render_device_d3d11::resize_viewport(uint32_t x, uint32_t y)
	{
		device_context->OMSetRenderTargets(0, nullptr, nullptr);
			
		rtv->Release();

		if (FAILED(swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)))
		{
			return ERRLVL_D3D11_RESIZE_BUFFERS;
		}

		ID3D11Texture2D *bb = nullptr;
		if (FAILED(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&bb))))
		{
			return ERRLVL_D3D11_SWAPCHAIN_GET_BUFFER;
		}

		if (FAILED(device->CreateRenderTargetView(bb, nullptr, &rtv)))
		{
			return ERRLVL_D3D11_DEVICE_CREATE_RTV;
		}

		bb->Release();

		device_context->OMSetRenderTargets(1, &rtv, nullptr);

		D3D11_VIEWPORT vp{};
		vp.Width = static_cast<float>(x);
		vp.Height = static_cast<float>(y);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		device_context->RSSetViewports(1, &vp);

		return 0;
	}
}
