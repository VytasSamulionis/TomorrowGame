#include "../include/Ms3dModel.h"

using namespace ms3d;

MESH::MESH () {
    TriIndex = NULL;
}

MESH::~MESH () {
    delete[] TriIndex;
}

JOINT::JOINT () {
    memset (&Absolute, 0, sizeof (D3DXMATRIX));
    Vertex = NULL;
    Transformed = NULL;
    Normal = NULL;
    RotatedNormal = NULL;
    NumVertices = 0;
    RotKeyFrame = NULL;
    TransKeyFrame = NULL;
}

JOINT::~JOINT () {
    delete[] Vertex;
    Vertex = NULL;
    delete[] Transformed;
    Transformed = NULL;
    delete[] Normal;
    Normal = NULL;
    delete[] RotatedNormal;
    RotatedNormal = NULL;
    delete[] RotKeyFrame;
    RotKeyFrame = NULL;
    delete[] TransKeyFrame;
    TransKeyFrame = NULL;
}

Ms3dModel::Ms3dModel () {
    m_Filename[0] = '\0';
    m_StartTime = -1.0f;
    m_EndTime = -1.0f;
    m_IsAnimationCompleted = false;
    m_Vertex = NULL;
    m_Transformed = NULL;
    m_Normal = NULL;
    m_RotatedNormal = NULL;
    m_NumVertices = 0;
    m_Triangle = NULL;
    m_NumTriangles = 0;
    m_Mesh = NULL;
    m_NumMeshes = 0;
    m_Material = NULL;
    m_NumMaterials = 0;
    m_Joint = NULL;
    m_NumJoints = 0;
    m_Log = NULL;
    m_IsEmpty = true;
    m_VertexData = NULL;
    ClearTransformations ();
}

Ms3dModel::Ms3dModel (LogManager* _log) {
    m_Filename[0] = '\0';
    m_StartTime = -1.0f;
    m_EndTime = -1.0f;
    m_IsAnimationCompleted = false;
    m_Vertex = NULL;
    m_Transformed = NULL;
    m_Normal = NULL;
    m_RotatedNormal = NULL;
    m_NumVertices = 0;
    m_Triangle = NULL;
    m_NumTriangles = 0;
    m_Mesh = NULL;
    m_NumMeshes = 0;
    m_Material = NULL;
    m_NumMaterials = 0;
    m_Joint = NULL;
    m_NumJoints = 0;
    m_Log = _log;
    m_IsEmpty = true;
    m_VertexData = NULL;
    ClearTransformations ();
}

Ms3dModel::~Ms3dModel () {
    Unload ();
}

void Ms3dModel::Unload () {
    ClearTransformations ();
    m_SkinId.clear();
    delete[] m_Vertex;
    m_Vertex = NULL;
    delete[] m_Transformed;
    m_Transformed = NULL;
    delete[] m_Normal;
    m_Normal = NULL;
    delete[] m_RotatedNormal;
    m_RotatedNormal = NULL;
    m_NumVertices = 0;
    delete[] m_Triangle;
    m_Triangle = NULL;
    m_NumTriangles = 0;
    delete[] m_Mesh;
    m_Mesh = NULL;
    m_NumMeshes = 0;
    delete[] m_Material;
    m_Material = NULL;
    m_NumMaterials = 0;
    delete[] m_Joint;
    m_Joint = NULL;
    m_NumJoints = 0;
    delete[] m_VertexData;
    m_VertexData = NULL;
}

void Ms3dModel::SetLog (LogManager* _Log) {
    m_Log = _Log;
}

