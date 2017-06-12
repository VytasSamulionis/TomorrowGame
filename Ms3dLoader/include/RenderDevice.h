/** @file RenderDevice.h */

#pragma once

#include "../include/Engine.h"
#include "../include/ErrorMessage.h"

/** Skin Manager interface. */
class ISkinManager {
public:
    /** Constructor. */
    ISkinManager () {};

    /** Destructor. */
    virtual ~ISkinManager () {};

    /** Adds texture to the manager.
    @param[in] _filename the name of the texture file
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return texture ID */
    virtual UINT AddTexture (const char* _filename) = 0;

    /** Getter: pointer to IDirect3DTexture9 interface.
    @param[in] _id texture ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return pointer to IDirect3DTexture9 interface */
    virtual void* GetTexture (UINT _id) const = 0;
    
    /** Getter: ID of the texture if it is loaded.
    @param[in] _filename texture filename
    @return texture ID */
    virtual UINT GetTextureId (const char* _filename) const = 0;
    
    /** Getter: texture file name.
    @param[in] _id texture ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return texture filename */
    virtual const char* GetTextureName (UINT _id) const = 0;

    /** Returns texture texel.
    @param[in] _id texture ID
    @param[in] _u texture u coordinate
    @param[in] _v texture v coordinate 
    @param[out] _texel texture texel */
    virtual void GetTextureTexel (UINT _id, float _u, float _v, BYTE(& _texel)[4]) = 0;
    
    /** Saves texture.
    @param[in] _id texture ID
    @param[in] _filename the path where texture should be saved 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_API_CALL D3DXSaveTextureToFile failure */
    virtual void SaveTexture (UINT _id, const char* _filename) = 0;

    /** Remove all the textures from the skin manager.
    Skins are also removed because some of them could become invalid. */
    virtual void RemoveTextures () = 0;
    
    /** Checks if texture is loaded.
    @param[in] _filename  texture filename
    @return @c true texture is loaded. @c false otherwise. */
    virtual bool IsTextureLoaded (const char* _filename) const = 0;
    
    /** Adds material to the skin manager.
    @param[in] _material material information
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 

    @return material ID */
    virtual UINT AddMaterial (vs3d::MATERIAL _material) = 0;
    
    /** Getter: material.
    @param[in] _id material ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return vs3d::MATERIAL structure with material information */
    virtual vs3d::MATERIAL GetMaterial (UINT _id) const = 0;
    
    /** Getter: material ID.
    @param[in] _material  material
    @return material ID */
    virtual UINT GetMaterialId (const vs3d::MATERIAL& _material) const = 0;
    
    /** Removes all the materials from the skin manager.
    Skins are also removed because some of them could become invalid. */
    virtual void RemoveMaterials () = 0;
    
    /** Checks if the material is loaded.
    @param[in] _material  material which should be checked
    @return @c true material is loaded. @c false otherwise */
    virtual bool IsMaterialLoaded (const vs3d::MATERIAL& _material) const = 0;

    /** Adds skin to the skin manager.
    @param[in] _filename texture filename
    @param[in] _material material
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return skin ID */
    virtual UINT AddSkin (const char* _filename, vs3d::MATERIAL _material) = 0;
    
    /** Adds skin to the skin manager.
    @param[in] _filename array of texture filenames
    @param[in] _numTextures number of the texture filenames in the array
    @param[in] _material material
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return skin ID */
    virtual UINT AddSkin (const char* _filename[], UINT _numTextures, 
                          vs3d::MATERIAL _material) = 0;
    
    /** Adds skin to the skin manager.
    @param[in] _textureId texture ID
    @param[in] _materialId material ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin ID */
    virtual UINT AddSkin (UINT _textureId, UINT _materialId) = 0;

    /** Adds skin to the skin manager.
    @param[in] _textureId texture ID array
    @param[in] _numTextures number of the texture ID in the array
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin ID */
    virtual UINT AddSkin (UINT _textureId[], UINT _numTextures) = 0;

