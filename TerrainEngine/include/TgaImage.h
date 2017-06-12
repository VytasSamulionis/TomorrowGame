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
#ifndef TGA_LOADER_H
#define TGA_LOADER_H

#include <string>
#include <Windows.h>

#pragma pack (push, packing)
#pragma pack (1)


/* DESCRIPTION: structure for TarGA image header */
struct TgaHeader {
    unsigned char IdLength;
    char ColorMapType;
    char ImageType;
    unsigned short ColorMapFirstEntry;
    unsigned short ColorMapLength;
    unsigned char ColorMapEntrySize;
    unsigned short OriginX;
    unsigned short OriginY;
    unsigned short Width;
    unsigned short Height;
    char Depth;
    char Descriptor;
};

#pragma pack (pop, packing)

/* DESCRIPTION: Loads TarGA header and image.
        It supports only uncompressed true-color images.
*/
class TgaImage {
public:
    TgaImage ();
    TgaImage (std::string _Filename);
    /*  DESCRIPTION: constructor - loads targa image
        ARGUMENTS: fileName - file name of the targa image
    */
    TgaImage (const TgaImage& _TgaImage);
    /*  DESCRIPTION: deep copy constructore */
    TgaImage& operator= (const TgaImage& _TgaImage);
    /*  DESCRIPTION: deep copy assignment */
    void Load (std::string _Filename);
    void Unload ();
    bool IsLoaded () const;
    unsigned short GetWidth () const;
    /*  DESCRIPTION: returns width of the image
        RETURN: width
    */
    unsigned short GetHeight () const;
    /*  DESCRIPTION: returns height of the image
        RETURN: height
    */
    unsigned char* GetImage () const;
    /*  DESCRIPTION: returns dynammically allocated memory of image data
        RETURN: pointer to the block of memory where image data is saved
        COMMENTS: memory must be deallocated with delete[]
    */
    int GetComponents () const;
    /*  DESCRIPTION: returns components of targa image
        RETURN: components:
                    GL_LUMINANCE8 (0x8040)
                    GL_RGB8 (0x8051)
                    GL_RGBA8 (0x8058)
                    0 - unknown components
    */
    int GetFormat () const;
    /*  DESCRIPTION: returns format of targa image
        RETURN: format:
                    GL_LUMINANCE (0x1909)
                    GL_BGR_EXT (0x80E0)
                    GL_BGRA_EXT (0x80E1)
                    0 - unknown format
    */
    unsigned int GetBytesPerPixel () const;
    /*  DESCRIPTION: returns bpp of a loaded image
        RETURN:
            bpp
    */
    void CreateImage (UINT _Width, UINT _Height, UINT _Bpp);
    void SetPixelColor (UINT _x, UINT _y, UCHAR _Red, UCHAR _Green, UCHAR _Blue);
    void GetPixelColor (UINT _x, UINT _y, UCHAR& _Red, UCHAR& _Green, UCHAR& _Blue);
    void SaveImage (const char* _Filename);
    bool IsError () const;
    /*  DESCRIPTION: checks if error occured
        RETURN: 
            true - error
            false - no errors
    */
    ~TgaImage ();
    /*  DESCRIPTION: destructor */
private:
    bool m_Error;
    TgaHeader* m_TgaHeader;   // header of tga
    unsigned char* m_ImageData;   // image data
    int m_ImageSize;  // size of image data in bytes
    void copy (const TgaImage& _TgaImage);
};

#endif