void Ms3dModel::LoadVertices (char*& _modelData, VERTEX*& _vertex, USHORT& _numVertices) {
    memcpy (&_numVertices, _modelData, sizeof (USHORT));
    _modelData += 2;
    try {
        _vertex = new VERTEX[_numVertices];
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of memory. (Ms3dModel::LoadVertices)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    memcpy (_vertex, _modelData, sizeof (VERTEX) * _numVertices);
    _modelData += sizeof (VERTEX) * _numVertices;
}

VERTEX* Ms3dModel::GetVertices (short _jointId) {
    if (_jointId == -1) {
        return m_Vertex;
    }
    if (-1 > _jointId || _jointId >= m_NumJoints) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Joint[_jointId].Vertex;
}

void Ms3dModel::LoadTriangles (char*& _modelData) {
    memcpy (&m_NumTriangles, _modelData, sizeof (USHORT));
    _modelData += 2;
    UINT size = m_NumTriangles * sizeof (TRIANGLE);
    try {
        m_Triangle = new TRIANGLE[m_NumTriangles];
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of memory. (Ms3dLoader::LoadTriangles)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    for (UINT i = 0; i < m_NumTriangles; i++) {
        memcpy (&(m_Triangle[i].Flag), _modelData, sizeof (USHORT));
        _modelData += sizeof (USHORT);
        memcpy (&(m_Triangle[i].VertIndex[0]), _modelData, 68);
        _modelData += 68;
        //memcpy (m_Triangle, _modelData, size);
    }
    //_modelData += size;
}

TRIANGLE* Ms3dModel::GetTriangles () {
    return m_Triangle;
}

void Ms3dModel::LoadMeshes (char*& _modelData) {
    memcpy (&m_NumMeshes, _modelData, sizeof (USHORT));    
    _modelData += 2;
    try {
        m_Mesh = new MESH[m_NumMeshes];
        UINT size;
        UINT maxNumTriangles = 0;
        for (UINT i = 0; i < m_NumMeshes; i++) {
            memcpy (&m_Mesh[i], _modelData, 35);   // read the first 35 bytes of a m_Mesh
            _modelData += 35;
            size = m_Mesh[i].NumTriangles * sizeof (USHORT);
            m_Mesh[i].TriIndex = new USHORT[m_Mesh[i].NumTriangles];
            memcpy (m_Mesh[i].TriIndex, _modelData, size); // read triangle indices
            _modelData += size;
            memcpy (&m_Mesh[i].Material, _modelData, sizeof (char));   // read material index
            _modelData += 1;

            m_SkinId.push_back (INVALID_ID);
            if (m_Mesh[i].NumTriangles > maxNumTriangles) {
                maxNumTriangles = m_Mesh[i].NumTriangles;
            }
        }
        m_VertexData = new vs3d::UUVERTEX[maxNumTriangles * 3];
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of memory. (Ms3dLoader::LoadMeshes)\n");
        }
        #endif
        Unload ();
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

MESH* Ms3dModel::GetMeshes () {
    return m_Mesh;
}

void Ms3dModel::LoadMaterials (char*& _modelData) {
    memcpy (&m_NumMaterials, _modelData, sizeof (USHORT));
    _modelData += 2;
    UINT size = m_NumMaterials * sizeof (MATERIAL);
    try {
        m_Material = new MATERIAL[m_NumMaterials];
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of memory. (Ms3dLoader::LoadMaterials)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    memcpy (m_Material, _modelData, size);
    _modelData += size;
}

MATERIAL* Ms3dModel::GetMaterials () {
    return m_Material;
}

void Ms3dModel::LoadJoints (char*& _modelData) {
    memcpy (&m_NumJoints, _modelData, sizeof (USHORT));
    _modelData += 2;
    try {
        m_Joint = new JOINT[m_NumJoints];
        UINT size;
        for (UINT i = 0; i < m_NumJoints; i++) {
            memcpy (&m_Joint[i], _modelData, 93);
            _modelData += 93;
            size = m_Joint[i].NumRotFrames * sizeof (KEYFRAME);
            m_Joint[i].RotKeyFrame = new KEYFRAME[m_Joint[i].NumRotFrames];
            memcpy (m_Joint[i].RotKeyFrame, _modelData, size);
            _modelData += size;
            size = m_Joint[i].NumTransFrames * sizeof (KEYFRAME);
            m_Joint[i].TransKeyFrame = new KEYFRAME[m_Joint[i].NumTransFrames];
            memcpy (m_Joint[i].TransKeyFrame, _modelData, size);
            _modelData += size;
        }
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of memory. (Ms3dLoader::LoadJoints)\n");
        }
        #endif
        Unload ();
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

JOINT* Ms3dModel::GetJoints () {
    return m_Joint;
}

void Ms3dModel::Load (const char* _filename) {
    Unload ();
    FILE* modelFile = NULL;
    modelFile = fopen (_filename, "rb");
    if (!modelFile) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot open file %s. (Ms3dModel::Load)\n", _filename);
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_FILE_NOT_FOUND, _filename);
    }
    // copy file data to buffer
    if (fseek (modelFile, 0, SEEK_END)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot seek file %s (Ms3dModel::Load)\n", _filename);
        }
        #endif
        fclose (modelFile);
        THROW_DETAILED_ERROR (ERRC_BAD_FILE, _filename);
    }
    UINT fileSize = ftell (modelFile);
    if (fseek (modelFile, 0, SEEK_SET)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot seek file %s (Ms3dModel::Load)\n", _filename);
        }
        #endif
        fclose (modelFile);
        THROW_DETAILED_ERROR (ERRC_BAD_FILE, _filename);
    }
    char* model = (char*) malloc (fileSize);
    char* originModelAdr = model;   // origin model address
    if (!model) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of memory. (Ms3dLoader::Load)\n");
        }
        #endif
        fclose (modelFile);
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    if (fread (model, 1, fileSize, modelFile) != fileSize) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Cannot read file (%s). (Ms3dModel::Load)\n", _filename);
        }
        #endif
        free (model);
        fclose (modelFile);
        THROW_DETAILED_ERROR (ERRC_BAD_FILE, _filename);
    }
    fclose (modelFile);
    // check id string
    char idString[11];
    memcpy (idString, model, 10);   // 10 bytes id string
    idString[10] = 0;
    if (strcmp (idString, "MS3D000000") != 0) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: ID string (%s) does not match (%s). (Ms3dModel::Load)\n", idString, _filename);
        }
        #endif
        free (originModelAdr);
        THROW_DETAILED_ERROR (ERRC_BAD_FILE, _filename);
    }
    // check version
    model += 10;    // seek to version
    int version;
    memcpy (&version, model, 4);
    if (version != 3 && version != 4) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Wrong file version (%d) (%s) (Ms3dModel::Load)\n", version, _filename);
        }
        #endif
        free (originModelAdr);
        THROW_DETAILED_ERROR (ERRC_BAD_FILE, _filename);
    }
    model += 4;
    // check if memory allocation is required    
    SetFilename (_filename);
    VERTEX* vertex = NULL;
    USHORT numVertices = 0;
    try {
        LoadVertices (model, vertex, numVertices);
        LoadTriangles (model);
        LoadMeshes (model);
        LoadMaterials (model);
        // skip unnecessary information
        model += 12;
        LoadJoints (model);
    } catch (ErrorMessage&) {
        Unload ();
        delete[] vertex;
        free (originModelAdr);
        throw;
    }

    // additional setups
    USHORT* vertexIndex = NULL;
    try {
        // Count the vertices by joints their belong
        vertexIndex = new USHORT[numVertices];
        for (UINT i = 0; i < numVertices; i++) {
            if (vertex[i].BoneId == -1) {
                m_NumVertices++;
            } else {
                m_Joint[vertex[i].BoneId].NumVertices++;
            }
        }
        if (m_NumVertices > 0) {
            m_Vertex = new VERTEX[m_NumVertices];
            m_Transformed = new VERTEX[m_NumVertices];
            m_Normal = new D3DXVECTOR3[m_NumVertices];
            m_RotatedNormal = new D3DXVECTOR3[m_NumVertices];
            m_NumVertices = 0;
        }

        // find parents
        for (UINT i = 0; i < m_NumJoints; i++) {
            if (m_Joint[i].NumVertices > 0) {
                m_Joint[i].Vertex = new VERTEX[m_Joint[i].NumVertices];
                m_Joint[i].Transformed = new VERTEX[m_Joint[i].NumVertices];
                m_Joint[i].Normal = new D3DXVECTOR3[m_Joint[i].NumVertices];
                m_Joint[i].RotatedNormal = new D3DXVECTOR3[m_Joint[i].NumVertices];
                m_Joint[i].NumVertices = 0;
            }
            if (m_Joint[i].Parent[0] != '\0') {
                for (UINT j = 0; j < m_NumJoints; j++) {
                    if (strcmp (m_Joint[i].Parent, m_Joint[j].Name) == 0) {
                        m_Joint[i].ParentId = j;
                        break;
                    }
                }
            } else {
                m_Joint[i].ParentId = -1;
            }
        }

        // create matrices
        for (UINT i = 0; i < m_NumJoints; i++) {
            m_Joint[i].Local = CreateRotationMatrix (m_Joint[i].Rotation);
            m_Joint[i].Local._41 = m_Joint[i].Position[0];
            m_Joint[i].Local._42 = m_Joint[i].Position[1];
            m_Joint[i].Local._43 = m_Joint[i].Position[2];
            if (m_Joint[i].ParentId != -1) {
                UINT pid = m_Joint[i].ParentId;
                // why not absolute * local?
                m_Joint[i].Absolute = m_Joint[i].Local * m_Joint[pid].Absolute;
            } else {
                m_Joint[i].Absolute = m_Joint[i].Local;
            }
            m_Joint[i].Final = m_Joint[i].Absolute;
        }

        // transform vertices
        UINT bid;   // bone id
        for (UINT i = 0; i < numVertices; i++) {
            if (vertex[i].BoneId != -1) {
                bid = vertex[i].BoneId;
                InvTranslate (vertex[i].Vertex, m_Joint[bid].Final); 
                InvRotate (vertex[i].Vertex, m_Joint[bid].Final);
                vertexIndex[i] = m_Joint[bid].NumVertices;
                m_Joint[bid].Vertex[m_Joint[bid].NumVertices++] = vertex[i];
            } else {
                vertexIndex[i] = m_NumVertices;
                m_Vertex[m_NumVertices++] = vertex[i];
            }
        }

        // transform normals
        for (UINT i = 0; i < m_NumTriangles; i++) {
            for (UINT j = 0; j < 3; j++) {  // loop through each index
                VERTEX* vert = &vertex[m_Triangle[i].VertIndex[j]];
                m_Triangle[i].JointIndex[j] = vert->BoneId;
                m_Triangle[i].VertIndex[j] = vertexIndex[m_Triangle[i].VertIndex[j]];
                if (vert->BoneId != -1) {
                    InvRotate (m_Triangle[i].Normal[j], m_Joint[vert->BoneId].Final);
                    m_Joint[vert->BoneId].Normal[m_Triangle[i].VertIndex[j]] = m_Triangle[i].Normal[j];
                } else {
                    m_Normal[m_Triangle[i].VertIndex[j]] = m_Triangle[i].Normal[j];
                }
            }
        }
    } catch (std::bad_alloc) {
        delete[] vertexIndex;
        delete[] vertex;
        free (originModelAdr);
        Unload ();
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    delete[] vertexIndex;
    delete[] vertex;
    free (originModelAdr);
    UpdateBounds ();
}

