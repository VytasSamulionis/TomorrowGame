/** @file Terrain.h */

/** TODO: Complete the implementation of the geomipmap. */

#pragma once

#include <Windows.h>
#include <cmath>
#include "../include/Log.h"
#include "../include/TgaImage.h"
#include "../../TerrainEngineLoader/include/TerrainEngine.h"

#ifdef _DEBUG
    #pragma comment (lib, "lib/Debug/Log.lib")
    #pragma comment (lib, "lib/Debug/RendererLoader.lib")
    #pragma comment (lib, "lib/Debug/ErrorMessage.lib")
#else
    #pragma comment (lib, "lib/Log.lib")
    #pragma comment (lib, "lib/RendererLoader.lib")
    #pragma comment (lib, "lib/ErrorMessage.lib")
#endif

/** Maximum number of the tiles. */
#define MAX_TILE_NUM 4

/** Number of the different index variants. */
#define NUM_INDEX_VARIANTS 16

/** Terrain light mode. */
enum TERRAIN_LIGHT_MODE {
    TLM_HEIGHT_BASED,   /**< Height based lighting. */
    TLM_LIGHTMAP,       /**< Lightmap lighting. */
    TLM_SLOPE_LIGHT     /**< Slope lighting. */
};

/** Tile region. */
struct TileRegion {
    UCHAR Lowest;   /**< Lowest. */
    UCHAR Optimal;  /**< Optimal. */
    UCHAR Highest;  /**< Highest. */
};

/** Terrain patch. */
struct Patch {
    float Distance;     /**< Distance to the camera. */
    UCHAR Lod;          /**< Patch level of the detail. */
    bool IsCulled;      /**< Patch is culled. */
    // data
    void* VertexData;   /**< Patch vertex data. */
    /*WORD* IndexData;
    UINT NumIndices;*/
    PatchBounds Bounds; /**< Bounds of the patch. */
};

/** Patch neighbor. */
struct Neighbor {
    bool Left;      /**< Left neighbor. */
    bool Right;     /**< Right neighbor. */
    bool Top;       /**< Top neighbor. */
    bool Bottom;    /**< Bottom neighbor. */
};

/** Patch indices information. */
struct IndicesInfo {
    UINT Offset;    /**< Offset. */
    UINT Number;    /**< Number of the indices. */
};

/** Terrain. */
class Terrain: public ITerrain {
public:
    /** Constructor.
    @param[in] _log pointer to the LogManager
    @param[in] _device pointer to the RenderDevice */
    Terrain (LogManager* _log, RenderDevice* _device);
    
    /** Constructor.
    @param[in] _device pointer to the RenderDevice */
    Terrain (RenderDevice* _device);

    /** Destructor. */
    ~Terrain ();

    /** Initializes terrain.
    @param[in] _patchSize the size of the patch
    @param[in] _fvf vertex format.
    Supported vertex formats:
    - FVF_UL
    - FVF_UL2

    @param[in] _skinId skin ID 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_NOT_READY heightmap is not loaded
        - @c ERRC_UNKNOWN_FVF invalid vertex format
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_API_CALL*/
    void Init (UCHAR _patchSize, VERTEXFORMATTYPE _vft, UINT _skinId);

    /** Releases terrain resources. */
    void Shutdown ();

    /** Updates the terrain.
    @param[in] _cameraPos camera position
    @param[in] _view view matrix
    @param[in] _proj projection matrix 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_NOT_READY either heightmap is not loaded or terrain is not initialized
        - @c ERRC_OUT_OF_RANGE */
    void Update (VECTOR3 _cameraPos, MATRIX44 _view, MATRIX44 _proj);

    /** Renders terrain.
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid
        - @c ERRC_API_CALL
        - @c ERRC_OUT_OF_RANGE */
    void Render ();

    /** Checks if patch is picked.
    @param[in] _rayOrigin origin of the ray
    @param[in] _rayDirection direction of the ray
    @param[in] _x patch x coordinate
    @param[in] _z patch z coordinate
    @param[out] _intersectedParts intersected parts
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory

    @return @c true patch is picked. @c false otherwise */
    bool IsPatchPicked (VECTOR3 _rayOrigin, VECTOR3 _rayDirection, 
                        UINT _x, UINT _z, std::vector<PatchBounds>& _intersectedParts);
    
    /** Checks if vertex is picked.
    @param[in] _rayOrigin origin of the ray
    @param[in] _rayDirection direction of the ray
    @param[in] _bounds bounds of the triangle
    @param[in] _viewPoint view point
    @param[out] _vertex coordinates of the vertices
    @param[out] _distance distance to the intersection
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_UNKNOWN_FVF invalid vertex format

    @return @c true vertex is picked. @c false otherwise */
    bool IsVertexPicked (VECTOR3 _rayOrigin, VECTOR3 _rayDirection, 
                         PatchBounds _bounds, VECTOR3 _viewPoint, 
                         POINT& _vertex, float& _distance);
    
