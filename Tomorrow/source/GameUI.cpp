#include "../include/GameUI.h"

GameUI::GameUI (RenderDevice* _Device, UINT _WindowWidth, UINT _WindowHeight) {
    m_Device = _Device;
    m_IsMainHoodEnabled = false;
    m_HoodPanelSkinId = INVALID_ID;
    m_HoodPanelVertex = NULL;
    m_MainHoodSkinId = INVALID_ID;
    m_MainHoodVertex = NULL;
    m_MainHoodIndex = NULL;
    m_MainInfoVertex = NULL;
    m_HoodMapSkinId = INVALID_ID;
    m_HoodMapVertex = NULL;
    m_CastleHitPointsVertex = NULL;
    m_CastleHitPointsIndex = NULL;
    m_InfoButton = NULL;
    m_IsDetailsButtonVisible = false;
    m_DetailsButtonSkinId = INVALID_ID;
    m_IsUpgradeButtonVisible = false;
    m_UpgradeButtonSkinId = INVALID_ID;
    m_MessageBox = NULL;
    m_MessageBoxSkinId = INVALID_ID;
    m_IsMessageBoxOpened = false;
    ZeroMemory (m_MessageBoxTitle, MAX_PATH);
    ZeroMemory (m_MessageBoxText, MAX_PATH);
    m_MaxHitPoints = 100;
    m_HitPoints = 100;
    m_WindowWidth = _WindowWidth;
    m_WindowHeight = _WindowHeight;
    m_Header[0] = '\0';
    m_Description[0] = '\0';
    m_ResourceText[0] = '\0';
}

GameUI::~GameUI () {
    delete[] m_HoodPanelVertex;
    delete[] m_MainHoodVertex;
    delete[] m_MainHoodIndex;
    delete[] m_MainInfoVertex;
    delete[] m_HoodMapVertex;
    delete[] m_CastleHitPointsVertex;
    delete[] m_CastleHitPointsIndex;
    delete[] m_InfoButton;
    delete[] m_MessageBox;
}

void GameUI::Render (const VECTOR3& _Position, float _delta) {
    if (m_IsMainHoodEnabled) {
        m_Device->GetVCacheManager()->Render (
            PT_TRIANGLELIST,
            m_HoodMapVertex,
            4,
            m_MainHoodIndex,
            6,
            VFT_TL,
            m_HoodMapSkinId);
        m_Device->GetVCacheManager()->Flush();
        m_Device->GetVCacheManager()->Render (
            PT_TRIANGLELIST,
            m_MainInfoVertex,
            4,
            m_MainHoodIndex,
            6,
            VFT_TLC,
            INVALID_ID);
        m_Device->GetVCacheManager()->Flush();
        if (strlen(m_Description) == 0) {
            m_Device->GetVCacheManager()->Render (
                PT_TRIANGLELIST,
                m_CastleHitPointsVertex,
                8,
                m_CastleHitPointsIndex,
                12,
                VFT_TLC,
                INVALID_ID);
        }
        m_Device->EnablePointsScale();
        m_Device->SetPointsSize (2.0f);
        vs3d::TLCVERTEX* mapObjectData = new vs3d::TLCVERTEX[m_MapObjects.size()];
        UINT numData = 0;
        std::map<UINT, MapObject>::iterator i;
        for (i = m_MapObjects.begin(); i != m_MapObjects.end(); i++) {
            mapObjectData[numData].Color = i->second.Color;
            mapObjectData[numData].X = i->second.Location[0];
            mapObjectData[numData].Y = i->second.Location[1];
            mapObjectData[numData].Z = 0.0f;
            mapObjectData[numData].RHW = 1.0f;
            numData++;
        }
        m_Device->GetVCacheManager()->Render (PT_POINT, mapObjectData, numData, NULL, 0, VFT_TLC, INVALID_ID);
        m_Device->DisablePointsScale();
        delete[] mapObjectData;
        m_Device->GetVCacheManager()->Render (
            PT_TRIANGLELIST,
            m_HoodPanelVertex,
            4,
            m_MainHoodIndex,
            6,
            VFT_TL,
            m_HoodPanelSkinId);
        m_Device->SetTextureStageState (0, TSS_ALPHAARG1, TA_TEXTURE);
        m_Device->SetTextureStageState (0, TSS_ALPHAOP, TOP_SELECTARG1);
        m_Device->SetAlphaBlendState (AS_SRCBLEND, BLEND_SRCALPHA);
        m_Device->SetAlphaBlendState (AS_DESTBLEND, BLEND_INVSRCALPHA);
        m_Device->EnableAlphaBlend ();
        m_Device->GetVCacheManager()->Render (
            PT_TRIANGLELIST, 
            m_MainHoodVertex,
            4,
            m_MainHoodIndex,
            6,
            VFT_TL,
            m_MainHoodSkinId);
        m_Device->DisableAlphaBlend ();
        m_Device->GetVCacheManager()->SetTextSize (12);
        m_Device->GetVCacheManager()->RenderText (
            m_ResourceText,
            0xff000000,
            m_ResourceTextPos.X,
            m_ResourceTextPos.Y);
        if (m_IsMessageBoxOpened) {
            m_Device->GetVCacheManager()->Render (
                PT_TRIANGLELIST,
                m_MessageBox,
                4,
                m_MainHoodIndex,
                6,
                VFT_TL,
                m_MessageBoxSkinId);
            m_Device->GetVCacheManager()->Flush (); // render now to make text on top
            m_Device->GetVCacheManager()->SetTextSize (16);
            m_Device->GetVCacheManager()->RenderText (
                m_MessageBoxTitle,
                0xff000000,
                m_MessageBox[1].X + MESSAGE_BOX_TITLE_OFFSET_X,
                m_MessageBox[1].Y + MESSAGE_BOX_TITLE_OFFSET_Y);
            m_Device->GetVCacheManager()->SetTextSize (10);
            m_Device->GetVCacheManager()->RenderText (
                m_MessageBoxText,
                0xff000000,
                m_MessageBox[1].X + MESSAGE_BOX_TEXT_OFFSET_X,
                m_MessageBox[1].Y + MESSAGE_BOX_TEXT_OFFSET_Y);
        }
        if (strlen(m_Description) == 0) {
            m_Device->GetVCacheManager()->SetTextSize(16);
            m_Device->GetVCacheManager()->RenderText(
                "Castle HP:", 
                0xff000000, 
                m_CastleHPTextPos.X,
                m_CastleHPTextPos.Y);
        } else {
            m_Device->GetVCacheManager()->SetTextSize(16);
            m_Device->GetVCacheManager()->RenderText(
                m_Header,
                0xff000000,
                m_HeaderTextPos.X,
                m_HeaderTextPos.Y);
            m_Device->GetVCacheManager()->SetTextSize(10);
            m_Device->GetVCacheManager()->RenderText(
                m_Description,
                0xff000000,
                m_DescriptionTextPos.X,
                m_DescriptionTextPos.Y);
        }
        if (m_IsDetailsButtonVisible) {
            m_Device->GetVCacheManager()->Render (
                PT_TRIANGLELIST, 
                m_InfoButton, 
                4, 
                m_MainHoodIndex, 
                6, 
                VFT_TL, 
                m_DetailsButtonSkinId);
        } else if (m_IsUpgradeButtonVisible) {
            m_Device->GetVCacheManager()->Render (
                PT_TRIANGLELIST, 
                m_InfoButton, 
                4, 
                m_MainHoodIndex, 
                6, 
                VFT_TL, 
                m_UpgradeButtonSkinId);
        }

        std::list<MessageInfo>::iterator msg = m_Message.begin();
        UINT numMessages = 0;
        while (msg != m_Message.end() && numMessages < MAX_NUM_MESSAGES) {
            m_Device->GetVCacheManager()->SetTextSize (16);
            m_Device->GetVCacheManager()->RenderText (
                msg->Text,
                msg->Color,
                msg->Position.X,
                msg->Position.Y + 20.0f * numMessages);
            msg->Duration -= _delta;
            
            if (msg->Duration < 0) {
                msg = m_Message.erase (msg);
            } else {
                msg++;
            }
            numMessages++;
        }
        m_Device->GetVCacheManager()->SetTextSize (12);
        m_Device->GetVCacheManager()->RenderText (
            m_NextWaveTimeMsg,
            0xff000000,
            m_NextWaveTimeMsgPos.X,
            m_NextWaveTimeMsgPos.Y);
    }
}

