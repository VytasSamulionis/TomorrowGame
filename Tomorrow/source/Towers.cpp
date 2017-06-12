#include "../include/Game.h"

void Game::SetupTowersInfo () {
    vs3d::MATERIAL material;
    material.Ambient = vs3d::COLORVALUE (0.8f, 0.8f, 0.8f, 1.0f);
    material.Diffuse = vs3d::COLORVALUE (1.0f, 1.0f, 1.0f, 1.0f);
    material.Emissive = vs3d::COLORVALUE (0.0f, 0.0f, 0.0f, 0.0f);
    material.Specular = vs3d::COLORVALUE (0.0f, 0.0f, 0.0f, 0.0f);
    material.Power = 0.0f;
    UINT towerSkin = m_Device->GetSkinManager()->AddSkin ("data/obj/basic_tower/tower_texture.jpg", material);
    TowerInfo basicTower;
    basicTower.Id = m_ObjManager->Load ("data/obj/basic_tower/Tower1.obj", towerSkin);
    basicTower.Type = BASIC_TOWER;
    basicTower.Level = 0;
    basicTower.Price = 10;
    basicTower.AttackSpeed = 5.0f;
    basicTower.Power = 10;
    basicTower.Radius = 150.0f;
    basicTower.ShootDelay = 5.0f;
    basicTower.Gun = NULL;
    basicTower.Size = 2;
    basicTower.BuildingTime = 2.0f;
    basicTower.MaxBuildingTime = 2.0f;
    strcpy (basicTower.Description, BASIC_TOWER_DESCRIPTION);
    m_PreparedTowers.push_back (basicTower);

    TowerUpgradeInfo basicTowerUpgrade;
    basicTowerUpgrade.UpgradeTime[0] = 2.0f;
    basicTowerUpgrade.UpgradeTime[1] = 4.0f;
    basicTowerUpgrade.UpgradeTime[2] = 7.0f;
    basicTowerUpgrade.UpgradeTime[3] = 10.0f;
    basicTowerUpgrade.UpgradeTime[4] = 12.0f;

    basicTowerUpgrade.AttackSpeedDecrease[0] = 0.2f;
    basicTowerUpgrade.AttackSpeedDecrease[1] = 0.5f;
    basicTowerUpgrade.AttackSpeedDecrease[2] = 0.8f;
    basicTowerUpgrade.AttackSpeedDecrease[3] = 1.0f;
    basicTowerUpgrade.AttackSpeedDecrease[4] = 1.5f;

    basicTowerUpgrade.PowerIncrease[0] = 1;
    basicTowerUpgrade.PowerIncrease[1] = 3;
    basicTowerUpgrade.PowerIncrease[2] = 5;
    basicTowerUpgrade.PowerIncrease[3] = 7;
    basicTowerUpgrade.PowerIncrease[4] = 10;

    basicTowerUpgrade.RadiusIncrease[0] = 10.0f;
    basicTowerUpgrade.RadiusIncrease[1] = 20.0f;
    basicTowerUpgrade.RadiusIncrease[2] = 30.0f;
    basicTowerUpgrade.RadiusIncrease[3] = 40.0f;
    basicTowerUpgrade.RadiusIncrease[4] = 50.0f;

    basicTowerUpgrade.Price[0] = 10;
    basicTowerUpgrade.Price[1] = 20;
    basicTowerUpgrade.Price[2] = 40;
    basicTowerUpgrade.Price[3] = 80;
    basicTowerUpgrade.Price[4] = 160;
    m_UpgradeInfo.push_back (basicTowerUpgrade);

    towerSkin = m_Device->GetSkinManager()->AddSkin ("data/obj/slowing_tower/SlowingTowerTexture.jpg", material);
    TowerInfo slowingTower;
    slowingTower.Id = m_ObjManager->Load ("data/obj/slowing_tower/SlowingTower.obj", towerSkin);
    slowingTower.Type = SLOWING_TOWER;
    slowingTower.Level = 0;
    slowingTower.Price = 20;
    slowingTower.AttackSpeed = 10.0f;
    slowingTower.Power = 0;
    slowingTower.Radius = 200.0f;
    slowingTower.ShootDelay = 10.0f;
    slowingTower.Gun = NULL;
    slowingTower.Size = 2;
    slowingTower.BuildingTime = 4.0f;
    slowingTower.MaxBuildingTime = 4.0f;
    strcpy (slowingTower.Description, SLOWING_TOWER_DESCRIPTION);
    m_PreparedTowers.push_back (slowingTower);

    TowerUpgradeInfo slowingTowerUpgrade;
    slowingTowerUpgrade.UpgradeTime[0] = 4.0f;
    slowingTowerUpgrade.UpgradeTime[1] = 4.0f;
    slowingTowerUpgrade.UpgradeTime[2] = 4.0f;
    slowingTowerUpgrade.UpgradeTime[3] = 4.0f;
    slowingTowerUpgrade.UpgradeTime[4] = 4.0f;

    slowingTowerUpgrade.AttackSpeedDecrease[0] = 0.0f;
    slowingTowerUpgrade.AttackSpeedDecrease[1] = 0.0f;
    slowingTowerUpgrade.AttackSpeedDecrease[2] = 0.0f;
    slowingTowerUpgrade.AttackSpeedDecrease[3] = 0.0f;
    slowingTowerUpgrade.AttackSpeedDecrease[4] = 0.0f;

    slowingTowerUpgrade.PowerIncrease[0] = 0;
    slowingTowerUpgrade.PowerIncrease[1] = 0;
    slowingTowerUpgrade.PowerIncrease[2] = 0;
    slowingTowerUpgrade.PowerIncrease[3] = 0;
    slowingTowerUpgrade.PowerIncrease[4] = 0;

    slowingTowerUpgrade.RadiusIncrease[0] = 10.0f;
    slowingTowerUpgrade.RadiusIncrease[1] = 20.0f;
    slowingTowerUpgrade.RadiusIncrease[2] = 40.0f;
    slowingTowerUpgrade.RadiusIncrease[3] = 60.0f;
    slowingTowerUpgrade.RadiusIncrease[4] = 80.0f;

    slowingTowerUpgrade.Price[0] = 10;
    slowingTowerUpgrade.Price[1] = 15;
    slowingTowerUpgrade.Price[2] = 20;
    slowingTowerUpgrade.Price[3] = 30;
    slowingTowerUpgrade.Price[4] = 40;
    m_UpgradeInfo.push_back (slowingTowerUpgrade);

    towerSkin = m_Device->GetSkinManager()->AddSkin ("data/obj/area_tower/AreaTowerTexture.jpg", material);
    TowerInfo areaTower;
    areaTower.Id = m_ObjManager->Load ("data/obj/area_tower/AreaTower.obj", towerSkin);
    areaTower.Type = AREA_TOWER;
    areaTower.Level = 0;
    areaTower.Price = 50;
    areaTower.AttackSpeed = 5.0f;
    areaTower.Power = 10;
    areaTower.Radius = 100.0f;
    areaTower.ShootDelay = 5.0f;
    areaTower.Gun = NULL;
    areaTower.Size = 8;
    areaTower.BuildingTime = 5.0f;
    areaTower.MaxBuildingTime = 5.0f;
    strcpy (areaTower.Description, AREA_TOWER_DESCRIPTION);
    m_PreparedTowers.push_back (areaTower);

    TowerUpgradeInfo areaTowerUpgrade;
    areaTowerUpgrade.UpgradeTime[0] = 6.0f;
    areaTowerUpgrade.UpgradeTime[1] = 10.0f;
    areaTowerUpgrade.UpgradeTime[2] = 14.0f;
    areaTowerUpgrade.UpgradeTime[3] = 18.0f;
    areaTowerUpgrade.UpgradeTime[4] = 20.0f;

    areaTowerUpgrade.AttackSpeedDecrease[0] = 0.2f;
    areaTowerUpgrade.AttackSpeedDecrease[1] = 0.5f;
    areaTowerUpgrade.AttackSpeedDecrease[2] = 0.8f;
    areaTowerUpgrade.AttackSpeedDecrease[3] = 1.0f;
    areaTowerUpgrade.AttackSpeedDecrease[4] = 1.5f;

    areaTowerUpgrade.PowerIncrease[0] = 1;
    areaTowerUpgrade.PowerIncrease[1] = 3;
    areaTowerUpgrade.PowerIncrease[2] = 5;
    areaTowerUpgrade.PowerIncrease[3] = 7;
    areaTowerUpgrade.PowerIncrease[4] = 10;

    areaTowerUpgrade.RadiusIncrease[0] = 10.0f;
    areaTowerUpgrade.RadiusIncrease[1] = 15.0f;
    areaTowerUpgrade.RadiusIncrease[2] = 20.0f;
    areaTowerUpgrade.RadiusIncrease[3] = 25.0f;
    areaTowerUpgrade.RadiusIncrease[4] = 30.0f;

    areaTowerUpgrade.Price[0] = 50;
    areaTowerUpgrade.Price[1] = 80;
    areaTowerUpgrade.Price[2] = 140;
    areaTowerUpgrade.Price[3] = 200;
    areaTowerUpgrade.Price[4] = 300;
    m_UpgradeInfo.push_back (areaTowerUpgrade);

    /* Tower ghosts */
    m_TowerGhost.push_back (m_ObjManager->GetModel(m_PreparedTowers[BASIC_TOWER].Id)->MakeCopy());    /* basic tower */
    m_TowerGhost.push_back (m_ObjManager->GetModel(m_PreparedTowers[SLOWING_TOWER].Id)->MakeCopy());    /* slowing tower */
    m_TowerGhost.push_back (m_ObjManager->GetModel(m_PreparedTowers[AREA_TOWER].Id)->MakeCopy());    /* area tower */
}

