#include "ResourceManager.h"

// DirectX
#include "WICTextureLoader.h"
#include "DXMacros.h"

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager()
{
    // Meshes are stack-allocated and automatically cleaned up.

    // Cleanup Shaders
    for( auto& pair : shaders )
    {
        delete pair.second;
    }

    // Release Textures
	for ( auto& pair : textures) 
    {
        ReleaseMacro( pair.second );
	}

    // Release SamplerStates
    for( auto& pair : samplers )
    {
        ReleaseMacro( pair.second );
    }
}

/*
 * Register an ID3D11Device and ID3D11DeviceContext with the ResourceManager.
 * @param   device              The ID3D11Device to register.
 * @param   deviceContext       The ID3D11DeviceContext to register.
 */
void ResourceManager::RegisterDeviceAndContext( ID3D11Device* const device, ID3D11DeviceContext* const deviceContext )
{
    this->device = device;
    this->deviceContext = deviceContext;
}

/*
 * Access the ResourceManager Singleton
 */
ResourceManager* ResourceManager::instance() 
{
    static ResourceManager manager;
    return &manager;
}

#pragma region Getters 

Mesh* ResourceManager::GetMesh( const std::string& id ) 
{
    auto& iter = meshes.find( id );
    if( iter == meshes.cend() )
    {
        return nullptr;
    }

    return &iter->second;
}

ISimpleShader* ResourceManager::GetShader( const std::string& id ) 
{
    auto& iter = shaders.find( id );
    if( iter == shaders.cend() )
    {
        return nullptr;
    }

    return iter->second;
}

ID3D11ShaderResourceView* ResourceManager::GetTexture( const std::string& id ) 
{
    auto& iter = textures.find( id );
    if( iter == textures.cend() )
    {
        return nullptr;
    }

    return iter->second;
}

ID3D11SamplerState* ResourceManager::GetSamplerState( const std::string& id )
{
    auto& iter = samplers.find( id );
    if( iter == samplers.cend() )
    {
        return nullptr;
    }

    return iter->second;
}

#pragma endregion

/*
 * Register an ID3D11ShaderResourceView* with the ResourceManager.
 * @param       id          The id to store the ID3D11ShaderResourceView* at.
 * @param       filename    The filename of the texture.
 * @return  A bool indicating if the operation was successful.
 */
bool ResourceManager::RegisterTexture( std::string&& id, LPCWSTR filename )
{
    // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
    if( !device || !deviceContext )
    {
        return false;
    }

    // Bail if there is already an ID3D11ShaderResourceView* stored at that id
    if( textures.find( id ) != textures.cend() )
    {
        return false;
    }

    // Create the ID3D11ShaderResourceView*
    {
        ID3D11ShaderResourceView* pTexture;
        DirectX::CreateWICTextureFromFile(device, deviceContext, filename, 0, &pTexture);
        textures.emplace( std::forward<std::string>( id ), pTexture );
    }

    return true;
}


/*
 * Register an ID3D11SamplerState* with the ResourceManager.
 * @param       id          The id to store the ID3D11SamplerState* at.
 * @param       filename    The description used to create the ID3D11SamplerState.
 * @return  A bool indicating if the operation was successful.
 */
bool ResourceManager::RegisterSamplerState( std::string&& id, D3D11_SAMPLER_DESC pSamplerDesc )
{
    // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
    if( !device || !deviceContext )
    {
        return false;
    }

    // Bail if there is already an ID3D11SamplerState* stored at that id
    if( samplers.find( id ) != samplers.cend() )
    {
        return false;
    }

    // Create the ID3D11SamplerState*
    {
        ID3D11SamplerState* pSamplerState;
        device->CreateSamplerState( &pSamplerDesc, &pSamplerState );
        samplers.emplace( std::forward<std::string>( id ), pSamplerState );
    }

    return true;
}
