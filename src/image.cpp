/*
* EGE (Easy Graphics Engine)
* filename  image.cpp

本文件集中所有对image基本操作的接口和类定义
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include "ege_head.h"

//#ifdef _ITERATOR_DEBUG_LEVEL
//#undef _ITERATOR_DEBUG_LEVEL
//#endif

#include "lpng/png.h"
#include "lpng/pnginfo.h"
#include "ocidl.h"
#include "olectl.h"

#include <math.h>

namespace ege {

void IMAGE::reset() {
	m_initflag = IMAGE_INIT_FLAG;
	m_hDC = NULL;
	m_hBmp = NULL;
	m_width = 0;
	m_height = 0;
	m_pBuffer = NULL;
	m_color = 0;
	m_fillcolor = 0;
	m_aa = false;
	memset(&m_vpt, 0, sizeof(m_vpt));
	memset(&m_texttype, 0, sizeof(m_texttype));
	memset(&m_linestyle, 0, sizeof(m_linestyle));
	m_linewidth = 0.0f;
	m_bk_color = 0;
	m_texture = NULL;
#ifdef EGE_GDIPLUS
	m_graphics=NULL;
	m_pen=NULL;
	m_brush=NULL;
#endif
}

void IMAGE::construct(int width, int height) {
	HDC refDC = NULL;

	if (graph_setting.hwnd) {
		refDC = ::GetDC(graph_setting.hwnd);
	}

	reset();
	initimage(refDC, width, height);
	setdefaultattribute();

	if (refDC) {
		::ReleaseDC(graph_setting.hwnd, refDC);
	}
}

IMAGE::IMAGE() {
	construct(1, 1);
}

IMAGE::IMAGE(int width, int height) {
	// 截止到 0
	if (width < 0) width = 0;
	if (height < 0) height = 0;
	construct(width, height);
}

IMAGE::IMAGE(const IMAGE &img) {
	reset();
	initimage(img.m_hDC, img.m_width, img.m_height);
	setdefaultattribute();
	BitBlt(m_hDC, 0, 0, img.m_width, img.m_height, img.m_hDC, 0, 0, SRCCOPY);
}

IMAGE::~IMAGE() {
	gentexture(false);
	deleteimage();
}

void IMAGE::inittest(const WCHAR* strCallFunction) const {
	if (m_initflag != IMAGE_INIT_FLAG) {
		WCHAR str[60];
		wsprintfW(str, L"Fatal error: read/write at 0x%p. At function '%s'", this, strCallFunction);
		MessageBoxW(graph_setting.hwnd, str, L"EGE ERROR message", MB_ICONSTOP);
		ExitProcess((UINT)grError);
	}
}

void
IMAGE::gentexture(bool gen) {
	if (!gen) {
		if (m_texture != NULL) {
			delete (Gdiplus::Bitmap*)m_texture;
			m_texture = NULL;
		}
	} else {
		if (m_texture != NULL) {
			gentexture(true);
		}
		Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(getwidth(), getheight(), getwidth() * 4, PixelFormat32bppARGB, (BYTE*)getbuffer());
		m_texture = bitmap;
	}
}

int
IMAGE::deleteimage() {
#ifdef EGE_GDIPLUS
	if (NULL!=m_graphics)
		delete m_graphics;
	m_graphics=NULL;
	if (NULL!=m_pen)
		delete m_pen;
	m_pen=NULL;
	if (NULL!=m_brush)
		delete m_brush;
	m_brush=NULL;
#endif

	HBITMAP hbmp  = (HBITMAP)GetCurrentObject(m_hDC, OBJ_BITMAP);
	HBRUSH  hbr   = (HBRUSH)GetCurrentObject(m_hDC, OBJ_BRUSH);
	HPEN    hpen  = (HPEN)GetCurrentObject(m_hDC, OBJ_PEN);
	HFONT   hfont = (HFONT)GetCurrentObject(m_hDC, OBJ_FONT);

	DeleteDC(m_hDC);
	m_hDC = NULL;

	DeleteObject(hbmp);
	DeleteObject(hbr);
	DeleteObject(hpen);
	DeleteObject(hfont);

	return 0;
}

HBITMAP newbitmap(int width, int height, PDWORD* p_bmp_buf) {
	HBITMAP bitmap;
	BITMAPINFO bmi = {{0}};
	PDWORD bmp_buf;

	// 截止到 1
	if (width < 1) width = 1;
	if (height < 1) height = 1;

	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biSizeImage = width * height * 4;

	bitmap = CreateDIBSection(
		NULL,
		&bmi,
		DIB_RGB_COLORS,
		(VOID**)&bmp_buf,
		NULL,
		0
		);

	if (p_bmp_buf) {
		*p_bmp_buf = bmp_buf;
	}

	return bitmap;
}


void IMAGE::initimage(HDC refDC, int width, int height) {
	HDC dc = CreateCompatibleDC(refDC);
	PDWORD bmp_buf;
	HBITMAP bitmap = newbitmap(width, height, &bmp_buf);

	if (bitmap == NULL) {
		internal_panic(L"Fatal Error: Create Bitmap fail in 'IMAGE::initimage'");
	}

	::SelectObject(dc, bitmap);

	m_hDC     = dc;
	m_hBmp    = bitmap;
	m_width   = width;
	m_height  = height;
	m_pBuffer = bmp_buf;

	setviewport(0, 0, m_width, m_height, 1, this);
}

void IMAGE::setdefaultattribute() {
	setcolor(LIGHTGRAY, this);
	setbkcolor(BLACK, this);
	SetBkMode(m_hDC, OPAQUE); //TRANSPARENT);
	setfillstyle(SOLID_FILL, 0, this);
	setlinestyle(PS_SOLID, 0, 1, this);
	settextjustify(LEFT_TEXT, TOP_TEXT, this);
	setfont(16, 0, "SimSun", this);
	enable_anti_alias(false);
}

#ifdef EGE_GDIPLUS

Gdiplus::Graphics*  IMAGE::getGraphics() {
	if (NULL == m_graphics) {
		m_graphics=new Gdiplus::Graphics(m_hDC);
		m_graphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
		m_graphics->SetSmoothingMode(m_aa? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeNone);
	}
	return m_graphics;
}
Gdiplus::Pen* IMAGE::getPen() {
	if (NULL == m_pen) {
		m_pen = new Gdiplus::Pen(m_color,m_linewidth);
		m_pen->SetDashStyle(linestyle_to_dashstyle(m_linestyle.linestyle));
	}
	return m_pen;
}
Gdiplus::Brush* IMAGE::getBrush() {
	if (NULL == m_brush) {
		m_brush = new Gdiplus::SolidBrush(m_fillcolor);
	}
	return m_brush;
}

void IMAGE::set_pattern(Gdiplus::Brush* brush) {
	if (NULL != m_brush) {
		delete m_brush;
	}
	m_brush = brush;
}
#endif

void IMAGE::enable_anti_alias(bool enable){
	m_aa = enable;
#ifdef EGE_GDIPLUS
	if (NULL != m_graphics) {
		m_graphics->SetSmoothingMode(m_aa? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeNone);
	}
#endif
}

int IMAGE::resize_f(int width, int height) {
	inittest(L"IMAGE::resize_f");

	// 截止到 0
	if (width < 0) width = 0;
	if (height < 0) height = 0;

	if (width == m_width && height == m_height) {
		setviewport(0, 0, m_width, m_height, 1, this);
		return 0;
	}

	PDWORD bmp_buf;
	HBITMAP bitmap     = newbitmap(width, height, &bmp_buf);
	HBITMAP old_bitmap = (HBITMAP)SelectObject(this->m_hDC, bitmap);
	DeleteObject(old_bitmap);

	m_hBmp    = bitmap;
	m_width   = width;
	m_height  = height;
	m_pBuffer = bmp_buf;

	setviewport(0, 0, m_width, m_height, 1, this);

	return 0;
}

int IMAGE::resize(int width, int height) {
	inittest(L"IMAGE::resize");
	int ret = this->resize_f(width, height);
	cleardevice(this);
	return ret;
}

IMAGE&
IMAGE::operator = (const IMAGE &img) {
	inittest(L"IMAGE::operator=");
	this->copyimage(&img);
	return *this;
}

void
IMAGE::copyimage(PCIMAGE pSrcImg) {
	inittest(L"IMAGE::copyimage");
	PCIMAGE img = CONVERT_IMAGE_CONST(pSrcImg);
	this->getimage(img, 0, 0, img->getwidth(), img->getheight());
	CONVERT_IMAGE_END;
}

int
IMAGE::getimage(PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight) {
	inittest(L"IMAGE::getimage");
	PCIMAGE img = CONVERT_IMAGE_CONST(pSrcImg);
	this->resize_f(srcWidth, srcHeight);
	BitBlt(this->m_hDC, 0, 0, srcWidth, srcHeight, img->m_hDC, srcX, srcY, SRCCOPY);
	CONVERT_IMAGE_END;
	return grOk;
}

int
IMAGE::getimage(int srcX, int srcY, int srcWidth, int srcHeight) {
	PIMAGE img = CONVERT_IMAGE_CONST(0);
	this->getimage(img, srcX, srcY, srcWidth, srcHeight);
	CONVERT_IMAGE_END;
	return grOk;
}

void
IMAGE::putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, DWORD dwRop) const {
	inittest(L"IMAGE::putimage");
	PIMAGE img = CONVERT_IMAGE(pDstImg);
	BitBlt(img->m_hDC, dstX, dstY, dstWidth, dstHeight, m_hDC, srcX, srcY, dwRop);
	CONVERT_IMAGE_END;
}

void
IMAGE::putimage(PIMAGE pDstImg, int dstX, int dstY, DWORD dwRop) const {
	this->putimage(pDstImg, dstX, dstY, m_width, m_height, 0, 0, dwRop);
}

void
IMAGE::putimage(int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, DWORD dwRop) const {
	PIMAGE img = CONVERT_IMAGE(0);
	this->putimage(img, dstX, dstY, dstWidth, dstHeight, srcX, srcY, dwRop);
	CONVERT_IMAGE_END;
}

void
IMAGE::putimage(int dstX, int dstY, DWORD dwRop) const {
	PIMAGE img = CONVERT_IMAGE(0);
	this->putimage(img, dstX, dstY, dwRop);
	CONVERT_IMAGE_END;
}

int
IMAGE::getimage(LPCSTR filename, int zoomWidth, int zoomHeight) {
	inittest(L"IMAGE::getimage");
	{
		int ret = getimage_pngfile(this, filename);
		//grIOerror means it's not a png file
		if (ret != grIOerror) return ret;
	}

	const std::wstring& wszPath = mb2w(filename);
	return getimage(wszPath.c_str(), zoomWidth, zoomHeight);
}

inline void getimage_from_IPicture(PIMAGE self, IPicture* pPicture) {
	long  lWidth, lHeight, lWidthPixels, lHeightPixels;
	{
		::HDC ScreenDC = ::GetDC(NULL);
		pPicture->get_Width (&lWidth);
		pPicture->get_Height(&lHeight);
		// convert Himetric units to pixels
		lWidthPixels  = ::MulDiv(lWidth,  ::GetDeviceCaps(ScreenDC, LOGPIXELSX), 2540);
		lHeightPixels = ::MulDiv(lHeight, ::GetDeviceCaps(ScreenDC, LOGPIXELSY), 2540);
		::ReleaseDC(NULL, ScreenDC);
	}

	self->resize_f(lWidthPixels, lHeightPixels);
	{
		HDC dc = self->m_hDC;

		pPicture->Render(
			dc,
			0, 0,
			lWidthPixels,
			lHeightPixels,
			0,
			lHeight,
			lWidth,
			-lHeight,
			0
			);
	}
}

int
IMAGE::getimage(LPCWSTR filename, int zoomWidth, int zoomHeight) {
	(void)zoomWidth, (void)zoomHeight; // ignore
	inittest(L"IMAGE::getimage");
	{
		int ret = getimage_pngfile(this, filename);
		//grIOerror means it's not a png file
		if (ret != grIOerror) return ret;
	}

	struct IPicture *pPicture;
	OLECHAR         wszPath[MAX_PATH*2+1];
	WCHAR           szPath[MAX_PATH*2+1] = L"";
	HRESULT         hr;

	if (wcsstr(filename, L"http://")) {
		lstrcpyW(szPath, filename);
	} else if (filename[1] == ':') {
		lstrcpyW(szPath, filename);
	} else {
		GetCurrentDirectoryW(MAX_PATH*2, szPath);
		lstrcatW(szPath, L"\\");
		lstrcatW(szPath, filename);
	}

	lstrcpyW(wszPath, szPath);

	hr = OleLoadPicturePath(
		wszPath,
		0,
		0,
		0,
		IID_IPicture,
		(void**)&pPicture
		);

	if(FAILED(hr)) {
		return grIOerror;
	}

	getimage_from_IPicture(this, pPicture);

	pPicture->Release();

	return grOk;
}

// private function
static
int
saveimagetofile(PCIMAGE img, FILE* fp) {
	BITMAPFILEHEADER bmpfHead = {0};
	BITMAPINFOHEADER bmpinfo = {0};
	int pitch = img->m_width * 3, addbit, y, x, zero = 0;
	addbit = 4 - (pitch & 3);
	if (pitch & 3) pitch = ((pitch + 4) & ~3); else addbit = 0;

	bmpfHead.bfType = *(WORD*)"BM";
	bmpfHead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpfHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pitch * img->m_height;
	bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.biBitCount = 24;
	bmpinfo.biHeight = img->m_height;
	bmpinfo.biWidth = img->m_width;
	bmpinfo.biPlanes = 1;
	bmpinfo.biSizeImage = pitch * img->m_height;
	//bmpinfo.biXPelsPerMeter
	fwrite(&bmpfHead, sizeof(bmpfHead), 1, fp);
	fwrite(&bmpinfo, sizeof(bmpinfo), 1, fp);

	for (y = img->m_height - 1; y >= 0; --y) {
		for (x = 0; x < img->m_width; ++x) {
			DWORD col = img->m_pBuffer[y * img->m_width + x];
			//col = RGBTOBGR(col);
			size_t ret = fwrite(&col, 3, 1, fp);
			if (ret < 1) goto ERROR_BREAK;
		}
		if (addbit > 0)
			fwrite(&zero, addbit, 1, fp);
	}
	return 0;
ERROR_BREAK:
	return grIOerror;
}

int
IMAGE::saveimage(LPCSTR  filename) const {
	FILE *fp = NULL;
	int ret;
	fp = fopen(filename, "wb");
	if (fp == NULL) return grIOerror;
	ret = saveimagetofile(this, fp);
	fclose(fp);
	return ret;
}

int
IMAGE::saveimage(LPCWSTR filename) const {
	FILE *fp = NULL;
	int ret;
	fp = _wfopen(filename, L"wb");
	if (fp == NULL) return grIOerror;
	ret = saveimagetofile(this, fp);
	fclose(fp);
	return ret;
}

void getimage_from_png_struct(PIMAGE self, void* vpng_ptr, void* vinfo_ptr) {
	png_structp png_ptr = (png_structp)vpng_ptr;
	png_infop info_ptr = (png_infop)vinfo_ptr;
	png_set_expand(png_ptr);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR|PNG_TRANSFORM_EXPAND, NULL);
	self->resize_f((int)(info_ptr->width), (int)(info_ptr->height)); //png_get_IHDR

	PDWORD m_pBuffer = self->m_pBuffer;
	const png_uint_32 width = info_ptr->width;
	const png_uint_32 height = info_ptr->height;
	const png_uint_32 depth = info_ptr->pixel_depth;
	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	for (uint32 i = 0; i < height; ++i) {
		if (depth == 24) {
			for (uint32 j = 0; j < width; ++j) {
				m_pBuffer[i * width + j] = 0xFFFFFF & (DWORD&)row_pointers[i][j * 3];
			}
		} else if (depth == 32) {
			for (uint32 j = 0; j < width; ++j) {
				m_pBuffer[i * width + j] = ((DWORD*)(row_pointers[i]))[j];
				if ( (m_pBuffer[i * width + j] & 0xFF000000) == 0) {
					m_pBuffer[i * width + j] = 0;
				}
			}
		}
	}
}

int
IMAGE::getpngimg(FILE* fp) {
	png_structp png_ptr;
	png_infop info_ptr;

	{
		char header[16];
		uint32 number = 8;
		fread(header, 1, number, fp);
		int isn_png = png_sig_cmp((png_const_bytep)header, 0, number);

		if (isn_png) {
			return grIOerror;
		}
		fseek(fp, 0, SEEK_SET);
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		return grAllocError;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return grAllocError;
	}
	
	png_init_io(png_ptr, fp);
	getimage_from_png_struct(this, png_ptr, info_ptr);

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	return grOk;
}

int
IMAGE::savepngimg(FILE* fp, int bAlpha) const {
	unsigned long i, j;
	png_structp png_ptr;
	png_infop info_ptr;
	png_colorp palette;
	png_byte *image;
	png_bytep *row_pointers;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	uint32 pixelsize = bAlpha ? 4 : 3;
	uint32 width = m_width, height = m_height;
	if (png_ptr == NULL) {
		return -1;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, NULL);
		return -1;
	}

	/*if (setjmp(png_jmpbuf(png_ptr)))
	{
	png_destroy_write_struct(&png_ptr, &info_ptr);
	return -1;
	}// */

	png_init_io(png_ptr, fp);

	png_set_IHDR(png_ptr, info_ptr, width, height, 8, bAlpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));
	png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);

	png_write_info(png_ptr, info_ptr);

	png_set_packing(png_ptr);

	image = (png_byte *)malloc(width * height * pixelsize * sizeof(png_byte) + 4);
	if(image == NULL) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return -1;
	}

	row_pointers = (png_bytep *)malloc(height * sizeof(png_bytep));
	if(row_pointers == NULL) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		free(image);
		image = NULL;
		return -1;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; ++j) {
			(DWORD&)image[(i * width  + j) * pixelsize] = RGBTOBGR(m_pBuffer[i * width + j]);
		}
		row_pointers[i] = (png_bytep)image + i * width * pixelsize;
	}

	png_write_image(png_ptr, row_pointers);

	png_write_end(png_ptr, info_ptr);

	png_free(png_ptr, palette);
	palette = NULL;

	png_destroy_write_struct(&png_ptr, &info_ptr);

	free(row_pointers);
	row_pointers = NULL;

	free(image);
	image = NULL;

	return 0;
}