    /** Adds skin to the skin manager.
    @param[in] _textureId texture ID array
    @param[in] _numTextures number of the texture ID in the array
    @param[in] _materialId material ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin ID */
    virtual UINT AddSkin (UINT _textureId[], UINT _numTextures, UINT _materialId) = 0;
    
    /** Adds skin to the skin manager.
    @param[in] _materialId material ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin ID */
    virtual UINT AddSkin (UINT _materialId) = 0;
    
    /** Adds skin to the skin manager.
    @param[in] _filename texture filename
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return skin ID */
    virtual UINT AddSkin (const char* _filename) = 0;
    
    /** Adds skin to the skin manager.
    @param[in] _filename texture filenames array
    @param[in] _numTextures number of the texture filenames in the array
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure

    @return skin ID */
    virtual UINT AddSkin (const char* _filename[], UINT _numTextures) = 0;
    
    /** Adds skin to the skin manager.
    @param[in] _material material information
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 

    @return skin ID */
    virtual UINT AddSkin (vs3d::MATERIAL _material) = 0;
    
    /** Getter: skin.
    @param[in] _id skin ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return skin */
    virtual vs3d::SKIN GetSkin (UINT _id) const = 0;
    
    /** Getter: pointer to the IDirect3DTexture9 interface.
    @param[in] _id skin ID
    @param[in] _stage texture stage
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return pointer to the IDirect3DTexture9 interface */
    virtual void* GetSkinTexture (UINT _id, UINT _stage) const = 0;
    
    /** Getter: skin material.
    @param[in] _id skin ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return material information */
    virtual vs3d::MATERIAL GetSkinMaterial (UINT _id) const = 0;
    
    /** Getter: skin ID.
    @param[in] _textureId texture ID array
    @param[in] _numTextures number of the texture ID in the array
    @param[in] _materialId material ID
    @return skin ID */
    virtual UINT GetSkinId (UINT _textureId[], UINT _numTextures, UINT _materialId) const = 0;
    
    /** Getter: the number of the skin's textures.
    @param[in] _id skin ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return the number of the skin's textures */
    virtual UINT GetSkinNumTextures (UINT _id) const = 0;
    
    /** Removes all the skins from the skin manager. */
    virtual void RemoveSkins () = 0;
    
    /** Checks if the skin is loaded.
    @param[in] _textureId  texture ID array
    @param[in] _numTextures  number of the texture ID in the array
    @param[in] _materialId  material ID
    @return @c true skin is loaded. @c false otherwise */
    virtual bool IsSkinLoaded (UINT _textureId[], UINT _numTextures, UINT _materialId) const = 0;
    
    /** Removes textures, materials and skins from the skin manager. */
    virtual void RemoveAll () = 0;

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
    virtual void DrawOnTexture (UINT _targetId, UINT _textureId, POINT _start, UINT _height, UINT _width) = 0;

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
    virtual void PreviewOnTexture (UINT _targetId, UINT _textureId, POINT _start, UINT _height, UINT _width) = 0;

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
    virtual void DrawOnTexture (UINT _targetId, UINT _textureId, float _u, float _v, UINT _height, UINT _width) = 0;

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
    virtual void PreviewOnTexture (UINT _targetId, UINT _textureId, float _u, float _v, UINT _height, UINT _width) = 0;

    /** Restores the texture.
    @param[in] _textureId texture ID
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID
        - @c ERRC_API_CALL texture's LockRect() failure

    @see ISkinManager::PreviewOnTexture() */
    virtual void RestoreTexture (UINT _textureId) = 0;

    /** Setter: texture transparency when it is drawn on the other texture.
    @param[in] _transparency  transparency value between 0.0f and 1.0f */
    virtual void SetTexturePaintTransparency (float _transparency) = 0;
};

/** Vertex cache manager. */
class IVertexCacheManager {
public:
    /** Constructor. */
    IVertexCacheManager () {};

    /** Destructor. */
    virtual ~IVertexCacheManager () {};

