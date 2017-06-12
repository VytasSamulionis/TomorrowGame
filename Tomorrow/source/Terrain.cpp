#include "../include/Game.h"

void Game::UpdateTerrain () {
    m_Terrain->GetTerrain()->Update(
        m_Camera->GetPosition(), 
        m_Device->GetViewMatrix(),
        m_Device->GetProjectionMatrix());
}

void Game::RenderTerrain () {
    m_Terrain->GetTerrain()->Render();
}

bool Game::IsTerrainClicked (POINT& _Point) {
    POINT mouse;
    GetCursorPos (&mouse);
    mouse.y = m_WindowHeight - mouse.y;
    VECTOR3 origin, direction;
    cml::make_pick_ray ((float)mouse.x, (float)mouse.y, 
        m_Device->GetViewMatrix(), 
        m_Device->GetProjectionMatrix(), 
        m_Device->GetViewportMatrix ((float)m_WindowWidth, (float)m_WindowHeight), 
        origin, direction, false);

    float distance = 9999.0f;
    POINT point;
    for (UINT i = 0; i < m_Terrain->GetTerrain()->GetPatchesPerSide(); i++) {
        for (UINT j = 0; j < m_Terrain->GetTerrain()->GetPatchesPerSide(); j++) {
            std::vector<PatchBounds> bounds;
            if (m_Terrain->GetTerrain()->IsPatchPicked (origin, direction, j, i, bounds)) {
                float dist;
                for (UINT k = 0; k < bounds.size(); k++) {
                    if (m_Terrain->GetTerrain()->IsVertexPicked (origin, direction, bounds[k], m_Camera->GetPosition(), point, dist)) {
                        if (dist < distance) {
                            _Point = point;
                            distance = dist;
                        }
                    }
                }
            }
        }
    }
    if (distance < 9998.0f) {
        return true;
    }
    return false;
}