bool Game::CreateTower (TowerType _type, POINT _point) {
    bool isThereSpace = true;
    int halfSize = m_PreparedTowers[_type].Size / 2;
    int terrainSize = m_Terrain->GetTerrain()->GetSize ();
    for (int i = -halfSize; i <= halfSize && isThereSpace; i++) {
        for (int j = -halfSize; j <= halfSize && isThereSpace; j++) {
            if (_point.x + i >= 0 && _point.y + j >= 0 &&
                _point.x + i < terrainSize && _point.y + j < terrainSize) {
                    if (m_Occupied[_point.x + i][_point.y + j]) {
                        isThereSpace = false;
                    }
            }
        }
    }
    if (isThereSpace) {
        float u = (float)_point.x / m_Terrain->GetTerrain()->GetSize();
        float v = (float)_point.y / m_Terrain->GetTerrain()->GetSize();
        BYTE texel[4];
        m_Device->GetSkinManager()->GetTextureTexel (m_BuildingFieldTextureId, u, v, texel);
        if (texel[1] + texel[2] + texel[3] > 0) {   /* if texel is not black */
            isThereSpace = false;
        }
    }
    if (isThereSpace) {
        UINT tower = m_ObjManager->GetModel(m_PreparedTowers[_type].Id)->MakeCopy();
        switch (_type) {
            case BASIC_TOWER:
                m_ObjManager->GetModel(tower)->Scale(10.0f);
                break;
            case SLOWING_TOWER:
                m_ObjManager->GetModel(tower)->Scale(20.0f);
                break;
            case AREA_TOWER:
                m_ObjManager->GetModel(tower)->Scale(20.0f);
                break;
        }
        float x = _point.x * m_Terrain->GetTerrain()->GetScale(0);
        float y = m_Terrain->GetTerrain()->GetScaledHeight(_point.x, _point.y);
        float z = _point.y * m_Terrain->GetTerrain()->GetScale(2);
        m_GameUI->UpdateTowerMark (tower, VECTOR3 (x, y, z));
        m_ObjManager->GetModel(tower)->TranslateX (x);
        m_ObjManager->GetModel(tower)->TranslateY (y);
        m_ObjManager->GetModel(tower)->TranslateZ (z);
        m_ObjManager->GetModel(tower)->Prepare ();
        TowerInfo info;
        info.Id = tower;
        info.Type = _type;
        info.Level = MAX_TOWER_LEVEL + 1;
        info.Location = _point;
        info.AttackSpeed = m_PreparedTowers[_type].AttackSpeed;
        info.Power = m_PreparedTowers[_type].Power;
        info.Radius = m_PreparedTowers[_type].Radius;
        info.ShootDelay = m_PreparedTowers[_type].ShootDelay;
        info.Gun = new Bullet (3.0f, 3.0f);
        info.Gun->Init (m_Device, NULL);
        switch (_type) {
            case BASIC_TOWER:
                info.Gun->SetColor (0xff888888);
                break;
            case SLOWING_TOWER:
                info.Gun->SetColor (0xff0000ff);
                break;
            case AREA_TOWER:
                info.Gun->SetColor (0xffaaaaaa);
                break;
        }
        info.Gun->SetMaxRange (info.Radius);
        info.Gun->SetMaxTime (0.5f);
        for (UINT i = 0; i < 1; i++) {
            info.Gun->AddParticle();
        }
        info.Size = m_PreparedTowers[_type].Size;
        info.BuildingTime = m_PreparedTowers[_type].BuildingTime;
        info.MaxBuildingTime = m_PreparedTowers[_type].MaxBuildingTime;
        m_Towers.push_back (info);
        int halfSize = info.Size / 2;
        for (int i = -halfSize; i <= halfSize; i++) {
            for (int j = -halfSize; j <= halfSize; j++) {
                int terrainSize = m_Terrain->GetTerrain()->GetSize ();
                if (_point.x + i >= 0 && _point.y + j >= 0 &&
                    _point.x + i < terrainSize && _point.y + j < terrainSize) {
                    m_Occupied[_point.x + i][_point.y + j] = true;
                }
            }
        }
        m_Audio->Play3D (m_AudioBankId, "Tower", VECTOR3 (x, y, z), VECTOR3 (0.0f, 0.0f, 1.0f), VECTOR3 (0.0f, 1.0f, 0.0f));
        return true;
    } else {
        m_GameUI->ShowMessage ("Tower cannot be placed here.", 0xffff0000, 3.0f);
    }
    return false;
}

