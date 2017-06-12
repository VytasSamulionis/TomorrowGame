#include "../include/Game.h"

void Game::AddEnemyWave (const EnemyAdditionDetails& _details) {
    WaveInfo wave;
    wave.Delay = _details.Delay;
    wave.EnemySpawnTime = _details.SpawnTime;
    wave.SpawnTimeRemaining = 0.0f;
    strcpy (wave.Filename, _details.Filename.c_str());
    strcpy (wave.AnimationFile, _details.AnimInfoFile.c_str());
    wave.StrengthBonus = floor (m_EnemyWaves.size () / 5.0f) / 100.0f * 50.0f;   /* +50% for each iteration */
    wave.HitPoints = _details.HitPoints + (UINT)(_details.HitPoints * wave.StrengthBonus);
    wave.IsFast = _details.IsFast;
    wave.Speed = _details.Speed;
    wave.AttackSpeed = _details.AttackSpeed;
    wave.MaxAmmo = _details.MaxAmmo;
    wave.Power = _details.Power + (UINT)(_details.Power * wave.StrengthBonus);
    wave.NumEnemies = _details.NumEnemies;
    wave.NumResources = _details.NumResources;
    wave.ShotTime = _details.ShotTime;
    wave.ShotPauseTime = _details.ShotPauseTime;
    m_EnemyWaves.push_back (wave);
}

void Game::AddEnemyWaves () {
    /* First wave */
    EnemyAdditionDetails details;
    details.Delay = 120.0f;
    details.SpawnTime = 20.321f;
    details.Filename = "data/ms3d/AlienScout/AlienScout.ms3d";
    details.AnimInfoFile = "data/ms3d/AlienScout/AlienScoutAnimInfo.txt";
    details.HitPoints = 100;
    details.Speed = 20.0f;
    details.AttackSpeed = 1.0f;
    details.MaxAmmo = 6.0f;
    details.Power = 10;
    details.NumResources = 2;
    details.NumEnemies = 5;
    details.ShotTime = 0.3f;
    details.ShotPauseTime = 0.95f;
    details.IsFast = false;
    AddEnemyWave (details);
    details.Speed = 40.0f;
    details.NumResources = 5;
    details.IsFast = true;
    AddEnemyWave (details);
    details.Filename = "data/ms3d/AlienInfantry/AlienInfantry.ms3d";
    details.AnimInfoFile = "data/ms3d/AlienInfantry/AlienInfantryAnimInfo.txt";
    details.HitPoints = 200;
    details.Speed = 20.0f;
    details.AttackSpeed = 0.1f;
    details.MaxAmmo = 5.0f;
    details.Power = 3;
    details.ShotTime = 50.0f;
    details.ShotPauseTime = 0.0f;
    details.IsFast = false;
    AddEnemyWave (details);
    details.Speed = 40.0f;
    details.NumResources = 10;
    details.IsFast = true;
    AddEnemyWave (details);
    details.Filename = "data/ms3d/AlienBoss/AlienBoss.ms3d";
    details.AnimInfoFile = "data/ms3d/AlienBoss/AlienBossAnimInfo.txt";
    details.HitPoints = 500;
    details.Speed = 10.0f;
    details.AttackSpeed = 0.1f;
    details.MaxAmmo = 5.0f;
    details.Power = 6;
    details.NumResources = 20;
    details.NumEnemies = 1;
    details.ShotTime = 50.0f;
    details.ShotPauseTime = 0.0f;
    details.IsFast = false;
    AddEnemyWave (details);
}

void Game::UpdateEnemyWaves (float _delta) {
    UINT i = 0;
    UINT numWaves = m_EnemyWaves.size();
    while (i < numWaves) {
        if (m_EnemyWaves[i].Delay > 0) {
            m_EnemyWaves[i].Delay -= _delta * m_SpeedUpFactor;
            m_NextWaveTimeLeft = m_EnemyWaves[i].Delay > 0 ? m_EnemyWaves[i].Delay : 0.0f;
            break;
        }
        if (m_EnemyWaves[i].Delay <= 0) {
            if (m_EnemyWaves[i].NumEnemies > 0) {
                m_EnemyWaves[i].SpawnTimeRemaining -= _delta * m_SpeedUpFactor;
                if (m_EnemyWaves[i].SpawnTimeRemaining <= 0.0f) {
                    NewEnemy (i, 1.0f);
                    m_EnemyWaves[i].SpawnTimeRemaining = m_EnemyWaves[i].EnemySpawnTime;
                    m_EnemyWaves[i].NumEnemies--;
                }
            }
            if (i == numWaves - 1) { /* the last wave */
                AddEnemyWaves ();
            }
        }
        i++;
    }
}

