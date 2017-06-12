/** @file Engine.h */
#pragma once

#include <Windows.h>
#include <cml/cml.h>

/** Maximum integer value which represents invalid ID. */
#define INVALID_ID (UINT) -1

/** 3d vector. */
typedef cml::vector3f VECTOR3;
/** Quaternion. */
typedef cml::quaternionf_p QUATERNION;
/** 4x4 matrix. */
typedef cml::matrix44f_r MATRIX44;

// Texture Arguments
/** Mask value for all arguments. */
#define TA_SELECTMASK        0x0000000f 
/** The texture argument is the diffuse color interpolated 
from vertex components during Gouraud shading. */
#define TA_DIFFUSE           0x00000000 
/** The texture argument is the result of the previous blending stage.*/
#define TA_CURRENT           0x00000001 
/** The texture argument is the texture color for this texture stage. */
#define TA_TEXTURE           0x00000002 
/** The texture argument is the texture factor. */
#define TA_TFACTOR           0x00000003 
/** The texture argument is the specular color interpolated 
from vertex components during Gouraud shading. */
#define TA_SPECULAR          0x00000004 
/** The texture argument is a temporary register color for read or write. */
#define TA_TEMP              0x00000005 
/** Select a constant from a texture stage. */
#define TA_CONSTANT          0x00000006 
/** Take the complement of the argument x, (1.0 - x). */
#define TA_COMPLEMENT        0x00000010 
/** Replicate the alpha information to all color 
channels before the operation completes. */
#define TA_ALPHAREPLICATE    0x00000020 

/** Light type. */
enum LIGHTTYPE {
    LIGHT_POINT = 1,    /**< Point light. */
    LIGHT_SPOT,         /**< Spot light. */
    LIGHT_DIRECTIONAL   /**< Directional light. */
};

/** Blend type. */
enum BLENDTYPE {
    BLEND_ZERO = 1,             /**< Blend factor is (0, 0, 0, 0). */
    BLEND_ONE = 2,              /**< Blend factor is (1, 1, 1, 1). */
    BLEND_SRCCOLOR = 3,         /**< Blend factor is (Rs, Gs, Bs, As). */
    BLEND_INVSRCCOLOR = 4,      /**< Blend factor is (1 - Rs, 1 - Gs, 1 - Bs, 1 - As). */
    BLEND_SRCALPHA = 5,         /**< Blend factor is (As, As, As, As). */
    BLEND_INVSRCALPHA = 6,      /**< Blend factor is ( 1 - As, 1 - As, 1 - As, 1 - As). */
    BLEND_DESTALPHA = 7,        /**< Blend factor is (Ad Ad Ad Ad). */
    BLEND_INVDESTALPHA = 8,     /**< Blend factor is (1 - Ad 1 - Ad 1 - Ad 1 - Ad). */
    BLEND_DESTCOLOR = 9,        /**< Blend factor is (Rd, Gd, Bd, Ad). */
    BLEND_INVDESTCOLOR = 10,    /**< Blend factor is (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad). */
    BLEND_SRCALPHASAT = 11,     /**< Blend factor is (f, f, f, 1); where f = min(As, 1 - Ad). */
};

/** Alpha blend state types. */
enum ALPHABLENDSTATETYPE { 
    AS_SRCBLEND,    /**< Source blend. */
    AS_DESTBLEND,   /**< Destination blend. */
};

/** Point sprite state type. */
enum POINTSPRITESTATETYPE {
    PS_POINTSIZE_MIN,   /**< Minimum point's size. */
    PS_POINTSIZE_MAX,   /**< Maximum point's size. */
    PS_POINTSCALE_A,    /**< Point scale. */
    PS_POINTSCALE_B,    /**< Point scale. */
    PS_POINTSCALE_C,    /**< Point scale. */
};

/** Texture stage state type. */
enum TEXTURESTAGESTATETYPE {
    TSS_COLOROP = 1,                /**< Texture state state enumeration member. */
    TSS_COLORARG1 = 2,              /**< Texture state state enumeration member. */
    TSS_COLORARG2 = 3,              /**< Texture state state enumeration member. */
    TSS_ALPHAOP = 4,                /**< Texture state state enumeration member. */
    TSS_ALPHAARG1 = 5,              /**< Texture state state enumeration member. */
    TSS_ALPHAARG2 = 6,              /**< Texture state state enumeration member. */
    TSS_BUMPENVMAT00 = 7,           /**< Texture state state enumeration member. */
    TSS_BUMPENVMAT01 = 8,           /**< Texture state state enumeration member. */
    TSS_BUMPENVMAT10 = 9,           /**< Texture state state enumeration member. */
    TSS_BUMPENVMAT11 = 10,          /**< Texture state state enumeration member. */
    TSS_TEXCOORDINDEX = 11,         /**< Texture state state enumeration member. */
    TSS_BUMPENVLSCALE = 22,         /**< Texture state state enumeration member. */
    TSS_BUMPENVLOFFSET = 23,        /**< Texture state state enumeration member. */
    TSS_TEXTURETRANSFORMFLAGS = 24, /**< Texture state state enumeration member. */
    TSS_COLORARG0 = 26,             /**< Texture state state enumeration member. */
    TSS_ALPHAARG0 = 27,             /**< Texture state state enumeration member. */
    TSS_RESULTARG = 28,             /**< Texture state state enumeration member. */
    TSS_CONSTANT = 32               /**< Texture state state enumeration member. */
};

