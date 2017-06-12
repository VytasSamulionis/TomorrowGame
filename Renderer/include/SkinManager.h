/** @file SkinManager.h */

#pragma once

#include "../include/RenderDevice.h"
#include "../include/Log.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdio>

#ifdef _DEBUG
    #pragma comment (lib, "lib/Debug/Log.lib")
    #pragma comment (lib, "lib/Debug/ErrorMessage.lib")
#else
    #pragma comment (lib, "lib/Log.lib")
    #pragma comment (lib, "lib/ErrorMessage.lib")
#endif

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

/** Information of the original texture. */
struct TextureBackup {
    std::vector<void*> Data;    /**< Texture data of every level. */
    std::vector<UINT> Size;     /**< Texture size of every level. */

    /** Destructor. */
    ~TextureBackup () {
        for (UINT i = 0; i < Data.size(); i++) {
            delete[] Data[i];
        }
    }
};

/** Skin manager.
Skin is a pair of texture and material.
This class is responsible for loading textures, saving materials and
making skins. It has few methods to modify the textures. */
class SkinManager: public ISkinManager {
public:
    /** Constructor.
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    SkinManager (LPDIRECT3DDEVICE9 _device);

    /** Destructor. */
    ~SkinManager ();

    /** Adds texture to the manager.
    @param[in] _filename the name of the texture file
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return texture ID */
    UINT AddTexture (const char* _filename);

    /** Getter: pointer to IDirect3DTexture9 interface.
    @param[in] _id texture ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return pointer to IDirect3DTexture9 interface */
    void* GetTexture (UINT _id) const;

    /** Getter: ID of the texture if it is loaded.
    @param[in] _filename texture filename
    @return texture ID */
    UINT GetTextureId (const char* _filename) const;
    
    /** Getter: texture file name.
    @param[in] _id texture ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return texture filename */
    const char* GetTextureName (UINT _id) const;

    /** Returns texture texel.
    @param[in] _id texture ID
    @param[in] _u texture u coordinate
    @param[in] _v texture v coordinate 
    @param[out] _texel texture texel */
    void GetTextureTexel (UINT _id, float _u, float _v, BYTE(& _texel)[4]);
    
    /** Saves texture.
    @param[in] _id texture ID
    @param[in] _filename the path where texture should be saved 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_API_CALL D3DXSaveTextureToFile failure */
    void SaveTexture (UINT _id, const char* _filename);

    /** Remove all the textures from the skin manager.
    Skins are also removed because some of them could become invalid. */
    void RemoveTextures ();
    
    /** Checks if texture is loaded.
    @param[in] _filename texture filename
    @return @c true texture is loaded. @c false otherwise. */
    bool IsTextureLoaded (const char* _filename) const;
    
    /** Adds material to the skin manager.
    @param[in] _material material information
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 

    @return material ID */
    UINT AddMaterial (vs3d::MATERIAL _material);
    
    /** Getter: material.
    @param[in] _id material ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return vs3d::MATERIAL structure with material information */
    vs3d::MATERIAL GetMaterial (UINT _id) const;
    
    /** Getter: material ID.
    @param[in] _material material
    @return material ID */
    UINT GetMaterialId (const vs3d::MATERIAL& _material) const;
    
    /** Removes all the materials from the skin manager.
    Skins are also removed because some of them could become invalid. */
    void RemoveMaterials ();
    
    /** Checks if the material is loaded.
    @param[in] _material material which should be checked
    @return @c true material is loaded. @c false otherwise */
    bool IsMaterialLoaded (const vs3d::MATERIAL& _material) const;
   
    /** Adds skin to the skin manager.
    @param[in] _filename texture filename
    @param[in] _material material
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return skin ID */
    UINT AddSkin (const char* _filename, vs3d::MATERIAL _material);
    
    /** Adds skin to the skin manager.
    @param[in] _filename array of texture filenames
    @param[in] _numTextures number of the texture filenames in the array
    @param[in] _material material
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return skin ID */
    UINT AddSkin (const char* _filename[], UINT _numTextures, 
                  vs3d::MATERIAL _material);
    
