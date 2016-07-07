#pragma once

#include "stdafx.h"
#include "strsafe.h"
#include "empty.h"
#include "time.h"

#include "Particle.h"
#include "Pool.h"
#include "Button.h"
#include "Menu.h"
#include "Map.h"
#include "Bar.h"
#include "Bullet.h"
#include "BulletManager.h"


#include <sstream>
#include <string>


////////////////////////////////////////物件設定/////////////////////////////////////////////////////

class Avatar{
public:
	int	level;
	int	exp;
	int	maxExp;
	int	maxHp;
	int	maxMp;
};


class text_bar : public Image{
public:
	text_bar();
	~text_bar();

	void main(Drawer *drawer){
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

		paint(drawer);
		Object::main();
	}

};

text_bar::text_bar() :
	Image(){
	imgId = 500;
	setARGB(192);
};

text_bar::~text_bar(){
};




class my_ship : public Image {
public:

	my_ship(BulletManager*);
	~my_ship();

	BulletManager *bulletManager;


	void moveCharacter(){
		if(keyboard.PRESS_LEFT){
			x -= 15 ;
		}
		if(keyboard.PRESS_RIGHT){
			x += 15 ;
		}
		if(keyboard.PRESS_UP){
			y += 15 ;
		}
		if(keyboard.PRESS_DOWN){
			y -= 15 ;
		}
		
		if(x < FRAME_LEFT + 50){
			x = FRAME_LEFT + 50;
		}
		if(x > FRAME_RIGHT - 50){
			x = FRAME_RIGHT	- 50;
		}
		if(y > FRAME_UP	- 60){
			y = FRAME_UP - 60;
		}
		if(y < FRAME_DOWN + 60){
			y = FRAME_DOWN	+ 60;
		}
		
	}

	void main(Drawer *drawer){

		if(state==0){
			set_body(25);
			setImage(50,50,50,50,0,0);
			moveCharacter();
			if(keyboard.PRESS_NATK){
				setState(1);
			} 

			if(hp<0)setState(99);
		}

		else if(state==1){
			set_body(25);
			setImage(50,50,50,50,0,0);
			if(time == 0){
				bulletManager->addBullet(x,y,0,0,0.0,1,1,10,0,1,0);
			}
			else{		
				moveCharacter();
			}
			if(time > 3){
				setState(0);
			}
		}

		else if(state==99){
			if(time<25){
				imgRad+=17;
				setARGB(255-time * 10);
				setImage(30+time*7,30+time*7,30+time*7,30+time*7,0,imgRad);
			}
			else{
				setARGB(0);
			}
		}
		paint(drawer);
		Object::main();
	}

};

my_ship::my_ship(BulletManager *bulletManager){
	this->bulletManager = bulletManager;
	setImage(30,30,30,30,0,0);
	imgId = 1000;
	max_hp=100;
	hp=100;
}


my_ship::~my_ship(){
}

class enemy_ship : public Image
{
public:
	enemy_ship();
	~enemy_ship();

	HpBar *hpBar;
	int kind;
	bool exist;
	float spd,rate,r;
	BulletManager *bulletManager;

	void setExist(bool exist){
		this->exist = exist;
	}

	void main(Drawer *drawer){
		if(kind == 0){
			if(state==0){
				if(time==0){
					hp=30;
					set_body(20);
					hpBar = new HpBar(this);
				}
				if(time%30==0){
					bulletManager->addBullet(x,y,0,30,270,1,1,0,0,1,100);
				}
				hpBar->main(drawer);
				if(hp<0)setState(99);
				if(isOut())setState(99);
			}

			else if(state==1){
			}

			else if(state==99){
				if(time<25){
					hpBar->main(drawer);
					imgRad+=17;
					setARGB(255 - time*10);
					setImage(30 + time*7,30 + time*7,30 + time*7,30 + time*7, 0, imgRad);
				}
				else{
					delete hpBar;
					hpBar = NULL;
					setARGB(0);
					setExist(false);
				}
			}
		}
		else if(kind == 1){
			//雜兵
			if(state==0){
				if(time==0){
					imgId = 3001;
					hp = 30;
					max_hp = 30;
					set_body(20);
					setImage(40, 40, 40, 40, 0, 0);
					hpBar = new HpBar(this);
				}
				if(time%50==0){
					changeRad((int)random(3.5,7)*45);
					changeRate(1);
				}
				else if(time%50 > 30){
					changeRate(0);
				}
				hpBar->main(drawer);
				if(hp<0)setState(99);
				if(isOut())setState(99);
			}

			else if(state==1){
			}

			else if(state==99){
				if(time<25){
					imgRad+=17;
					setARGB(255 - time*10);
					setImage(30 + time*7,30 + time*7,30 + time*7,30 + time*7, 0, imgRad);
					hpBar->main(drawer);
				}
				else{
					delete hpBar;
					hpBar = NULL;
					setARGB(0);
					setExist(false);
				}
			}
		}

		paint(drawer);
		Object::main();
	}

};