    /** Creates static vertex buffer.
    Vertices are inserted into the buffer.
    @param[in] _vertex vertices which are inserted to the created buffer
    @param[in] _numVertices number of the vertices
    @param[in] _vft vertex format
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format 

    @return static buffer ID */
    virtual UINT CreateStaticVertexBuffer (void* _vertex, UINT _numVertices, VERTEXFORMATTYPE _vft) = 0;

    /** Adds vertices to the existing static vertex buffer.
    @param[in] _vertexBufferId static buffer ID
    @param[in] _vertex the vertices
    @param[in] _numVertices number of the vertices
    @param[in] _vft vertex format 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format */
    virtual void AddToStaticVertexBuffer (UINT _vertexBufferId, void* _vertex, UINT _numVertices, VERTEXFORMATTYPE _vft) = 0;

    /** Removes all the static vertex buffers. */
    virtual void ClearStaticVertexBuffers () = 0;

    /** Creates static index buffer.
    Indices are inserted into the buffer.
    @param[in] _index indices which are inserted to the created buffer
    @param[in] _numIndices number of the indices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL

    @return static buffer ID */
    virtual UINT CreateStaticIndexBuffer (WORD* _index, UINT _numIndices) = 0;

    /** Adds indices to the existing static index buffer.
    @param[in] _indexBufferId static index buffer ID
    @param[in] _index the indices
    @param[in] _numIndices number of the indices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL */
    virtual void AddToStaticIndexBuffer (UINT _indexBufferId, WORD* _index, UINT _numIndices) = 0;

    /** Removes all the static index buffers. */
    virtual void ClearStaticIndexBuffers () = 0;

    /** Creates particle buffer.
    @param[in] _size size of the buffer
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL

    @return particle buffer ID */
    virtual UINT CreateParticleBuffer (UINT _size) = 0;

    /** Removes all the particle buffers. */
    virtual void ClearParticleBuffers () = 0;

    /** Renders static vertex with index buffers.
    @param[in] _type primitive type
    @param[in] _vertexBufferId static vertex buffer ID
    @param[in] _startVertex at which vertex the rendering should start
    @param[in] _indexBufferId static index buffer ID
    @param[in] _startIndex at which index the rendering should start
    @param[in] _numPrimitives the number of primitives
    @param[in] _vft vertex format
    @param[in] _skinId skin ID. Pass INVALID_ID if no skin is needed 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    virtual void Render (PRIMITIVETYPE _type,
                         UINT _vertexBufferId, UINT _startVertex,
                         UINT _indexBufferId, UINT _startIndex,
                         UINT _numPrimitives,
                         VERTEXFORMATTYPE _vft, UINT _skinId) = 0;

    /** Renders vertices with static index buffer.
    @param[in] _type primitive type
    @param[in] _vertex array of the vertices
    @param[in] _numVertices the number of the vertices
    @param[in] _indexBufferId static index buffer ID
    @param[in] _startIndex at which index the rendering should start
    @param[in] _numPrimitives the number of primitives
    @param[in] _vft vertex format
    @param[in] _skinId skin ID. Pass INVALID_ID if no skin is needed 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid 
        - @c ERRC_API_CALL 
        - @c ERRC_OUT_OF_RANGE */
    virtual void Render (PRIMITIVETYPE _type,
                         void* _vertex, UINT _numVertices,
                         UINT _indexBufferId, UINT _startIndex,
                         UINT _numPrimitives, VERTEXFORMATTYPE _vft, UINT _skinId) = 0;

    /** Renders static vertex buffer with dynamic indices.
    @param[in] _type primitive type
    @param[in] _vertexBufferId static vertex buffer ID
    @param[in] _startVertex at which vertex the rendering should start
    @param[in] _index array of the indices
    @param[in] _numIndices the number of the indices
    @param[in] _numPrimitives the number of primitives
    @param[in] _vft vertex format
    @param[in] _skinId skin ID. Pass INVALID_ID if no skin is needed 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL index buffer Lock() failure */
    virtual void Render (PRIMITIVETYPE _type,
                         UINT _vertexBufferId, UINT _startVertex,
                         WORD* _index, UINT _numIndices,
                         UINT _numPrimitives, VERTEXFORMATTYPE _vft, UINT _skinId) = 0;

