#pragma once

#include "directx.h"
#include <queue>

class Drawer{

public:
	Drawer(){
		setMapSize(SCREEN_WIDTH, SCREEN_HEIGHT);	
		setCamera();	
		moveCamera(0, 0);
	};

	~Drawer(){
		if  (direct3DDevice!=NULL){
			direct3DDevice->Release();
		}
		if  (direct3D != NULL){
			direct3D->Release();
		}
		for(int i=0;i<10000;i++){	
			if  (texture[i]) {
				texture[i]->Release();
				texture[i] = NULL;
			}
		}
		delete []texture;
	}

	void drawAll(){
		if(direct3DDevice->Present( NULL, NULL, NULL, NULL )==D3DERR_DEVICELOST){
		}
	}

	void clear(){
		direct3DDevice->Clear(0, NULL, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0,0,0),1.0f, 0 );
	}

	void loadTexture(int img, LPCWSTR file)
	{
		D3DXCreateTextureFromFileExW(direct3DDevice, file, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL,&texture[img]);
	}

	bool initD3D(HWND hWnd){
		if((direct3D=Direct3DCreate9(D3D_SDK_VERSION))==NULL){
			return false;
		}
		D3DDISPLAYMODE d3ddm;	
		direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		
		/*
		/Full screen
		/d3dpp.Windowed = false; 
		/d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		/d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;
		/d3dpp.BackBufferHeight = SCREEN_HEIGHT;
		/d3dpp.BackBufferWidth = SCREEN_WIDTH; 
	   */
		
		d3dpp.Windowed = true;  
		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;		
		d3dpp.FullScreen_RefreshRateInHz = 0;   
		d3dpp.BackBufferHeight = 0;
		d3dpp.BackBufferWidth = 0; 
		d3dpp.hDeviceWindow = hWnd;
		d3dpp.BackBufferCount = 1; 
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; 
		if(FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &direct3DDevice))){
			return false;
		}
		direct3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//		direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		
		direct3DDevice->SetRenderState(D3DRS_ZENABLE, false);

		/*
		linear
		direct3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		direct3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		direct3DDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
		*/

		texture = new LPDIRECT3DTEXTURE9[10000];

		for(int i=0;i<10000;i++){	
			texture[i] = NULL;
		}

		//main menu
		loadTexture(0, L"image/buttom_mainmenu_start.bmp"); 	
		loadTexture(1, L"image/buttom_mainmenu_start_choosed.bmp"); 
		loadTexture(2, L"image/buttom_mainmenu_option.bmp"); 
		loadTexture(3, L"image/buttom_mainmenu_option_choosed.bmp"); 
		loadTexture(4, L"image/buttom_mainmenu_music.bmp"); 
		loadTexture(5, L"image/buttom_mainmenu_music_choosed.bmp"); 
		loadTexture(6, L"image/buttom_mainmenu_quit.bmp"); 
		loadTexture(7, L"image/buttom_mainmenu_quit_choosed.bmp"); 
		//start menu
		loadTexture(20, L"image/buttom_startmenu_training.bmp"); 
		loadTexture(21, L"image/buttom_startmenu_training_choosed.bmp"); 
		loadTexture(22, L"image/buttom_startmenu_battle.bmp"); 
		loadTexture(23, L"image/buttom_startmenu_battle_choosed.bmp"); 
		loadTexture(24, L"image/buttom_startmenu_stage.bmp"); 
		loadTexture(25, L"image/buttom_startmenu_stage_choosed.bmp"); 
		loadTexture(26, L"image/buttom_startmenu_status.bmp"); 
		loadTexture(27, L"image/buttom_startmenu_status_choosed.bmp"); 
		loadTexture(28, L"image/buttom_startmenu_return.bmp"); 
		loadTexture(29, L"image/buttom_startmenu_return_choosed.bmp"); 
		//training menu
		loadTexture(30, L"image/buttom_training_menu_001.bmp"); 
		loadTexture(31, L"image/buttom_training_menu_001_choosed.bmp");
		loadTexture(32, L"image/buttom_training_menu_002.bmp"); 
		loadTexture(33, L"image/buttom_training_menu_002_choosed.bmp");
		loadTexture(34, L"image/buttom_training_menu_003.bmp"); 
		loadTexture(35, L"image/buttom_training_menu_003_choosed.bmp");
		//title
		loadTexture(100, L"image/title.bmp");
		//background
		loadTexture(200, L"image/map_grass.bmp");
		//interface
		loadTexture(300, L"image/gui_00.bmp");
		//text
		loadTexture(500, L"image/text_bar_0.bmp");
		//flash
		loadTexture(800, L"image/light_0.bmp");
		//loadTexture(810, L"image/hit_01.bmp");
		loadTexture(810, L"image/explode_0.bmp");
		loadTexture(820, L"image/effect_swordshadow_01.bmp");
		//alley