enemy_ship::enemy_ship(){
	setImage(30,30,30,30,0,0);
	setARGB(255);
	imgId = 1000;
	max_hp=100;
	hp=100;
}

enemy_ship::~enemy_ship(){
}


class enemy_ship_manager : public Object{
public:
	enemy_ship_manager();
	enemy_ship_manager(BulletManager*);
	~enemy_ship_manager();

	enemy_ship ES[100];
	BulletManager *bulletManager;

	void clear(){
		for(int i=0;i<100;i++){
			if(ES[i].exist){
				ES[i].exist=false;
			}
		}
	}

void create_ES(float x,float y,int z,float spd,float r,int kind, int state){
	for(int i=0;i<100;i++){
		if(!ES[i].exist ){
			ES[i].exist = true;
			ES[i].clean();
			ES[i].setPosition(x, y);
			ES[i].setSpeed(spd, r, 1);
			ES[i].setImgId(3000);
			ES[i].setImage(20,30,20,30,0,r);
			ES[i].setARGB(255, 255, 255, 255);
			ES[i].setBlend(Image::ALPHA_NORMAL);
			ES[i].kind = kind;		
			ES[i].bulletManager = bulletManager;

			//OBJ_num++;
			break;
		}
	}
}
	
	void main(Drawer *drawer){
		for(int i=0;i<100;i++){
			if(ES[i].exist){
				ES[i].main(drawer);
			}
		}
	}

};

enemy_ship_manager::enemy_ship_manager(){
	for(int i = 0; i < 100; i++){
		ES[i].exist = false;
	}
}
enemy_ship_manager::enemy_ship_manager(BulletManager *bulletManager){
	for(int i = 0; i < 100; i++){
		ES[i].exist = false;
	}
	this->bulletManager = bulletManager;
}

enemy_ship_manager::~enemy_ship_manager(){
}

class Game{
public:
	Game();
	~Game();

	static const int STATE_MAINMENU = 0;
	static const int STATE_STARTMENU = 1;
	static const int STATE_TRAINMENU = 10;

	int  state;
	int  nextState;
	long time;
	
	Menu *mainMenu;
	int menu_btnPressed;
	bool menu_choosed;
	int menu_countdown;

	Anime *imgTheme;

	Menu *startMenu;
	int    start_btnPressed;
	bool   start_choosed;
	int    start_countdown;
	
	Menu *trainingMenu;
	int    training_btnPressed;
	bool   training_choosed;
	int    training_countdown;


	text_bar *t_bar;
	my_ship *MS;
	HpBar   *MS_Bar;
	BulletManager *MBM;
	BulletManager *EBM;
	enemy_ship_manager *ESM;
	Map *map;

	Pool *PP;

void training_start(){
	EBM = new BulletManager;
	ESM = new enemy_ship_manager(EBM);
	MBM = new BulletManager;
	MS = new my_ship(MBM);
	MS_Bar = new HpBar(MS);
	map = new Map(0, 15, 15);
}

void training_running(Drawer *drawer){
	if(map!=NULL)map->main(drawer);
	if(MS!=NULL)MS->main(drawer);
	if(MS_Bar!=NULL)MS_Bar->main(drawer);
	if(t_bar!=NULL)t_bar->main(drawer);
	if(MBM!=NULL)MBM->main(drawer);
	if(ESM!=NULL)ESM->main(drawer);
	if(EBM!=NULL)EBM->main(drawer);
	if(PP!=NULL)PP->main(drawer);
	drawer->setMapSize(map->getAllWidth(), map->getAllHeight());
	drawer->setCamera(SCREEN_WIDTH/3, SCREEN_HEIGHT/4, SCREEN_WIDTH*2/3, SCREEN_HEIGHT/2);
	drawer->moveCamera(MS->getX(), MS->getY());
}

void training_end(){
	if(MS!=NULL){
		delete MS;
		MS=NULL;
	}
	if(MS_Bar!=NULL){
		delete MS_Bar;
		MS_Bar=NULL;
	}
	if(EBM!=NULL){
		delete EBM;
		EBM=NULL;
	}
	if(PP!=NULL){
		delete PP;
		PP=NULL;
	}
	if(map!=NULL){
		delete map;
		map=NULL;
	}
}

void collide(){

	for(int i = 0; i < 1000; i++){
		if(Image::crash( EBM->bullet + i , MS ) && EBM->bullet[i].exist ){
			EBM->bullet[i].setState(Bullet::STATE_HIT);
			MS->injured(1);
		}
	}

	for(int i = 0; i < 1000; i++){
		for(int j = 0; j < 100; j++ ){
			if(Image::crash( MBM->bullet + i , ESM->ES + j) && MBM->bullet[i].exist && ESM->ES[j].exist ){
				MBM->bullet[i].setState(Bullet::STATE_HIT);
				ESM->ES[j].injured(5);
				float rad = MBM->bullet[i].getRad();
				float x = (MBM->bullet[i].getX()*1 + ESM->ES[j].getX()*4)/5 + random(0, 20);
				float y = (MBM->bullet[i].getY()*1 + ESM->ES[j].getY()*4)/5 + random(0, 20);
				PP->addParticle(4, 1, x, y, 0, 0, 0, 0);
				break;
			}
		}
	}

}