void Game::RenderTowerRange (float _x, float _y, float _z, float _range) {
    vs3d::ULCVERTEX data[31];
    UINT numData = 0;
    WORD index[30 * 3];
    UINT numIndices = 0;
        
    for (UINT i = 0; i < 30; i++) {
        float angle = i * 2 * (float)M_PI / 30.0f;
        data[numData].Color = 0x330000ff;
        data[numData].X = _x + (cosf(angle) * _range);
        data[numData].Y = _y;
        data[numData].Z = _z + (sinf(angle) * _range);
        if (i > 0) {
            index[numIndices++] = numData - 1;
            index[numIndices++] = 30;   // center vertex
            index[numIndices++] = numData;
        }
        numData++;
    }
    data[numData].Color = 0x330000ff;
    data[numData].X = _x;
    data[numData].Y = _y;
    data[numData].Z = _z;
    numData++;
    index[numIndices++] = index[numIndices-1];
    index[numIndices++] = 30;
    index[numIndices++] = index[0];
    m_Device->SetAlphaBlendState (AS_SRCBLEND, BLEND_SRCALPHA);
    m_Device->SetAlphaBlendState (AS_DESTBLEND, BLEND_INVSRCALPHA);
    m_Device->EnableAlphaBlend ();
    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, data, numData, index, numIndices, VFT_ULC, INVALID_ID);
    m_Device->DisableAlphaBlend ();
}