bool GameUI::InitMainHood (const char* _MapTexture, const char* _HoodTexture, float _MapSizeX, float _MapSizeY) {
    SetupMainScreen ();

    m_MapSizeX = _MapSizeX;
    m_MapSizeY = _MapSizeY;

    float hoodBottomOffset = (m_WindowWidth - HOOD_WIDTH) / 2;
    if (hoodBottomOffset < 0) {
        hoodBottomOffset = 0;
    }

    m_HoodPanelSkinId = m_Device->GetSkinManager()->AddSkin("data/hood/Panel.png");

    m_HoodPanelVertex = new vs3d::TLVERTEX[4];
    m_HoodPanelVertex[0] = vs3d::TLVERTEX (hoodBottomOffset, HOOD_PANEL_HEIGHT, 0.0f, 1.0f, 0xffffffff, 0.0, 1.0);
    m_HoodPanelVertex[1] = vs3d::TLVERTEX (hoodBottomOffset, 0.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f);
    m_HoodPanelVertex[2] = vs3d::TLVERTEX (m_WindowWidth - hoodBottomOffset, 0.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    m_HoodPanelVertex[3] = vs3d::TLVERTEX (m_WindowWidth - hoodBottomOffset, HOOD_PANEL_HEIGHT, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);

    m_MainHoodSkinId = m_Device->GetSkinManager()->AddSkin(_HoodTexture);

    m_MainHoodVertex = new vs3d::TLVERTEX[4];
    m_MainHoodVertex[0] = vs3d::TLVERTEX (hoodBottomOffset, (float)m_WindowHeight, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    m_MainHoodVertex[1] = vs3d::TLVERTEX (hoodBottomOffset, m_WindowHeight - HOOD_HEIGHT, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.005f);
    m_MainHoodVertex[2] = vs3d::TLVERTEX (m_WindowWidth - hoodBottomOffset, m_WindowHeight - HOOD_HEIGHT, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.005f);
    m_MainHoodVertex[3] = vs3d::TLVERTEX (m_WindowWidth - hoodBottomOffset, (float)m_WindowHeight, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);

    m_MainHoodIndex = new WORD[6];
    m_MainHoodIndex[0] = 0;
    m_MainHoodIndex[1] = 1;
    m_MainHoodIndex[2] = 3;
    m_MainHoodIndex[3] = 1;
    m_MainHoodIndex[4] = 2;
    m_MainHoodIndex[5] = 3;

    m_HoodMapSkinId = m_Device->GetSkinManager()->AddSkin (_MapTexture);

    m_HoodMapVertex = new vs3d::TLVERTEX[4];
    m_HoodMapVertex[0] = vs3d::TLVERTEX (hoodBottomOffset + MAP_UPPER_X, m_WindowHeight - HOOD_HEIGHT + MAP_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f);
    m_HoodMapVertex[1] = vs3d::TLVERTEX (hoodBottomOffset + MAP_UPPER_X, m_WindowHeight - HOOD_HEIGHT + MAP_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    m_HoodMapVertex[2] = vs3d::TLVERTEX (hoodBottomOffset + MAP_LOWER_X, m_WindowHeight - HOOD_HEIGHT + MAP_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);
    m_HoodMapVertex[3] = vs3d::TLVERTEX (hoodBottomOffset + MAP_LOWER_X, m_WindowHeight - HOOD_HEIGHT + MAP_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);

    m_MainInfoVertex = new vs3d::TLCVERTEX[4];
    m_MainInfoVertex[0] = vs3d::TLCVERTEX (hoodBottomOffset + INFO_UPPER_X, m_WindowHeight - HOOD_HEIGHT + INFO_LOWER_Y, 0.0f, 1.0f, 0xffcdcdcd);
    m_MainInfoVertex[1] = vs3d::TLCVERTEX (hoodBottomOffset + INFO_UPPER_X, m_WindowHeight - HOOD_HEIGHT + INFO_UPPER_Y, 0.0f, 1.0f, 0xffcdcdcd);
    m_MainInfoVertex[2] = vs3d::TLCVERTEX (hoodBottomOffset + INFO_LOWER_X, m_WindowHeight - HOOD_HEIGHT + INFO_UPPER_Y, 0.0f, 1.0f, 0xffcdcdcd);
    m_MainInfoVertex[3] = vs3d::TLCVERTEX (hoodBottomOffset + INFO_LOWER_X, m_WindowHeight - HOOD_HEIGHT + INFO_LOWER_Y, 0.0f, 1.0f, 0xffcdcdcd);

    m_DetailsButtonSkinId = m_Device->GetSkinManager()->AddSkin ("data/hood/Details.png");
    m_UpgradeButtonSkinId = m_Device->GetSkinManager()->AddSkin ("data/hood/Upgrade.png");

    m_InfoButton = new vs3d::TLVERTEX[4];
    m_InfoButton[0] = vs3d::TLVERTEX (hoodBottomOffset + DETAILS_UPPER_X, m_WindowHeight - HOOD_HEIGHT + DETAILS_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    m_InfoButton[1] = vs3d::TLVERTEX (hoodBottomOffset + DETAILS_UPPER_X, m_WindowHeight - HOOD_HEIGHT + DETAILS_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f);
    m_InfoButton[2] = vs3d::TLVERTEX (hoodBottomOffset + DETAILS_LOWER_X, m_WindowHeight - HOOD_HEIGHT + DETAILS_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    m_InfoButton[3] = vs3d::TLVERTEX (hoodBottomOffset + DETAILS_LOWER_X, m_WindowHeight - HOOD_HEIGHT + DETAILS_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);

    m_MessageBoxSkinId = m_Device->GetSkinManager()->AddSkin ("data/hood/MessageBox.png");

    m_MessageBox = new vs3d::TLVERTEX[4];
    float centerX = m_WindowWidth / 2.0f - (MESSAGE_BOX_WIDTH / 2.0f);
    float centerY = m_WindowHeight / 2.0f - (MESSAGE_BOX_HEIGHT / 2.0f);
    m_MessageBox[0] = vs3d::TLVERTEX (centerX, centerY + MESSAGE_BOX_HEIGHT, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    m_MessageBox[1] = vs3d::TLVERTEX (centerX, centerY, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f);
    m_MessageBox[2] = vs3d::TLVERTEX (centerX + MESSAGE_BOX_WIDTH, centerY, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    m_MessageBox[3] = vs3d::TLVERTEX (centerX + MESSAGE_BOX_WIDTH, centerY + MESSAGE_BOX_HEIGHT, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);

    m_CastleHitPointsVertex = new vs3d::TLCVERTEX[8];
    m_CastleHitPointsVertex[0] = vs3d::TLCVERTEX (hoodBottomOffset + HP_UPPER_X, m_WindowHeight - HOOD_HEIGHT + HP_LOWER_Y, 0.0f, 1.0f, 0xff00ff00);
    m_CastleHitPointsVertex[1] = vs3d::TLCVERTEX (hoodBottomOffset + HP_UPPER_X, m_WindowHeight - HOOD_HEIGHT + HP_UPPER_Y, 0.0f, 1.0f, 0xff00ff00);
    m_CastleHitPointsVertex[2] = vs3d::TLCVERTEX (hoodBottomOffset + HP_LOWER_X, m_WindowHeight - HOOD_HEIGHT + HP_UPPER_Y, 0.0f, 1.0f, 0xff00ff00);
    m_CastleHitPointsVertex[3] = vs3d::TLCVERTEX (hoodBottomOffset + HP_LOWER_X, m_WindowHeight - HOOD_HEIGHT + HP_LOWER_Y, 0.0f, 1.0f, 0xff00ff00);
    m_CastleHitPointsVertex[4] = vs3d::TLCVERTEX (hoodBottomOffset + HP_LOWER_X, m_WindowHeight - HOOD_HEIGHT + HP_LOWER_Y, 0.0f, 1.0f, 0xffff0000);
    m_CastleHitPointsVertex[5] = vs3d::TLCVERTEX (hoodBottomOffset + HP_LOWER_X, m_WindowHeight - HOOD_HEIGHT + HP_UPPER_Y, 0.0f, 1.0f, 0xffff0000);
    m_CastleHitPointsVertex[6] = vs3d::TLCVERTEX (hoodBottomOffset + HP_LOWER_X, m_WindowHeight - HOOD_HEIGHT + HP_UPPER_Y, 0.0f, 1.0f, 0xffff0000);
    m_CastleHitPointsVertex[7] = vs3d::TLCVERTEX (hoodBottomOffset + HP_LOWER_X, m_WindowHeight - HOOD_HEIGHT + HP_LOWER_Y, 0.0f, 1.0f, 0xffff0000);

    m_CastleHitPointsIndex = new WORD[12];
    m_CastleHitPointsIndex[0] = 0;
    m_CastleHitPointsIndex[1] = 1;
    m_CastleHitPointsIndex[2] = 3;
    m_CastleHitPointsIndex[3] = 1;
    m_CastleHitPointsIndex[4] = 2;
    m_CastleHitPointsIndex[5] = 3;
    m_CastleHitPointsIndex[6] = 4;
    m_CastleHitPointsIndex[7] = 5;
    m_CastleHitPointsIndex[8] = 7;
    m_CastleHitPointsIndex[9] = 5;
    m_CastleHitPointsIndex[10] = 6;
    m_CastleHitPointsIndex[11] = 7;

    m_CastleHPTextPos.X = hoodBottomOffset + CASTLE_HP_X;
    m_CastleHPTextPos.Y = m_WindowHeight - HOOD_HEIGHT + CASTLE_HP_Y;
    m_HeaderTextPos.X = hoodBottomOffset + HEADER_X;
    m_HeaderTextPos.Y = m_WindowHeight - HOOD_HEIGHT + HEADER_Y;

    m_DescriptionTextPos.X = hoodBottomOffset + DESC_X;
    m_DescriptionTextPos.Y = m_WindowHeight - HOOD_HEIGHT + DESC_Y;

    m_ResourceTextPos.X = hoodBottomOffset + RESOURCE_X;
    m_ResourceTextPos.Y = RESOURCE_Y;

    m_NextWaveTimeMsgPos.X = hoodBottomOffset + NEW_WAVE_X;
    m_NextWaveTimeMsgPos.Y = NEW_WAVE_Y;
    m_NextWaveTimeMsg[0] = '\0';

    return true;
}

void GameUI::EnableDescription (const char* _Header, const char* _Description) {
    if (_Header) {
        strcpy (m_Header, _Header);
    } else {
        m_Header[0] = '\0';
    }
    if (_Description) {
        strcpy (m_Description, _Description);
    } else {
        m_Description[0] = '\0';
    }
}

void GameUI::DisableDescription () {
    m_Header[0] = '\0';
    m_Description[0] = '\0';
}

void GameUI::EnableMainHood () {
    if (m_MainHoodVertex) {
        m_IsMainHoodEnabled = true;
    }
}

void GameUI::DisableMainHood () {
    m_IsMainHoodEnabled = false;
}

void GameUI::UpdateNumResources (UINT _numResources) {
    sprintf (m_ResourceText, "Resources: %u", _numResources);
}

void GameUI::SetCastleHitPoints (UINT _HitPoints) {
    if (_HitPoints > m_MaxHitPoints) {
        m_HitPoints = m_MaxHitPoints;
    } else {
        m_HitPoints = _HitPoints;
    }
    float hp = m_CastleHitPointsVertex[0].X + (HP_LOWER_X - HP_UPPER_X) * m_HitPoints / m_MaxHitPoints;
    m_CastleHitPointsVertex[2].X = hp;
    m_CastleHitPointsVertex[3].X = hp;
    m_CastleHitPointsVertex[4].X = hp;
    m_CastleHitPointsVertex[5].X = hp;
}

void GameUI::DecreaseCastleHitPoints (UINT _hitPoints) {
    if (m_HitPoints < _hitPoints) {
        m_HitPoints = 0;
    } else {
        m_HitPoints -= _hitPoints;
    }
    float hp = m_CastleHitPointsVertex[0].X + (HP_LOWER_X - HP_UPPER_X) * m_HitPoints / m_MaxHitPoints;
    m_CastleHitPointsVertex[2].X = hp;
    m_CastleHitPointsVertex[3].X = hp;
    m_CastleHitPointsVertex[4].X = hp;
    m_CastleHitPointsVertex[5].X = hp;
}

void GameUI::IncreaseCastleHitPoints (UINT _hitPoints) {
    m_HitPoints += _hitPoints;
    if (m_HitPoints > m_MaxHitPoints) {
        m_HitPoints = m_MaxHitPoints;
    }
    float hp = m_CastleHitPointsVertex[0].X + (HP_LOWER_X - HP_UPPER_X) * m_HitPoints / m_MaxHitPoints;
    m_CastleHitPointsVertex[2].X = hp;
    m_CastleHitPointsVertex[3].X = hp;
    m_CastleHitPointsVertex[4].X = hp;
    m_CastleHitPointsVertex[5].X = hp;
}

UINT GameUI::GetCastleHitPoints () {
    return m_HitPoints;
}

void GameUI::SetMaxCastleHitPoints (UINT _MaxHitPoints) {
    m_MaxHitPoints = _MaxHitPoints;
    if (m_HitPoints > m_MaxHitPoints) {
        m_HitPoints = m_MaxHitPoints;
    }
    float hp = m_CastleHitPointsVertex[0].X + (HP_LOWER_X - HP_UPPER_X) * m_HitPoints / m_MaxHitPoints;
    m_CastleHitPointsVertex[2].X = hp;
    m_CastleHitPointsVertex[3].X = hp;
    m_CastleHitPointsVertex[4].X = hp;
    m_CastleHitPointsVertex[5].X = hp;
}

void GameUI::UpdateMark (UINT _Id, VECTOR3 _Position, DWORD _Color) {
    std::map<UINT, MapObject>::iterator i;
    i = m_MapObjects.find (_Id);
    float mapPositionX = (m_WindowWidth - HOOD_WIDTH) / 2.0f;
    if (mapPositionX < 0.0f) {
        mapPositionX = 0.0f;
    }
    mapPositionX += MAP_UPPER_X;
    float mapSizeX = MAP_LOWER_X - MAP_UPPER_X;
    float mapPositionY = m_WindowHeight - HOOD_HEIGHT + MAP_UPPER_Y;
    float mapSizeY = MAP_LOWER_Y - MAP_UPPER_Y;
    if (i == m_MapObjects.end()) {
        MapObject object;
        object.Color = _Color;
        object.Location[0] = mapPositionX + mapSizeX * _Position[0] / m_MapSizeX;
        object.Location[1] = mapPositionY +  mapSizeY * (1.0f - _Position[2] / m_MapSizeY);
        m_MapObjects[_Id] = object;
    } else {
        i->second.Location[0] = mapPositionX + mapSizeX * _Position[0] / m_MapSizeX;
        i->second.Location[1] = mapPositionY + mapSizeY * (1.0f - _Position[2] / m_MapSizeY);
    }
}

void GameUI::UpdateEnemyMark (UINT _EnemyId, VECTOR3 _Position) {
    UpdateMark (10000 + _EnemyId, _Position, 0xffff0000);
}

void GameUI::RemoveEnemyMark (UINT _EnemyId) {
    std::map<UINT, MapObject>::iterator i = m_MapObjects.find (10000 + _EnemyId);
    if (i != m_MapObjects.end()) {
        m_MapObjects.erase (i);
    }
}

void GameUI::UpdateTowerMark (UINT _TowerId, VECTOR3 _Position) {
    UpdateMark (20000 + _TowerId, _Position, 0xff0000ff);
}

void GameUI::RemoveTowerMark (UINT _TowerId) {
    std::map<UINT, MapObject>::iterator i = m_MapObjects.find (20000 + _TowerId);
    if (i != m_MapObjects.end()) {
        m_MapObjects.erase (i);
    }
}

void GameUI::RemoveAllMarks () {
    m_MapObjects.clear ();
}

bool GameUI::IsCursorOnHood (POINT _Cursor) {
    float hoodBottomOffset = (m_WindowWidth - HOOD_WIDTH) / 2.0f;
    if (hoodBottomOffset < 0) {
        hoodBottomOffset = 0;
    }
    float leftY = m_WindowHeight - HOOD_HEIGHT + HOOD_MAP_UPPER_CORNER_Y;
    float leftMiddleX = hoodBottomOffset + HOOD_MAP_UPPER_CORNER_X;
    float middleY = m_WindowHeight - HOOD_HEIGHT + HOOD_MAP_LOWER_CORNER_Y;
    float rightMiddleX = hoodBottomOffset + HOOD_INFO_LOWER_CORNER_X;
    float rightY = m_WindowHeight - HOOD_HEIGHT + HOOD_INFO_UPPER_CORNER_Y;
    if (_Cursor.x <= leftMiddleX && _Cursor.y > leftY ||
        _Cursor.x > leftMiddleX && _Cursor.x < rightMiddleX && _Cursor.y > middleY ||
        _Cursor.x >= rightMiddleX && _Cursor.y > rightY) {
            return true;
    }
    return false;
}

bool GameUI::IsCursorOnTowerA (POINT _cursor) {
    float hoodBottomOffset = (m_WindowWidth - HOOD_WIDTH) / 2.0f;
    if (hoodBottomOffset < 0) {
        hoodBottomOffset = 0;
    }
    float upperX = hoodBottomOffset + HOOD_TOWER_A_UPPER_X;
    float upperY = m_WindowHeight - HOOD_HEIGHT + HOOD_TOWER_A_UPPER_Y;
    float lowerX = hoodBottomOffset + HOOD_TOWER_A_LOWER_X;
    float lowerY = m_WindowHeight - HOOD_HEIGHT + HOOD_TOWER_A_LOWER_Y;
    if (_cursor.x > upperX && _cursor.x < lowerX &&
        _cursor.y > upperY && _cursor.y < lowerY) {
            return true;
    }
    return false;
}

bool GameUI::IsCursorOnTowerB (POINT _cursor) {
    float hoodBottomOffset = (m_WindowWidth - HOOD_WIDTH) / 2.0f;
    if (hoodBottomOffset < 0) {
        hoodBottomOffset = 0;
    }
    float upperX = hoodBottomOffset + HOOD_TOWER_B_UPPER_X;
    float upperY = m_WindowHeight - HOOD_HEIGHT + HOOD_TOWER_B_UPPER_Y;
    float lowerX = hoodBottomOffset + HOOD_TOWER_B_LOWER_X;
    float lowerY = m_WindowHeight - HOOD_HEIGHT + HOOD_TOWER_B_LOWER_Y;
    if (_cursor.x > upperX && _cursor.x < lowerX &&
        _cursor.y > upperY && _cursor.y < lowerY) {
            return true;
    }
    return false;
}

bool GameUI::IsCursorOnTowerC (POINT _cursor) {
    float hoodBottomOffset = (m_WindowWidth - HOOD_WIDTH) / 2.0f;
    if (hoodBottomOffset < 0) {
        hoodBottomOffset = 0;
    }
    float upperX = hoodBottomOffset + HOOD_TOWER_C_UPPER_X;
    float upperY = m_WindowHeight - HOOD_HEIGHT + HOOD_TOWER_C_UPPER_Y;
    float lowerX = hoodBottomOffset + HOOD_TOWER_C_LOWER_X;
    float lowerY = m_WindowHeight - HOOD_HEIGHT + HOOD_TOWER_C_LOWER_Y;
    if (_cursor.x > upperX && _cursor.x < lowerX &&
        _cursor.y > upperY && _cursor.y < lowerY) {
            return true;
    }
    return false;
}

bool GameUI::IsCursorOnMap (POINT _cursor, VECTOR3& _mapPosition) {
    float hoodBottomOffset = (m_WindowWidth - HOOD_WIDTH) / 2.0f;
    if (hoodBottomOffset < 0) {
        hoodBottomOffset = 0;
    }
    float upperX = hoodBottomOffset + MAP_UPPER_X;
    float upperY = m_WindowHeight - HOOD_HEIGHT + MAP_UPPER_Y;
    float lowerX = hoodBottomOffset + MAP_LOWER_X;
    float lowerY = m_WindowHeight - HOOD_HEIGHT + MAP_LOWER_Y;
    float cursorOnMapX = _cursor.x - upperX;
    float cursorOnMapY = _cursor.y - upperY;
    if (_cursor.x > upperX && _cursor.x < lowerX &&
        _cursor.y > upperY && _cursor.y < lowerY) {
            _mapPosition[0] = cursorOnMapX / (lowerX - upperX) * 1120.0f;
            _mapPosition[1] = 220.0f;
            _mapPosition[2] = cursorOnMapY / (lowerY - upperY) * 1120.0f;
            return true;
    }
    return false;
}

bool GameUI::IsCursorOnInfoButton (POINT _cursor) {
    if (strlen (m_Description) > 0) {
        float hoodBottomOffset = (m_WindowWidth - HOOD_WIDTH) / 2.0f;
        if (hoodBottomOffset < 0) {
            hoodBottomOffset = 0;
        }
        float upperX = hoodBottomOffset + DETAILS_UPPER_X;
        float upperY = m_WindowHeight - HOOD_HEIGHT + DETAILS_UPPER_Y;
        float lowerX = hoodBottomOffset + DETAILS_LOWER_X;
        float lowerY = m_WindowHeight - HOOD_HEIGHT + DETAILS_LOWER_Y;
        if (_cursor.x > upperX && _cursor.x < lowerX &&
            _cursor.y > upperY && _cursor.y < lowerY) {
                return true;
        }
    }
    return false;
}

bool GameUI::IsUpgradeButtonVisible () {
    return m_IsUpgradeButtonVisible;
}

void GameUI::EnableUpgradeButton () {
    m_IsUpgradeButtonVisible = true;
    m_IsDetailsButtonVisible = false;
}

void GameUI::DisableUpgradeButton () {
    m_IsUpgradeButtonVisible = false;
}

bool GameUI::IsDetailsButtonVisible () {
    return m_IsDetailsButtonVisible;
}

void GameUI::EnableDetailsButton () {
    m_IsDetailsButtonVisible = true;
    m_IsUpgradeButtonVisible = false;
}

void GameUI::DisableDetailsButton () {
    m_IsDetailsButtonVisible = false;
}

void GameUI::UpdateNextWaveTime (float _timeLeft) {
    float min = floor (_timeLeft / 60.0f);
    float sec = floor (_timeLeft - min * 60.0f);
    sprintf (m_NextWaveTimeMsg, "Next wave after %.0f:%.0f", min, sec);
}

void GameUI::ShowMessage (const char* _message, DWORD _color, float _duration) {
    MessageInfo message;
    strcpy (message.Text, _message);
    message.Color = _color;
    message.Duration = _duration;
    message.Position.X = m_WindowWidth * 0.4f;
    message.Position.Y = m_WindowHeight * 0.5f;
    m_Message.push_back (message);
    if (m_Message.size () >= MAX_NUM_MESSAGES) {
        m_Message.pop_front ();
    }
}

void GameUI::ShowMessageBox (const char* _title, const char* _message) {
    m_IsMessageBoxOpened = true;
    strcpy (m_MessageBoxTitle, _title);
    strcpy (m_MessageBoxText, _message);
}

bool GameUI::IsMessageBoxOpened () {
    return m_IsMessageBoxOpened;
}

void GameUI::ProcessMessageBoxInput (POINT _cursor) {
    float x = m_WindowWidth / 2.0f - (MESSAGE_BOX_WIDTH / 2.0f);
    float y = m_WindowHeight / 2.0f - (MESSAGE_BOX_HEIGHT / 2.0f);
    float upperX = x + MESSAGE_BOX_CLOSE_UPPER_X;
    float upperY = y + MESSAGE_BOX_CLOSE_UPPER_Y;
    float lowerX = x + MESSAGE_BOX_CLOSE_LOWER_X;
    float lowerY = y + MESSAGE_BOX_CLOSE_LOWER_Y;
    if (_cursor.x > upperX && _cursor.x < lowerX &&
        _cursor.y > upperY && _cursor.y < lowerY) {
            m_IsMessageBoxOpened = false;
    }
}

void GameUI::RenderLoadingScreen () {
    m_Device->BeginRendering (true, false, true);
    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, m_MainScreenBuffer, 0, INVALID_ID, 0, 2, VFT_TL, m_MainScreenSkin.LoadingScreen);
    m_Device->EndRendering ();
}

void GameUI::RenderControlsScreen () {
    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, m_MainScreenBuffer, 0, INVALID_ID, 0, 2, VFT_TL, m_MainScreenSkin.ControlsScreen);
}

void GameUI::RenderMainScreen (bool _isSaved) {
    m_Device->SetTextureStageState (0, TSS_ALPHAARG1, TA_TEXTURE);
    m_Device->SetTextureStageState (0, TSS_ALPHAOP, TOP_SELECTARG1);
    m_Device->SetTextureStageState (1, TSS_COLOROP, TOP_DISABLE);

    m_Device->SetAlphaBlendState (AS_SRCBLEND, BLEND_SRCALPHA);
    m_Device->SetAlphaBlendState (AS_DESTBLEND, BLEND_INVSRCALPHA);

    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, m_MainScreenBuffer, 0, INVALID_ID, 0, 2, VFT_TL, m_MainScreenSkin.Background);
    POINT cursor;
    GetCursorPos (&cursor);

    UINT startGameSkin = IsCursorOnNewGame (cursor) ? m_MainScreenSkin.OnNewGame : m_MainScreenSkin.NewGame;
    m_Device->EnableAlphaBlend();
    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, m_MainScreenBuffer, 6, INVALID_ID, 0, 2, VFT_TL, startGameSkin);

    UINT continueSkin = IsCursorOnContinueGame (cursor) ? m_MainScreenSkin.OnContinue : m_MainScreenSkin.Continue;
    if (!_isSaved) {
        continueSkin = m_MainScreenSkin.InactiveContinue;
    }
    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, m_MainScreenBuffer, 12, INVALID_ID, 0, 2, VFT_TL, continueSkin);

    UINT controlsSkin = IsCursorOnControlsButton (cursor) ? m_MainScreenSkin.OnControls : m_MainScreenSkin.Controls;
    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, m_MainScreenBuffer, 18, INVALID_ID, 0, 2, VFT_TL, controlsSkin);

    UINT exitSkin = IsCursorOnExitGame (cursor) ? m_MainScreenSkin.OnExit : m_MainScreenSkin.Exit;
    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, m_MainScreenBuffer, 24, INVALID_ID, 0, 2, VFT_TL, exitSkin);

    m_Device->DisableAlphaBlend();
}

