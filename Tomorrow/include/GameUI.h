#include "../include/RenderDevice.h"
#include <map>
#include <list>

#define HOOD_HEIGHT 226.0f
#define HOOD_WIDTH 1024.0f
#define HOOD_PANEL_HEIGHT 40.0f
#define HOOD_MAP_UPPER_CORNER_X 208.0f
#define HOOD_MAP_UPPER_CORNER_Y 14.0f
#define HOOD_MAP_LOWER_CORNER_Y 130.0f
#define HOOD_INFO_LOWER_CORNER_X 842.0f
#define HOOD_INFO_UPPER_CORNER_Y 0.0f
/* Basic tower */
#define HOOD_TOWER_A_UPPER_X 298.0f
#define HOOD_TOWER_A_UPPER_Y 138.0f
#define HOOD_TOWER_A_LOWER_X 382.0f
#define HOOD_TOWER_A_LOWER_Y 219.0f
/* Slowing tower */
#define HOOD_TOWER_B_UPPER_X 434.0f
#define HOOD_TOWER_B_UPPER_Y 138.0f
#define HOOD_TOWER_B_LOWER_X 518.0f
#define HOOD_TOWER_B_LOWER_Y 219.0f
/* Area tower */
#define HOOD_TOWER_C_UPPER_X 571.0f
#define HOOD_TOWER_C_UPPER_Y 138.0f
#define HOOD_TOWER_C_LOWER_X 655.0f
#define HOOD_TOWER_C_LOWER_Y 219.0f

#define MAP_UPPER_X 8.0f
#define MAP_UPPER_Y 23.0f
#define MAP_LOWER_X 200.0f
#define MAP_LOWER_Y 219.0f
#define INFO_UPPER_X 844.0f
#define INFO_UPPER_Y 2.0f
#define INFO_LOWER_X 1023.0f
#define INFO_LOWER_Y 225.0f
#define HP_UPPER_X 870.0f
#define HP_UPPER_Y 50.0f
#define HP_LOWER_X 990.0f
#define HP_LOWER_Y 65.0f
#define DETAILS_UPPER_X 890.0f
#define DETAILS_UPPER_Y 165.0f
#define DETAILS_LOWER_X 970.0f
#define DETAILS_LOWER_Y 190.0f
#define CASTLE_HP_X 870.0f
#define CASTLE_HP_Y 25.0f
#define HEADER_X 870.0f
#define HEADER_Y 25.0f
#define DESC_X 870.0f
#define DESC_Y 50.0f
#define RESOURCE_X 50.0f
#define RESOURCE_Y 10.0f
#define NEW_WAVE_X 300.0f
#define NEW_WAVE_Y 10.0f
#define MESSAGE_BOX_HEIGHT 300.0f
#define MESSAGE_BOX_WIDTH 375.0f
#define MESSAGE_BOX_TITLE_OFFSET_X 17.0f
#define MESSAGE_BOX_TITLE_OFFSET_Y 17.0f
#define MESSAGE_BOX_TEXT_OFFSET_X 30.0f
#define MESSAGE_BOX_TEXT_OFFSET_Y 75.0f
#define MESSAGE_BOX_CLOSE_UPPER_X 316.0f
#define MESSAGE_BOX_CLOSE_UPPER_Y 10.0f
#define MESSAGE_BOX_CLOSE_LOWER_X 365.0f
#define MESSAGE_BOX_CLOSE_LOWER_Y 45.0f

#define MAIN_SCREEN_NEW_GAME_UPPER_X 40.0f
#define MAIN_SCREEN_NEW_GAME_UPPER_Y 40.0f
#define MAIN_SCREEN_NEW_GAME_LOWER_X 343.0f
#define MAIN_SCREEN_NEW_GAME_LOWER_Y 100.0f
#define MAIN_SCREEN_CONTINUE_UPPER_X 40.0f
#define MAIN_SCREEN_CONTINUE_UPPER_Y 140.0f
#define MAIN_SCREEN_CONTINUE_LOWER_X 321.0f
#define MAIN_SCREEN_CONTINUE_LOWER_Y 200.0f
#define MAIN_SCREEN_CONTROLS_UPPER_X 680.0f
#define MAIN_SCREEN_CONTROLS_UPPER_Y 155.0f
#define MAIN_SCREEN_CONTROLS_LOWER_X 961.0f
#define MAIN_SCREEN_CONTROLS_LOWER_Y 215.0f
#define MAIN_SCREEN_EXIT_UPPER_X 40.0f
#define MAIN_SCREEN_EXIT_UPPER_Y 240.0f
#define MAIN_SCREEN_EXIT_LOWER_X 166.0f
#define MAIN_SCREEN_EXIT_LOWER_Y 300.0f

#define MAX_NUM_MESSAGES 5