    /** Renders dynamically vertices with indices.
    @param[in] _type primitive type
    @param[in] _vertex array of the vertices
    @param[in] _numVertices the number of the vertices
    @param[in] _index array of the indices
    @param[in] _numIndices the number of the indices
    @param[in] _vft vertex format
    @param[in] _skinId skin ID. Pass INVALID_ID if no skin is needed
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid 
        - @c ERRC_API_CALL */
    virtual void Render (PRIMITIVETYPE _type,
                         void* _vertex, UINT _numVertices, 
                         WORD* _index, UINT _numIndices, 
                         VERTEXFORMATTYPE _vft, UINT _skinId) = 0;

    /** Renders particles.
    @param[in] _particle array of the particles
    @param[in] _numParticles number of the particles
    @param[in] _bufferId particle buffer ID
    @param[in] _skinId skin ID. Pass INVALID_ID if no skin is needed 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE particle buffer ID is invalid
        - @c ERRC_API_CALL */
    virtual void RenderParticles (vs3d::ULCVERTEX* _particle, UINT _numParticles, UINT _bufferId, UINT _skinId) = 0;

    /** Create an effect.
    @param[in] _effectData pointer to either the effect data in the
    memory or the filename of the effect file
    @param[in] _dataSize size of the effect data in the memory. If the
    effect is in the file, this parameter is irrelevent
    @param[in] _isFromFile the effect is in the file
    @param[in] _isCompiled the effect is compiled
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_API_CALL
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_FILE_NOT_FOUND effect file is not found
    @return effect ID */
    virtual UINT CreateEffect (void* _effectData, UINT _dataSize, 
                               bool _isFromFile) = 0;

    /** Sets the effect's technique.
    @param[in] _effectId ID of the effect which describes the technique
    @param[in] _techniqueName technique name in the effect
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID
        - @c ERRC_INVALID_PARAMETER invalid technique name */
    virtual void EnableEffect (UINT _effectId, const char* _techniqueName) = 0;

    /** Sets effect's texture parameter name.
    It is used when skin has to be set. If effect's texture parameter is not
    set, then the skin's texture will not be set.
    @param[in] _effectId effect ID
    @param[in] _stage texture stage [0-7]
    @param[in] _texParamName texture parameter name
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE either invalid effect ID or invalid stage */
    virtual void SetEffectTextureParamName (UINT _effectId, UINT _stage, 
                                                const char* _texParamName) = 0;

    /** Sets effect's texture.
    @param[in] _effectId effect ID
    @param[in] _texParamName texture parameter name
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    virtual void SetEffectTexture (UINT _effectId, const char* _texParamName, UINT _textureId) = 0;

    /** Sets effect's material diffuse color parameter name.
    It is used when skin has to be set. If effect's material diffuse parameter
    is set, then the specified parameter will be set to skin's diffuse 
    material color.
    @param[in] _effectId effect ID
    @param[in] _diffuseParamName material diffuse color parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    virtual void SetEffectMtrlDiffuseParamName (UINT _effectId, const char* _diffuseParamName) = 0;

    /** Sets effect's material ambient color parameter name.
    It is used when skin has to be set. If effect's material ambient parameter
    is set, then the specified parameter will be set to skin's ambient
    material color.
    @param[in] _effectId effect ID
    @param[in] _ambientParamName material ambient color parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    virtual void SetEffectMtrlAmbientParamName (UINT _effectId, const char* _ambientParamName) = 0;

    /** Sets effect's material specular color parameter name.
    It is used when skin has to be set. If effect's material specular
    parameter is set, then the specified parameter will be set to skin's
    specular material color.
    @param[in] _effectId effect ID
    @param[in] _specularParamName material specular color parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    virtual void SetEffectMtrlSpecularParamName (UINT _effectId, const char* _specularParamName) = 0;

    /** Sets effect's material emissive color parameter name.
    It is used when skin has to be set. If effect's material emissive parameter
    is set, then the specified parameter will be set to skin's emissive
    material color.
    @param[in] _effectId effect ID
    @param[in] _emissiveParamName material emissive color parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    virtual void SetEffectMtrlEmissiveParamName (UINT _effectId, const char* _emissiveParamName) = 0;

    /** Sets effect's material power parameter name.
    It is used when skin has to be set. If effect's material power parameter
    is set, then the specified parameter will be set to skin's power
    material value.
    @param[in] _effectId effect ID
    @param[in] _powerParamName material power parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    virtual void SetEffectMtrlPowerParamName (UINT _effectId, const char* _powerParamName) = 0;

    /** Sets the parameter value.
    @param[in] _parameterName parameter name
    @param[in] _value a pointer to the parameter value
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID
        - @c ERRC_INVALID_PARAMETER */
    virtual void SetEffectParameter (UINT _effectId, const char* _parameterName, void* _value) = 0;

