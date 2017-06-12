#include "../include/Engine.h"

using namespace vs3d;

COLORVALUE::COLORVALUE () {
    r = g = b = 0.0f;
    a = 1.0f;
}

COLORVALUE::COLORVALUE (float _r, float _g, float _b, float _a):
    r (_r), g (_g), b (_b), a (_a) {}

bool COLORVALUE::operator == (const COLORVALUE& _color) {
    if (r == _color.r && g == _color.g && b == _color.b && a == _color.a) {
        return true;
    }
    return false;
}

MATERIAL::MATERIAL () {
    /* Default DirectX material values. */
    Diffuse = COLORVALUE (1.0f, 1.0f, 1.0f, 0.0f);
    Ambient = COLORVALUE (0.0f, 0.0f, 0.0f, 0.0f);
    Specular = COLORVALUE (0.0f, 0.0f, 0.0f, 0.0f);
    Emissive = COLORVALUE (0.0f, 0.0f, 0.0f, 0.0f);
    Power = 0.0f;
}

MATERIAL::MATERIAL (COLORVALUE _diffuse, COLORVALUE _specular,
                    COLORVALUE _ambient, COLORVALUE _emissive,
                    float _power):
    Diffuse (_diffuse),
    Specular (_specular),
    Ambient (_ambient),
    Emissive (_emissive),
    Power (_power) {}

bool MATERIAL::operator == (const MATERIAL& _material) {
    if (Diffuse == _material.Diffuse &&
        Ambient == _material.Ambient &&
        Specular == _material.Specular &&
        Emissive == _material.Emissive &&
        Power == _material.Power) {
            return true;
    }
    return false;
}

SKIN::SKIN () {
    for (UINT i = 0; i < 8; i++) {
        TextureId[i] = INVALID_ID;
    }
    NumTextures = 0;
    MaterialId = INVALID_ID;
}

bool SKIN::operator == (const SKIN& _skin) {
    if (NumTextures == _skin.NumTextures && MaterialId == _skin.MaterialId) {
        for (UINT i = 0; i < NumTextures; i++) {
            if (TextureId[i] != _skin.TextureId[i]) {
                return false;
            }
        }
    }
    return true;
}

UPVERTEX::UPVERTEX () {
    X = Y = Z = 0.0f;
}

UPVERTEX::UPVERTEX (float _x, float _y, float _z):
    X (_x), Y (_y), Z (_z) {}

UUVERTEX::UUVERTEX () {
    X = Y = Z = 0.0f;
    Normal[0] = Normal[1] = Normal[2] = 0.0f;
    Tu = Tv = 0.0f;
}

UUVERTEX::UUVERTEX (float _x, float _y, float _z,
                    float _normalX, float _normalY, float _normalZ,
                    float _tu, float _tv):
    X (_x), Y (_y), Z (_z),    
    Tu (_tu), Tv (_tv) {

    Normal[0] = _normalX;
    Normal[1] = _normalY;
    Normal[2] = _normalZ;
}

UUVERTEX2::UUVERTEX2 () {
    X = Y = Z = 0.0f;
    Normal[0] = Normal[1] = Normal[2] = 0.0f;
    Tu1 = Tv1 = Tu2 = Tv2 = 0.0f;
}

UUVERTEX2::UUVERTEX2 (float _x, float _y, float _z,
                      float _normalX, float _normalY, float _normalZ,
                      float _tu1, float _tv1, float _tu2, float _tv2):
    X (_x), Y (_y), Z (_z),    
    Tu1 (_tu1), Tv1 (_tv1),
    Tu2 (_tu2), Tv2 (_tv2) {

    Normal[0] = _normalX;
    Normal[1] = _normalY;
    Normal[2] = _normalZ;
}

ULVERTEX::ULVERTEX () {
    X = Y = Z = 0.0f;
    Color = 0xffffffff;
    Tu = Tv = 0.0f;
}

ULVERTEX::ULVERTEX (float _x, float _y, float _z,
                    DWORD _color, float _tu, float _tv):
    X (_x), Y (_y), Z (_z),
    Color (_color), Tu (_tu), Tv (_tv) {}

ULVERTEX2::ULVERTEX2 () {
    X = Y = Z = 0.0f;
    Color = 0xffffffff;
    Tu1 = Tv1 = Tu2 = Tv2 = 0.0f;
}

ULVERTEX2::ULVERTEX2 (float _x, float _y, float _z, DWORD _color,
                      float _tu1, float _tv1, float _tu2, float _tv2):
    X (_x), Y (_y), Z (_z), Color (_color),
    Tu1 (_tu1), Tv1 (_tv1), Tu2 (_tu2), Tv2 (_tv2) {}

ULCVERTEX::ULCVERTEX () {
    X = Y = Z = 0.0f;
    Color = 0xffffffff;
}

ULCVERTEX::ULCVERTEX (float _x, float _y, float _z, DWORD _color):
    X (_x), Y (_y), Z (_z), Color (_color) {}

TPVERTEX::TPVERTEX () {
    X = Y = Z = RHW = 0.0f;
}

TPVERTEX::TPVERTEX (float _x, float _y, float _z, float _rhw):
    X (_x), Y (_y), Z (_z), RHW (_rhw) {}

TLVERTEX::TLVERTEX () {
    X = Y = Z = RHW = 0.0f;
    Color = 0xffffffff;
    Tu = Tv = 0.0f;
}

TLVERTEX::TLVERTEX (float _x, float _y, float _z, float _rhw,
                    DWORD _color, float _tu, float _tv):
    X (_x), Y (_y), Z (_z), RHW (_rhw),
    Color (_color), Tu (_tu), Tv (_tv) {}

TLCVERTEX::TLCVERTEX () {
    X = Y = Z = RHW = 0.0f;
    Color = 0xffffffff;
}

TLCVERTEX::TLCVERTEX (float _x, float _y, float _z, 
                      float _rhw, DWORD _color):
    X (_x), Y (_y), Z (_z), RHW (_rhw), Color (_color) {}