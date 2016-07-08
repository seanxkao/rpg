#include "Bar.h"
#include "Bullet.h"
#include "Button.h"
#include "Controller.h"
#include "Enemy.h"
#include "Keyboard.h"
#include "Map.h"
#include "Menu.h"
#include "Particle.h"
#include "Pool.h"
#include "System.h"
#include "Text.h"

////////////////////////////////////////物件設定/////////////////////////////////////////////////////

Drawer *drawer = new Drawer();

class my_ship : public Body {
public:
	int revive;
	my_ship(BodyManager*, Controller*);
	my_ship();
	~my_ship();

	void setController(Controller *controller){
		this->controller = controller;
	}
	void setAvatar(Avatar *avatar){
		this->avatar = avatar;
	}
	void moveBody(){

		float direction = controller->getDirection();
		if	(direction>=0){
			this->direction = direction;
			Vector2D *velocity = Vector2D::polar(8, direction);

			x += velocity->getX();
			y += velocity->getY();

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

	virtual void draw(Drawer *drawer){
		switch(state){
			case STATE_NORMAL:

				setImage(50,50,50,50,0,0);
				if(time%16 == 0){
					setImgId(1000);
				}
				else if(time%16 == 4){
					setImgId(1001);
				}
				else if(time%16 == 8){
					setImgId(1000);
				}
				else if(time%16 == 12){
					setImgId(1002);
				}
				break;
			case STATE_DAMAGED:
				
				setImage(50,50,50,50,0,0);
				break;
			case STATE_DISAPPEAR:
				
				if(time<25){
					imgRad+=17;
					setARGB(255-time * 10);
					setImage(30+time*7,30+time*7,30+time*7,30+time*7,0,imgRad);
				}
				else{
					setARGB(0);
				}
				break;
		}
		Body::draw(drawer);
	}
	virtual void mainStart(){
		bdyEnable = false;
		atkEnable = false;
		Body::mainStart();
	}

	virtual void mainProc(){

		if(state==0){
			setBody(BDY_NORMAL, 20);
			setSpeed(3, 0, 0);
			moveBody();
			if(controller->getNormalAtk()){
				setState(1);
			}
			if(hp<0)setState(99);
		}

		else if(state==1){
			if(time==0){
				if(rand()<RAND_MAX/2)
					swordShadow->wield(5, direction - 100, direction + 100);
				else
					swordShadow->wield(5, direction + 100, direction - 100);

			}
			if(time==1){
				status = avatar->getStatus();
				int atk = (int)randomRange(status->getMinPAtk(), status->getMaxPAtk());
				setAttack(atk, 0, 10, 100, direction-45, direction+45);
			}

			setBody(BDY_NORMAL, 25);
			if(time == 0){
				//bulletManager->addBullet(x,y,0,0,0.0,1,1,10,0,1,0);
			}
			else{		
				//moveBody();
			}
			if(time > 4){
				setState(0);
			}
		}
		else if(state==10){
			if(time>4){
				setState(0);
			}
		}

		else if(state==99){
		}
		
		revive++;
		if(revive>=60){
			if(hp<maxHp)hp++;
			revive = 0;
		}

		Body::mainProc();
	}

protected:

	Controller *controller;
	Avatar *avatar;

};

my_ship::my_ship(BodyManager *manager, Controller *controller) : Body(manager){
	setController(controller);
	setImage(30,30,30,30,0,0);
	imgId = 1000;
	maxHp=100;
	hp=100;
	revive = 0;
	status = new Status(10);
}

my_ship::my_ship() :
	Body(NULL){
	setImage(30,30,30,30,0,0);
	imgId = 1000;
	maxHp = 100;
	hp = 100;
	status = new Status(10);
}

my_ship::~my_ship(){
}


class Game{
public:
	Game(){
		state = 0;
		nextState = 0;
		time = 0;

		imgTheme = NULL;
		trainingMenu = NULL;
		mainMenu = NULL;
		startMenu = NULL;
		MS = NULL;
		MS_Bar = NULL;
		PP = NULL;
		TM = NULL;
		keyboard = new Keyboard(20, 10);
		system = new System(1000, 100);
		
		avatar = new Avatar();
	}
	~Game();

	static const int STATE_MAINMENU = 0;
	static const int STATE_STARTMENU = 1;
	static const int STATE_TRAINMENU = 10;

	int  state;
	int  nextState;
	long time;
	
	Keyboard *keyboard;
	System *system;
	Avatar *avatar;
	Anime *imgTheme;
	Menu *mainMenu;
	Menu *startMenu;
	Menu *trainingMenu;

	AnimeBlock		*t_bar;
	my_ship			*MS;
	HpBar			*MS_Bar;
	MyController	*myController;
	BodySystem		*EBM;
	BodySystem		*MBM;
	Pool			*PP;
	TextManager		*TM;
	Map				*map;
	

Keyboard* getKeyboard(){
	return keyboard;
} 


void training_start(){
	EBM = new BodySystem(1000);
	MBM = new BodySystem(1000);

	myController = new MyController();
	MS = new my_ship(NULL, myController);
	MS->setAvatar(avatar);
	MS_Bar = new HpBar(MS, MS);
	MS_Bar->setTarget(0, 70);
	MS_Bar->setBarSize(50, 10, 50, 10);

	
	PlayerPanel *playerPanel = new PlayerPanel(MS);
	playerPanel->setAvatar(avatar);
	system->addCom(playerPanel);

	TM = new TextManager(200);
	map = new Map(0, 15, 15);
	drawer->setCamera(SCREEN_WIDTH/3, SCREEN_HEIGHT/4, SCREEN_WIDTH*2/3, SCREEN_HEIGHT/2);
	
	t_bar = new AnimeBlock;
	t_bar -> setPosition(0,SCREEN_HEIGHT/2+70);

	PP = new Pool(10000);
	
	int **m = new2D(15, 15, int);
	for(int i = 0; i < 15; i++){	
		for(int j = 0; j < 15; j++){
			m[i][j] = 200;
		}
	}
	map->addChip(m);
}

void training_running(){
	if(map!=NULL)map->main();
	if(t_bar!=NULL)t_bar->main();
	if(EBM!=NULL)EBM->main();

	if(MS!=NULL)MS->main();
	if(MS_Bar!=NULL)MS_Bar->main();

	if(PP!=NULL)PP->main();
	if(TM!=NULL)TM->main();

	//移動攝影機
	drawer->setMapSize(map->getAllWidth(), map->getAllHeight());
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
	if(PP!=NULL){
		delete PP;
		PP=NULL;
	}
	if(map!=NULL){
		delete map;
		map=NULL;
	}
	if(TM!=NULL){
		delete TM;
		TM=NULL;
	}
}

void collide(){
	

	for(int i = 0; i < 1000; i++){
		for(int j = 0; j < 100; j++ ){
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
					PP->addParticle(4, 1, x, y, 0, 0, 0, 0);

					TM->addText("5", 25, 25, 50, 50 , 255, 255, 0, 0, 0, 0, EB_X, EB_Y + 80, 0, 0, EB_X, EB_Y + 80, 20, 0, EB_X, EB_Y + 130);
					break;
				}
			}
		}
	}
	
	for(int j = 0; j < 100; j++ ){
		Body *EB = EBM->getBody(j);
		if(EB!=NULL){
			if(Body::crash(MS , EB)){
				int injure = EB->onDamaged(MS);

				float EB_X = EB->getX();
				float EB_Y = EB->getY();
				float MS_X = MS->getX();
				float MS_Y = MS->getY();

				float P_X = random((MS_X*1 + EB_X*4)/5, 20);
				float P_Y = random((MS_Y*1 + EB_Y*4)/5, 20);

				PP->addParticle(4, 1, P_X, P_Y, 0, 0, 0, 0);

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
				PP->addParticle(4, 1, P_X, P_Y, 0, 0, 0, 0);

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


	void draw(Drawer *drawer){
		if(map!=NULL){
			map->draw(drawer);
		}
		if(t_bar!=NULL)t_bar->draw(drawer);
		if(MBM!=NULL)MBM->draw(drawer);
		if(EBM!=NULL)EBM->draw(drawer);
		if(MS!=NULL)MS->draw(drawer);
		if(MS_Bar!=NULL)MS_Bar->draw(drawer);
		if(PP!=NULL)PP->draw(drawer);
		if(TM!=NULL)TM->draw(drawer);
		if(system!=NULL)system->draw(drawer);

		//移動攝影機
		if(map!=NULL){
			drawer->setMapSize(map->getAllWidth(), map->getAllHeight());
		}
		if(MS!=NULL){
			drawer->moveCamera(MS->getX(), MS->getY());
		}
	}

	void stateStart(){
		switch(state){
			case STATE_MAINMENU:
				mainMenu = new MainMenu();
				system->addCtrlCom(mainMenu);
				break;
			case STATE_STARTMENU:
				startMenu = new StartMenu();
				system->addCtrlCom(startMenu);
				break;
			case STATE_TRAINMENU:
				trainingMenu = new TrainingMenu();
				system->addCtrlCom(trainingMenu);
				break;
			case 100:
				training_start();
				MS->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);
				break;
			case 101:
				training_start();
				MS->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);
				t_bar = new AnimeBlock;
				t_bar -> setPosition(0,SCREEN_HEIGHT/2+70);
				break;
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
		keyboard->main();
		if(system!=NULL){
			system->main();
			system->onInput(keyboard);
		}
		if(state == STATE_MAINMENU){
			const int BTN_START = 0;
			const int BTN_OPTION = 1;
			const int BTN_MUSIC = 2;
			const int BTN_QUIT = 3;

			if(mainMenu->isFinished()){
				//切換狀態
				if(mainMenu->getPressed() == 0){
					change_state(1);
				}
				else if(mainMenu->getPressed() == 3){
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
			}
		}
		
		else if(state == STATE_TRAINMENU){

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
			}
		}

		else if(state==100){
			myController->onInput(keyboard);
			//if(myController->getStat())playerPanel->setState(1);
			

				if (time%35==0){
					Enemy *enemy = EBM->addEnemySoldier(SCREEN_WIDTH/2, SCREEN_WIDTH/2,0,0);
					HpBar *hpBar = new HpBar(enemy, enemy);
					hpBar->setTarget(0, 70);
					hpBar->setBarSize(50, 10, 50, 10);
					system->addCom(hpBar);
				}
				for(int i=0;i<20;i++){

					/*
					// BOX 1
					float r = random(0,360);
					float rr = random(0,20);
					float l = random(150,250);
					float s = random(l/30,l/18);
					float a = random(s/20,s/12);
					PP->addParticle(45,0,SCREEN_WIDTH/2 + vectorX(l,r) , SCREEN_HEIGHT/2 + vectorY(l,r),vectorX(s,r+90+rr) ,vectorY(s,r+90+rr),vectorX(a,r+180+rr) ,vectorY(a,r+180+rr));
					*/		

					// BOX 2
					/*
					float r = random(0,360);
					float rr = random(0,20);
					float l = random(150,300);
					float s = random(l/20,l/12);
					float a = random(s/60,s/40);
					PP->addParticle(30,0, SCREEN_WIDTH/2 + vectorX(l,r) ,  SCREEN_HEIGHT/2 + vectorY(l,r),vectorX(s,r+rr) ,vectorY(s,r+rr),vectorX(a,r+180+rr) ,vectorY(a,r+180+rr));
					*/

				}
				
			

/*
			if(time>100 && time<400){
				if(time%1==0){
					//EBM->create_EB(SCREEN_WIDTH/2,SCREEN_HEIGHT-200,0,12,time*7.3,4,1,90,0,1,0,0,0);
					EBM->create_EB(SCREEN_WIDTH/2,SCREEN_HEIGHT-200,0,random(12,10),random(0,360),4,1,90,0,1,0,0,0,0);

				}
				if(time%3==0){
					EBM->addEnemy(SCREEN_WIDTH/2,SCREEN_HEIGHT-200,0,0);
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
			if(time==0){

				training_start();
				
				MS->setPosition(SCREEN_WIDTH/2,FRAME_DOWN+50);

				t_bar = new AnimeBlock;
				t_bar -> setPosition(0,SCREEN_HEIGHT/2+70);
			}


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


		if(EBM!=NULL){
			for(int i = 0; i < 100; i++){
				Enemy *EB = (Enemy*)EBM->getBody(i);
				if(EB!=NULL){
					if(EB->isFinished()){
						int exp = EB->getExp();
						avatar->addExp(exp);
						EBM->release(i);
					}
				}
			}
		}

		system->garbageCollect();

		if(state == nextState){
			time++;
		}
		

		return end;
	}
	void mainEnd(){
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
	if(MS_Bar != NULL){
		delete MS_Bar;
		MS_Bar = NULL;
	}
	if(keyboard != NULL){
		delete keyboard;
		keyboard = NULL;
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

////////////////////////////////////////設定視窗/////////////////////////////////////////////////////

wc.cbSize = sizeof(WNDCLASSEX);
wc.style = CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc = WndProc;
wc.cbClsExtra =0;
wc.cbWndExtra =0;
wc.hInstance =hCurInst;
wc.hIcon =(HICON)LoadImage(NULL,IDI_APPLICATION,IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_SHARED);
wc.hCursor =(HCURSOR)LoadImage(NULL,IDC_ARROW,IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_SHARED);
wc.hbrBackground =(HBRUSH)GetStockObject(WHITE_BRUSH);
wc.lpszMenuName =NULL;
wc.lpszClassName = szClassName;
wc.hIconSm =(HICON)LoadImage(NULL,IDI_APPLICATION,IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_SHARED);

////////////////////////////////////////產生視窗/////////////////////////////////////////////////////


if ((atom = RegisterClassEx(&wc) )== 0){
	return FALSE;
}

hWnd = CreateWindow(szClassName,TEXT("RPG"),WS_EX_TOPMOST | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,    // set window to new resolution
						NULL, NULL,hCurInst,NULL );
if (!hWnd) {
	return FALSE;
}
ShowWindow(hWnd,nCmdShow);
UpdateWindow(hWnd);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

drawer = new Drawer();
if(!drawer->initD3D(hWnd))return FALSE;

timeNow = GetTickCount();
timeLast = timeNow;

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
		drawer->clear();
		if(game.game_main()){
			SendMessage(hWnd,WM_CLOSE,0,0);
		}
		game.draw(drawer);
		drawer->drawAll();
	}
}
	
delete drawer;
return (int)msg.wParam ;
}


LRESULT CALLBACK WndProc (HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Keyboard *keyboard = game.getKeyboard();
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

