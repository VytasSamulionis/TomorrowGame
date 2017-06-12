#include "../include/Game.h"

void Game::ProcessMainScreenInput () {
    if (m_IsShowingControls) {
        if (m_Input->WasPressed (VK_ESCAPE)) {
            m_IsShowingControls = false;
        }
    } else {
        if (m_Input->WasPressed (VK_LBUTTON)) {
            POINT cursor;
            GetCursorPos (&cursor);
            if (m_GameUI->IsCursorOnNewGame (cursor)) {
                m_IsContinuing = false;
                m_IsLevelLoaded = false;
                m_IsPaused = false;
            } else if (m_GameUI->IsCursorOnContinueGame (cursor) && m_IsSaved) {
                m_IsContinuing = true;
                m_IsPaused = false;
                if (m_IsLevelLoaded) {
                    UnpauseSounds ();
                }
            } else if (m_GameUI->IsCursorOnControlsButton (cursor)) {
                m_IsShowingControls = true;
            } else if (m_GameUI->IsCursorOnExitGame (cursor)) {
                PostQuitMessage (0);
            }
        }
    }
}

void Game::ProcessInGameInput () {
    if (m_IsPaused) {
        ProcessMainScreenInput ();
    } else {
        if (!m_IsLevelLoaded) {
            return;
        }
        POINT mouse;
        GetCursorPos (&mouse);
        MoveCamera ();
        RotateCamera (mouse);
        if (m_GameUI->IsMessageBoxOpened ()) {
            if (m_Input->WasPressed (VK_LBUTTON)) {
                m_GameUI->ProcessMessageBoxInput (mouse);
            }
        } else if (!m_GameUI->IsCursorOnHood (mouse)) {
            if (!m_ShouldRenderTowerGhost && m_SelectedTowerId != INVALID_ID) {
                ShowUpgradeInfo (m_SelectedTowerId);
                m_KeepInfoMessage = true;
                if (m_Towers[m_SelectedTowerId].Level < MAX_TOWER_LEVEL && m_Towers[m_SelectedTowerId].BuildingTime <= 0.0f) {
                    m_GameUI->EnableUpgradeButton ();
                }
            } else if (!m_ShouldRenderTowerGhost && !m_KeepInfoMessage) {
                m_GameUI->DisableDescription();
            }
            if (m_Input->WasPressed (VK_LBUTTON)) {
                if (m_ShouldRenderTowerGhost) {
                    if (m_Resource.NumResources >= m_PreparedTowers[m_BuildingTowerType].Price) {
                        POINT point;
                        if (IsTerrainClicked (point)) {
                            if (CreateTower (m_BuildingTowerType, point)) {
                                m_Resource.NumResources -= m_PreparedTowers[m_BuildingTowerType].Price;
                                m_GameUI->UpdateNumResources (m_Resource.NumResources);
                                m_SelectedTowerId = m_Towers.size() - 1;
                                ShowUpgradeInfo (m_SelectedTowerId);
                                m_KeepInfoMessage = true;
                                m_GameUI->DisableDetailsButton ();
                                if (m_Towers[m_SelectedTowerId].Level < MAX_TOWER_LEVEL && m_Towers[m_SelectedTowerId].BuildingTime <= 0.0f) {
                                    m_GameUI->EnableUpgradeButton ();
                                }
                                m_ShouldRenderTowerGhost = false;
                            }
                        }
                    } else {
                        m_GameUI->ShowMessage ("Not enough resources.", 0xffff0000, 3.0f);
                    }
                }
            }
        } else {
            ProcessInputOnHood (mouse);
        }
        bool wasRButtonPressed = m_Input->WasPressed (VK_RBUTTON);
        bool wasEscPressed = m_Input->WasPressed (VK_ESCAPE);
        bool shouldShowMainScreen = true;
        if (wasRButtonPressed || wasEscPressed) {
            if (!m_GameUI->IsMessageBoxOpened()) {
                POINT position;
                GetCursorPos (&position);
                bool changeSelection = false;
                if (wasRButtonPressed) {
                    changeSelection = m_IsMoving && position.x == m_StartingPosition.x && position.y == m_StartingPosition.y;
                } else {
                    changeSelection = true;
                }
                if (changeSelection) {
                    if (m_ShouldRenderTowerGhost) {
                        shouldShowMainScreen = false;
                    }
                    m_ShouldRenderTowerGhost = false;
                    m_GameUI->DisableDetailsButton ();
                    if (!m_KeepInfoMessage) {
                        m_GameUI->DisableDescription();
                    }
                    UINT towerId;
                    if (wasRButtonPressed && IsSelectingTower (towerId)) {
                        ShowUpgradeInfo (towerId);
                        m_KeepInfoMessage = true;
                        if (m_Towers[towerId].Level < MAX_TOWER_LEVEL && m_Towers[towerId].BuildingTime <= 0.0f) {
                            m_GameUI->EnableUpgradeButton ();
                        }
                        m_SelectedTowerId = towerId;
                    } else {
                        if (m_SelectedTowerId != INVALID_ID) {
                            shouldShowMainScreen = false;
                        }
                        m_KeepInfoMessage = false;
                        m_GameUI->DisableUpgradeButton ();
                        m_SelectedTowerId = INVALID_ID;
                    }
                }
                m_IsMoving = false;
            }
        }
        if (m_Input->IsPressed (VK_SPACE)) {
            m_SpeedUpFactor = 5.0f;
        } else {
            m_SpeedUpFactor = 1.0f;
        }
        if (wasEscPressed && shouldShowMainScreen) {
            Save ();
            m_IsSaved = m_GameUI->GetCastleHitPoints() != 0;
            PauseSounds ();
            m_IsPaused = true;
            m_Device->SetTextureStageState (0, TSS_ALPHAARG1, TA_TEXTURE);
            m_Device->SetTextureStageState (0, TSS_ALPHAOP, TOP_SELECTARG1);
        }
    }
}

