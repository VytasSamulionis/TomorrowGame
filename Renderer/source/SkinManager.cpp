#include "../include/SkinManager.h"

#include <DxErr.h>
#pragma comment (lib, "dxerr.lib")

using namespace vs3d;

SkinManager::SkinManager (LPDIRECT3DDEVICE9 _device): m_Device (_device) {
    try {
        #ifdef _DEBUG
        m_Log = new LogManager ("log_skin_manager.txt");
        #else
        m_Log = NULL;
        #endif
    } catch (std::bad_alloc) {
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    m_NumTextures = 0;
    m_Texture = NULL;
    m_NumMaterials = 0;
    m_Material = NULL;
    m_NumSkins = 0;
    m_Skin = NULL;
    m_TexturePaintTransparency = 1.0f;
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Skin Manager is up and running.\n");
    }
    #endif
}

SkinManager::~SkinManager () {
    #ifdef _DEBUG
    if (m_Log) {
        m_Log->Log ("Shutting down Skin Manager...\n");
    }
    #endif
    RemoveAll ();
    #ifdef _DEBUG
    if (m_Log) {
        delete m_Log;
        m_Log = NULL;
    }
    #endif
}

UINT SkinManager::AddTexture (const char* _filename) {
    // check if texture is not already loaded
    if (IsTextureLoaded (_filename)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Warning: Texture %s is already loaded.\n", _filename);
        }
        #endif
        return GetTextureId (_filename);
    }
    // load texture
    IDirect3DTexture9* texture;
    HRESULT hr = D3DXCreateTextureFromFileA (m_Device, _filename, &texture);
    if (FAILED (hr)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Unable to load texture: %s\n", _filename);
        }
        #endif
        char msg[250];
        strcpy (msg, DXGetErrorStringA(hr));
		#ifdef _DEBUG
        m_Log->Log (msg);
		#endif
        THROW_DETAILED_ERROR (ERRC_API_CALL, "D3DXCreateTextureFromFile() failed.");
    }
    // check and allocate memory if it is necessary
    if (m_NumTextures % 50 == 0) {
        m_Texture = (TEXTURE*) realloc (m_Texture, sizeof (TEXTURE) * (m_NumTextures + 50));
        if (!m_Texture) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory.\n");
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    // add texture
    strcpy (m_Texture[m_NumTextures].Name, _filename);
    m_Texture[m_NumTextures].Data = (void*) texture;
    try {
        m_TextureBackups.push_back(new TextureBackup);
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of memory.\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }

    return m_NumTextures++;
}

void* SkinManager::GetTexture (UINT _id) const {
    if (_id >= m_NumTextures) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: texture id (%d) is out of range.\n", _id);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Texture[_id].Data;
}

UINT SkinManager::GetTextureId (const char* _name) const {
    for (UINT i = 0; i < m_NumTextures; i++) {
        if (strcmp (m_Texture[i].Name, _name) == 0) {
            return i;
        }
    }
    return INVALID_ID;
}

const char* SkinManager::GetTextureName (UINT _id) const {
    if (_id >= m_NumTextures) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: texture id (%d) is out of range.\n", _id);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Texture[_id].Name;
}

void SkinManager::GetTextureTexel (UINT _id, float _u, float _v, BYTE(& _texel)[4]) {
    if (_id >= m_NumTextures) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: texture id (%d) is out of range.\n", _id);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)m_Texture[_id].Data;
    D3DSURFACE_DESC texDesc;
    texture->GetLevelDesc(0, &texDesc);
    POINT point;
    point.x = (UINT)(texDesc.Width * _u);
    point.y = (UINT)(texDesc.Height * _v);
    D3DLOCKED_RECT source;
    if (FAILED (texture->LockRect (0, &source, NULL, D3DLOCK_READONLY))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "LockRect() failure.");
    }
    UINT index = point.x * GetPixelSize (texDesc.Format) + source.Pitch * point.y;
    _texel[3] = ((BYTE*)source.pBits)[index];       // blue
    _texel[2] = ((BYTE*)source.pBits)[index + 1];   // green
    _texel[1] = ((BYTE*)source.pBits)[index + 2];   // red
    _texel[0] = ((BYTE*)source.pBits)[index + 3];   // alpha
    texture->UnlockRect (0);
}