void Game::RenderTowerGhost (TowerType _type) {
    POINT mouse;
    GetCursorPos (&mouse);
    if (!m_GameUI->IsCursorOnHood(mouse)) {
        POINT location;
        if (IsTerrainClicked (location)) {
            m_ObjManager->GetModel(m_TowerGhost[_type])->ClearTransformations();
            switch (_type) {
                case BASIC_TOWER:
                    m_ObjManager->GetModel(m_TowerGhost[_type])->Scale(10.0f);
                    break;
                case SLOWING_TOWER:
                case AREA_TOWER:
                    m_ObjManager->GetModel(m_TowerGhost[_type])->Scale(20.0f);
                    break;
            }
            float x = location.x * m_Terrain->GetTerrain()->GetScale(0);
            float y = m_Terrain->GetTerrain()->GetScaledHeight(location.x, location.y);
            float z = location.y * m_Terrain->GetTerrain()->GetScale(2);
            m_ObjManager->GetModel(m_TowerGhost[_type])->TranslateX(x);
            m_ObjManager->GetModel(m_TowerGhost[_type])->TranslateY(y);
            m_ObjManager->GetModel(m_TowerGhost[_type])->TranslateZ(z);            
            /*m_Device->SetTextureStageState (0, TSS_ALPHAARG1, TA_DIFFUSE);
            m_Device->SetTextureStageState (0, TSS_ALPHAOP, TOP_SELECTARG1);
            m_Device->SetAlphaBlendState (AS_SRCBLEND, BLEND_SRCALPHA);
            m_Device->SetAlphaBlendState (AS_DESTBLEND, BLEND_INVSRCALPHA);*/
            //m_Device->EnableAlphaBlend ();
            m_ObjManager->GetModel(m_TowerGhost[_type])->RenderDynamic();
            
            RenderTowerRange(x, y + 0.01f, z, m_PreparedTowers[_type].Radius);
            //m_Device->DisableAlphaBlend ();
        }
    }
}