/** Texture operation. */
enum TEXTUREOP {
    TOP_DISABLE = 1,                    /**< Texture operation enumeration member. */
    TOP_SELECTARG1 = 2,                 /**< Texture operation enumeration member. */
    TOP_SELECTARG2 = 3,                 /**< Texture operation enumeration member. */
    TOP_MODULATE = 4,                   /**< Texture operation enumeration member. */
    TOP_MODULATE2X = 5,                 /**< Texture operation enumeration member. */
    TOP_MODULATE4X = 6,                 /**< Texture operation enumeration member. */
    TOP_ADD = 7,                        /**< Texture operation enumeration member. */
    TOP_ADDSIGNED = 8,                  /**< Texture operation enumeration member. */
    TOP_ADDSIGNED2X = 9,                /**< Texture operation enumeration member. */
    TOP_SUBTRACT = 10,                  /**< Texture operation enumeration member. */
    TOP_ADDSMOOTH = 11,                 /**< Texture operation enumeration member. */
    TOP_BLENDDIFFUSEALPHA = 12,         /**< Texture operation enumeration member. */
    TOP_BLENDTEXTUREALPHA = 13,         /**< Texture operation enumeration member. */
    TOP_BLENDFACTORALPHA = 14,          /**< Texture operation enumeration member. */
    TOP_BLENDTEXTUREALPHAPM = 15,       /**< Texture operation enumeration member. */
    TOP_BLENDCURRENTALPHA = 16,         /**< Texture operation enumeration member. */
    TOP_PREMODULATE = 17,               /**< Texture operation enumeration member. */
    TOP_MODULATEALPHA_ADDCOLOR = 18,    /**< Texture operation enumeration member. */
    TOP_MODULATECOLOR_ADDALPHA = 19,    /**< Texture operation enumeration member. */
    TOP_MODULATEINVALPHA_ADDCOLOR = 20, /**< Texture operation enumeration member. */
    TOP_MODULATEINVCOLOR_ADDALPHA = 21, /**< Texture operation enumeration member. */
    TOP_BUMPENVMAP = 22,                /**< Texture operation enumeration member. */
    TOP_BUMPENVMAPLUMINANCE = 23,       /**< Texture operation enumeration member. */
    TOP_DOTPRODUCT3 = 24,               /**< Texture operation enumeration member. */
    TOP_MULTIPLYADD = 25,               /**< Texture operation enumeration member. */
    TOP_LERP = 26                       /**< Texture operation enumeration member. */
};

/** Render state type. */
enum RENDERSTATETYPE {
    RS_CULL_CW,                 /**< cull clockwise ordered triangles. */
    RS_CULL_CCW,                /**< cull counter cw ordered triangles. */
    RS_CULL_NONE,               /**< render front- and backsides. */
    RS_DEPTH_READWRITE,         /**< read and write depth buffer. */
    RS_DEPTH_READONLY,          /**< read but don't write depth buffer. */
    RS_DEPTH_NONE,              /**< no read or write with depth buffer. */
    RS_DRAW_POINTS,             /**< render just vertices. */
    RS_DRAW_WIRE,               /**< render triangulated wire. */
    RS_DRAW_SOLID,              /**< render solid polygons. */
    RS_SHADE_FLAT,              /**< flat shading. */
    RS_SHADE_GOURAUD,           /**< gouraud shading. */
    RS_STENCIL_DISABLE,         /**< stencilbuffer off. */
    RS_STENCIL_ENABLE,          /**< stencilbuffer off. */
    RS_STENCIL_FUNC_ALWAYS,     /**< stencil pass mode. */
    RS_STENCIL_FUNC_LESSEQUAL,  /**< stencil pass mode. */
    RS_STENCIL_MASK,            /**< stencil mask. */
    RS_STENCIL_WRITEMASK,       /**< stencil write mask. */
    RS_STENCIL_REF,             /**< reference value. */
    RS_STENCIL_FAIL_DECR,       /**< stencil fail decrements. */
    RS_STENCIL_FAIL_INCR,       /**< stencil fail increments. */
    RS_STENCIL_FAIL_KEEP,       /**< stencil fail keeps. */
    RS_STENCIL_ZFAIL_DECR,      /**< stencil pass but z fail decrements. */
    RS_STENCIL_ZFAIL_INCR,      /**< stencil pass but z fail increments. */
    RS_STENCIL_ZFAIL_KEEP,      /**< stencil pass but z fail keeps. */
    RS_STENCIL_PASS_DECR,       /**< stencil and z pass decrements. */
    RS_STENCIL_PASS_INCR,       /**< stencil and z pass increments. */
    RS_STENCIL_PASS_KEEP,       /**< stencil and z pass keeps. */
};