void SkinManager::SaveTexture (UINT _id, const char* _filename) {
    if (_id >= m_NumTextures) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: texture id (%d) is out of range.\n", _id);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    if (FAILED (D3DXSaveTextureToFileA (_filename, D3DXIFF_JPG, (LPDIRECT3DTEXTURE9)m_Texture[_id].Data, NULL))) {
        THROW_DETAILED_ERROR (ERRC_API_CALL, "D3DXSaveTextureToFile() failed.");
    }
}

void SkinManager::RemoveTextures () {
    if (m_Texture) {
        for (UINT i = 0; i < m_NumTextures; i++) {
            ((LPDIRECT3DTEXTURE9) m_Texture[i].Data)->Release ();
        }
        free (m_Texture);
        m_Texture = NULL;
        m_NumTextures = 0;
    }
    RemoveSkins ();
}

bool SkinManager::IsTextureLoaded (const char* _name) const {
    if (GetTextureId (_name) == INVALID_ID) {
        return false;
    }
    return true;
}

UINT SkinManager::AddMaterial (MATERIAL _material) {
    // check if material exists
    if (IsMaterialLoaded (_material)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Warning: adding existing material.\n");
        }
        #endif
        return GetMaterialId (_material);
    }
    // check and allocate memory if it is necessary
    if (m_NumMaterials % 50 == 0) {
        int size = sizeof (MATERIAL) * (m_NumMaterials + 50);
        m_Material = (MATERIAL*) realloc (m_Material, size);
        if (!m_Material) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory.\n");
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    // add material
    m_Material[m_NumMaterials] = _material;

    return m_NumMaterials++;
}

MATERIAL SkinManager::GetMaterial (UINT _id) const {
    if (_id < 0 || _id >= m_NumMaterials) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: material id (%d) is out of range.\n", _id);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Material[_id];
}

UINT SkinManager::GetMaterialId (const MATERIAL& _material) const {
    for (UINT i = 0; i < m_NumMaterials; i++) {
        if (m_Material[i] == _material) {
            return i;
        }
    }
    return INVALID_ID;
}

void SkinManager::RemoveMaterials () {
    if (m_Material) {
        free (m_Material);
        m_Material = NULL;
        m_NumMaterials = 0;
    }
    RemoveSkins ();
}

bool SkinManager::IsMaterialLoaded (const MATERIAL& _material) const {
    if (GetMaterialId (_material) == INVALID_ID) {
        return false;
    }
    return true;
}

UINT SkinManager::AddSkin (const char* _filename, MATERIAL _material) {
    // add texture and material to manager
    UINT textureId = AddTexture (_filename);
    UINT materialId = AddMaterial (_material);
    // check for duplication
    if (IsSkinLoaded (&textureId, 1, materialId)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin with such texture (id: %d) and material (id: %d) is already laoded.\n", textureId, materialId);
        }
        #endif
        return GetSkinId (&textureId, 1, materialId);
    }
    // allocate memory if necessary
    if (m_NumSkins % 50 == 0) {
        int size = sizeof (SKIN) * (m_NumSkins + 50);
        m_Skin = (SKIN*) realloc (m_Skin, size);
        if (!m_Skin) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory.\n");
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    // add skin
    m_Skin[m_NumSkins].TextureId[0] = textureId;
    m_Skin[m_NumSkins].MaterialId = materialId;
    m_Skin[m_NumSkins].NumTextures = 1;

    return m_NumSkins++;
}