void Ms3dModel::SetFilename (const char* _filename) {
    strcpy (m_Filename, _filename);
}

const char* Ms3dModel::GetFilename () const {
    return m_Filename;
}

bool Ms3dModel::IsEmpty () const {
    return m_IsEmpty;
}

inline bool Ms3dModel::IsLoaded () const {
    if (m_Filename[0] == '\0' || !m_Triangle ||
        !m_Mesh || !m_Material || !m_Joint) {

        return false;
    }
    return true;
}

bool Ms3dModel::IsAnimationCompleted () const {
    return m_IsAnimationCompleted;
}

void Ms3dModel::Animate (float _speed, float _startTime, float _endTime, bool _loop) {
    if (!IsLoaded ()) {
        return;
    }
    if (m_StartTime != _startTime || m_EndTime != _endTime) {
        m_TimerLastClock = timeGetTime ();
        m_LastTime = _startTime;
        m_StartTime = _startTime;
        m_EndTime = _endTime;
        m_IsAnimationCompleted = false;
    }
    DWORD timerCurClock = timeGetTime ();

    float time = (timerCurClock - m_TimerLastClock) * 0.001f * _speed;
    m_TimerLastClock = timerCurClock;

    time += m_LastTime;
    m_LastTime = time;

    if (time > _endTime) {
        if (_loop) {
            m_LastTime = _startTime;
            time = _startTime;
        } else {
            time = _endTime;
            m_IsAnimationCompleted = true;
        }
    }
    for (UINT i = 0; i < m_NumJoints; i++) {
        D3DXMATRIX tempMat;
        UINT frame = 0;
        if (m_Joint[i].NumRotFrames == 0 && m_Joint[i].NumTransFrames == 0) {
            m_Joint[i].Final = m_Joint[i].Absolute;
            continue;
        }
        while (frame < m_Joint[i].NumTransFrames && m_Joint[i].TransKeyFrame[frame].Time < time) {
            frame++;
        }
        m_Joint[i].CurTransFrame = frame;

        float translation[3];
        float deltaT = 1.0f;
        float interp = 0.0f;

        if (frame == 0) {
            memcpy (translation, m_Joint[i].TransKeyFrame[0].Param, sizeof (float[3]));
        } else if (frame == m_Joint[i].NumTransFrames) {
            memcpy (translation, m_Joint[i].TransKeyFrame[frame - 1].Param, sizeof (float[3]));
        } else {
            KEYFRAME* curKF = &m_Joint[i].TransKeyFrame[frame];
            KEYFRAME* prevKF = &m_Joint[i].TransKeyFrame[frame-1];

            deltaT = curKF->Time - prevKF->Time;
            interp = (time - prevKF->Time) / deltaT;

            translation[0] = prevKF->Param[0] + (curKF->Param[0] - prevKF->Param[0]) * interp;
            translation[1] = prevKF->Param[1] + (curKF->Param[1] - prevKF->Param[1]) * interp;
            translation[2] = prevKF->Param[2] + (curKF->Param[2] - prevKF->Param[2]) * interp;
        }
        frame = 0;
        while (frame < m_Joint[i].NumRotFrames && m_Joint[i].RotKeyFrame[frame].Time < time) {
            frame++;
        }
        if (frame == 0) {
            tempMat = CreateRotationMatrix (m_Joint[i].RotKeyFrame[0].Param);
        } else if (frame == m_Joint[i].NumRotFrames) {
            tempMat = CreateRotationMatrix (m_Joint[i].RotKeyFrame[frame - 1].Param);
        } else {
            KEYFRAME* curKF = &m_Joint[i].RotKeyFrame[frame];
            KEYFRAME* prevKF = &m_Joint[i].RotKeyFrame[frame-1];

            deltaT = curKF->Time - prevKF->Time;
            interp = (time - prevKF->Time) / deltaT;

            D3DXQUATERNION quatCur = CreateQuaternion (curKF->Param[0], curKF->Param[1], curKF->Param[2]);
            D3DXQUATERNION quatPrev = CreateQuaternion (prevKF->Param[0], prevKF->Param[1], prevKF->Param[2]);
            D3DXQUATERNION final;
            D3DXQuaternionSlerp (&final, &quatPrev, &quatCur, interp);

            D3DXMatrixRotationQuaternion (&tempMat, &final);
        }
        tempMat._41 = translation[0];
        tempMat._42 = translation[1];
        tempMat._43 = translation[2];

        D3DXMATRIX finalMat = tempMat * m_Joint[i].Local;
        if (m_Joint[i].ParentId == -1) {
            m_Joint[i].Final = finalMat;
        } else {
            m_Joint[i].Final = finalMat * m_Joint[m_Joint[i].ParentId].Final;
        }
    }
}