void Game::UpdateTowers (float _delta) {
    for (UINT i = 0; i < m_Towers.size(); i++) {
        if (m_Towers[i].BuildingTime <= 0.0f) {
            UpdateTowerShooting (i, _delta);
            bool wasShot = false;
            std::list<TowerGunInfo>::iterator gun = m_Towers[i].GunInfo.begin();
            while (gun != m_Towers[i].GunInfo.end()) {
                if (gun->Target->IsDead) {
                    gun = m_Towers[i].GunInfo.erase (gun);
                } else {
                    if (!gun->IsShootingUpdated) {
                        gun->ShootingTime -= _delta * m_SpeedUpFactor;
                    }
                    if (!gun->IsShootingUpdated && gun->ShootingTime <= 0.0f) {
                        if (gun->Target->HitPoints > 0) {
                            m_Audio->Play3D (
                                m_AudioBankId, 
                                "Grunt", 
                                gun->Target->Position, 
                                m_Waypoints[gun->Target->ActiveWaypoint].Direction, 
                                VECTOR3 (0.0f, 1.0f, 0.0f));
                        }
                        gun->Target->HitPoints -= m_Towers[i].Power;
                        gun->IsShootingUpdated = true;
                    }
                    bool wasErased = false;
                    if (gun->IsTargetAcquired && gun->Target->HitPoints <= 0) {
                        gun->Target->HitPoints = 0;
                        if (m_Towers[i].Type == SLOWING_TOWER) {
                            gun->Target->NumSlowedDown--;
                        } else {
                            gun->Target->NumAttackers--;
                        }
                        if (gun->Target->NumAttackers == 0 && gun->Target->NumSlowedDown == 0) {
                            m_Resource.NumResources += gun->Target->NumResources;
                            m_Score += gun->Target->NumResources;
                            m_GameUI->UpdateNumResources (m_Resource.NumResources);
                            m_Audio->Stop (gun->Target->SoundId);
                            gun->Target->IsDead = true;
                            gun->Target->AnimationSpeed = 1.0f;
                            gun->Target->CurrentAnimation = "Die";
                            gun->Target->LoopAnimation = false;
                            gun->Target->SelfDestructionTime = 4.0f;
                            gun = m_Towers[i].GunInfo.erase (gun);
                            wasErased = true;
                        }
                        if (!wasErased) {
                            gun->IsTargetAcquired = false;
                            gun->IsShootingUpdated = true;
                        }
                    }
                    if (!wasErased) {
                        gun++;
                    }
                }
            }
        }
    }
}

void Game::RenderBuildingTime (UINT _towerId) {
    float timeLeft = m_Towers[_towerId].BuildingTime / m_Towers[_towerId].MaxBuildingTime;

    MATRIX44 view = m_Device->GetViewMatrix();
    VECTOR3 right (view(0, 0), view(1, 0), view(2, 0));
    right = right.normalize();
    VECTOR3 up (view(0, 1), view(1, 1), view(2, 1));
    up = up.normalize();
    VECTOR3 forward (view(0, 2), view(1, 2), view(2, 2));
    forward = forward.normalize();

    vs3d::ULCVERTEX timer[31];
    UINT numVertices = 0;
    WORD index[30 * 3];
    UINT numIndices = 0;

    float min[3], max[3];
    m_ObjManager->GetModel(m_Towers[_towerId].Id)->GetBounds (min, max);
    float x = m_Towers[_towerId].Location.x * m_Terrain->GetTerrain()->GetScale (0);
    float y = max[1];
    float z = m_Towers[_towerId].Location.y * m_Terrain->GetTerrain()->GetScale (2);

    /* center */
    VECTOR3 position (x, y, z);
    VECTOR3 location = position + right + up + forward;
    timer[numVertices].Color = 0xff0000ff;
    timer[numVertices].X = x;
    timer[numVertices].Y = y;
    timer[numVertices].Z = z;
    numVertices++;
    for (UINT i = 0; i < (UINT)(30 * timeLeft); i++) {
        float angle = i * 2 * (float)M_PI / 30.0f;
        location = position + (cosf(angle) * 10.0f) * right + (sinf(angle) * 10.0f) * up + forward;
        timer[numVertices].Color = 0xff0000ff;
        timer[numVertices].X = location[0];
        timer[numVertices].Y = location[1];
        timer[numVertices].Z = location[2];
        if (i > 0) {
            index[numIndices++] = numVertices - 1;
            index[numIndices++] = 0;   // center vertex
            index[numIndices++] = numVertices;
        }
        numVertices++;
    }
    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, timer, numVertices, index, numIndices, VFT_ULC, INVALID_ID);
}

