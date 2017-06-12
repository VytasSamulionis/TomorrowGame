/* 
 * AUTHOR:
 *	Vytas Samulionis
 *	vytas.samulionis@mif.stud.vu.lt
 *	Vilnius University, The Faculty of Mathematics and Informatics
 *	Software Engineering, Course 1, Group 4
 *
 * DESCRIPTION: class loads uncompressed true-color tga image file.
 *
 * VERSION:
 *  Version v1:
 *      Added deep copy constructore and assignment operator;
 *      Added default constructor;
 *      Added load method;
 *  Release date: 2010-05-21
 *  Code was written in MS Visual Studio 2008. 
 *  It was compiled with MS Visual Studio 2008.
 *
 */
#include "../include/TgaImage.h"
#include <string>
#include <fstream>

typedef unsigned short ushort;
typedef unsigned char uchar;

using namespace std;

void TgaImage::copy (const TgaImage& _TgaImage) {
    m_Error = _TgaImage.m_Error;
    if (_TgaImage.m_TgaHeader != NULL) {
        m_TgaHeader = new TgaHeader;
        *m_TgaHeader = *(_TgaImage.m_TgaHeader);
    }
    m_ImageSize = _TgaImage.m_ImageSize;
    if (_TgaImage.m_ImageData != NULL) {
        m_ImageData = new unsigned char[m_ImageSize];
        for (int i = 0; i < m_ImageSize; i++) {
            m_ImageData[i] = _TgaImage.m_ImageData[i];
        }
    }
}

void TgaImage::Load (string _Filename) {
    delete m_TgaHeader;
    delete[] m_ImageData;
    m_Error = false;
    fstream input (_Filename.c_str (), ios::in | ios::binary);
    if (!input.fail ()) {
        m_TgaHeader = new (nothrow) TgaHeader;
        if (m_TgaHeader == NULL) {
            m_Error = true;
        } else {
            input.read ((char*)&m_TgaHeader->IdLength, sizeof (char));
            input.read ((char*)&m_TgaHeader->ColorMapType, sizeof (char));
            input.read ((char*)&m_TgaHeader->ImageType, sizeof (char));
            // only supports uncompressed true-color images
            if (m_TgaHeader->ImageType != 2) {
                m_Error = true;
            } else {
                char* headerMember;
                headerMember = (char*) &m_TgaHeader->ColorMapFirstEntry;
                input.read (headerMember, sizeof (short));
                headerMember = (char*) &m_TgaHeader->ColorMapLength;
                input.read (headerMember, sizeof (short));
                headerMember = (char*)&m_TgaHeader->ColorMapEntrySize;
                input.read (headerMember, sizeof (char));
                input.read ((char*)&m_TgaHeader->OriginX, sizeof (short));
                input.read ((char*)&m_TgaHeader->OriginY, sizeof (short));
                input.read ((char*)&m_TgaHeader->Width, sizeof (short));
                input.read ((char*)&m_TgaHeader->Height, sizeof (short));
                input.read ((char*)&m_TgaHeader->Depth, sizeof (char));
                input.read ((char*)&m_TgaHeader->Descriptor, sizeof (char));
                if (!input.fail () && !input.eof ()) {
                    if (m_TgaHeader->Depth % 8 == 0 && m_TgaHeader->Depth <= 32) {
                        int depthBytes = m_TgaHeader->Depth / 8;
                        m_ImageSize = m_TgaHeader->Width * m_TgaHeader->Height;
                        m_ImageSize *= depthBytes;
                        m_ImageData = new (nothrow) uchar[m_ImageSize];
                        if (m_ImageData == NULL) {
                            m_Error = true;
                        } else {                        
                            input.read ((char*) m_ImageData, m_ImageSize);
                            if (input.fail ()) {
                                m_Error = true;
                            }
                        }
                    } else {
                        m_Error = true;
                    }
                } else {
                    m_Error = true;
                }
            }
        }
        input.close ();
    } else {
        m_Error = true;
    }
}

void TgaImage::Unload () {
    if (m_TgaHeader) {
        delete m_TgaHeader;
        m_TgaHeader = NULL;
    }
    if (m_ImageData) {
        delete[] m_ImageData;
        m_ImageData = NULL;
    }
}

bool TgaImage::IsLoaded () const {
    if (m_ImageData) {
        return true;
    }
    return false;
}

TgaImage::TgaImage (string fileName) {
    Load (fileName);
}

TgaImage::TgaImage () {
    m_TgaHeader = NULL;
    m_ImageData = NULL;
}

TgaImage::TgaImage (const TgaImage& _TgaImage) {
    m_TgaHeader = NULL;
    m_ImageData = NULL;
    copy (_TgaImage);
}

TgaImage& TgaImage::operator= (const TgaImage& _TgaImage) {
    if (this != &_TgaImage) {
        delete m_TgaHeader;
        delete[] m_ImageData;
        copy (_TgaImage);
    }
    return *this;
}