bool GameUI::IsCursorOnNewGame (POINT _cursor) {
    float upperX = MAIN_SCREEN_NEW_GAME_UPPER_X;
    float upperY = MAIN_SCREEN_NEW_GAME_UPPER_Y;
    float lowerX = MAIN_SCREEN_NEW_GAME_LOWER_X;
    float lowerY = MAIN_SCREEN_NEW_GAME_LOWER_Y;
    if (_cursor.x > upperX && _cursor.x < lowerX &&
        _cursor.y > upperY && _cursor.y < lowerY) {
            return true;
    }
    return false;
}

bool GameUI::IsCursorOnContinueGame (POINT _cursor) {
    float upperX = MAIN_SCREEN_CONTINUE_UPPER_X;
    float upperY = MAIN_SCREEN_CONTINUE_UPPER_Y;
    float lowerX = MAIN_SCREEN_CONTINUE_LOWER_X;
    float lowerY = MAIN_SCREEN_CONTINUE_LOWER_Y;
    if (_cursor.x > upperX && _cursor.x < lowerX &&
        _cursor.y > upperY && _cursor.y < lowerY) {
            return true;
    }
    return false;
}

bool GameUI::IsCursorOnControlsButton (POINT _cursor) {
    float upperX = MAIN_SCREEN_CONTROLS_UPPER_X;
    float upperY = MAIN_SCREEN_CONTROLS_UPPER_Y;
    float lowerX = MAIN_SCREEN_CONTROLS_LOWER_X;
    float lowerY = MAIN_SCREEN_CONTROLS_LOWER_Y;
    if (_cursor.x > upperX && _cursor.x < lowerX &&
        _cursor.y > upperY && _cursor.y < lowerY) {
            return true;
    }
    return false;
}

