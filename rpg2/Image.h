#pragma once

#include "stdafx.h"
#include "Object.h"
#include "Drawer.h"
#include "Status.h"

class Image : public Object{
public:
	Image();
	Image(int);
	virtual ~Image();

	enum STATE{
		STATE_IN,	//出現 0
		STATE_ON,	//顯示 1
		STATE_OUT	//消失 2
	};

	enum ALPHABLENDING{
		ALPHA_NORMAL,	//普通 0
		ALPHA_LIGHT,	//發光 1
		ALPHA_COLOR		//變色 2
	};

	void setTexture(float texLeft,float texTop, float texWidth, float texHeight){
		this->texLeft = texLeft;
		this->texTop = texTop;
		this->texWidth = texWidth;
		this->texHeight = texHeight;
	}
	
	void setTexture(){
		setTexture(0, 0, 1, 1);
	}

	void setImage(float imgLeft,float imgTop,float imgWidth,float imgHeight,float imgZ,float imgRad){
		this->imgLeft = imgLeft;
		this->imgTop = imgTop;
		this->imgWidth = imgWidth;
		this->imgHeight = imgHeight;
		this->imgZ = imgZ;
		this->imgRad = imgRad;
	}

	void setARGB(int colorA, int colorR, int colorG, int colorB){
		if(colorA < 0)		this->colorA = 0;
		else if(colorA>255)	this->colorA = 255;
		else				this->colorA = colorA;		
		if(colorR < 0)		this->colorR = 0;
		else if(colorR>255)	this->colorR = 255;
		else				this->colorR = colorR;
		if(colorG < 0)		this->colorG = 0;
		else if(colorG>255)	this->colorG = 255;
		else				this->colorG = colorG;
		if(colorB < 0)		this->colorB = 0;
		else if(colorB>255)	this->colorB = 255;
		else				this->colorB = colorB;
	}

	void setARGB(int colorA){
		setARGB(colorA, this->colorR, this->colorG, this->colorB);
	}

	void setBlend(int blendMode,int blendTimes){
		this->blendMode	= blendMode;
		if (blendTimes < 1) {
			this->blendTimes = 1;
		}
		else{
			this->blendTimes = blendTimes;
		}
	}

	void setBlend(int blendMode){
		setBlend(blendMode, 1);
	}

	void setImgId(int imgId){
		this->imgId = imgId;
	}

	bool isOut(){
		return (x<FRAME_LEFT || x>FRAME_RIGHT || y<FRAME_DOWN || y>FRAME_UP);
	}

	float getRad(){
		return rad;
	}

	void setFixed(bool fixed){
		this->fixed= fixed;
	}

