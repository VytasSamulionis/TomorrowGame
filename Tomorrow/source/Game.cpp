#include "../include/Game.h"

Game::Game (HWND _hMain, HINSTANCE _instance) {
    m_Input = new InputSystem;
    m_WindowWidth = 1024;
    m_WindowHeight = 768;
    m_RendererLoader = new RendererLoader (_instance);
    m_Log = NULL;
    m_RendererLoader->CreateDevice ();
    m_Device = m_RendererLoader->GetDevice();
    //m_Device->InitFullScreen (_hMain, m_WindowWidth, m_WindowHeight);
    m_Device->InitWindowed (_hMain, m_WindowWidth, m_WindowHeight);
    m_TerrainLoader = new TerrainEngineLoader (_instance);
    m_TerrainLoader->CreateTerrainEngine(m_Device);
    m_Terrain = m_TerrainLoader->GetTerrainEngine();

    m_Camera = new FromAboveCamera;
    m_Camera->SetPosition (VECTOR3(600.0f, 1050.0f, 150.0f));
    //m_Camera->SetForward (VECTOR3(0.0f, 0.0f, -1.0f));
    m_Camera->SetViewAngle (3.14f / 3);
    m_Camera->SetZoomSpeed (0.0f);

    m_ObjManager = new ObjManager (m_Device);

    m_Ms3dLoader = new Ms3dLoader ();
    //UINT alienId = m_Ms3dLoader->LoadModel ("data/ms3d/Alien.ms3d");
    //m_Ms3dLoader->GetModel(alienId)->Scale (10.0f, 10.0f, 10.0f);
    m_GameUI = new GameUI(m_Device, m_WindowWidth, m_WindowHeight);
    m_GameUI->InitMainHood ("data/terrain_texture/TerrainTexture.jpg", "data/hood/Hood.png", 1280.0f, 1280.0f);
    m_IsPaused = true;
    m_IsShowingControls = false;
    m_IsLevelLoaded = false;
    m_GameUI->EnableMainHood ();
    m_GameUI->SetMaxCastleHitPoints (2000);
    m_GameUI->SetCastleHitPoints (2000);

    m_AudioLoader = new AudioEngineLoader (_instance);
    m_AudioLoader->CreateAudioEngine ();
    m_Audio = m_AudioLoader->GetAudioEngine ();
    m_Audio->Initialize ("data/sounds/Win/Sounds.xgs");
    m_AudioBankId = m_Audio->LoadInMemory ("data/sounds/Win/Sound Bank.xsb", "data/sounds/Win/Wave Bank.xwb");
    m_Audio->SetListenerPosition (m_Camera->GetPosition ());
    m_Audio->SetListenerFront (m_Camera->GetLookingPoint ());
    m_Audio->SetListenerTop (m_Camera->GetUpVector ());
    m_ForestSoundId = INVALID_ID;

    m_SpeedUpFactor = 1.0f;

    m_BuildingFieldTextureId = m_Device->GetSkinManager()->AddTexture ("data/terrain_texture/BuildingField.jpg");

    SetupScene ();

    m_Device->GetVCacheManager()->SetTextStyle("Arial");
    m_Device->GetVCacheManager()->SetTextSize(14);

    vs3d::LIGHT light;
    ZeroMemory (&light, sizeof (vs3d::LIGHT));
    light.Type = LIGHT_DIRECTIONAL;
    light.Diffuse = vs3d::COLORVALUE (1.0f, 1.0f, 1.0f, 1.0f);
    VECTOR3 lightDirection (0.6f, -0.3f, 0.6f);
    lightDirection.normalize();
    light.Direction = lightDirection;
    m_Device->SetLight (0, light);
    m_Device->EnableLight (0, true);
    m_Device->SetAmbientLight (0x00676767);

    FILE* save = fopen ("Save.txt", "r");
    m_IsSaved = true;
    if (!save) {
        m_IsSaved = false;
    } else {
        UINT hitPoints = 0;
        fscanf (save, "%u", &hitPoints);
        if (hitPoints == 0) {
            m_IsSaved = false;
        }
        fclose (save);
    }
}

Game::~Game () {
    UnloadLevel ();
    for (UINT i = 0; i < m_Towers.size(); i++) {
        delete m_Towers[i].Gun;
    }
    delete m_GameUI;
    delete m_Ms3dLoader;
    delete m_RendererLoader;
    delete m_TerrainLoader;
    delete m_AudioLoader;
    delete m_ObjManager;
    delete m_Log;
    delete m_Camera;
    delete m_Input;
}