void Game::ReadEnemyAnimation (const char* _filename, EnemyInfo& _enemy) {
    FILE* file = fopen (_filename, "r");
    if (file) {
        while (!feof (file)) {
            char name[MAX_PATH];
            EnemyAnimation animation;
            if (fscanf (file, "%s start: %f end: %f", name, &animation.Start, &animation.End) == 3) {
                _enemy.Animation.insert (std::pair<std::string,EnemyAnimation>(std::string (name), animation));
            }
        }
    } else {
        THROW_DETAILED_ERROR (ERRC_FILE_NOT_FOUND, _filename);
    }
}

void Game::NewEnemy (UINT _waveIndex, float _animSpeed) {
    if (m_Waypoints.size() == 0) {
        return;
    }
    EnemyInfo enemy;
    enemy.Id = m_Ms3dLoader->LoadModel (m_EnemyWaves[_waveIndex].Filename);
    enemy.WaveId = _waveIndex;
    enemy.HitPoints = m_EnemyWaves[_waveIndex].HitPoints;
    enemy.MaxHitPoints = enemy.HitPoints;
    enemy.IsFast = m_EnemyWaves[_waveIndex].IsFast;
    enemy.Speed = m_EnemyWaves[_waveIndex].Speed;
    enemy.SlowDownFactor = 1.0f;
    enemy.NumSlowedDown = 0;
    enemy.AttackSpeed = m_EnemyWaves[_waveIndex].AttackSpeed;
    enemy.RemainingTimeToShoot = 0.0f;
    enemy.Gun = new Beam (3.0f, 30.0f, 0.01f, 50, m_EnemyWaves[_waveIndex].ShotTime, m_EnemyWaves[_waveIndex].ShotPauseTime);
    enemy.Gun->Init (m_Device, NULL);
    enemy.Gun->SetColor (0xffCC0099);
    enemy.Gun->SetMaxRange (200.0f);
    enemy.Gun->SetMaxTime (1.0f);
    enemy.MaxAmmo = m_EnemyWaves[_waveIndex].MaxAmmo;
    enemy.Ammo = enemy.MaxAmmo;
    enemy.Power = m_EnemyWaves[_waveIndex].Power;
    float x = m_Waypoints[0].Position.x * m_Terrain->GetTerrain()->GetScale(0);
    float y = m_Terrain->GetTerrain()->GetScaledHeight(m_Waypoints[0].Position.x, m_Waypoints[0].Position.y);
    float z = m_Waypoints[0].Position.y * m_Terrain->GetTerrain()->GetScale(2);
    m_Ms3dLoader->GetModel(enemy.Id)->Scale(10.0f, 10.0f, 10.0f);
    m_Ms3dLoader->GetModel(enemy.Id)->Translate (x, y, z);
    m_Ms3dLoader->GetModel(enemy.Id)->Rotate (-3.14f / 2.0f, 3.14f, 0.0f);
    enemy.ActiveWaypoint = 0;
    enemy.Distance = 0.0f;
    enemy.Position = VECTOR3(x, y, z);
    enemy.NumAttackers = 0;
    enemy.NumResources = m_EnemyWaves[_waveIndex].NumResources;
    enemy.Direction = m_Waypoints[0].Direction;
    cml::vector2f oldDirection (0.0f, -1.0f);
    cml::vector2f newDirection (enemy.Direction[0], enemy.Direction[2]);
    float angle = cml::signed_angle_2D (newDirection, oldDirection);
    m_Ms3dLoader->GetModel(enemy.Id)->Rotate (0.0f, angle, 0.0f);
    enemy.SoundId = m_Audio->Play3D (m_AudioBankId, "Steps", enemy.Position, m_Waypoints[enemy.ActiveWaypoint].Direction, VECTOR3 (0.0f, 1.0f, 0.0f));
    enemy.SelfDestructionTime = 0.0f;
    /*enemy.AnimationStart = _animStart;
    enemy.AnimationEnd = _animEnd;*/
    ReadEnemyAnimation (m_EnemyWaves[_waveIndex].AnimationFile, enemy);
    enemy.AnimationSpeed = _animSpeed;
    enemy.LoopAnimation = true;
    if (enemy.IsFast) {
        enemy.CurrentAnimation = "Run";
    } else {
        enemy.CurrentAnimation = "Walk";
    }
    enemy.IsDead = false;
    m_Enemies.push_back (enemy);
}

