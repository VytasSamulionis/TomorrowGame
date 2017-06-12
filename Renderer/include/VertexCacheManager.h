/** @file VertexCacheManager.h */

#pragma once

#include "../include/RenderDevice.h"
#include "../include/SkinManager.h"
#include "../include/VertexCache.h"
#include "../include/Log.h"
//#include "../include/AVLTreeVertexFormats.h"
#include "../include/RenderCache.h"
#include "../include/d3dfont.h"
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")

#ifdef _DEBUG
    #pragma comment (lib, "lib/Debug/log.lib")
    #pragma comment (lib, "lib/Debug/ErrorMessage.lib")
#else
    #pragma comment (lib, "lib/log.lib")
    #pragma comment (lib, "lib/ErrorMessage.lib")
#endif


#define MAX_VERTEX_NUM 17000    /**< Size of vertex buffer. */
#define MAX_INDEX_NUM 51000     /**< Size of index buffer. */
#define CACHE_NUM 10            /**< How many caches to create. */

class VertexCache;
class RenderCache;
namespace RenderCacheNS {
    struct Node;
}
//class AVLTreeVertexFormats;
//namespace AvlTreeVFormat {
//    struct Node;
//}

#define NUM_VERTEX_FORMATS 9    /**< Number of the vertex formats. */

#define VF_UPVERTEX     m_VertexDecl[0]     /**< Untransformed position only vertex. */
#define VF_UUVERTEX     m_VertexDecl[1]     /**< Untransformed unlit vertex. */
#define VF_UUVERTEX2    m_VertexDecl[2]     /**< Untransformed unlit vertex. */
#define VF_ULVERTEX     m_VertexDecl[3]     /**< Untransformed lit vertex. */
#define VF_ULVERTEX2    m_VertexDecl[4]     /**< Untransformed lit vertex. */
#define VF_ULCVERTEX    m_VertexDecl[5]     /**< Untransformed lit colored vertex. */
#define VF_TPVERTEX     m_VertexDecl[6]     /**< Transformed position vertex. */
#define VF_TLVERTEX     m_VertexDecl[7]     /**< Transformed lit vertex. */
#define VF_TLCVERTEX    m_VertexDecl[8]     /**< Transformed lit colored vertex. */

/** Keeping info of a vertex cache array. */
struct VertexCacheInfo {
    VertexCache* Cache[CACHE_NUM];  /**< Vertex caches. */
    VertexCache* Empty;             /**< Pointer to the empty cache. */
    VertexCache* Fullest;           /**< Pointer to the fullest cache. */
};

/** Keeping various type vertex caches. */
struct CacheInfo {
    VertexCacheInfo Static;            /**< Static. */
    VertexCacheInfo Dynamic;           /**< Dynamic. */
    VertexCacheInfo IndexedStatic;     /**< Static with indices. */
    VertexCacheInfo IndexedDynamic;    /**< Dynamic with indices. */
};

/** Static vertex buffer. */
struct StaticVertexBuffer {
    LPDIRECT3DVERTEXBUFFER9 Buffer; /**< Vertex buffer. */
    UINT NumVertices;               /**< Number of the vertices. */
};

/** Static index buffer. */
struct StaticIndexBuffer {
    LPDIRECT3DINDEXBUFFER9 Buffer;  /**< Index buffer. */
    UINT NumIndices;                /**< Number of the indices. */
};

/** Particle buffer. */
struct Particle {
    LPDIRECT3DVERTEXBUFFER9 Buffer; /**< Vertex buffer. */
    UINT Size;                      /**< Size of the buffer. */
};

struct EffectData {
    ID3DXEffect* Effect;
    D3DXHANDLE TextureHandle[8];
    D3DXHANDLE MtrlDiffuseHandle;
    D3DXHANDLE MtrlAmbientHandle;
    D3DXHANDLE MtrlSpecularHandle;
    D3DXHANDLE MtrlEmissiveHandle;
    D3DXHANDLE MtrlPowerHandle;
};

