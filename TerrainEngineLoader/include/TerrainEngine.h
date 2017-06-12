/** @file TerrainEngine.h */

#pragma once

#include <Windows.h>
#include <vector>
#include "../include/RenderDevice.h"
#include "../include/ErrorMessage.h"
#include "../include/Log.h"

/** Terrain patch bounds. */
struct PatchBounds {
    float Min[3];   /**< Minimum bounds values. */
    float Max[3];   /**< Maximum bounds values. */
    PatchBounds* Children;  /**< Next bounds. */
    /** Constructor. */
    PatchBounds () {
        Children = NULL;
    }
};

/** Terrain sky box. */
class ISkyBox {
public:
    /** Initializes the skybox.
    @param[in] _x x position
    @param[in] _y y position
    @param[in] _z z position
    @param[in] _size size of the box
    @param[in] _topSkinId skin ID of the box top
    @param[in] _bottomSkinId skin ID of the box bottom
    @param[in] _leftSkinId skin ID of the left box wall
    @param[in] _rightSkinId skin ID of the right box wall
    @param[in] _farSkinId skin ID of the farthest box wall
    @param[in] _nearSkinId skin ID of the nearest box wall
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format */
    virtual void Init (float _x, float _y, float _z, float _size,
                       UINT _topSkinId, UINT _bottomSkinId,
                       UINT _leftSkinId, UINT _rightSkinId,
                       UINT _farSkinId, UINT _nearSkinId) = 0;

    /** Renders the skybox. 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL*/
    virtual void Render () = 0;
};

/** Terrain water. */
class ITerrainWater {
public:
    /** Renders terrain water.
    @param[in] _worldSize world size 
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid
        - @c ERRC_API_CALL */
    virtual void Render (float _worldSize) = 0;

    /** Setter: water color. 
    @param[in] _red red color
    @param[in] _green green color
    @param[in] _blue blue color
    @param[in] _alpha alpha value */
    virtual void SetColor (UCHAR _red, UCHAR _green, UCHAR _blue, UCHAR _alpha) = 0;

    /** Setter: skin ID.
    @param[in] _skinId skin ID */
    virtual void SetSkinId (UINT _skinId) = 0;

    /** Setter: water height.
    @param[in] _height water height */
    virtual void SetWaterHeight (float _height) = 0;

    /** Getter: water height.
    @return water height */
    virtual float GetWaterHeight () const = 0;
};

/** Terrain. */
class ITerrain {
public:
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
        - @c ERRC_API_CALL */
    virtual void Init (UCHAR _patchSize, VERTEXFORMATTYPE _vft, UINT _skinId) = 0;

    /** Releases terrain resources. */
    virtual void Shutdown () = 0;

    /** Updates the terrain.
    @param[in] _cameraPos camera position
    @param[in] _view view matrix
    @param[in] _proj projection matrix 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_NOT_READY either heightmap is not loaded or terrain is not initialized
        - @c ERRC_OUT_OF_RANGE */
    virtual void Update (VECTOR3 _cameraPos, MATRIX44 _view, MATRIX44 _proj) = 0;

    /** Renders terrain.
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid
        - @c ERRC_API_CALL
        - @c ERRC_OUT_OF_RANGE */
    virtual void Render () = 0;

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
    virtual bool IsPatchPicked (VECTOR3 _rayOrigin, VECTOR3 _rayDirection, 
                                UINT _x, UINT _z, std::vector<PatchBounds>& _intersectedParts) = 0;

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
    virtual bool IsVertexPicked (VECTOR3 _rayOrigin, VECTOR3 _rayDirection,
                                 PatchBounds _bounds, VECTOR3 _viewPoint, 
                                 POINT& _vertex, float& _distance) = 0;

    /** Updates terrain data. 
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE
        - @c ERRC_UNKNOWN_FVF */
    virtual void UpdateData () = 0;

    /** Enables terrain brute force rendering.
    @param[in] _enable brute force should be enabled */
    virtual void EnableBruteForce (bool _enable) = 0;

    /** Checks if brute force is enabled.
    @return @c true brute force is enabled. @c false otherwise */
    virtual bool IsBruteForceEnabled () const = 0;

    /** Getter: patches per side.
    @return patches per side */
    virtual UCHAR GetPatchesPerSide () const = 0;

    /** Checks if point is visible.
    @param[in] _x terrain x coordinate
    @param[in] _z terrain z coordinate
    @return @c true point is visible. @c false otherwise */
    virtual bool IsPointVisible (UINT _x, UINT _z) const = 0;

    /** Enables log.
    @param[in] _log pointer to LogManager. */
    virtual void EnableLog (LogManager* _log) = 0;

    /** Creates new heightmap.
    @param[in] _size size of the heightmap
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    virtual void NewHeightmap (UINT _size) = 0;

    /** Loads heightmap.
    @param[in] _heightmapFile path to the heightmap file
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file does not exist
        - @c ERRC_BAD_FILE file is corrupted
        - @c ERRC_OUT_OF_MEM not enough memory */
    virtual void LoadHeightmap (const char* _heightmapFile) = 0;

    /** Saves heightmap.
    @param[in] _heightmapFile path of the file 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file cannot be opened
        - @c ERRC_BAD_FILE cannot write to file */
    virtual void SaveHeightmap (const char* _heightmapFile) = 0;

    /** Releases heightmap resources. */
    virtual void UnloadHeightmap () = 0;

    /** Getter: heightmap file.
    @param[out] _filename heightmap file */
    virtual void GetHeightmapFile (char (& _filename)[MAX_PATH]) = 0;

    /** Getter: scaled height.
    @param[in] _x coordinate x of the terrain
    @param[in] _z coordinate z of the terrain
    @return scaled height
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters */
    virtual float GetScaledHeight (UINT _x, UINT _z) const = 0;

