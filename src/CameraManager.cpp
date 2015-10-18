#include "CameraManager.h"
#include "DebugCamera.h"

CameraManager::CameraManager()
{
    // Always create a DebugCamera.
    m_pDebugCamera = new DebugCamera( 0.0f, 0.0f, -5.0f );
    m_pActiveCamera = m_pDebugCamera;
}

CameraManager::~CameraManager()
{
    // Cleanup all registered Cameras.
    delete m_pDebugCamera;
    for( auto& pair : m_cameras )
    {
        delete pair.second;
    }
}

/*
 * Access the CameraManager Singleton.
 */
CameraManager* CameraManager::instance( void )
{
    static CameraManager manager;
    return &manager;
}

/*
 * Returns an instance of the in-use Camera.
 */
Camera* const CameraManager::GetActiveCamera( void ) const
{
    return m_pActiveCamera;
}

/*
 * Attempt to retrieve the Camera associated with the provided id.
 * @param   id      The id of the Camera to locate.
 */
Camera* const CameraManager::GetCameraByID( const std::string& id ) const
{
    auto& iter = m_cameras.find( id );
    if( iter == m_cameras.cend() )
    {
        return nullptr;
    }
    else
    {
        return iter->second;
    }
}

/*
 * Set the in-use Camera.
 * @param   id      The id associated with that Camera
 * @return  Boolean indicating if this operation was successful.
 */
bool CameraManager::SetActiveCamera( const std::string& id )
{
    auto& iter = m_cameras.find( id );
    if( iter == m_cameras.cend() )
    {
        return false;
    }

    m_pActiveCamera = iter->second;

    return true;
}

/*
 * Sets the DebugCamera as the in-use Camera.
 */
void CameraManager::ActivateDebugCamera( void )
{
    m_pActiveCamera = m_pDebugCamera;
}

/*
 * Update the CameraManager.
 * @param   deltaTime   The delta time value for this frame.
 */
void CameraManager::Update( float deltaTime )
{
    m_pActiveCamera->Update( deltaTime );
}