	virtual void setVertex(float cameraX, float cameraY){
		float vcos = cosf((float)(imgRad/180*3.14));
		float vsin = sinf((float)(imgRad/180*3.14));
		if(fixed){
			vertex[0].x= ((vcos * -imgLeft) - (vsin * imgHeight) + x) / SCREEN_WIDTH*2 - 1;   
			vertex[0].y= ((vsin * -imgLeft) + (vcos * imgHeight) + y) / SCREEN_HEIGHT*2 - 1; 
			vertex[1].x= ((vcos * imgWidth) - (vsin * imgHeight) + x) / SCREEN_WIDTH *2 - 1;   
			vertex[1].y= ((vsin * imgWidth) + (vcos * imgHeight) + y) / SCREEN_HEIGHT*2 - 1;
			vertex[2].x= ((vcos * -imgLeft) - (vsin * -imgTop) + x) / SCREEN_WIDTH*2 - 1;   
			vertex[2].y= ((vsin * -imgLeft) + (vcos * -imgTop) + y) / SCREEN_HEIGHT*2  - 1;
			vertex[3].x= ((vcos * imgWidth) - (vsin * -imgTop) + x) / SCREEN_WIDTH*2 - 1;   
			vertex[3].y= ((vsin * imgWidth) + (vcos * -imgTop) + y) / SCREEN_HEIGHT*2 - 1;
		}
		else{
			vertex[0].x= ((vcos * -imgLeft) - (vsin * imgHeight) + x - cameraX) / SCREEN_WIDTH*2 - 1;   
			vertex[0].y= ((vsin * -imgLeft) + (vcos * imgHeight) + y - cameraY) / SCREEN_HEIGHT*2 - 1; 
			vertex[1].x= ((vcos * imgWidth) - (vsin * imgHeight) + x - cameraX) / SCREEN_WIDTH *2 - 1;   
			vertex[1].y= ((vsin * imgWidth) + (vcos * imgHeight) + y - cameraY) / SCREEN_HEIGHT*2 - 1;
			vertex[2].x= ((vcos * -imgLeft) - (vsin * -imgTop) + x - cameraX) / SCREEN_WIDTH*2 - 1;   
			vertex[2].y= ((vsin * -imgLeft) + (vcos * -imgTop) + y - cameraY) / SCREEN_HEIGHT*2  - 1;
			vertex[3].x= ((vcos * imgWidth) - (vsin * -imgTop) + x - cameraX) / SCREEN_WIDTH*2 - 1;   
			vertex[3].y= ((vsin * imgWidth) + (vcos * -imgTop) + y - cameraY) / SCREEN_HEIGHT*2 - 1;
		}
		vertex[0].z = vertex[1].z = vertex[2].z = vertex[3].z = 0.5;
		vertex[0].tu = vertex[2].tu = texLeft;
		vertex[1].tu = vertex[3].tu = texWidth;
		vertex[0].tv = vertex[1].tv = texTop;
		vertex[2].tv = vertex[3].tv = texHeight;
		vertex[0].specular = vertex[1].specular = vertex[2].specular = vertex[3].specular = D3DCOLOR_ARGB(colorA, colorR, colorG, colorB);
		vertex[0].diffuse  = vertex[1].diffuse  = vertex[2].diffuse  = vertex[3].diffuse  = D3DCOLOR_ARGB(colorA, colorR, colorG, colorB);

	}

	virtual UINT vertexSize(){
		return sizeof(D3DVERTEX)*4;
	}

	virtual void drawVertex(LPDIRECT3DDEVICE9 &direct3DDevice){
		direct3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	virtual void draw(Drawer *drawer){
		if(imgId == -1) return;

		LPDIRECT3DDEVICE9 direct3DDevice = drawer->getDevice();
		float cameraX = drawer->getCameraX();
		float cameraY = drawer->getCameraY();
		LPDIRECT3DTEXTURE9 texture = *(drawer->getTexture()+imgId);

		setVertex(cameraX, cameraY);
		
		//將頂點複製到頂點緩衝區
		void *pVertices = NULL;
		vertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
		memcpy(pVertices, vertex, vertexSize());
		vertexBuffer->Unlock();
		//繪製模式
		if (blendMode == ALPHA_NORMAL){
			direct3DDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
			direct3DDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		}
		else if(blendMode == ALPHA_LIGHT){
			direct3DDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
			direct3DDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ONE );
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_SPECULAR);
			direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		}
		else if(blendMode == ALPHA_COLOR){
			direct3DDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
			direct3DDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_SPECULAR);
			direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		}

		direct3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(D3DVERTEX));
		direct3DDevice->SetTexture(0, texture);
		
		direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		direct3DDevice->SetRenderState(D3DRS_ZENABLE, true);
		direct3DDevice->BeginScene();	
		for(int i = 0; i < blendTimes; i++){
			drawVertex(direct3DDevice);
		}
		direct3DDevice->EndScene();
		direct3DDevice->SetRenderState(D3DRS_ZENABLE, false);
		direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	protected:

	class Point{
	public:
		Point(){
		};
		virtual ~Point(){};

		float x;
		float y;
		float len;
		float rad;
		int colorA;
		int colorR;
		int colorG;
		int colorB;

		void setColor(int colorA, int colorR, int colorG, int colorB){
			if(colorA < 0)		this->colorA = 0;
			else if(colorA>255)	this->colorA = 255;
			else				this->colorA = colorA;		
			if(colorR < 0)		this->colorR = 0;
			else if(colorR>255)	this->colorR = 255;
			else				this->colorR = colorR;
			if(colorG < 0)		this->colorG = 0;
			else if(colorG>255)	this->colorG = 255;
			else				this->colorG = colorG;
			if(colorB < 0)		this->colorB = 0;
			else if(colorB>255)	this->colorB = 255;
			else				this->colorB = colorB;
		}

		void setColor(int colorA){
			setColor(colorA, this->colorR, this->colorG, this->colorB);
		}

		void setPoint(float len, float rad, int colorA, int colorR, int colorG, int colorB){
			
			setColor(colorA, colorR, colorG, colorB);
			this->len = len;
			this->rad = rad;
			x = len*cosf(rad/180*3.14);
			y = len*sinf(rad/180*3.14);
		}

		void setPoint(float len, float rad, int colorA){
			setPoint(len, rad, colorA, this->colorR, this->colorG, this->colorB);
		}

		D3DCOLOR getColor(){
			return D3DCOLOR_ARGB(colorA, colorR, colorG, colorB);
		}
	};

	int length;
	D3DVERTEX *vertex;
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;

	float imgLeft;
	float imgTop;
	float imgWidth;
	float imgHeight;
	float imgZ;
	float imgRad;
	int   imgId;
	float texLeft;
	float texTop;
	float texWidth;
	float texHeight;
	int colorA;
	int colorR;
	int colorG;
	int colorB;
	int blendMode;
	int blendTimes;

	bool fixed;
	bool go_out;
	
	virtual void mainProc(){
	}
	