void Game::ChangeEnemyDirection (std::list<EnemyInfo>::iterator _enemy) {
    if (_enemy->ActiveWaypoint + 1 < m_Waypoints.size()) {
        cml::vector2f newDirection = cml::vector2f(m_Waypoints[_enemy->ActiveWaypoint].Direction[0], m_Waypoints[_enemy->ActiveWaypoint].Direction[2]).normalize();
        cml::vector2f oldDirection = cml::vector2f(_enemy->Direction[0], _enemy->Direction[2]).normalize();
        float angle = cml::signed_angle_2D(newDirection, oldDirection);
        m_Ms3dLoader->GetModel(_enemy->Id)->Rotate (0.0f, angle, 0.0f);
        _enemy->Direction = m_Waypoints[_enemy->ActiveWaypoint].Direction;
    } else {    // Reload ammo and send enemy back to fight
        _enemy->Ammo = _enemy->MaxAmmo;
        _enemy->RemainingTimeToShoot = 0.0f;
        float x = m_Waypoints[0].Position.x * m_Terrain->GetTerrain()->GetScale(0);
        float y = m_Terrain->GetTerrain()->GetScaledHeight(m_Waypoints[0].Position.x, m_Waypoints[0].Position.y);
        float z = m_Waypoints[0].Position.y * m_Terrain->GetTerrain()->GetScale(2);
        m_Ms3dLoader->GetModel(_enemy->Id)->ClearTransformations();
        m_Ms3dLoader->GetModel(_enemy->Id)->Scale(10.0f, 10.0f, 10.0f);
        m_Ms3dLoader->GetModel(_enemy->Id)->Translate (x, y, z);
        m_Ms3dLoader->GetModel(_enemy->Id)->Rotate (-3.14f / 2.0f, 3.14f, 0.0f);
        _enemy->ActiveWaypoint = 0;
        _enemy->Position = VECTOR3(x, y, z);
        _enemy->Direction = m_Waypoints[0].Direction;
        _enemy->Distance = 0.0f;
        cml::vector2f oldDirection (0.0f, -1.0f);
        cml::vector2f newDirection (_enemy->Direction[0], _enemy->Direction[2]);
        float angle = cml::signed_angle_2D (newDirection, oldDirection);
        m_Ms3dLoader->GetModel(_enemy->Id)->Rotate (0.0f, angle, 0.0f);
    }
}