void Game::RenderTowers (bool _isRenderingShadowMap, bool _isRenderingInactive) {
    for (UINT i = 0; i < m_Towers.size(); i++) {
        bool shouldRender = true;
        bool isInactive = false;
        if (!_isRenderingShadowMap) {
            shouldRender = m_Terrain->GetTerrain()->IsPointVisible(m_Towers[i].Location.x, m_Towers[i].Location.y);
            isInactive = m_Towers[i].BuildingTime > 0.0f;
            if (_isRenderingInactive) {
                if (!isInactive) {
                    shouldRender = false;
                }
            } else {
                if (isInactive) {
                    shouldRender = false;
                }
            }
        }
        if (shouldRender) {
            m_ObjManager->GetModel(m_Towers[i].Id)->Render ();
        }
    }
}

void Game::MakeTowerUpgrade (UINT _towerId) {
    UINT lvl = m_Towers[_towerId].Level;
    UINT type = m_Towers[_towerId].Type;
    m_Towers[_towerId].Radius += m_UpgradeInfo[type].RadiusIncrease[lvl];
    m_Towers[_towerId].AttackSpeed -= m_UpgradeInfo[type].AttackSpeedDecrease[lvl];
    m_Towers[_towerId].ShootDelay = m_Towers[_towerId].AttackSpeed;
    m_Towers[_towerId].Power += m_UpgradeInfo[type].PowerIncrease[lvl];
    m_Towers[_towerId].Level++;
    m_GameUI->ShowMessage ("Upgraded successfully", 0xff00ff00, 3.0f);
}

float Game::GetDistanceTime (UINT _towerId, const VECTOR3& _origin, const VECTOR3& _destination) {
    float x = _origin[0] - _destination[0];
    float y = _origin[1] - _destination[1];
    float z = _origin[2] - _destination[2];
    float length = sqrtf (x * x + y * y + z * z);
    return length / m_Towers[_towerId].Radius * m_Towers[_towerId].Gun->GetMaxTime ();
}

bool Game::IsEnemyInTowerRange (float _TowerX, float _TowerY, UINT _TowerId, std::list<EnemyInfo>::iterator _Enemy, VECTOR3& _NextPosition) {
    if (_Enemy->IsDead) {
        return false;
    }
    float height = m_Terrain->GetTerrain()->GetScaledHeight(m_Towers[_TowerId].Location.x, m_Towers[_TowerId].Location.y);
    VECTOR3 origin (_TowerX, height + 5.0f, _TowerY);
    _NextPosition = _Enemy->Position + m_Waypoints[_Enemy->ActiveWaypoint].Direction * _Enemy->Speed * _Enemy->SlowDownFactor * m_SpeedUpFactor * GetDistanceTime(_TowerId, origin, _Enemy->Position);
    float enemyX = _NextPosition[0];
    float enemyY = _NextPosition[2];
    float x = _TowerX - enemyX;
    float y = _TowerY - enemyY;
    return (sqrt(x * x + y * y) <= m_Towers[_TowerId].Radius);
}

void Game::TowerShoot (float _TowerX, float _TowerY, UINT _TowerId, std::list<EnemyInfo>::iterator _Enemy, const VECTOR3& _NextPosition) {
    float height = m_Terrain->GetTerrain()->GetScaledHeight(m_Towers[_TowerId].Location.x, m_Towers[_TowerId].Location.y);
    VECTOR3 origin (_TowerX, height + 55.0f, _TowerY);
    VECTOR3 destination = _NextPosition;
    destination[1] += 35.0f;
    m_Towers[_TowerId].Gun->SetPosition (origin, destination);
    for (UINT i = 0; i < 1; i++) {
        m_Towers[_TowerId].Gun->AddParticle();
    }
    TowerGunInfo gun;
    gun.IsTargetAcquired = true;
    gun.ShootingTime = m_Towers[_TowerId].Gun->GetLifeTime();
    gun.IsShootingUpdated = false;
    gun.Target = _Enemy;
    m_Towers[_TowerId].GunInfo.push_back (gun);
    m_Towers[_TowerId].SoundId = m_Audio->Play3D (m_AudioBankId, "Shot", origin, VECTOR3 (0.0f, 0.0f, 1.0f), VECTOR3 (0.0f, 1.0f, 0.0f));
}