struct MemCursor {
	png_const_bytep data;
	png_size_t      size;
	png_size_t      cur;
};

static void read_data_from_MemCursor(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
	MemCursor& cursor = *(MemCursor*)png_get_io_ptr(png_ptr);
	png_size_t count = min(cursor.size - cursor.cur, byteCountToRead);
	memcpy(outBytes, cursor.data + cursor.cur, count);
	cursor.cur += count;
}

inline int getimage_from_resource(PIMAGE self, HRSRC hrsrc) {
	if (hrsrc) {
		HGLOBAL         hg = LoadResource(0, hrsrc);
		DWORD           dwSize = SizeofResource(0, hrsrc);
		LPVOID          pvRes = LockResource(hg);

		if (dwSize >= 8 && png_check_sig((png_const_bytep)pvRes, 8)) {
			png_structp png_ptr;
			png_infop info_ptr;
			MemCursor cursor;
			cursor.data = (png_const_bytep)pvRes;
			cursor.size = dwSize;
			cursor.cur  = 0;

			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (png_ptr == NULL) {
				return grAllocError;
			}
			info_ptr = png_create_info_struct(png_ptr);
			if (info_ptr == NULL) {
				png_destroy_read_struct(&png_ptr, NULL, NULL);
				return grAllocError;
			}

			png_set_read_fn(png_ptr, &cursor, read_data_from_MemCursor);
			getimage_from_png_struct(self, png_ptr, info_ptr);

			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		} else {
			HGLOBAL         hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwSize);
			LPVOID          pvData;
			struct IPicture *pPicture;
			IStream         *pStm;
			HRESULT         hr;

			if (hGlobal == NULL || (pvData = GlobalLock(hGlobal)) == NULL) {
				return grAllocError;
			}
			memcpy(pvData, pvRes, dwSize);
			GlobalUnlock(hGlobal);
			if (S_OK != CreateStreamOnHGlobal(hGlobal, TRUE, &pStm)) {
				return grNullPointer;
			}

			hr = OleLoadPicture(
				pStm,
				(LONG)dwSize,
				TRUE,
				IID_IPicture,
				(void**)&pPicture
				);

			GlobalFree(hGlobal);

			if(FAILED(hr)) {
				return grIOerror;
			}

			getimage_from_IPicture(self, pPicture);

			pPicture->Release();
		}

		return grOk;
	}

	return grIOerror;
}

int
IMAGE::getimage(LPCSTR pResType, LPCSTR pResName, int zoomWidth, int zoomHeight) {
	(void)zoomWidth, (void)zoomHeight; // ignore
	inittest(L"IMAGE::getimage");
	struct _graph_setting * pg = &graph_setting;
	HRSRC hrsrc = FindResourceA(pg->instance, pResName, pResType);
	return getimage_from_resource(this, hrsrc);
}


int
IMAGE::getimage(LPCWSTR pResType, LPCWSTR pResName, int zoomWidth, int zoomHeight) {
	(void)zoomWidth, (void)zoomHeight; // ignore
	inittest(L"IMAGE::getimage");
	struct _graph_setting * pg = &graph_setting;
	HRSRC hrsrc = FindResourceW(pg->instance, pResName, pResType);
	return getimage_from_resource(this, hrsrc);
}
int
IMAGE::getimage(void* pMem, long size) {
	inittest(L"IMAGE::getimage");

	if (pMem) {
		DWORD           dwSize = size;
		HGLOBAL         hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwSize);
		LPVOID          pvData;
		struct IPicture *pPicture;
		IStream         *pStm;
		HRESULT         hr;

		if (hGlobal == NULL || (pvData = GlobalLock(hGlobal)) == NULL) {
			return grAllocError;
		}
		memcpy(pvData, pMem, dwSize);
		GlobalUnlock(hGlobal);
		if (S_OK != CreateStreamOnHGlobal(hGlobal, TRUE, &pStm)) {
			return grNullPointer;
		}

		hr = OleLoadPicture(
			pStm,
			(LONG)dwSize,
			TRUE,
			IID_IPicture,
			(void**)&pPicture
			);

		GlobalFree(hGlobal);

		if(FAILED(hr)) {
			return grIOerror;
		}

		getimage_from_IPicture(this, pPicture);

		pPicture->Release();

		return grOk;
	}

	return grIOerror;
}

void
IMAGE::putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop) const {
	inittest(L"IMAGE::putimage");
	const PCIMAGE img = CONVERT_IMAGE(pDstImg);
	if (img) {
		SetStretchBltMode(img->m_hDC, COLORONCOLOR);
		StretchBlt(img->m_hDC, dstX, dstY, dstWidth, dstHeight, m_hDC, srcX, srcY, srcWidth, srcHeight, dwRop);
	}
	CONVERT_IMAGE_END;
}

/* private function */
static
void
fix_rect_1size(
	PCIMAGE pdest,
	PCIMAGE psrc,
	int* nXOriginDest,   // x-coord of destination upper-left corner
	int* nYOriginDest,   // y-coord of destination upper-left corner
	int* nXOriginSrc,    // x-coord of source upper-left corner
	int* nYOriginSrc,    // y-coord of source upper-left corner
	int* nWidthSrc,      // width of source rectangle
	int* nHeightSrc      // height of source rectangle
	)
{
	/* prepare viewport region and carry out coordinate transformation */
	struct viewporttype _vpt = pdest->m_vpt;
	*nXOriginDest += _vpt.left;
	*nYOriginDest += _vpt.top;
	/* default value proc */
	if (*nWidthSrc == 0) {
		*nWidthSrc  = psrc->m_width;
		*nHeightSrc = psrc->m_height;
	}
	/* fix src rect */
	if (*nWidthSrc > psrc->m_width) {
		*nWidthSrc -= *nWidthSrc - psrc->m_width;
		*nWidthSrc = psrc->m_width;
	}
	if (*nHeightSrc > psrc->m_height) {
		*nHeightSrc -= *nHeightSrc - psrc->m_height;
		*nHeightSrc = psrc->m_height;
	}
	if (*nXOriginSrc < 0) {
		*nWidthSrc += *nXOriginSrc;
		*nXOriginDest += *nXOriginSrc;
		*nXOriginSrc = 0;
	}
	if (*nYOriginSrc < 0) {
		*nHeightSrc += *nYOriginSrc;
		*nYOriginDest += *nYOriginSrc;
		*nYOriginSrc = 0;
	}
	/* fix dest vpt rect */
	if (*nXOriginDest < _vpt.left) {
		int dx = _vpt.left - *nXOriginDest;
		*nXOriginDest    += dx;
		*nXOriginSrc     += dx;
		*nWidthSrc       -= dx;
	}
	if (*nYOriginDest < _vpt.top) {
		int dy = _vpt.top - *nYOriginDest;
		*nYOriginDest    += dy;
		*nYOriginSrc     += dy;
		*nHeightSrc      -= dy;
	}
	if (*nXOriginDest + *nWidthSrc > _vpt.right) {
		int dx = *nXOriginDest + *nWidthSrc - _vpt.right;
		*nWidthSrc -= dx;
	}
	if (*nYOriginDest + *nHeightSrc > _vpt.bottom) {
		int dy = *nYOriginDest + *nHeightSrc - _vpt.bottom;
		*nHeightSrc -= dy;
	}
}

int
IMAGE::putimage_transparent(
	PIMAGE imgdest,         // handle to dest
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	color_t crTransparent, // color to make transparent
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
) const {
	inittest(L"IMAGE::putimage_transparent");
	const PIMAGE img = CONVERT_IMAGE(imgdest);
	if (img) {
		PCIMAGE imgsrc = this;
		int y, x;
		DWORD ddx, dsx;
		DWORD *pdp, *psp, cr;
		// fix rect
		fix_rect_1size(
			img,
			imgsrc,
			&nXOriginDest,
			&nYOriginDest,
			&nXOriginSrc,
			&nYOriginSrc,
			&nWidthSrc,
			&nHeightSrc
			);
		// draw 
		pdp = img->m_pBuffer + nYOriginDest * img->m_width + nXOriginDest;
		psp = imgsrc->m_pBuffer + nYOriginSrc  * imgsrc->m_width + nXOriginSrc;
		ddx = img->m_width - nWidthSrc;
		dsx = imgsrc->m_width - nWidthSrc;
		cr = crTransparent & 0x00FFFFFF;
		for (y=0; y<nHeightSrc; ++y) {
			for (x=0; x<nWidthSrc; ++x, ++psp, ++pdp) {
				if ((*psp & 0x00FFFFFF) != cr) {
					*pdp = EGECOLORA(*psp, EGEGET_A(*pdp));
				}
			}
			pdp += ddx;
			psp += dsx;
		}
	}
	CONVERT_IMAGE_END;
	return grOk;
}