private:
	void init(){
		go_out = false;

		setTexture();
		setARGB(255, 255, 255, 255);
		setBlend(ALPHA_NORMAL);

		setTarget(NULL, 0, 0);
		setFixed(false);

		imgId = -1;
		//DirectX初始化
		extern Drawer *drawer;
		length = 4;
		vertex = new D3DVERTEX[4];
		LPDIRECT3DDEVICE9 device = drawer->getDevice();
		device->CreateVertexBuffer(sizeof(D3DVERTEX)*4, D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX  , D3DPOOL_MANAGED ,  &vertexBuffer, NULL);
	}
};

Image::Image() :
	Object(){
	init();
};

Image::Image(int imgId) :
	Object(){
	init();
	this->imgId = imgId;
};

Image::~Image(){
};

class Anime : public Image {

public:

	Anime();
	Anime(int);
	virtual ~Anime();

	static const int IN_MODE_FADE = 0;
	
	void setIn(int inTime,int inMode,float inX,float inY){
		this->inTime = inTime;
		this->inMode = inMode;
		this->inX = inX;
		this->inY = inY;
	}
	void setOn(int onTime,int onMode,float onX,float onY){
		this->onTime = onTime;
		this->onMode = onMode;
		this->onX = onX;
		this->onY = onY;
	}
	void setOut(int outTime,int outMode,float outX,float outY){
		this->outTime = outTime;
		this->outMode = outMode;
		this->outX = outX;
		this->outY = outY;
	}
	
	virtual void mainProc(){
		switch(state){
			case STATE_IN:
				if(inMode == 0){
					if(time < inTime){
						float ratio = (float)time / (float)inTime;
						setPosition(inX + (onX-inX) * ratio, inY + (onY-inY) * ratio);
						setARGB((int)255*ratio);
					}
					else{
						setState(STATE_IN);
					}
				}
				break;
			case STATE_ON:

				setPosition(onX, onY);
				setARGB(255);

				if(onMode == 0){
					if(time >= onTime){
						setState(2);
					}
				}
				else if(onMode == 1){
				}
				break;
			case STATE_OUT:
				if(outMode == 0){
					if(time < outTime){
						float ratio = (float)time / (float)outTime;
						setPosition (onX + (outX-onX) * ratio, onY + (outY-onY) * ratio);
						setARGB ((int)255 * (1 - ratio));
					}
					else{
						setARGB (0);
						finish();
					}
				}
				break;
		}
		Image::mainProc();
	}

protected:
	int inTime, inMode, inX, inY;
	int onTime, onMode, onX, onY;
	int outTime, outMode , outX, outY;
};

Anime::Anime(): 
	Image(){
	
};
Anime::Anime(int imgId): 
	Image(imgId){
};

Anime::~Anime(){
};

class ThemeAnime : public Anime{
public:
	ThemeAnime();
	~ThemeAnime();
};

ThemeAnime::ThemeAnime() :
	Anime(100){
	setIn (30 ,0 ,SCREEN_WIDTH/2 ,SCREEN_HEIGHT/2+270);
	setOn (0  ,1 ,SCREEN_WIDTH/2 ,SCREEN_HEIGHT/2+200);
	setOut(30 ,0 ,SCREEN_WIDTH/2 ,SCREEN_HEIGHT/2+130);
	setImage(400,100,400,100,0,0);
};
ThemeAnime::~ThemeAnime(){
};