void Game::StartNew () {
    m_GameUI->RenderLoadingScreen ();
    UnloadLevel ();
    SetupTowersInfo ();
    LoadLevel("c_level.terrain");
    m_Timer.StartCounter ();
    VECTOR3 position (600.0f, 600.0f, 250.0f);
    VECTOR3 front (0.0f, 0.0f, 1.0f);
    VECTOR3 top (0.0f, 1.0f, 0.0f);
    m_ForestSoundId = m_Audio->Play3D (m_AudioBankId, "Forest", position, front, top);
    AddEnemyWaves ();
}
            
                
void Game::Update () {
    m_Timer.EndCounter ();
    float delta = (float)m_Timer.GetTimeDelta();
    if (m_SpeedUpFactor > 1.0001f && m_Enemies.size() == 0) {
        delta = m_NextWaveTimeLeft / m_SpeedUpFactor;
    }

    char framesPerSecond[20];
    double fps = (double) m_Timer.GetFramesPerSecond();
    sprintf (framesPerSecond, "%.2f fps", fps);

    m_Timer.StartCounter ();
    m_Camera->Update (delta);
    m_Audio->SetListenerPosition (m_Camera->GetPosition ());
    m_Audio->SetListenerFront (m_Camera->GetLookingPoint ());
    m_Audio->SetListenerTop (m_Camera->GetUpVector ());
    m_Audio->Update3DSoundPosition (m_ForestSoundId, VECTOR3 (600.0f, 200.0f, 250.0f));
    UpdateEnemyMovement (delta);
    m_Camera->SetZoomSpeed (0.0f);
    m_Device->LookAt (
        m_Camera->GetPosition(), 
        m_Camera->GetLookingPoint() + m_Camera->GetPosition(),
        m_Camera->GetUpVector());
    UpdateEnemies (delta);

    RenderShadowMap (delta);

    m_Device->BeginRendering (true, false, true);
    if (m_ShouldRenderTowerGhost) {
        m_Device->GetVCacheManager()->EnableEffect (m_TerrainEffect, "ShadowedSceneWithBuildingField");
    } else {
        m_Device->GetVCacheManager()->EnableEffect (m_TerrainEffect, "ShadowedScene");
    }
    //m_Device->SetTextureStageState (1, TSS_COLOROP, TOP_MODULATE);
    UpdateTerrain ();
    RenderTerrain();
        
    //m_Device->GetVCacheManager()->DisableEffects ();
    //RenderTerrainWater ();
    //m_Device->SetTextureStageState (1, TSS_COLOROP, TOP_DISABLE);
    //m_Device->GetVCacheManager()->Flush();
    //m_Device->EnableLighting (true);
    m_Device->GetVCacheManager()->EnableEffect (m_ObjectEffect, "ShadowedScene");
    //float frustum[6][4];
    extract_frustum_planes (m_Device->GetViewMatrix(), m_Device->GetProjectionMatrix(), m_Frustum, cml::z_clip_zero);
    for (UINT i = 0; i < m_Objects.size(); i++) {
        if (IsObjectVisible (m_Frustum, m_Objects[i])) {
            m_ObjManager->GetModel(m_Objects[i])->Render();
        }
        //m_Device->GetVCacheManager()->Flush();
    }
    /*m_Device->Clear (false, false, true);
    m_Ms3dLoader->GetModel(alienId)->Translate(0.0f, 0.0f, 0.1f);*/
    RenderEnemies (delta);
    //m_Device->GetVCacheManager()->Flush();
    UpdateTowers (delta);
    RenderTowers (false, false);    /* render active towers */
    m_Device->GetVCacheManager()->EnableEffect (m_ObjectEffect, "UnlitScene");
    RenderTowers (false, true);     /* render inactive towers */
    if (m_ShouldRenderTowerGhost) {
        RenderTowerGhost (m_BuildingTowerType);
    }
    m_Device->GetVCacheManager()->DisableEffects ();
    //m_Device->EnableLighting (false);
    for (UINT i = 0; i < m_Towers.size(); i++) {
        if (m_Towers[i].BuildingTime > 0.0f) {
            RenderBuildingTime (i);
            m_Towers[i].BuildingTime -= delta * m_SpeedUpFactor;
            if (m_Towers[i].BuildingTime <= 0.0f) {
                if (m_Towers[i].Level < MAX_TOWER_LEVEL) {
                    MakeTowerUpgrade (i);
                } else {
                    m_Towers[i].Level = 0;
                }
                if (m_SelectedTowerId == i) {
                    ShowUpgradeInfo (i);
                    if (m_Towers[i].Level < MAX_TOWER_LEVEL && m_Towers[i].BuildingTime <= 0.0f) {
                        m_GameUI->EnableUpgradeButton ();
                    }
                }
            }
        }
        if (!m_Towers[i].Gun->IsEmpty()) {
            m_Towers[i].Gun->Update (delta * m_SpeedUpFactor);
            m_Towers[i].Gun->Render ();
        }
    }
    if (m_SelectedTowerId != INVALID_ID) {
        float x = m_Towers[m_SelectedTowerId].Location.x * m_Terrain->GetTerrain()->GetScale(0);
        float y = m_Terrain->GetTerrain()->GetScaledHeight(m_Towers[m_SelectedTowerId].Location.x, m_Towers[m_SelectedTowerId].Location.y) + 0.01f;
        float z = m_Towers[m_SelectedTowerId].Location.y * m_Terrain->GetTerrain()->GetScale(2);
        RenderTowerRange (x, y, z, m_Towers[m_SelectedTowerId].Radius);
    }
    std::list<EnemyInfo>::iterator i;
    for (i = m_Enemies.begin(); i != m_Enemies.end(); i++) {
        RenderEnemyHealthBar (i);
    }
    UpdateEnemyWaves (delta);
    //m_Device->Clear (false, false, true);
    //if (m_Towers.size() > 1) {
        /*  for (UINT i = 0; i < m_Towers.size(); i++) {
            RenderTowerRange (i);
        }*/
    //}
                        
    m_Resource.TimeRemaining -= delta * m_SpeedUpFactor;
    if (m_Resource.TimeRemaining < 0.0f) {
        m_Resource.TimeRemaining += m_Resource.Time;
        m_Resource.NumResources += m_Resource.Stride;
        m_Score += m_Resource.Stride;
        m_GameUI->UpdateNumResources (m_Resource.NumResources);
    }
    if (m_IsMoving) {
        RenderCameraMovement ();
    }
    m_GameUI->UpdateNextWaveTime (m_NextWaveTimeLeft);
    m_GameUI->Render (m_Camera->GetPosition(), delta);
    m_Device->GetVCacheManager()->RenderText (framesPerSecond, 0xffffffff, 50, 50);
    m_Device->EndRendering ();
    m_Audio->Update ();
    m_Input->Reset ();
}

