/** @file Ms3dModel.h */
#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <cstdio>
#include <cstring>
#include "../include/Log.h"
#include "../include/Engine.h"
#include "../include/RenderDevice.h"
#include "../include/ErrorMessage.h"

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")

#ifdef _DEBUG
    #pragma comment (lib, "lib/Debug/Log.lib")
    #pragma comment (lib, "lib/Debug/RendererLoader.lib")
    #pragma comment (lib, "lib/Debug/ErrorMessage.lib")
#else
    #pragma comment (lib, "lib/Log.lib")
    #pragma comment (lib, "lib/RendererLoader.lib")
    #pragma comment (lib, "lib/ErrorMessage.lib")
#endif

#pragma pack (push, packing)
#pragma pack (1)

/** Contains the structures of the model data. */
namespace ms3d {
    /** Model vertex. */
    struct VERTEX {
        UCHAR Flag;         /**< Unimportant. */
        D3DXVECTOR3 Vertex; /**< Vertex coordinates. */
        char BoneId;        /**< ID of the bone. */
        UCHAR Unused;       /**< Unimportant. */
    };

    /** Model triangle. */
    struct TRIANGLE {
        USHORT Flag;            /**< Unimportant. */
        short JointIndex[3];   /**< Index of the joint which has the triangle's vertex. */
        USHORT VertIndex[3];    /**< Indices of the 3 vertices. */
        D3DXVECTOR3 Normal[3];  /**< Normals for each vertex. */
        float TexCoord[2][3];   /**< Texture coordinates for each vertex. */
        UCHAR Smoothing;        /**< Smoothing */
        UCHAR Group;            /**< Group */
    };

    /** Model mesh. */
    struct MESH {
        UCHAR Flag;             /**< Unimportant. */
        char Name[32];          /**< Mesh name. */
        USHORT NumTriangles;    /**< Number of triangles in the mesh. */
        USHORT* TriIndex;       /**< Indices of the triangles. */
        char Material;          /**< Material ID. -1 - no material. */

        MESH ();    /**< Constructor. */
        ~MESH ();   /**< Destructor. */
    };

    /** Model material. */
    struct MATERIAL {
        char Name[32];          /**< Material name. */
        D3DCOLORVALUE Ambient;  /**< Ambient color. */
        D3DCOLORVALUE Diffuse;  /**< Diffuse color. */
        D3DCOLORVALUE Specular; /**< Specular color. */
        D3DCOLORVALUE Emissive; /**< Emissive color. */
        float Shininess;        /**< Specular color hardness. */
        float Transparency;     /**< Transparency. */
        char Mode;              /**< Mode. */
        char Texture[128];      /**< texture filename. */
        char Alpha[128];        /**< alpha texture filename. */
    };

    /** Model keyframe. */
    struct KEYFRAME {
        float Time;     /**< Time of the keyframe. */
        float Param[3]; /**< Transformation values. */
    };

    /** Model joint. */
    struct JOINT {
        UCHAR Flag;                 /**< Unimportant. */
        char Name[32];              /**< Joint name. */
        char Parent[32];            /**< Parent joint name. */
        float Rotation[3];          /**< Rotation. */
        float Position[3];          /**< Position. */
        USHORT NumRotFrames;        /**< Number of the rotation keyframes. */
        USHORT NumTransFrames;      /**< Number of the translation keyframes. */
        KEYFRAME* RotKeyFrame;      /**< Rotation keyframes. */
        KEYFRAME* TransKeyFrame;    /**< Translation keyframes. */

        VERTEX* Vertex;             /**< Array of the vertices which belongs to the joint. */
        VERTEX* Transformed;        /**< Array of the transformed vertices. */
        D3DXVECTOR3* Normal;        /**< Array of the normals. */
        D3DXVECTOR3* RotatedNormal; /**< Array of the transformed normals. */
        USHORT NumVertices;         /**< Number of the vertices in the array. */
        short ParentId;             /**< Parent joint ID. */
        D3DXMATRIX Local;           /**< Local matrix. */
        D3DXMATRIX Absolute;        /**< Absolute matrix. */
        D3DXMATRIX Final;           /**< Final matrix. */
        USHORT CurRotFrame;         /**< Current rotation keyframe. */
        USHORT CurTransFrame;       /**< Current transformation keyframe. */

