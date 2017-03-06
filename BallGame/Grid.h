#pragma once

#include "Ball.h"


struct CCell
{
	std::vector<CBall *> m_balls;
};

class CGrid
{
	friend class CBallController;

public:
	CGrid(int width, int height, int cellSize);
	~CGrid();

	//add ball to the grid
	void AddBall(CBall* pball);
	//add ball to specific cell
	void AddBall(CBall* pball, CCell* pcell);
	//get cell based on cell coordinates
	CCell* GetCell(int x, int y);
	//get cell based on position
	CCell* GetCell(const glm::vec2& pos);
	//remove ball from it's own cell
	void RemoveBall(CBall* pball);

private:
	//use vector to replace 2D array
	std::vector<CCell> m_cells;
	int m_iCellSize;
	int m_iWidth;
	int m_iHeight;
	int m_iCellX;
	int m_iCellY;

};