class GameUI {
public:
    GameUI (RenderDevice* _Device, UINT _WindowWidth, UINT _WindowHeight);
    ~GameUI ();
    void RenderMainScreen (bool _isSaved);
    void RenderLoadingScreen ();
    void RenderControlsScreen ();
    bool IsCursorOnNewGame (POINT _cursor);
    bool IsCursorOnContinueGame (POINT _cursor);
    bool IsCursorOnControlsButton (POINT _cursor);
    bool IsCursorOnExitGame (POINT _cursor);
    void Render (const VECTOR3& _Position, float _delta);
    bool InitMainHood (const char* _MapTexture, const char* _HoodTexture, float _MapSizeX, float _MapSizeY);
    void UpdateNumResources (UINT _numResources);
    void EnableDescription (const char* _Header, const char* _Description);
    void DisableDescription ();
    void EnableMainHood ();
    void DisableMainHood ();
    void SetCastleHitPoints (UINT _hitPoints);
    void DecreaseCastleHitPoints (UINT _hitPoints);
    void IncreaseCastleHitPoints (UINT _hitPoints);
    UINT GetCastleHitPoints ();
    void SetMaxCastleHitPoints (UINT _maxHitPoints);
    void UpdateEnemyMark (UINT _EnemyId, VECTOR3 _Position);
    void RemoveEnemyMark (UINT _EnemyId);
    void UpdateTowerMark (UINT _TowerId, VECTOR3 _Position);
    void RemoveTowerMark (UINT _TowerId);
    void RemoveAllMarks ();
    bool IsCursorOnHood (POINT _cursor);
    bool IsCursorOnTowerA (POINT _cursor);
    bool IsCursorOnTowerB (POINT _cursor);
    bool IsCursorOnTowerC (POINT _cursor);
    bool IsCursorOnMap (POINT _cursor, VECTOR3& _mapPosition);
    bool IsCursorOnInfoButton (POINT _cursor);
    bool IsUpgradeButtonVisible ();
    void EnableUpgradeButton ();
    void DisableUpgradeButton ();
    bool IsDetailsButtonVisible ();
    void EnableDetailsButton ();
    void DisableDetailsButton ();
    void UpdateNextWaveTime (float _timeLeft);
    void ShowMessage (const char* _message, DWORD _color, float _duration);
    void ShowMessageBox (const char* _title, const char* _message);
    bool IsMessageBoxOpened ();
    void ProcessMessageBoxInput (POINT _cursor);
private:
    struct Offset {
        float X;
        float Y;
    };

    struct MessageInfo {
        char Text[MAX_PATH];
        float Duration;
        DWORD Color;
        Offset Position;
    };

    struct MapObject {
        DWORD Color;
        cml::vector2f Location;
    };

    struct MainScreenSkin {
        UINT Background;
        UINT NewGame;
        UINT OnNewGame;
        UINT Continue;
        UINT OnContinue;
        UINT InactiveContinue;
        UINT Controls;
        UINT OnControls;
        UINT Exit;
        UINT OnExit;
        UINT LoadingScreen;
        UINT ControlsScreen;
    };

    void SetupMainScreen ();
    void UpdateMark (UINT _Id, VECTOR3 _Position, DWORD _Color);

    bool m_IsMainHoodEnabled;

    UINT m_MainHoodSkinId;
    vs3d::TLVERTEX* m_MainHoodVertex;
    WORD* m_MainHoodIndex;

    UINT m_HoodPanelSkinId;
    vs3d::TLVERTEX* m_HoodPanelVertex;

    float m_MapSizeX;
    float m_MapSizeY;
    UINT m_HoodMapSkinId;
    vs3d::TLVERTEX* m_HoodMapVertex;
    vs3d::TLCVERTEX* m_HoodMapPos;
    std::map<UINT, MapObject> m_MapObjects; // Key = type * 10000 + id where type = {enemy = 1, tower = 2, object = 3}

    vs3d::TLCVERTEX* m_MainInfoVertex;
    vs3d::TLCVERTEX* m_CastleHitPointsVertex;
    WORD* m_CastleHitPointsIndex;
    UINT m_HitPoints;
    UINT m_MaxHitPoints;
    Offset m_CastleHPTextPos;
    char m_Header[MAX_PATH];
    Offset m_HeaderTextPos;
    char m_Description[MAX_PATH];
    Offset m_DescriptionTextPos;
    char m_ResourceText[50];
    Offset m_ResourceTextPos;

    char m_NextWaveTimeMsg[MAX_PATH];
    Offset m_NextWaveTimeMsgPos;

    vs3d::TLVERTEX* m_MessageBox;
    UINT m_MessageBoxSkinId;
    char m_MessageBoxTitle[MAX_PATH];
    char m_MessageBoxText[MAX_PATH];
    bool m_IsMessageBoxOpened;
    vs3d::TLVERTEX* m_InfoButton;
    bool m_IsDetailsButtonVisible;
    UINT m_DetailsButtonSkinId;
    bool m_IsUpgradeButtonVisible;
    UINT m_UpgradeButtonSkinId;

    std::list<MessageInfo> m_Message;

    MainScreenSkin m_MainScreenSkin;
    UINT m_MainScreenBuffer;

    RenderDevice* m_Device;
    UINT m_WindowWidth;
    UINT m_WindowHeight;
};