/** Manages the caches. */
class VertexCacheManager: public IVertexCacheManager {
public:
    /** Constructor.
    @param[in] _device a pointer to the rendering device
    @param[in] _skin a pointer to the skin manager
    @param[in] _log a pointer to the log manager 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL */
    VertexCacheManager (LPDIRECT3DDEVICE9 _device,
                        SkinManager* _skin,
                        LogManager* _log);

    /** Destructor. */
    ~VertexCacheManager ();

    /** Creates static vertex buffer.
    Vertices are inserted into the buffer.
    @param[in] _vertex vertices which are inserted to the created buffer
    @param[in] _numVertices number of the vertices
    @param[in] _vft vertex format
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_VF invalid vertex format 

    @return static buffer ID */
    UINT CreateStaticVertexBuffer (void* _vertex, UINT _numVertices, VERTEXFORMATTYPE _vft);

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
        - @c ERRC_UNKNOWN_VF invalid vertex format */
    void AddToStaticVertexBuffer (UINT _vertexBufferId, void* _vertex, UINT _numVertices, VERTEXFORMATTYPE _vft);

    /** Removes all the static vertex buffers. */
    void ClearStaticVertexBuffers ();

    /** Creates static index buffer.
    Indices are inserted into the buffer.
    @param[in] _index indices which are inserted to the created buffer
    @param[in] _numIndices number of the indices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL

    @return static buffer ID */
    UINT CreateStaticIndexBuffer (WORD* _index, UINT _numIndices);

    /** Adds indices to the existing static index buffer.
    @param[in] _indexBufferId static index buffer ID
    @param[in] _index the indices
    @param[in] _numIndices number of the indices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL */
    void AddToStaticIndexBuffer (UINT _indexBufferId, WORD* _index, UINT _numIndices);

    /** Removes all the static index buffers. */
    void ClearStaticIndexBuffers ();

    /** Creates particle buffer.
    @param[in] _size size of the buffer
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL

    @return particle buffer ID */
    UINT CreateParticleBuffer (UINT _size);

    /** Removes all the particle buffers. */
    void ClearParticleBuffers ();

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
    void Render (PRIMITIVETYPE _type,
                    UINT _vertexBufferId, UINT _startVertex,
                    UINT _indexBufferId, UINT _startIndex,
                    UINT _numPrimitives,
                    VERTEXFORMATTYPE _vft, UINT _skinId);

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
    void Render (PRIMITIVETYPE _type,
                 void* _vertex, UINT _numVertices,
                 UINT _indexBufferId, UINT _startIndex,
                 UINT _numPrimitives, VERTEXFORMATTYPE _vft, UINT _skinId);

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
    void Render (PRIMITIVETYPE _type,
                 UINT _vertexBufferId, UINT _startVertex,
                 WORD* _index, UINT _numIndices,
                 UINT _numPrimitives, VERTEXFORMATTYPE _vft, UINT _skinId);

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
    void Render (PRIMITIVETYPE _type,
                 void* _vertex, UINT _numVertices, 
                 WORD* _index, UINT _numIndices, 
                 VERTEXFORMATTYPE _vft, UINT _skinId);

    /** Renders particles.
    @param[in] _particle array of the particles
    @param[in] _numParticles number of the particles
    @param[in] _bufferId particle buffer ID
    @param[in] _skinId skin ID. Pass INVALID_ID if no skin is needed 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE particle buffer ID is invalid
        - @c ERRC_API_CALL */
    void RenderParticles (vs3d::ULCVERTEX* _particle, UINT _numParticles, UINT _bufferId, UINT _skinId);

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
    UINT CreateEffect (void* _effectData, UINT _dataSize, 
                       bool _isFromFile);

    /** Sets the effect's technique.
    @param[in] _effectId ID of the effect which describes the technique
    @param[in] _techniqueName technique name in the effect
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID
        - @c ERRC_INVALID_PARAMETER invalid technique name */
    void EnableEffect (UINT _effectId, const char* _techniqueName);

