#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// STD
#include <string>
#include <map>

// DirectX
#include <d3d11.h>

#include "Mesh.h"
#include "SimpleShader.h"

class ResourceManager
{
private:
    std::map<std::string, Mesh*> meshes;
    std::map<std::string, ISimpleShader*> shaders;
    std::map<std::string, ID3D11ShaderResourceView*> textures;

    // Private to prevent instantiation/destruction outside of instance() method.
    ResourceManager();
    ~ResourceManager();

    // Deleted to prevent copying/moving
    ResourceManager( const ResourceManager& rhs ) = delete;
    ResourceManager( ResourceManager&& rhs ) = delete;
    ResourceManager& operator=( const ResourceManager& rhs ) = delete;
    ResourceManager& operator=( ResourceManager&& rhs ) = delete;
public:
    static ResourceManager* instance();

    void LoadResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

    Mesh*                           GetMesh( const std::string& id );
    ISimpleShader*                  GetShader( const std::string& id );
    ID3D11ShaderResourceView*       GetTexture( const std::string& id );

    void AddMesh( std::string&& id, Mesh* toAdd);
    void AddShader( std::string&& id, ISimpleShader* toAdd);
    void AddTexture( std::string&& id, ID3D11ShaderResourceView* toAdd);
};

#endif
