#pragma once

#include "../include/RenderDevice.h"
#include "../include/RendererLoader.h"
#include "../include/AudioEngineLoader.h"
#include "../include/Log.h"
#include "../include/Window.h"
#include "../include/TerrainEngineLoader.h"
#include "../include/TerrainEngine.h"
#include "../include/ObjManager.h"
#include "../include/ObjModel.h"
#include "../include/FromAboveCamera.h"
#include "../include/InputSystem.h"
#include "../include/FPS_Counter.h"
#include "../include/Ms3dManager.h"
#include "../include/Beam.h"
#include "../include/Bullet.h"
#include "../include/GameUI.h"
#include "../include/Descriptions.h"
#include <vector>
#include <list>
#include <map>

#ifdef _DEBUG
    #pragma comment (lib, "lib/Debug/RendererLoader.lib")
    #pragma comment (lib, "lib/Debug/AudioEngineLoader.lib")
    #pragma comment (lib, "lib/Debug/Log.lib")
    #pragma comment (lib, "lib/Debug/Window.lib")
    #pragma comment (lib, "lib/Debug/TerrainEngineLoader.lib")
    #pragma comment (lib, "lib/Debug/CameraSystem.lib")
    #pragma comment (lib, "lib/Debug/InputSystem.lib")
    #pragma comment (lib, "lib/Debug/ObjManager.lib")
    #pragma comment (lib, "lib/Debug/Ms3dManager.lib")
    #pragma comment (lib, "lib/Debug/ParticleSystem.lib")
#else
    #pragma comment (lib, "lib/RendererLoader.lib")
    #pragma comment (lib, "lib/AudioEngineLoader.lib")
    #pragma comment (lib, "lib/Log.lib")
    #pragma comment (lib, "lib/Window.lib")
    #pragma comment (lib, "lib/TerrainEngineLoader.lib")
    #pragma comment (lib, "lib/CameraSystem.lib")
    #pragma comment (lib, "lib/InputSystem.lib")
    #pragma comment (lib, "lib/ObjManager.lib")
    #pragma comment (lib, "lib/Ms3dManager.lib")
    #pragma comment (lib, "lib/ParticleSystem.lib")
#endif

#define MIN_HEIGHT 180.0f
#define MAX_TOWER_LEVEL 5

enum TowerType {
    BASIC_TOWER = 0,
    SLOWING_TOWER = 1,
    AREA_TOWER = 2
};

struct EnemyAnimation {
    float Start;
    float End;
};

struct EnemyInfo {
    UINT Id;
    UINT WaveId;
    UINT ActiveWaypoint;
    int HitPoints;
    int MaxHitPoints;
    bool IsFast;
    float Speed;
    float SlowDownFactor;
    UINT NumSlowedDown;
    float AttackSpeed;
    float RemainingTimeToShoot;
    Beam* Gun;
    float Ammo;
    float MaxAmmo;
    UINT Power;
    VECTOR3 Position;
    VECTOR3 Direction;
    float Distance;
    UINT NumAttackers;
    UINT NumResources;
    UINT SoundId;
    std::map<std::string, EnemyAnimation> Animation;
    std::string CurrentAnimation;
    float AnimationSpeed;
    bool LoopAnimation;
    bool IsDead;
    float SelfDestructionTime;
};

struct WaveInfo {
    float Delay;
    float EnemySpawnTime;
    float SpawnTimeRemaining;
    char Filename[MAX_PATH];
    char AnimationFile[MAX_PATH];
    float StrengthBonus;
    UINT HitPoints;
    bool IsFast;
    float Speed;
    float AttackSpeed;
    float MaxAmmo;
    UINT Power;
    UINT NumEnemies;
    UINT NumResources;
    float ShotTime;
    float ShotPauseTime;
};

struct TowerGunInfo {
    float ShootingTime;
    bool IsTargetAcquired;
    bool IsShootingUpdated;
    std::list<EnemyInfo>::iterator Target;
};

