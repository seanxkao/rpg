#include "Bar.h"
#include "Bullet.h"
#include "Button.h"
#include "Drawer.h"
#include "Enemy.h"
#include "Keyboard.h"
#include "Map.h"
#include "Menu.h"
#include "PlayerCharacter.h"
#include "Particle.h"
#include "Pool.h"
#include "System.h"
#include "Text.h"

////////////////////////////////////////物件設定/////////////////////////////////////////////////////

list<Object*> *Object::allObject = new list<Object*>;
vector<Object*> *Object::drawQueue = new vector<Object*>;


class Game{
public:
	Game(){
		state = 0;
		nextState = 0;
		time = 0;

		currMenu = NULL;
		MS = NULL;
		pool = new Pool(1000);
		TM = NULL;
		system = new System();
		avatar = new Avatar();
		
		EBM = new BodySystem(1000);
		MBM = new BodySystem(1000);
	}
	~Game();

	static const int STATE_MAINMENU = 0;
	static const int STATE_STARTMENU = 1;
	static const int STATE_TRAINMENU = 10;

	int  state;
	int  nextState;
	long time;
	
	System *system;
	Avatar *avatar;
	Menu *currMenu;

	PlayerCharacter			*MS;
	MyController	*myController;
	BodySystem		*EBM;
	BodySystem		*MBM;
	Pool			*pool;
	TextManager		*TM;
	Map				*map;
	
	void training_start(){
		myController = new MyController();
		MS = new PlayerCharacter(myController);
		MS->setAvatar(avatar);
		
		Bar *MS_Bar = system->createBar(0, MS, MS);
		MS_Bar->setParent(0, 70);
		MS_Bar->setBarSize(50, 10, 50, 10);
		
		ID3DXFont *font;
		D3DXCreateFont(system->getDrawer()->getDevice(), 46, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &font);   
		TM = new TextManager(200, font);
		
		PlayerPanel *playerPanel = new PlayerPanel(system->getDrawer(), MS);
		playerPanel->setAvatar(avatar);
		system->getDrawer()->setCamera(SCREEN_WIDTH/3, SCREEN_HEIGHT/4, SCREEN_WIDTH*2/3, SCREEN_HEIGHT/2);
		

		map = new Map(0, 15, 15);		
		int **m = new2D(15, 15, int);
		for(int i=0;i<15;i++){	
			for(int j=0;j<15;j++){
				m[i][j] = 200;
			}
		}
		map->addChip(m);
	}

	void training_running(){
		if(pool!=NULL)pool->main();
		
		//移動攝影機
		system->getDrawer()->setMapSize(map->getAllWidth(), map->getAllHeight());
		system->getDrawer()->moveCamera(MS->getX(), MS->getY());
	}

	void training_end(){
		delete myController;
		MS->finish();
		delete TM;	
		map->finish();
		delete pool;
	}

	void collide(){
		for(int i=0;i<1000;i++){
			for(int j=0;j<1000;j++){
				Body *MB = MBM->getBody(i);
				Body *EB = EBM->getBody(j);
				if(MB!=NULL && EB!=NULL){
					if(Body::crash(MB, EB)){
						MB->setState(Body::STATE_DAMAGED);
						EB->onDamaged(MB);
						float EB_X = EB->getX();
						float EB_Y = EB->getY();
						float MB_X = MB->getX();
						float MB_Y = MB->getY();
						float x = (MB_X*1 + EB_X*4)/5 + random(0, 20);
						float y = (MB_Y*1 + EB_Y*4)/5 + random(0, 20);
						pool->addParticle(4, 1, x, y, 0, 0, 0, 0);

						TM->addText("5", 25, 25, 50, 50 , 255, 255, 0, 0, 0, 0, EB_X, EB_Y + 80, 0, 0, EB_X, EB_Y + 80, 20, 0, EB_X, EB_Y + 130);
						break;
					}
				}
			}
		}
		
		for(int j=0;j<1000;j++){
			Body *EB = EBM->getBody(j);
			if(EB!=NULL){
				if(Body::crash(MS, EB)){
					int injure = EB->onDamaged(MS);
					float EB_X = EB->getX();
					float EB_Y = EB->getY();
					float MS_X = MS->getX();
					float MS_Y = MS->getY();
					float P_X = random((MS_X*1 + EB_X*4)/5, 20);
					float P_Y = random((MS_Y*1 + EB_Y*4)/5, 20);

					pool->addParticle(4, 1, P_X, P_Y, 0, 0, 0, 0);
					
					stringstream ss;
					ss<<injure;
					string test=ss.str();

					TM->addText(test, 25, 25, 50, 50 , 255, 255, 255, 255, 0, 0, EB_X, EB_Y + 80, 0, 0, EB_X, EB_Y + 80, 20, 0, EB_X, EB_Y + 130);
				}
				
				if(Body::crash(EB, MS)){
					int injure = MS->onDamaged(EB);
					float EB_X = EB->getX();
					float EB_Y = EB->getY();
					float MS_X = MS->getX();
					float MS_Y = MS->getY();
					float P_X = random((MS_X*4 + EB_X*1)/5, 20);
					float P_Y = random((MS_Y*4 + EB_Y*1)/5, 20);
					pool->addParticle(4, 1, P_X, P_Y, 0, 0, 0, 0);

					stringstream ss;
					ss<<injure;
					string test=ss.str();

					TM->addText(test, 25, 25, 50, 50 , 255, 255, 0, 0, 0, 0, MS_X, MS_Y + 80, 0, 0, MS_X, MS_Y + 80, 20, 0, MS_X, MS_Y + 130);
				}
			}
		}
	}