    /** Disables effects and returns functionality of fixed function pipeline. */
    virtual void DisableEffects () = 0;

    /** Setter: text size.
    @param[in] _size  text size */
    virtual void SetTextSize (int _size) = 0;

    /** Setter: text style.
    @param[in] _style  text style */
    virtual void SetTextStyle (const char* _style) = 0;

    /** Renders text.
    @param[in] _text text
    @param[in] _color color of the text
    @param[in] _x position of the text on the x axis
    @param[in] _y position of the text on the y axis 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL */
    virtual void RenderText (const char* _text, DWORD _color, float _x, float _y) = 0;

    /** Forces all the data to be flushed to the GPU. 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_API_CALL */
    virtual void Flush () = 0;
};

/** Render device. */
class RenderDevice {
public:
    /** Constructor. */
    RenderDevice () {};

    /** Destructor. */
    virtual ~RenderDevice () {};

    /** Initializes graphics API for full screen window.
    @param[in] _hwnd handle to the window
    @param[in] _width width of the window
    @param[in] _height height of the window 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL
        - @c ERRC_OUT_OF_MEM not enough memory */
    virtual void InitFullScreen (HWND _hwnd, UINT _width, UINT _height) = 0;
    
    /** Initializes graphics API for windowed screen window.
    @param[in] _hwnd handle to window
    @param[in] _width width of window
    @param[in] _height height of window
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_API_CALL 
        - @c ERRC_OUT_OF_MEM not enough memory */
    virtual void InitWindowed (HWND _hwnd, UINT _width, UINT _height) = 0;

    /** Changes clear color (background).
    Default clear color is white.
    @param[in] _red red color value in range of [0.0; -1.0]
    @param[in] _green green color value in range of [0.0; -1.0]
    @param[in] _blue blue color value in range of [0.0; -1.0] */
    virtual void SetClearColor (float _red, float _green, float _blue) = 0;
    
    /** Checks if device is running and ready.
    @return @c true device is ready. @c false otherwise. */
    virtual bool IsRunning () const = 0;
    
    /** Clears buffers.
    @param[in] _target target buffer should be cleared
    @param[in] _stencil stencil buffer should be cleared
    @param[in] _zBuffer zbuffer should be cleared 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL Clear() failure 
        - @c ERRC_NO_DEVICE device is not ready */
    virtual void Clear (bool _target, bool _stencil, bool _zBuffer) = 0;
    
    /** Clears buffers and starts rendering.
    @param[in] _target target buffer should be cleared
    @param[in] _stencil stencil buffer should be cleared
    @param[in] _zBuffer zbuffer should be cleared
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL
        - @c ERRC_NO_DEVICE device is not ready */
    virtual void BeginRendering (bool _target, bool _stencil, bool _zBuffer) = 0;
    
    /** Ends rendering. */
    virtual void EndRendering () = 0;
    
    /** Releases resources. */
    virtual void Release () = 0;
    