void Game::UpdateBasicTowerShooting (float _towerX, float _towerY, UINT _towerId, float _delta) {
    VECTOR3 nextPosition;
    std::list<TowerGunInfo>::iterator towerGun;
    towerGun = m_Towers[_towerId].GunInfo.begin();
    if (towerGun != m_Towers[_towerId].GunInfo.end () &&
        towerGun->IsTargetAcquired &&
        IsEnemyInTowerRange (_towerX, _towerY, _towerId, towerGun->Target, nextPosition)) {
            TowerShoot (_towerX, _towerY, _towerId, towerGun->Target, nextPosition);
            m_Towers[_towerId].GunInfo.pop_front (); /* Remove the old one because a new one was added. */
            m_Towers[_towerId].ShootDelay -= _delta * m_SpeedUpFactor;
    } else {
        if (towerGun != m_Towers[_towerId].GunInfo.end () && towerGun->IsTargetAcquired) {
            towerGun->Target->NumAttackers--;
            m_Towers[_towerId].GunInfo.erase (towerGun);
        }
        std::list<EnemyInfo>::iterator i;
        for (i = m_Enemies.begin(); i != m_Enemies.end(); i++) {
            if (IsEnemyInTowerRange (_towerX, _towerY, _towerId, i, nextPosition)) {
                TowerShoot (_towerX, _towerY, _towerId, i, nextPosition);
                i->NumAttackers++;
                m_Towers[_towerId].ShootDelay -= _delta * m_SpeedUpFactor;
                break;
            }
        }
    }
}

void Game::UpdateSlowingTowerShooting (float _towerX, float _towerY, UINT _towerId, float _delta) {
    VECTOR3 nextPosition;
    std::list<EnemyInfo>::iterator i;
    std::list<TowerGunInfo>::iterator towerGun;
    towerGun = m_Towers[_towerId].GunInfo.begin();
    while (towerGun != m_Towers[_towerId].GunInfo.end()) {
        towerGun->Target->NumSlowedDown--;
        if (towerGun->Target->NumSlowedDown == 0) {
            towerGun->Target->SlowDownFactor = 1.0f;
            towerGun->Target->AnimationSpeed = 1.0f;
        }
        towerGun = m_Towers[_towerId].GunInfo.erase (towerGun);
    }
    for (i = m_Enemies.begin(); i != m_Enemies.end(); i++) {
        if (IsEnemyInTowerRange (_towerX, _towerY, _towerId, i, nextPosition)) {
            TowerShoot (_towerX, _towerY, _towerId, i, nextPosition);
            i->NumSlowedDown++;
            i->SlowDownFactor = 0.5f;
            i->AnimationSpeed = 0.5f;
        }
    }
    m_Towers[_towerId].ShootDelay -= _delta * m_SpeedUpFactor;
}

void Game::UpdateAreaTowerShooting (float _towerX, float _towerY, UINT _towerId, float _delta) {
    VECTOR3 nextPosition;
    std::list<EnemyInfo>::iterator i;
    std::list<TowerGunInfo>::iterator towerGun;
    towerGun = m_Towers[_towerId].GunInfo.begin();
    while (towerGun != m_Towers[_towerId].GunInfo.end()) {
        towerGun->Target->NumAttackers--;
        towerGun = m_Towers[_towerId].GunInfo.erase (towerGun);
    }
    for (i = m_Enemies.begin(); i != m_Enemies.end(); i++) {
        if (IsEnemyInTowerRange (_towerX, _towerY, _towerId, i, nextPosition)) {
            TowerShoot (_towerX, _towerY, _towerId, i, nextPosition);
            i->NumAttackers++;
        }
    }
    m_Towers[_towerId].ShootDelay -= _delta * m_SpeedUpFactor;
}

void Game::UpdateTowerShooting (UINT _towerId, float _Delta) {
    float towerX = m_Towers[_towerId].Location.x * m_Terrain->GetTerrain()->GetScale(0);
    float towerY = m_Towers[_towerId].Location.y * m_Terrain->GetTerrain()->GetScale(2);
    if (m_Towers[_towerId].ShootDelay < m_Towers[_towerId].AttackSpeed) {
        m_Towers[_towerId].ShootDelay -= _Delta * m_SpeedUpFactor;
        if (m_Towers[_towerId].ShootDelay < 0.0f) {
            m_Towers[_towerId].ShootDelay = m_Towers[_towerId].AttackSpeed;
        }
    }

    if (fabs (m_Towers[_towerId].ShootDelay - m_Towers[_towerId].AttackSpeed) < 0.0001f) {
        switch (m_Towers[_towerId].Type) {
            case BASIC_TOWER:
                UpdateBasicTowerShooting (towerX, towerY, _towerId, _Delta);
                break;
            case SLOWING_TOWER:
                UpdateSlowingTowerShooting (towerX, towerY, _towerId, _Delta);
                break;
            case AREA_TOWER:
                UpdateAreaTowerShooting (towerX, towerY, _towerId, _Delta);
                break;
        }
    }
}