	void change_state(int state){
		this->nextState = state;
	}

	void draw(){
		Drawer *drawer = system->getDrawer();
		if(pool!=NULL)pool->draw(drawer);
		system->draw();
		drawer->drawAll();
	}

	void stateStart(){
		switch(state){
			case STATE_MAINMENU:
				currMenu = system->createMenu(0);
				break;
			case STATE_STARTMENU:
				currMenu = system->createMenu(1);
				break;
			case STATE_TRAINMENU:
				currMenu = system->createMenu(2);
				break;
			case 100:
			{
				training_start();
				MS->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);
				AnimeBlock *animeBlock = new AnimeBlock;
				animeBlock->setPosition(0,SCREEN_HEIGHT/2+70);
				break;
			}
			case 101:
			{
				training_start();
				MS->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);
				AnimeBlock *animeBlock = new AnimeBlock;
				animeBlock->setPosition(0,SCREEN_HEIGHT/2+70);
				break;
			}
			case 102:
			{
				training_start();
				MS->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);
				AnimeBlock *animeBlock = new AnimeBlock;
				animeBlock -> setPosition(0,SCREEN_HEIGHT/2+70);
				break;
			}
		}
	}
	void stateEnd(){
	}
	void mainStart(){
		if(state != nextState){
			stateEnd();
			time = 0;
			state = nextState;
		}
		if(time==0){
			stateStart();
		}
	}
	bool mainProc(){
		bool end = false;
		if(system!=NULL){
			system->onInput();
		}
		if(state == STATE_MAINMENU){
			const int BTN_START = 0;
			const int BTN_OPTION = 1;
			const int BTN_MUSIC = 2;
			const int BTN_QUIT = 3;
			if(currMenu!=NULL&&currMenu->isFinished()){
				int pressed = currMenu->getPressed();
				currMenu = NULL;
				if(pressed==0){
					change_state(1);
				}
				else if(pressed==3){
					end = true;
				}
				else{
					change_state(999);
				}
			}
		}
		else if(state == STATE_STARTMENU){
			//start menu
			static const int BTN_TRAINING = 0;
			static const int BTN_BATTLE = 1;
			
			if(currMenu!=NULL&&currMenu->isFinished()){
				int pressed = currMenu->getPressed();
				currMenu = NULL;
				if(pressed==0){
					change_state(10);
				}
				else if(pressed==4){
					change_state(0);
				}
				else{
					change_state(999);
				}
			}
		}
		else if(state == STATE_TRAINMENU){
			if(currMenu!=NULL&&currMenu->isFinished()){
				int pressed = currMenu->getPressed();
				currMenu = NULL;
				if(pressed==0){
					change_state(100);
				}
				else if(pressed==1){
					change_state(101);
				}
				else if(pressed==2){
					change_state(102);
				}
				else if(pressed==4){
					change_state(0);
				}
				else{
					change_state(999);
				}
			}
		}
		else if(state==100){
			myController->onInput(system->getKeyboard());
			if (time%35==0){
				Enemy *enemy = EBM->addEnemySoldier(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,0,0);
				Bar *enemyHpBar = system->createBar(0, enemy, enemy);
				enemyHpBar->setParent(0, 70);
				enemyHpBar->setBarSize(50, 10, 50, 10);
			}
			for(int i=0;i<200;i++){
				// BOX 1
				/*
				float r = random(0,360);
				float rr = random(0,10);
				float l = random(50,80);
				float s = random(l/30,l/18);
				float a = random(s/5,s/3);
				pool->addParticle(45,0,SCREEN_WIDTH/2 + vectorX(l,r) , SCREEN_HEIGHT/2 + vectorY(l,r),vectorX(s,r+90+rr) ,vectorY(s,r+90+rr),vectorX(a,r+180+rr) ,vectorY(a,r+180+rr));
				*/

				// BOX 2
				/*
				float r = random(0,360);
				float rr = random(0,20);
				float l = random(150,300);
				float s = random(l/20,l/12);
				float a = random(s/60,s/40);
				pool->addParticle(30,0, SCREEN_WIDTH/2 + vectorX(l,r) ,  SCREEN_HEIGHT/2 + vectorY(l,r),vectorX(s,r+rr) ,vectorY(s,r+rr),vectorX(a,r+180+rr) ,vectorY(a,r+180+rr));
				*/
			}

			if(time>=10000){
				training_end();
				change_state(10);
			}
			collide();
			training_running();
		}

		else if(state==101){
			if(time<300){
				if(time%20==0){
					//EBM->addBullet(SCREEN_WIDTH/2,SCREEN_HEIGHT-200,0, 7,time*7.3, 36 , 1 , 10 , 0 ,1,100);
				}
			}
			else if(time==300){
				//EBM->clear();
			}
			else if(time==400){
				training_end();
				change_state(10);
			}
			collide();
			training_running();
		}

		else if(state==102){
			if(time>100 && time<400){
				if(time%3==0){
					//EBM->addBullet(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0,16,MS,3,1,20,0,1,100);
					//EBM->addBullet(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0,random(12,10),random(0,360),4,1,90,0,1,100);
				}
			}
			else if(time==400){
			}
			else if(time==500){
				training_end();
				change_state(10);
			}
			collide();
			training_running();

		}


		system->main();
		
		if(EBM!=NULL){
			for(int i=0;i<1000;i++){
				Enemy *enemy = (Enemy*)EBM->getBody(i);
				if(enemy!=NULL){
					if(enemy->isFinished()){
						int exp = enemy->getExp();
						avatar->addExp(exp);
						EBM->removeBody(i);
					}
				}
			}
		}
		
		draw();
		system->garbageCollect();
		return end;
	}
	void mainEnd(){
		time++;
	}

	bool game_main(){
		mainStart();
		bool end = mainProc();
		mainEnd();
		return end;
	}
};