void Game::RenderMainScreen () {
    m_Device->BeginRendering (true, false, true);
    if (m_IsShowingControls) {
        m_GameUI->RenderControlsScreen ();
    } else {
        m_GameUI->RenderMainScreen (m_IsSaved);
    }
    m_Device->EndRendering ();
}

void Game::RenderShadowMap (float _delta) {
    m_Device->BeginRenderingToShadowMap ();
    m_Device->Clear (true, false, true);
    m_Terrain->GetTerrain()->Update (m_ShadowMapLightEye, m_ShadowMapLightView, m_ShadowMapLightProj);
    RenderTerrain();
    m_Device->SetCullingState (RS_CULL_CW);
    RenderTowers (true, false);
    for (UINT i = 0; i < m_Objects.size(); i++) {
        m_ObjManager->GetModel(m_Objects[i])->Render();
    }
    std::list<EnemyInfo>::iterator i;
    for (i = m_Enemies.begin(); i != m_Enemies.end(); i++) {
        m_Ms3dLoader->GetModel(i->Id)->Render(m_Device);
    }
    m_Device->EndRenderingToShadowMap ();
    m_Device->SetCullingState (RS_CULL_CCW);

    m_Device->SetShadowMap (m_TerrainEffect, "g_ShadowMap");
    m_Device->SetShadowMap (m_ObjectEffect, "g_ShadowMap");
    m_Device->GetVCacheManager()->SetEffectParameter (m_TerrainEffect, "g_World", m_Device->GetWorldMatrix().data());
    m_Device->GetVCacheManager()->SetEffectParameter (m_ObjectEffect, "g_World", m_Device->GetWorldMatrix().data());
    m_Device->GetVCacheManager()->SetEffectParameter (m_TerrainEffect, "g_LightsWorldViewProjection", m_ShadowMapLightViewProj.data());
    m_Device->GetVCacheManager()->SetEffectParameter (m_ObjectEffect, "g_LightsWorldViewProjection", m_ShadowMapLightViewProj.data());
    MATRIX44 wvp = m_Device->GetWorldMatrix() * m_Device->GetViewMatrix() * m_Device->GetProjectionMatrix();
    m_Device->GetVCacheManager()->SetEffectParameter (m_TerrainEffect, "g_WorldViewProjection", wvp.data());
    m_Device->GetVCacheManager()->SetEffectParameter (m_ObjectEffect, "g_WorldViewProjection", wvp.data());
}

void Game::SetupScene () {
    m_Device->EnableLighting (false);
    m_Device->SetClearColor (0.2f, 0.4f, 0.8f);
    m_Device->SetProjectionView ((float)m_WindowHeight / m_WindowWidth, 1.0f, 3000.0f);

    m_Device->SetTextureStageState (0, TSS_COLORARG0, TA_DIFFUSE);
    m_Device->SetTextureStageState (0, TSS_COLORARG1, TA_TEXTURE);
    m_Device->SetTextureStageState (0, TSS_COLOROP, TOP_MODULATE);

    m_Device->SetTextureStageState (1, TSS_COLORARG1, TA_TEXTURE);
    m_Device->SetTextureStageState (1, TSS_COLORARG2, TA_CURRENT);
    m_Device->SetTextureStageState (1, TSS_COLOROP, TOP_MODULATE);

    SetupShaders ();
}

