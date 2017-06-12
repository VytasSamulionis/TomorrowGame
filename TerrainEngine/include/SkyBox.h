/** @file SkyBox.h */
#pragma once

#include "../../TerrainEngineLoader/include/TerrainEngine.h"

/** Terrain sky box. */
class SkyBox: public ISkyBox {
public:
    /** Constructor.
    @param[in] _device the pointer to the RenderDevice */
    SkyBox (RenderDevice* _device);

    /** Destructor. */
    ~SkyBox ();

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
        - @c ERRC_UNKNOWN_FVF invalid vertex format
    */
    void Init (float _x, float _y, float _z, float _size,
               UINT _topSkinId, UINT _bottomSkinId,
               UINT _leftSkinId, UINT _rightSkinId,
               UINT _farSkinId, UINT _nearSkinId);

    /** Renders the skybox. 
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL*/
    void Render ();
private:
    /** Initializes the top of the skybox.
    @param[in] _size size of the box
    @param[in] _skinId skin ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format */
    void InitTopSide (float _size, UINT _skinId);

    /** Initializes the bottom of the skybox.
    @param[in] _size size of the box
    @param[in] _skinId skin ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format */
    void InitBottomSide (float _size, UINT _skinId);

    /** Initializes the bottom of the skybox.
    @param[in] _size size of the box
    @param[in] _skinId skin ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format*/
    void InitLeftSide (float _size, UINT _skinId);

    /** Initializes the bottom of the skybox.
    @param[in] _size size of the box
    @param[in] _skinId skin ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format*/
    void InitRightSide (float _size, UINT _skinId);

    /** Initializes the bottom of the skybox.
    @param[in] _size size of the box
    @param[in] _skinId skin ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format*/
    void InitFarSide (float _size, UINT _skinId);

    /** Initializes the bottom of the skybox.
    @param[in] _size size of the box
    @param[in] _skinId skin ID
    @exception ErrorMessage

    - Possible error codes:
        - @c ERRC_OUT_OF_RANGE vertex buffer ID is invalid
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_API_CALL
        - @c ERRC_UNKNOWN_FVF invalid vertex format*/
    void InitNearSide (float _size, UINT _skinId);

    RenderDevice* m_Device; /**< Pointer to the RenderDevice. */
    float m_Center[3];      /**< Center of the box. */
    UINT m_VertexBufferId;  /**< Static vertex buffer ID. */
    UINT m_SkinId[6];       /**< Skin ID for each box wall. */
};