UINT SkinManager::AddSkin (const char* _filename[], UINT _numTextures, MATERIAL _material) {
    // add texture and material to manager
    UINT* textureId = NULL;
    try {
        textureId = new UINT[_numTextures];
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of memory. (SkinManager::AddSkin)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
     
    for (UINT i = 0; i < _numTextures; i++) {
        textureId[i] = AddTexture (_filename[i]);;
    }
    UINT materialId = AddMaterial (_material);
    // check for duplication
    if (IsSkinLoaded (textureId, _numTextures, materialId)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin with such texture (id: %d) and material (id: %d) is already laoded.\n", textureId, materialId);
        }
        #endif
        UINT skinId = GetSkinId (textureId, _numTextures, materialId);
        delete[] textureId;
        return skinId;
    }
    // allocate memory if necessary
    if (m_NumSkins % 50 == 0) {
        int size = sizeof (SKIN) * (m_NumSkins + 50);
        m_Skin = (SKIN*) realloc (m_Skin, size);
        if (!m_Skin) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory.\n");
            }
            #endif
            delete[] textureId;
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    // add skin
    for (UINT i = 0; i < _numTextures; i++) {
        m_Skin[m_NumSkins].TextureId[i] = textureId[i];
    }
    m_Skin[m_NumSkins].MaterialId = materialId;
    m_Skin[m_NumSkins].NumTextures = _numTextures;
    delete[] textureId;

    return m_NumSkins++;
}

UINT SkinManager::AddSkin (UINT _textureId, UINT _materialId) {
    // check ranges
    if (_textureId >= m_NumTextures && _textureId != INVALID_ID || 
        _materialId >= m_NumMaterials && _materialId != INVALID_ID) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Texture (%d) or material (%d) id is out of range.\n", _textureId, _materialId);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    // check for duplication
    if (IsSkinLoaded (&_textureId, 1, _materialId)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin with such texture (id: %d) and material (id: %d) is already loaded.\n", _textureId, _materialId);
        }
        #endif
        return GetSkinId (&_textureId, 1, _materialId);
    }
    // allocate memory if necessary
    if (m_NumSkins % 50 == 0) {
        int size = sizeof (SKIN) * (m_NumSkins + 50);
        m_Skin = (SKIN*) realloc (m_Skin, size);
        if (!m_Skin) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory.\n");
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    // add skin
    m_Skin[m_NumSkins].TextureId[0] = _textureId;
    m_Skin[m_NumSkins].MaterialId = _materialId;
    m_Skin[m_NumSkins].NumTextures = 1;

    return m_NumSkins++;
}

UINT SkinManager::AddSkin (UINT _textureId[], UINT _numTextures) {
    // check ranges
    for (UINT i = 0; i < _numTextures; i++) {
        if (_textureId[i] >= m_NumTextures) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Texture (%d) id is out of range.\n", _textureId[i]);
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_RANGE);
        }
    }
    // check for duplication
    if (IsSkinLoaded (_textureId, _numTextures, INVALID_ID)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin with such textures is already loaded.\n");
        }
        #endif
        return GetSkinId (_textureId, _numTextures, INVALID_ID);
    }
    // allocate memory if necessary
    if (m_NumSkins % 50 == 0) {
        int size = sizeof (SKIN) * (m_NumSkins + 50);
        m_Skin = (SKIN*) realloc (m_Skin, size);
        if (!m_Skin) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory.\n");
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    // add skin
    for (UINT i = 0; i < _numTextures; i++) {
        m_Skin[m_NumSkins].TextureId[i] = _textureId[i];
    }
    m_Skin[m_NumSkins].MaterialId = INVALID_ID;
    m_Skin[m_NumSkins].NumTextures = _numTextures;

    return m_NumSkins++;
}

UINT SkinManager::AddSkin (UINT _textureId[], UINT _numTextures, UINT _materialId) {
    // check ranges
    for (UINT i = 0; i < _numTextures; i++) {
        if (_textureId[i] >= m_NumTextures) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Texture (%d) id is out of range.\n", _textureId[i]);
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_RANGE);
        }
    }
    if (_materialId >= m_NumMaterials) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Material (%d) id is out of range.\n", _materialId);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    // check for duplication
    if (IsSkinLoaded (_textureId, _numTextures, _materialId)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin with such texture and material (id: %d) is already loaded.\n", _materialId);
        }
        #endif
        return GetSkinId (_textureId, _numTextures, _materialId);
    }
    // allocate memory if necessary
    if (m_NumSkins % 50 == 0) {
        int size = sizeof (SKIN) * (m_NumSkins + 50);
        m_Skin = (SKIN*) realloc (m_Skin, size);
        if (!m_Skin) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory.\n");
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    // add skin
    for (UINT i = 0; i < _numTextures; i++) {
        m_Skin[m_NumSkins].TextureId[i] = _textureId[i];
    }
    m_Skin[m_NumSkins].MaterialId = _materialId;
    m_Skin[m_NumSkins].NumTextures = _numTextures;

    return m_NumSkins++;
}