    /** Adds skin to the skin manager.
    @param[in] _textureId texture ID
    @param[in] _materialId material ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin ID */
    UINT AddSkin (UINT _textureId, UINT _materialId);
    
    /** Adds skin to the skin manager.
    @param[in] _textureId texture ID array
    @param[in] _numTextures number of the texture ID in the array
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin ID */
    UINT AddSkin (UINT _textureId[], UINT _numTextures);

    /** Adds skin to the skin manager.
    @param[in] _textureId texture ID array
    @param[in] _numTextures number of the texture ID in the array
    @param[in] _materialId material ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin ID */
    UINT AddSkin (UINT _textureId[], UINT _numTextures, UINT _materialId);
    
    /** Adds skin to the skin manager.
    @param[in] _materialId material ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin ID */
    UINT AddSkin (UINT _materialId);
    
    /** Adds skin to the skin manager.
    @param[in] _filename texture filename
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return skin ID */
    UINT AddSkin (const char* _filename);
    
    /** Adds skin to the skin manager.
    @param[in] _filename texture filenames array
    @param[in] _numTextures number of the texture filenames in the array
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return skin ID */
    UINT AddSkin (const char* _filename[], UINT _numTextures);
    
    /** Adds skin to the skin manager.
    @param[in] _material material information
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 

    @return skin ID */
    UINT AddSkin (vs3d::MATERIAL _material);
    
    /** Getter: skin.
    @param[in] _id skin ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin */
    vs3d::SKIN GetSkin (UINT _id) const;
    
    /** Getter: pointer to the IDirect3DTexture9 interface.
    @param[in] _id skin ID
    @param[in] _stage texture stage
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return pointer to the IDirect3DTexture9 interface */
    void* GetSkinTexture (UINT _id, UINT _stage) const;
    
    /** Getter: skin material.
    @param[in] _id skin ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return material information */
    vs3d::MATERIAL GetSkinMaterial (UINT _id) const;
    
    /** Getter: skin ID.
    @param[in] _textureId texture ID array
    @param[in] _numTextures number of the texture ID in the array
    @param[in] _materialId material ID
    @return skin ID */
    UINT GetSkinId (UINT _textureId[], UINT _numTextures, UINT _materialId) const;
    
    /** Getter: the number of the skin's textures.
    @param[in] _id skin ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return the number of the skin's textures */
    UINT GetSkinNumTextures (UINT _id) const;
    
    /** Removes all the skins from the skin manager. */
    void RemoveSkins ();
    
    /** Checks if the skin is loaded.
    @param[in] _textureId texture ID array
    @param[in] _numTextures number of the texture ID in the array
    @param[in] _materialId material ID
    @return @c true skin is loaded. @c false otherwise */
    bool IsSkinLoaded (UINT _textureId[], UINT _numTextures, UINT _materialId) const;
    
    /** Removes textures, materials and skins from the skin manager. */
    void RemoveAll ();
    
    /** Draws the texture on the other texture.
    @param[in] _targetId the texture ID on which the drawing will be done
    @param[in] _textureId the texture ID which will be drawn on the target texture
    @param[in] _start the point on the texture where the texture should be drawn
    @param[in] _height height of the drawn texture
    @param[in] _width width of the drawn texture
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_INVALID_PARAMETER invalid parameters */
    void DrawOnTexture (UINT _targetId, UINT _textureId, POINT _start, UINT _height, UINT _width);

    /** Draws the texture on the other texture temporary.
    The texture can be restored by RestoreTexture.
    @param[in] _targetId the texture ID on which the drawing will be done
    @param[in] _textureId the texture ID which will be drawn on the target texture
    @param[in] _start the point on the texture where the texture should be drawn
    @param[in] _height height of the drawn texture
    @param[in] _width width of the drawn texture
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_INVALID_PARAMETER invalid parameters */
    void PreviewOnTexture (UINT _targetId, UINT _textureId, POINT _start, UINT _height, UINT _width);