void Game::ProcessInputOnHood (POINT _mouse) {
    VECTOR3 mapPosition;
    if (!m_ShouldRenderTowerGhost && m_SelectedTowerId != INVALID_ID) {
        ShowUpgradeInfo (m_SelectedTowerId);
        m_KeepInfoMessage = true;
        if (m_Towers[m_SelectedTowerId].Level < MAX_TOWER_LEVEL && m_Towers[m_SelectedTowerId].BuildingTime <= 0.0f) {
            m_GameUI->EnableUpgradeButton ();
        }
    } else if (!m_ShouldRenderTowerGhost && !m_KeepInfoMessage) {
        m_GameUI->DisableDescription();
    }
    if (m_GameUI->IsCursorOnTowerA (_mouse)) {
        bool wasLButtonPressed = m_Input->WasPressed (VK_LBUTTON);
        if (!m_ShouldRenderTowerGhost || wasLButtonPressed) {
            char message[MAX_PATH];
            sprintf (
                message, 
                "Attack Speed: %.2f\nPower: %u\nRadius: %.2f\nPrice: %u\nName: Basic tower",
                m_PreparedTowers[BASIC_TOWER].AttackSpeed,
                m_PreparedTowers[BASIC_TOWER].Power,
                m_PreparedTowers[BASIC_TOWER].Radius,
                m_PreparedTowers[BASIC_TOWER].Price);
            m_GameUI->EnableDescription ("Basic Tower", message);
            if (m_GameUI->IsUpgradeButtonVisible()) {
                m_GameUI->DisableUpgradeButton ();
            }
            if (m_GameUI->IsDetailsButtonVisible()) {
                m_GameUI->DisableDetailsButton ();
            }
            m_BuildingTowerType = BASIC_TOWER;
            m_KeepInfoMessage = false;
            if (wasLButtonPressed) {
                m_ShouldRenderTowerGhost = true;
                m_GameUI->EnableDetailsButton ();
            }
        }
    } else if (m_GameUI->IsCursorOnTowerB (_mouse)) {
        bool wasLButtonPressed = m_Input->WasPressed (VK_LBUTTON);
        if (!m_ShouldRenderTowerGhost || wasLButtonPressed) {
            char message[MAX_PATH];
            sprintf (
                message, 
                "Power: %u\nRadius: %.2f\nPrice: %u\nName: Slowing tower",
                m_PreparedTowers[SLOWING_TOWER].Power,
                m_PreparedTowers[SLOWING_TOWER].Radius,
                m_PreparedTowers[SLOWING_TOWER].Price);
            m_GameUI->EnableDescription ("Slowing Tower", message);
            if (m_GameUI->IsUpgradeButtonVisible()) {
                m_GameUI->DisableUpgradeButton ();
            }
            if (m_GameUI->IsDetailsButtonVisible()) {
                m_GameUI->DisableDetailsButton ();
            }
            m_BuildingTowerType = SLOWING_TOWER;
            m_KeepInfoMessage = false;
            if (wasLButtonPressed) {
                m_ShouldRenderTowerGhost = true;
                m_GameUI->EnableDetailsButton ();
            }
        }
    } else if (m_GameUI->IsCursorOnTowerC (_mouse)) {
        bool wasLButtonPressed = m_Input->WasPressed (VK_LBUTTON);
        if (!m_ShouldRenderTowerGhost || wasLButtonPressed) {
            char message[MAX_PATH];
            sprintf (
                message, 
                "Attack Speed: %.2f\nPower: %u\nRadius: %.2f\nPrice: %u\nName: Area tower",
                m_PreparedTowers[AREA_TOWER].AttackSpeed,
                m_PreparedTowers[AREA_TOWER].Power,
                m_PreparedTowers[AREA_TOWER].Radius,
                m_PreparedTowers[AREA_TOWER].Price);
            m_GameUI->EnableDescription ("Area Tower", message);
            if (m_GameUI->IsUpgradeButtonVisible()) {
                m_GameUI->DisableUpgradeButton ();
            }
            if (m_GameUI->IsDetailsButtonVisible()) {
                m_GameUI->DisableDetailsButton ();
            }
            m_BuildingTowerType = AREA_TOWER;
            m_KeepInfoMessage = false;
            if (wasLButtonPressed) {
                m_ShouldRenderTowerGhost = true;
                m_GameUI->EnableDetailsButton ();
            }
        }
    } else if (m_GameUI->IsCursorOnInfoButton (_mouse)) {
        if (m_Input->WasPressed (VK_LBUTTON)) {
            if (m_GameUI->IsDetailsButtonVisible ()) {
                switch (m_BuildingTowerType) {
                    case BASIC_TOWER:
                        m_GameUI->ShowMessageBox ("Basic Tower details", m_PreparedTowers[BASIC_TOWER].Description);
                        break;
                    case SLOWING_TOWER:
                        m_GameUI->ShowMessageBox ("Slowing Tower details", m_PreparedTowers[SLOWING_TOWER].Description);
                        break;
                    case AREA_TOWER:
                        m_GameUI->ShowMessageBox ("Area Tower details", m_PreparedTowers[AREA_TOWER].Description);
                }
            } else if (m_GameUI->IsUpgradeButtonVisible ()) {
                TowerType type = m_Towers[m_SelectedTowerId].Type;
                UINT lvl = m_Towers[m_SelectedTowerId].Level;
                if (lvl < MAX_TOWER_LEVEL) {
                    if (m_Resource.NumResources >= m_UpgradeInfo[type].Price[lvl]) {
                        m_Resource.NumResources -= m_UpgradeInfo[type].Price[lvl];
                        m_GameUI->UpdateNumResources (m_Resource.NumResources);
                        m_Towers[m_SelectedTowerId].BuildingTime = m_UpgradeInfo[type].UpgradeTime[lvl];
                        m_Towers[m_SelectedTowerId].MaxBuildingTime = m_UpgradeInfo[type].UpgradeTime[lvl];
                        ShowUpgradeInfo (m_SelectedTowerId);
                        m_GameUI->DisableUpgradeButton ();
                    } else {
                        m_GameUI->ShowMessage ("Not enough resources", 0xffff0000, 3.0f);
                    }
                }
            }
        }
    } else if (!m_ShouldRenderTowerGhost && m_SelectedTowerId != INVALID_ID) {
        ShowUpgradeInfo (m_SelectedTowerId);
        m_KeepInfoMessage = true;
        if (m_Towers[m_SelectedTowerId].Level < MAX_TOWER_LEVEL && m_Towers[m_SelectedTowerId].BuildingTime <= 0.0f) {
            m_GameUI->EnableUpgradeButton ();
        }
    }
    //m_Log->Log("%u %u\n", _Mouse.x, _Mouse.y);
}