    /** Sets effect's texture parameter name.
    It is used when skin has to be set. If effect's texture parameter is not
    set, then the skin's texture will not be set.
    @param[in] _effectId effect ID
    @param[in] _stage texture stage [0-7]
    @param[in] _texParamName texture parameter name
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE either invalid effect ID or invalid stage */
    void SetEffectTextureParamName (UINT _effectId, UINT _stage, 
                                    const char* _texParamName);

    /** Sets effect's texture.
    @param[in] _effectId effect ID
    @param[in] _texParamName texture parameter name
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    void SetEffectTexture (UINT _effectId, const char* _texParamName, UINT _textureId);

    /** Sets effect's material diffuse color parameter name.
    It is used when skin has to be set. If effect's material diffuse parameter
    is set, then the specified parameter will be set to skin's diffuse 
    material color.
    @param[in] _effectId effect ID
    @param[in] _diffuseParamName material diffuse color parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    void SetEffectMtrlDiffuseParamName (UINT _effectId, const char* _diffuseParamName);

    /** Sets effect's material ambient color parameter name.
    It is used when skin has to be set. If effect's material ambient parameter
    is set, then the specified parameter will be set to skin's ambient
    material color.
    @param[in] _effectId effect ID
    @param[in] _ambientParamName material ambient color parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    void SetEffectMtrlAmbientParamName (UINT _effectId, const char* _ambientParamName);

    /** Sets effect's material specular color parameter name.
    It is used when skin has to be set. If effect's material specular
    parameter is set, then the specified parameter will be set to skin's
    specular material color.
    @param[in] _effectId effect ID
    @param[in] _specularParamName material specular color parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    void SetEffectMtrlSpecularParamName (UINT _effectId, const char* _specularParamName);

    /** Sets effect's material emissive color parameter name.
    It is used when skin has to be set. If effect's material emissive parameter
    is set, then the specified parameter will be set to skin's emissive
    material color.
    @param[in] _effectId effect ID
    @param[in] _emissiveParamName material emissive color parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    void SetEffectMtrlEmissiveParamName (UINT _effectId, const char* _emissiveParamName);

    /** Sets effect's material power parameter name.
    It is used when skin has to be set. If effect's material power parameter
    is set, then the specified parameter will be set to skin's power
    material value.
    @param[in] _effectId effect ID
    @param[in] _powerParamName material power parameter name
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID */
    void SetEffectMtrlPowerParamName (UINT _effectId, const char* _powerParamName);

    /** Sets the parameter value.
    @param[in] _parameterName parameter name
    @param[in] _value a pointer to the parameter value
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid effect ID
        - @c ERRC_INVALID_PARAMETER */
    void SetEffectParameter (UINT _effectId, const char* _parameterName, void* _value);

    /** Disables effects and returns functionality of fixed function pipeline. */
    void DisableEffects ();
    
    /** Setter: text size.
    @param[in] _size text size */
    void SetTextSize (int _size);

    /** Setter: text style.
    @param[in] _style text style */
    void SetTextStyle (const char* _style);

    /** Renders text.
    @param[in] _text text
    @param[in] _color color of the text
    @param[in] _x position of the text on the x axis
    @param[in] _y position of the text on the y axis 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL */
    void RenderText (const char* _text, DWORD _color, float _x, float _y);

    /** Sets skin for rendering.
    @param[in] _skinId: id of skin in skin manager
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_API_CALL */
    void SetSkin (UINT _skinId);
    
    /** Getter: current skin ID which is set for rendering. 
    @return current skin ID which is set for rendering. */
    UINT GetActiveSkinId () const;
    
