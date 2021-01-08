#include "Graphics.h"
#include "dxerr.h"
#include <sstream>

namespace wrl = Microsoft::WRL;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException( __LINE__,__FILE__,hr )

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//用于消除锯齿的采样模式[LeranOpenGL网站的采样模式说的很好
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	//我们希望将此缓冲区用作渲染目标RenderTarget
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	//sd.OutputWindow = (HWND)696969;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;


	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// for checking results of d3d functions
	HRESULT hr;

	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));


	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	//GFX_THROW_INFO
	GFX_THROW_INFO(pSwap->GetBuffer(0,__uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(),nullptr,&pTarget));
}

//Graphics::~Graphics()
//{
//	if (pContext != nullptr)
//	{
//		pContext->Release();
//	}
//	if (pSwap != nullptr)
//	{
//		pSwap->Release();
//	}
//	if (pDevice != nullptr)
//	{
//		pDevice->Release();
//	}
//
//	if (pTarget != nullptr)
//	{
//		pTarget->Release();
//	}
// 
//}

void Graphics::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	} 
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = {red,green,blue,1.0f};
	pContext->ClearRenderTargetView(pTarget.Get(),color);
}

void Graphics::DrawTestTriangle()
{

	HRESULT hr;

	struct Vertex
	{
		struct
		{
			float x;
			float y;
		}pos;
	
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		}color;

	};

	// 一个三角形和两个三角形 D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	//const Vertex vertices[] =
	//{
	//	{0.0f,0.5f},
	//	{0.5f,-0.5f},
	//	{-0.5f,-0.5f},

	//	//{0.5f,1.0f}, 
	//	//{1.0f,0.5f},
	//	//{0.5f,0.5f},
	//};

	//一条线 D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	//const Vertex vertices[] =
	//{
	//	{0.0f,0.5f},
	//	{0.5f,-0.5f},
	//	{-0.5f,-0.5f},
	//
	//};

	//3条线连成三角形 D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	//const Vertex vertices[] =
	//{
	//	{0.0f,0.5f},
	//	{0.5f,-0.5f},
	//	{0.5f,-0.5f},
	//	{-0.5f,-0.5f},
	//	{-0.5f,-0.5f},
	//	{0.0f,0.5f},
	//};



	//3条线连成三角形 D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
	//const Vertex vertices[] =
	//{
	//	{0.0f,0.5f},
	//	{0.5f,-0.5f}, 
	//	{-0.5f,-0.5f}, 
	//	{0.0f,0.5f},
	//};

	//const Vertex vertices[] = 
	//{
	//	{0.0f,0.5f,1.0f,0.0f,0.0f},
	//	{0.5f,-0.5f,0.0f,1.0f,0.0f},
	//	{-0.5f,-0.5f,0.0f,0.0f,1.0f}, 
	//};


	Vertex vertices[] =
	{
		{0.0f,0.5f,255,0,0,0},
		{0.5f,-0.5f,0,255,0,0},
		{-0.5f,-0.5f,0,0,255,0},
		{-0.3f,0.3f,0,255,0,0},
		{0.3f,0.3f,0,0,255,0},
		{0.0f,-0.8f,255,0,0,0},
	};

	vertices[0].color.g = 255;

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer)); 


	//Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


	//create index buffer
	const unsigned short indices[] =
	{
		0,1,2,
		0,2,3,
		0,4,1,
		2,1,5,
	};

	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&ibd,&isd,&pIndexBuffer));

	pContext->IASetIndexBuffer(pIndexBuffer.Get(),DXGI_FORMAT_R16_UINT,0u);


	wrl::ComPtr<ID3DBlob> pBlob; 

	//create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader; 
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	//GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr, pVertexShader.GetAddressOf()));
	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	//bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(),nullptr,0u);



	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		//{"Color",0,DXGI_FORMAT_R32G32B32_FLOAT,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0},		//8u 当我们在读取颜色数据前，我们要先经过8个byte大小的位置数据
		{"Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0},		//8u 当我们在读取颜色数据前，我们要先经过8个byte大小的位置数据
	};

	//bind vertex layout
	GFX_THROW_INFO(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	//bind vertex layout 至关重要的一行代码
	pContext->IASetInputLayout(pInputLayout.Get());
	  
	
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	//bind pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	//bind render target
	//pContext->OMSetRenderTargets(1u, &pTarget, nullptr);
	pContext->OMSetRenderTargets(1u,pTarget.GetAddressOf(),nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u,&vp); 

	//GFX_THROW_INFO_ONLY
	//GFX_THROW_INFO_ONLY(pContext->Draw(3u, 0u));
	//pContext->Draw(3u, 0u);
	//GFX_THROW_INFO_ONLY(pContext->Draw((UINT)std::size(vertices), 0u));

	GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}


Graphics::HrException::HrException(int line, const char * file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	: Exception(line, file), hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	} 
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Chili Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}


const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
Graphics::InfoException::InfoException(int line, const char * file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}


const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Chili Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}