class ImageFan : public Image {
public:
	ImageFan(int length);
	virtual ~ImageFan();

	UINT vertexSize(){
		return sizeof(D3DVERTEX)*(length+1);
	}

	void setVertex(float cameraX, float cameraY){
		vertex[0].x = (x - cameraX)/SCREEN_WIDTH*2 - 1;
		vertex[0].y = (y - cameraY)/SCREEN_HEIGHT*2 - 1;
		vertex[0].z = 0.5;
		vertex[0].tu = texLeft;
		vertex[0].tv = texTop;
		vertex[0].specular = fan[0].getColor();
		vertex[0].diffuse= fan[0].getColor();

		for(int i = 1; i < length+1; i++){
			vertex[i].x = (fan[i].x + x - cameraX)/SCREEN_WIDTH*2 - 1;
			vertex[i].y = (fan[i].y + y - cameraY)/SCREEN_HEIGHT*2 - 1;
			vertex[i].z = 0.5;
			vertex[i].tu = texLeft + (texWidth-texLeft)*(float)i/(float)length;
			vertex[i].tv = texHeight;
			vertex[i].specular = fan[i].getColor();
			vertex[i].diffuse= fan[i].getColor();
		}
	}

	void drawVertex(LPDIRECT3DDEVICE9 &direct3DDevice){
		direct3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, length-2 );
	}
	
protected:
	Point *fan;
};

ImageFan::ImageFan(int length){

	setTexture();
	setBlend(ALPHA_NORMAL);

	extern Drawer *drawer;

	this->length = length;
	this->vertex = new D3DVERTEX[length+1];
	this->fan = new Point[length+1];
	LPDIRECT3DDEVICE9 device = drawer->getDevice();
	device->CreateVertexBuffer( sizeof(D3DVERTEX)*(length+1), D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX  , D3DPOOL_MANAGED ,  &vertexBuffer, NULL);

	fan[0].x = 20*cosf(0);
	fan[0].y = 20*sinf(0);
	fan[0].colorA = 255;
	fan[0].colorR = 255;
	fan[0].colorG = 255;
	fan[0].colorB = 255;

	for(int i = 1; i < length+1; i++){
		float r = -(float)i*1;
		fan[i].x = 300*cosf((float)r/180*3.14);
		fan[i].y = 300*sinf((float)r/180*3.14);
		fan[i].colorA = 255;
		fan[i].colorR = 255;
		fan[i].colorG = 255;
		fan[i].colorB = 255;
	}
	imgId = 0;
};

ImageFan::~ImageFan(){
};


class ImageStrip : public Image {
public:
	ImageStrip(int, float, float, float);
	virtual ~ImageStrip();
	

	UINT vertexSize(){
		return sizeof(D3DVERTEX)*(length*2+2);
	}

	void setVertex(float cameraX, float cameraY){
		for(int i = 0; i < length*2 + 2; i+=2){
			vertex[i].x = (strip[i].x + x - cameraX)/SCREEN_WIDTH*2 - 1;
			vertex[i].y = (strip[i].y + y - cameraY)/SCREEN_HEIGHT*2 - 1;
			vertex[i].z = 0.5;
			vertex[i].tu = texLeft + (texWidth)*(float)i/(float)length/2;
			vertex[i].tv = texHeight;
			vertex[i].specular = strip[i].getColor();
			vertex[i].diffuse= strip[i].getColor();
			
			vertex[i+1].x = (strip[i+1].x + x - cameraX)/SCREEN_WIDTH*2 - 1;
			vertex[i+1].y = (strip[i+1].y + y - cameraY)/SCREEN_HEIGHT*2 - 1;
			vertex[i+1].z = 0.5;
			vertex[i+1].tu = texLeft + (texWidth)*(float)i/(float)length/2;
			vertex[i+1].tv = texTop;
			vertex[i+1].specular = strip[i+1].getColor();
			vertex[i+1].diffuse= strip[i+1].getColor();
		}
	}