void Game::RenderCameraMovement () {
    VECTOR3 arrowPos[9];
    POINT cursor;
    GetCursorPos (&cursor);
    float x = (cursor.x - m_StartingPosition.x) * 1.0f;
    float y = (cursor.y - m_StartingPosition.y) * 1.0f;
    float length = sqrtf (x * x + y * y);
    arrowPos[0] = VECTOR3 (2.0f, 0.0f, 0.0f);
    arrowPos[1] = VECTOR3 (-2.0f, 0.0f, 0.0f);
    arrowPos[2] = VECTOR3 (-2.0f, length - 10.0f, 0.0f);
    arrowPos[3] = VECTOR3 (2.0f, 0.0f, 0.0f);
    arrowPos[4] = VECTOR3 (-2.0f, length - 10.0f, 0.0f);
    arrowPos[5] = VECTOR3 (2.0f, length - 10.0f, 0.0f);
    arrowPos[6] = VECTOR3 (-10.0f, length - 10.0f, 0.0f);
    arrowPos[7] = VECTOR3 (0.0f, length, 0.0f);
    arrowPos[8] = VECTOR3 (10.0f, length - 10.0f, 0.0f);
    cml::vector2f start ((float)m_StartingPosition.x, (float)m_StartingPosition.y);
    cml::vector2f end  = cml::vector2f ((float)cursor.x, (float)cursor.y) - start;
    cml::vector2f initPos (0.0f, 1.0f);
    float angle = cml::signed_angle_2D (initPos, end);
    MATRIX44 rotation;
    cml::matrix_rotation_world_z (rotation, angle);
    MATRIX44 translation;
    cml::matrix_translation (translation, (float)m_StartingPosition.x, (float)m_StartingPosition.y, 0.0f);
    for (UINT i = 0; i < 9; i++) {
        arrowPos[i] = cml::transform_point(rotation * translation, arrowPos[i]);
    }
    vs3d::TLCVERTEX arrow[9];
    for (UINT i = 0; i < 9; i++) {
        arrow[i].Color = 0xff0000ff;
        arrow[i].RHW = 1.0f;
        arrow[i].X = arrowPos[i][0];
        arrow[i].Y = arrowPos[i][1];
        arrow[i].Z = 0.0f;
    }
    m_Device->SetCullingState (RS_CULL_NONE);
    m_Device->GetVCacheManager()->Render (PT_TRIANGLELIST, arrow, 9, NULL, 0, VFT_TLC, INVALID_ID);
}

