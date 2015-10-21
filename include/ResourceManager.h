#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// STD
#include <string>
#include <unordered_map>
#include <utility>

// DirectX
#include <d3d11.h>
#include "Mesh.h"
#include "SimpleShader.h"
#include "Material.h"

class ResourceManager
{
private:
    std::unordered_map<std::string, Mesh> meshes;
    std::unordered_map<std::string, ISimpleShader*> shaders;
    std::unordered_map<std::string, ID3D11ShaderResourceView*> textures;
	std::unordered_map<std::string, ID3D11SamplerState*> samplers;
	std::unordered_map<std::string, Material*> materials;

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;

    // Private to prevent instantiation/destruction outside of instance() method.
    ResourceManager();
    ~ResourceManager();

    // Deleted to prevent copying/moving
    ResourceManager( const ResourceManager& rhs ) = delete;
    ResourceManager( ResourceManager&& rhs ) = delete;
    ResourceManager& operator=( const ResourceManager& rhs ) = delete;
    ResourceManager& operator=( ResourceManager&& rhs ) = delete;
public:

    // Getters
    Mesh*                           GetMesh(const std::string& id );
    ISimpleShader*                  GetShader(const std::string& id );
    ID3D11ShaderResourceView*       GetTexture(const std::string& id );
	ID3D11SamplerState*             GetSamplerState(const std::string& id);
	Material*						GetMaterial(const std::string& id);

    /*
     * Access the ResourceManager Singleton
     */
    static ResourceManager* instance();

	ID3D11DeviceContext* GetDeviceContext() { return this->deviceContext; }

    /*
     * Register an ID3D11Device and ID3D11DeviceContext with the ResourceManager.
     * @param   device              The ID3D11Device to register.
     * @param   deviceContext       The ID3D11DeviceContext to register.
     */
    void RegisterDeviceAndContext( ID3D11Device* const device, ID3D11DeviceContext* const deviceContext );

    /*
     * Register a 3D Mesh with the ResourceManager
     * @param       id      The id to store the Mesh at.
     * @param       args    The arguments passed to the Mesh constructor.
     * @return  A bool indicating if the operation was successful.
     */
    template <typename... Args>
    bool RegisterMesh( const std::string& id, Args&&... args )
    {
        // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
        if( !device || !deviceContext )
        {
            return false;
        }

        // Bail is there is already a Mesh stored at that id
        if( meshes.find( id ) != meshes.cend() )
        {
            return false;
        }

        // Construct the Mesh
        meshes.emplace
        ( 
            std::piecewise_construct,
            std::forward_as_tuple( id ),
            std::forward_as_tuple( args..., device )
        );

        return true;
    }

    /*
     * Register an ISimpleShader* with the ResourceManager.
     * @param       id          The id to store the ISimpleShader* at.
     * @param       filename    The filename of the compiled shader object.
     * @return  A bool indicating if the operation was successful.
     */
    template <typename T>
    bool RegisterShader( const std::string& id, LPCWSTR filename )
    {
        static_assert( std::is_base_of<ISimpleShader, T>::value, "Must be an instance of ISimpleShader!" );

        // Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
        if( !device || !deviceContext )
        {
            return false;
        }

        // Bail if there is already an ISimpleShader stored at that id
        if( shaders.find( id ) != shaders.cend() )
        {
            return false;
        }

        // Create the ISimpleShader
        {
            T* shader = new T( device, deviceContext );
            shader->LoadShaderFile( filename );

            shaders.emplace( id, shader );
        }

        return true;
    }

    /*
     * Register an ID3D11ShaderResourceView* with the ResourceManager.
     * @param       id          The id to store the ID3D11ShaderResourceView* at.
     * @param       filename    The filename of the texture.
     * @return  A bool indicating if the operation was successful.
     */
    bool RegisterTexture(const std::string& id, LPCWSTR filename );

	/*
	* Register an ID3D11SamplerState* with the ResourceManager.
	* @param       id          The id to store the ID3D11SamplerState* at.
	* @param       filename    The description used to create the ID3D11SamplerState.
	* @return  A bool indicating if the operation was successful.
	*/
	bool RegisterSamplerState(const std::string& id, D3D11_SAMPLER_DESC pSamplerDesc);

	/*
	* Register a material with the ResourceManager
	* @param       id        The id to store the Material at.
	* @param       material  The material to add.
	* @return  A bool indicating if the operation was successful.
	*/
	bool RegisterMaterial(const std::string& id, Material* material)
	{
		// Bail if there is not a registered ID3D11Device or ID3D11DeviceContext
		if (!device || !deviceContext)
		{
			return false;
		}

		// Bail is there is already a Material stored at that id
		if (materials.find(id) != materials.cend())
		{
			return false;
		}

		// Add the Mesh
		materials.emplace(id, material);

		return true;
	}
};

#endif