void Game::SetupShaders () {
    m_TerrainEffect = m_Device->GetVCacheManager()->CreateEffect ("data/effect/terrain.fx", 0, true);
    m_ObjectEffect = m_Device->GetVCacheManager()->CreateEffect ("data/effect/shadowed.fx", 0, true);
    m_Device->GetVCacheManager()->SetEffectTextureParamName (m_TerrainEffect, 0, "g_Texture");
    m_Device->GetVCacheManager()->SetEffectTextureParamName (m_TerrainEffect, 1, "g_DetailMap");
    m_Device->GetVCacheManager()->SetEffectTexture (m_TerrainEffect, "g_BuildingField", m_BuildingFieldTextureId);
    m_Device->GetVCacheManager()->SetEffectTextureParamName (m_ObjectEffect, 0, "g_Texture");
    float ambient = 0.7f;
    m_Device->GetVCacheManager()->SetEffectParameter (m_TerrainEffect, "g_Ambient", &ambient);
    m_Device->GetVCacheManager()->SetEffectParameter (m_ObjectEffect, "g_Ambient", &ambient);
    m_ShadowMapFarClip = 1800.0f;
    m_Device->GetVCacheManager()->SetEffectParameter (m_TerrainEffect, "g_FarClip", &m_ShadowMapFarClip);
    m_Device->GetVCacheManager()->SetEffectParameter (m_ObjectEffect, "g_FarClip", &m_ShadowMapFarClip);

    /* Setup shadow map. */
    cml::matrix_perspective_xfov_LH (
        m_ShadowMapLightProj, 
        3.1416f / 2.0f, 
        m_WindowWidth / (float)m_WindowHeight, 
        1.0f, 
        m_ShadowMapFarClip, 
        cml::z_clip_zero);
    m_ShadowMapLightEye = VECTOR3 (-100.0f, 800.0f, -100.0f);
    m_ShadowMapLightTarget =  VECTOR3 (600.0f, 150.0f, 550.0f);
    m_ShadowMapLightDir = m_ShadowMapLightTarget - m_ShadowMapLightEye;
    m_ShadowMapLightDir = cml::normalize (m_ShadowMapLightDir);
    m_Device->GetVCacheManager()->SetEffectParameter (m_ObjectEffect, "g_LightPos", m_ShadowMapLightEye.data());
    m_Device->GetVCacheManager()->SetEffectParameter (m_ObjectEffect, "g_LightDir", m_ShadowMapLightDir.data());
    cml::matrix_look_at_LH (m_ShadowMapLightView, m_ShadowMapLightEye, m_ShadowMapLightDir + m_ShadowMapLightEye, VECTOR3 (0.0f, 1.0f, 0.0f));
    m_ShadowMapLightViewProj = m_ShadowMapLightView * m_ShadowMapLightProj;
    m_Device->CreateShadowMap (1024, m_ShadowMapLightViewProj, m_ShadowMapFarClip);
}

void Game::UnloadLevel () {
    m_Score = 0;
    m_IsMoving = false;
    m_IsLevelLoaded = false;
    m_KeepInfoMessage = false;
    m_ShouldRenderTowerGhost = false;
    m_Ms3dLoader->UnloadModels ();
    std::list<EnemyInfo>::iterator enemy;
    for (enemy = m_Enemies.begin(); enemy != m_Enemies.end(); enemy++) {
        if (!enemy->IsDead) {
            m_Audio->Stop (enemy->SoundId);
        }
    }
    m_Enemies.clear ();
    m_EnemyWaves.clear ();
    m_ObjManager->UnloadAll ();
    m_Objects.clear ();
    m_Waypoints.clear ();
    m_SelectedTowerId = INVALID_ID;
    m_Towers.clear ();
    m_PreparedTowers.clear ();
    m_TowerGhost.clear ();
    m_UpgradeInfo.clear ();
    m_Occupied.clear ();
    m_GameUI->RemoveAllMarks ();
    if (m_ForestSoundId != INVALID_ID) {
        m_Audio->Stop (m_ForestSoundId);
        m_ForestSoundId = INVALID_ID;
    }
}