    /** Getter: active vertex buffer ID.
    @param[in] _isStatic buffer is static
    @param[in] _isForStatic if buffer is not static, it is for static rendering
    @return active vertex buffer ID */
    inline UINT GetActiveVertexBufferId (bool _isStatic, bool _isForStatic = false) const {
        if (_isStatic) {
            return m_ActiveStaticVertexBuffer;
        }
        if (_isForStatic) {
            if (m_IsCacheVertexForStatic) {
                return m_ActiveCacheVertexBuffer;
            } else {
                return INVALID_ID;
            }
        }
        return m_ActiveCacheVertexBuffer;
    }

    /** Setter: active vertex buffer ID.
    @param[in] _isStatic buffer is static
    @param[in] _bufferId buffer ID
    @param[in] _isForStatic if buffer is not static, it is for static rendering */
    inline void SetActiveVertexBufferId (bool _isStatic, UINT _bufferId, bool _isForStatic = false) {
        m_IsCacheVertexForStatic = _isForStatic;
        if (_isStatic) {
            m_ActiveStaticVertexBuffer = _bufferId;
            if (_bufferId != INVALID_ID) {
                m_ActiveCacheVertexBuffer = INVALID_ID;
            }
        } else {
            m_ActiveCacheVertexBuffer = _bufferId;
            if (_bufferId != INVALID_ID) {
                m_ActiveStaticVertexBuffer = INVALID_ID;
            }
        }
    }

    /** Getter: number of the vertices in the static vertex buffer.
    @param[in] _id static vertex buffer ID 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return number of the vertices in the static vertex buffer */
    inline UINT GetVertexBufferNumVertices (UINT _id) const {
        if (_id >= m_StaticVertexBuffers.size()) {
            THROW_ERROR (ERRC_OUT_OF_RANGE);
        }
        return m_StaticVertexBuffers[_id].NumVertices;
    }

    /** Getter: pointer to the static vertex buffer.
    @param[in] _id static vertex buffer ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return pointer to the static vertex buffer */
    inline LPDIRECT3DVERTEXBUFFER9 GetVertexBufferData (UINT _id) const {
        if (_id >= m_StaticVertexBuffers.size()) {
            THROW_ERROR (ERRC_OUT_OF_RANGE);
        }
        return m_StaticVertexBuffers[_id].Buffer;
    }

    /** Getter: active index buffer ID.
    @param[in] _isStatic buffer is static
    @param[in] _isForStatic if buffer is not static, it is for static rendering
    @return active index buffer ID */
    inline UINT GetActiveIndexBufferId (bool _isStatic, bool _isForStatic = false) const {
        if (_isStatic) {
            return m_ActiveStaticIndexBuffer;
        }
        if (_isForStatic) {
            if (m_IsCacheIndexForStatic) {
                return m_ActiveCacheIndexBuffer;
            } else {
                return INVALID_ID;
            }
        }
        return m_ActiveCacheIndexBuffer;
    }

    /** Setter: active index buffer ID.
    @param[in] _isStatic buffer is static
    @param[in] _bufferId buffer ID
    @param[in] _isForStatic if buffer is not static, it is for static rendering */
    inline void SetActiveIndexBufferId (bool _isStatic, UINT _bufferId, bool _isForStatic = false) {
        m_IsCacheIndexForStatic = _isForStatic;
        if (_isStatic) {
            m_ActiveStaticIndexBuffer = _bufferId;
            if (_bufferId != INVALID_ID) {
                m_ActiveCacheIndexBuffer = INVALID_ID;
            }
        } else {
            m_ActiveCacheIndexBuffer = _bufferId;
            if (_bufferId != INVALID_ID) {
                m_ActiveStaticIndexBuffer = INVALID_ID;
            }
        }
    }

    /** Getter: number of the indices in the static index buffer.
    @param[in] _id static index buffer ID 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return number of the indices in the static index buffer */
    inline UINT GetIndexBufferNumIndices (UINT _id) const {
        if (_id >= m_StaticIndexBuffers.size()) {
            THROW_ERROR (ERRC_OUT_OF_RANGE);
        }
        return m_StaticIndexBuffers[_id].NumIndices;
    }

