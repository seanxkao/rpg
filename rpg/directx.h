#pragma once

#include "math.h"
#include "d3d9.h"
#include "d3dx9.h"


#define  D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)

struct D3DVERTEX {
	float x, y, z;
	DWORD diffuse,specular;
	float tu, tv; 
};