    /** Updates terrain data. 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_UNKNOWN_FVF */
    void UpdateData ();
    
    /** Enables log.
    @param[in] _log pointer to LogManager. */
    void EnableLog (LogManager* _log);

    /** Creates new heightmap.
    @param[in] _size size of the heightmap
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    void NewHeightmap (UINT _size);

    /** Loads heightmap.
    @param[in] _heightmapFile path to the heightmap file
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file does not exist
        - @c ERRC_BAD_FILE file is corrupted
        - @c ERRC_OUT_OF_MEM not enough memory */
    void LoadHeightmap (const char* _heightmapFile);
    
    /** Saves heightmap.
    @param[in] _heightmapFile path of the file 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file cannot be opened
        - @c ERRC_BAD_FILE cannot write to file */
    void SaveHeightmap (const char* _heightmapFile);
    
    /** Releases heightmap resources. */
    void UnloadHeightmap ();
    
    /** Getter: scaled height.
    @param[in] _x coordinate x of the terrain
    @param[in] _z coordinate z of the terrain
    @return scaled height
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters */
    float GetScaledHeight (UINT _x, UINT _z) const;
    
    /** Getter: height.
    @param[in] _x coordinate x of the terrain
    @param[in] _z coordinate z of the terrain
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters

    @return height */
    UCHAR GetHeight (UINT _x, UINT _z) const;
    
    /** Setter: height.
    @param[in] _x coordinate x of the terrain
    @param[in] _z coordinate z of the terrain
    @param[in] _height terrain height 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters */
    void SetHeight (UINT _x, UINT _z, UCHAR _height);
    
    /** Setter: terrain scale.
    @param[in] _scaleX terrain x scale
    @param[in] _scaleY terrain y scale
    @param[in] _scaleZ terrain z scale 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_INVALID_PARAMETER scale is a negative value */
    void SetScale (float _scaleX, float _scaleY, float _scaleZ);

    /** Getter: terrain size.
    @return terrain size */
    inline UINT GetSize () const {
        return m_HeightmapSize;
    }
    
    /** Enables terrain brute force rendering.
    @param[in] _enable brute force should be enabled */
    inline void EnableBruteForce (bool _enable) {
        m_IsBruteForceEnabled = _enable;
    }

    /** Checks if brute force is enabled.
    @return @c true brute force is enabled. @c false otherwise */
    inline bool IsBruteForceEnabled () const {
        return m_IsBruteForceEnabled;
    }

    /** Getter: patches per side.
    @return patches per side */
    inline UCHAR GetPatchesPerSide () const {
        return m_PatchesPerSide;
    }

    /** Checks if point is visible.
    @param[in] _x terrain x coordinate
    @param[in] _z terrain z coordinate
    @return @c true point is visible. @c false otherwise */
    inline bool IsPointVisible (UINT _x, UINT _z) const {
        UINT patchX = _x / (m_PatchSize - 1);
        UINT patchZ = _z / (m_PatchSize - 1);
        return !m_Patch[patchZ * m_PatchesPerSide + patchX].IsCulled;
    }

    /** Getter: scale.
    @param[in] _i coordinate number (x = 0, y = 1, z = 2)
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE invalid parameter

    @return scale */
    inline float GetScale (UCHAR _i) {
        if (_i >= 3) {
            THROW_ERROR (ERRC_OUT_OF_RANGE);
        }
        return m_Scale[_i];
    }

    /** Getter: heightmap file.
    @param[out] _filename heightmap file */
    inline void GetHeightmapFile (char (& _filename)[MAX_PATH]) {
        strcpy (_filename, m_HeightmapFile);
    }

    /** Loads lightmap.
    Lightmap size must be equal to heightmap size.
    @param[in] _filename lightmap's filename
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file does not exist
        - @c ERRC_BAD_FILE file file is corrupted
        - @c ERRC_OUT_OF_MEM not enough memory */
    void LoadLightmap (const char* _filename);

    /** Saves lightmap.
    @param[in] _filename lightmap's filename 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file cannot be opened
        - @c ERRC_BAD_FILE cannot write to file */
    void SaveLightmap (const char* _filename);

    /** Releases lightmap resources. */
    void UnloadLightmap ();

    /** Getter: lightmap file.
    @param[out] _filename lightmap file */
    inline void GetLightmapFile (char (& _filename)[MAX_PATH]) {
        strcpy (_filename, m_LightmapFile);
    }