void Game::UpdateEnemyMovement (float _Delta) {
    std::list<EnemyInfo>::iterator i;
    for (i = m_Enemies.begin(); i != m_Enemies.end(); i++) {
        if ((int)i->ActiveWaypoint == m_FinalWaypointIndex) {
            if (i->CurrentAnimation.compare ("Shoot") == 0 && i->Ammo > 0.0f) {
                i->Ammo -= _Delta * m_SpeedUpFactor;
                i->RemainingTimeToShoot -= _Delta * m_SpeedUpFactor;
                i->Gun->Update (_Delta * m_SpeedUpFactor);
            } else if (m_Ms3dLoader->GetModel(i->Id)->IsAnimationCompleted ()) {
                if (!i->IsDead) {
                    i->CurrentAnimation = "Shoot";
                    VECTOR3 position = i->Position;
                    position[1] += 20.0f;
                    i->Gun->SetPosition (position + 50.0f * i->Direction, position + 200.0f * i->Direction);
                    i->Gun->AddParticle ();
                    i->LoopAnimation = true;
                }
            }
            while (i->RemainingTimeToShoot < 0.0f) {
                if (m_GameUI->GetCastleHitPoints () != 0) {
                    m_GameUI->DecreaseCastleHitPoints (i->Power);
                    if (m_GameUI->GetCastleHitPoints () == 0) {
                        char gameOverMsg[MAX_PATH];
                        sprintf (gameOverMsg, "Game Over");
                        m_GameUI->ShowMessage (gameOverMsg, 0xffff0000, 120.0f);
                        sprintf (gameOverMsg, "Your score is %u", m_Score);
                        m_GameUI->ShowMessage (gameOverMsg, 0xff00ff00, 120.0f);
                    }
                }
                i->RemainingTimeToShoot += i->AttackSpeed;
            }
            if (i->Ammo > 0.0f) {
                continue;
            } else {
                i->Gun->Destroy ();
                if (i->IsFast) {
                    i->CurrentAnimation = "Run";
                } else {
                    i->CurrentAnimation = "Walk";
                }
                i->LoopAnimation = true;
            }
        }
        if (i->IsDead) {
            continue;
        }
        i->Distance += i->Speed * i->SlowDownFactor * m_SpeedUpFactor * _Delta;
        while (i->Distance > m_Waypoints[i->ActiveWaypoint].Length) {
            i->Distance -= m_Waypoints[i->ActiveWaypoint].Length;
            i->ActiveWaypoint++;
            if (i->ActiveWaypoint == m_Waypoints.size() - 1) {
                i->Distance = m_Waypoints[i->ActiveWaypoint].Length;
            }
        }
        float posx = m_Waypoints[i->ActiveWaypoint].Position.x * m_Terrain->GetTerrain()->GetScale(0);
        float posz = m_Waypoints[i->ActiveWaypoint].Position.y * m_Terrain->GetTerrain()->GetScale(2);
        VECTOR3 position = VECTOR3 (posx, 0.0f, posz);
        VECTOR3 newPosition = position + m_Waypoints[i->ActiveWaypoint].Direction * i->Distance;
        newPosition[1] = GetHeight (newPosition);
        position[1] = newPosition[1];
        m_GameUI->UpdateEnemyMark (i->Id, newPosition);
        float x = newPosition[0] - i->Position[0];
        float y = newPosition[1] - i->Position[1];
        float z = newPosition[2] - i->Position[2];
        m_Ms3dLoader->GetModel(i->Id)->Translate(x, y, z);
        i->Position = newPosition;
        if (i->ActiveWaypoint != m_FinalWaypointIndex || i->Ammo <= 0.0f) {
            ChangeEnemyDirection (i);
        }
        if (i->ActiveWaypoint == m_FinalWaypointIndex && i->Ammo > 0.0f) {
            i->CurrentAnimation = "PrepareToShoot";
            i->LoopAnimation = false;
        }
        m_Audio->Update3DSoundPosition (i->SoundId, i->Position);
        m_Audio->Update3DSoundFront (i->SoundId, m_Waypoints[i->ActiveWaypoint].Direction);
    }
}

void Game::UpdateEnemies (float _delta) {
    std::list<EnemyInfo>::iterator i = m_Enemies.begin();
    while (i != m_Enemies.end()) {
        std::map<std::string, EnemyAnimation>::iterator animation;
        animation = i->Animation.find (i->CurrentAnimation);
        if (animation != i->Animation.end ()) {
            m_Ms3dLoader->GetModel(i->Id)->Animate(
                i->AnimationSpeed * m_SpeedUpFactor, 
                animation->second.Start,
                animation->second.End,
                i->LoopAnimation);
        }
        i++;
    }
}

void Game::RenderEnemies (float _delta) {
    std::list<EnemyInfo>::iterator i = m_Enemies.begin();
    while (i != m_Enemies.end()) {
        if (IsEnemyVisible (m_Frustum, i)) {
            m_Ms3dLoader->GetModel(i->Id)->Render(m_Device);
        }
        if (i->IsDead) {
            i->SelfDestructionTime -= _delta * m_SpeedUpFactor;
            if (i->SelfDestructionTime <= 0.0f) {
                m_Ms3dLoader->GetModel(i->Id)->Unload ();
                m_GameUI->RemoveEnemyMark (i->Id);
                delete i->Gun;
                i = m_Enemies.erase (i);
            } else {
                i++;
            }
        } else {
            i->Gun->Render ();
            i++;
        }
    }
    
}

