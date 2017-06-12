/** @file ObjModel.h */
#pragma once

#include <Windows.h>
#include <cstdio>
#include <vector>
#include "..\include\RenderDevice.h"
#include "..\include\ObjManager.h"

class ObjManager;

#ifdef _DEBUG
    /*#define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>*/
    #pragma comment (lib, "lib/Debug/RendererLoader.lib")
    #pragma comment (lib, "lib/Debug/ErrorMessage.lib")
    /*#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define new DEBUG_NEW*/
#else
    #pragma comment (lib, "lib/RendererLoader.lib")
    #pragma comment (lib, "lib/ErrorMessage.lib")
#endif

/** obj model vertex. */
struct ObjModelVertex {
    float X;    /**< Model's vertex x coordinate. */
    float Y;    /**< Model's vertex y coordinate. */
    float Z;    /**< Model's vertex z coordinate. */
};

/** obj model texture coordinates. */
struct ObjModelTexture {
    float U;    /**< Model's texture u coordinate. */
    float V;    /**< Model's texture v coordinate. */
};

/** obj model normals. */
struct ObjModelNormal {
    float X;    /**< Model's x normal. */
    float Y;    /**< Model's y normal. */
    float Z;    /**< Model's z normal. */
};

/** obj model triangle. */
struct ObjModelFace {
    UINT VertexIndex[3];                /**< 3 vertices which make the triangle. */
    std::vector<UINT> TextureIndices;   /**< Texture indices of the triangle vertices. */
    std::vector<UINT> NormalIndices;    /**< Normal indices of the triangle vertices. */
};

/** Object of the model.
A model can be combined of few objects.
This structure describes the object.
ObjModelObject::ModelData is used for rendering the object dynamically.
The vertices prepared for the rendering are restored there and
in this way the preparation for the rendering every frame is avoided.
It is done only when vertex information has been changed.*/
struct ObjModelObject {
    std::vector<ObjModelFace> Faces;    /**< Object faces (aka triangles). */
    void* ModelData;                    /**< Model data. */
    UINT NumVertices;                   /**< Number of the vertices. */
};

/** Model mesh. */
struct ObjModelMesh {
    char MaterialName[MAX_PATH];            /**< Mesh material name. */
    UINT SkinId;                            /**< Mesh skin ID. */
    std::vector<ObjModelObject> Objects;    /**< The vector of the mesh objects. @see ObjModelObject */
};

/** Model rendering information. */
struct ObjRenderingInfo {
    UINT BufferId;          /**< Static buffer ID of the model. */
    UINT StartVertex;       /**< At which vertex the rendering should start. */
    UINT PrimitiveCount;    /**< Number of the primitives. */
    UINT SkinId;            /**< Skin ID. */
    VERTEXFORMATTYPE VertexFormat;  /**< Vertex format. */
    bool IsPrepared;        /**< Is data prepared for the rendering. */
};

/** Model bounds rendering information. */
struct ObjBoundsRenderingInfo {
    ObjRenderingInfo RenderingInfo; /**< Information for the rendering. @see ObjRenderingInfo */
    UINT IndexBufferId;             /**< Static index buffer ID. */
    UINT StartIndex;                /**< At which index the rendering should start. */
};

/** Loads and manipulates loaded data of the *.obj format model. */
class ObjModel {
public:
    /** Constructor. 
    @param[in] _device A pointer to the rendering device. 
    @param[in] _manager A pointer to the ObjManager object. */
    ObjModel (RenderDevice* _device, ObjManager* _manager);

    /** Destructor. */
    ~ObjModel ();

    /** Loads the *.obj format model. 
    @param[in] _filename A filename of the model.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_FILE_NOT_FOUND specified file does not exist.
        - @c ERRC_BAD_FILE file is either corrupted or not *.obj format
        - @c ERRC_OUT_OF_MEM not enough memory to load the model
        - @c ERRC_OUT_OF_RANGE invalid ID 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure */
    void Load (const char* _filename);

    /** Unloads the model. */
    void Unload ();

    /** Prepares the model for the rendering.
    It needs to be called before the static rendering.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_BAD_FILE models without textures and with normals are not supported
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format 

    @see ObjModel::Render()
    @see ObjModel::RenderBounds() */
    void Prepare ();

    /** Renders the model using static buffers. 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL index buffer Lock() failure */
    void Render ();

    /** Renders the model using dynamic buffers.
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_BAD_FILE models without textures and with normals are not supported
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL index buffer Lock() failure */
    void RenderDynamic ();