    /** Getter: pointer to the static index buffer.
    @param[in] _id static index buffer ID
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid ID

    @return pointer to the static index buffer */
    inline LPDIRECT3DINDEXBUFFER9 GetIndexBufferData (UINT _id) const {
        if (_id >= m_StaticIndexBuffers.size()) {
            THROW_ERROR (ERRC_OUT_OF_RANGE);
        }
        return m_StaticIndexBuffers[_id].Buffer;
    }

    /** Getter: pointer to the rendering device.
    @return pointer to the rendering device */
    inline LPDIRECT3DDEVICE9 GetDirect3DDevice () {
        return m_Device;
    }

    /** Getter: pointer to the skin manager.
    @return pointer to the skin manager */
    inline SkinManager* GetSkinManager () {
        return m_Skin;
    }

    /*inline AVLTreeVertexFormats* GetVertexFormatsTree() {
        return m_VertexFormatsTree;
    }*/

    /** Getter: pointer to the log manager.
    @return pointer to the log manager */
    inline LogManager* GetLog () {
        return m_Log;
    }

    /** Forces all the data to be flushed to the GPU. 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_API_CALL */
    void Flush ();

    /** Getter: vertex declaration.
    @return vertex declaration */
    IDirect3DVertexDeclaration9* GetVertexDeclaration (VERTEXFORMATTYPE _vft);

    /** Getter: active effect.
    @return active effect. */
    ID3DXEffect* GetActiveEffect ();

    /** Getter: effect.
    @return effect. */
    ID3DXEffect* GetEffect (UINT _effectId);

private:
    /** Creates vertex declarations.
    @warning Used in the contructor.
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_API_CALL */
    void CreateVertexDeclarations ();

    /** Forces the render cache and all its nodes to be flushed to the GPU.
    @param[in] _node tree node
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_API_CALL */
    void Flush (RenderCacheNS::Node* _node);

    /** Getter: vertex size.
    @param[in] _vft vertex format
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_UNKNOWN_VF invalid vertex format 
        
    @return vertex size in bytes */
    UINT GetVertexSize (VERTEXFORMATTYPE _vft);

    //AVLTreeVertexFormats* m_VertexFormatsTree;
    RenderCache** m_VertexFormatCaches;      /**< Array of the pointers to the vertex caches. */

    /** Vertex declaration array. */
    IDirect3DVertexDeclaration9* m_VertexDecl[NUM_VERTEX_FORMATS];
    std::vector<EffectData> m_Effects;    /**< Effects. */
    EffectData* m_ActiveEffect;     /**< Active effect. If effects are disabled, it is set to NULL. */
    SkinManager* m_Skin;            /**< Pointer to the skin manager. */
    UINT m_ActiveSkin;              /**< Skin which is set for the rendering. */
    IDirect3DDevice9* m_Device;     /**< Pointer to the rendering device. */
    LogManager* m_Log;              /**< Pointer to the log manager. */
    CD3DFont* m_Font;               /**< Font rendering class. */
    int m_FontSize;                 /**< Font size. */
    char m_FontStyle[MAX_PATH];     /**< Font style .*/

    std::vector<StaticVertexBuffer> m_StaticVertexBuffers;  /**< Static vertex buffers. */
    UINT m_ActiveStaticVertexBuffer;        /**< Active static vertex buffer. */
    UINT m_ActiveCacheVertexBuffer;         /**< Active vertex cache vertex buffer. */
    bool m_IsCacheVertexForStatic;          /**< Active vertex cache vertex buffer is for static rendering. */
    std::vector<StaticIndexBuffer> m_StaticIndexBuffers;    /**< Static index buffers. */
    UINT m_ActiveStaticIndexBuffer;         /**< Active static index buffer. */
    UINT m_ActiveCacheIndexBuffer;          /**< Active vertex cache index buffer. */
    bool m_IsCacheIndexForStatic;           /**< Active vertex cache index buffer is for static rendering. */
    std::vector<Particle> m_ParticleBuffers;    /**< Particle buffers. */
};