	void drawVertex(LPDIRECT3DDEVICE9 &direct3DDevice){
		direct3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, length*2);
	}


	void setAllStrip(float len1, float len2, float rad, int alpha){
		for(int i=0;i<length*2+2;i+=2){
			strip[i].setPoint(len1, rad, alpha);
			strip[i+1].setPoint(len2, rad, alpha);
		}
	}

	void addStrip(int num, float len1, float len2, float rad, int alpha){
		
		float dLen1 = (len1 - strip[0].len)/num;
		float dLen2 = (len2 - strip[1].len)/num;
		float dRad = (rad - strip[0].rad)/num;
		float dAlpha = (alpha - strip[0].colorA)/num;
		
		/*
		float dLen1 = 0;
		float dLen2 = 0;
		float dRad = 6;
		float dAlpha = 5;
		*/
		for(int i = length*2+1; i>=num*2; i--){
			strip[i].setPoint (strip[i-num*2].len, strip[i-num*2].rad, strip[i-num*2].colorA, strip[i-num*2].colorR, strip[i-num*2].colorG, strip[i-num*2].colorB);
		}

		for(int i = num*2-1; i >= 0; i-=2){
			strip[i-1].setPoint(strip[i+1].len + dLen1, strip[i+1].rad + dRad, strip[i+1].colorA + dAlpha, 255, 255, 255);
			strip[i].setPoint(strip[i+2].len + dLen2, strip[i+2].rad + dRad, strip[i+2].colorA + dAlpha, 255, 255, 255);
		}
	}	

protected:
	Point *strip;
	Image *target;

};

ImageStrip::ImageStrip(int length, float len1, float len2, float rad){

	setTexture();

	extern Drawer *drawer;
	this->length = length;
	this->vertex = new D3DVERTEX[length*2+2];
	this->strip = new Point[length*2+2];
	LPDIRECT3DDEVICE9 device = drawer->getDevice();
	device->CreateVertexBuffer( sizeof(D3DVERTEX)*(length*2+2), D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX  , D3DPOOL_MANAGED ,  &vertexBuffer, NULL);
};

ImageStrip::~ImageStrip(){
};

class SwordShadow : public ImageStrip{
public:
	float swordLen1;
	float swordLen2;
	float swordRad;

	SwordShadow(int length, float len1, float len2, float rad) :
		ImageStrip(length, len1, len2, rad){
		swordLen1 = len1;
		swordLen2 = len2;
		setBlend(ALPHA_LIGHT, 3);
		imgId = 820;
	}
	virtual ~SwordShadow(){
	}

	virtual void wield(int wieldTime, float wieldStart, float wieldEnd){
		this->wieldTime = wieldTime;
		this->wieldStart = wieldStart;
		this->wieldEnd = wieldEnd;
		time = 0;
		setAllStrip(swordLen1, swordLen2, wieldStart, 255);
	}
	float getSwordRad(){
		return swordRad;
	}
	virtual void mainProc(){
		swordRad = wieldStart + (wieldEnd-wieldStart)*(float)time/wieldTime;
		if(time<=wieldTime){
			addStrip(5, swordLen1, swordLen2, swordRad, 255);
		}
		for(int i = 0; i < length*2+2; i++){
			strip[i].setColor(strip[i].colorA*0.85, strip[i].colorR*0.9, strip[i].colorG*0.9, strip[i].colorB);
			//strip[i].setColor(strip[i].colorA*0.85, strip[i].colorR*3/4+rand()%64, strip[i].colorG*3/4+rand()%64, strip[i].colorB*3/4+rand()%64);
		}
		Image::mainProc();
	}

protected:
	float wieldStart;
	float wieldEnd;
	float wieldTime;
};

class AnimeBlock : public Image{
public:
	AnimeBlock() : Image(){
		imgId = 500;
		setARGB(192);
	}

	virtual ~AnimeBlock(){
	}

	void mainProc(){
		if(time<20){
			setImage(0,64,(float)time / 20 * SCREEN_WIDTH,64,0,0);
			setTexture(0,0,(float)time / 20,1);
		}
		else if (time<60){
			setImage(0, 64, SCREEN_WIDTH ,64 ,0 ,0);
			setTexture(0, 0, 1, 1) ;
		}
		else if (time<80){
			setImage( -((float)time - 60)/20 * SCREEN_WIDTH ,64,SCREEN_WIDTH,64,0,0);
			setTexture((float)(time - 60)/20,0,1,1) ;
		}
		else {
			setARGB(0);
		}

		Image::mainProc();
	}

};


class Component : public Object, public Drawable{
};