	void change_state(int state){
		this->nextState = state;
	}

	bool game_main(Drawer *drawer){

		bool end = false;
		if(state == STATE_MAINMENU){
			//main menu
			static const int BTN_START = 0;
			static const int BTN_OPTION = 1;
			static const int BTN_MUSIC = 2;
			static const int BTN_QUIT = 3;

			if(mainMenu == NULL){
				mainMenu = new MainMenu();
			}
			else {
				mainMenu->main(drawer);
			}			
			if(mainMenu->isFinished()){
				if(mainMenu->getPressed() == 0){
					change_state(1);
				}
				else if(mainMenu->getPressed() == 3){
					end = true;
				}
				else{
					change_state(999);
				}
				delete mainMenu;
				mainMenu = NULL;
			}
		}
		else if(state == STATE_STARTMENU){
			//start menu
			static const int BTN_TRAINING = 0;
			static const int BTN_BATTLE = 1;

			if(startMenu == NULL){
				startMenu = new StartMenu();
			}
			else {
				startMenu->main(drawer);
			}
			if(startMenu->isFinished()){
				if(startMenu->getPressed() == 0){
					change_state(10);
				}
				else if(startMenu->getPressed() == 4){
					change_state(0);
				}
				else{
					change_state(999);
				}
				delete startMenu;
				startMenu = NULL;
			}
		}
		
		else if(state == STATE_TRAINMENU){
			if(trainingMenu == NULL){
				trainingMenu = new TrainingMenu();
			}
			else {
				trainingMenu->main(drawer);
			}

			
			if(trainingMenu->isFinished()){
				if(trainingMenu->getPressed()==0){
					change_state(100);
				}
				else if(trainingMenu->getPressed()==1){
					change_state(101);
				}
				else if(trainingMenu->getPressed()==2){
					change_state(102);
				}
				else if(trainingMenu->getPressed()==4){
					change_state(0);
				}
				else{
					change_state(999);
				}
				delete trainingMenu;
				trainingMenu = NULL;
			}


		}

		else if(state==100){
			if(time==0){


				training_start();
				
				MS->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);

				t_bar = new text_bar;
				t_bar -> setPosition(0,SCREEN_HEIGHT/2+70);

				PP = new Pool(10000);
				PP ->setPosition(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
				
				int **m = new2D(15, 15, int);
				for(int i = 0; i < 15; i++){	
					for(int j = 0; j < 15; j++){
						m[i][j] = 200;
					}
				}
				map->addChip(m);
			}

		

			if(time<4000){

				for(int i=0;i<20;i++){

					/*
					// BOX 1
					float r = random(0,360);
					float rr = random(0,20);
					float l = random(150,250);
					float s = random(l/30,l/18);
					float a = random(s/20,s/12);
					PP->addParticle(45,0, PP->x + turn_x(l,r) ,  PP->y + turn_y(l,r),turn_x(s,r+90+rr) ,turn_y(s,r+90+rr),turn_x(a,r+180+rr) ,turn_y(a,r+180+rr));
					*/					

					// BOX 2
					/*
					float r = random(0,360);
					float rr = random(0,20);
					float l = random(150,300);
					float s = random(l/20,l/12);
					float a = random(s/60,s/40);
					PP->addParticle(30,0, PP->getX() + vectorX(l,r) ,  PP->getY() + vectorY(l,r),vectorX(s,r+rr) ,vectorY(s,r+rr),vectorX(a,r+180+rr) ,vectorY(a,r+180+rr));
					*/

					/*			
					// BOX 3
					float r = random(0,360);
					float rr = random(0,20);
					float l = random(2,1);
					float s = random(l*4.5,l*1.6);
					float a = random(s/44,s/44);
					PP->addParticle(45,0, PP->x + turn_x(l,r) ,  PP->y + turn_y(l,r),turn_x(s,r+rr) ,turn_y(s,r+rr),turn_x(a,r+180+rr) ,turn_y(a,r+180+rr));
					*/
				}
				
			}

			if (time%30==0){
				ESM->create_ES(SCREEN_WIDTH/2, SCREEN_WIDTH/2,0,5,270,1,0);
			}
/*
			if(time>100 && time<400){
				if(time%1==0){
					//EBM->create_EB(SCREEN_WIDTH/2,SCREEN_HEIGHT-200,0,12,time*7.3,4,1,90,0,1,0,0,0);
					EBM->create_EB(SCREEN_WIDTH/2,SCREEN_HEIGHT-200,0,random(12,10),random(0,360),4,1,90,0,1,0,0,0,0);

				}
				if(time%3==0){
					ESM->create_ES(SCREEN_WIDTH/2,SCREEN_HEIGHT-200,0,7,time*2,0,0);
				}
			}
			else if(time==400){
				EBM->clear();
			}
			
			else if(time==500){
				training_end();
				change_state(10);
			}
*/
			collide();
			training_running(drawer);



/*

// Create a colour for the text - in this case blue
D3DCOLOR fontColor = D3DCOLOR_ARGB(255,0,0,255);    

// Create a rectangle to indicate where on the screen it should be drawn
RECT rct;
rct.left=2;
rct.right=780;
rct.top=10;
rct.bottom=rct.top+20;
 
// Draw some text 

std::stringstream ss;

std::string str="";

ss<<MS->hp;
ss>>str;


//d_text->DrawText(NULL, (LPCWSTR)(str), -1, &rct, 0, fontColor );


*/

		}

		else if(state==101){
			if(time==0){
				training_start();
				MS->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);
				
				t_bar = new text_bar;
				t_bar -> setPosition(0,SCREEN_HEIGHT/2+70);
			}

			for(int i=0;i<1000;i++ ){
				if(Image::crash( EBM->bullet + i , MS ) && EBM->bullet[i].exist ){
					EBM->bullet[i].setState(Bullet::STATE_HIT);
					MS->injured(1);
				}
			}

			if(time<300){
				if(time%20==0){
					EBM->addBullet(SCREEN_WIDTH/2,SCREEN_HEIGHT-200,0, 7,time*7.3, 36 , 1 , 10 , 0 ,1,100);
				}
			}
			else if(time==300){
				EBM->clear();
			}
			
			else if(time==400){
				training_end();
				change_state(10);
			}
			training_running(drawer);
		}