void Ms3dModel::Render (RenderDevice* _device) {
    ISkinManager* skin = _device->GetSkinManager ();
    IVertexCacheManager* vcache = _device->GetVCacheManager ();
    MATRIX44 transform = m_Scale * m_Rotation * m_Translation;
    for (UINT i = 0; i < m_NumJoints; i++) {
        if (m_Joint[i].NumVertices > 0) {
            D3DXVec3TransformCoordArray (
                &(m_Joint[i].Transformed[0].Vertex), 
                sizeof (VERTEX), 
                &(m_Joint[i].Vertex[0].Vertex), 
                sizeof (VERTEX), 
                &(m_Joint[i].Final * *(D3DXMATRIX*)transform.data()), 
                m_Joint[i].NumVertices);
            D3DXVec3TransformNormalArray (
                m_Joint[i].RotatedNormal, 
                sizeof (D3DXVECTOR3), 
                m_Joint[i].Normal, 
                sizeof (D3DXVECTOR3), 
                &(m_Joint[i].Final * *(D3DXMATRIX*)transform.data()), 
                m_Joint[i].NumVertices);
        }
    }
    if (m_NumVertices > 0) {
        D3DXVec3TransformCoordArray (
            &(m_Transformed[0].Vertex), 
            sizeof (VERTEX), 
            &(m_Vertex[0].Vertex), 
            sizeof (VERTEX), 
            (D3DXMATRIX*)transform.data(), 
            m_NumVertices);
        D3DXVec3TransformNormalArray (
            m_RotatedNormal, 
            sizeof (D3DXVECTOR3), 
            m_Normal, 
            sizeof (D3DXVECTOR3), 
            (D3DXMATRIX*)transform.data(), 
            m_NumVertices);
    }

    for (UINT i = 0; i < m_NumMeshes; i++) {
        if (m_Mesh[i].Material >= 0) {
            if (m_SkinId[i] == INVALID_ID) {
                vs3d::MATERIAL mat;
                UINT size = sizeof (float) * 4;
                ms3d::MATERIAL* curMat = &m_Material[m_Mesh[i].Material];
                memcpy (&mat.Diffuse, &curMat->Diffuse, size);
                memcpy (&mat.Ambient, &curMat->Ambient, size);
                memcpy (&mat.Specular, &curMat->Specular, size);
                memcpy (&mat.Emissive, &curMat->Emissive, size);
                mat.Power = curMat->Shininess;
                mat.Diffuse.a = curMat->Transparency;
                m_SkinId[i] = skin->AddSkin (curMat->Texture, mat);
            }
        }
        
        UINT vertNum = 0;

        for (UINT j = 0; j < m_Mesh[i].NumTriangles; j++) {
            
            TRIANGLE* tri = &m_Triangle[m_Mesh[i].TriIndex[j]];

            for (UINT k = 0; k < 3; k++) {
                VERTEX* vert = NULL;
                D3DXVECTOR3* normal = NULL;
                if (tri->JointIndex[k] == -1) {
                    vert = &m_Transformed[tri->VertIndex[k]];
                    normal = &m_RotatedNormal[tri->VertIndex[k]];
                } else {
                    vert = &m_Joint[tri->JointIndex[k]].Transformed[tri->VertIndex[k]];
                    normal = &m_Joint[tri->JointIndex[k]].RotatedNormal[tri->VertIndex[k]];
                }

                m_VertexData[vertNum].Tu = tri->TexCoord[0][k];
                m_VertexData[vertNum].Tv = tri->TexCoord[1][k];
                /*if (vert->BoneId == -1) {
                    D3DXVec3TransformCoord (&vecVertex, &vert->Vertex, (D3DXMATRIX*)transform.data());
                    renderVertex.x = vecVertex.x;
                    renderVertex.y = vecVertex.y;
                    renderVertex.z = vecVertex.z;
                    D3DXVec3TransformNormal (&vecNormal, &tri->Normal[k], (D3DXMATRIX*)m_Rotation.data());
                    renderVertex.Normal[0] = vecNormal.x;
                    renderVertex.Normal[1] = vecNormal.y;
                    renderVertex.Normal[2] = vecNormal.z;
                } else {
                    JOINT* curJoint = &m_Joint[vert->BoneId];
                    D3DXMATRIX finalTransformation = curJoint->Final * *(D3DXMATRIX*)m_Rotation.data();
                    D3DXVec3TransformNormal (&vecNormal, &tri->Normal[k], &finalTransformation);
                    renderVertex.Normal[0] = vecNormal.x;
                    renderVertex.Normal[1] = vecNormal.y;
                    renderVertex.Normal[2] = vecNormal.z;
                    finalTransformation = curJoint->Final * *(D3DXMATRIX*)transform.data();
                    D3DXVec3TransformCoord (&vecVertex, &vert->Vertex, &finalTransformation);
                    renderVertex.x = vecVertex.x;
                    renderVertex.y = vecVertex.y;
                    renderVertex.z = vecVertex.z;
                }*/
                m_VertexData[vertNum].X = vert->Vertex.x;
                m_VertexData[vertNum].Y = vert->Vertex.y;
                m_VertexData[vertNum].Z = vert->Vertex.z;
                m_VertexData[vertNum].Normal[0] = normal->x;
                m_VertexData[vertNum].Normal[1] = normal->y;
                m_VertexData[vertNum].Normal[2] = normal->z;
                vertNum++;
            }
        }
        //vcache->Render (PT_TRIANGLELIST, vertNum, renderVertexArray, 0, 0, FVF_UU, skinId);
        vcache->Render (PT_TRIANGLELIST, m_VertexData, vertNum, NULL, 0, VFT_UU, m_SkinId[i]);
        //m_Log->Log ("%u\n", m_SkinId[i]);
    }
}