    /** Getter: brightness.
    @param[in] _x terrain x coordinate
    @param[in] _z terrain z coordinate
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters

    @return brightness */
    UCHAR GetBrightness (UINT _x, UINT _z) const;

    /** Setter: brightness.
    @param[in] _x terrain x coordinate
    @param[in] _z terrain z coordinate
    @param[in] _value brightness value
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters */
    void SetBrightness (UINT _x, UINT _z, UCHAR _value);

    /** Setter: light's color.
    @param[in] _red red component
    @param[in] _green green component
    @param[in] _blue blue component */
    void SetLightColor (UCHAR _red, UCHAR _green, UCHAR _blue);

    /** Setter: slope lighting.
    @param[in] _lightDirX light's x direction
    @param[in] _lightDirZ light's z direction
    @param[in] _minBrightness minimum brightness
    @param[in] _maxBrightness maximum brightness
    @param[in] _softness light's softness 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    void SetSlopeLighting (int _lightDirX, int _lightDirZ,
                              UINT _minBrightness, UINT _maxBrightness,
                              float _softness);

    /** Setter: height based lighting. */
    inline void SetHeightBasedLighting () {
        m_LightMode = TLM_HEIGHT_BASED;
    }
    
    /** Setter: lightmap lighting.
    @param[in] _filename lightmap's filename
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file does not exist
        - @c ERRC_BAD_FILE file file is corrupted
        - @c ERRC_OUT_OF_MEM not enough memory */
    inline void SetLightmapLighting (const char* _filename) {
        LoadLightmap (_filename);
    }

    /** Setter: density of the detail map.
    @param[in] _factor density */
    inline void SetDetailMapDensity (UINT _factor) {
        m_RepeatDetailMap = _factor;
    }

    /** Loads texture tile.
    @param[in] _level tile level
    @param[in] _filename texture's filename 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE level is incorrect
        - @c ERRC_BAD_FILE cannot load texture */
    void LoadTextureTile (UINT _level, const char* _filename);

    /** Releases tile resources.
    @param[in] _level tile level
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE level is incorrect */
    void UnloadTextureTile (UINT _level);

    /** Releases all tiles resources. */
    void UnloadAllTextureTiles ();

    /** Generates the texture map.
    Tiles should be loaded before calling this method.
    @param[in] _filename a file where the texture will be save
    @param[in] _size size of the texture
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE level is incorrect */
    void GenerateTextureMap (const char* _filename, UINT _size);

    /** Getter: terrain color.
    @param[in] _x terrain x coordinate
    @param[in] _z terrain z coordinate
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters

    @return ARGB format terrain color */
    DWORD GetColor (UINT _x, UINT _z);
protected:
    /** Getter: texture presence.
    @param[in] _height terrain height
    @param[in] _level tile level
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE level is incorrect 

    @return texture presence */
    float GetTexPresence (UCHAR _height, UINT _level);

    /** Getter: texture coordinates.
    @param[in] _texture texture
    @param[out] _x coordinate x
    @param[out] _y coordinate y */
    void GetTexCoords (const TgaImage& _texture, UINT& _x, UINT& _y);

    /** Interpolates the height.
    @param[in] _x terrain's x coordinate
    @param[in] _z terrain's z coordinate
    @param[in] _ratio ratio */
    UCHAR InterpolateHeight (UINT _x, UINT _z, float _ratio);

    /** Updates regions. */
    void UpdateRegions ();

    /** Loads the data.
    @param[in] _filename the stored data location
    @param[out] _data loaded data
    @param[out] _size size of the loaded data 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file does not exist
        - @c ERRC_BAD_FILE file is corrupted
        - @c ERRC_OUT_OF_MEM not enough memory*/
    void LoadData (const char* _filename, UCHAR*& _data, UINT& _size);

    /** Unloads data.
    @param[out] _data the data which has to be unloaded
    @param[out] _size size of the data */
    void UnloadData (UCHAR*& _data, UINT& _size);

    /** Saves the data.
    @param[in] _filename the file where to save the data
    @param[in] _data data to save
    @param[in] _size size of the data 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file cannot be opened
        - @c ERRC_BAD_FILE cannot write to file */
    void SaveData (const char* _filename, UCHAR* _data, UINT _size);

    /** Releases patch bounds.
    @param[in] _bounds patch bounds */
    void ReleasePatchBounds (PatchBounds _bounds);

    /** Checks if ray intersects patch.
    @param[in] _rayOrigin origin of the ray
    @param[in] _rayDirection direction of the ray
    @param[in] _bounds patch bounds
    @param[out] _intersectedParts intersected parts 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory

    @return @c true ray intersects patch. @c false otherwise */
    bool IsRayIntersectingPatch (VECTOR3 _rayOrigin, VECTOR3 _rayDirection, 
                                 PatchBounds _bounds, std::vector<PatchBounds>& _intersectedParts);