		else if(state==102){
			if(time==0){

				training_start();
				
				MS->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);

				t_bar = new text_bar;
				t_bar -> setPosition(0,SCREEN_HEIGHT/2+70);
			}

			for(int i=0;i<1000;i++ ){
				if(Image::crash( EBM->bullet + i , MS ) && EBM->bullet[i].exist ){
					EBM->bullet[i].setState(Bullet::STATE_HIT);
					MS->injured(1);
				}
			}


			if(time>100 && time<400){
				if(time%3==0){
					EBM->addBullet(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0,16,MS,3,1,20,0,1,100);
					EBM->addBullet(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0,random(12,10),random(0,360),4,1,90,0,1,100);
				}
			}
			else if(time==400){
				EBM->clear();
			}
			else if(time==500){
				training_end();
				change_state(10);
			}
			training_running(drawer);

		}

		if(state == nextState){
			time++;
		}
		else{
			time = 0;
			state = nextState;
		}
		
		return end;
	}
};


Game::Game(){
	state = 0;
	nextState = 0;
	time = 0;

	imgTheme = NULL;
	trainingMenu = NULL;
	mainMenu = NULL;
	startMenu = NULL;
	MS = NULL;
	MS_Bar = NULL;
	EBM = NULL;
	PP = NULL;
}