        JOINT ();   /** Constructor. */
        ~JOINT ();  /** Destructor. */
    };
}

#pragma pack (pop, packing)

/** A ms3d model data and methods to manipulate it. */
class Ms3dModel {
public:
    /** Constructor. */
    Ms3dModel ();

    /** Constructor.
    @param[in] _log a pointer to the log */
    Ms3dModel (LogManager* _log);

    /** Destructor. */
    ~Ms3dModel ();

    /** Setter: log.
    @param[in] _log a pointer to log */
    void SetLog (LogManager* _log);

    /** Loads the model.
    @param[in] _filename the name of the ms3d model file
    @exception ErrorMessage 

    - Possible error codes:
        - @c ERRC_FILE_NOT_FOUND the specified file does not exist
        - @c ERRC_BAD_FILE the file is either corrupted or not *.ms3d format
        - @c ERRC_OUT_OF_MEM not enough memory to load the file */
    void Load (const char* _filename);

    /** Unloads the model. */
    void Unload ();

    /** Getter: vertices.
    @param[in] _jointId joint ID
    @Exception ErrorMessage

    -Possible error codes:
        - @c ERRC_OUT_OF_RANGE invalid joint ID

    @return model vertices */
    ms3d::VERTEX* GetVertices (short _jointId);
    
    /** Getter: triangles.
    @return model triangles */
    ms3d::TRIANGLE* GetTriangles ();
    
    /** Getter: meshes.
    @return model meshes */
    ms3d::MESH* GetMeshes ();

    /** Getter: materials.
    @return model materials */
    ms3d::MATERIAL* GetMaterials ();
    
    /** Getter: joints.
    @return model joints */
    ms3d::JOINT* GetJoints ();

    /** Getter: filename.
    @return filename of the model */
    const char* GetFilename () const;

    /** Checks if model is empty.
    @return @c true model is empty
    @return @c false model is not empty */
    bool IsEmpty () const;

    /** Checks if model is loaded.
    @return @c true model is loaded
    @return @c false model is not loaded */
    inline bool IsLoaded () const;

    /** Animates the model.
    @param[in] _speed the animation speed. 1.0f is a normal speed.
    @param[in] _startTime the time of the animation where it has to start
    @param[in] _endTime the time of the animation where it has to stop
    @param[in] _loop whether loop the animation or not */
    void Animate (float _speed, float _startTime, float _endTime, bool _loop);

    /** Checks if model's animation is completed.
    If animation is looping, it returns always false.
    @return @c true model's animation is completed. @c false otherwise. */
    bool IsAnimationCompleted () const;

    /** Renders the model.
    @param[in] _device a pointer to the renderer
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid
        - @c ERRC_API_CALL */
    void Render (RenderDevice* _device);

    /** Scales the model.
    @param[in] _x x axis scale value
    @param[in] _y y axis scale value
    @param[in] _z z axis scale value */
    void Scale (float _x, float _y, float _z);

    /** Translates the model.
    @param[in] _x x axis translation
    @param[in] _y y axis translation
    @param[in] _z z axis translation */
    void Translate (float _x, float _y, float _z);

    /** Rotates the model.
    Values are radians.
    @param[in] _x x axis rotation
    @param[in] _y y axis rotation
    @param[in] _z z axis rotation */
    void Rotate (float _x, float _y, float _z);

    /** Clears scale, translation and rotation. */
    void ClearTransformations ();

    /** Getter: bounds.
    @param[out] _min minimum x, y and z coordinates values
    @param[out] _max maximum x, y and z coordinates values */
    void GetBounds (float(&_min)[3], float(&_max)[3]);

private:
    /** Updates the bounds of the model. */
    void UpdateBounds ();

    /** Loads the vertices.
    @param[in,out] _modelData a pointer to model file data
    @param[out] _vertex loaded vertices
    @param[out] _numVertices number of the loaded vertices
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory to load vertices
        - @c ERRC_BAD_FILE the ms3d model file is corrupted */
    void LoadVertices (char*& _modelData, ms3d::VERTEX*& _vertex, USHORT& _numVertices);
    