struct TowerInfo {
    UINT Id;
    TowerType Type;
    UINT Level;
    UINT Price;
    char Description[MAX_PATH * 2];
    float Radius;
    float AttackSpeed;
    float ShootDelay;
    UINT Power;
    POINT Location;
    Bullet* Gun;
    std::list<TowerGunInfo> GunInfo;
    UINT SoundId;
    UINT Size;
    float BuildingTime;
    float MaxBuildingTime;
};

struct TowerUpgradeInfo {
    float UpgradeTime[MAX_TOWER_LEVEL];
    float RadiusIncrease[MAX_TOWER_LEVEL];
    float AttackSpeedDecrease[MAX_TOWER_LEVEL];
    UINT PowerIncrease[MAX_TOWER_LEVEL];
    UINT Price[MAX_TOWER_LEVEL];
};

struct ResourceInfo {
    float Time;
    float TimeRemaining;
    UINT Stride;
    UINT NumResources;
};

struct WaypointInfo {
    POINT Position;
    float Length;
    VECTOR3 Direction;
};

struct EnemyAdditionDetails {
    float Delay;
    float SpawnTime;
    std::string Filename;
    std::string AnimInfoFile;
    UINT HitPoints;
    float Speed;
    float AttackSpeed;
    float MaxAmmo;
    UINT Power;
    UINT NumResources;
    UINT NumEnemies; 
    float ShotTime; 
    float ShotPauseTime; 
    bool IsFast;
};

class Game {
public:
    Game (HWND _hMain, HINSTANCE _instance);
    ~Game ();
    /* Input */
    void ProcessMainScreenInput ();
    void ProcessInGameInput ();
    void ProcessInputOnHood (POINT _mouse);
    void ResetInput ();
    void UpdateInput (UINT _Message, UINT _WParam);

    /* Update */
    void Update ();

    /* Setup */
    void StartNew ();
    
    void UnloadLevel ();
    void LoadLevel (const char* _levelFile);
    void Save ();
    void Load ();

