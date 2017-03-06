#include "Grid.h"



CGrid::CGrid(int width, int height, int cellSize) : 
	m_iWidth(width),
	m_iHeight(height),
	m_iCellSize(cellSize)
{
	m_iCellX = (int)ceil(m_iWidth / m_iCellSize);
	m_iCellY = (int)ceil(m_iHeight / m_iCellSize);

	if (m_iCellSize > m_iWidth) m_iCellX = 1;
	if (m_iCellSize > m_iHeight) m_iCellY = 1;


	//Pre Allocate all the cells
	const int RESERVIEDBALL = 20;
	m_cells.resize(m_iCellX * m_iCellY);
	for (size_t i = 0; i < m_cells.size(); i++)
	{
		//reserve balls for cell
		m_cells[i].m_balls.reserve(RESERVIEDBALL);
	}


}


CGrid::~CGrid()
{
}



void CGrid::AddBall(CBall* pBall)
{
	CCell* pCell = GetCell(pBall->m_cPosition);
	pCell->m_balls.push_back(pBall);
	pBall->m_pCell = pCell;
	pBall->m_iCellIndex = pCell->m_balls.size() - 1;
}

void CGrid::AddBall(CBall* pBall, CCell* pCell)
{
	pCell->m_balls.push_back(pBall);
	pBall->m_pCell = pCell;
	pBall->m_iCellIndex = pCell->m_balls.size() - 1;
}

CCell* CGrid::GetCell(int x, int y)
{
	if (x < 0)
	{
		x = 0;
	}
	if (x >= m_iCellX)
	{
		x = m_iCellX - 1;
	}
	if (y < 0)
	{
		y = 0;
	}
	if (y >= m_iCellY)
	{
		y = m_iCellY - 1;
	}

	return &m_cells[y * m_iCellX + x];
}

CCell* CGrid::GetCell(const glm::vec2& pos)
{
	int x = (int)(pos.x / m_iCellSize);
	int y = (int)(pos.y / m_iCellSize);
	return GetCell(x, y);
}

void CGrid::RemoveBall(CBall* pBall)
{
	//reference to avoid extra typing
	std::vector<CBall*>& balls = pBall->m_pCell->m_balls;

	balls.back()->m_iCellIndex = pBall->m_iCellIndex;

	//delete an element from a vector
	balls[pBall->m_iCellIndex] = balls.back();
	balls.pop_back();

	pBall->m_iCellIndex = -1;
	pBall->m_pCell = nullptr;
}