UINT SkinManager::AddSkin (UINT _materialId) {
    if (_materialId >= m_NumMaterials) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Material (%d) id is out of range. (SkinManager::AddSkin)\n", _materialId);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    UINT texId = INVALID_ID;
    if (IsSkinLoaded (&texId, 0, _materialId)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin with such material (%d) and without texture is already loaded. (SkinManager::AddSkin)\n", _materialId);
        }
        #endif
        return GetSkinId (&texId, 0, _materialId);
    }
    if (m_NumSkins % 50 == 0) {
        int size = sizeof (SKIN) * (m_NumSkins + 50);
        m_Skin = (SKIN*) realloc (m_Skin, size);
        if (!m_Skin) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory. (SkinManager::AddSkin)\n");
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    m_Skin[m_NumSkins].TextureId[0] = INVALID_ID;
    m_Skin[m_NumSkins].MaterialId = _materialId;
    m_Skin[m_NumSkins].NumTextures = 0;

    return m_NumSkins++;
}

UINT SkinManager::AddSkin (const char* _filename) {
    UINT textureId = AddTexture (_filename);
    if (IsSkinLoaded (&textureId, 1, INVALID_ID)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin with such texture (%d) and without material is already loaded. (SkinManager::AddSkin)\n", textureId);
        }
        #endif
        return GetSkinId (&textureId, 1, INVALID_ID);
    }
    if (m_NumSkins % 50 == 0) {
        int size = sizeof (SKIN) * (m_NumSkins + 50);
        m_Skin = (SKIN*) realloc (m_Skin, size);
        if (!m_Skin) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory. (SkinManager::AddSkin)\n");
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    m_Skin[m_NumSkins].TextureId[0] = textureId;
    m_Skin[m_NumSkins].MaterialId = INVALID_ID;
    m_Skin[m_NumSkins].NumTextures = 1;

    return m_NumSkins++;
}

UINT SkinManager::AddSkin (const char* _filename[], UINT _numTextures) {
    UINT* textureId = NULL;
    try {
        textureId = new UINT[_numTextures];
    } catch (std::bad_alloc) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Out of memory. (SkinManager::AddSkin)\n");
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_MEM);
    }
    for (UINT i = 0; i < _numTextures; i++) {
        textureId[i] = AddTexture (_filename[i]);
    }
    if (IsSkinLoaded (textureId, _numTextures, INVALID_ID)) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin with such texture and without material is already loaded. (SkinManager::AddSkin)\n");
        }
        #endif
        UINT skinId = GetSkinId (textureId, _numTextures, INVALID_ID);
        delete[] textureId;
        return skinId;
    }
    if (m_NumSkins % 50 == 0) {
        int size = sizeof (SKIN) * (m_NumSkins + 50);
        m_Skin = (SKIN*) realloc (m_Skin, size);
        if (!m_Skin) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory. (SkinManager::AddSkin)\n");
            }
            #endif
            delete[] textureId;
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    for (UINT i = 0; i < _numTextures; i++) {
        m_Skin[m_NumSkins].TextureId[i] = textureId[i];
    }
    m_Skin[m_NumSkins].MaterialId = INVALID_ID;
    m_Skin[m_NumSkins].NumTextures = _numTextures;
    delete[] textureId;

    return m_NumSkins++;
}

