#pragma once

#include "Image.h"

class Map: public Object{
public:
	Map(int mapId, int mapWidth, int mapHeight): Object(){
		x = 0;
		y = 0;
		mapChipWidth = 60;
		mapChipHeight = 60;
		this->mapId = mapId;
		this->mapWidth = mapWidth;
		this->mapHeight = mapHeight;
		mapChip = new2D(mapWidth, mapHeight, MapChip*);
	}

	virtual ~Map(){
	}

	float getAllWidth(){
		return mapWidth*mapChipWidth;
	}
	
	float getAllHeight(){
		return mapHeight*mapChipHeight;
	}
	
	void addChip(int x, int y, int imgId, bool forbid){
		mapChip[x][y] = new MapChip(imgId);
		mapChip[x][y]->setPosition(this->x+x*mapChipWidth, this->y+y*mapChipHeight);
		mapChip[x][y]->setImage(0, 0, mapChipWidth, mapChipHeight, 0);
		mapChip[x][y]->forbid = forbid;
	}
	
	void addChip(int x, int y, int imgId){
		addChip(x, y, imgId, false);
	}

	void addChip(int **map){
		for(int i=0;i<mapWidth;i++){	
			for(int j=0;j<mapHeight;j++){
				addChip(i, j, map[i][j]);
			}
		}
	}

protected:
	float mapChipWidth;
	float mapChipHeight;
	int mapWidth;
	int mapHeight;
	int mapId;
	
	class MapChip: public Image{
	public:
		MapChip(int imgId): Image(imgId){
			setZ(0);
		};
		virtual ~MapChip(){
		};
		bool forbid;
	};
	MapChip ***mapChip;
};