Game::~Game(){
	if(mainMenu != NULL){
		delete mainMenu;
		mainMenu = NULL;
	}
	if(startMenu != NULL){
		delete startMenu;
		startMenu = NULL;
	}
	if(MS != NULL){
		delete MS;
		MS = NULL;
	}
	if(MS_Bar != NULL){
		delete MS_Bar;
		MS_Bar = NULL;
	}
	if(EBM != NULL){
		delete EBM;
		EBM = NULL;
	}
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[] = TEXT("平田的世界");
MSG msg={0};

//時間變數
DWORD timeLast;
DWORD timeNow;
DWORD tCheck;   


int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{

////////////////////////////////////////視窗變數宣告/////////////////////////////////////////////////

HWND hWnd;
WNDCLASSEX wc;
ATOM atom;

UNREFERENCED_PARAMETER( hCurInst );

////////////////////////////////////////設定視窗/////////////////////////////////////////////////////

wc.cbSize = sizeof(WNDCLASSEX);
wc.style = CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc = WndProc;
wc.cbClsExtra =0;
wc.cbWndExtra =0;
wc.hInstance =hCurInst;
wc.hIcon =(HICON)LoadImage(NULL,MAKEINTRESOURCE(IDI_APPLICATION),IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_SHARED);
wc.hCursor =(HCURSOR)LoadImage(NULL,MAKEINTRESOURCE(IDC_ARROW),IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_SHARED);
wc.hbrBackground =(HBRUSH)GetStockObject(WHITE_BRUSH);
wc.lpszMenuName =NULL;
wc.lpszClassName = szClassName;
wc.hIconSm =(HICON)LoadImage(NULL,MAKEINTRESOURCE(IDI_APPLICATION),IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_SHARED);

////////////////////////////////////////產生視窗/////////////////////////////////////////////////////


if ((atom = RegisterClassEx(&wc) )== 0){
	return FALSE;
}

hWnd = CreateWindowW(szClassName,TEXT("平田的世界"),WS_EX_TOPMOST | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,    // set window to new resolution
						NULL, NULL,hCurInst,NULL );
if (!hWnd) {
	return FALSE;
}
ShowWindow(hWnd,nCmdShow);
UpdateWindow(hWnd);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

Drawer *drawer = new Drawer();

if(!drawer->InitD3D(hWnd)){
	return FALSE;
}

/*
if (FAILED( D3DXCreateSprite(direct3DDevice,&direct3DSprite) ) )
{
	return false;
}
*/
timeNow = GetTickCount();
timeLast = timeNow;

Game game;

while(1){
	if (PeekMessageW(&msg,NULL,0,0,PM_REMOVE)){
		if (msg.message == WM_QUIT ){
			break;
    	}
        TranslateMessage(&msg);
		DispatchMessage(&msg);	
	}

    timeNow =GetTickCount();
	if (timeNow - timeLast >= 16 ){

		timeLast = timeNow;
		if(game.game_main(drawer)){
			SendMessage(hWnd,WM_CLOSE,0,0);
		}
		drawer->drawAll();
	}
}
	
delete drawer;
return (int)msg.wParam ;
}


LRESULT CALLBACK WndProc (HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg){
	case WM_CREATE:
		srand( (unsigned)time(NULL) );
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
        if(wp==PLAYER1_LEFT){
			press(KEY_LEFT,true);
			keyboard.PRESS_LEFT = true;
		}
        if(wp==PLAYER1_RIGHT){
			press(KEY_RIGHT,true);
			keyboard.PRESS_RIGHT = true;
		}
        if(wp==PLAYER1_UP){
			 press(KEY_UP,true);
			keyboard.PRESS_UP = true;
		}
		if(wp==PLAYER1_DOWN){
			 press(KEY_DOWN,true);
			keyboard.PRESS_DOWN = true;
		}
        if(wp==PLAYER1_NATK){
			 press(KEY_NATK,true);
			 keyboard.PRESS_NATK = true;
		}
        if(wp==PLAYER1_SATK){
			 press(KEY_SATK,true);
			 keyboard.PRESS_SATK = true;
		}
		if(wp==VK_ESCAPE){
			SendMessage(hWnd,WM_CLOSE,0,0);
		}
	break;

	case WM_KEYUP:
        if(wp==PLAYER1_LEFT){
			press(KEY_LEFT,false);
			keyboard.PRESS_LEFT=false;
		}
        if(wp==PLAYER1_RIGHT){
			press(KEY_RIGHT,false);
			keyboard.PRESS_RIGHT=false;
		}
        if(wp==PLAYER1_UP){
			press(KEY_UP,false);
			keyboard.PRESS_UP=false;
		}
		if(wp==PLAYER1_DOWN){
			press(KEY_DOWN,false);
			keyboard.PRESS_DOWN=false;
		}
        if(wp==PLAYER1_NATK){
			 press(KEY_NATK,false);
			 keyboard.PRESS_NATK = false;
		}
        if(wp==PLAYER1_SATK){
			 press(KEY_SATK,false);
			 keyboard.PRESS_SATK = false;
		}
		break;
	default:
	return (DefWindowProc(hWnd,msg,wp,lp));

}

return 0;
}

