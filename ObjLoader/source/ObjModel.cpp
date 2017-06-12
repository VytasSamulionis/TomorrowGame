#include "..\include\ObjModel.h"
#include <cstdio>

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

ObjModel::ObjModel (RenderDevice* _device, ObjManager* _manager):
    m_Device (_device),
    m_Manager (_manager),
    m_SkinId (INVALID_ID),
    m_OriginalSize (1.0f),
    m_IsTexture (false),
    m_IsNormal (false),
    m_BoundsColor (0xff000000),
    m_ModelColor (0xffffffff) {
        m_Filename[0] = '\0';
        m_MtlFilename[0] = '\0';
        m_IsOutdated = true;
        m_Scale.identity();
        m_Translation.identity();
        m_Rotation.identity();
}

ObjModel::~ObjModel () {
    Unload ();
    /*#ifdef _DEBUG
        _CrtDumpMemoryLeaks();
    #endif*/
}

void ObjModel::Unload () {
    m_Filename[0] = '\0';
    m_MtlFilename[0] = '\0';
    m_IsTexture = m_IsNormal = false;
    m_SkinId = INVALID_ID;
    m_OriginalSize = 1.0f;
    m_BoundsColor = 0xff000000;
    m_ModelColor = 0xffffffff;
    m_Vertices.clear();
    m_Textures.clear();
    m_Normals.clear();
    for (UINT i = 0; i < m_Meshes.size(); i++) {
        for (UINT j = 0; j < m_Meshes[i].Objects.size(); j++) {
            delete[] m_Meshes[i].Objects[j].ModelData;
        }
    }
    m_Meshes.clear();
    m_IsOutdated = true;
}