/** Fog modes. */
enum FOGMODE {
    FOG_NONE,   /**< No fog. */
    FOG_EXP,    /**< Exponential fog. */
    FOG_EXP2,   /**< Exponential fog. */
    FOG_LINEAR  /**< Linear fog. */
};

/** Primitive types. */
enum PRIMITIVETYPE {
    PT_POINT,           /** Point. */
    PT_LINELIST,        /** Line list. */
    PT_LINESTRIP,       /** Line strip. */
    PT_TRIANGLELIST,    /** Triangle list. */
    PT_TRIANGLESTRIP,   /** Triangle strip. */
};

/** Fixed vertex formats. */
enum FVFTYPE {
    FVF_UP = 1,     /**< untransformed position. */
    FVF_UU,         /**< untransformed unlit. */
    FVF_UU2,        /**< untransformed unlit with 2 texture coordinates. */
    FVF_UL,         /**< untransformed lit .*/
    FVF_UL2,        /**< untransformed lit with 2 texture coordinates. */
    FVF_ULC         /**< untransformed lit colored. */
};

/** Engine structure namespace. */
namespace vs3d {

    /** Untransformed position vertex. */
    struct UPVERTEX {
        float x;        /**< x coordinate. */
        float y;        /**< y coordinate. */
        float z;        /**< z coordinate. */

        UPVERTEX ();    /**< Constructor. */
        /** Constructor.
        @param[in] _x x coordinate
        @param[in] _y y coordinate
        @param[in] _z z coordinate */
        UPVERTEX (float _x, float _y, float _z);
    };

    /** Untransformed unlit vertex. */
    struct UUVERTEX {
        float x;        /**< x coordinate. */
        float y;        /**< y coordinate. */
        float z;        /**< z coordinate. */
        float Normal[3];    /**< Normals. */
        float tu;       /**< u texture coordinate. */
        float tv;       /**< v texture coordinate. */

        UUVERTEX ();    /**< Constructor. */

        /** Constructor.
        @param[in] _x x coordinate
        @param[in] _y y coordinate
        @param[in] _z z coordinate 
        @param[in] _normalX x coordinate normal
        @param[in] _normalY y coordinate normal
        @param[in] _normalZ z coordinate normal
        @param[in] _tu u texture coordinate
        @param[in] _tv v texture coordinate */
        UUVERTEX (float _x, float _y, float _z,
                  float _normalX, float _normalY, float _normalZ,
                  float _tu, float _tv);
    };

    /** Untransformed unlit vertex. */
    struct UUVERTEX2 {
        float x;        /**< x coordinate. */
        float y;        /**< y coordinate. */
        float z;        /**< z coordinate. */
        float Normal[3];    /**< Normals. */
        float tu1;      /**< 1st u texture coordinate. */
        float tv1;      /**< 1st v texture coordinate. */
        float tu2;      /**< 2nd u texture coordinate. */
        float tv2;      /**< 2nd v texture coordinate. */

        UUVERTEX2 ();   /**< Constructor. */

        /** Constructor.
        @param[in] _x x coordinate
        @param[in] _y y coordinate
        @param[in] _z z coordinate 
        @param[in] _normalX x coordinate normal
        @param[in] _normalY y coordinate normal
        @param[in] _normalZ z coordinate normal
        @param[in] _tu1 1st u texture coordinate
        @param[in] _tv1 1st v texture coordinate
        @param[in] _tu2 2nd u texture coordinate
        @param[in] _tv2 2nd v texture coordinate*/
        UUVERTEX2 (float _x, float _y, float _z,
                   float _normalX, float _normalY, float _normalZ,
                   float _tu1, float _tv1, float _tu2, float _tv2);
    };

    /** Untransformed lit vertex. */
    struct ULVERTEX {
        float x;        /**< x coordinate. */
        float y;        /**< y coordinate. */
        float z;        /**< z coordinate. */
        DWORD Color;    /**< Color. */
        float tu;       /**< u texture coordinate. */
        float tv;       /**< v texture coordinate. */

