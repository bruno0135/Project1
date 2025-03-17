#pragma once
#include <raylib.h>
#include "Globals.h"
#include <unordered_map>
#include "Point.h"

enum class Path {
	
	NOTWALKABLE = 1,
	WALKABLE = 0,
};

class Movement {
public: 
	Point startIndex;
	Point movedIndex;
	Directions movementDir;
};

class NavMesh
{
public:
	NavMesh();
	~NavMesh();

	AppStatus Load(int data[], int w, int h);
	std::vector<Movement> GetBestPath(int sx, int sy, int fx, int fy, Directions dir);
	Point GetPixelToGo(Point index, int size);
	bool CheckIfPointInIndex(Point point, Point index);
	void DrawDebug(const Color& col);
	Point GetPathIndex(int x, int y) const;
	bool CheckItsJustAbove(Point point, int size);
	bool ItsWalkable(Point p);
	bool RightHalf(Point p);
	Point GetPathCenterPos(int index_x, int index_y);
	Point GetPathToPixels(int index_x, int index_y);
private:

	//Tile map
	Path** map;

	//Size of the tile map
	int size, width, height;
};