UINT ObjModel::MakeCopy () {
    ObjModel* modelCopy = NULL;
    UINT id;
    try {
        modelCopy = new ObjModel(m_Device, m_Manager);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    try {
        strcpy (modelCopy->m_Filename, m_Filename);
        strcpy (modelCopy->m_MtlFilename, m_MtlFilename);
        modelCopy->m_IsMtlFile = m_IsMtlFile;
        modelCopy->m_SkinId = m_SkinId;
        modelCopy->m_OriginalSize = m_OriginalSize;
        for (UINT i = 0; i < m_Vertices.size(); i++) {
            ObjModelVertex vertex;
            vertex.X = m_Vertices[i].X;
            vertex.Y = m_Vertices[i].Y;
            vertex.Z = m_Vertices[i].Z;
            modelCopy->m_Vertices.push_back (vertex);
        }
        for (UINT i = 0; i < m_Textures.size(); i++) {
            ObjModelTexture texture;
            texture.U = m_Textures[i].U;
            texture.V = m_Textures[i].V;
            modelCopy->m_Textures.push_back (texture);
        }
        for (UINT i = 0; i < m_Normals.size(); i++) {
            ObjModelNormal normal;
            normal.X = m_Normals[i].X;
            normal.Y = m_Normals[i].Y;
            normal.Z = m_Normals[i].Z;
            modelCopy->m_Normals.push_back (normal);
        }
        for (UINT i = 0; i < m_Meshes.size(); i++) {
            ObjModelMesh mesh;
            strcpy (mesh.MaterialName, m_Meshes[i].MaterialName);
            mesh.SkinId = m_Meshes[i].SkinId;
            for (UINT j = 0; j < m_Meshes[i].Objects.size(); j++) {
                ObjModelObject object;
                for (UINT k = 0; k < m_Meshes[i].Objects[j].Faces.size(); k++) {
                    ObjModelFace face;
                    for (UINT l = 0; l < 3; l++) {
                        face.VertexIndex[l] = m_Meshes[i].Objects[j].Faces[k].VertexIndex[l];
                    }
                    for (UINT l = 0; l < m_Meshes[i].Objects[j].Faces[k].TextureIndices.size(); l++) {
                        face.TextureIndices.push_back (m_Meshes[i].Objects[j].Faces[k].TextureIndices[l]);
                    }
                    for (UINT l = 0; l < m_Meshes[i].Objects[j].Faces[k].NormalIndices.size(); l++) {
                        face.NormalIndices.push_back (m_Meshes[i].Objects[j].Faces[k].NormalIndices[l]);
                    }
                    object.Faces.push_back (face);
                }
                object.ModelData = NULL;
                object.NumVertices = 0;
                mesh.Objects.push_back (object);
            }
            modelCopy->m_Meshes.push_back (mesh);
        }
        modelCopy->m_ActiveMesh = m_ActiveMesh;
        modelCopy->m_ModelColor = m_ModelColor;
        modelCopy->m_BoundsColor = m_BoundsColor;
        for (UINT i = 0; i < 3; i++) {
            modelCopy->m_BoundMax[i] = m_BoundMax[i];
            modelCopy->m_BoundMin[i] = m_BoundMin[i];
        }
        modelCopy->m_FirstTimeBoundsUpdate = m_FirstTimeBoundsUpdate;
        modelCopy->m_IsTexture = m_IsTexture;
        modelCopy->m_IsNormal = m_IsNormal;
        modelCopy->m_BoundsInfo.RenderingInfo.IsPrepared = false;
        modelCopy->m_IsOutdated = true;
        id = m_Manager->AddModel (modelCopy);
    } catch (std::bad_alloc) {
        delete modelCopy;
        THROW_ERROR (ERRC_OUT_OF_MEM);
    } catch (ErrorMessage) {
        delete modelCopy;
        throw;
    }
    return id;
}

void ObjModel::PrepareModelRendering () {
    try {
        for (UINT i = 0; i < m_Meshes.size(); i++) {
            for (UINT j = 0; j < m_Meshes[i].Objects.size(); j++) {
                ObjModelObject& object = m_Meshes[i].Objects[j];
                if (!m_IsTexture && !m_IsNormal) {            
                    vs3d::ULCVERTEX* modelData = new vs3d::ULCVERTEX[object.Faces.size() * 3];
                    UINT numVertices = 0;
                    for (UINT k = 0; k < object.Faces.size(); k++) {
                        for (UINT l = 0; l < 3; l++) {
                            UINT index = object.Faces[k].VertexIndex[l];
                            //VECTOR3 vertex (m_Vertices[index].x, m_Vertices[index].y, m_Vertices[index].z);
                            //vertex = cml::transform_point (m_Scale * m_Rotation * m_Translation, vertex);
                            modelData[numVertices].Color = m_ModelColor;
                            modelData[numVertices].X = m_Vertices[index].X;
                            modelData[numVertices].Y = m_Vertices[index].Y;
                            modelData[numVertices++].Z = m_Vertices[index].Z;
                        }
                    }
                    D3DXVec3TransformCoordArray (
                        (D3DXVECTOR3*)&(modelData[0].X), 
                        sizeof (vs3d::ULCVERTEX), 
                        (D3DXVECTOR3*)&(modelData[0].X), 
                        sizeof (vs3d::ULCVERTEX),
                        (D3DXMATRIX*)(m_Scale * m_Rotation * m_Translation).data(),
                        numVertices);
                    UINT skinId;
                    if (m_IsMtlFile) {
                        skinId = m_Meshes[i].SkinId;
                    } else {
                        skinId = m_SkinId;
                    }
                    ObjRenderingInfo info;
                    try {
                        info.BufferId = m_Manager->AddModelToRenderingBuffer (modelData, numVertices, VFT_ULC, info.StartVertex);
                        info.PrimitiveCount = (UINT) (numVertices / 3);
                        info.VertexFormat = VFT_ULC;
                        info.SkinId = skinId;
                        info.IsPrepared = true;
                        delete[] modelData;
                    } catch (ErrorMessage) {
                        delete[] modelData;
                        info.IsPrepared = false;
                        throw;
                    }
                    m_RenderingInfo.push_back (info);
                } else if (m_IsTexture && !m_IsNormal) {
                    vs3d::ULVERTEX* modelData = new vs3d::ULVERTEX[object.Faces.size() * 3];
                    UINT numVertices = 0;
                    for (UINT k = 0; k < object.Faces.size(); k++) {
                        for (UINT l = 0; l < 3; l++) {
                            UINT index = object.Faces[k].VertexIndex[l];
                            /*VECTOR3 vertex (m_Vertices[index].x, m_Vertices[index].y, m_Vertices[index].z);
                            vertex = cml::transform_point (m_Scale * m_Rotation * m_Translation, vertex);*/
                            modelData[numVertices].Color = m_ModelColor;
                            modelData[numVertices].X = m_Vertices[index].X;
                            modelData[numVertices].Y = m_Vertices[index].Y;
                            modelData[numVertices].Z = m_Vertices[index].Z;
                            index = object.Faces[k].TextureIndices[l];
                            modelData[numVertices].Tu = m_Textures[index].U;
                            modelData[numVertices].Tv = m_Textures[index].V;
                            numVertices++;
                        }
                    }
                    D3DXVec3TransformCoordArray (
                        (D3DXVECTOR3*)&(modelData[0].X), 
                        sizeof (vs3d::ULVERTEX), 
                        (D3DXVECTOR3*)&(modelData[0].X), 
                        sizeof (vs3d::ULVERTEX),
                        (D3DXMATRIX*)(m_Scale * m_Rotation * m_Translation).data(),
                        numVertices);
                    UINT skinId;
                    if (m_IsMtlFile) {
                        skinId = m_Meshes[i].SkinId;
                    } else {
                        skinId = m_SkinId;
                    }
                    ObjRenderingInfo info;
                    try {
                        info.BufferId = m_Manager->AddModelToRenderingBuffer (modelData, numVertices, VFT_UL, info.StartVertex);
                        info.PrimitiveCount = (UINT) (numVertices / 3);
                        info.VertexFormat = VFT_UL;
                        info.SkinId = skinId;
                        info.IsPrepared = true;
                        delete[] modelData;
                    } catch (ErrorMessage) {
                        delete[] modelData;
                        info.IsPrepared = false;
                        throw;
                    }
                    m_RenderingInfo.push_back (info);
                } else if (!m_IsTexture && m_IsNormal) {
                    THROW_ERROR (ERRC_BAD_FILE);
                } else {
                    vs3d::UUVERTEX* modelData = new vs3d::UUVERTEX[object.Faces.size() * 3];
                    UINT numVertices = 0;
                    for (UINT k = 0; k < object.Faces.size(); k++) {
                        for (UINT l = 0; l < 3; l++) {
                            UINT index = object.Faces[k].VertexIndex[l];
                            /*VECTOR3 vertex (m_Vertices[index].x, m_Vertices[index].y, m_Vertices[index].z);
                            vertex = cml::transform_point (m_Scale * m_Rotation * m_Translation, vertex);*/
                            modelData[numVertices].X = m_Vertices[index].X;
                            modelData[numVertices].Y = m_Vertices[index].Y;
                            modelData[numVertices].Z = m_Vertices[index].Z;
                            index = object.Faces[k].TextureIndices[l];
                            modelData[numVertices].Tu = m_Textures[index].U;
                            modelData[numVertices].Tv = m_Textures[index].V;
                            index = object.Faces[k].NormalIndices[l];
                            /*VECTOR3 normal (m_Normals[index].x, m_Normals[index].y, m_Normals[index].z);
                            normal = cml::transform_vector (m_Rotation, normal);*/
                            modelData[numVertices].Normal[0] = m_Normals[index].X;
                            modelData[numVertices].Normal[1] = m_Normals[index].Y;
                            modelData[numVertices].Normal[2] = m_Normals[index].Z;
                            numVertices++;
                        }
                    }
                    D3DXVec3TransformCoordArray (
                        (D3DXVECTOR3*)&(modelData[0].X), 
                        sizeof (vs3d::UUVERTEX), 
                        (D3DXVECTOR3*)&(modelData[0].X), 
                        sizeof (vs3d::UUVERTEX),
                        (D3DXMATRIX*)(m_Scale * m_Rotation * m_Translation).data(),
                        numVertices);
                    D3DXVec3TransformNormalArray (
                        (D3DXVECTOR3*)&(modelData[0].Normal[0]),
                        sizeof (vs3d::UUVERTEX),
                        (D3DXVECTOR3*)&(modelData[0].Normal[0]),
                        sizeof (vs3d::UUVERTEX),
                        (D3DXMATRIX*)(m_Scale * m_Rotation * m_Translation).data(),
                        numVertices);
                    UINT skinId;
                    if (m_IsMtlFile) {
                        skinId = m_Meshes[i].SkinId;
                    } else {
                        skinId = m_SkinId;
                    }
                    ObjRenderingInfo info;
                    try {
                        info.BufferId = m_Manager->AddModelToRenderingBuffer (modelData, numVertices, VFT_UU, info.StartVertex);
                        info.PrimitiveCount = (UINT) (numVertices / 3);
                        info.VertexFormat = VFT_UU;
                        info.SkinId = skinId;
                        info.IsPrepared = true;
                        delete[] modelData;
                    } catch (ErrorMessage) {
                        info.IsPrepared = false;
                        delete[] modelData;
                        throw;
                    }
                    m_RenderingInfo.push_back (info);
                }
            }
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void ObjModel::PrepareBoundsRendering () {
    vs3d::ULCVERTEX vertex[8];
    WORD index[24];
    SetBoundsVertex (vertex[0], m_BoundMin[0], m_BoundMin[1], m_BoundMin[2]);
    SetBoundsVertex (vertex[1], m_BoundMax[0], m_BoundMin[1], m_BoundMin[2]);
    SetBoundsVertex (vertex[2], m_BoundMin[0], m_BoundMin[1], m_BoundMax[2]);
    SetBoundsVertex (vertex[3], m_BoundMax[0], m_BoundMin[1], m_BoundMax[2]);
    SetBoundsVertex (vertex[4], m_BoundMin[0], m_BoundMax[1], m_BoundMin[2]);
    SetBoundsVertex (vertex[5], m_BoundMax[0], m_BoundMax[1], m_BoundMin[2]);
    SetBoundsVertex (vertex[6], m_BoundMin[0], m_BoundMax[1], m_BoundMax[2]);
    SetBoundsVertex (vertex[7], m_BoundMax[0], m_BoundMax[1], m_BoundMax[2]);
    index[0] = 0;   index[1] = 1;  
    index[2] = 1;   index[3] = 3;
    index[4] = 3;   index[5] = 2;
    index[6] = 2;   index[7] = 0;
    index[8] = 4;   index[9] = 5;
    index[10] = 5;   index[11] = 7;
    index[12] = 7;   index[13] = 6;
    index[14] = 6;   index[15] = 4;
    index[16] = 0;   index[17] = 4;
    index[18] = 2;   index[19] = 6;
    index[20] = 1;   index[21] = 5;
    index[22] = 3;   index[23] = 7;
    //m_BoundsVertexId = m_Device->GetVCacheManager()->CreateStaticVertexBuffer (vertex, 8, FVF_ULC);
    //m_BoundsIndexId = m_Device->GetVCacheManager()->CreateStaticIndexBuffer (index, 24);
    /*if (m_BoundsVertexId == INVALID_ID || m_BoundsIndexId == INVALID_ID) {
        return VS3D_FAIL;
    }*/
    m_BoundsInfo.RenderingInfo.IsPrepared = false;
    m_BoundsInfo.StartIndex = m_Manager->GetIndexBufferNumIndices();
    m_BoundsInfo.IndexBufferId = m_Manager->AddToIndexBuffer (index, 24);
    m_BoundsInfo.RenderingInfo.BufferId = m_Manager->AddModelToRenderingBuffer (vertex, 8, VFT_ULC, m_BoundsInfo.RenderingInfo.StartVertex);
    m_BoundsInfo.RenderingInfo.PrimitiveCount = 12;
    m_BoundsInfo.RenderingInfo.VertexFormat = VFT_ULC;
    m_BoundsInfo.RenderingInfo.SkinId = INVALID_ID;
    m_BoundsInfo.RenderingInfo.IsPrepared = true;
}

void ObjModel::Prepare () {
    PrepareModelRendering ();
    PrepareBoundsRendering ();
}

void ObjModel::Render () {
    for (UINT i = 0; i < m_RenderingInfo.size(); i++) {
        if (m_RenderingInfo[i].IsPrepared) {
            m_Device->GetVCacheManager()->Render (
                    PT_TRIANGLELIST, 
                    m_RenderingInfo[i].BufferId, 
                    m_RenderingInfo[i].StartVertex, 
                    (WORD*)0, 0, 
                    m_RenderingInfo[i].PrimitiveCount,
                    m_RenderingInfo[i].VertexFormat, 
                    m_RenderingInfo[i].SkinId);
        }
    }
}

void ObjModel::RenderDynamic () {
    try {
        for (UINT i = 0; i < m_Meshes.size(); i++) {
            for (UINT j = 0; j < m_Meshes[i].Objects.size(); j++) {
                ObjModelObject& object = m_Meshes[i].Objects[j];
                if (!m_IsTexture && !m_IsNormal) {
                    if (m_IsOutdated) {
                        delete[] object.ModelData;
                        object.ModelData = new vs3d::ULCVERTEX[object.Faces.size() * 3];
                        vs3d::ULCVERTEX* modelData = (vs3d::ULCVERTEX*)object.ModelData;
                        object.NumVertices = 0;
                        for (UINT k = 0; k < object.Faces.size(); k++) {
                            for (UINT l = 0; l < 3; l++) {
                                UINT index = object.Faces[k].VertexIndex[l];
                                //VECTOR3 vertex (m_Vertices[index].x, m_Vertices[index].y, m_Vertices[index].z);
                                //vertex = cml::transform_point (m_Scale * m_Rotation * m_Translation, vertex);
                                modelData[object.NumVertices].Color = m_ModelColor;
                                modelData[object.NumVertices].X = m_Vertices[index].X;
                                modelData[object.NumVertices].Y = m_Vertices[index].Y;
                                modelData[object.NumVertices++].Z = m_Vertices[index].Z;
                            }
                        }
                        D3DXVec3TransformCoordArray (
                            (D3DXVECTOR3*)&(modelData[0].X), 
                            sizeof (vs3d::ULCVERTEX), 
                            (D3DXVECTOR3*)&(modelData[0].X), 
                            sizeof (vs3d::ULCVERTEX),
                            (D3DXMATRIX*)(m_Scale * m_Rotation * m_Translation).data(),
                            object.NumVertices);
                        m_IsOutdated = false;
                    }
                    UINT skinId;
                    if (m_IsMtlFile) {
                        skinId = m_Meshes[i].SkinId;
                    } else {
                        skinId = m_SkinId;
                    }
                    m_Device->GetVCacheManager()->Render (
                        PT_TRIANGLELIST, 
                        object.ModelData, object.NumVertices,
                        (WORD*)NULL, 0, 
                        VFT_ULC, 
                        skinId);
                    //delete[] modelData;
                } else if (m_IsTexture && !m_IsNormal) {
                    if (m_IsOutdated) {
                        delete[] object.ModelData;
                        object.ModelData = new vs3d::ULVERTEX[object.Faces.size() * 3];
                        vs3d::ULVERTEX* modelData = (vs3d::ULVERTEX*)object.ModelData;
                        object.NumVertices = 0;
                        for (UINT k = 0; k < object.Faces.size(); k++) {
                            for (UINT l = 0; l < 3; l++) {
                                UINT index = object.Faces[k].VertexIndex[l];
                                //VECTOR3 vertex (m_Vertices[index].x, m_Vertices[index].y, m_Vertices[index].z);
                                //vertex = cml::transform_point (m_Scale * m_Rotation * m_Translation, vertex);
                                modelData[object.NumVertices].Color = m_ModelColor;
                                modelData[object.NumVertices].X = m_Vertices[index].X;
                                modelData[object.NumVertices].Y = m_Vertices[index].Y;
                                modelData[object.NumVertices].Z = m_Vertices[index].Z;
                                index = object.Faces[k].TextureIndices[l];
                                modelData[object.NumVertices].Tu = m_Textures[index].U;
                                modelData[object.NumVertices].Tv = m_Textures[index].V;
                                object.NumVertices++;
                            }
                        }
                        D3DXVec3TransformCoordArray (
                            (D3DXVECTOR3*)&(modelData[0].X), 
                            sizeof (vs3d::ULVERTEX), 
                            (D3DXVECTOR3*)&(modelData[0].X), 
                            sizeof (vs3d::ULVERTEX),
                            (D3DXMATRIX*)(m_Scale * m_Rotation * m_Translation).data(),
                            object.NumVertices);
                        m_IsOutdated = false;
                    }
                    UINT skinId;
                    if (m_IsMtlFile) {
                        skinId = m_Meshes[i].SkinId;
                    } else {
                        skinId = m_SkinId;
                    }
                    m_Device->GetVCacheManager()->Render (
                        PT_TRIANGLELIST, 
                        object.ModelData, object.NumVertices,
                        (WORD*)0, 0, 
                        VFT_UL, 
                        skinId);
                } else if (!m_IsTexture && m_IsNormal) {
                    THROW_ERROR (ERRC_BAD_FILE);
                } else {
                    if (m_IsOutdated) {
                        delete[] object.ModelData;
                        object.ModelData = new vs3d::UUVERTEX[object.Faces.size() * 3];
                        vs3d::UUVERTEX* modelData = (vs3d::UUVERTEX*)object.ModelData;
                        object.NumVertices = 0;
                        for (UINT k = 0; k < object.Faces.size(); k++) {
                            for (UINT l = 0; l < 3; l++) {
                                UINT index = object.Faces[k].VertexIndex[l];
                                //VECTOR3 vertex (m_Vertices[index].x, m_Vertices[index].y, m_Vertices[index].z);
                                //vertex = cml::transform_point (m_Scale * m_Rotation * m_Translation, vertex);
                                modelData[object.NumVertices].X = m_Vertices[index].X;
                                modelData[object.NumVertices].Y = m_Vertices[index].Y;
                                modelData[object.NumVertices].Z = m_Vertices[index].Z;
                                index = object.Faces[k].TextureIndices[l];
                                modelData[object.NumVertices].Tu = m_Textures[index].U;
                                modelData[object.NumVertices].Tv = m_Textures[index].V;
                                index = object.Faces[k].NormalIndices[l];
                                //VECTOR3 normal (m_Normals[index].x, m_Normals[index].y, m_Normals[index].z);
                                //normal = cml::transform_vector (m_Rotation, normal);
                                modelData[object.NumVertices].Normal[0] = m_Normals[index].X;
                                modelData[object.NumVertices].Normal[1] = m_Normals[index].Y;
                                modelData[object.NumVertices].Normal[2] = m_Normals[index].Z;
                                object.NumVertices++;
                            }
                        }
                        D3DXVec3TransformCoordArray (
                            (D3DXVECTOR3*)&(modelData[0].X), 
                            sizeof (vs3d::UUVERTEX), 
                            (D3DXVECTOR3*)&(modelData[0].X), 
                            sizeof (vs3d::UUVERTEX),
                            (D3DXMATRIX*)(m_Scale * m_Rotation * m_Translation).data(),
                            object.NumVertices);
                        D3DXVec3TransformNormalArray (
                            (D3DXVECTOR3*)&(modelData[0].Normal[0]),
                            sizeof (vs3d::UUVERTEX),
                            (D3DXVECTOR3*)&(modelData[0].Normal[0]),
                            sizeof (vs3d::UUVERTEX),
                            (D3DXMATRIX*)(m_Scale * m_Rotation * m_Translation).data(),
                            object.NumVertices);
                        m_IsOutdated = false;
                    }
                    UINT skinId;
                    if (m_IsMtlFile) {
                        skinId = m_Meshes[i].SkinId;
                    } else {
                        skinId = m_SkinId;
                    }
                    m_Device->GetVCacheManager()->Render (
                        PT_TRIANGLELIST, 
                        object.ModelData, object.NumVertices,
                        (WORD*)0, 0, 
                        VFT_UU, 
                        skinId);
                    //delete[] modelData;
                }
            }
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void ObjModel::RenderBounds () {
    if (m_BoundsInfo.RenderingInfo.IsPrepared) {
        m_Device->GetVCacheManager()->Render(
                        PT_LINELIST, 
                        m_BoundsInfo.RenderingInfo.BufferId,
                        m_BoundsInfo.RenderingInfo.StartVertex,
                        m_BoundsInfo.IndexBufferId,
                        m_BoundsInfo.StartIndex,
                        m_BoundsInfo.RenderingInfo.PrimitiveCount,
                        m_BoundsInfo.RenderingInfo.VertexFormat, 
                        m_BoundsInfo.RenderingInfo.SkinId);
    }
}

void ObjModel::RenderBoundsDynamic () {
    vs3d::ULCVERTEX vertex[8];
    WORD index[24];
    VECTOR3 minB (m_BoundMin[0], m_BoundMin[1], m_BoundMin[2]);
    VECTOR3 maxB (m_BoundMax[0], m_BoundMax[1], m_BoundMax[2]);
    minB = cml::transform_point (m_Scale * m_Rotation * m_Translation, minB);
    maxB = cml::transform_point (m_Scale * m_Rotation * m_Translation, maxB);
    SetBoundsVertex (vertex[0], minB[0], minB[1], minB[2]);
    SetBoundsVertex (vertex[1], maxB[0], minB[1], minB[2]);
    SetBoundsVertex (vertex[2], minB[0], minB[1], maxB[2]);
    SetBoundsVertex (vertex[3], maxB[0], minB[1], maxB[2]);
    SetBoundsVertex (vertex[4], minB[0], maxB[1], minB[2]);
    SetBoundsVertex (vertex[5], maxB[0], maxB[1], minB[2]);
    SetBoundsVertex (vertex[6], minB[0], maxB[1], maxB[2]);
    SetBoundsVertex (vertex[7], maxB[0], maxB[1], maxB[2]);
    index[0] = 0;   index[1] = 1;  
    index[2] = 1;   index[3] = 3;
    index[4] = 3;   index[5] = 2;
    index[6] = 2;   index[7] = 0;
    index[8] = 4;   index[9] = 5;
    index[10] = 5;   index[11] = 7;
    index[12] = 7;   index[13] = 6;
    index[14] = 6;   index[15] = 4;
    index[16] = 0;   index[17] = 4;
    index[18] = 2;   index[19] = 6;
    index[20] = 1;   index[21] = 5;
    index[22] = 3;   index[23] = 7;
    m_Device->GetVCacheManager()->Render (PT_LINELIST, vertex, 8, index, 24, VFT_ULC, INVALID_ID);
}

void ObjModel::SetTransparency (float _transparency) {
    m_ModelColor = m_ModelColor >> 8;
    m_ModelColor = (((UINT)(255 * _transparency)) & 0xff) << 24 | m_ModelColor;
}

UINT ObjModel::GetMeshIndex (const char (&_materialName)[MAX_PATH]) {
    for (UINT i = 0; i < m_Meshes.size(); i++) {
        if (strcmp (m_Meshes[i].MaterialName, _materialName) == 0) {
            return i;
        }
    }
    return INVALID_ID;
}

void ObjModel::UpdateActiveMesh (const char (&_materialName)[MAX_PATH]) {
    try {
        m_ActiveMesh = GetMeshIndex (_materialName);
        if (m_ActiveMesh == INVALID_ID) {   // active material was not found
            ObjModelMesh newMesh;
            strcpy (newMesh.MaterialName, _materialName);
            newMesh.SkinId = INVALID_ID;
            m_ActiveMesh = m_Meshes.size();
            m_Meshes.push_back (newMesh);
        }
        ObjModelObject object;
        object.ModelData = NULL;
        object.NumVertices = 0;
        m_Meshes[m_ActiveMesh].Objects.push_back (object);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
}

void ObjModel::Load (const char* _filename) {
    Unload ();
    strcpy (m_Filename, _filename);
    FILE* file = fopen (_filename, "r");
    if (!file) {
        THROW_DETAILED_ERROR (ERRC_FILE_NOT_FOUND, _filename);
    }
    char fileLine[MAX_PATH];    // we will be reading by line
    char firstCharacters[MAX_PATH]; // a string to determine the type of a line
    m_FirstTimeBoundsUpdate = true;
    try {
        while (fgets (fileLine, MAX_PATH, file) != NULL) {
            sscanf (fileLine, "%s", firstCharacters);        
            // skip comments
            if (strcmp(firstCharacters, "#") != 0) {
                // get mtl filename
                if (strcmp(firstCharacters, "mtllib") == 0) {
                    sscanf (fileLine, "mtllib %s", m_MtlFilename);
                } else if (strcmp(firstCharacters, "v") == 0) {
                    ObjModelVertex vertex;
                    sscanf (fileLine, "v %f %f %f", &vertex.X, &vertex.Y, &vertex.Z);
                    m_Vertices.push_back (vertex);
                    UpdateBounds (vertex);
                } else if (strcmp(firstCharacters, "vt") == 0) {
                    ObjModelTexture texture;
                    sscanf (fileLine, "vt %f %f", &texture.U, &texture.V);
                    m_Textures.push_back (texture);
                } else if (strcmp(firstCharacters, "vn") == 0) {
                    ObjModelNormal normal;
                    sscanf (fileLine, "vn %f %f %f", &normal.X, &normal.Y, &normal.Z);
                    m_Normals.push_back (normal);
                } else if (strcmp(firstCharacters, "usemtl") == 0) {
                    char materialName[MAX_PATH];
                    sscanf (fileLine, "usemtl %s", materialName);
                    UpdateActiveMesh (materialName);
                } else if (strcmp(firstCharacters, "f") == 0) {
                    LoadFace(fileLine);
                }
            }
        }
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    fclose (file);
    LoadMaterials ();
}

void ObjModel::LoadFace (const char (&_line)[MAX_PATH]) {
    UINT lenght = strlen (_line);
    UINT numSeperators = 0;
    for (UINT i = 0; i < lenght; i++) {
        if (_line[i] == '/') {
            numSeperators++;
        }
    }
    ObjModelFace face;
    if (numSeperators == 0) {
        sscanf (_line, "f %u %u %u", 
            &face.VertexIndex[0],
            &face.VertexIndex[1],
            &face.VertexIndex[2]);
    } else if (numSeperators == 3) {
        m_IsTexture = true;
        face.TextureIndices.resize (3);
        sscanf (_line, "f %u/%u %u/%u %u/%u",
            &face.VertexIndex[0], &face.TextureIndices[0],
            &face.VertexIndex[1], &face.TextureIndices[1],
            &face.VertexIndex[2], &face.TextureIndices[2]);
    } else if (!strstr (_line, "//")) {
        if (numSeperators == 6) {
            m_IsTexture = true;
            m_IsNormal = true;
            face.TextureIndices.resize (3);
            face.NormalIndices.resize (3);
            sscanf (_line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
                &face.VertexIndex[0], &face.TextureIndices[0], &face.NormalIndices[0],
                &face.VertexIndex[1], &face.TextureIndices[1], &face.NormalIndices[1],
                &face.VertexIndex[2], &face.TextureIndices[2], &face.NormalIndices[2]);
        } else {
            THROW_ERROR (ERRC_BAD_FILE);
        }
    } else {
        if (numSeperators == 6) {
            m_IsNormal = true;
            face.NormalIndices.resize (3);
            sscanf (_line, "f %u//%u %u//%u %u//%u",
                &face.VertexIndex[0], &face.NormalIndices[0],
                &face.VertexIndex[1], &face.NormalIndices[1],
                &face.VertexIndex[2], &face.NormalIndices[2]);
        } else {
            THROW_ERROR (ERRC_BAD_FILE);
        }
    }
    // make vertices start from 0
    for (UINT i = 0; i < 3; i++) {
        face.VertexIndex[i]--;
        if (m_IsTexture) {
            face.TextureIndices[i]--;
        }
        if (m_IsNormal) {
            face.NormalIndices[i]--;
        }
    }
    ObjModelObject& object = m_Meshes[m_ActiveMesh].Objects[m_Meshes[m_ActiveMesh].Objects.size()-1];
    try {
        object.Faces.push_back (face);
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    //m_Meshes[m_ActiveMesh].Objects.push_back(object);
}

void ObjModel::LoadMaterials () {
    char mtlFilepath[MAX_PATH];
    strcpy(mtlFilepath, m_Filename);
    for (int i = strlen(mtlFilepath) - 1; i >= 0; i--) {
        if ((mtlFilepath[i] < 93) && (mtlFilepath[i] > 91) && (i < (int)strlen(mtlFilepath)-1)) {
            mtlFilepath[i+1] = '\0';
            break;
        }
    }
    strcat (mtlFilepath, m_MtlFilename);
    FILE* file = fopen (mtlFilepath, "r");
    if (!file) {
        m_IsMtlFile = false;
    } else {
        m_IsMtlFile = true;    
        char fileLine[MAX_PATH];    // we will be reading by line
        char firstCharacters[MAX_PATH]; // a string to determine the type of a line
        UINT currentMesh = 0;
        vs3d::MATERIAL material;
        char textureFile[MAX_PATH];
        bool firstMaterial = true;  // to know if it's first material we read
        while (fgets (fileLine, MAX_PATH, file) != NULL) {
            sscanf (fileLine, "%s", firstCharacters);        
            if (strcmp(firstCharacters, "#") != 0) {    // not a comment line
                if (strcmp(firstCharacters, "newmtl") == 0) {
                    if ((!firstMaterial) && (currentMesh != INVALID_ID)) {
                        if (strlen (textureFile) > 0) {
                            m_Meshes[currentMesh].SkinId = m_Device->GetSkinManager()->AddSkin(textureFile, material);
                        } else {
                            m_Meshes[currentMesh].SkinId = m_Device->GetSkinManager()->AddSkin(material);
                        }
                    } else if (firstMaterial) {
                        firstMaterial = false;
                    }
                    char materialName[MAX_PATH];
                    sscanf (fileLine, "newmtl %s", materialName);
                    currentMesh = GetMeshIndex (materialName);
                    textureFile[0] = '\0';
                    material = vs3d::MATERIAL (
                        vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f), 
                        vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f),
                        vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f),
                        vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f),
                        0.0f);
                    material.Ambient = vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
                    material.Diffuse = vs3d::COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
                    material.Specular = vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
                    material.Emissive = vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
                    material.Power = 1.0f;
                } else if ((strcmp(firstCharacters, "Ka") == 0) && (currentMesh != INVALID_ID)) {
                    sscanf (fileLine, "Ka %f %f %f", 
                        &material.Ambient.r,
                        &material.Ambient.g,
                        &material.Ambient.b);
                    material.Ambient.a = 1.0f;
                } else if ((strcmp(firstCharacters, "Kd") == 0) && (currentMesh != INVALID_ID)) {
                    sscanf (fileLine, "Kd %f %f %f",
                        &material.Diffuse.r,
                        &material.Diffuse.g,
                        &material.Diffuse.b);
                    material.Diffuse.a = 1.0f;
                } else if ((strcmp(firstCharacters, "Ks") == 0) && (currentMesh != INVALID_ID)) {
                    sscanf (fileLine, "Ks %f %f %f",
                        &material.Specular.r,
                        &material.Specular.g,
                        &material.Specular.b);
                    material.Specular.a = 1.0f;
                } else if ((strcmp(firstCharacters, "d") == 0) && (currentMesh != INVALID_ID)) {
                    float a;    // alpha component
                    sscanf (fileLine, "d %f", &a);
                    if (a < 0.99f) {
                        material.Diffuse.a = a;
                    }
                } else if ((strcmp(firstCharacters, "Ns") == 0) && (currentMesh != INVALID_ID)) {
                    sscanf (fileLine, "Ns %f", &material.Power);
                } else if ((strcmp(firstCharacters, "map_Kd") == 0) && (currentMesh != INVALID_ID)) {
                    sscanf (fileLine, "map_Kd %s", textureFile);
                }
            }
        }
        fclose (file);
        if ((!firstMaterial) && (currentMesh != INVALID_ID)) {
            if (strlen (textureFile) > 0) {
                UINT textureId = m_Device->GetSkinManager()->AddTexture (textureFile);
                //material.Ambient = vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f);
                //material.Diffuse = vs3d::COLORVALUE(1.0f, 1.0f, 1.0f, 0.0f);
                //material.Specular = vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f);
                //material.Emissive = vs3d::COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f);
                //material.Power = 0.0f;
                UINT materialId = m_Device->GetSkinManager()->AddMaterial (material);
                m_Meshes[currentMesh].SkinId = m_Device->GetSkinManager()->AddSkin(&textureId, 1, materialId);
            } else {
                m_Meshes[currentMesh].SkinId = m_Device->GetSkinManager()->AddSkin(material);
            }
        }
    }
}

void ObjModel::Translate (float _x, float _y, float _z) {
    MATRIX44 translation;
    cml::matrix_translation (translation, _x, _y, _z);
    //MakeTransformation (translation);
    m_Translation *= translation;
    m_IsOutdated = true;
}

void ObjModel::TranslateX (float _x) {
    MATRIX44 translation;
    cml::matrix_translation (translation, _x, 0.0f, 0.0f);
    //MakeTransformation (translation);
    m_Translation *= translation;
    m_IsOutdated = true;
}

void ObjModel::TranslateY (float _y) {
    MATRIX44 translation;
    cml::matrix_translation (translation, 0.0f, _y, 0.0f);
    //MakeTransformation (translation);
    m_Translation *= translation;
    m_IsOutdated = true;
}

void ObjModel::TranslateZ (float _z) {
    MATRIX44 translation;
    cml::matrix_translation (translation, 0.0f, 0.0f, _z);
    //MakeTransformation (translation);
    m_Translation *= translation;
    m_IsOutdated = true;
}

void ObjModel::Scale (float _scale) {
    /*for (UINT i = 0; i < m_Vertices.size(); i++) {
        m_Vertices[i].x *= _Scale;
        m_Vertices[i].y *= _Scale;
        m_Vertices[i].z *= _Scale;
    }
    for (UINT i = 0; i < 3; i++) {
        m_BoundMax[i] *= _Scale;
        m_BoundMin[i] *= _Scale;
    }
    m_OriginalSize *= _Scale;
    m_IsOutdated = true;*/
    MATRIX44 scale;
    cml::matrix_scale (scale, _scale, _scale, _scale);
    m_Scale *= scale;
    m_IsOutdated = true;
}

void ObjModel::ScaleX (float _scale) {
    MATRIX44 scale;
    cml::matrix_scale (scale, _scale, 1.0f, 1.0f);
    //MakeTransformation (scale);
    m_Scale *= scale;
    m_IsOutdated = true;
}

void ObjModel::ScaleY (float _scale) {
    MATRIX44 scale;
    cml::matrix_scale (scale, 1.0f, _scale, 1.0f);
    //MakeTransformation (scale);
    m_Scale *= scale;
    m_IsOutdated = true;
}

void ObjModel::ScaleZ (float _scale) {
    MATRIX44 scale;
    cml::matrix_scale (scale, 1.0f, 1.0f, _scale);
    //MakeTransformation (scale);
    m_Scale *= scale;
    m_IsOutdated = true;
}

void ObjModel::Rotate (float _x, float _y, float _z) {
    MATRIX44 rotation;
    rotation(0,0) = 1.0f;
    rotation(1,1) = 1.0f;
    rotation(2,2) = 1.0f;
    rotation(3,3) = 1.0f;
    //cml::matrix_rotation_world_x (rotation, _Radians);
    cml::matrix_rotation_euler (rotation, _x, _y, _z, cml::euler_order_xyz);
    //MakeTransformation (rotation);
    m_Rotation *= rotation;
    m_IsOutdated = true;
}

void ObjModel::RotateAboutX (float _radians) {
    MATRIX44 rotation;
    rotation(0,0) = 1.0f;
    rotation(1,1) = 1.0f;
    rotation(2,2) = 1.0f;
    rotation(3,3) = 1.0f;
    //cml::matrix_rotation_world_x (rotation, _Radians);
    cml::matrix_rotation_euler (rotation, _radians, 0.0f, 0.0f, cml::euler_order_xyz);
    //MakeTransformation (rotation);
    m_Rotation *= rotation;
    m_IsOutdated = true;
}

void ObjModel::RotateAboutY (float _radians) {
    MATRIX44 rotation;
    rotation(0,0) = 1.0f;
    rotation(1,1) = 1.0f;
    rotation(2,2) = 1.0f;
    rotation(3,3) = 1.0f;
    //cml::matrix_rotation_world_y (rotation, _Radians);
    cml::matrix_rotation_euler (rotation, 0.0f, _radians, 0.0f, cml::euler_order_xyz);
    //MakeTransformation (rotation);
    m_Rotation *= rotation;
    m_IsOutdated = true;
}

void ObjModel::RotateAboutZ (float _radians) {
    MATRIX44 rotation;
    rotation(0,0) = 1.0f;
    rotation(1,1) = 1.0f;
    rotation(2,2) = 1.0f;
    rotation(3,3) = 1.0f;
    //cml::matrix_rotation_world_z (rotation, _Radians);
    cml::matrix_rotation_euler (rotation, 0.0f, 0.0f, _radians, cml::euler_order_xyz);
    //MakeTransformation (rotation);
    m_Rotation *= rotation;
    m_IsOutdated = true;
}

//void ObjModel::Transform (const MATRIX44& _Transformation) {
//    MakeTransformation (_Transformation);
//}

void ObjModel::ClearTransformations () {
    m_Scale.identity();
    m_Translation.identity();
    m_Rotation.identity();
}

void ObjModel::MakeTransformation (const MATRIX44& _transformation) {
    for (UINT i = 0; i < m_Vertices.size(); i++) {
        VECTOR3 vertex (m_Vertices[i].X, m_Vertices[i].Y, m_Vertices[i].Z);
        vertex = cml::transform_point (_transformation, vertex);
        m_Vertices[i].X = vertex[0];
        m_Vertices[i].Y = vertex[1];
        m_Vertices[i].Z = vertex[2];
    }
    m_FirstTimeBoundsUpdate = true;
    for (UINT i = 0; i < m_Vertices.size(); i++) {
        UpdateBounds (m_Vertices[i]);
    }
    m_IsOutdated = true;
}

void ObjModel::UpdateBounds (const ObjModelVertex& _vertex) {
    VECTOR3 vertex (_vertex.X, _vertex.Y, _vertex.Z);
    vertex = cml::transform_point (m_Scale * m_Rotation * m_Translation, vertex);
    if (m_FirstTimeBoundsUpdate) {
        for (UINT i = 0; i < 3; i++) {
            m_BoundMax[i] = vertex[i];
            m_BoundMin[i] = vertex[i];
        }
        m_FirstTimeBoundsUpdate = false;
    } else {
        for (UINT i = 0; i < 3; i++) {
            if (m_BoundMax[i] < vertex[i]) {
                m_BoundMax[i] = vertex[i];
            } else if (m_BoundMin[i] > vertex[i]) {
                m_BoundMin[i] = vertex[i];
            }
        }
    }
}