void Game::MoveCamera () {
    if (m_Input->IsPressed (VK_RBUTTON) && !m_IsMoving) {
        GetCursorPos (&m_StartingPosition);
        m_IsMoving = true;
    }
    if (m_IsMoving) {
        POINT pos;
        GetCursorPos (&pos);
        float x = (pos.x - m_StartingPosition.x) * 1.0f;
        float y = (m_StartingPosition.y - pos.y) * 1.0f;
        m_Camera->SetSlideSpeed (x);
        VECTOR3 nextPosition = m_Camera->GetNextPosition ();
        if (nextPosition[0] < -300.0f || nextPosition[0] > 1500.0f || nextPosition[2] < -300.0f || nextPosition[2] > 1500.0f) {
            m_Camera->SetSlideSpeed(0.0f);
        }
        m_Camera->SetSpeed (y);
        nextPosition = m_Camera->GetNextPosition ();
        if (nextPosition[0] < -300.0f || nextPosition[0] > 1500.0f || nextPosition[2] < -300.0f || nextPosition[2] > 1500.0f) {
            m_Camera->SetSpeed(0.0f);
        }
    } else {
        m_Camera->SetSlideSpeed (0.0f);
        m_Camera->SetSpeed (0.0f);
    }
}

void Game::RotateCamera (POINT _mouse) {
    float windowLeft = m_WindowWidth * 0.01f;
    float windowRight = m_WindowWidth * 0.99f;
    
    if (_mouse.x < windowLeft) {
        m_Camera->SetRotationSpeed (-3.0f);
    } else if (_mouse.x > windowRight) {
        m_Camera->SetRotationSpeed (3.0f);
    } else {
        m_Camera->SetRotationSpeed (0.0f);
    }
}

void Game::ZoomCamera (float _speed) {
    m_Camera->SetZoomSpeed(_speed);
    if (m_Camera->GetNextPosition()[1] < MIN_HEIGHT || m_Camera->GetNextPosition()[1] > 1500.0f) {
        m_Camera->SetZoomSpeed (0.0f);
    }
}

void Game::ResetInput () {
    m_Input->Reset ();
}

void Game::UpdateInput (UINT _message, UINT _wParam) {
    m_Input->Update (_message, _wParam);
}