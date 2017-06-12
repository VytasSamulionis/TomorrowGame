/** @file VertexCache.h */

#pragma once

#include "../include/VertexCacheManager.h"
//#include "../include/AVLTreeRenderElements.h"

class VertexCacheManager;
//struct AvlTreeRender::Node;

/** Static rendering information. */
struct StaticRendering {
    INT BaseVertexIndex;    /**< At which vertex the rendering should start. */
    UINT StartIndex;        /**< At which index the rendering should start. */
    /** Static vertex buffer ID. 
    INVALID_ID indicates that vertices for the dynamic rendering should be used. */
    UINT VertexBufferId;
    /** Static index buffer ID. 
    INVALID_ID indicates that indices for the dynamic rendering should be used. */
    UINT IndexBufferId;
    UINT NumPrimitives;     /**< Number of the primitives. */
};

/** Renders the data. */
class VertexCache {
public:
    /** Constructor.
    @param[in] _vft vertex format
    @param[in] _type primitive type
    @param[in] _vcm pointer to the vertex cache manager
    @param[in] _skinId skin ID
    @param[in] _maxVertices maximum number of the vertices
    @param[in] _maxIndices maximum number of the indices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_UNKNOWN_vft invalid vertex format
        - @c ERRC_API_CALL */
    VertexCache (VERTEXFORMATTYPE _vft,
                 PRIMITIVETYPE _type,
                 VertexCacheManager* _vcm,
                 UINT _skinId,
                 UINT _maxVertices,
                 UINT _maxIndices);

    /** Destructor. */
    ~VertexCache ();

    /** Adds vertices and indices.
    @param[in] _vertex array of the vertices
    @param[in] _numVertices number of the vertices
    @param[in] _index array of the indices
    @param[in] _numIndices number of the indices 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid 
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL */
    void Add (void* _vertex, UINT _numVertices,
              WORD* _index, UINT _numIndices);

    /** Adds vertices.
    @param[in] _vertex array of the vertices
    @param[in] _numVertices number of the vertices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid 
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_OUT_OF_RANGE */
    void AddVertices (void* _vertex, UINT _numVertices);

    /** Adds indices.
    @param[in] _index array of the indices
    @param[in] _numIndices number of the indices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL index buffer Lock() failure */
    void AddIndices (WORD* _index, UINT _numIndices);

    /** Adds indices.
    It starts from the number of the vertices in the cache and adds
    the specified number of the indices increasing by one.
    @param[in] _numIndices number of the indices to add
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    void AddIndices (UINT _numIndices);

    /** Adds vertices for the static rendering.
    @param[in] _vertex array of the vertices
    @param[in] _numVertices number of the vertices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid 
        - @c ERRC_API_CALL 
        - @c ERRC_OUT_OF_RANGE */
    void AddStaticVertices (void* _vertex, UINT _numVertices);

    /** Adds indices for the static rendering.
    @param[in] _index array of the indices
    @param[in] _numIndices number of the indices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL index buffer Lock() failure */
    void AddStaticIndices (WORD* _index, UINT _numIndices);

    /** Adds static rendering.
    @param[in] _staticRendering static rendering information 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    void AddStaticRendering (const StaticRendering& _staticRendering);

    /** Getter: number of the vertices.
    @return number of the vertices */
    inline UINT GetNumVertices () const {
        return m_NumVertices;
    }

    /** Getter: number of the static vertices.
    @return number of the static vertices */
    inline UINT GetNumStaticVertices () const {
        return m_NumStaticVertices;
    }

    /** Getter: number of the indices.
    @return number of the indices */
    inline UINT GetNumIndices () const {
        return m_NumIndices;
    }

    /** Getter: number of the static indices.
    @return number of the static indices. */
    inline UINT GetNumStaticIndices () const {
        return m_NumStaticIndices;
    }