inline void Ms3dModel::UpdateEmptyStatus () {
    if (m_Filename[0] != '\0' || m_Vertex || m_Triangle ||
        m_Mesh || m_Material || m_Joint) {
        
        m_IsEmpty = false;
    }
    m_IsEmpty = true;
}

D3DXMATRIX Ms3dModel::CreateRotationMatrix (float _vector[3]) const {
	float	sx, sy, sz, cx, cy, cz;
	D3DXMATRIX matRet;

	D3DXMatrixIdentity (&matRet);

	sz = (float)sin (_vector[2]);
	cz = (float)cos (_vector[2]);
	sy = (float)sin (_vector[1]);
	cy = (float)cos (_vector[1]);
	sx = (float)sin (_vector[0]);
	cx = (float)cos (_vector[0]);

	matRet._11	= cy*cz;
	matRet._12 = cy*sz;
	matRet._13 = -sy;
	matRet._21 = sx*sy*cz+cx*-sz;
	matRet._22 = sx*sy*sz+cx*cz;
	matRet._23 = sx*cy;
	matRet._31 = (cx*sy*cz+-sx*-sz);
	matRet._32 = (cx*sy*sz+-sx*cz);
	matRet._33 = cx*cy;
	matRet._41 = 0.0;
	matRet._42 = 0.0;
	matRet._43 = 0.0;
	
	return matRet;
}