void Game::LoadLevel (const char* _levelFile) {
    FILE* file = fopen (_levelFile, "r");
    if (!file) {
        THROW_ERROR (ERRC_FILE_NOT_FOUND);
    }

    //m_Device->GetSkinManager()->RemoveAll();

    char lightmapFile[MAX_PATH];
    float scaleX, scaleY, scaleZ;  
    fscanf (file, "%f %f %f", &scaleX, &scaleY, &scaleZ);
    m_Terrain->GetTerrain()->SetScale (scaleX, scaleY, scaleZ);
    UINT lightmode;
    fscanf (file, "%u", &lightmode);
    UCHAR lightMode = (UCHAR)lightmode;
    UINT minBrightness, maxBrightness;
    int slopeLightingDirX, slopeLightingDirZ;
    float slopeLightingSoftness;
    switch (lightMode) {
        case 1:
            fgets (lightmapFile, MAX_PATH, file);   // reading empty line
            fgets (lightmapFile, MAX_PATH, file);
            lightmapFile[strlen(lightmapFile) - 1] = '\0';  // remove the '\n' character
            break;
        case 2:
            fscanf (file, "%d %d %u %u %f", &slopeLightingDirX, &slopeLightingDirZ, 
                &minBrightness, &maxBrightness, &slopeLightingSoftness);
            break;
    }
    UINT lightR, lightG, lightB;
    fscanf (file, "%u %u %u", &lightR, &lightG, &lightB);
    UCHAR lightColor[3];
    lightColor[0] = (UCHAR)lightR;
    lightColor[1] = (UCHAR)lightG;
    lightColor[2] = (UCHAR)lightB;
    m_Terrain->GetTerrain()->SetLightColor (lightColor[0], lightColor[1], lightColor[2]);

    UINT isTerrainReady;
    fscanf (file, "%u", &isTerrainReady);
    UINT terrainSkinId;
    VERTEXFORMATTYPE vft;
    if (isTerrainReady) {
        UINT numTextures;
        fscanf (file, "%u", &numTextures);
        char texture[MAX_PATH];
        UINT* textureId = new UINT[numTextures];
        fgets (texture, MAX_PATH, file);    // reading empty line
        for (UINT i = 0; i < numTextures; i++) {
            //fscanf (file, "%s", texture);
            fgets (texture, MAX_PATH, file);
            texture[strlen(texture) - 1] = '\0';    // remove \n character
            textureId[i] = m_Device->GetSkinManager()->AddTexture(texture);
        }
        terrainSkinId = m_Device->GetSkinManager()->AddSkin(textureId, numTextures);
        delete[] textureId;
        fscanf (file, "%u", &vft);
    } else {
        terrainSkinId = INVALID_ID;
        vft = VFT_UL2;
    }

    UINT isWaterReady;
    fscanf (file, "%u", &isWaterReady);
    if (isWaterReady) {
        char waterTexture[MAX_PATH];
        fgets (waterTexture, MAX_PATH, file);   // reading empty line
        fgets (waterTexture, MAX_PATH, file);
        waterTexture[strlen(waterTexture) - 1] = '\0';  // remove \n character
        UINT waterSkinId = m_Device->GetSkinManager()->AddSkin(waterTexture);
        m_Terrain->GetTerrainWater()->SetSkinId(waterSkinId);
        UINT waterR, waterG, waterB, waterA;
        fscanf (file, "%u %u %u %u", &waterR, &waterG, &waterB, &waterA);
        //m_WaterR = (UCHAR)waterR;
        //m_WaterG = (UCHAR)waterG;
        //m_WaterB = (UCHAR)waterB;
        //m_WaterA = (UCHAR)waterA;
        m_Terrain->GetTerrainWater()->SetColor (waterR, waterG, waterB, waterA);
        float waterHeight;
        fscanf (file, "%f", &waterHeight);
        m_Terrain->GetTerrainWater()->SetWaterHeight(waterHeight);
    }
    //m_IsWaterPrepared = true;
    //m_IsWaterRendered = true;
    //CheckMenuItem (GetMenu (hMain), IDM_WATER_TURNON, MF_CHECKED);
    //CheckMenuItem (GetMenu (hMain), IDM_WATER_TURNOFF, MF_UNCHECKED);
    UINT isSkyboxReady;
    fscanf (file, "%d", &isSkyboxReady);
    if (isSkyboxReady) {
        char skyboxTexture[MAX_PATH];
        //fscanf (file, "%s", skyboxTexture);
        fgets (skyboxTexture, MAX_PATH, file);  // read empty line
        fgets (skyboxTexture, MAX_PATH, file);
        skyboxTexture[strlen(skyboxTexture) - 1] = '\0';    // remove the \n character
        UINT skyBoxTop = m_Device->GetSkinManager()->AddSkin (skyboxTexture);
        fgets (skyboxTexture, MAX_PATH, file);
        skyboxTexture[strlen(skyboxTexture) - 1] = '\0';    // remove the \n character
        UINT skyBoxBottom = m_Device->GetSkinManager()->AddSkin (skyboxTexture);
        fgets (skyboxTexture, MAX_PATH, file);
        skyboxTexture[strlen(skyboxTexture) - 1] = '\0';    // remove the \n character
        UINT skyBoxLeft = m_Device->GetSkinManager()->AddSkin (skyboxTexture);
        fgets (skyboxTexture, MAX_PATH, file);
        skyboxTexture[strlen(skyboxTexture) - 1] = '\0';    // remove the \n character
        UINT skyBoxRight = m_Device->GetSkinManager()->AddSkin (skyboxTexture);
        fgets (skyboxTexture, MAX_PATH, file);
        skyboxTexture[strlen(skyboxTexture) - 1] = '\0';    // remove the \n character
        UINT skyBoxFar = m_Device->GetSkinManager()->AddSkin (skyboxTexture);
        fgets (skyboxTexture, MAX_PATH, file);
        skyboxTexture[strlen(skyboxTexture) - 1] = '\0';    // remove the \n character
        UINT skyBoxNear = m_Device->GetSkinManager()->AddSkin (skyboxTexture);
        float skyBoxX, skyBoxY, skyBoxZ, skyBoxSize;
        fscanf (file, "%f %f %f %f", &skyBoxX, &skyBoxY, &skyBoxZ, &skyBoxSize);
        m_Terrain->GetSkyBox()->Init (skyBoxX, skyBoxY, skyBoxZ, skyBoxSize,
                                      skyBoxTop, skyBoxBottom, skyBoxLeft, 
                                      skyBoxRight, skyBoxFar, skyBoxNear);
    }

    UINT numWaypoints;
    fscanf (file, "%u", &numWaypoints);
    fscanf (file, "%d", &m_FinalWaypointIndex);
    for (UINT i = 0; i < numWaypoints; i++) {
        WaypointInfo waypoint;
        fscanf (file, "%u %u", &waypoint.Position.x, &waypoint.Position.y);
        m_Waypoints.push_back (waypoint);
    }

    for (UINT i = 0; i < numWaypoints - 1; i++) {
        float x = m_Waypoints[i].Position.x * scaleX - m_Waypoints[i+1].Position.x * scaleX;
        float y = m_Waypoints[i].Position.y * scaleZ - m_Waypoints[i+1].Position.y * scaleZ;
        m_Waypoints[i].Length = sqrt (x * x + y * y);
        VECTOR3 start (m_Waypoints[i].Position.x * scaleX, 0.0f, m_Waypoints[i].Position.y * scaleZ);
        VECTOR3 end (m_Waypoints[i+1].Position.x * scaleX, 0.0f, m_Waypoints[i+1].Position.y * scaleZ);
        m_Waypoints[i].Direction = (end - start).normalize();
    }
    m_Waypoints[numWaypoints - 1].Length = 0.0f;
    m_Waypoints[numWaypoints - 1].Direction = VECTOR3 (1.0f, 0.0f, 0.0f);

    UINT numObjects;
    fscanf (file, "%u", &numObjects);
    char objFile[MAX_PATH];
    char objTexture[MAX_PATH];
    for (UINT i = 0; i < numObjects; i++) {
        fgets (objFile, MAX_PATH, file);    // read empty line
        fgets (objFile, MAX_PATH, file);
        objFile[strlen(objFile) - 1] = '\0';    // remove the \n character
        fgets (objTexture, MAX_PATH, file);
        objTexture[strlen(objTexture) - 1] = '\0';    // remove the \n character
        vs3d::MATERIAL material;
        material.Ambient = vs3d::COLORVALUE (0.8f, 0.8f, 0.8f, 1.0f);
        material.Diffuse = vs3d::COLORVALUE (1.0f, 1.0f, 1.0f, 1.0f);
        material.Emissive = vs3d::COLORVALUE (0.0f, 0.0f, 0.0f, 0.0f);
        material.Specular = vs3d::COLORVALUE (0.0f, 0.0f, 0.0f, 0.0f);
        material.Power = 0.0f;
        UINT objId = m_ObjManager->Load (objFile, m_Device->GetSkinManager()->AddSkin(objTexture, material));
        VECTOR3 position;
        fscanf (file, "%f %f %f", &(position[0]), &(position[1]), &(position[2]));
        VECTOR3 rotation;
        fscanf (file, "%f %f %f", &(rotation[0]), &(rotation[1]), &(rotation[2]));
        VECTOR3 scale;
        fscanf (file, "%f %f %f", &(scale[0]), &(scale[1]), &(scale[2]));
        m_ObjManager->GetModel (objId)->ScaleX (scale[0]);
        m_ObjManager->GetModel (objId)->ScaleY (scale[1]);
        m_ObjManager->GetModel (objId)->ScaleZ (scale[2]);
        m_ObjManager->GetModel (objId)->Rotate (rotation[0], rotation[1], rotation[2]);
        m_ObjManager->GetModel (objId)->Translate (position[0], position[1], position[2]);
        m_ObjManager->GetModel (objId)->Prepare ();
        m_Objects.push_back (objId);
    }

    UINT size;
    fscanf (file, "%u", &size);
    m_Terrain->GetTerrain()->NewHeightmap (size);
    UINT height;
    for (UINT i = 0; i < size; i++) {
        for (UINT j = 0; j < size; j++) {
            fscanf (file, "%u", &height);
            m_Terrain->GetTerrain()->SetHeight (j, i, (UCHAR)height);
        }
        /*fscanf (file, "%u", &height);
        m_Terrain->SetHeight (size - 1, i, height);*/
    }
    switch (lightMode) {
        case 0:
            m_Terrain->GetTerrain()->SetHeightBasedLighting();
            break;
        case 1:
            m_Terrain->GetTerrain()->SetLightmapLighting(lightmapFile);
            break;
        case 2:
            m_Terrain->GetTerrain()->SetSlopeLighting (slopeLightingDirX, slopeLightingDirZ,
                        minBrightness, maxBrightness,
                        slopeLightingSoftness);
            break;
    }
    
    m_Terrain->GetTerrain()->Init (17, vft, terrainSkinId);

    m_Terrain->GetTerrain()->EnableBruteForce(true);

    fclose (file);

    m_IsLevelLoaded = true;

    for (UINT i = 0; i < size; i++) {
        std::vector<bool> occupied;
        m_Occupied.push_back(occupied);
        for (UINT j = 0; j < size; j++) {
            m_Occupied[i].push_back (false);
        }
    }

    m_Resource.NumResources = 0;
    m_Resource.Stride = 1;
    m_Resource.Time = 5.0f;
    m_Resource.TimeRemaining = 5.0f;
    m_GameUI->UpdateNumResources (m_Resource.NumResources);
}