TgaImage::~TgaImage () {
    Unload ();
}

ushort TgaImage::GetWidth () const {
    if (!m_Error) {
        return m_TgaHeader->Width;
    }
    return 0;
}

ushort TgaImage::GetHeight () const {
    if (!m_Error) {
        return m_TgaHeader->Height;
    }
    return 0;
}

uchar* TgaImage::GetImage () const {
    if (!m_Error) {
        uchar* image = new (nothrow) uchar[m_ImageSize];
        if (image == NULL) {
            return NULL;
        }
        for (int i = 0; i < m_ImageSize; i++) {
            image[i] = m_ImageData[i];
        }
        return image;
    }
    return NULL;
}

int TgaImage::GetFormat () const {
    int format;
    switch (m_TgaHeader->Depth) {
        case 8:
            format = 0x1909;    // GL_LUMINANCE
            break;
        case 24:
            format = 0x80E0;    // GL_BGR_EXT
            break;
        case 32:
            format = 0x80E1;    // GL_BGRA_EXT
            break;
        default:
            format = 0;         // unknown
    }
    return format;
}

int TgaImage::GetComponents () const {
    int components;
    switch (m_TgaHeader->Depth) {
        case 8:
            components = 0x8040;    // GL_LUMINANCE8
            break;
        case 24:
            components = 0x8051;    // GL_RGB8
            break;
        case 32:
            components = 0x8058;    // GL_RGBA8
            break;
        default:
            components = 0;         // unknown
    }
    return components;
}

unsigned int TgaImage::GetBytesPerPixel () const {
    if (!m_Error) {
        return (unsigned int)m_TgaHeader->Depth;
    }
    return 0;
}

void TgaImage::CreateImage (UINT _Width, UINT _Height, UINT _Bpp) {
    if (m_ImageData) {
        delete[] m_ImageData;
        m_ImageData = NULL;
    }
    if (m_TgaHeader) {
        delete m_TgaHeader;
        m_TgaHeader = NULL;
    }
    m_TgaHeader = new TgaHeader;
    if (!m_TgaHeader) {
        m_Error = true;
        return;
    }
    m_TgaHeader->IdLength = 0;
    m_TgaHeader->ColorMapType = 0;
    m_TgaHeader->ImageType = 2;
    m_TgaHeader->ColorMapFirstEntry = 0;
    m_TgaHeader->ColorMapLength = 0;
    m_TgaHeader->ColorMapEntrySize = 0;
    m_TgaHeader->OriginX = 0;
    m_TgaHeader->OriginY = 0;
    m_TgaHeader->Width = _Width;
    m_TgaHeader->Height = _Height;
    m_TgaHeader->Depth = _Bpp;
    m_TgaHeader->Descriptor = 0;
    UINT bpp = _Bpp / 8;
    m_ImageSize = _Width * _Height * bpp;
    m_ImageData = new UCHAR[m_ImageSize];
    memset (m_ImageData, 0, m_ImageSize);
    if (!m_ImageData) {
        m_Error = true;
        return;
    }
}

void TgaImage::SetPixelColor (UINT _x, UINT _y, UCHAR _Red, UCHAR _Green, UCHAR _Blue) {
    if (!m_ImageData) {
        return;
    }
    if (_x < m_TgaHeader->Width && _y < m_TgaHeader->Height) {
        UINT bpp = m_TgaHeader->Depth / 8;
        m_ImageData[(_y * m_TgaHeader->Height + _x) * bpp] = _Red;
        m_ImageData[(_y * m_TgaHeader->Height + _x) * bpp + 1] = _Green;
        m_ImageData[(_y * m_TgaHeader->Height + _x) * bpp + 2] = _Blue;
    }
}

void TgaImage::GetPixelColor (UINT _x, UINT _y, UCHAR& _Red, UCHAR& _Green, UCHAR& _Blue) {
    if (!m_ImageData) {
        return;
    }
    if (_x < m_TgaHeader->Width && _y < m_TgaHeader->Height) {
        UINT bpp = m_TgaHeader->Depth / 8;
        _Red = m_ImageData[(_y * m_TgaHeader->Height + _x) * bpp];
        _Green = m_ImageData[(_y * m_TgaHeader->Height + _x) * bpp + 1];
        _Blue = m_ImageData[(_y * m_TgaHeader->Height + _x) * bpp + 2];
    }
}

void TgaImage::SaveImage (const char* _Filename) {
    FILE* tgaFile = fopen (_Filename, "wb");
    if (!tgaFile) {
        m_Error = true;
        return;
    }
    fwrite (m_TgaHeader, 1, sizeof (TgaHeader), tgaFile);
    fwrite (m_ImageData, 1, m_ImageSize, tgaFile);
    fclose (tgaFile);
}

bool TgaImage::IsError () const {
    return m_Error;
}