D3DXQUATERNION Ms3dModel::CreateQuaternion (float _x, float _y, float _z) const {
    float dSY = sin(_z * 0.5f);
	float dSP = sin(_y * 0.5f);
	float dSR = sin(_x * 0.5f);
	float dCY = cos(_z * 0.5f);
	float dCP = cos(_y * 0.5f);
	float dCR = cos(_x * 0.5f);
    float* quat = new float[4];

	quat[0] = dSR * dCP * dCY - dCR * dSP * dSY;
	quat[1] = dCR * dSP * dCY + dSR * dCP * dSY;
	quat[2] = dCR * dCP * dSY - dSR * dSP * dCY;
	quat[3] = dCR * dCP * dCY + dSR * dSP * dSY;

    D3DXQUATERNION retQuat (quat);

    delete quat;
    return retQuat;
}

void Ms3dModel::InvRotate (D3DXVECTOR3& _vector, const D3DXMATRIX& _matrix) const {
    float tmp[3];
    tmp[0] = _vector.x * _matrix._11 + _vector.y * _matrix._12 + _vector.z * _matrix._13;
	tmp[1] = _vector.x * _matrix._21 + _vector.y * _matrix._22 + _vector.z * _matrix._23;
	tmp[2] = _vector.x * _matrix._31 + _vector.y * _matrix._32 + _vector.z * _matrix._33;
    _vector.x = tmp[0];
    _vector.y = tmp[1];
    _vector.z = tmp[2];
}

