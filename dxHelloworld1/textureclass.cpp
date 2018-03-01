////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"
#include <vector>
#include "lodepng.h"
#include <strstream>
#include <string>
//#include "WICTextureLoader.h"
#include <d3d11.h>

TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, WCHAR* filename)
{
	HRESULT result;


	// Load the texture in.
	//result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	//if(FAILED(result))
	//{
	//	return false;
	//}
	//
	//return true;

	//HRESULT result;

	//UINT pixels[64] = {
	//	0xff00ff00, 0xff0000ff,
	//	0xffff0000, 0xffffffff,
	//};
	//UINT pixels[32*32] = {
	//	0xffff0000, 0xff00ff00,
	//	0xff0000ff,
	//};

	std::vector<unsigned char> imageRGBA;
	unsigned nImageWidth, nImageHeight;
	std::wstring wsFileName(filename);
	std::string sFileName(wsFileName.begin(), wsFileName.end());
	unsigned nError = lodepng::decode(imageRGBA, nImageWidth, nImageHeight, sFileName);

	if (nError)
	{
		return false;
	}

	/*std::ostrstream strout;
	strout << nImageWidth << ", " << nImageHeight << ", arr_size = " << imageRGBA.size();
	std::string str = strout.str();
	std::wstring debug_info(str.begin(), str.end());
	MessageBox(NULL, debug_info.c_str(), L"", 0);*/

	//for (int i = 0; i < ARRAYSIZE(pixels); i++)
	//	pixels[i] = pixels[i%3];

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = &imageRGBA[0];
	//subresourceData.SysMemPitch = 8;
	subresourceData.SysMemPitch = nImageWidth * 4;
	subresourceData.SysMemSlicePitch = nImageWidth*nImageHeight*4;

	D3D11_TEXTURE2D_DESC texture2dDesc;
	texture2dDesc.Width = nImageWidth;
	texture2dDesc.Height = nImageHeight;
	texture2dDesc.MipLevels = 1;
	texture2dDesc.ArraySize = 1;
	texture2dDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2dDesc.SampleDesc.Count = 1;
	texture2dDesc.SampleDesc.Quality = 0;
	texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	texture2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture2dDesc.CPUAccessFlags = 0;
	texture2dDesc.MiscFlags = 0;

	ID3D11Texture2D *texture;
	result = device->CreateTexture2D(&texture2dDesc, &subresourceData, &texture);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	memset(&shaderResourceViewDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	result = device->CreateShaderResourceView(texture, &shaderResourceViewDesc, &m_texture);

	
	// result = DirectX::CreateWICTextureFromFile(device, context, wsFileName.c_str(), nullptr, &m_texture);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}


void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}