bool Game::IsObjectVisible (float _frustum[6][4], UINT _id) {
    float min[3], max[3];
    m_ObjManager->GetModel(_id)->GetBounds(min, max);
    for (UINT i = 0; i < 6; i++) {
        if (_frustum[i][0] * min[0] + _frustum[i][1] * min[1] + _frustum[i][2] * min[2] + _frustum[i][3] > 0)
            continue;
        if (_frustum[i][0] * max[0] + _frustum[i][1] * min[1] + _frustum[i][2] * min[2] + _frustum[i][3] > 0)
            continue;
        if (_frustum[i][0] * min[0] + _frustum[i][1] * max[1] + _frustum[i][2] * min[2] + _frustum[i][3] > 0)
            continue;
        if (_frustum[i][0] * max[0] + _frustum[i][1] * max[1] + _frustum[i][2] * min[2] + _frustum[i][3] > 0)
            continue;
        if (_frustum[i][0] * min[0] + _frustum[i][1] * min[1] + _frustum[i][2] * max[2] + _frustum[i][3] > 0)
            continue;
        if (_frustum[i][0] * max[0] + _frustum[i][1] * min[1] + _frustum[i][2] * max[2] + _frustum[i][3] > 0)
            continue;
        if (_frustum[i][0] * min[0] + _frustum[i][1] * max[1] + _frustum[i][2] * max[2] + _frustum[i][3] > 0)
            continue;
        if (_frustum[i][0] * max[0] + _frustum[i][1] * max[1] + _frustum[i][2] * max[2] + _frustum[i][3] > 0)
            continue;
        return false;
    }
    return true;
}

bool Game::IsSelectingTower (UINT& _towerId) {
    POINT mouse;
    GetCursorPos (&mouse);
    mouse.y = m_WindowHeight - mouse.y;
    VECTOR3 origin, direction;
    cml::make_pick_ray ((float)mouse.x, (float)mouse.y, 
        m_Device->GetViewMatrix(), 
        m_Device->GetProjectionMatrix(), 
        m_Device->GetViewportMatrix ((float)m_WindowWidth, (float)m_WindowHeight), 
        origin, direction, false);

    float minDistance = 9999.0f;
    _towerId = INVALID_ID;
    for (UINT i = 0; i < m_Towers.size(); i++) {
        float minBounds[3];
        float maxBounds[3];
        m_ObjManager->GetModel(m_Towers[i].Id)->GetBounds (minBounds, maxBounds);
        float distance;
        if (IsRayIntersectsObb (origin, direction, 
                                VECTOR3(minBounds[0], minBounds[1], minBounds[2]), 
                                VECTOR3(maxBounds[0], maxBounds[1], maxBounds[2]),
                                distance)) {
            if (distance < minDistance) {
                _towerId = i;
                minDistance = distance;
            }
        }
    }
    if (_towerId == INVALID_ID) {
        return false;
    }
    return true;
}


void Game::ShowUpgradeInfo (UINT _towerId) {
    char text[MAX_PATH];
    char header[15];
    UINT lvl = m_Towers[_towerId].Level;
    UINT type = m_Towers[_towerId].Type;
    bool isBuilding = false;
    if (lvl == MAX_TOWER_LEVEL + 1) {   /* Tower is being built */
        lvl = 0;
        isBuilding = true;
    }
    sprintf (header, "Tower Upgrade");
    if (m_Towers[_towerId].BuildingTime <= 0.0f) {
        if (lvl < MAX_TOWER_LEVEL) {
            sprintf (text, "LEVEL %u\n\nPrice: %u\nAttack Speed: %.2f\nPower: %u\nRadius: %.2f", 
                lvl + 1,
                m_UpgradeInfo[type].Price[lvl],
                m_Towers[_towerId].AttackSpeed - m_UpgradeInfo[type].AttackSpeedDecrease[lvl],
                m_UpgradeInfo[type].PowerIncrease[lvl] + m_Towers[_towerId].Power,
                m_UpgradeInfo[type].RadiusIncrease[lvl] + m_Towers[_towerId].Radius);
        } else {
            sprintf (text, "LEVEL %u\n\nYou have reached the\nmaximum level.", lvl + 1);
        }
    } else {
        if (isBuilding) {
            sprintf (header, "Building Tower");
            sprintf (text, "\nTower construction in\nprogress...");
        } else {
            sprintf (text, "\nUpgrading to level %u.", lvl + 2);
        }
    }
    m_GameUI->EnableDescription (
        header,
        text);
}