void Ms3dModel::InvTranslate (D3DXVECTOR3& _vector, const D3DXMATRIX& _matrix) const {
    _vector.x -= _matrix._41;
	_vector.y -= _matrix._42;
	_vector.z -= _matrix._43;
}

void Ms3dModel::Scale(float _x, float _y, float _z) {
    MATRIX44 scale;
    cml::matrix_scale (scale, _x, _y, _z);
    m_Scale *= scale;
    UpdateBounds ();
}

void Ms3dModel::Translate(float _x, float _y, float _z) {
    MATRIX44 translation;
    cml::matrix_translation (translation, _x, _y, _z);
    m_Translation *= translation;
    UpdateBounds ();
}

void Ms3dModel::Rotate(float _x, float _y, float _z) {
    MATRIX44 rotation;
    cml::matrix_rotation_euler (rotation, _x, _y, _z, cml::euler_order_xyz);
    m_Rotation *= rotation;
    UpdateBounds ();
}

void Ms3dModel::ClearTransformations() {
    m_Scale.identity();
    m_Rotation.identity();
    m_Translation.identity();
    if (m_Vertex) {
        UpdateBounds ();
    }
}

void Ms3dModel::GetBounds (float(&_min)[3], float(&_max)[3]) {
    _min[0] = m_Min[0]; _min[1] = m_Min[1]; _min[2] = m_Min[2];
    _max[0] = m_Max[0]; _max[1] = m_Max[1]; _max[2] = m_Max[2];
}