int
IMAGE::putimage_alphablend(
	PIMAGE imgdest,         // handle to dest
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	unsigned char alpha,    // alpha
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
) const {
	inittest(L"IMAGE::putimage_alphablend");
	const PIMAGE img = CONVERT_IMAGE(imgdest);
	if (img) {
		PCIMAGE imgsrc = this;
		int y, x;
		DWORD ddx, dsx;
		DWORD *pdp, *psp;
		// fix rect
		fix_rect_1size(
			img,
			imgsrc,
			&nXOriginDest,
			&nYOriginDest,
			&nXOriginSrc,
			&nYOriginSrc,
			&nWidthSrc,
			&nHeightSrc
			);
		// draw 
		pdp = img->m_pBuffer + nYOriginDest * img->m_width + nXOriginDest;
		psp = imgsrc->m_pBuffer + nYOriginSrc  * imgsrc->m_width + nXOriginSrc;
		ddx = img->m_width - nWidthSrc;
		dsx = imgsrc->m_width - nWidthSrc;
		for (y=0; y<nHeightSrc; ++y) {
			for (x=0; x<nWidthSrc; ++x, ++psp, ++pdp) {
				DWORD d=*pdp, s=*psp;
				*pdp = alphablend_inline(d, s, alpha);
			}
			pdp += ddx;
			psp += dsx;
		}
	}
	CONVERT_IMAGE_END;
	return grOk;
}

int
IMAGE::putimage_alphatransparent(
	PIMAGE imgdest,         // handle to dest
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	color_t crTransparent, // color to make transparent
	unsigned char alpha,    // alpha
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
) const {
	inittest(L"IMAGE::putimage_alphatransparent");
	const PIMAGE img = CONVERT_IMAGE(imgdest);
	if (img) {
		PCIMAGE imgsrc = this;
		int y, x;
		DWORD ddx, dsx;
		DWORD *pdp, *psp, cr;
		// fix rect
		fix_rect_1size(
			img,
			imgsrc,
			&nXOriginDest,
			&nYOriginDest,
			&nXOriginSrc,
			&nYOriginSrc,
			&nWidthSrc,
			&nHeightSrc
			);
		// draw 
		pdp = img->m_pBuffer + nYOriginDest * img->m_width + nXOriginDest;
		psp = imgsrc->m_pBuffer + nYOriginSrc  * imgsrc->m_width + nXOriginSrc;
		ddx = img->m_width - nWidthSrc;
		dsx = imgsrc->m_width - nWidthSrc;
		cr = crTransparent & 0x00FFFFFF;
		for (y=0; y<nHeightSrc; ++y) {
			for (x=0; x<nWidthSrc; ++x, ++psp, ++pdp) {
				if ((*psp & 0x00FFFFFF) != cr) {
					DWORD d=*pdp, s=*psp;
					*pdp = alphablend_inline(d, s, alpha);
				}
			}
			pdp += ddx;
			psp += dsx;
		}
	}
	CONVERT_IMAGE_END;
	return grOk;
}

int
IMAGE::putimage_withalpha(
	PIMAGE imgdest,         // handle to dest
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
) const {
	inittest(L"IMAGE::putimage_withalpha");
	const PIMAGE img = CONVERT_IMAGE(imgdest);
	if (img) {
		PCIMAGE imgsrc = this;
		int y, x;
		DWORD ddx, dsx;
		DWORD *pdp, *psp;
		// fix rect
		fix_rect_1size(
			img,
			imgsrc,
			&nXOriginDest,
			&nYOriginDest,
			&nXOriginSrc,
			&nYOriginSrc,
			&nWidthSrc,
			&nHeightSrc
			);
		// draw 
		pdp = img->m_pBuffer + nYOriginDest * img->m_width + nXOriginDest;
		psp = imgsrc->m_pBuffer + nYOriginSrc  * imgsrc->m_width + nXOriginSrc;
		ddx = img->m_width - nWidthSrc;
		dsx = imgsrc->m_width - nWidthSrc;
		for (y=0; y<nHeightSrc; ++y) {
			for (x=0; x<nWidthSrc; ++x, ++psp, ++pdp) {
				DWORD d=*pdp, s=*psp;
				DWORD alpha = EGEGET_A(s);
				*pdp = alphablend_inline(d, s, alpha);
			}
			pdp += ddx;
			psp += dsx;
		}
	}
	CONVERT_IMAGE_END;
	return grOk;
}

int
IMAGE::putimage_withalpha(
	PIMAGE imgdest,         // handle to dest
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	int nWidthDest,         // width of destination rectangle
	int nHeightDest,        // height of destination rectangle	
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
) const {
	inittest(L"IMAGE::putimage_withalpha");
	const PIMAGE img = CONVERT_IMAGE(imgdest);
	if (img) {
		PCIMAGE imgsrc = this;
		int x,y;
		DWORD ddx, dsx;
		DWORD *pdp, *psp;
		PIMAGE alphaSrc = newimage(nWidthSrc,nHeightSrc);
		
		BLENDFUNCTION bf;
		// fix rect
		fix_rect_1size(
			img,
			imgsrc,
			&nXOriginDest,
			&nYOriginDest,
			&nXOriginSrc,
			&nYOriginSrc,
			&nWidthSrc,
			&nHeightSrc
			);
		//premultiply alpha channel
		pdp = alphaSrc->m_pBuffer;
		psp = imgsrc->m_pBuffer + nYOriginSrc  * imgsrc->m_width + nXOriginSrc;
		ddx = 0;
		dsx = imgsrc->m_width - nWidthSrc;
		for (y=0; y<nHeightSrc; ++y) {
			for (x=0; x<nWidthSrc; ++x, ++psp, ++pdp) {
				DWORD d=*pdp, s=*psp;
				DWORD alpha = EGEGET_A(s);
				DWORD r = EGEGET_R(s);
				DWORD g = EGEGET_G(s);
				DWORD b = EGEGET_B(s);
				*pdp = EGERGBA(r*alpha>>8, g*alpha>>8,b*alpha>>8,alpha);
			}
			pdp += ddx;
			psp += dsx;
		}		
		
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 0xff;  
		bf.AlphaFormat = AC_SRC_ALPHA;  
		// draw 
		AlphaBlend(img->m_hDC,nXOriginDest,nYOriginDest,nWidthDest,
			nHeightDest,alphaSrc->m_hDC,0,0,
			nWidthSrc,nHeightSrc,bf);
	}
	CONVERT_IMAGE_END;
	return grOk;
}


int
IMAGE::putimage_alphafilter(
	PIMAGE imgdest,         // handle to dest
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	PCIMAGE imgalpha,       // alpha
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
) const {
	inittest(L"IMAGE::putimage_alphafilter");
	const PIMAGE img = CONVERT_IMAGE(imgdest);
	if (img) {
		PCIMAGE imgsrc = this;
		int y, x;
		DWORD ddx, dsx;
		DWORD *pdp, *psp, *pap;
		//DWORD sa = alpha + 1, da = 0xFF - alpha;
		// fix rect
		fix_rect_1size(
			img,
			imgsrc,
			&nXOriginDest,
			&nYOriginDest,
			&nXOriginSrc,
			&nYOriginSrc,
			&nWidthSrc,
			&nHeightSrc
			);
		// draw 
		pdp = img->m_pBuffer + nYOriginDest * img->m_width + nXOriginDest;
		psp = imgsrc->m_pBuffer + nYOriginSrc  * imgsrc->m_width + nXOriginSrc;
		pap = imgalpha->m_pBuffer + nYOriginSrc  * imgalpha->m_width + nXOriginSrc;
		ddx = img->m_width - nWidthSrc;
		dsx = imgsrc->m_width - nWidthSrc;
		for (y=0; y<nHeightSrc; ++y) {
			for (x=0; x<nWidthSrc; ++x, ++psp, ++pdp, ++pap) {
				DWORD d=*pdp, s=*psp;
				DWORD alpha = *pap & 0xFF;
				if (*pap) {
					*pdp = alphablend_inline(d, s, alpha);
				}
			}
			pdp += ddx;
			psp += dsx;
			pap += dsx;
		}
	}
	CONVERT_IMAGE_END;
	return grOk;
}


/* private function */
static
void
fix_rect_0size(
	PIMAGE pdest,
	int* nXOriginDest,   // x-coord of destination upper-left corner
	int* nYOriginDest,   // y-coord of destination upper-left corner
	int* nWidthDest,      // width of destination rectangle
	int* nHeightDest      // height of destination rectangle
	) {
	struct viewporttype _vpt = {0, 0, pdest->m_width, pdest->m_height};
	if (*nWidthDest == 0) {
		*nWidthDest = pdest->m_width;
	}
	if (*nHeightDest == 0) {
		*nHeightDest = pdest->m_height;
	}
	if (*nXOriginDest < _vpt.left) {
		int dx = _vpt.left - *nXOriginDest;
		*nXOriginDest    += dx;
	}
	if (*nYOriginDest < _vpt.top) {
		int dy = _vpt.top - *nYOriginDest;
		*nYOriginDest    += dy;
	}
	if (*nXOriginDest + *nWidthDest > _vpt.right) {
		int dx = *nXOriginDest + *nWidthDest - _vpt.right;
		*nWidthDest -= dx;
	}
	if (*nYOriginDest + *nHeightDest > _vpt.bottom) {
		int dy = *nYOriginDest + *nHeightDest - _vpt.bottom;
		*nHeightDest -= dy;
	}
}