    /** Initializes patch data.
    @param[in] _patchX x coordinate of the patch
    @param[in] _patchZ z coordinate of the patch 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_UNKNOWN_FVF unknown vertex format
        - @c ERRC_OUT_OF_RANGE */
    void InitPatchData (UINT _patchX, UINT _patchZ);

    /** Checks if patch is in frustum.
    @param[in] _x coordinate x
    @param[in] _y coordinate y
    @param[in] _z coordinate z
    @return @c true patch is in frustum. @c false otherwise */
    bool IsPatchInFrustum (float _x, float _y, float _z);

    /** Generates patch part bounds.
    @param[in] _x coordinate x
    @param[in] _z coordinate z
    @param[in] _size size of the patch
    @return patch part bounds */
    PatchBounds GeneratePatchPartBounds (float _x, float _z, float _size);

    /** Generates patch parts.
    @param[out] _part patch parts
    @param[in] _x coordinate x
    @param[in] _z coordinate z
    @param[in] _size size of the part 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    void GeneratePatchParts (PatchBounds& _part, float _x, float _z, float _size);

    /** Generate indices of the patch. 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    void GenerateIndices ();

    /** Generates indices for the fan.
    It is without left vertex.
    @param[in] _center center of the fan
    @param[in] _size size of the fan
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory 
    */
    void AddIndicesNoLeft (WORD _center, UINT _size);

    /** Generates indices for the fan.
    It is without right vertex.
    @param[in] _center center of the fan
    @param[in] _size size of the fan 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    void AddIndicesNoRight (WORD _center, UINT _size);

    /** Generates indices for the fan.
    It is without top vertex.
    @param[in] _center center of the fan
    @param[in] _size size of the fan 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    void AddIndicesNoTop (WORD _center, UINT _size);

    /** Generates indices for the fan.
    It is without bottom vertex.
    @param[in] _center center of the fan
    @param[in] _size size of the fan 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    void AddIndicesNoBottom (WORD _center, UINT _size);

    /** Generates indices for the fan.
    @param[in] _center center of the fan
    @param[in] _size size of the fan 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    void AddIndices (WORD _center, UINT _size);

    float m_Frustum[6][4];          /**< View frustum. */
    Patch* m_Patch;                 /**< Terrain patches. */
    std::vector<WORD> m_Indices;    /**< Indices. */
    UINT m_IndexBufferId;           /**< Index buffer ID. */
    /** Information of the index buffers. */
    std::vector<IndicesInfo> m_IndexBufferOffsets;  
    UCHAR m_PatchSize;              /**< Patch size. */
    UCHAR m_PatchesPerSide;         /**< Number of patches per side. */
    UCHAR m_MaxLod;                 /**< Maximum level of detail. */
    UINT m_VertexSize;              /**< Vertex size. */
    VERTEXFORMATTYPE m_VertexFormat;/**< Vertex format. */
    UINT m_SkinId;                  /**< Skin ID. */
    bool m_IsBruteForceEnabled;     /**< Brute force rendering is enabled. */

    /** Tiles for the procedural texture generation. */
    TgaImage m_Tile[MAX_TILE_NUM];
    /** Tiles' regions for the procedural texture generation. */
    TileRegion m_Region[MAX_TILE_NUM];
    UCHAR m_NumTiles;   /**< Number of the tiles. */

    bool m_IsUpdated;   /**< Is terrain updated. */

    UCHAR m_LightColor[3];  /**< Color of the light. */

    // heightmap
    float m_Scale[3];       /**< Terrain scale factors. */
    UINT m_HeightmapSize;   /**< Size of the heightmap. */
    char m_HeightmapFile[MAX_PATH]; /**< Heightmap filename. */ 
    UCHAR* m_Heightmap;     /**< Heightmap data. */

    // lightmap
    UINT m_LightmapSize;    /**< Lightmap size. */
    char m_LightmapFile[MAX_PATH];  /**< Lightmap filename. */
    UCHAR* m_Lightmap;      /**< Lightmap data. */

    // slope light
    int m_LightDirectionX;  /**< Light's x directio. */
    int m_LightDirectionZ;  /**< Light's z direction. */
    float m_MinBrightness;  /**< Minimum brightness. */
    float m_MaxBrightness;  /**< Maximum brightness. */
    float m_LightSoftness;  /**< Light's softness. */

    UCHAR m_RepeatDetailMap;    /**< Detail map density. */
    
    TERRAIN_LIGHT_MODE m_LightMode; /**< Current light mode. */

    LogManager* m_Log;      /**< Pointer to the LogManager. */
    RenderDevice* m_Device; /**< Pointer to the RenderDevice. */
};