    /** Renders the bounds of the model using static buffers.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory 
    */
    void RenderBounds ();

    /** Renders the bounds of the model using dynamic buffers.
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid
        - @c ERRC_API_CALL  */
    void RenderBoundsDynamic ();

    /** Makes the copy of the model.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory

    @return ID of the model. */
    UINT MakeCopy ();

    /** Sets the transparency of the model.
    It is only applied if vertex format includes the color. 
    @param[in] _transparency the alpha value. It must be between 0.0f and 1.0f */
    void SetTransparency (float _transparency);

    /** Translates the model. 
    @param[in] _x x axis transformation
    @param[in] _y y axis transformation
    @param[in] _z z axis transformation */
    void Translate (float _x, float _y, float _z);

    /** Translates the model on the x axis.
    @param[in] _x x axis transformation */
    void TranslateX (float _x);

    /** Translates the model on the y axis.
    @param[in] _y y axis transformation */
    void TranslateY (float _y);

    /** Translates the model on the z axis.
    @param[in] _z z axis transformation */
    void TranslateZ (float _z);

    /** Rotates the model.
    @param[in] _x x axis rotation in radians
    @param[in] _y y axis rotation in radians
    @param[in] _z z axis rotation in radians */
    void Rotate (float _x, float _y, float _z);

    /** Rotates the model on the x axis.
    @param[in] _radians x axis rotation in radians */
    void RotateAboutX (float _radians);

    /** Rotates the model on the y axis.
    @param[in] _radians y axis rotation in radians */
    void RotateAboutY (float _radians);

    /** Rotates the model on the z axis.
    @param[in] _radians z axis rotation in radians */
    void RotateAboutZ (float _radians);

    /** Scales the model.
    @param[in] _scale scale factor. It must be > 0.0f */
    void Scale (float _scale);

    /** Scales the model on the x axis.
    @param[in] _scale scale factor. It must be > 0.0f */
    void ScaleX (float _scale);

    /** Scales the model on the y axis.
    @param[in] _scale scale factor. It must be > 0.0f */
    void ScaleY (float _scale);

    /** Scales the model on the z axis.
    @param[in] _scale scale factor. It must be > 0.0f */
    void ScaleZ (float _scale);

    /** Clears the translation, rotation and scale transformations. */
    void ClearTransformations ();

    /** Sets the size of the model.
    @param[in] _size a scale factor which value must be > 0.0f */
    inline void SetSize (float _size) {
        float scale = 1.0f / m_OriginalSize * _size;
        Scale (scale);
        m_OriginalSize = _size;
    }

    /** Getter: bounds of the model. 
    @param[out] _min minimum values of the bounds
    @param[out] _max maximum values of the bounds */
    inline void GetBounds (float(& _min)[3], float(& _max)[3]) const {
        VECTOR3 minB (m_BoundMin[0], m_BoundMin[1], m_BoundMin[2]);
        VECTOR3 maxB (m_BoundMax[0], m_BoundMax[1], m_BoundMax[2]);
        minB = cml::transform_point (m_Scale * m_Rotation * m_Translation, minB);
        maxB = cml::transform_point (m_Scale * m_Rotation * m_Translation, maxB);
        //memcpy (_min, maxB.data(), sizeof(float) * 3);
        //memcpy (_max, minB.data(), sizeof(float) * 3);
        for (UINT i = 0; i < 3; i++) {
            _min[i] = minB[i];
            _max[i] = maxB[i];
        }
    }

    /** Setter: skin ID.
    @param[in] _skinId skin ID */
    inline void SetSkinId (UINT _skinId) {
        m_SkinId = _skinId;
    }

    /** Getter: skin ID.
    @return skin ID */
    inline UINT GetSkinId () const {
        return m_SkinId;
    }

    /** Getter: number of the vertices.
    @return number of the vertices */
    inline UINT GetNumVertices () const {
        return m_Vertices.size();
    }

    /** Getter: number of the texture coordinates.
    @return number of the texture coordinates */
    inline UINT GetNumTextures () const {
        return m_Textures.size();
    }

    /** Getter: number of the normals.
    @return number of the normals */
    inline UINT GetNumNormals () const {
        return m_Normals.size();
    }

    /** Getter: filename.
    @return model filename */
    inline const char* GetFilename () const {
        return m_Filename;
    }

    /** Setter: bounds color.
    @param[in] _color ARGB format color */
    inline void SetBoundsColor (DWORD _color) {
        m_BoundsColor = _color;
    }

    /** Setter: model color.
    @param[in] _color ARGB format color */
    inline void SetModelColor (DWORD _color) {
        m_ModelColor = _color;
    }