    /** Loads the triangles.
    @param[in] _modelData [in,out] a pointer to model file data
    @exception ErrorMessage 
    
    -Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory to load triangles */
    void LoadTriangles (char*& _modelData);

    /** Loads the meshes.
    @param[in] _modelData [in,out] a pointer to model file data
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory to load meshes */
    void LoadMeshes (char*& _modelData);

    /** Loads the materials.
    @param[in] _modelData [in,out] a pointer to model file data
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory to load materials */
    void LoadMaterials (char*& _modelData);

    /** Loads the joints.
    @param[in] _modelData [in,out] a pointer to model file data
    @exception ErrorMessage 
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory to load joints */
    void LoadJoints (char*& _modelData);

    /** Setter: filename
    @param[in] _filename filename of the model */
    void SetFilename (const char* _filename);

    /** Creates the rotation matrix from the euler angles. 
    @param[in] _vector 3 float values representing euler angles
    @return rotation matrix */
    D3DXMATRIX CreateRotationMatrix (float _vector[3]) const;

    /** Creates the rotation quaternion from the euler angles. 
    @param[in] _x x euler angle
    @param[in] _y y euler angle
    @param[in] _z z euler angle
    @return rotation quaternion */
    D3DXQUATERNION CreateQuaternion (float _x, float _y, float _z) const;

    /** Inverse rotation.
    @param[in] _vector [in,out] vector which will be transformed
    @param[in] _matrix rotation matrix */
    void InvRotate (D3DXVECTOR3& _vector, const D3DXMATRIX& _matrix) const;

    /** Inverse translation.
    @param[in] _vector [in,out] vector which will be transformed
    @param[in] _matrix translation matrix */
    void InvTranslate (D3DXVECTOR3& vector, const D3DXMATRIX& matrix) const;

    /** Updates empty status. */
    inline void UpdateEmptyStatus ();

    char m_Filename[128];   /**< Filename of the model. */
    bool m_IsEmpty;         /**< Empty status flag. */
    LogManager* m_Log;      /**< Pointer to log. */

    //ms3d::VERTEX* m_InitialVertex;  /**< The array of the initial model vertices. */
    //USHORT m_NumInitialVertices;    /**< Number of the initial vertices. */
    float m_StartTime;      /**< Animation's start time. */
    float m_EndTime;        /**< Animation's end time. */
    float m_LastTime;       /**< Last time of the animation. */
    DWORD m_TimerLastClock; /**< Animation's last clock time. */
    bool m_IsAnimationCompleted;    /**< If animation is completed then it is true. */

    ms3d::VERTEX* m_Vertex;         /**< The array of the model vertices which do not belong to any bone. */
    ms3d::VERTEX* m_Transformed;    /**< The array of the transformed vertices which do not belong to any bone. */
    D3DXVECTOR3* m_Normal;          /**< The array of the vertices' without bone normals. */
    D3DXVECTOR3* m_RotatedNormal;   /**< The array of the vertices' without bone transformed normals. */
    USHORT m_NumVertices;           /**< Number of the vertices. */

    ms3d::TRIANGLE* m_Triangle; /**< The array of the model triangles. */
    USHORT m_NumTriangles;      /**< Number of the triangles. */

    ms3d::MESH* m_Mesh;     /**< The array of the model meshes. */
    USHORT m_NumMeshes;     /**< Number of the meshes. */

    ms3d::MATERIAL* m_Material; /**< The array of the model materials. */
    USHORT m_NumMaterials;      /**< Number of the materials. */

    ms3d::JOINT* m_Joint;   /**< The array of the model joints. */
    USHORT m_NumJoints;     /**< Number of the joints. */

    float m_Min[3];     /**< The model min bounds. */
    float m_Max[3];     /**< The model max bounds. */

    vs3d::UUVERTEX* m_VertexData;   /**< The vertices for model rendering. */
    std::vector<UINT> m_SkinId;     /**< The array of the model skins. */

    MATRIX44 m_Translation;     /**< The model translation matrix. */
    MATRIX44 m_Rotation;        /**< The model rotation matrix. */
    MATRIX44 m_Scale;           /**< The model scale matrix. */
};