bool GameUI::IsCursorOnExitGame (POINT _cursor) {
    float upperX = MAIN_SCREEN_EXIT_UPPER_X;
    float upperY = MAIN_SCREEN_EXIT_UPPER_Y;
    float lowerX = MAIN_SCREEN_EXIT_LOWER_X;
    float lowerY = MAIN_SCREEN_EXIT_LOWER_Y;
    if (_cursor.x > upperX && _cursor.x < lowerX &&
        _cursor.y > upperY && _cursor.y < lowerY) {
            return true;
    }
    return false;
}

void GameUI::SetupMainScreen () {
    // Main Screen background
    float width = (float) m_WindowWidth;
    float height = (float) m_WindowHeight;
    vs3d::TLVERTEX mainScreenBackground[6];
    mainScreenBackground[0] = vs3d::TLVERTEX(0.0f, 0.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f);
    mainScreenBackground[1] = vs3d::TLVERTEX(width, 0.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenBackground[2] = vs3d::TLVERTEX(0.0f, height, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenBackground[3] = vs3d::TLVERTEX(0.0f, height, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenBackground[4] = vs3d::TLVERTEX(width, 0.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenBackground[5] = vs3d::TLVERTEX(width, height, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);

    vs3d::TLVERTEX mainScreenStartGame[6];
    mainScreenStartGame[0] = vs3d::TLVERTEX(MAIN_SCREEN_NEW_GAME_UPPER_X, MAIN_SCREEN_NEW_GAME_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f);
    mainScreenStartGame[1] = vs3d::TLVERTEX(MAIN_SCREEN_NEW_GAME_LOWER_X, MAIN_SCREEN_NEW_GAME_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenStartGame[2] = vs3d::TLVERTEX(MAIN_SCREEN_NEW_GAME_UPPER_X, MAIN_SCREEN_NEW_GAME_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenStartGame[3] = vs3d::TLVERTEX(MAIN_SCREEN_NEW_GAME_UPPER_X, MAIN_SCREEN_NEW_GAME_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenStartGame[4] = vs3d::TLVERTEX(MAIN_SCREEN_NEW_GAME_LOWER_X, MAIN_SCREEN_NEW_GAME_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenStartGame[5] = vs3d::TLVERTEX(MAIN_SCREEN_NEW_GAME_LOWER_X, MAIN_SCREEN_NEW_GAME_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);

    vs3d::TLVERTEX mainScreenContinueGame[6];
    mainScreenContinueGame[0] = vs3d::TLVERTEX(MAIN_SCREEN_CONTINUE_UPPER_X, MAIN_SCREEN_CONTINUE_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f);
    mainScreenContinueGame[1] = vs3d::TLVERTEX(MAIN_SCREEN_CONTINUE_LOWER_X, MAIN_SCREEN_CONTINUE_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenContinueGame[2] = vs3d::TLVERTEX(MAIN_SCREEN_CONTINUE_UPPER_X, MAIN_SCREEN_CONTINUE_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenContinueGame[3] = vs3d::TLVERTEX(MAIN_SCREEN_CONTINUE_UPPER_X, MAIN_SCREEN_CONTINUE_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenContinueGame[4] = vs3d::TLVERTEX(MAIN_SCREEN_CONTINUE_LOWER_X, MAIN_SCREEN_CONTINUE_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenContinueGame[5] = vs3d::TLVERTEX(MAIN_SCREEN_CONTINUE_LOWER_X, MAIN_SCREEN_CONTINUE_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);

    vs3d::TLVERTEX mainScreenControlsButton[6];
    mainScreenControlsButton[0] = vs3d::TLVERTEX(MAIN_SCREEN_CONTROLS_UPPER_X, MAIN_SCREEN_CONTROLS_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f);
    mainScreenControlsButton[1] = vs3d::TLVERTEX(MAIN_SCREEN_CONTROLS_LOWER_X, MAIN_SCREEN_CONTROLS_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenControlsButton[2] = vs3d::TLVERTEX(MAIN_SCREEN_CONTROLS_UPPER_X, MAIN_SCREEN_CONTROLS_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenControlsButton[3] = vs3d::TLVERTEX(MAIN_SCREEN_CONTROLS_UPPER_X, MAIN_SCREEN_CONTROLS_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenControlsButton[4] = vs3d::TLVERTEX(MAIN_SCREEN_CONTROLS_LOWER_X, MAIN_SCREEN_CONTROLS_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenControlsButton[5] = vs3d::TLVERTEX(MAIN_SCREEN_CONTROLS_LOWER_X, MAIN_SCREEN_CONTROLS_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);

    vs3d::TLVERTEX mainScreenExitGame[6];
    mainScreenExitGame[0] = vs3d::TLVERTEX(MAIN_SCREEN_EXIT_UPPER_X, MAIN_SCREEN_EXIT_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f);
    mainScreenExitGame[1] = vs3d::TLVERTEX(MAIN_SCREEN_EXIT_LOWER_X, MAIN_SCREEN_EXIT_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenExitGame[2] = vs3d::TLVERTEX(MAIN_SCREEN_EXIT_UPPER_X, MAIN_SCREEN_EXIT_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenExitGame[3] = vs3d::TLVERTEX(MAIN_SCREEN_EXIT_UPPER_X, MAIN_SCREEN_EXIT_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f);
    mainScreenExitGame[4] = vs3d::TLVERTEX(MAIN_SCREEN_EXIT_LOWER_X, MAIN_SCREEN_EXIT_UPPER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f);
    mainScreenExitGame[5] = vs3d::TLVERTEX(MAIN_SCREEN_EXIT_LOWER_X, MAIN_SCREEN_EXIT_LOWER_Y, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f);

    m_MainScreenSkin.LoadingScreen = m_Device->GetSkinManager()->AddSkin ("data/main/Loading.png");
    m_MainScreenSkin.ControlsScreen = m_Device->GetSkinManager()->AddSkin ("data/main/Controls.png");
    m_MainScreenSkin.Background = m_Device->GetSkinManager()->AddSkin ("data/main/Background.png");
    m_MainScreenSkin.NewGame = m_Device->GetSkinManager()->AddSkin ("data/main/NewGame.png");
    m_MainScreenSkin.OnNewGame = m_Device->GetSkinManager()->AddSkin ("data/main/OnNewGame.png");
    m_MainScreenSkin.Continue = m_Device->GetSkinManager()->AddSkin ("data/main/Continue.png");
    m_MainScreenSkin.OnContinue = m_Device->GetSkinManager()->AddSkin ("data/main/OnContinue.png");
    m_MainScreenSkin.InactiveContinue = m_Device->GetSkinManager()->AddSkin ("data/main/InactiveContinue.png");
	m_MainScreenSkin.Controls = m_Device->GetSkinManager()->AddSkin ("data/main/ControlsBtn.png");
    m_MainScreenSkin.OnControls = m_Device->GetSkinManager()->AddSkin ("data/main/OnControlsBtn.png");
    m_MainScreenSkin.Exit = m_Device->GetSkinManager()->AddSkin ("data/main/Exit.png");
    m_MainScreenSkin.OnExit = m_Device->GetSkinManager()->AddSkin ("data/main/OnExit.png");
    m_MainScreenBuffer = m_Device->GetVCacheManager()->CreateStaticVertexBuffer(mainScreenBackground, 6, VFT_TL);
    m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_MainScreenBuffer, mainScreenStartGame, 6, VFT_TL);
    m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_MainScreenBuffer, mainScreenContinueGame, 6, VFT_TL);
    m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_MainScreenBuffer, mainScreenControlsButton, 6, VFT_TL);
    m_Device->GetVCacheManager()->AddToStaticVertexBuffer(m_MainScreenBuffer, mainScreenExitGame, 6, VFT_TL);
}