    /** Should the loader use the mtl file to get the material information. 
    @param[in] _shouldUse @c true if it should use mtl file. @c false otherwise */
    inline void UseMtlFile (bool _shouldUse) {
        m_IsMtlFile = _shouldUse;
    }
private:
    /** Updates the active mesh.
    This is used while loading the model.
    It indicates to what mesh the data should be read. 
    @param[in] _materialName active material name 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory */
    void UpdateActiveMesh (const char (&_materialName)[MAX_PATH]);

    /** Getter: mesh index by material name.
    @param[in] _materialName material name 
    @return mesh index or INVALID_ID 
    if a mesh witch such material does not exist */
    UINT GetMeshIndex (const char (&_materialName)[MAX_PATH]);

    /** Loads model triangles.
    @param[in] _line a line of the model file 
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_BAD_FILE the format of the line is incorrect
        - @c ERRC_OUT_OF_MEM not enough memory to load faces */
    void LoadFace (const char (&_line)[MAX_PATH]);

    /** Loads the material infomation of the model.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory to load materials 
        - @c ERRC_OUT_OF_RANGE invalid ID 
        - @c ERRC_API_CALL D3DXCreateTextureFromFile failure */
    void LoadMaterials ();

    /** Transforms the vertices of the model by the specified transformation.
    @param[in] _transformation transformation which should be applied */
    void MakeTransformation (const MATRIX44& _transformation);

    /** Updates the bounds. It should be called for every vertex.
    @param[in] _vertex vertex which is checked */
    void UpdateBounds (const ObjModelVertex& _vertex);

    /** Prepares the model for the static rendering.
    It is called by ObjModel::Prepare() method.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_BAD_FILE models without textures and with normals are not supported
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format */
    void PrepareModelRendering ();

    /** Prepares the bounds for the static rendering.
    It is called by ObjModel::Prepare() method.
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format */
    void PrepareBoundsRendering ();

    /** Setter: bounds vertex information.
    @param[out] _vertex the vertex of the bounds
    @param[in] _x the x coordinate of the vertex
    @param[in] _y the y coordinate of the vertex
    @param[in] _z the z coordinate of the vertex */
    inline void SetBoundsVertex (vs3d::ULCVERTEX& _vertex, float _x, float _y, float _z) {
        _vertex.X = _x;
        _vertex.Y = _y;
        _vertex.Z = _z;
        _vertex.Color = m_BoundsColor;
    }

    RenderDevice* m_Device;         /**< A pointer to the RenderDevice. */
    char m_Filename[MAX_PATH];      /**< A filename of the model. */
    char m_MtlFilename[MAX_PATH];   /**< A *.mtl filename of the model. */
    bool m_IsMtlFile;               /**< Does the mtl file information should be used. */
    UINT m_SkinId;                  /**< Skin ID of the model. */
    float m_OriginalSize;           /**< A scale factor which transforms the 
                                        model to the original size. */
    std::vector<ObjModelVertex> m_Vertices;     /**< The vertices of the model. */
    std::vector<ObjModelTexture> m_Textures;    /**< The texture coordinates of the model. */
    std::vector<ObjModelNormal> m_Normals;      /**< The normals of the model. */
    std::vector<ObjModelMesh> m_Meshes;         /**< The meshes of the model. */
    UINT m_ActiveMesh;              /**< The active mesh index. */
    DWORD m_ModelColor;             /**< The color of the model. */
    DWORD m_BoundsColor;            /**< The color of the bounds. */
    float m_BoundMax[3];            /**< Maximum values of the bounds. */
    float m_BoundMin[3];            /**< Minimum values of the bounds. */
    bool m_FirstTimeBoundsUpdate;   /**< Is the bounds updating the first time. */
    bool m_IsTexture;               /**< Does the model has texture coordinates. */
    bool m_IsNormal;                /**< Does the model has normals. */
    MATRIX44 m_Scale;               /**< Scale transformation matrix. */
    MATRIX44 m_Translation;         /**< Translation transformation matrix. */
    MATRIX44 m_Rotation;            /**< Rotation transformation matrix. */

    std::vector<ObjRenderingInfo> m_RenderingInfo;  /**< The rendering information of the model. */
    ObjBoundsRenderingInfo m_BoundsInfo;    /**< The rendering information of the bounds. */
    ObjManager* m_Manager;          /**< A pointer to ObjManager object. */

    bool m_IsOutdated;  /**< Does the model rendering vertices should be updated.
                            @see ObjModelObject::ModelData */
};