    /** Getter: height.
    @param[in] _x coordinate x of the terrain
    @param[in] _z coordinate z of the terrain
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters

    @return height */
    virtual UCHAR GetHeight (UINT _x, UINT _z) const = 0;

    /** Getter: terrain size.
    @return terrain size */
    virtual UINT GetSize () const = 0;

    /** Setter: height.
    @param[in] _x coordinate x of the terrain
    @param[in] _z coordinate z of the terrain
    @param[in] _height terrain height 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters */
    virtual void SetHeight (UINT _x, UINT _z, UCHAR _height) = 0;

    /** Setter: terrain scale.
    @param[in] _scaleX terrain x scale
    @param[in] _scaleY terrain y scale
    @param[in] _scaleZ terrain z scale 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_INVALID_PARAMETER scale is a negative value */
    virtual void SetScale (float _scaleX, float _scaleY, float _scaleZ) = 0;

    /** Getter: scale.
    @param[in] _i coordinate number (x = 0, y = 1, z = 2)
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE invalid parameter

    @return scale */
    virtual float GetScale (UCHAR _i) = 0;

    /** Loads lightmap.
    Lightmap size must be equal to heightmap size.
    @param[in] _filename lightmap's filename
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file does not exist
        - @c ERRC_BAD_FILE file file is corrupted
        - @c ERRC_OUT_OF_MEM not enough memory */
    virtual void LoadLightmap (const char* _filename) = 0;

    /** Saves lightmap.
    @param[in] _filename lightmap's filename 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file cannot be opened
        - @c ERRC_BAD_FILE cannot write to file */
    virtual void SaveLightmap (const char* _filename) = 0;

    /** Releases lightmap resources. */
    virtual void UnloadLightmap () = 0;

    /** Getter: lightmap file.
    @param[out] _filename lightmap file */
    virtual void GetLightmapFile (char (& _filename)[MAX_PATH]) = 0;

    /** Getter: brightness.
    @param[in] _x terrain x coordinate
    @param[in] _z terrain z coordinate
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters

    @return brightness */
    virtual UCHAR GetBrightness (UINT _x, UINT _z) const = 0;

    /** Setter: brightness.
    @param[in] _x terrain x coordinate
    @param[in] _z terrain z coordinate
    @param[in] _value brightness value
    @exception ErrorMessage

    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters */
    virtual void SetBrightness (UINT _x, UINT _z, UCHAR _value) = 0;

    /** Setter: light's color.
    @param[in] _red red component
    @param[in] _green green component
    @param[in] _blue blue component */
    virtual void SetLightColor (UCHAR _red, UCHAR _green, UCHAR _blue) = 0;

    /** Setter: slope lighting.
    @param[in] _lightDirX light's x direction
    @param[in] _lightDirZ light's z direction
    @param[in] _minBrightness minimum brightness
    @param[in] _maxBrightness maximum brightness
    @param[in] _softness light's softness 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_MEM not enough memory */
    virtual void SetSlopeLighting (int _lightDirX, int _lightDirZ,
                                      UINT _minBrightness, UINT _maxBrightness,
                                      float _softness) = 0;

    /** Setter: height based lighting. */
    virtual void SetHeightBasedLighting () = 0;

    /** Setter: lightmap lighting.
    @param[in] _filename lightmap's filename
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_FILE_NOT_FOUND file does not exist
        - @c ERRC_BAD_FILE file file is corrupted
        - @c ERRC_OUT_OF_MEM not enough memory */
    virtual void SetLightmapLighting (const char* _filename) = 0;

    /** Setter: density of the detail map.
    @param[in] _factor density */
    virtual void SetDetailMapDensity (UINT _factor) = 0;

    /** Loads texture tile.
    @param[in] _level tile level
    @param[in] _filename texture's filename 
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE level is incorrect
        - @c ERRC_BAD_FILE cannot load texture */
    virtual void LoadTextureTile (UINT _level, const char* _filename) = 0;

    /** Releases tile resources.
    @param[in] _level tile level
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE level is incorrect */
    virtual void UnloadTextureTile (UINT _level) = 0;

    /** Releases all tiles resources. */
    virtual void UnloadAllTextureTiles () = 0;

    /** Generates the texture map.
    Tiles should be loaded before calling this method.
    @param[in] _filename a file where the texture will be save
    @param[in] _size size of the texture
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE level is incorrect */
    virtual void GenerateTextureMap (const char* _filename, UINT _size) = 0;

    /** Getter: terrain color.
    @param[in] _x terrain x coordinate
    @param[in] _z terrain z coordinate
    @exception ErrorMessage
    
    - Possible error codes: 
        - @c ERRC_OUT_OF_RANGE wrong parameters

    @return ARGB format terrain color */
    virtual DWORD GetColor (UINT _x, UINT _z) = 0;
};

/** Terrain engine DLL interface. */
class TerrainEngine {
public:
    /** Constructor. */
    virtual ~TerrainEngine () {};

    /** Getter: terrain.
    @return terrain interface */
    virtual ITerrain* GetTerrain () const = 0;

    /** Getter: skybox.
    @return skybox interface. */
    virtual ISkyBox* GetSkyBox () const = 0;

    /** Getter: new skybox.
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory

    @return new skybox interface.
    @warning The returned object has to be deleted manually. */
    virtual ISkyBox* CreateSkyBox () const = 0;

    /** Getter: terrain water.
    @return terrain water interface. */
    virtual ITerrainWater* GetTerrainWater () const = 0;
};

extern "C" {
    typedef HRESULT (*CREATETERRAINENGINE) (RenderDevice*, TerrainEngine**);
    typedef void (*RELEASETERRAINENGINE) (TerrainEngine**);
}