void Game::Save () {
    FILE* save = fopen ("Save.txt", "w");
    fprintf (save, "%u\n", m_GameUI->GetCastleHitPoints());
    fprintf (save, "%f %f %f %f %f %f\n", 
        m_Camera->GetPosition()[0], m_Camera->GetPosition()[1], m_Camera->GetPosition()[2],
        m_Camera->GetLookingPoint()[0], m_Camera->GetLookingPoint()[1], m_Camera->GetLookingPoint()[2]);
    fprintf (save, "%u\n", m_EnemyWaves.size());
    for (UINT i = 0; i < m_EnemyWaves.size(); i++) {
        fprintf (save, "%f %u %f\n", m_EnemyWaves[i].Delay, m_EnemyWaves[i].NumEnemies, m_EnemyWaves[i].SpawnTimeRemaining);
    }
    fprintf (save, "%u\n", m_Enemies.size());
    std::list<EnemyInfo>::iterator enemy;
    for (enemy = m_Enemies.begin(); enemy != m_Enemies.end(); enemy++) {
        fprintf (save, "%u %u %d %f %f %f %f %f %f %f %f %f %s %f %u %u %f\n",
            enemy->WaveId, enemy->ActiveWaypoint, enemy->HitPoints,
            enemy->SlowDownFactor, enemy->Ammo, 
            enemy->Position[0], enemy->Position[1], enemy->Position[2],
            enemy->Direction[0], enemy->Direction[1], enemy->Direction[2],
            enemy->Distance, enemy->CurrentAnimation.c_str(), 
            enemy->AnimationSpeed, enemy->LoopAnimation, enemy->IsDead,
            enemy->SelfDestructionTime);
    }
    fprintf (save, "%u\n", m_Towers.size());
    for (UINT i = 0; i < m_Towers.size(); i++) {
        fprintf (save, "%u %u %u %u %f %f\n", 
            m_Towers[i].Type, 
            m_Towers[i].Level, 
            m_Towers[i].Location.x, 
            m_Towers[i].Location.y,
            m_Towers[i].BuildingTime,
            m_Towers[i].MaxBuildingTime);
    }
    fprintf (save, "%f %f %u %u\n", m_Resource.Time, m_Resource.TimeRemaining,
        m_Resource.Stride, m_Resource.NumResources);
    fprintf (save, "%u\n", m_Score);
    fclose (save);
}