    void RenderMainScreen ();
    void RenderShadowMap (float _delta);
    /* Terrain */
    void UpdateTerrain ();
    void RenderTerrain ();
    bool IsTerrainClicked (POINT& _point);
    /* Camera */
    void MoveCamera ();
    void RotateCamera (POINT _mouse);
    void ZoomCamera (float _speed);
    void RenderCameraMovement ();
    /* Towers */
    bool IsSelectingTower (UINT& _towerId);
    void ShowUpgradeInfo (UINT _towerId);
    void SetupTowersInfo (); 
    void RenderTowerRange (float _x, float _y, float _z, float _range);
    bool CreateTower (TowerType _type, POINT _point);
    void RenderTowers (bool _isRenderingShadowMap, bool _isRenderingInactive);
    void UpdateTowers (float _delta);
    void RenderBuildingTime (UINT _towerId);
    void MakeTowerUpgrade (UINT _towerId);
    void RenderTowerGhost (TowerType _type);
    void UpdateTowerShooting (UINT _towerId, float _delta);
    void TowerShoot (float _TowerX, float _TowerY, UINT _TowerId, std::list<EnemyInfo>::iterator _Enemy, const VECTOR3& _NextPosition);
    void UpdateBasicTowerShooting (float _towerX, float _towerY, UINT _towerId, float _delta);
    void UpdateSlowingTowerShooting (float _towerX, float _towerY, UINT _towerId, float _delta);
    void UpdateAreaTowerShooting (float _towerX, float _towerY, UINT _towerId, float _delta);
    bool IsEnemyInTowerRange (float _TowerX, float _TowerY, UINT _TowerId, std::list<EnemyInfo>::iterator _Enemy, VECTOR3& _NextPosition);
    bool IsObjectVisible (float _frustum[6][4], UINT _id);
    /* Enemies */
    bool IsEnemyVisible (float _frustum[6][4], const std::list<EnemyInfo>::iterator _enemy);
    void AddEnemyWave (const EnemyAdditionDetails& _details);
    void UpdateEnemyWaves (float _delta);
    void AddEnemyWaves ();
    void NewEnemy (UINT _waveIndex, float _animSpeed);
    void ChangeEnemyDirection (std::list<EnemyInfo>::iterator _enemy);
    void UpdateEnemyMovement (float _delta);
    void UpdateEnemies (float _delta);
    void RenderEnemies (float _delta);
    void RenderEnemyHealthBar (const std::list<EnemyInfo>::iterator _enemy);
    /* Sound */
    void PauseSounds ();
    void UnpauseSounds ();
    /* Getters */
    inline UINT GetWindowHeight () const {
        return m_WindowHeight;
    }
    inline UINT GetWindowWidth () const {
        return m_WindowWidth;
    }
    inline bool IsPaused () const {
        return m_IsPaused;
    }
    inline bool IsLevelLoaded () const {
        return m_IsLevelLoaded;
    }
    inline bool IsContinuing () const {
        return m_IsContinuing;
    }
protected:
    void SetupScene ();
    void SetupShaders ();
    bool IsRayIntersectsObb (const VECTOR3& _rayOrigin, const VECTOR3& _rayDirection, 
                             const VECTOR3& _min, const VECTOR3& _max, float& _distance);
    void ReadEnemyAnimation (const char* _filename, EnemyInfo& _enemy);
    float GetHeight (const VECTOR3& _position);
    float GetDistanceTime (UINT _towerId, const VECTOR3& _origin, const VECTOR3& _destination);
private:
    RendererLoader* m_RendererLoader;
    TerrainEngineLoader* m_TerrainLoader;
    AudioEngineLoader* m_AudioLoader;
    LogManager* m_Log;
    RenderDevice* m_Device;
    IAudioEngine* m_Audio;
    FromAboveCamera* m_Camera;
    InputSystem* m_Input;
    Ms3dLoader* m_Ms3dLoader;
    TerrainEngine* m_Terrain;
    GameUI* m_GameUI;

    FpsCounter m_Timer;

    UINT m_WindowHeight;
    UINT m_WindowWidth;

    /* Moving camera */
    POINT m_StartingPosition;
    bool m_IsMoving;

    /* Shadow map details */
    float m_ShadowMapFarClip;
    MATRIX44 m_ShadowMapLightView;
    MATRIX44 m_ShadowMapLightProj;
    MATRIX44 m_ShadowMapLightViewProj;
    VECTOR3 m_ShadowMapLightDir;
    VECTOR3 m_ShadowMapLightEye;
    VECTOR3 m_ShadowMapLightTarget;

    bool m_IsLevelLoaded;
    bool m_IsContinuing;
    bool m_IsSaved;
    bool m_IsPaused;
    bool m_IsShowingControls;

    UINT m_TerrainEffect;   /* Terrain rendering effect */

    UINT m_AudioBankId;
    UINT m_ForestSoundId;

    float m_Frustum[6][4];

    UINT m_ObjectEffect;
    std::vector<UINT> m_Objects;
    std::vector<WaveInfo> m_EnemyWaves;
    float m_NextWaveTimeLeft;
    std::list<EnemyInfo> m_Enemies;
    std::vector<WaypointInfo> m_Waypoints;
    int m_FinalWaypointIndex;

    bool m_KeepInfoMessage;
    UINT m_BuildingFieldTextureId;
    std::vector<std::vector<bool>> m_Occupied;
    ObjManager* m_ObjManager;
    std::vector<TowerInfo> m_Towers;
    std::vector<TowerInfo> m_PreparedTowers;
    std::vector<TowerUpgradeInfo> m_UpgradeInfo;
    TowerType m_BuildingTowerType;
    UINT m_SelectedTowerId;
    std::vector<UINT> m_TowerGhost;
    bool m_ShouldRenderTowerGhost;

    float m_SpeedUpFactor;

    ResourceInfo m_Resource;
    UINT m_Score;
};