//		loadTexture(1000, L"image/my_ship_0.bmp");
//		loadTexture(1000, L"image/character_medica.bmp");
		loadTexture(1000, L"image/character_literace_00.bmp");
		loadTexture(1001, L"image/character_literace_01.bmp");
		loadTexture(1002, L"image/character_literace_02.bmp");
		loadTexture(1003, L"image/character_literace_03.bmp");
		loadTexture(1004, L"image/character_literace_04.bmp");
		loadTexture(1005, L"image/character_literace_05.bmp");
		loadTexture(1100, L"image/hp_0.bmp");
		loadTexture(1101, L"image/hp_1.bmp");
		loadTexture(1104, L"image/exp_0.bmp");
		loadTexture(1105, L"image/exp_1.bmp");
		//我方子彈
//		loadTexture(2000, L"image/my_bullet_0.bmp");
		loadTexture(2000, L"image/slash_1.bmp");
		loadTexture(2001, L"image/slash_2.bmp");
		loadTexture(2002, L"image/slash_3.bmp");
		loadTexture(2003, L"image/slash_4.bmp");
		//敵方角色
		loadTexture(3000, L"image/enemy_ship_0.bmp");
		loadTexture(3001, L"image/enemy_weak.bmp");
		//敵方子彈
		loadTexture(4000, L"image/enemy_bullet_0.bmp"); 
		loadTexture(4001, L"image/enemy_bullet_1.bmp"); 
		loadTexture(4002, L"image/enemy_bullet_2.bmp"); 
		loadTexture(4003, L"image/enemy_bullet_3.bmp"); 
		loadTexture(4004, L"image/enemy_bullet_4.bmp"); 
		loadTexture(4005, L"image/enemy_bullet_5.bmp"); 
		loadTexture(4006, L"image/enemy_bullet_6.bmp"); 
		loadTexture(4007, L"image/enemy_bullet_7.bmp"); 
		//text
		//D3DXCreateFont(direct3DDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &d_text);
		return true;
	}



	void moveCamera(float x, float y){
		//地圖小於攝影機視野-視角設定在中央
		//攝影機邊界不能超出地圖邊界
		if(mapWidth < SCREEN_WIDTH){
			cameraX = (SCREEN_WIDTH - mapWidth)/2;
		}
		else{
			if(x < cameraX + cameraLeft){
				cameraX = x - cameraLeft;
			}
			else if(x > cameraX + cameraRight){
				cameraX = x - cameraRight;
			}
			if(cameraX < 0){
				cameraX = 0;
			}
			else if(cameraX > mapWidth - SCREEN_WIDTH){
				cameraX = mapWidth - SCREEN_WIDTH;
			}
		}
		

		if(mapHeight < SCREEN_HEIGHT){
			cameraY = (SCREEN_HEIGHT - mapHeight)/2;
		}
		else{
			if(y < cameraY + cameraTop){
				cameraY = y - cameraTop;
			}
			else if(y > cameraY + cameraBottom){
				cameraY = y - cameraBottom;
			}
			if(cameraY < 0){
				cameraY = 0;
			}
			else if(cameraY > mapHeight - SCREEN_HEIGHT){
				cameraY = mapHeight - SCREEN_HEIGHT;
			}
		}
	}
	
	
	void setCamera(float left, float top, float right, float bottom){
		cameraLeft = left;
		cameraTop = top;
		cameraRight = right;
		cameraBottom = bottom;
	}
	void setCamera(){
		setCamera(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	void setMapSize(float width, float height){
		mapWidth = width;
		mapHeight = height;
	}

	float getCameraX(){
		return cameraX;
	}
	float getCameraY(){
		return cameraY;
	}

	LPDIRECT3DDEVICE9 getDevice(){
		return direct3DDevice;
	}

	LPDIRECT3DTEXTURE9* getTexture(int i){
		return &texture[i];
	}



protected:
	LPDIRECT3D9			direct3D; //DX物件宣告
	LPDIRECT3DDEVICE9	direct3DDevice;
	LPDIRECT3DTEXTURE9	*texture;
	LPD3DXFONT d_text;

	float cameraX;
	float cameraY;
	float cameraLeft;
	float cameraTop;
	float cameraRight;
	float cameraBottom;
	float mapWidth;
	float mapHeight;
};