void Game::RenderEnemyHealthBar (const std::list<EnemyInfo>::iterator _Enemy) {
    if (_Enemy == m_Enemies.end()) {
        return;
    }
    vs3d::ULCVERTEX healthBar[8];
    WORD index[12];
    MATRIX44 view = m_Device->GetViewMatrix();
    VECTOR3 right (view(0, 0), view(1, 0), view(2, 0));
    right = right.normalize();
    VECTOR3 up (view(0, 1), view(1, 1), view(2, 1));
    up = up.normalize();
    VECTOR3 forward (view(0, 2), view(1, 2), view(2, 2));
    forward = forward.normalize();
    for (UINT i = 0; i < 4; i++) {
        healthBar[i].Color = 0xff00ff00;
        healthBar[i+4].Color = 0xffff0000;
    }
    VECTOR3 position = _Enemy->Position;
    float min[3], max[3];
    m_Ms3dLoader->GetModel(_Enemy->Id)->GetBounds(min, max);
    position[1] = max[1];
    float halfWidth = 8.0f;
    float height = 2.0f;
    float health = (_Enemy->HitPoints / (float)_Enemy->MaxHitPoints - 0.5f) * halfWidth * 2.0f;
    VECTOR3 location = position + (-halfWidth * right) + 0.0f * up + forward;
    healthBar[0].X = location[0]; healthBar[0].Y = location[1]; healthBar[0].Z = location[2];
    location = position + (-halfWidth * right) + height * up + forward;
    healthBar[1].X = location[0]; healthBar[1].Y = location[1]; healthBar[1].Z = location[2];
    location = position + health * right + 0.0f * up + forward;
    healthBar[2].X = location[0]; healthBar[2].Y = location[1]; healthBar[2].Z = location[2];
    healthBar[4].X = location[0]; healthBar[4].Y = location[1]; healthBar[4].Z = location[2];
    location = position + health * right + height * up + forward;
    healthBar[3].X = location[0]; healthBar[3].Y = location[1]; healthBar[3].Z = location[2];
    healthBar[5].X = location[0]; healthBar[5].Y = location[1]; healthBar[5].Z = location[2];
    location = position + halfWidth * right + 0.0f * up + forward;
    healthBar[6].X = location[0]; healthBar[6].Y = location[1]; healthBar[6].Z = location[2];
    location = position + halfWidth * right + height * up + forward;
    healthBar[7].X = location[0]; healthBar[7].Y = location[1]; healthBar[7].Z = location[2];
    index[0] = 0; index[1] = 1; index[2] = 3;
    index[3] = 0; index[4] = 3; index[5] = 2;
    index[6] = 4; index[7] = 5; index[8] = 7;
    index[9] = 4; index[10] = 7; index[11] = 6;
    m_Device->GetVCacheManager()->Render(PT_TRIANGLELIST, healthBar, 8, index, 12, VFT_ULC, INVALID_ID);
}

bool Game::IsEnemyVisible (float _frustum[6][4], const std::list<EnemyInfo>::iterator _Enemy) {
    float min[3], max[3];
    m_Ms3dLoader->GetModel(_Enemy->Id)->GetBounds(min, max);
    for(UINT i = 0; i < 6; i++) {
        if(_frustum[i][0] * min[0] + _frustum[i][1] * min[1] + _frustum[i][2] * min[2] + _frustum[i][3] > 0)
            continue;
        if(_frustum[i][0] * max[0] + _frustum[i][1] * min[1] + _frustum[i][2] * min[2] + _frustum[i][3] > 0)
            continue;
        if(_frustum[i][0] * min[0] + _frustum[i][1] * max[1] + _frustum[i][2] * min[2] + _frustum[i][3] > 0)
            continue;
        if(_frustum[i][0] * max[0] + _frustum[i][1] * max[1] + _frustum[i][2] * min[2] + _frustum[i][3] > 0)
            continue;
        if(_frustum[i][0] * min[0] + _frustum[i][1] * min[1] + _frustum[i][2] * max[2] + _frustum[i][3] > 0)
            continue;
        if(_frustum[i][0] * max[0] + _frustum[i][1] * min[1] + _frustum[i][2] * max[2] + _frustum[i][3] > 0)
            continue;
        if(_frustum[i][0] * min[0] + _frustum[i][1] * max[1] + _frustum[i][2] * max[2] + _frustum[i][3] > 0)
            continue;
        if(_frustum[i][0] * max[0] + _frustum[i][1] * max[1] + _frustum[i][2] * max[2] + _frustum[i][3] > 0)
            continue;
        return false;
    }
    return true;
}