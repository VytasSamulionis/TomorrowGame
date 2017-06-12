/** @file TerrainWater.h */

#pragma once

#include "../../TerrainEngineLoader/include/TerrainEngine.h"

/** Terrain water. */
class TerrainWater: public ITerrainWater {
public:
    /** Constructor. */
    TerrainWater (RenderDevice* _device, UINT _skinId);

    /** Destructor. */
    ~TerrainWater ();

    /** Renders terrain water.
    @param[in] _worldSize world size 
    @exception ErrorMessage
    
    - Possible error codes:
        - @c ERRC_OUT_OF_MEM not enough memory
        - @c ERRC_NO_DEVICE device is not ready
        - @c ERRC_INVALID_PARAMETER vertex cahce's primitive type is invalid
        - @c ERRC_API_CALL */
    void Render (float _worldSize);

    /** Setter: water color. 
    @param[in] _red red color
    @param[in] _green green color
    @param[in] _blue blue color
    @param[in] _alpha alpha value */
    inline void SetColor (UCHAR _red, UCHAR _green, UCHAR _blue, UCHAR _alpha) {
        m_Color[0] = _red;
        m_Color[1] = _green;
        m_Color[2] = _blue;
        m_Transparency = _alpha;
    }

    /** Setter: skin ID.
    @param[in] _skinId skin ID */
    inline void SetSkinId (UINT _skinId) {
        m_SkinId = _skinId;
    }

    /** Setter: water height.
    @param[in] _height water height */
    inline void SetWaterHeight (float _height) {
        m_Height = _height;
    }

    /** Getter: water height.
    @return water height */
    inline float GetWaterHeight () const {
        return m_Height;
    }
private:
    vs3d::ULVERTEX m_Water[4];  /**< Water vertices. */
    UCHAR m_Transparency;       /**< Transparency. */
    UCHAR m_Color[3];           /**< Water color. */
    float m_Height;             /**< Water height. */
    float m_U;                  /**< Texture u coordinate. */
    float m_V;                  /**< Texture v coordinate. */
    RenderDevice* m_Device;     /**< Pointer to the RenderDevice. */
    UINT m_SkinId;              /**< Skin ID. */
};