void Ms3dModel::UpdateBounds () {
    MATRIX44 transform = m_Scale * m_Rotation * m_Translation;
    m_Min[0] = 99999.0f;
    m_Min[1] = 99999.0f;
    m_Min[2] = 99999.0f;
    m_Max[0] = -99999.0f;
    m_Max[1] = -99999.0f;
    m_Max[2] = -99999.0f;
    for (UINT i = 0; i < m_NumJoints; i++) {
        if (m_Joint[i].NumVertices > 0) {
            D3DXVec3TransformCoordArray (
                &(m_Joint[i].Transformed[0].Vertex), 
                sizeof (VERTEX), 
                &(m_Joint[i].Vertex[0].Vertex), 
                sizeof (VERTEX), 
                &(m_Joint[i].Final * *(D3DXMATRIX*)transform.data()), 
                m_Joint[i].NumVertices);
        }
    }
    if (m_NumVertices > 0) {
        D3DXVec3TransformCoordArray (
            &(m_Transformed[0].Vertex), 
            sizeof (VERTEX), 
            &(m_Vertex[0].Vertex), 
            sizeof (VERTEX), 
            (D3DXMATRIX*)transform.data(), 
            m_NumVertices);
    }
    for (UINT i = 0; i < m_NumJoints; i++) {
        for (UINT j = 0; j < m_Joint[i].NumVertices; j++) {
            if (m_Joint[i].Transformed[j].Vertex.x < m_Min[0]) {
                m_Min[0] = m_Joint[i].Transformed[j].Vertex.x;
            }
            if (m_Joint[i].Transformed[j].Vertex.y < m_Min[1]) {
                m_Min[1] = m_Joint[i].Transformed[j].Vertex.y;
            }
            if (m_Joint[i].Transformed[j].Vertex.z < m_Min[2]) {
                m_Min[2] = m_Joint[i].Transformed[j].Vertex.z;
            }
            if (m_Joint[i].Transformed[j].Vertex.x > m_Max[0]) {
                m_Max[0] = m_Joint[i].Transformed[j].Vertex.x;
            }
            if (m_Joint[i].Transformed[j].Vertex.y > m_Max[1]) {
                m_Max[1] = m_Joint[i].Transformed[j].Vertex.y;
            }
            if (m_Joint[i].Transformed[j].Vertex.z > m_Max[2]) {
                m_Max[2] = m_Joint[i].Transformed[j].Vertex.z;
            }
        }
    }
    for (UINT i = 0; i < m_NumVertices; i++) {
        if (m_Transformed[i].Vertex.x < m_Min[0]) {
            m_Min[0] = m_Transformed[i].Vertex.x;
        }
        if (m_Transformed[i].Vertex.y < m_Min[1]) {
            m_Min[1] = m_Transformed[i].Vertex.y;
        }
        if (m_Transformed[i].Vertex.z < m_Min[2]) {
            m_Min[2] = m_Transformed[i].Vertex.z;
        }
        if (m_Transformed[i].Vertex.x > m_Max[0]) {
            m_Max[0] = m_Transformed[i].Vertex.x;
        }
        if (m_Transformed[i].Vertex.y > m_Max[1]) {
            m_Max[1] = m_Transformed[i].Vertex.y;
        }
        if (m_Transformed[i].Vertex.z > m_Max[2]) {
            m_Max[2] = m_Transformed[i].Vertex.z;
        }
    }
}