void Game::Load () {
    FILE* load = fopen ("Save.txt", "r");
    UINT numHP;
    fscanf (load, "%u", &numHP);
    m_GameUI->SetCastleHitPoints (numHP);
    VECTOR3 position, lookingPoint, direction;
    fscanf (load, "%f %f %f %f %f %f", 
        &(position[0]), &(position[1]), &(position[2]),
        &(lookingPoint[0]), &(lookingPoint[1]), &(lookingPoint[2]));
    m_Camera->SetPosition (position);
    m_Camera->SetLookingPoint (lookingPoint);
    m_EnemyWaves.clear ();
    UINT numWaves;
    fscanf (load, "%u", &numWaves);
    while (numWaves > m_EnemyWaves.size()) {
        AddEnemyWaves ();
    }
    for (UINT i = 0; i < numWaves; i++) {
        fscanf (load, "%f %u %f", 
            &(m_EnemyWaves[i].Delay), 
            &(m_EnemyWaves[i].NumEnemies), 
            &(m_EnemyWaves[i].SpawnTimeRemaining));
    }
    m_Enemies.clear ();
    UINT numEnemies;
    fscanf (load, "%u", &numEnemies);
    std::list<EnemyInfo>::iterator enemy;
    UINT waveIndex, activeWaypoint;
    float slowDownFactor, ammo, distance, animSpeed, selfDestructionTime;
    UINT loopAnim, isDead;
    char currentAnim[MAX_PATH];
    int hitPoints;
    for (UINT i = 0; i < numEnemies; i++) {
        fscanf (load, "%u %u %d %f %f %f %f %f %f %f %f %f %s %f %u %u %f",
            &waveIndex, &activeWaypoint, &hitPoints,
            &slowDownFactor, &ammo, 
            &(position[0]), &(position[1]), &(position[2]),
            &(direction[0]), &(direction[1]), &(direction[2]),
            &distance, currentAnim, &animSpeed, 
            &loopAnim, &isDead, &selfDestructionTime);
        NewEnemy (waveIndex, animSpeed);
        EnemyInfo& enemy = m_Enemies.back();
        enemy.ActiveWaypoint = activeWaypoint;
        enemy.Ammo = ammo;
        enemy.Direction[0] = direction[0];
        enemy.Direction[1] = direction[1];
        enemy.Direction[2] = direction[2];
        enemy.Distance = distance;
        enemy.HitPoints = hitPoints;
        enemy.IsDead = isDead == 1 ? true : false;
        enemy.LoopAnimation = loopAnim == 1 ? true : false;
        enemy.Position[0] = position[0];
        enemy.Position[1] = position[1];
        enemy.Position[2] = position[2];
        enemy.SelfDestructionTime = selfDestructionTime;
        enemy.SlowDownFactor = slowDownFactor;
        enemy.CurrentAnimation = currentAnim;
        m_Ms3dLoader->GetModel(enemy.Id)->ClearTransformations();
        m_Ms3dLoader->GetModel(enemy.Id)->Scale(10.0f, 10.0f, 10.0f);
        m_Ms3dLoader->GetModel(enemy.Id)->Translate (position[0], position[1], position[2]);
        m_Ms3dLoader->GetModel(enemy.Id)->Rotate (-3.14f / 2.0f, 3.14f, 0.0f);
        cml::vector2f oldDirection (0.0f, -1.0f);
        cml::vector2f newDirection (enemy.Direction[0], enemy.Direction[2]);
        float angle = cml::signed_angle_2D (newDirection, oldDirection);
        m_Ms3dLoader->GetModel(enemy.Id)->Rotate (0.0f, angle, 0.0f);
    }
    m_Towers.clear ();
    UINT numTowers;
    fscanf (load, "%u", &numTowers);
    UINT level;
    TowerType type;
    POINT pos;
    float buildingTime, maxBuildingTime;
    for (UINT i = 0; i < numTowers; i++) {
        fscanf (load, "%u %u %u %u %f %f", &type, &level, &pos.x, &pos.y, &buildingTime, &maxBuildingTime);
        CreateTower (type, pos);
        if (level <= MAX_TOWER_LEVEL) {
            m_Towers[i].Level = 0;              /* tower must be already built */
            for (UINT j = 0; j < level; j++) {
                MakeTowerUpgrade (i);
            }
        }
        m_Towers[i].BuildingTime = buildingTime;
        m_Towers[i].MaxBuildingTime = maxBuildingTime;
    }
    fscanf (load, "%f %f %u %u", &m_Resource.Time, &m_Resource.TimeRemaining,
        &m_Resource.Stride, &m_Resource.NumResources);
    fscanf (load, "%u", &m_Score);
    fclose (load);
}

