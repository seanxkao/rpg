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

queue<Object*> *Object::createQueue = new queue<Object*>;


class Game{
public:
	Game(){

		state = 0;
		nextState = 0;
		time = 0;

		currMenu = NULL;
		player = NULL;
		pool = new Pool(3000);
		TM = NULL;
		
		
		system = new System();
		avatar = new Avatar();
		
		enemyManager = new BodySystem(1000);
		//playerProjManager = new BodySystem(1000);
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

	PlayerCharacter	*player;
	MyController	*myController;
	BodySystem		*enemyManager;
	BodySystem		*playerProjManager;
	Pool			*pool;
	TextManager		*TM;
	Map				*map;
	
	void training_start(){
		myController = new MyController();
		player = new PlayerCharacter(myController);
		player->setAvatar(avatar);
		
		Bar *playerHpBar = system->createBar(0, player, player);
		playerHpBar->setParent(0, 70);
		playerHpBar->setBarSize(50, 10, 50, 10);
		
		ID3DXFont *font;
		D3DXCreateFont(system->getDrawer()->getDevice(), 46, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &font);   
		TM = new TextManager(200, font);
		
		PlayerPanel *playerPanel = new PlayerPanel(system->getDrawer(), player);
		playerPanel->setAvatar(avatar);
		
		map = new Map(0, 15, 15);		
		int **m = new2D(15, 15, int);
		for(int i=0;i<15;i++){	
			for(int j=0;j<15;j++){
				m[i][j] = 200;
			}
		}
		map->addChip(m);
		
		system->getDrawer()->setCamera(SCREEN_WIDTH/3, SCREEN_HEIGHT/4, SCREEN_WIDTH*2/3, SCREEN_HEIGHT/2);
		system->getDrawer()->setMapSize(map->getAllWidth(), map->getAllHeight());

	}

	void training_running(){
		if(pool!=NULL)pool->main();
		
		//移動攝影機
		system->getDrawer()->moveCamera(player->getX(), player->getY());
	}

	void training_end(){
		delete myController;
		player->finish();
		delete TM;	
		map->finish();
		delete pool;
	}

	void collide(){
		/*
		for(int i=0;i<1000;i++){
			for(int j=0;j<1000;j++){
				Body *playerProj = playerProjManager->getBody(i);
				Body *enemy = enemyManager->getBody(j);
				if(playerProj!=NULL && enemy!=NULL){
					if(Body::crash(playerProj, enemy)){
						playerProj->setState(Body::STATE_DAMAGED);
						enemy->onDamaged(playerProj);
						float enemyX = enemy->getX();
						float enemyY = enemy->getY();
						float playerProjX = playerProj->getX();
						float playerProjY = playerProj->getY();
						float x = (playerProjX*1 + enemyX*4)/5 + random(0, 20);
						float y = (playerProjY*1 + enemyY*4)/5 + random(0, 20);
						pool->addParticle(4, 1, x, y, 0, 0, 0, 0);

						TM->addText("5", 25, 25, 50, 50 , 255, 255, 0, 0, 0, 0, enemyX, enemyY + 80, 0, 0, enemyX, enemyY + 80, 20, 0, enemyX, enemyY + 130);
						break;
					}
				}
			}
		}
		*/
		for(int j=0;j<1000;j++){
			Body *enemy = enemyManager->getBody(j);
			if(enemy!=NULL){
				if(Body::crash(player, enemy)){
					int injure = enemy->onDamaged(player);
					float enemyX = enemy->getX();
					float enemyY = enemy->getY();
					float playerX = player->getX();
					float playerY = player->getY();
					float particleX = random((playerX*1 + enemyX*4)/5, 20);
					float particleY = random((playerY*1 + enemyY*4)/5, 20);

					pool->addParticle(4, 1, particleX, particleY, 0, 0, 0, 0);
					
					stringstream ss;
					ss<<injure;
					string test=ss.str();

					Text *text = TM->addText(test, 25, 25, 50, 50 , 255, 255, 255, 255, 0, 0, enemyX, enemyY+130, 0, 0, enemyX, enemyY+130, 20, 0, enemyX, enemyY+200);
				}
				
				if(Body::crash(enemy, player)){
					int injure = player->onDamaged(enemy);
					float enemyX = enemy->getX();
					float enemyY = enemy->getY();
					float playerX = player->getX();
					float playerY = player->getY();
					float particleX = random((playerX*4 + enemyX*1)/5, 20);
					float particleY = random((playerY*4 + enemyY*1)/5, 20);
					pool->addParticle(4, 1, particleX, particleY, 0, 0, 0, 0);

					stringstream ss;
					ss<<injure;
					string test=ss.str();

					TM->addText(test, 25, 25, 50, 50 , 255, 255, 0, 0, 0, 0, playerX, playerY + 80, 0, 0, playerX, playerY + 80, 20, 0, playerX, playerY + 130);
				}
			}
		}
	}

	void change_state(int state){
		this->nextState = state;
	}

	void draw(){
		system->draw();
		system->getDrawer()->drawAll();
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
				player->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);
				AnimeBlock *animeBlock = new AnimeBlock;
				animeBlock->setPosition(0,SCREEN_HEIGHT/2+70);
				break;
			}
			case 101:
			{
				training_start();
				player->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);
				AnimeBlock *animeBlock = new AnimeBlock;
				animeBlock->setPosition(0,SCREEN_HEIGHT/2+70);
				break;
			}
			case 102:
			{
				training_start();
				player->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);
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
		system->create();
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
			if (time%60==0){
				Enemy *enemy = enemyManager->addEnemySoldier(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,0,0);
				Bar *enemyHpBar = system->createBar(0, enemy, enemy);
				enemyHpBar->setParent(0, 70);
				enemyHpBar->setBarSize(50, 10, 50, 10);
			}
			for(int i=0;i<40;i++){
				// BOX 1
				/*
				float r = random(0,360);
				float rr = random(0,10);
				float l = random(50,80);
				float s = random(l/30,l/18);
				float a = random(s*s/l*0.5,s*s/l*1.2);
				pool->addParticle(35,0,SCREEN_WIDTH/2 + vectorX(l,r) , SCREEN_HEIGHT/2 + vectorY(l,r),vectorX(s,r+90+rr) ,vectorY(s,r+90+rr),vectorX(a,r+180+rr) ,vectorY(a,r+180+rr));
				*/
				// BOX 2
				/*
				float r = random(0,360);
				float rr = random(0,10);
				float l = random(150,300)*random(150,300)/300;
				float s = random(l/40,l/24);
				float a = random(s/60,s/40);
				pool->addParticle(30,0, SCREEN_WIDTH/2 + vectorX(l,r) ,  SCREEN_HEIGHT/2 + vectorY(l,r),vectorX(s,r+90+rr) ,vectorY(s,r+90+rr),vectorX(a,r+180+rr) ,vectorY(a,r+180+rr));
				*/
			}

			//if(time>=10000){
			//	training_end();
			//	change_state(10);
			//}
			collide();
			training_running();
		}

		else if(state==101){
			if(time<300){
			}
			else if(time==300){
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

		if(enemyManager!=NULL){
			for(int i=0;i<1000;i++){
				Enemy *enemy = (Enemy*)enemyManager->getBody(i);
				if(enemy!=NULL){
					if(enemy->isFinished()){
						int exp = enemy->getExp();
						avatar->addExp(exp);
						enemyManager->removeBody(i);
					}
				}
			}
		}
		
		system->main();
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
	if(player != NULL){
		delete player;
		player = NULL;
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