    /** Checks if vertex cache is empty.
    @return @c true vertex cache is empty. @c false otherwise */
    inline bool IsEmpty () const {
        return m_IsEmpty;
    }

    /** Sends vertices and indices to the video card to render.
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid 
        - @c ERRC_API_CALL */
    void Render ();

private:
    /** Sets vertex buffer.
    @param[in] _id vertex buffer ID. If the value is INVALID_ID then 
    the dynamic buffer is set. Otherwise the static buffer is set.
    @param[in] _isForStatic is the dynamic buffer for static rendering 
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL */
    void SetVertexBuffer (UINT _Id, bool _IsForStatic = false);

    /** Sets index buffer.
    @param[in] _id index buffer ID. If the value is INVALID_ID then 
    the dynamic buffer is set. Otherwise the static buffer is set.
    @param[in] _isForStatic is the dynamic buffer for the static rendering
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL */
    void SetIndexBuffer (UINT _Id, bool _IsForStatic = false);

    /** Gets the DirectX primitive type.
    @param[in] _type primitive type
    @return DirectX primitive type */
    D3DPRIMITIVETYPE GetDirect3DType (PRIMITIVETYPE _Type);

    /** Gets the number of the primitives.
    @param[in] _type primitive type
    @param[in] _number number of the vertices if it does not have indices.
    Otherwise the number of the indices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_INVALID_PARAMETER invalid primitive type

    @return the number of the primitives */
    UINT GetPrimitiveCount (PRIMITIVETYPE _type, UINT _number);

    /** Joins the static rendering information if possible.
    @param[in] _staticRendering static rendering information
    @return @c true the static rendering has been joined. @c false otherwise */
    bool JoinStaticRendering (const StaticRendering& _staticRendering);

    /** Renders primitives.
    @param[in] _startVertex at which vertex the rendering should start
    @param[in] _startIndex at which index the rendering should start
    @param[in] _numPrimitives the number of the primitives
    @param[in] _isForStatic is the dynamic buffer for the static rendering
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_API_CALL */
    void Render (UINT _StartVertex, UINT _StartIndex, 
                 UINT _NumPrimitives, bool _IsIndexed, bool _IsForStatic = false);

    LPDIRECT3DVERTEXBUFFER9 m_vb;   /**< Vertex buffer. */
    UINT m_NumVertices;             /**< Number of the vertices. */
    UINT m_VertexSize;              /**< Size of the vertex in bytes. */
    VERTEXFORMATTYPE m_VertexFormat;         /**< Vertex format. */
    PRIMITIVETYPE m_Type;           /**< Primitive type. */
    IDirect3DVertexDeclaration9* m_VertexDecl;      /**< Vertex format declaration. */
    UINT m_SkinId;                  /**< Skin ID. */
    UINT m_MaxVertices;             /**< Maximum number of the vertices. */
    LPDIRECT3DINDEXBUFFER9 m_ib;    /**< Index buffer. */
    UINT m_NumIndices;              /**< Number of the indices. */
    UINT m_MaxIndices;              /**< Maximum number of the indices. */

    UINT m_Id;                      /**< Vertex cache ID. */
    bool m_IsEmpty;                 /**< Cache is empty. */
    bool m_IsFlushed;               /**< Cache has been flushed. */
    bool m_IsAddingWithoutIndices;  /**< Adding vertices without indices. */

    std::vector<StaticRendering> m_StaticRendering; /**< Static rendering informations. */
    LPDIRECT3DVERTEXBUFFER9 m_StaticVertexBuffer;   /**< Vertices for the static rendering. */
    UINT m_NumStaticVertices;                       /**< Number of the vertices for the static rendering. */
    LPDIRECT3DINDEXBUFFER9 m_StaticIndexBuffer;     /**< Indices for the static rendering. */
    UINT m_NumStaticIndices;                        /**< Number of the indices for the static rendering. */
    
    VertexCacheManager* m_vcm;      /**< Pointer to the parent vertex cache manager. */
};