UINT SkinManager::AddSkin (MATERIAL _material) {
    UINT materialId = AddMaterial (_material);
    UINT texId = INVALID_ID;
    if (IsSkinLoaded (&texId, 0, materialId)) {
		#ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin with such material (%d) and without texture is already loaded. (SkinManager::AddSkin)\n", materialId);
        }
		#endif
        return GetSkinId (&texId, 0, materialId);
    }
    if (m_NumSkins % 50 == 0) {
        int size = sizeof (SKIN) * (m_NumSkins + 50);
        m_Skin = (SKIN*) realloc (m_Skin, size);
        if (!m_Skin) {
            #ifdef _DEBUG
            if (m_Log) {
                m_Log->Log ("Error: Out of memory. (SkinManager::AddSkin)\n");
            }
            #endif
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
    m_Skin[m_NumSkins].TextureId[0] = INVALID_ID;
    m_Skin[m_NumSkins].MaterialId = materialId;
    m_Skin[m_NumSkins].NumTextures = 0;

    return m_NumSkins++;
}

SKIN SkinManager::GetSkin (UINT _id) const {
    if (_id >= m_NumSkins) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin id (%d) is out of range.\n", _id);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Skin[_id];
}

void* SkinManager::GetSkinTexture (UINT _id, UINT _stage) const {
    if (_id >= m_NumSkins) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin id (%d) is out of range.\n", _id);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    if (_stage >= m_Skin[_id].NumTextures) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: Incorrect texture stage %u. (SkinManager::GetSkinTexture)\n", _stage);
        }
        #endif
        THROW_DETAILED_ERROR (ERRC_OUT_OF_RANGE, "Incorrect texture stage.");
    }
    return GetTexture (m_Skin[_id].TextureId[_stage]);
}

MATERIAL SkinManager::GetSkinMaterial (UINT _id) const {
    if (_id >= m_NumSkins) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin id (%d) is out of range.\n", _id);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return GetMaterial (m_Skin[_id].MaterialId);
}

UINT SkinManager::GetSkinId (UINT _textureId[], UINT _numTextures, UINT _materialId) const {
    for (UINT i = 0; i < m_NumSkins; i++) {
        if (m_Skin[i].NumTextures == _numTextures && 
            m_Skin[i].MaterialId == _materialId) {
                bool isEqual = true;
                for (UINT j = 0; j < _numTextures; j++) {
                    if (m_Skin[i].TextureId[j] != _textureId[j]) {
                        isEqual = false;
                        break;
                    }
                }
                if (isEqual) {
                    return i;
                }
        }
    }
    return INVALID_ID;
}

UINT SkinManager::GetSkinNumTextures (UINT _id) const {
    if (_id >= m_NumSkins) {
        #ifdef _DEBUG
        if (m_Log) {
            m_Log->Log ("Error: skin id (%d) is out of range.\n", _id);
        }
        #endif
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    return m_Skin[_id].NumTextures;
}

void SkinManager::RemoveSkins () {
    if (m_Skin) {
        free (m_Skin);
        m_Skin = NULL;
        m_NumSkins = 0;
    }
}

bool SkinManager::IsSkinLoaded (UINT _textureId[], UINT _numTextures, UINT _materialId) const {
    if (GetSkinId (_textureId, _numTextures, _materialId) == INVALID_ID) {
        return false;
    }
    return true;
}

void SkinManager::RemoveAll () {
    /*RemoveTextures ();
    RemoveMaterials ();*/
    RemoveSkins ();
    for (UINT i = 0; i < m_TextureBackups.size(); i++) {
        delete m_TextureBackups[i];
    }
    m_TextureBackups.clear();
}

void SkinManager::DrawOnTexture (UINT _targetId, UINT _textureId, POINT _start, UINT _height, UINT _width) {
    return ChangeTexture (_targetId, _textureId, _start, _height, _width, false);
}

void SkinManager::PreviewOnTexture (UINT _targetId, UINT _textureId, POINT _start, UINT _height, UINT _width) {
    return ChangeTexture (_targetId, _textureId, _start, _height, _width, true);
}

void SkinManager::DrawOnTexture (UINT _targetId, UINT _textureId, float _u, float _v, UINT _height, UINT _width) {
    if (_targetId >= m_NumTextures || _textureId >= m_NumTextures) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)m_Texture[_targetId].Data;
    D3DSURFACE_DESC texDesc;
    texture->GetLevelDesc(0, &texDesc);
    POINT point;
    point.x = (UINT)(texDesc.Width * _u);
    point.y = (UINT)(texDesc.Height * _v);
    return ChangeTexture (_targetId, _textureId, point, _height, _width, false);
}

