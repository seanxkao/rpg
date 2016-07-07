#pragma once

#include "Image.h"

class Map : public Object{
public:
	Map(int, int, int);
	virtual ~Map();

	int mapId;

	float getAllWidth(){
		return mapWidth * mapChipWidth;
	}
	
	float getAllHeight(){
		return mapHeight * mapChipHeight;
	}
	void addChip(int x, int y, int imgId, bool forbid){
		mapChip[x][y] = new MapChip(imgId);
		mapChip[x][y]->setPosition(this->x + x*mapChipWidth, this->y + y*mapChipHeight);
		mapChip[x][y]->setImage(0, 0, mapChipWidth, mapChipHeight, 0, 0);
		mapChip[x][y]->forbid = forbid;
	}
	
	void addChip(int x, int y, int imgId){
		addChip(x, y, imgId, false);
	}

	void addChip(int **map){
		for(int i = 0; i < mapWidth; i++){	
			for(int j = 0; j < mapHeight; j++){
				addChip(i, j, map[i][j]);
			}
		}
	}
	void draw(Drawer *drawer){
		for(int i = 0; i < mapWidth; i++){	
			for(int j = 0; j < mapHeight; j++){
				mapChip[i][j]->draw(drawer);
			}
		}
	}

	virtual void mainProc(){
	}
	

protected:
	class MapChip : public Image{
	public:
		MapChip();
		MapChip(int);
		virtual ~MapChip();
		
		bool forbid;

	};

	float mapChipWidth;
	float mapChipHeight;
	
	int mapWidth;
	int mapHeight;
	
	MapChip ***mapChip;

};

Map::MapChip::MapChip() :
	Image(){

};
Map::MapChip::MapChip(int imgId) :
	Image(imgId){

};
Map::MapChip::~MapChip(){
};

Map::Map(int mapId, int mapWidth, int mapHeight) :
	Object(){
	x = 0;
	y = 0;
	mapChipWidth = 60;
	mapChipHeight = 60;
	this->mapId = mapId;
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;


	mapChip = new2D(mapWidth, mapHeight, MapChip*);
}

Map::~Map(){
}