    // views

    /** Changes projection view.
    @param[in] _fovy field of view in the y direction, in radians
    @param[in] _znear z-value of the near view-plane
    @param[in] _zfar z-value of the far view-plane
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTrasnform() failure 
        - @c ERRC_NO_DEVICE device is not ready */
    virtual void SetProjectionView (float _fovy, float _znear, float _zfar) = 0;
    
    /** Changes camera view.
    @param[in] _eye position of the camera
    @param[in] _at where to look
    @param[in] _up vector pointing upwards
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTrasnform() failure 
        - @c ERRC_NO_DEVICE device is not ready */
    virtual void LookAt (const VECTOR3& _eye, 
                            const VECTOR3& _at, 
                            const VECTOR3& _up) = 0;
    

    // transformations

    /** Translates world matrix.
    @param[in] _x translation in x coordinate
    @param[in] _y translation in y coordinate
    @param[in] _z translation in z coordinate 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTrasnform() failure */
    virtual void TranslateWorldMatrix (float _x, float _y, float _z) = 0;
    
    /** Rotates world matrix.
    @param[in] _rotation euler angles 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTrasnform() failure */
    virtual void RotateWorldMatrix (const VECTOR3& _rotation) = 0;

    // rendering states

    /** Changes cull mode.
    Valid RENDERSTATETYPE values:
    - RS_CULL_CW
    - RS_CULL_CCW
    - RS_CULL_NONE
    @param[in] _state culling mode 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    virtual void SetCullingState (RENDERSTATETYPE _state) = 0;
    
    /** Changes depth buffer state.
    Valid RENDERSTATETYPE values:
    - RS_DEPTH_READWRITE
    - RS_DEPTH_READONLY
    - RS_DEPTH_NONE 
    @param[in] _state depth buffer state
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    virtual void SetDepthBufferState (RENDERSTATETYPE _state) = 0;
    
    /** Changes rendering mode.
    Valid RENDERSTATETYPE values:
    - RS_DRAW_POINTS
    - RS_DRAW_WIRE
    - RS_DRAW_SOLID
    @param[in] _state render mode
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    virtual void SetDrawingState (RENDERSTATETYPE _state) = 0;
    
    /** Changes stencil buffer mode.
    Valid RENDERSTATETYPE values:
    - RS_STENCIL_DISABLE
    - RS_STENCIL_ENABLE
    - RS_STENCIL_FUNC_ALWAYS
    - RS_STENCIL_FUNC_LESSEQUAL
    - RS_STENCIL_FAIL_DECR
    - RS_STENCIL_FAIL_INC
    - RS_STENCIL_FAIL_KEEP
    - RS_STENCIL_ZFAIL_DECR
    - RS_STENCIL_ZFAIL_INCR
    - RS_STENCIL_ZFAIL_KEEP
    - RS_STENCIL_PASS_DECR
    - RS_STENCIL_PASS_INCR
    - RS_STENCIL_PASS_KEEP
    @param[in] _state stencil buffer mode
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    virtual void SetStencilBufferState (RENDERSTATETYPE _state) = 0;
    
    /** Changes stencil buffer mode.
    Valid RENDERSTATETYPE values:
    - RS_STENCIL_MASK
    - RS_STENCIL_WRITEMASK
    - RS_STENCIL_REF
    @param[in] _state stencil buffer mode
    @param[in] _value value
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    virtual void SetStencilBufferState (RENDERSTATETYPE _state, DWORD _value) = 0;
    
    /** Changes shading mode.
    Valid RENDERSTATETYPE values:
    - RS_SHADE_FLAT
    - RS_SHADE_GOURAUD
    @param[in] _state shading mode 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    virtual void SetShadingState (RENDERSTATETYPE _state) = 0;

    /** Changes texture stage state.
    @param[in] _stage stage of a texture
    @param[in] _type what state to change
    @param[in] _value TEXTUREOP or TA value, depends on state
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL SetTextureStageState() failure
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    virtual void SetTextureStageState (UINT _stage, TEXTURESTAGESTATETYPE _type, DWORD _value) = 0;
    
    /** Enables point sprites. 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void EnablePointSprites () = 0;

    /** Disables point sprites.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void DisablePointSprites () = 0;

    /** Enables point scale. 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void EnablePointsScale () = 0;

    /** Disables point scale.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void DisablePointsScale () = 0;

    /** Setter: point size.
    @param[in] _size size of the point 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void SetPointsSize (float _size) = 0;

    /** Changes point sprite state.
    @param[in] _type what state to change
    @param[in] _value value
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure 
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    virtual void SetPointSpriteState (POINTSPRITESTATETYPE _type, float _value) = 0;

    /** Changes alpha blend state.
    @param[in] _type what state to change
    @param[in] _blend blend type 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure 
        - @c ERRC_INVALID_PARAMETER invalid parameter */
    virtual void SetAlphaBlendState (ALPHABLENDSTATETYPE _type, BLENDTYPE _blend) = 0;