void SkinManager::PreviewOnTexture (UINT _targetId, UINT _textureId, float _u, float _v, UINT _height, UINT _width) {
    if (_targetId >= m_NumTextures || _textureId >= m_NumTextures) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)m_Texture[_targetId].Data;
    D3DSURFACE_DESC texDesc;
    texture->GetLevelDesc(0, &texDesc);
    POINT point;
    point.x = (UINT)(texDesc.Width * _u);
    point.y = (UINT)(texDesc.Height * _v);
    return ChangeTexture (_targetId, _textureId, point, _height, _width, true);
}


void SkinManager::ChangeTexture (UINT _targetId, UINT _textureId, POINT _start, UINT _height, UINT _width, bool _temporary) {
    if (_targetId >= m_NumTextures || _textureId >= m_NumTextures) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)m_Texture[_textureId].Data;
    LPDIRECT3DTEXTURE9 target = (LPDIRECT3DTEXTURE9)m_Texture[_targetId].Data;
    D3DSURFACE_DESC texDesc;
    texture->GetLevelDesc(0, &texDesc);
    if (_height > texDesc.Height || _width > texDesc.Width) {
        THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
    D3DSURFACE_DESC targetDesc;
    target->GetLevelDesc(0, &targetDesc);
    if ((UINT)_start.x > targetDesc.Width || (UINT)_start.y > targetDesc.Height) {
        THROW_ERROR (ERRC_INVALID_PARAMETER);
    }
    if (m_TextureBackups[_targetId]->Data.size() == 0) {
        InitTextureBackup(_targetId);
        BackupTexture (_targetId);
    }
    if (_temporary) {
        RestoreTexture (_targetId);
    }
    UINT oldHeight = 0, oldWidth = 0;
    for (UINT i = 0; i < target->GetLevelCount(); i++) {
        texture->GetLevelDesc(i, &texDesc);
        D3DLOCKED_RECT source;
        texture->LockRect (i, &source, NULL, 0);
        target->GetLevelDesc(i, &targetDesc);
        if (i == 0) {
            oldHeight = targetDesc.Height;
            oldWidth = targetDesc.Width;
        }
        D3DLOCKED_RECT rect;
        RECT area;
        float ratioH = (float)targetDesc.Height / oldHeight;
        float ratioW = (float)targetDesc.Width / oldWidth;
        area.left = (LONG)(_start.x * ratioW);
        area.top = (LONG)(_start.y * ratioH);
        area.right = (LONG)(_start.x * ratioW + _width * ratioW);
        area.bottom = (LONG)(_start.y * ratioH + _height * ratioH);
        target->LockRect (i, &rect, &area, 0);
        BYTE* dest = (BYTE*)rect.pBits;
        for (int j = 0; j < (LONG)(_height * ratioH); j++) {
            BYTE* dest2 = dest;
            for (int k = 0; k < (LONG)(_width * ratioW); k++) {
                UINT sourceHeight = (UINT)(texDesc.Height / (_height * ratioH));
                UINT sourceWidth = (UINT)(texDesc.Width / (_width * ratioW));
                UINT offset = j * sourceHeight * source.Pitch + k * sourceWidth * GetPixelSize (texDesc.Format);
                BYTE b = GetValue ((BYTE*)source.pBits + offset, sourceHeight, sourceWidth, source.Pitch);
                BYTE g = GetValue (((BYTE*)source.pBits) + offset + 1, sourceHeight, sourceWidth, source.Pitch);
                BYTE r = GetValue (((BYTE*)source.pBits) + offset + 2, sourceHeight, sourceWidth, source.Pitch);
                b = (BYTE)(b * m_TexturePaintTransparency) + (BYTE)(dest2[0] * (1.0f - m_TexturePaintTransparency));
                g = (BYTE)(g * m_TexturePaintTransparency) + (BYTE)(dest2[1] * (1.0f - m_TexturePaintTransparency));
                r = (BYTE)(r * m_TexturePaintTransparency) + (BYTE)(dest2[2] * (1.0f - m_TexturePaintTransparency));
                memset (dest2, b, 1);    // B
                dest2++;
                memset (dest2, g, 1);    // G
                dest2++;
                memset (dest2, r, 1);    // R
                dest2++;
                memset (dest2, 0xff, 1);    // A
                dest2++;
            }
            dest += rect.Pitch;
        }
        target->UnlockRect(i);
        texture->UnlockRect (i);
    }
    if (!_temporary) {
        BackupTexture (_targetId);
    }
}