Game::~Game(){
	if(MS != NULL){
		delete MS;
		MS = NULL;
	}
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[] = TEXT("RPG");
MSG msg={0};

//時間變數
DWORD timeLast;
DWORD timeNow;
DWORD tCheck;   

Game game;

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{

////////////////////////////////////////視窗變數宣告/////////////////////////////////////////////////

HWND hWnd;
WNDCLASSEX wc;
ATOM atom;

UNREFERENCED_PARAMETER(hCurInst);

//setup window
wc.cbSize = sizeof(WNDCLASSEX);
wc.style = CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc = WndProc;
wc.cbClsExtra = 0;
wc.cbWndExtra = 0;
wc.hInstance = hCurInst;
wc.hIcon = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
wc.lpszMenuName = NULL;
wc.lpszClassName = szClassName;
wc.hIconSm = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

//create window
if ((atom = RegisterClassEx(&wc) )== 0){
	return FALSE;
}

hWnd = CreateWindow(szClassName,TEXT("RPG"),WS_EX_TOPMOST | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hCurInst, NULL);
if (!hWnd)return FALSE;

ShowWindow(hWnd,nCmdShow);
UpdateWindow(hWnd);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
Drawer *drawer = game.system->getDrawer();
if(!game.system->getDrawer()->initD3D(hWnd))return FALSE;

timeNow = GetTickCount();
timeLast = timeNow;

while(1){
	if (PeekMessageW(&msg,NULL,0,0,PM_REMOVE)){
		if (msg.message==WM_QUIT ){
			break;
    	}
        TranslateMessage(&msg);
		DispatchMessage(&msg);	
	}

    timeNow = GetTickCount();
	if (timeNow-timeLast>= 16){
		timeLast = timeNow;
		bool isFinished;
		game.system->getDrawer()->clear();
		isFinished = game.game_main();
		if(isFinished)SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
}
	
return (int)msg.wParam ;
}


LRESULT CALLBACK WndProc (HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Keyboard *keyboard = game.system->getKeyboard();
	switch(msg){
	case WM_CREATE:
		srand( (unsigned)time(NULL) );
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
        if(wp==PLAYER1_LEFT){
			keyboard->press(Keyboard::KEY_LEFT);
		}
        if(wp==PLAYER1_RIGHT){
			keyboard->press(Keyboard::KEY_RIGHT);
		}
        if(wp==PLAYER1_UP){
			keyboard->press(Keyboard::KEY_UP);
		}
		if(wp==PLAYER1_DOWN){
			keyboard->press(Keyboard::KEY_DOWN);
		}
        if(wp==PLAYER1_NATK){
			keyboard->press(Keyboard::KEY_NATK);
		}
        if(wp==PLAYER1_SATK){
			keyboard->press(Keyboard::KEY_SATK);
		}
        if(wp==PLAYER1_STAT){
			keyboard->press(Keyboard::KEY_STAT);
		}
		if(wp==VK_ESCAPE){
			SendMessage(hWnd,WM_CLOSE,0,0);
		}
	break;

	case WM_KEYUP:
        if(wp==PLAYER1_LEFT){
			keyboard->release(Keyboard::KEY_LEFT);
		}
        if(wp==PLAYER1_RIGHT){
			keyboard->release(Keyboard::KEY_RIGHT);
		}
        if(wp==PLAYER1_UP){
			keyboard->release(Keyboard::KEY_UP);
		}
		if(wp==PLAYER1_DOWN){
			keyboard->release(Keyboard::KEY_DOWN);
		}
        if(wp==PLAYER1_NATK){
			keyboard->release(Keyboard::KEY_NATK);
		}
        if(wp==PLAYER1_SATK){
			keyboard->release(Keyboard::KEY_SATK);
		}
        if(wp==PLAYER1_STAT){
			keyboard->release(Keyboard::KEY_STAT);
		}
		break;
	default:
	return (DefWindowProc(hWnd,msg,wp,lp));

}

return 0;
}