int
IMAGE::imagefilter_blurring_4 (
	int intensity,
	int alpha,
	int nXOriginDest,
	int nYOriginDest,
	int nWidthDest,
	int nHeightDest
) {
	inittest(L"IMAGE::imagefilter_blurring_4");
	struct _graph_setting * pg = &graph_setting;
	DWORD* buff = pg->g_t_buff;
	int x2, y2, ix, iy;
	DWORD *pdp, lsum, sumRB, sumG;
	int ddx, dldx;
	int centerintensity;
	int intensity2, intensity3, intensity4;
	int intensity2f, intensity3f, intensity4f;
	PIMAGE imgdest = this;

	x2 = nXOriginDest + nWidthDest - 1;
	y2 = nYOriginDest + nHeightDest - 1;
	pdp = imgdest->m_pBuffer + nYOriginDest * imgdest->m_width + nXOriginDest;
	ddx = imgdest->m_width - nWidthDest;
	dldx = imgdest->m_width;
	centerintensity = (0xFF - intensity) * alpha >> 8;
	intensity2 = intensity * alpha / 2 >> 8;
	intensity3 = intensity * alpha / 3 >> 8;
	intensity4 = intensity * alpha / 4 >> 8;
	intensity2f = (intensity * alpha % (2 * alpha)) / 2;
	intensity3f = (intensity * alpha % (3 * alpha)) / 3;
	intensity4f = (intensity * alpha % (4 * alpha)) / 4;
	{
		ix = nXOriginDest;
		{
			sumRB = (pdp[dldx]&0xFF00FF) + (pdp[1]&0xFF00FF);
			sumG = (pdp[dldx]&0xFF00) + (pdp[1]&0xFF00);
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity2 + ((sumRB * intensity2f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity2 + ((sumG * intensity2f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		for (ix = nXOriginDest + 1; ix < x2; ++ix) {
			sumRB = (lsum&0xFF00FF) + (pdp[dldx]&0xFF00FF) + (pdp[1]&0xFF00FF);
			sumG = (lsum&0xFF00) + (pdp[dldx]&0xFF00) + (pdp[1]&0xFF00);
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity3 + ((sumRB * intensity3f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity3 + ((sumG * intensity3f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		{
			sumRB = (lsum&0xFF00FF) + (pdp[dldx]&0xFF00FF);
			sumG = (lsum&0xFF00) + (pdp[dldx]&0xFF00);
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity2 + ((sumRB * intensity2f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity2 + ((sumG * intensity2f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		pdp += ddx;
	}
	for (iy = nYOriginDest + 1; iy < y2; ++iy) {
		ix = nXOriginDest;
		{
			sumRB = (buff[ix]&0xFF00FF) + (pdp[dldx]&0xFF00FF) + (pdp[1]&0xFF00FF);
			sumG = (buff[ix]&0xFF00) + (pdp[dldx]&0xFF00) + (pdp[1]&0xFF00);
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity3 + ((sumRB * intensity3f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity3 + ((sumG * intensity3f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		for (ix = nXOriginDest + 1; ix < x2; ++ix) {
			sumRB = (lsum&0xFF00FF) + (buff[ix]&0xFF00FF) + (pdp[dldx]&0xFF00FF) + (pdp[1]&0xFF00FF);
			sumG = (lsum&0xFF00) + (buff[ix]&0xFF00) + (pdp[dldx]&0xFF00) + (pdp[1]&0xFF00);
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity4 + ((sumRB * intensity4f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity4 + ((sumG * intensity4f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		{
			sumRB = (lsum&0xFF00FF) + (buff[ix]&0xFF00FF) + (pdp[dldx]&0xFF00FF);
			sumG = (lsum&0xFF00) + (buff[ix]&0xFF00) + (pdp[dldx]&0xFF00);
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity3 + ((sumRB * intensity3f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity3 + ((sumG * intensity3f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		pdp += ddx;
	}
	{
		ix = nXOriginDest;
		{
			sumRB = (buff[ix]&0xFF00FF) + (pdp[1]&0xFF00FF);
			sumG = (buff[ix]&0xFF00) + (pdp[1]&0xFF00);
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity2 + ((sumRB * intensity2f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity2 + ((sumG * intensity2f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		for (ix = nXOriginDest + 1; ix < x2; ++ix) {
			sumRB = (lsum&0xFF00FF) + (buff[ix]&0xFF00FF) + (pdp[1]&0xFF00FF);
			sumG = (lsum&0xFF00) + (buff[ix]&0xFF00) + (pdp[1]&0xFF00);
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity3 + ((sumRB * intensity3f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity3 + ((sumG * intensity3f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		{
			sumRB = (lsum&0xFF00FF) + (buff[ix]&0xFF00FF);
			sumG = (lsum&0xFF00) + (buff[ix]&0xFF00);
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity2 + ((sumRB * intensity2f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity2 + ((sumG * intensity2f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		//pdp += ddx;
	}
	return grOk;
}

int
IMAGE::imagefilter_blurring_8 (
	int intensity,
	int alpha,
	int nXOriginDest,
	int nYOriginDest,
	int nWidthDest,
	int nHeightDest
) {
	inittest(L"IMAGE::imagefilter_blurring_4");
	struct _graph_setting * pg = &graph_setting;
	DWORD* buff = pg->g_t_buff, lbuf;
	int x2, y2, ix, iy;
	DWORD *pdp, lsum, sumRB, sumG;
	int ddx, dldx;
	int centerintensity;
	int intensity2, intensity3, intensity4;
	int intensity2f, intensity3f, intensity4f;

	PIMAGE imgdest = this;
	x2 = nXOriginDest + nWidthDest - 1;
	y2 = nYOriginDest + nHeightDest - 1;
	pdp = imgdest->m_pBuffer + nYOriginDest * imgdest->m_width + nXOriginDest;
	ddx = imgdest->m_width - nWidthDest;
	dldx = imgdest->m_width;

	centerintensity = (0xFF - intensity) * alpha >> 8;
	intensity2 = intensity * alpha / 3 >> 8;
	intensity3 = intensity * alpha / 5 >> 8;
	intensity4 = intensity * alpha / 8 >> 8;
	intensity2f = (intensity * alpha % (3 * alpha)) / 3;
	intensity3f = (intensity * alpha % (5 * alpha)) / 5;
	intensity4f = (intensity * alpha % (8 * alpha)) / 8;
	{
		ix = nXOriginDest;
		{
			sumRB = (pdp[1]&0xFF00FF)
				+ (pdp[dldx]&0xFF00FF) + (pdp[dldx+1]&0xFF00FF);
			sumG = + (pdp[1]&0xFF00)
				+ (pdp[dldx]&0xFF00) + (pdp[dldx+1]&0xFF00);
			lbuf = buff[ix];
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity2 + ((sumRB * intensity2f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity2 + ((sumG * intensity2f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		for (ix = nXOriginDest + 1; ix < x2; ++ix) {
			sumRB = (lsum&0xFF00FF) + (pdp[1]&0xFF00FF)
				+ (pdp[dldx-1]&0xFF00FF) + (pdp[dldx]&0xFF00FF) + (pdp[dldx+1]&0xFF00FF);
			sumG = (lsum&0xFF00) + (pdp[1]&0xFF00)
				+ (pdp[dldx-1]&0xFF00) + (pdp[dldx]&0xFF00) + (pdp[dldx+1]&0xFF00);
			lbuf = buff[ix];
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity3 + ((sumRB * intensity3f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity3 + ((sumG * intensity3f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		{
			sumRB = (lsum&0xFF00FF)
				+ (pdp[dldx-1]&0xFF00FF) + (pdp[dldx]&0xFF00FF);
			sumG = (lsum&0xFF00)
				+ (pdp[dldx-1]&0xFF00) + (pdp[dldx]&0xFF00);
			lbuf = buff[ix];
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity2 + ((sumRB * intensity2f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity2 + ((sumG * intensity2f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		pdp += ddx;
	}
	for (iy = nYOriginDest + 1; iy < y2; ++iy) {
		ix = nXOriginDest;
		{
			sumRB = (buff[ix]&0xFF00FF) + (buff[ix+1]&0xFF00FF)
				+ (pdp[1]&0xFF00FF)
				+ (pdp[dldx]&0xFF00FF) + (pdp[dldx+1]&0xFF00FF);
			sumG = (buff[ix]&0xFF00) + (buff[ix+1]&0xFF00)
				+ (pdp[1]&0xFF00)
				+ (pdp[dldx]&0xFF00) + (pdp[dldx+1]&0xFF00);
			lbuf = buff[ix];
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity3 + ((sumRB * intensity3f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity3 + ((sumG * intensity3f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		for (ix = nXOriginDest + 1; ix < x2; ++ix) {
			sumRB = (lbuf&0xFF00FF) + (buff[ix]&0xFF00FF) + (buff[ix+1]&0xFF00FF)
				+ (lsum&0xFF00FF) + (pdp[1]&0xFF00FF)
				+ (pdp[dldx-1]&0xFF00FF) + (pdp[dldx]&0xFF00FF) + (pdp[dldx+1]&0xFF00FF);
			sumG = (lbuf&0xFF00) + (buff[ix]&0xFF00) + (buff[ix+1]&0xFF00)
				+ (lsum&0xFF00) + (pdp[1]&0xFF00)
				+ (pdp[dldx-1]&0xFF00) + (pdp[dldx]&0xFF00) + (pdp[dldx+1]&0xFF00);
			lbuf = buff[ix];
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity4 + ((sumRB * intensity4f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity4 + ((sumG * intensity4f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		{
			sumRB = (lbuf&0xFF00FF) + (buff[ix]&0xFF00FF)
				+ (lsum&0xFF00FF)
				+ (lbuf&0xFF00FF) + (pdp[dldx]&0xFF00FF);
			sumG = (buff[ix-1]&0xFF00) + (buff[ix]&0xFF00)
				+ (lsum&0xFF00)
				+ (pdp[dldx-1]&0xFF00) + (pdp[dldx]&0xFF00);
			lbuf = buff[ix];
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity3 + ((sumRB * intensity3f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity3 + ((sumG * intensity3f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		pdp += ddx;
	}
	{
		ix = nXOriginDest;
		{
			sumRB = (buff[ix]&0xFF00FF) + (buff[ix+1]&0xFF00FF)
				+ (pdp[1]&0xFF00FF);
			sumG = (buff[ix]&0xFF00) + (buff[ix+1]&0xFF00)
				+ (pdp[1]&0xFF00);
			lbuf = buff[ix];
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity2 + ((sumRB * intensity2f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity2 + ((sumG * intensity2f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		for (ix = nXOriginDest + 1; ix < x2; ++ix) {
			sumRB = (lbuf&0xFF00FF) + (buff[ix]&0xFF00FF) + (buff[ix+1]&0xFF00FF)
				+ (lsum&0xFF00FF) + (pdp[1]&0xFF00FF);
			sumG = (lbuf&0xFF00) + (buff[ix]&0xFF00) + (buff[ix+1]&0xFF00)
				+ (lsum&0xFF00) + (pdp[1]&0xFF00);
			lbuf = buff[ix];
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity3 + ((sumRB * intensity3f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity3 + ((sumG * intensity3f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		{
			sumRB = (lbuf&0xFF00FF) + (buff[ix]&0xFF00FF)
				+ (lsum&0xFF00FF);
			sumG = (lbuf&0xFF00) + (buff[ix]&0xFF00)
				+ (lsum&0xFF00);
			lbuf = buff[ix];
			buff[ix] = lsum = *pdp;
			sumRB = sumRB * intensity2 + ((sumRB * intensity2f >> 8) & 0xFF00FF) + (lsum&0xFF00FF) * centerintensity;
			sumG = sumG * intensity2 + ((sumG * intensity2f >> 8)) + (lsum&0xFF00) * centerintensity;
			*pdp = ((sumRB & 0xFF00FF00) | (sumG & 0xFF0000)) >> 8;
			++pdp;
		}
		//pdp += ddx;
	}
	return grOk;
}

int
IMAGE::imagefilter_blurring (
	int intensity,
	int alpha,
	int nXOriginDest,
	int nYOriginDest,
	int nWidthDest,
	int nHeightDest
) {
	inittest(L"IMAGE::imagefilter_blurring");
	PIMAGE imgdest = this;

	fix_rect_0size(imgdest, &nXOriginDest, &nYOriginDest, &nWidthDest, &nHeightDest);
	if (nWidthDest <= 0 || nHeightDest <= 0) {
		return grInvalidRegion;
	}

	if (alpha < 0 || alpha > 0x100) {
		alpha = 0x100;
	}

	if (intensity <= 0x80) {
		imagefilter_blurring_4(
			intensity * 2,
			alpha,
			nXOriginDest,
			nYOriginDest,
			nWidthDest,
			nHeightDest
		);
	} else {
		imagefilter_blurring_8(
			(intensity - 0x80) * 2,
			alpha,
			nXOriginDest,
			nYOriginDest,
			nWidthDest,
			nHeightDest
		);
	}
	return grOk;
}

int 
IMAGE::putimage_rotate(
	PIMAGE imgtexture,
	int nXOriginDest,
	int nYOriginDest,
	float centerx,
	float centery,
	float radian,
	int btransparent,           // transparent (1) or not (0)
	int alpha,                  // in range[0, 256], alpha== -1 means no alpha
	int smooth
)
{
	return ege::putimage_rotate(this, imgtexture, nXOriginDest, nYOriginDest, centerx, centery, radian, btransparent, alpha, smooth);
}

int 
IMAGE::putimage_rotatezoom(
	PIMAGE imgtexture,
	int nXOriginDest,
	int nYOriginDest,
	float centerx,
	float centery,
	float radian,
	float zoom,
	int btransparent,           // transparent (1) or not (0)
	int alpha,                  // in range[0, 256], alpha== -1 means no alpha
	int smooth
	)
{
	return ege::putimage_rotatezoom(this, imgtexture, nXOriginDest, nYOriginDest, centerx, centery, radian, zoom, btransparent, alpha, smooth);
}


#define BILINEAR_INTERPOLATION(s, LT, RT, LB, RB, x, y) \
{\
	alphaA = (int)(x*0x100);\
	alphaB = 0xFF - alphaA;\
	Trb = (((LT & 0xFF00FF) * alphaB + (RT & 0xFF00FF) * alphaA) & 0xFF00FF00) >> 8;\
	Tg =  (((LT & 0xFF00) * alphaB + (RT & 0xFF00) * alphaA) & 0xFF0000) >> 8;\
	Brb = (((LB & 0xFF00FF) * alphaB + (RB & 0xFF00FF) * alphaA) & 0xFF00FF00) >> 8;\
	Bg =  (((LB & 0xFF00) * alphaB + (RB & 0xFF00) * alphaA) & 0xFF0000) >> 8;\
	alphaA = (int)(y*0x100);\
	alphaB = 0xFF - alphaA;\
	crb = ((Trb * alphaB + Brb * alphaA) & 0xFF00FF00);\
	cg =  ((Tg * alphaB + Bg * alphaA) & 0xFF0000);\
	s = (crb | cg) >> 8;\
}

/* ege 3d 结构 */
struct point2d {
	float x;
	float y;
};

struct point3d {
	float x;
	float y;
	float z;
};

struct vector2d {
	struct point2d p[2];
};

struct vector3d {
	struct point3d p[2];
};

typedef struct trangle2d {
	struct point2d p[3];
	int color;
}trangle2d;

struct trangle3d {
	struct point3d p[3];
	int color;
};

/* private funcion */
static
int float2int(float f) {
	if (f>=0) {
		return (int)(f + .5);
	} else {
		return (int)(f - .5);
	}
}

/* private funcion */
static
void
draw_flat_scanline(PIMAGE dc_dest, const struct vector2d* vt, PCIMAGE dc_src, const struct vector2d* svt, int x1, int x2) {
	float dw = vt->p[1].x - vt->p[0].x, rw = svt->p[1].x - svt->p[0].x;
	int s = float2int((float)vt->p[0].x), e = float2int((float)vt->p[1].x), y = float2int((float)vt->p[0].y), w = e - s;
	DWORD* lp_dest_bmp_byte = (DWORD*)dc_dest->getbuffer();
	DWORD* lp_src_bmp_byte = (DWORD*)dc_src->getbuffer();
	int dest_w = dc_dest->getwidth();
	int src_w = dc_src->getwidth();
	if (w>0) {
		int i, bx = s;
		struct vector2d _svt = *svt;
		_svt.p[0].x += (float)((s - vt->p[0].x) * rw / dw);
		_svt.p[1].x += (float)((e - vt->p[1].x) * rw / dw);
		{
			float curx = _svt.p[0].x + .5f, cury = _svt.p[0].y + .5f;
			float dx = (_svt.p[1].x - _svt.p[0].x) / w;
			float dy = (_svt.p[1].y - _svt.p[0].y) / w;

			if (s<x1) {
				s = x1;
			}
			if (e>x2) {
				e = x2;
			}
			curx += (s - bx) * dx;
			cury += (s - bx) * dy;

			for (i = s; i<e; ++i, curx += dx, cury += dy) {
				lp_dest_bmp_byte[dest_w * y + i] = lp_src_bmp_byte[src_w * (int)(cury) + (int)(curx)];
			}
		}
	}
}

/* private funcion */
static
void
draw_flat_scanline_transparent(PIMAGE dc_dest, const struct vector2d* vt, PCIMAGE dc_src, const struct vector2d* svt, int x1, int x2) {
	float dw = vt->p[1].x - vt->p[0].x, rw = svt->p[1].x - svt->p[0].x;
	int s = (int)(vt->p[0].x+.5), e = (int)(vt->p[1].x+.5), y = (int)(vt->p[0].y+.5), w = e - s;
	DWORD* lp_dest_bmp_byte = (DWORD*)dc_dest->getbuffer();
	DWORD* lp_src_bmp_byte = (DWORD*)dc_src->getbuffer();
	DWORD  col;
	int dest_w = dc_dest->getwidth();
	int src_w = dc_src->getwidth();
	if (w>0) {
		int i, bx = s;
		struct vector2d _svt = *svt;
		_svt.p[0].x += (float)((s - vt->p[0].x) * rw / dw);
		_svt.p[1].x += (float)((e - vt->p[1].x) * rw / dw);
		{
			float curx = _svt.p[0].x + .5f, cury = _svt.p[0].y + .5f;
			float dx = (_svt.p[1].x - _svt.p[0].x) / w;
			float dy = (_svt.p[1].y - _svt.p[0].y) / w;

			if (s<x1) {
				s = x1;
			}
			if (e>x2) {
				e = x2;
			}
			curx += (s - bx) * dx;
			cury += (s - bx) * dy;

			for (i = s; i<e; ++i, curx += dx, cury += dy) {
				col = lp_src_bmp_byte[src_w * (int)(cury) + (int)(curx)];
				if (col) {
					lp_dest_bmp_byte[dest_w * y + i] = col;
				}
			}
		}
	}
}

/* private funcion */
static
void
draw_flat_scanline_alpha(PIMAGE dc_dest, const struct vector2d* vt, PCIMAGE dc_src, const struct vector2d* svt, int x1, int x2, int alpha) {
	float dw = vt->p[1].x - vt->p[0].x, rw = svt->p[1].x - svt->p[0].x;
	int s = float2int((float)vt->p[0].x), e = float2int((float)vt->p[1].x), y = float2int((float)vt->p[0].y), w = e - s;
	DWORD* lp_dest_bmp_byte = (DWORD*)dc_dest->getbuffer();
	DWORD* lp_src_bmp_byte = (DWORD*)dc_src->getbuffer();
	DWORD sa = alpha, da = 0xFF - sa;

	int dest_w = dc_dest->getwidth();
	int src_w = dc_src->getwidth();
	if (w>0) {
		int i, bx = s;
		struct vector2d _svt = *svt;
		_svt.p[0].x += (float)((s - vt->p[0].x) * rw / dw);
		_svt.p[1].x += (float)((e - vt->p[1].x) * rw / dw);
		{
			float curx = _svt.p[0].x + .5f, cury = _svt.p[0].y + .5f;
			float dx = (_svt.p[1].x - _svt.p[0].x) / w;
			float dy = (_svt.p[1].y - _svt.p[0].y) / w;

			if (s<x1) {
				s = x1;
			}
			if (e>x2) {
				e = x2;
			}
			curx += (s - bx) * dx;
			cury += (s - bx) * dy;

			for (i = s; i<e; ++i, curx += dx, cury += dy) {
				DWORD d = lp_dest_bmp_byte[dest_w * y + i], s = lp_src_bmp_byte[src_w * (int)(cury) + (int)(curx)];
				d = ((d&0xFF00FF)*da & 0xFF00FF00) | ((d&0xFF00)*da >> 16 << 16);
				s = ((s&0xFF00FF)*sa & 0xFF00FF00) | ((s&0xFF00)*sa >> 16 << 16);
				lp_dest_bmp_byte[dest_w * y + i] = (d + s) >> 8;
			}
		}
	}
}

/* private funcion */
static
void
draw_flat_scanline_alphatrans(PIMAGE dc_dest, const struct vector2d* vt, PCIMAGE dc_src, const struct vector2d* svt, int x1, int x2, int alpha) {
	float dw = vt->p[1].x - vt->p[0].x, rw = svt->p[1].x - svt->p[0].x;
	int s = (int)(vt->p[0].x+.5), e = (int)(vt->p[1].x+.5), y = (int)(vt->p[0].y+.5), w = e - s;
	DWORD* lp_dest_bmp_byte = (DWORD*)dc_dest->getbuffer();
	DWORD* lp_src_bmp_byte = (DWORD*)dc_src->getbuffer();
	DWORD sa = alpha, da = 0xFF - sa;

	int dest_w = dc_dest->getwidth();
	int src_w = dc_src->getwidth();
	if (w>0) {
		int i, bx = s;
		struct vector2d _svt = *svt;
		_svt.p[0].x += (float)((s - vt->p[0].x) * rw / dw);
		_svt.p[1].x += (float)((e - vt->p[1].x) * rw / dw);
		{
			float curx = _svt.p[0].x + .5f, cury = _svt.p[0].y + .5f;
			float dx = (_svt.p[1].x - _svt.p[0].x) / w;
			float dy = (_svt.p[1].y - _svt.p[0].y) / w;

			if (s<x1) {
				s = x1;
			}
			if (e>x2) {
				e = x2;
			}
			curx += (s - bx) * dx;
			cury += (s - bx) * dy;

			for (i = s; i<e; ++i, curx += dx, cury += dy) {
				DWORD s = lp_src_bmp_byte[src_w * (int)(cury) + (int)(curx)];
				if (s) {
					DWORD d = lp_dest_bmp_byte[dest_w * y + i];
					d = ((d&0xFF00FF)*da & 0xFF00FF00) | ((d&0xFF00)*da >> 16 << 16);
					s = ((s&0xFF00FF)*sa & 0xFF00FF00) | ((s&0xFF00)*sa >> 16 << 16);
					lp_dest_bmp_byte[dest_w * y + i] = (d + s) >> 8;
				}
			}
		}
	}
}

/* private funcion */
/* static
color_t bilinear_interpolation(color_t LT, color_t RT, color_t LB, color_t RB, double x, double y) {
	color_t Trb, Tg, Brb, Bg, crb, cg;
	color_t alphaA, alphaB;
	alphaA = (color_t)(x * 0x100);
	alphaB = 0xFF - alphaA;
	Trb = (((LT & 0xFF00FF) * alphaB + (RT & 0xFF00FF) * alphaA) & 0xFF00FF00) >> 8;
	Tg =  (((LT & 0xFF00) * alphaB + (RT & 0xFF00) * alphaA) & 0xFF0000) >> 8;
	Brb = (((LB & 0xFF00FF) * alphaB + (RB & 0xFF00FF) * alphaA) & 0xFF00FF00) >> 8;
	Bg =  (((LB & 0xFF00) * alphaB + (RB & 0xFF00) * alphaA) & 0xFF0000) >> 8;
	alphaA = (color_t)(y * 0x100);
	alphaB = 0xFF - alphaA;
	crb = ((Trb * alphaB + Brb * alphaA) & 0xFF00FF00);
	cg =  ((Tg * alphaB + Bg * alphaA) & 0xFF0000);
	return (crb | cg) >> 8;
}// */

/* private funcion */
static
void
draw_flat_scanline_s(PIMAGE dc_dest, const struct vector2d* vt, PCIMAGE dc_src, const struct vector2d* svt, int x1, int x2) {
	float dw = vt->p[1].x - vt->p[0].x, rw = svt->p[1].x - svt->p[0].x;
	int s = float2int((float)vt->p[0].x), e = float2int((float)vt->p[1].x), y = float2int((float)vt->p[0].y), w = e - s;
	DWORD* lp_dest_bmp_byte = (DWORD*)dc_dest->getbuffer();
	DWORD* lp_src_bmp_byte = (DWORD*)dc_src->getbuffer();
	DWORD Trb, Tg, Brb, Bg, crb, cg;
	DWORD alphaA, alphaB;
	int dest_w = dc_dest->getwidth();
	int src_w = dc_src->getwidth();
	//int src_h = dc_src->h;

	if (w>0) {
		int i, bx = s;
		struct vector2d _svt = *svt;
		_svt.p[0].x += (float)((s - vt->p[0].x) * rw / dw);
		_svt.p[1].x += (float)((e - vt->p[1].x) * rw / dw);
		{
			float curx = _svt.p[0].x, cury = _svt.p[0].y;
			float dx = (_svt.p[1].x - _svt.p[0].x) / w;
			float dy = (_svt.p[1].y - _svt.p[0].y) / w;

			if (s<x1) {
				s = x1;
			}
			if (e>x2) {
				e = x2;
			}
			curx += (s - bx) * dx;
			cury += (s - bx) * dy;

			for (i = s; i<e; ++i, curx += dx, cury += dy) {
				int ix = (int)curx, iy = (int)cury;
				double fx = curx - ix, fy = cury - iy;
				DWORD* lp_src_byte = lp_src_bmp_byte + src_w * iy + ix;
				DWORD col;
				BILINEAR_INTERPOLATION(
					col,
					lp_src_byte[0],
					lp_src_byte[1],
					lp_src_byte[src_w],
					lp_src_byte[src_w+1],
					fx,
					fy
					);
				lp_dest_bmp_byte[dest_w * y + i] = col;
			}
		}
	}
}

/* private funcion */
static
void
draw_flat_scanline_transparent_s(PIMAGE dc_dest, const struct vector2d* vt, PCIMAGE dc_src, const struct vector2d* svt, int x1, int x2) {
	float dw = vt->p[1].x - vt->p[0].x, rw = svt->p[1].x - svt->p[0].x;
	int s = float2int((float)vt->p[0].x), e = float2int((float)vt->p[1].x), y = float2int((float)vt->p[0].y), w = e - s;
	DWORD* lp_dest_bmp_byte = (DWORD*)dc_dest->getbuffer();
	DWORD* lp_src_bmp_byte = (DWORD*)dc_src->getbuffer();
	DWORD Trb, Tg, Brb, Bg, crb, cg;
	DWORD alphaA, alphaB;
	int dest_w = dc_dest->getwidth();
	int src_w = dc_src->getwidth();
	//int src_h = dc_src->h;

	if (w>0) {
		int i, bx = s;
		struct vector2d _svt = *svt;
		_svt.p[0].x += (float)((s - vt->p[0].x) * rw / dw);
		_svt.p[1].x += (float)((e - vt->p[1].x) * rw / dw);
		{
			float curx = _svt.p[0].x, cury = _svt.p[0].y;
			float dx = (_svt.p[1].x - _svt.p[0].x) / w;
			float dy = (_svt.p[1].y - _svt.p[0].y) / w;

			if (s<x1) {
				s = x1;
			}
			if (e>x2) {
				e = x2;
			}
			curx += (s - bx) * dx;
			cury += (s - bx) * dy;

			for (i = s; i<e; ++i, curx += dx, cury += dy) {
				int ix = (int)curx, iy = (int)cury;
				float fx = curx - ix, fy = cury - iy;
				DWORD* lp_src_byte = lp_src_bmp_byte + src_w * iy + ix;
				DWORD col;
				BILINEAR_INTERPOLATION(
					col,
					lp_src_byte[0],
					lp_src_byte[1],
					lp_src_byte[src_w],
					lp_src_byte[src_w+1],
					fx,
					fy
					);
				if (col) {
					lp_dest_bmp_byte[dest_w * y + i] = col;
				}
			}
		}
	}
}

/* private funcion */
static
void
draw_flat_scanline_alpha_s(PIMAGE dc_dest, const struct vector2d* vt, PCIMAGE dc_src, const struct vector2d* svt, int x1, int x2, int alpha) {
	float dw = vt->p[1].x - vt->p[0].x, rw = svt->p[1].x - svt->p[0].x;
	int s = float2int((float)vt->p[0].x), e = float2int((float)vt->p[1].x), y = float2int((float)vt->p[0].y), w = e - s;
	DWORD* lp_dest_bmp_byte = (DWORD*)dc_dest->getbuffer();
	DWORD* lp_src_bmp_byte = (DWORD*)dc_src->getbuffer();
	DWORD Trb, Tg, Brb, Bg, crb, cg;
	DWORD alphaA, alphaB;
	DWORD sa = alpha, da = 0xFF - sa;

	int dest_w = dc_dest->getwidth();
	int src_w = dc_src->getwidth();
	//int src_h = dc_src->h;

	if (w>0) {
		int i, bx = s;
		struct vector2d _svt = *svt;
		_svt.p[0].x += (float)((s - vt->p[0].x) * rw / dw);
		_svt.p[1].x += (float)((e - vt->p[1].x) * rw / dw);
		{
			float curx = _svt.p[0].x, cury = _svt.p[0].y;
			float dx = (_svt.p[1].x - _svt.p[0].x) / w;
			float dy = (_svt.p[1].y - _svt.p[0].y) / w;

			if (s<x1) {
				s = x1;
			}
			if (e>x2) {
				e = x2;
			}
			curx += (s - bx) * dx;
			cury += (s - bx) * dy;

			for (i = s; i<e; ++i, curx += dx, cury += dy) {
				int ix = (int)curx, iy = (int)cury;
				float fx = curx - ix, fy = cury - iy;
				DWORD* lp_src_byte = lp_src_bmp_byte + src_w * iy + ix;
				DWORD col;
				BILINEAR_INTERPOLATION(
					col,
					lp_src_byte[0],
					lp_src_byte[1],
					lp_src_byte[src_w],
					lp_src_byte[src_w+1],
					fx,
					fy
					);
				{
					DWORD d = lp_dest_bmp_byte[dest_w * y + i];
					d = (((d&0xFF00FF)*da & 0xFF00FF00) | ((d&0xFF00)*da & 0xFF0000)) >> 8;
					col = (((col&0xFF00FF)*sa & 0xFF00FF00) | ((col&0xFF00)*sa & 0xFF0000)) >> 8;
					lp_dest_bmp_byte[dest_w * y + i] = d + col;
				}
			}
		}
	}
}

/* private funcion */
static
void
draw_flat_scanline_alphatrans_s(PIMAGE dc_dest, const struct vector2d* vt, PCIMAGE dc_src, const struct vector2d* svt, int x1, int x2, int alpha) {
	float dw = vt->p[1].x - vt->p[0].x, rw = svt->p[1].x - svt->p[0].x;
	int s = float2int((float)vt->p[0].x), e = float2int((float)vt->p[1].x), y = float2int((float)vt->p[0].y), w = e - s;
	DWORD* lp_dest_bmp_byte = (DWORD*)dc_dest->getbuffer();
	DWORD* lp_src_bmp_byte = (DWORD*)dc_src->getbuffer();
	DWORD Trb, Tg, Brb, Bg, crb, cg;
	DWORD alphaA, alphaB;
	DWORD sa = alpha, da = 0xFF - sa;

	int dest_w = dc_dest->getwidth();
	int src_w = dc_src->getwidth();
	//int src_h = dc_src->h;

	if (w>0) {
		int i, bx = s;
		struct vector2d _svt = *svt;
		_svt.p[0].x += (float)((s - vt->p[0].x) * rw / dw);
		_svt.p[1].x += (float)((e - vt->p[1].x) * rw / dw);
		{
			float curx = _svt.p[0].x, cury = _svt.p[0].y;
			float dx = (_svt.p[1].x - _svt.p[0].x) / w;
			float dy = (_svt.p[1].y - _svt.p[0].y) / w;

			if (s<x1) {
				s = x1;
			}
			if (e>x2) {
				e = x2;
			}
			curx += (s - bx) * dx;
			cury += (s - bx) * dy;

			for (i = s; i<e; ++i, curx += dx, cury += dy) {
				int ix = (int)curx, iy = (int)cury;
				float fx = curx - ix, fy = cury - iy;
				DWORD* lp_src_byte = lp_src_bmp_byte + src_w * iy + ix;
				DWORD col;
				BILINEAR_INTERPOLATION(
					col,
					lp_src_byte[0],
					lp_src_byte[1],
					lp_src_byte[src_w],
					lp_src_byte[src_w+1],
					fx,
					fy
					);
				if (col) {
					DWORD d = lp_dest_bmp_byte[dest_w * y + i];
					d = (((d&0xFF00FF)*da & 0xFF00FF00) | ((d&0xFF00)*da & 0xFF0000)) >> 8;
					col = (((col&0xFF00FF)*sa & 0xFF00FF00) | ((col&0xFF00)*sa & 0xFF0000)) >> 8;
					lp_dest_bmp_byte[dest_w * y + i] = d + col;
				}
			}
		}
	}
}

/* private funcion */
static
void
draw_flat_trangle_alpha(PIMAGE dc_dest, const struct trangle2d* dt, PCIMAGE dc_src, const struct trangle2d* tt, int x1, int y1, int x2, int y2, int transparent, int alpha) {
	struct trangle2d  t2d = *dt;
	struct trangle2d  t3d = *tt;
	int b_alpha;
	//int width = x2 - x1, height = y2 - y1;

	if (alpha >= 0 && alpha < 0x100) {
		b_alpha = 1;
	} else {
		b_alpha = 0;
	}
	{
		struct point2d _t;
		struct point2d _t3;

		if (t2d.p[1].y > t2d.p[2].y) {
			SWAP(t2d.p[1], t2d.p[2], _t);
			SWAP(t3d.p[1], t3d.p[2], _t3);
		}
		if (t2d.p[0].y > t2d.p[1].y) {
			SWAP(t2d.p[0], t2d.p[1], _t);
			SWAP(t3d.p[0], t3d.p[1], _t3);
		}
		if (t2d.p[1].y > t2d.p[2].y) {
			SWAP(t2d.p[1], t2d.p[2], _t);
			SWAP(t3d.p[1], t3d.p[2], _t3);
		}
	}
	{
		float dd;
		int s = float2int((float)t2d.p[0].y), e = float2int((float)t2d.p[2].y), h, m = float2int((float)t2d.p[1].y);
		int rs, re;
		int i, lh, rh;
		float dm = t2d.p[1].y, dh;
		struct point2d pl, pr, pt;
		struct point2d spl, spr;

		pl.x = t2d.p[1].x - t2d.p[0].x;
		pr.x = t2d.p[2].x - t2d.p[0].x;
		pl.y = t2d.p[1].y - t2d.p[0].y;
		pr.y = t2d.p[2].y - t2d.p[0].y;
		spl.x = t3d.p[1].x - t3d.p[0].x;
		spr.x = t3d.p[2].x - t3d.p[0].x;
		spl.y = t3d.p[1].y - t3d.p[0].y;
		spr.y = t3d.p[2].y - t3d.p[0].y;
		dh = dm - s;
		h = m - s;
		rs = s;
		if (s < y1) {
			s = y1;
		}
		if (m >= y2) {
			m = y2;
		}
		/*if (pl.y > pr.y) {
		dd = pr.y / pl.y;
		pl.x *= dd;
		spl.x *= dd;
		spl.y *= dd;
		} else {
		dd = pl.y / pr.y;
		pr.x *= dd;
		spr.x *= dd;
		spr.y *= dd;
		}// */
		if (pl.x > pr.x) {
			SWAP(pl, pr, pt);
			SWAP(spl, spr, pt);
		} else {
			;
		}
		lh = float2int((float)(pl.y + t2d.p[0].y)) - float2int((float)(t2d.p[0].y));
		rh = float2int((float)(pr.y + t2d.p[0].y)) - float2int((float)(t2d.p[0].y));
		if (h > 0) {
			for (i=s; i<m; ++i) {
				struct vector2d vt;
				struct vector2d svt;
				//float dt = (float)(i - rs) / h;
				float dlt = (float)(i - rs) / lh;
				float drt = (float)(i - rs) / rh;

				/*
				vt.p[0].x = t2d.p[0].x + pl.x * dt;
				vt.p[1].x = t2d.p[0].x + pr.x * dt;
				vt.p[0].y = vt.p[1].y = i;

				svt.p[0].x = t3d.p[0].x + spl.x * dt;
				svt.p[1].x = t3d.p[0].x + spr.x * dt;
				svt.p[0].y = t3d.p[0].y + spl.y * dt;
				svt.p[1].y = t3d.p[0].y + spr.y * dt;
				// */
				vt.p[0].x = t2d.p[0].x + pl.x * dlt;
				vt.p[1].x = t2d.p[0].x + pr.x * drt;
				vt.p[0].y = vt.p[1].y = (float)(i);

				svt.p[0].x = t3d.p[0].x + spl.x * dlt;
				svt.p[1].x = t3d.p[0].x + spr.x * drt;
				svt.p[0].y = t3d.p[0].y + spl.y * dlt;
				svt.p[1].y = t3d.p[0].y + spr.y * drt;

				if (b_alpha) {
					if (transparent) {
						draw_flat_scanline_alphatrans(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
					} else {
						draw_flat_scanline_alpha(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
					}
				} else {
					if (transparent) {
						draw_flat_scanline_transparent(dc_dest, &vt, dc_src, &svt, x1, x2);
					} else {
						draw_flat_scanline(dc_dest, &vt, dc_src, &svt, x1, x2);
					}
				}
			}
		}
		if (pl.y > pr.y) {
			dd = pr.y / pl.y;
			pl.x *= dd;
			spl.x *= dd;
			spl.y *= dd;
		} else {
			dd = pl.y / pr.y;
			pr.x *= dd;
			spr.x *= dd;
			spr.y *= dd;
		}
		if (m >= y1 && m < y2 && m<e) {
			struct vector2d vt;
			struct vector2d svt;

			vt.p[0].x = t2d.p[0].x + pl.x;
			vt.p[1].x = t2d.p[0].x + pr.x;
			vt.p[0].y = vt.p[1].y = (float)(m);

			svt.p[0].x = t3d.p[0].x + spl.x;
			svt.p[1].x = t3d.p[0].x + spr.x;
			svt.p[0].y = t3d.p[0].y + spl.y;
			svt.p[1].y = t3d.p[0].y + spr.y;

			if (b_alpha) {
				if (transparent) {
					draw_flat_scanline_alphatrans(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
				} else {
					draw_flat_scanline_alpha(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
				}
			} else {
				if (transparent) {
					draw_flat_scanline_transparent(dc_dest, &vt, dc_src, &svt, x1, x2);
				} else {
					draw_flat_scanline(dc_dest, &vt, dc_src, &svt, x1, x2);
				}
			}
		}// */
		pl.x = t2d.p[0].x - t2d.p[2].x;
		pr.x = t2d.p[1].x - t2d.p[2].x;
		pl.y = t2d.p[0].y - t2d.p[2].y;
		pr.y = t2d.p[1].y - t2d.p[2].y;
		spl.x = t3d.p[0].x - t3d.p[2].x;
		spr.x = t3d.p[1].x - t3d.p[2].x;
		spl.y = t3d.p[0].y - t3d.p[2].y;
		spr.y = t3d.p[1].y - t3d.p[2].y;

		dh = e - dm;
		h = e - m;
		re = e;
		if (m < y1) {
			m = y1 - 1;
		}
		if (e >= y2) {
			e = y2 - 1;
		}
		/*if (pl.y < pr.y) {
		dd = pr.y / pl.y;
		pl.x *= dd;
		spl.x *= dd;
		spl.y *= dd;
		} else {
		dd = pl.y / pr.y;
		pr.x *= dd;
		spr.x *= dd;
		spr.y *= dd;
		}// */
		if (pl.x > pr.x) {
			SWAP(pl, pr, pt);
			SWAP(spl, spr, pt);
		} else {
			;
		}
		lh = float2int((float)(t2d.p[2].y)) - float2int((float)(pl.y + t2d.p[2].y));
		rh = float2int((float)(t2d.p[2].y)) - float2int((float)(pr.y + t2d.p[2].y));
		if (h > 0) {
			for (i=e; i>m; --i) {
				struct vector2d vt;
				struct vector2d svt;
				//float dt = (float)(re - i) / h;
				float dlt = (float)(re - i) / lh;
				float drt = (float)(re - i) / rh;

				/*
				vt.p[0].x = t2d.p[2].x + pl.x * dt;
				vt.p[1].x = t2d.p[2].x + pr.x * dt;
				vt.p[0].y = vt.p[1].y = i;

				svt.p[0].x = t3d.p[2].x + spl.x * dt;
				svt.p[1].x = t3d.p[2].x + spr.x * dt;
				svt.p[0].y = t3d.p[2].y + spl.y * dt;
				svt.p[1].y = t3d.p[2].y + spr.y * dt;
				// */
				vt.p[0].x = t2d.p[2].x + pl.x * dlt;
				vt.p[1].x = t2d.p[2].x + pr.x * drt;
				vt.p[0].y = vt.p[1].y = (float)(i);

				svt.p[0].x = t3d.p[2].x + spl.x * dlt;
				svt.p[1].x = t3d.p[2].x + spr.x * drt;
				svt.p[0].y = t3d.p[2].y + spl.y * dlt;
				svt.p[1].y = t3d.p[2].y + spr.y * drt;


				if (b_alpha) {
					if (transparent) {
						draw_flat_scanline_alphatrans(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
					} else {
						draw_flat_scanline_alpha(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
					}
				} else {
					if (transparent) {
						draw_flat_scanline_transparent(dc_dest, &vt, dc_src, &svt, x1, x2);
					} else {
						draw_flat_scanline(dc_dest, &vt, dc_src, &svt, x1, x2);
					}
				}
			}
		}
	}
}

/* private funcion */
static
void
draw_flat_trangle_alpha_s(PIMAGE dc_dest, const struct trangle2d* dt, PCIMAGE dc_src, const struct trangle2d* tt, int x1, int y1, int x2, int y2, int transparent, int alpha) {
	struct trangle2d  t2d = *dt;
	struct trangle2d  t3d = *tt;
	int b_alpha;
	//int width = x2 - x1, height = y2 - y1;

	if (alpha >= 0 && alpha < 0x100) {
		b_alpha = 1;
	} else {
		b_alpha = 0;
	}
	{
		struct point2d _t;
		struct point2d _t3;

		if (t2d.p[1].y > t2d.p[2].y) {
			SWAP(t2d.p[1], t2d.p[2], _t);
			SWAP(t3d.p[1], t3d.p[2], _t3);
		}
		if (t2d.p[0].y > t2d.p[1].y) {
			SWAP(t2d.p[0], t2d.p[1], _t);
			SWAP(t3d.p[0], t3d.p[1], _t3);
		}
		if (t2d.p[1].y > t2d.p[2].y) {
			SWAP(t2d.p[1], t2d.p[2], _t);
			SWAP(t3d.p[1], t3d.p[2], _t3);
		}
	}
	{
		float dd;
		int s = float2int((float)t2d.p[0].y), e = float2int((float)t2d.p[2].y), h, m = float2int((float)t2d.p[1].y);
		int rs, re;
		int i, lh, rh;
		float dm = t2d.p[1].y, dh;
		struct point2d pl, pr, pt;
		struct point2d spl, spr;

		pl.x = t2d.p[1].x - t2d.p[0].x;
		pr.x = t2d.p[2].x - t2d.p[0].x;
		pl.y = t2d.p[1].y - t2d.p[0].y;
		pr.y = t2d.p[2].y - t2d.p[0].y;
		spl.x = t3d.p[1].x - t3d.p[0].x;
		spr.x = t3d.p[2].x - t3d.p[0].x;
		spl.y = t3d.p[1].y - t3d.p[0].y;
		spr.y = t3d.p[2].y - t3d.p[0].y;
		dh = dm - s;
		h = m - s;
		rs = s;
		if (s < y1) {
			s = y1;
		}
		if (m >= y2) {
			m = y2;
		}
		/*if (pl.y > pr.y) {
		dd = pr.y / pl.y;
		pl.x *= dd;
		spl.x *= dd;
		spl.y *= dd;
		} else {
		dd = pl.y / pr.y;
		pr.x *= dd;
		spr.x *= dd;
		spr.y *= dd;
		}// */
		if (pl.x > pr.x) {
			SWAP(pl, pr, pt);
			SWAP(spl, spr, pt);
		} else {
			;
		}
		lh = float2int((float)(pl.y + t2d.p[0].y)) - float2int((float)(t2d.p[0].y));
		rh = float2int((float)(pr.y + t2d.p[0].y)) - float2int((float)(t2d.p[0].y));
		if (h > 0) {
			for (i=s; i<m; ++i) {
				struct vector2d vt;
				struct vector2d svt;
				//float dt = (float)(i - rs) / h;
				float dlt = (float)(i - rs) / lh;
				float drt = (float)(i - rs) / rh;

				/*
				vt.p[0].x = t2d.p[0].x + pl.x * dt;
				vt.p[1].x = t2d.p[0].x + pr.x * dt;
				vt.p[0].y = vt.p[1].y = i;

				svt.p[0].x = t3d.p[0].x + spl.x * dt;
				svt.p[1].x = t3d.p[0].x + spr.x * dt;
				svt.p[0].y = t3d.p[0].y + spl.y * dt;
				svt.p[1].y = t3d.p[0].y + spr.y * dt;
				// */
				vt.p[0].x = t2d.p[0].x + pl.x * dlt;
				vt.p[1].x = t2d.p[0].x + pr.x * drt;
				vt.p[0].y = vt.p[1].y = (float)(i);

				svt.p[0].x = t3d.p[0].x + spl.x * dlt;
				svt.p[1].x = t3d.p[0].x + spr.x * drt;
				svt.p[0].y = t3d.p[0].y + spl.y * dlt;
				svt.p[1].y = t3d.p[0].y + spr.y * drt;

				if (b_alpha) {
					if (transparent) {
						draw_flat_scanline_alphatrans_s(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
					} else {
						draw_flat_scanline_alpha_s(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
					}
				} else {
					if (transparent) {
						draw_flat_scanline_transparent_s(dc_dest, &vt, dc_src, &svt, x1, x2);
					} else {
						draw_flat_scanline_s(dc_dest, &vt, dc_src, &svt, x1, x2);
					}
				}
			}
		}
		if (pl.y > pr.y) {
			dd = pr.y / pl.y;
			pl.x *= dd;
			spl.x *= dd;
			spl.y *= dd;
		} else {
			dd = pl.y / pr.y;
			pr.x *= dd;
			spr.x *= dd;
			spr.y *= dd;
		}
		if (m >= y1 && m < y2 && m<e) {
			struct vector2d vt;
			struct vector2d svt;

			vt.p[0].x = t2d.p[0].x + pl.x;
			vt.p[1].x = t2d.p[0].x + pr.x;
			vt.p[0].y = vt.p[1].y = (float)(m);

			svt.p[0].x = t3d.p[0].x + spl.x;
			svt.p[1].x = t3d.p[0].x + spr.x;
			svt.p[0].y = t3d.p[0].y + spl.y;
			svt.p[1].y = t3d.p[0].y + spr.y;

			if (b_alpha) {
				if (transparent) {
					draw_flat_scanline_alphatrans_s(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
				} else {
					draw_flat_scanline_alpha_s(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
				}
			} else {
				if (transparent) {
					draw_flat_scanline_transparent_s(dc_dest, &vt, dc_src, &svt, x1, x2);
				} else {
					draw_flat_scanline_s(dc_dest, &vt, dc_src, &svt, x1, x2);
				}
			}
		}
		pl.x = t2d.p[0].x - t2d.p[2].x;
		pr.x = t2d.p[1].x - t2d.p[2].x;
		pl.y = t2d.p[0].y - t2d.p[2].y;
		pr.y = t2d.p[1].y - t2d.p[2].y;
		spl.x = t3d.p[0].x - t3d.p[2].x;
		spr.x = t3d.p[1].x - t3d.p[2].x;
		spl.y = t3d.p[0].y - t3d.p[2].y;
		spr.y = t3d.p[1].y - t3d.p[2].y;

		dh = e - dm;
		h = e - m;
		re = e;
		if (m < y1) {
			m = y1 - 1;
		}
		if (e >= y2) {
			e = y2 - 1;
		}
		/*if (pl.y < pr.y) {
		dd = pr.y / pl.y;
		pl.x *= dd;
		spl.x *= dd;
		spl.y *= dd;
		} else {
		dd = pl.y / pr.y;
		pr.x *= dd;
		spr.x *= dd;
		spr.y *= dd;
		}// */
		if (pl.x > pr.x) {
			SWAP(pl, pr, pt);
			SWAP(spl, spr, pt);
		} else {
			;
		}
		lh = float2int((float)(t2d.p[2].y)) - float2int((float)(pl.y + t2d.p[2].y));
		rh = float2int((float)(t2d.p[2].y)) - float2int((float)(pr.y + t2d.p[2].y));
		if (h > 0) {
			for (i=e; i>m; --i) {
				struct vector2d vt;
				struct vector2d svt;
				//float dt = (float)(re - i) / h;
				float dlt = (float)(re - i) / lh;
				float drt = (float)(re - i) / rh;

				/*
				vt.p[0].x = t2d.p[2].x + pl.x * dt;
				vt.p[1].x = t2d.p[2].x + pr.x * dt;
				vt.p[0].y = vt.p[1].y = i;

				svt.p[0].x = t3d.p[2].x + spl.x * dt;
				svt.p[1].x = t3d.p[2].x + spr.x * dt;
				svt.p[0].y = t3d.p[2].y + spl.y * dt;
				svt.p[1].y = t3d.p[2].y + spr.y * dt;
				// */
				vt.p[0].x = t2d.p[2].x + pl.x * dlt;
				vt.p[1].x = t2d.p[2].x + pr.x * drt;
				vt.p[0].y = vt.p[1].y = (float)(i);

				svt.p[0].x = t3d.p[2].x + spl.x * dlt;
				svt.p[1].x = t3d.p[2].x + spr.x * drt;
				svt.p[0].y = t3d.p[2].y + spl.y * dlt;
				svt.p[1].y = t3d.p[2].y + spr.y * drt;


				if (b_alpha) {
					if (transparent) {
						draw_flat_scanline_alphatrans_s(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
					} else {
						draw_flat_scanline_alpha_s(dc_dest, &vt, dc_src, &svt, x1, x2, alpha);
					}
				} else {
					if (transparent) {
						draw_flat_scanline_transparent_s(dc_dest, &vt, dc_src, &svt, x1, x2);
					} else {
						draw_flat_scanline_s(dc_dest, &vt, dc_src, &svt, x1, x2);
					}
				}
			}
		}
	}
}

int
putimage_trangle(
				   PIMAGE imgdest,
				   PCIMAGE imgtexture,
				   const struct trangle2d * dt,// dest trangle, original
				   const struct trangle2d * tt,// textture trangle uv 0.0 - 1.0
				   int btransparent,
				   int alpha,
				   int smooth
				   )
{
	PIMAGE dc_dest = imgdest;
	PCIMAGE dc_src  = imgtexture;
	if (dc_dest) {
		struct trangle2d _dt = *dt;
		struct trangle2d _tt = *tt;
		int x1 = 0, y1 = 0, x2 = dc_dest->getwidth(), y2 = dc_dest->getheight(), i;

		if (smooth) {
			for (i=0; i<3; ++i) {
				_tt.p[i].x = (float)float2int((float)(_tt.p[i].x * (dc_src->getwidth() - 2)));
				_tt.p[i].y = (float)float2int((float)(_tt.p[i].y * (dc_src->getheight() - 2)));
			}
		} else {
			for (i=0; i<3; ++i) {
				_tt.p[i].x = (float)float2int((float)(_tt.p[i].x * (dc_src->getwidth() - 1)));
				_tt.p[i].y = (float)float2int((float)(_tt.p[i].y * (dc_src->getheight() - 1)));
			}
		}

		if (smooth) {
			if (dc_src->getwidth() > 1 && dc_src->getheight() > 1) {
				draw_flat_trangle_alpha_s(dc_dest, &_dt, dc_src, &_tt, x1, y1, x2, y2, btransparent, alpha);
			}
		} else {
			draw_flat_trangle_alpha(dc_dest, &_dt, dc_src, &_tt, x1, y1, x2, y2, btransparent, alpha);
		}
	}
	return grOk;
}

int
putimage_rotate(
				  PIMAGE imgdest,
				  PCIMAGE imgtexture,
				  int nXOriginDest,
				  int nYOriginDest,
				  float centerx,
				  float centery,
				  float radian,
				  int btransparent,           // transparent (1) or not (0)
				  int alpha,                  // in range[0, 256], alpha==256 means no alpha
				  int smooth
				  )
{
	PIMAGE dc_dest  = CONVERT_IMAGE(imgdest);
	PCIMAGE dc_src  = imgtexture;
	if (dc_dest) {
		struct trangle2d _tt[2];
		struct trangle2d _dt[2];
		double dx, dy, cr = cos(radian), sr = -sin(radian);
		int i, j;
		_tt[0].p[0].x = 0;
		_tt[0].p[0].y = 0;
		_tt[0].p[1].x = 0;
		_tt[0].p[1].y = 1;
		_tt[0].p[2].x = 1;
		_tt[0].p[2].y = 1;
		_tt[1].p[0] = _tt[0].p[2];
		_tt[1].p[1].x = 1;
		_tt[1].p[1].y = 0;
		_tt[1].p[2] = _tt[0].p[0];
		memcpy(&_dt, &_tt, sizeof(trangle2d)*2);
		for (j = 0; j<2; ++j) {
			for (i = 0; i<3; ++i) {
				_dt[j].p[i].x = (_dt[j].p[i].x - centerx) * (dc_src->getwidth());
				_dt[j].p[i].y = (_dt[j].p[i].y - centery) * (dc_src->getheight());
				dx = cr * _dt[j].p[i].x - sr * _dt[j].p[i].y;
				dy = sr * _dt[j].p[i].x + cr * _dt[j].p[i].y;
				_dt[j].p[i].x = (float)float2int((float)((dx + nXOriginDest) + FLOAT_EPS));
				_dt[j].p[i].y = (float)float2int((float)((dy + nYOriginDest) + FLOAT_EPS));
			}
		}
		putimage_trangle(
			dc_dest,
			imgtexture,
			&_dt[0],
			&_tt[0],
			btransparent,
			alpha,
			smooth
			);
		putimage_trangle(
			dc_dest,
			imgtexture,
			&_dt[1],
			&_tt[1],
			btransparent,
			alpha,
			smooth
			);
	}
	CONVERT_IMAGE_END;
	return grOk;
}

int
putimage_rotatezoom(
					  PIMAGE imgdest,
					  PCIMAGE imgtexture,
					  int nXOriginDest,
					  int nYOriginDest,
					  float centerx,
					  float centery,
					  float radian,
					  float zoom,
					  int btransparent,           // transparent (1) or not (0)
					  int alpha,                  // in range[0, 256], alpha==256 means no alpha
					  int smooth
					  )
{
	PIMAGE dc_dest  = CONVERT_IMAGE(imgdest);
	PCIMAGE dc_src  = imgtexture;
	if (dc_dest) {
		struct trangle2d _tt[2];
		struct trangle2d _dt[2];
		double dx, dy, cr = cos(radian), sr = -sin(radian);
		int i, j;
		_tt[0].p[0].x = 0;
		_tt[0].p[0].y = 0;
		_tt[0].p[1].x = 0;
		_tt[0].p[1].y = 1;
		_tt[0].p[2].x = 1;
		_tt[0].p[2].y = 1;
		_tt[1].p[0] = _tt[0].p[2];
		_tt[1].p[1].x = 1;
		_tt[1].p[1].y = 0;
		_tt[1].p[2] = _tt[0].p[0];
		memcpy(&_dt, &_tt, sizeof(trangle2d)*2);
		for (j = 0; j<2; ++j) {
			for (i = 0; i<3; ++i) {
				_dt[j].p[i].x = (_dt[j].p[i].x - centerx) * (dc_src->getwidth());
				_dt[j].p[i].y = (_dt[j].p[i].y - centery) * (dc_src->getheight());
				dx = cr * _dt[j].p[i].x - sr * _dt[j].p[i].y;
				dy = sr * _dt[j].p[i].x + cr * _dt[j].p[i].y;
				_dt[j].p[i].x = (float)float2int((float)((dx * zoom + nXOriginDest) + FLOAT_EPS));
				_dt[j].p[i].y = (float)float2int((float)((dy * zoom + nYOriginDest) + FLOAT_EPS));
			}
		}
		putimage_trangle(
			dc_dest,
			imgtexture,
			&_dt[0],
			&_tt[0],
			btransparent,
			alpha,
			smooth
			);
		putimage_trangle(
			dc_dest,
			imgtexture,
			&_dt[1],
			&_tt[1],
			btransparent,
			alpha,
			smooth
			);
	}
	CONVERT_IMAGE_END;
	return grOk;
}

int putimage_rotatetransparent(
	PIMAGE imgdest,
	PCIMAGE imgsrc,
	int xCenterDest,
	int yCenterDest,
	int xOriginSrc,
	int yOriginSrc,
	int widthSrc,
	int heightSrc,
	int xCenterSrc,
	int yCenterSrc,
	color_t crTransparent,
	float radian,
	float zoom
	)
{
	const PIMAGE img = CONVERT_IMAGE(imgdest);
	int zoomed_width=widthSrc*zoom;
	int zoomed_height=heightSrc*zoom;
	int zoomed_center_x = (xCenterSrc-xOriginSrc)*zoom;
	int zoomed_center_y = (yCenterSrc-yOriginSrc)*zoom;
	/* zoom */
	PIMAGE zoomed_img = newimage(zoomed_width,zoomed_height);
	putimage(zoomed_img,0,0,zoomed_width,zoomed_height,
		imgsrc,xOriginSrc,yOriginSrc,widthSrc,heightSrc,SRCCOPY);
	/* rotation */
	for (int x=0;x<zoomed_width;x++) {
		for (int y=0;y<zoomed_height;y++) {
			/* zoomed_img is newly created and have no transform/viewport, so we can use buffer directly */
			color_t color = zoomed_img->m_pBuffer[y * zoomed_img->m_width + x];
			double src_x = ((x-zoomed_center_x)*cos(radian)-(y-zoomed_center_y)*sin(radian))+xCenterDest;
			double src_y=((x-zoomed_center_x)*sin(radian)+(y-zoomed_center_y)*cos(radian))+yCenterDest;
			if (color !=crTransparent) {
				/*  
				the rotated pixel may span(partly) more than one pixels
				see:
				https://stackoverflow.com/questions/36201381/how-to-rotate-image-canvas-pixel-manipulation
				*/		
				putpixel_savealpha(src_x,src_y,color,img);
				putpixel_savealpha(src_x+0.5,src_y,color,img);
				putpixel_savealpha(src_x,src_y+0.5,color,img);
				putpixel_savealpha(src_x+0.5,src_y+0.5,color,img);
			}
		}
	}
	delimage(zoomed_img);
	CONVERT_IMAGE_END;
	return grOk;
};

int putimage_rotatetransparent(
	PIMAGE imgdest,
	PCIMAGE imgsrc,
	int xCenterDest,
	int yCenterDest,
	int xCenterSrc,
	int yCenterSrc,
	color_t crTransparent,
	float radian,
	float zoom
	) 
{
	return putimage_rotatetransparent(imgdest,imgsrc,
		xCenterDest,yCenterDest,0,0,
		imgsrc->getwidth(),imgsrc->getheight(),
		xCenterSrc,yCenterSrc,
		crTransparent,radian,zoom);
}



int
getwidth(PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		return img->m_width;
	}
	CONVERT_IMAGE_END;
	return 0;
}

int
getheight(PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		return img->m_height;
	}
	CONVERT_IMAGE_END;
	return 0;
}

int
getx(PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		POINT pt;
		GetCurrentPositionEx(img->m_hDC, &pt);
		CONVERT_IMAGE_END;
		return pt.x;
	}
	CONVERT_IMAGE_END;
	return -1;
}

int
gety(PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		POINT pt;
		GetCurrentPositionEx(img->m_hDC, &pt);
		CONVERT_IMAGE_END;
		return pt.y;
	}
	CONVERT_IMAGE_END;
	return -1;
}

PIMAGE
newimage() {
	return new IMAGE(1, 1);
}

PIMAGE
newimage(int width, int height) {
	if (width  < 1) width  = 1;
	if (height < 1) height = 1;
	return new IMAGE(width, height);
}

void
delimage(PCIMAGE pImg) {
	delete const_cast<PIMAGE>(pImg);
}

color_t* getbuffer(PIMAGE pImg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pImg);
	CONVERT_IMAGE_END;
	return img->getbuffer();
}

const color_t* getbuffer(PCIMAGE pImg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pImg);
	CONVERT_IMAGE_END;
	return img->getbuffer();
}

HDC getHDC(PCIMAGE pImg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pImg);
	CONVERT_IMAGE_END;
	return img->getdc();
}

int resize_f(PIMAGE pDstImg, int width, int height) {
	return pDstImg->resize_f(width, height);
}

int resize(PIMAGE pDstImg, int width, int height) {
	return pDstImg->resize(width, height);
}

#define EGE_GETIMAGE_CHK_NULL(p)                                            \
	do {                                                                    \
		if (p == NULL)                                                      \
			internal_panic(L"Fatal Error: pass NULL to `ege::getimage`");   \
	} while (0)

int
getimage(PIMAGE pDstImg, int srcX, int srcY, int srcWidth, int srcHeight) {
	EGE_GETIMAGE_CHK_NULL(pDstImg);
	return pDstImg->getimage(srcX, srcY, srcWidth, srcHeight);
}

int
getimage(PIMAGE pDstImg, PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight) {
	EGE_GETIMAGE_CHK_NULL(pDstImg);
	return pDstImg->getimage(pSrcImg, srcX, srcY, srcWidth, srcHeight);
}

void
putimage(int dstX, int dstY, PCIMAGE pSrcImg, DWORD dwRop) {
	pSrcImg = CONVERT_IMAGE_CONST(pSrcImg);
	pSrcImg->putimage(dstX, dstY, dwRop);
}

void
putimage(int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop) {
	pSrcImg = CONVERT_IMAGE_CONST(pSrcImg);
	pSrcImg->putimage(dstX, dstY, dstWidth, dstHeight, srcX, srcY, dwRop);
}

void
putimage(PIMAGE pDstImg, int dstX, int dstY, PCIMAGE pSrcImg, DWORD dwRop) {
	pSrcImg = CONVERT_IMAGE_CONST(pSrcImg);
	pSrcImg->putimage(pDstImg, dstX, dstY, dwRop);
}

void
putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop) {
	pSrcImg = CONVERT_IMAGE_CONST(pSrcImg);
	pSrcImg->putimage(pDstImg, dstX, dstY, dstWidth, dstHeight, srcX, srcY, dwRop);
}

int
getimage(PIMAGE pDstImg, LPCSTR pImgFile, int zoomWidth, int zoomHeight) {
	EGE_GETIMAGE_CHK_NULL(pDstImg);
	return pDstImg->getimage(pImgFile, zoomWidth, zoomHeight);
}

int
getimage(PIMAGE pDstImg, LPCWSTR pImgFile, int zoomWidth, int zoomHeight) {
	EGE_GETIMAGE_CHK_NULL(pDstImg);
	return pDstImg->getimage(pImgFile, zoomWidth, zoomHeight);
}

int
getimage(PIMAGE pDstImg, LPCSTR pResType, LPCSTR pResName, int zoomWidth, int zoomHeight) {
	EGE_GETIMAGE_CHK_NULL(pDstImg);
	return pDstImg->getimage(pResType, pResName, zoomWidth, zoomHeight);
}

int
getimage(PIMAGE pDstImg, LPCWSTR pResType, LPCWSTR pResName, int zoomWidth, int zoomHeight) {
	EGE_GETIMAGE_CHK_NULL(pDstImg);
	return pDstImg->getimage(pResType, pResName, zoomWidth, zoomHeight);
}

void
putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop) {
	pSrcImg = CONVERT_IMAGE_CONST(pSrcImg);
	pSrcImg->putimage(pDstImg, dstX, dstY, dstWidth, dstHeight, srcX, srcY, srcWidth, srcHeight, dwRop);
}

void
putimage(int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop) {
	pSrcImg = CONVERT_IMAGE_CONST(pSrcImg);
	pSrcImg->putimage(NULL, dstX, dstY, dstWidth, dstHeight, srcX, srcY, srcWidth, srcHeight, dwRop);
}

int
putimage_transparent(
	PIMAGE imgdest,         // handle to dest
	PCIMAGE imgsrc,         // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	color_t crTransparent,  // color to make transparent
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
	)
{
	imgsrc = CONVERT_IMAGE_CONST(imgsrc);
	return imgsrc->putimage_transparent(imgdest, nXOriginDest, nYOriginDest, crTransparent, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);
}

int
putimage_alphablend(
	PIMAGE imgdest,         // handle to dest
	PCIMAGE imgsrc,         // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	unsigned char alpha,    // alpha
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
	)
{
	imgsrc = CONVERT_IMAGE_CONST(imgsrc);
	return imgsrc->putimage_alphablend(imgdest, nXOriginDest, nYOriginDest, alpha, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);
}

int
putimage_alphatransparent(
	PIMAGE imgdest,         // handle to dest
	PCIMAGE imgsrc,         // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	color_t crTransparent, // color to make transparent
	unsigned char alpha,    // alpha
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
	)
{
	imgsrc = CONVERT_IMAGE_CONST(imgsrc);
	return imgsrc->putimage_alphatransparent(imgdest, nXOriginDest, nYOriginDest, crTransparent, alpha, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);
}

int
putimage_withalpha(
	PIMAGE imgdest,         // handle to dest
	PCIMAGE imgsrc,         // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
	)
{
	imgsrc = CONVERT_IMAGE_CONST(imgsrc);
	return imgsrc->putimage_withalpha(imgdest, nXOriginDest, nYOriginDest, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);
}

int EGEAPI putimage_withalpha(
	PIMAGE imgdest,         // handle to dest
	PCIMAGE imgsrc,         // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	int nWidthDest,         // width of destination rectangle
	int nHeightDest,        // height of destination rectangle
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
){
	imgsrc = CONVERT_IMAGE_CONST(imgsrc);
	return imgsrc->putimage_withalpha(imgdest, nXOriginDest, nYOriginDest, nWidthDest,nHeightDest, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);	
}

int putimage_alphafilter(
	PIMAGE imgdest,         // handle to dest
	PCIMAGE imgsrc,         // handle to source
	int nXOriginDest,       // x-coord of destination upper-left corner
	int nYOriginDest,       // y-coord of destination upper-left corner
	PCIMAGE imgalpha,       // alpha
	int nXOriginSrc,        // x-coord of source upper-left corner
	int nYOriginSrc,        // y-coord of source upper-left corner
	int nWidthSrc,          // width of source rectangle
	int nHeightSrc          // height of source rectangle
)
{
	imgsrc = CONVERT_IMAGE_CONST(imgsrc);
	return imgsrc->putimage_alphafilter(imgdest, nXOriginDest, nYOriginDest, imgalpha, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc);
}

int
imagefilter_blurring (
	PIMAGE imgdest,         // handle to dest
	int intensity,
	int alpha,
	int nXOriginDest,
	int nYOriginDest,
	int nWidthDest,
	int nHeightDest
 )
{
	PIMAGE img = CONVERT_IMAGE(imgdest);
	int ret = 0;
	if (img) {
		ret = img->imagefilter_blurring(intensity, alpha, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest);
	}
	CONVERT_IMAGE_END;
	return ret;
}

static BOOL nocaseends(LPCSTR suffix,LPCSTR text) {
	int len_suffix,len_text;
	LPCSTR p_suffix;
	LPCSTR p_text;
	len_suffix = strlen(suffix);
	len_text = strlen(text);
	if ( (len_text < len_suffix) || (len_text == 0) ) {
		return FALSE;
	}
	p_suffix = suffix;
	p_text = (text + (len_text-len_suffix));
	while (*p_text!=0) {
		if (toupper(*p_text)!=toupper(*p_suffix))
			return FALSE;
		p_text++;
		p_suffix++; 
	}	
	return TRUE;
}

static BOOL nocaseends(LPCWSTR suffix,LPCWSTR text) {
	int len_suffix,len_text;
	LPCWSTR p_suffix;
	LPCWSTR p_text;
	len_suffix = wcslen(suffix);
	len_text = wcslen(text);
	if ( (len_text < len_suffix) || (len_text == 0) ) {
		return FALSE;
	}
	p_suffix = suffix;
	p_text = (text + (len_text-len_suffix));
	while (*p_text!=0) {
		if (towupper(*p_text)!=towupper(*p_suffix))
			return FALSE;
		p_text++;
		p_suffix++; 
	}	
	return TRUE;
}


int
saveimage(PCIMAGE pimg, LPCSTR  filename) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	int ret = 0;
	if (img) {
		if (nocaseends(".bmp",filename))
			ret = img->saveimage(filename);
		else if (nocaseends(".png",filename)) 
			ret = savepng(pimg,filename);
		else
			ret = savepng(pimg,filename);
	}
	CONVERT_IMAGE_END;
	return ret;
}


int
saveimage(PCIMAGE pimg, LPCWSTR filename) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	int ret = 0;
	if (img) {
		if (nocaseends(L".bmp",filename))
			ret = img->saveimage(filename);
		else if (nocaseends(L".png",filename)) 
			ret = savepng(pimg,filename);
		else
			ret = savepng(pimg,filename);	
	}
	CONVERT_IMAGE_END;
	return ret;
}


int
getimage_pngfile(PIMAGE pimg, LPCSTR  filename) {
	FILE *fp = NULL;
	int ret;
	fp = fopen(filename, "rb");
	if (fp == NULL) return grFileNotFound;
	ret = pimg->getpngimg(fp);
	fclose(fp);
	return ret;
}

int
getimage_pngfile(PIMAGE pimg, LPCWSTR filename) {
	FILE *fp = NULL;
	int ret;
	fp = _wfopen(filename, L"rb");
	if (fp == NULL) return grFileNotFound;
	ret = pimg->getpngimg(fp);
	fclose(fp);
	return ret;
}

int
savepng(PCIMAGE pimg, LPCSTR  filename, int bAlpha) {
	FILE *fp = NULL;
	int ret;
	pimg = CONVERT_IMAGE_CONST(pimg);
	fp = fopen(filename, "wb");
	if (fp == NULL) return grFileNotFound;
	ret = pimg->savepngimg(fp, bAlpha);
	fclose(fp);
	return ret;
}

int
savepng(PCIMAGE pimg, LPCWSTR filename, int bAlpha) {
	FILE *fp = NULL;
	int ret;
	pimg = CONVERT_IMAGE_CONST(pimg);
	fp = _wfopen(filename, L"wb");
	if (fp == NULL) return grFileNotFound;
	ret = pimg->savepngimg(fp, bAlpha);
	fclose(fp);
	return ret;
}

void
ege_enable_aa(bool enable, PIMAGE pimg) {
	PIMAGE img  = CONVERT_IMAGE(pimg);
	img->enable_anti_alias(enable);
	CONVERT_IMAGE_END;
}

} // namespace ege
