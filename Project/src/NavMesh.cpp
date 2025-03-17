#include "NavMesh.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "LogMessages.h"
#include <cstring>
#include <cmath>
#include "Point.h"

NavMesh::NavMesh()
{
	map = nullptr;
	width = 0;
	height = 0;
}
NavMesh::~NavMesh()
{
    if (map != nullptr)
    {
        for (int i = 0; i < width; ++i)
        {
            delete[] map[i];
        }
        delete[] map;
        map = nullptr;
    }
}

AppStatus NavMesh::Load(int data[], int w, int h)
{
    size = w * h;
    width = w;
    height = h;

    if (map != nullptr) delete[] map;

    map = new Path * [width]; 
    for (int i = 0; i < width; ++i) 
    {
        map[i] = new Path[height]; 
    }

    for (int j = 0; j < height; ++j) 
    {
        for (int i = 0; i < width; ++i)
        {
            Path path = static_cast<Path>(data[j * width + i]); 
            if (data[j * width + i] == 0) path = Path::WALKABLE;
            else path = Path::NOTWALKABLE;

            map[i][j] = path;
        }
    }

	if (map == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}

std::vector<Movement> NavMesh::GetBestPath(int startPosX, int startPosY, int endPosX, int endPosY, Directions d)
{
    Point startIndex = GetPathIndex(startPosX, startPosY); 
    Point endIndex = GetPathIndex(endPosX, endPosY); 

    if (startIndex.x < 0 || startIndex.x >= width || startIndex.y < 0 || startIndex.y >= height ||
        endIndex.x < 0 || endIndex.x >= width || endIndex.y < 0 || endIndex.y >= height) {
        LOG("Error: Start or end position out of bounds.");
        return {}; 
    }

    if (map[static_cast<int>(startIndex.x)][static_cast<int>(startIndex.y)] != Path::WALKABLE) {
        LOG("Error: Start position not walkable.");
        return {}; 
    }
    if (map[static_cast<int>(endIndex.x)][static_cast<int>(endIndex.y)] != Path::WALKABLE) {
        LOG("Error: End position not walkable.");
        return {}; 
    }

    std::vector<std::vector<bool>> explored(width, std::vector<bool>(height, false));

    std::vector<std::vector<Movement>> allRoutes;
    std::vector<Movement> currentRoute;
    Movement currentMovement;

    std::vector<Movement> bestRoute;

    currentMovement.startIndex = startIndex;
    currentMovement.movedIndex = startIndex;
    currentMovement.movementDir = d;
    currentRoute.push_back(currentMovement);
    allRoutes.push_back(currentRoute);

    bool found = false;
    int iterations = 0;

    while (!found && !allRoutes.empty() && iterations < MAX_ITERATIONS)
    {
        std::vector<std::vector<Movement>> newRoutes;

        while (!allRoutes.empty() && !found)
        {
            currentRoute = allRoutes.front();
            allRoutes.erase(allRoutes.begin());

            for (int j = 0; j < 4 && !found; j++)
            {
                currentMovement.startIndex = currentRoute.back().movedIndex;

                switch ((Directions)j)
                {
                case Directions::RIGHT:
                    if (currentRoute.back().movementDir == Directions::LEFT) continue;
                    currentMovement.movedIndex.x = currentMovement.startIndex.x + 1;
                    currentMovement.movedIndex.y = currentMovement.startIndex.y;
                    break;
                case Directions::LEFT:
                    if (currentRoute.back().movementDir == Directions::RIGHT) continue;
                    currentMovement.movedIndex.x = currentMovement.startIndex.x - 1;
                    currentMovement.movedIndex.y = currentMovement.startIndex.y;
                    break;
                case Directions::UP:
                    if (currentRoute.back().movementDir == Directions::DOWN) continue;
                    currentMovement.movedIndex.x = currentMovement.startIndex.x;
                    currentMovement.movedIndex.y = currentMovement.startIndex.y - 1;
                    break;
                case Directions::DOWN:
                    if (currentRoute.back().movementDir == Directions::UP) continue;
                    currentMovement.movedIndex.x = currentMovement.startIndex.x;
                    currentMovement.movedIndex.y = currentMovement.startIndex.y + 1;
                    break;
                }

                if (currentMovement.movedIndex.x < 0 || currentMovement.movedIndex.x >= width ||
                    currentMovement.movedIndex.y < 0 || currentMovement.movedIndex.y >= height)
                {
                    continue;
                }

                if (map[static_cast<int>(currentMovement.movedIndex.x)][static_cast<int>(currentMovement.movedIndex.y)] == Path::WALKABLE &&
                    !explored[static_cast<int>(currentMovement.movedIndex.x)][static_cast<int>(currentMovement.movedIndex.y)])
                {
                    currentMovement.movementDir = static_cast<Directions>(j);
                    if (currentMovement.movedIndex.x == endIndex.x && currentMovement.movedIndex.y == endIndex.y)
                    {
                        std::vector<Movement> newRoute = currentRoute;
                        newRoute.push_back(currentMovement);
                        bestRoute = newRoute;
                        bestRoute.erase(bestRoute.begin());
                        found = true;
                    }
                    else
                    {
                        std::vector<Movement> newRoute = currentRoute;
                        newRoute.push_back(currentMovement);
                        newRoutes.push_back(newRoute);
                        explored[static_cast<int>(currentMovement.movedIndex.x)][static_cast<int>(currentMovement.movedIndex.y)] = true;
                    }
                }
            }
        }

        allRoutes.insert(allRoutes.end(), newRoutes.begin(), newRoutes.end());
        iterations++;
    }

    std::vector<Movement> filteredRoute;
    if (!bestRoute.empty())
    {
        filteredRoute.push_back(bestRoute[0]);

        for (size_t i = 1; i < bestRoute.size(); ++i)
        {
            if (bestRoute[i].movementDir != bestRoute[i - 1].movementDir)
            {
                filteredRoute.back().movedIndex = bestRoute[i].startIndex;
                filteredRoute.push_back(bestRoute[i]);
            }
            else
            {
                filteredRoute.back().movedIndex = bestRoute[i].movedIndex;
            }
        }
    }

    return filteredRoute;
}

Point NavMesh::GetPathIndex(int pixel_x, int pixel_y) const
{

    Point index;
    index.x = static_cast<int>(pixel_x / PATH_SIZE);
    index.y = static_cast<int>(pixel_y / PATH_SIZE);

	if (index.x < 0 || index.x >= width)
	{
		LOG("Error: Index out of bounds on x. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, index.x, index.y);
	}
	if (index.y < 0 || index.y >= height)
	{
		LOG("Error: Index out of bounds on y. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, index.x, index.y);
	}

	return index;
}

bool NavMesh::ItsWalkable(Point point)
{
    Point pointIndex = GetPathIndex(point.x, point.y);
    if (pointIndex.x < 0 || pointIndex.x >= width || pointIndex.y < 0 || pointIndex.y >= height) {
        return false;
    }
    return map[pointIndex.x][pointIndex.y] == Path::WALKABLE;
}

bool NavMesh::CheckIfPointInIndex(Point point, Point index)
{
    Point pointIndex = GetPathIndex(point.x, point.y);
    return pointIndex.x == index.x && pointIndex.y == index.y;
}

Point NavMesh::GetPixelToGo(Point d, int s)
{
    Point pixel;
    int displacement = (PATH_SIZE - s)/2;

    pixel.x = (d.x * PATH_SIZE) + displacement;
    pixel.y = (d.y * PATH_SIZE) + displacement;

    return pixel;
}

Point NavMesh::GetPathCenterPos(int index_x, int index_y)
{
	Point centerPos;
	centerPos.x = index_x * PATH_SIZE + PATH_SIZE / 2;
	centerPos.y = index_y * PATH_SIZE + PATH_SIZE / 2;

	return centerPos;
}

Point NavMesh::GetPathToPixels(int index_x, int index_y)
{
    Point centerPos;
    centerPos.x = index_x * PATH_SIZE;
    centerPos.y = index_y * PATH_SIZE;

    return centerPos;
}

bool NavMesh::CheckItsJustAbove(Point p, int size)
{
    int displacement = (PATH_SIZE - size) / 2;
    if ((p.x+displacement) % PATH_SIZE == 0 && (p.y + displacement) % PATH_SIZE == 0) return true;
    else return false;
}
bool NavMesh::RightHalf(Point p)
{
    return p.x > (width / 2) - 1;
}

void NavMesh::DrawDebug(const Color& color)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Point path = { i, j};
            if (map[i][j] == Path::WALKABLE)
            {
                DrawRectangle(GetPixelToGo(path, PATH_SIZE).x, GetPixelToGo(path, PATH_SIZE).y, PATH_SIZE, PATH_SIZE, CLITERAL(Color){ 50, 50, 50, 255 });
            }
            else if (map[i][j] == Path::NOTWALKABLE)
            {
                DrawRectangle(GetPixelToGo(path, PATH_SIZE).x, GetPixelToGo(path, PATH_SIZE).y, PATH_SIZE, PATH_SIZE, CLITERAL(Color){ 0, 0, 0, 255 });
            }
        }
    }
}