bool Game::IsRayIntersectsObb (const VECTOR3& _rayOrigin, const VECTOR3& _rayDirection, 
                         const VECTOR3& _min, const VECTOR3& _max, float& _distance) {
    float t0, t1, tmp;
    float min = -99999.9f;
    float max  =  99999.9f;
    float epsilon = 0.00001f;
    
    for (UINT i = 0; i < 3; i++) {
      if (fabs(_rayDirection[i]) < epsilon) {
          if ((_rayOrigin[i] < _min[i]) || (_rayOrigin[i] > _max[i]) ) {
              return false;
          }
      } else {
          t0 = (_min[i] - _rayOrigin[i]) / _rayDirection[i];
          t1 = (_max[i] - _rayOrigin[i]) / _rayDirection[i];
          if (t0 > t1) {
            tmp = t0; 
            t0 = t1; 
            t1 = tmp;
          }
          if (t0 > min) {
            min = t0;
          }
          if (t1 < max) {
            max = t1;
          }
          if (min > max) {
            return false;
          }
          if (max < 0.0f) {
            return false;
          }
      }
    }
    
    if (min > 0.0f) {
      _distance = min;
    } else {
      _distance = max;
    }
    
    return true;
}

float Game::GetHeight (const VECTOR3& _Position) {
    float x = _Position[0] / m_Terrain->GetTerrain()->GetScale(0);
    float z = _Position[2] / m_Terrain->GetTerrain()->GetScale(2);
    UINT mapX = (UINT)x;
    UINT mapZ = (UINT)z;
    x = x - floorf (x);
    z = z - floorf (z);
    float h1, h2, h3;
    float r1, r2, r;
    if (x + z < 1.0f) {
        h1 = m_Terrain->GetTerrain()->GetScaledHeight(mapX + 1, mapZ);
        h2 = m_Terrain->GetTerrain()->GetScaledHeight(mapX, mapZ + 1);
        h3 = m_Terrain->GetTerrain()->GetScaledHeight(mapX, mapZ);
        r1 = h2 - (h2 - h1) * x;
        r2 = h3 + (h1 - h3) * x;
        r = r2 + (r1 - r2) * z / (1.0f - x);
    } else {
        h1 = m_Terrain->GetTerrain()->GetScaledHeight(mapX, mapZ + 1);
        h2 = m_Terrain->GetTerrain()->GetScaledHeight(mapX + 1, mapZ);
        h3 = m_Terrain->GetTerrain()->GetScaledHeight(mapX + 1, mapZ + 1);
        r1 = h2 - (h2 - h1) * (1.0f - x);
        r2 = h3 + (h1 - h3) * (1.0f - x);
        r = r2 + (r1 - r2) * (1.0f - z) / x;
    }
    return r;
}

void Game::PauseSounds () {
    std::list<EnemyInfo>::iterator enemy;
    for (enemy = m_Enemies.begin (); enemy != m_Enemies.end (); enemy++) {
        if (!enemy->IsDead) {
            m_Audio->Pause (enemy->SoundId);
        }
    }
    m_Audio->Pause (m_ForestSoundId);
}

void Game::UnpauseSounds () {
    std::list<EnemyInfo>::iterator enemy;
    for (enemy = m_Enemies.begin (); enemy != m_Enemies.end (); enemy++) {
        if (!enemy->IsDead) {
            m_Audio->Unpause (enemy->SoundId);
        }
    }
    m_Audio->Unpause (m_ForestSoundId);
}