    /** Enables alpha blend.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void EnableAlphaBlend () = 0;

    /** Disables alpha blend. 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void DisableAlphaBlend () = 0;

    // lighting
    
    /** Setter: ambient light.
    @param[in] _color ambient light's ARGB format color
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void SetAmbientLight (DWORD _color) = 0;

    /** Sets light.
    @param[in] _index light index [0-8]
    @param[in] _light light information 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_API_CALL SetLight() failure 
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER invalid _light parameter */
    virtual void SetLight (DWORD _index, const vs3d::LIGHT& _light) = 0;
    
    /** Enable light.
    @param[in] _index which light
    @param[in] _enable enable light
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL LightEnable() failure 
        - @c ERRC_NO_DEVICE device is not ready */
    virtual void EnableLight (DWORD _index, bool _enable) = 0;
    
    /** Enable lighting.
    @param[in] _enable enable lighting 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void EnableLighting (bool _enable) = 0;

    virtual void CreateShadowMap (UINT _size, const MATRIX44& _lightWorldView, float _farClip) = 0;

    virtual void BeginRenderingToShadowMap () = 0;

    virtual void EndRenderingToShadowMap () = 0;

    virtual void SetShadowMap (UINT _effectId, const char* _shadowMapParamName) = 0;
    
    /** Enables fog.
    @param[in] _start the distance where the fog will start
    @param[in] _end the distance where the fog will end
    @param[in] _density fog density
    @param[in] _mode fog mode
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void EnableFog (float _start, float _end, float _density, FOGMODE _mode) = 0;

    /** Disables fog. 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetRenderState() failure */
    virtual void DisableFog () = 0;

    // managers

    /** Returns pointer to SkinManager interface.
    @return pointer to SkinManager interface */
    virtual ISkinManager* GetSkinManager () const = 0;
    
    /** Returns pointer to VertexCacheManager interface.
    @return pointer to VertexCacheManager interface */
    virtual IVertexCacheManager* GetVCacheManager () const = 0;
    
    /** Getter: projection matrix.
    @return projection matrix */
    virtual MATRIX44 GetProjectionMatrix () const = 0;

    /** Getter: view matrix.
    @return view matrix */
    virtual MATRIX44 GetViewMatrix () const = 0;

    /** Getter: world matrix.
    @return world matrix */
    virtual MATRIX44 GetWorldMatrix () const = 0;

    /** Setter: world matrix.
    @param[in] _matrix world matrix
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_API_CALL SetTransform() failure */
    virtual void SetWorldMatrix (MATRIX44 _matrix) = 0;

    /** Getter: viewport matrix.
    @return viewport matrix */
    virtual MATRIX44 GetViewportMatrix (float _ScreenWidth, float _ScreenHeight) const = 0;
};

extern "C" {
    typedef HRESULT (*CREATERENDERDEVICE) (HINSTANCE, RenderDevice**);
    typedef void (*RELEASERENDERDEVICE) (RenderDevice**);
}