void SkinManager::RestoreTexture (UINT _textureId) {
    if (_textureId >= m_TextureBackups.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    if (m_TextureBackups[_textureId]->Data.size() != 0) {
        LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)m_Texture[_textureId].Data;
        for (UINT i = 0; i < texture->GetLevelCount(); i++) {
            D3DLOCKED_RECT rect;
            if (FAILED (texture->LockRect (i, &rect, NULL, 0))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "LockRect() failed.");
            }
            memcpy (rect.pBits, m_TextureBackups[_textureId]->Data[i], m_TextureBackups[_textureId]->Size[i]);
            texture->UnlockRect(i);
        }
    }
}

void SkinManager::BackupTexture (UINT _textureId) {
    if (_textureId >= m_TextureBackups.size()) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    if (m_TextureBackups[_textureId]->Data.size() != 0) {
        LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)m_Texture[_textureId].Data;
        for (UINT i = 0; i < texture->GetLevelCount(); i++) {
            D3DLOCKED_RECT rect;
            if (FAILED (texture->LockRect (i, &rect, NULL, 0))) {
                THROW_DETAILED_ERROR (ERRC_API_CALL, "LockRect() failed.");
            }
            memcpy (m_TextureBackups[_textureId]->Data[i], rect.pBits, m_TextureBackups[_textureId]->Size[i]);
            texture->UnlockRect(i);
        }
    }
}

void SkinManager::InitTextureBackup (UINT _textureId) {
    if (_textureId >= m_NumTextures) {
        THROW_ERROR (ERRC_OUT_OF_RANGE);
    }
    LPDIRECT3DTEXTURE9 texture = (LPDIRECT3DTEXTURE9)m_Texture[_textureId].Data;
    m_TextureBackups[_textureId]->Data.resize (texture->GetLevelCount());
    m_TextureBackups[_textureId]->Size.resize (texture->GetLevelCount());
    for (UINT i = 0; i < texture->GetLevelCount(); i++) {
        D3DSURFACE_DESC texDesc;
        texture->GetLevelDesc(i, &texDesc);
        UINT pixelSize = GetPixelSize (texDesc.Format);
        m_TextureBackups[_textureId]->Size[i] = texDesc.Height * texDesc.Width * pixelSize;
        try {
            m_TextureBackups[_textureId]->Data[i] = new BYTE[m_TextureBackups[_textureId]->Size[i]];
        } catch (std::bad_alloc) {
            THROW_ERROR (ERRC_OUT_OF_MEM);
        }
    }
}

BYTE SkinManager::GetValue (BYTE* _data, UINT _height, UINT _width, UINT _pitch) {
    UINT value = 0, number = 0;
    for (UINT i = 0; i < _height; i++) {
        BYTE* dest = _data + i * _pitch;
        for (UINT j = 0; j < _width; j++) {
            value += dest[0];
            dest += 4;
            number++;
        }
    }
    if (number == 0) {
        number = 1;
    }
    return (BYTE)(value / number);
}

UINT SkinManager::GetPixelSize (D3DFORMAT _format) {
    switch (_format) {
        case D3DFMT_R5G6B5:
        case D3DFMT_X1R5G5B5:
        case D3DFMT_A1R5G5B5:
        case D3DFMT_A4R4G4B4:
        case D3DFMT_A8R3G3B2:
        case D3DFMT_X4R4G4B4:
            return 2;
        case D3DFMT_R8G8B8:
            return 3;
        case D3DFMT_A8R8G8B8:
        case D3DFMT_X8R8G8B8:
        case D3DFMT_A2B10G10R10:
        case D3DFMT_A8B8G8R8:
        case D3DFMT_X8B8G8R8:
        case D3DFMT_G16R16:
        case D3DFMT_A2R10G10B10:
            return 4;
        default:
            return 0;
    }
}