    /** Changes the texture by drawing another texture on it. 
    @param[in] _targetId the texture ID on which the drawing will be done
    @param[in] _textureId the texture ID which will be drawn on the target texture
    @param[in] _start the point on the texture where the texture should be drawn
    @param[in] _height height of the drawn texture
    @param[in] _width width of the drawn texture
    @param[in] _temporary does a texture backup should be made
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_INVALID_PARAMETER invalid parameters */
    void ChangeTexture (UINT _targetId, UINT _textureId, POINT _start, UINT _height, UINT _width, bool _temporary);

    /** Draws the texture on the other texture.
    @param[in] _targetId the texture ID on which the drawing will be done
    @param[in] _textureId the texture ID which will be drawn on the target texture
    @param[in] _u the u coordinate of the texture where the texture should be drawn
    @param[in] _v the v coordinate of the texture where the texture should be drawn
    @param[in] _height height of the drawn texture
    @param[in] _width width of the drawn texture
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_INVALID_PARAMETER invalid parameters */
    void DrawOnTexture (UINT _targetId, UINT _textureId, float _u, float _v, UINT _height, UINT _width);

    /** Draws the texture on the other texture temporary.
    The texture can be restored by SkinManager::RestoreTexture().
    @param[in] _targetId the texture ID on which the drawing will be done
    @param[in] _textureId the texture ID which will be drawn on the target texture
    @param[in] _u the u coordinate of the texture where the texture should be drawn
    @param[in] _v the v coordinate of the texture where the texture should be drawn
    @param[in] _height height of the drawn texture
    @param[in] _width width of the drawn texture 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_INVALID_PARAMETER invalid parameters */
    void PreviewOnTexture (UINT _targetId, UINT _textureId, float _u, float _v, UINT _height, UINT _width);

    /** Restores the texture.
    @param[in] _textureId texture ID
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_API_CALL texture's LockRect() failure

    @see SkinManager::PreviewOnTexture() */
    void RestoreTexture (UINT _textureId);

    /** Makes the backup of the texture.
    @param[in] _textureId texture ID which backup should be made
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_API_CALL texture's LockRect() failure */
    void BackupTexture (UINT _textureId);

    /** Setups the backup texture.
    @param[in] _textureId texture ID which backup should be made 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_OUT_OF_MEM not enough memory */
    void InitTextureBackup (UINT _textureId);

    /** Setter: texture transparency when it is drawn on the other texture.
    @param[in] _transparency transparency value between 0.0f and 1.0f */
    inline void SetTexturePaintTransparency (float _transparency) {
        m_TexturePaintTransparency = _transparency;
        if (m_TexturePaintTransparency > 1.0f) {
            m_TexturePaintTransparency = 1.0f;
        }
        if (m_TexturePaintTransparency < 0.0f) {
            m_TexturePaintTransparency = 0.0f;
        }
    }

    /** Getter: the pixel's color component value of the shrinked texture.
    The color component is the one which is pointed by the pointer to the texture data.
    @param[in] _data a pointer to the texture data
    @param[in] _height a height of the texture
    @param[in] _width a width of the texture
    @param[in] _pitch a pitch value of the texture
    @return pixels' color component */
    BYTE GetValue (BYTE* _data, UINT _height, UINT _width, UINT _pitch);

    /** Getter: pixel's size by its format.
    @param[in] _format pixel format
    @return pixel's size */
    UINT GetPixelSize (D3DFORMAT _format);

protected:
    /** Transparency of the texture which is drawn on the other texture. */
    float m_TexturePaintTransparency;   

    /** Texture backups. */
    std::vector<TextureBackup*> m_TextureBackups;

    UINT m_NumTextures;             /**< Number of the textures. */
    vs3d::TEXTURE* m_Texture;       /**< Textures. */
    
    UINT m_NumMaterials;            /**< Number of the materials. */
    vs3d::MATERIAL* m_Material;     /**< Materials. */

    UINT m_NumSkins;                /**< Number of the skins. */
    vs3d::SKIN* m_Skin;             /**< Skins. */

    LPDIRECT3DDEVICE9 m_Device;     /**< Pointer to RenderDevice. */

private:
    LogManager* m_Log;          /**< Log. */
};