        ULVERTEX ();    /**< Constructor. */

        /** Constructor.
        @param[in] _x x coordinate
        @param[in] _y y coordinate
        @param[in] _z z coordinate 
        @param[in] _color color
        @param[in] _tu u texture coordinate
        @param[in] _tv v texture coordinate */
        ULVERTEX (float _x, float _y, float _z,
                  DWORD _color, float _tu, float _tv);
    };

    /** Untransformed lit vertex. */
    struct ULVERTEX2 {
        float x;        /**< x coordinate. */
        float y;        /**< y coordinate. */
        float z;        /**< z coordinate. */
        DWORD Color;    /**< Color. */
        float tu1;      /**< 1st u texture coordinate. */
        float tv1;      /**< 1st v texture coordinate. */
        float tu2;      /**< 2nd u texture coordinate. */
        float tv2;      /**< 2nd v texture coordinate. */

        ULVERTEX2 ();   /**< Constructor. */

        /** Constructor.
        @param[in] _x x coordinate
        @param[in] _y y coordinate
        @param[in] _z z coordinate 
        @param[in] _color color
        @param[in] _tu1 1st u texture coordinate
        @param[in] _tv1 1st v texture coordinate
        @param[in] _tu2 2nd u texture coordinate
        @param[in] _tv2 2nd v texture coordinate*/
        ULVERTEX2 (float _x, float _y, float _z, DWORD _color,
                   float _tu1, float _tv1, float _tu2, float _tv2);
    };

    /** Untransformed lit colored vertex. */
    struct ULCVERTEX {
        float x;        /**< x coordinate. */
        float y;        /**< y coordinate. */
        float z;        /**< z coordinate. */
        DWORD Color;    /**< Color. */

        ULCVERTEX ();   /**< Constructor. */

        /** Constructor.
        @param[in] _x x coordinate
        @param[in] _y y coordinate
        @param[in] _z z coordinate 
        @param[in] _color color */
        ULCVERTEX (float _x, float _y, float _z, DWORD _color);
    };

    /** Color value. */
    struct COLORVALUE {
        float r;    /**< red */
        float g;    /**< green */
        float b;    /**< blue */
        float a;    /**< alpha */

        COLORVALUE ();  /** Constructor. */

        /** Constructor.
        @param[in] _r red value
        @param[in] _g green value
        @param[in] _b blue value
        @param[in] _a alpha value */
        COLORVALUE (float _r, float _g, float _b, float _a);

        /** Compares the two colors. */
        bool operator == (const COLORVALUE& _color);
    };

    /** Material. */
    struct MATERIAL {
        COLORVALUE Diffuse;     /**< Diffuse. */
        COLORVALUE Specular;    /**< Specular. */
        COLORVALUE Ambient;     /**< Ambient. */
        COLORVALUE Emissive;    /**< Emissive. */
        float Power;            /**< Power. */

        /** Constructor. */
        MATERIAL ();
       
        /** Constructor.
        @param[in] _diffuse diffuse color
        @param[in] _specular specular color
        @param[in] _ambient ambient color
        @param[in] _emissive emissive color
        @param[in] _power power */
        MATERIAL (COLORVALUE _diffuse, COLORVALUE _specular,
                  COLORVALUE _ambient, COLORVALUE _emissive,
                  float _power);

        /** Compares two materials. */
        bool operator == (const MATERIAL& _material);
    };

    /** Texture. */
    struct TEXTURE {
        char Name[255]; /**< Filename of the texture. */
        void* Data;     /**< Texture data. */
    };

    /** Skin. */
    struct SKIN {
        UINT TextureId[8];  /**< Texture ID. */
        UINT NumTextures;   /**< Number of the textures. */
        UINT MaterialId;    /**< Material ID. */

        /** Constructor. */
        SKIN ();

        /** Compares two skins. */
        bool operator == (const SKIN& _skin);
    };

    /** Light information. */
    struct LIGHT {
        LIGHTTYPE Type;         /**< Light type. */
        COLORVALUE Diffuse;     /**< Diffuse color. */
        COLORVALUE Specular;    /**< Specular color. */
        COLORVALUE Ambient;     /**< Ambient color. */
        VECTOR3 Position;       /**< Position. */
        VECTOR3 Direction;      /**< Direction. */
        float Range;            /**< Range. */
        float Falloff;          /**< Falloff. */
        float Attenuation0;     /**< Attentuation. */
        float Attenuation1;     /**< Attentuation. */
        float Attenuation2;     /**< Attentuation. */
        float Theta;            /**< Theta. */
        float Phi;              /**< Phi. */
    };

}
