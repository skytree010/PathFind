#include "JumpPointSerch.h"
#include "stdafx.h"


JPS::JPS()
{
	EX = dfDEFAULTEX;
	EY = dfDEFAULTEY;

	Start = new stNODE;
	Start->X = dfDEFAULTSX;
	Start->Y = dfDEFAULTSY;
	Start->Parent = nullptr;
	Start->G = 0;
	Start->H = ((abs(Start->X - EX) + (abs(Start->Y - EY))));
	Start->F = Start->G + Start->H;

	memset(TileMap, 0, sizeof(char[dfMAP_HEIGHT][dfMAP_WIDTH]));

	OpenList = new stList;
	OpenList->Data = nullptr;
	OpenList->Priv = nullptr;
	OpenList->Next = OpenList_End;

	OpenList_End = new stList;
	OpenList_End->Next = nullptr;
	OpenList_End->Priv = OpenList;
	OpenList_End->Data = nullptr;

	CloseList = new stList;
	CloseList->Data = nullptr;
	CloseList->Next = nullptr;
	CloseList->Priv = nullptr;

	Find = nullptr;
}

JPS::~JPS()
{
	EraseAll();
	delete Start;
}


JPS::stNODE* JPS::GetList(stList* ListType)
{
	stNODE* Node;
	stList* DelList;
	if (ListType->Next == nullptr || ListType->Next == OpenList_End)
		return nullptr;
	Node = ListType->Next->Data;
	DelList = ListType->Next;
	ListType->Next = DelList->Next;
	if (DelList->Next != nullptr)
		DelList->Next->Priv = ListType;
	delete(DelList);
	return Node;
}

bool JPS::InsertNode(stList* ListType, stNODE *Node)
{
	stList* NewList;
	NewList = new stList;
	if (NewList == nullptr)
		return false;
	NewList->Data = Node;
	if (ListType == OpenList)
	{
		NewList->Next = OpenList_End;
		NewList->Priv = OpenList_End->Priv;
		OpenList_End->Priv->Next = NewList;
		OpenList_End->Priv = NewList;
		return true;
	}
	else if (ListType == CloseList)
	{
		NewList->Next = CloseList->Next;
		NewList->Priv = CloseList;
		if (CloseList->Next == nullptr)
		{
			CloseList->Next = NewList;
			return true;
		}
		CloseList->Next->Priv = NewList;
		CloseList->Next = NewList;
		return true;
	}
	return false;
}

bool JPS::SortList(stList* ListType)
{
	stList *Point;
	stList *NextPoint;
	stList *Search;
	int PointF;

	Search = ListType->Next;
	if (Search == OpenList_End)
		return true;
	Point = Search->Next;
	while (1)
	{
		if (Point == OpenList_End)
			break;
		NextPoint = Point->Next;
		PointF = Point->Data->F;
		Search = ListType->Next;
		while (Search != Point)
		{
			if (PointF < Search->Data->F)
			{
				Point->Priv->Next = Point->Next;
				Point->Next->Priv = Point->Priv;
				Point->Next = Search;
				Point->Priv = Search->Priv;
				Search->Priv->Next = Point;
				Search->Priv = Point;
				break;
			}
			Search = Search->Next;
		}
		Point = NextPoint;
	}
	return true;
}

bool JPS::CreateNode(stNODE *pNode)
{
	int iX;
	int iY;
	int NodeX;
	int NodeY;
	int pX;
	int pY;
	int ResultX;
	int ResultY;
	if (pNode == nullptr)
		return false;
	if (pNode->Parent == nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			if (Jump(pNode->X, pNode->Y, i, &iX, &iY))
			{
 				if(iX != pNode->X || iY != pNode->Y)
					MakeNode(pNode, iX, iY);
			}
		}
		return true;
	}
	NodeX = pNode->X;
	NodeY = pNode->Y;
	pX = pNode->Parent->X;
	pY = pNode->Parent->Y;
	iX = NodeX - pX;
	iY = NodeY - pY;
	
	if (iX > 0 && iY == 0)		//RR
	{
		if (Jump(NodeX + 1, NodeY, Dir_RR, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (CheckTile(NodeX, NodeY + 1) == false && CheckTile(NodeX + 1, NodeY + 1) == true)
		{
			if (Jump(NodeX + 1, NodeY + 1, Dir_DR, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		if (CheckTile(NodeX, NodeY - 1) == false && CheckTile(NodeX + 1, NodeY - 1) == true)
		{
			if (Jump(NodeX + 1, NodeY - 1, Dir_UR, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		return true;
	}
	else if (iX > 0 && iY > 0)		//RD
	{
		if (Jump(NodeX + 1, NodeY + 1, Dir_DR, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (Jump(NodeX + 1, NodeY, Dir_RR, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (Jump(NodeX, NodeY + 1, Dir_DD, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (CheckTile(NodeX - 1, NodeY) == false && CheckTile(NodeX - 1, NodeY + 1) == true)
		{
			if (Jump(NodeX - 1, NodeY + 1, Dir_DL, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		if (CheckTile(NodeX, NodeY - 1) == false && CheckTile(NodeX + 1, NodeY - 1) == true)
		{
			if (Jump(NodeX + 1, NodeY - 1, Dir_UR, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		return true;
	}
	else if (iX == 0 && iY > 0)		//DD
	{
		if (Jump(NodeX, NodeY + 1, Dir_DD, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (CheckTile(NodeX - 1, NodeY) == false && CheckTile(NodeX - 1, NodeY + 1) == true)
		{
			if (Jump(NodeX - 1, NodeY + 1, Dir_DL, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		if (CheckTile(NodeX + 1, NodeY) == false && CheckTile(NodeX + 1, NodeY + 1) == true)
		{
			if (Jump(NodeX + 1, NodeY + 1, Dir_DR, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		return true;
	}
	else if (iX < 0 && iY > 0)		//LD
	{
		if (Jump(NodeX - 1, NodeY + 1, Dir_DL, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (Jump(NodeX - 1, NodeY, Dir_LL, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (Jump(NodeX, NodeY + 1, Dir_DD, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (CheckTile(NodeX, NodeY - 1) == false && CheckTile(NodeX - 1, NodeY - 1) == true)
		{
			if (Jump(NodeX - 1, NodeY - 1, Dir_UL, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		if (CheckTile(NodeX + 1, NodeY) == false && CheckTile(NodeX + 1, NodeY + 1) == true)
		{
			if (Jump(NodeX + 1, NodeY + 1, Dir_DR, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		return true;
	}
	else if (iX < 0 && iY == 0)		//LL
	{
		if (Jump(NodeX - 1, NodeY, Dir_LL, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (CheckTile(NodeX, NodeY - 1) == false && CheckTile(NodeX - 1, NodeY - 1) == true)
		{
			if (Jump(NodeX - 1, NodeY - 1, Dir_UL, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		if (CheckTile(NodeX, NodeY + 1) == false && CheckTile(NodeX - 1, NodeY + 1) == true)
		{
			if (Jump(NodeX - 1, NodeY + 1, Dir_DL, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		return true;
	}
	else if (iX < 0 && iY < 0)		//LU
	{
		if (Jump(NodeX - 1, NodeY - 1, Dir_UL, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (Jump(NodeX - 1, NodeY, Dir_LL, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (Jump(NodeX, NodeY - 1, Dir_UU, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (CheckTile(NodeX + 1, NodeY) == false && CheckTile(NodeX + 1, NodeY - 1) == true)
		{
			if (Jump(NodeX + 1, NodeY - 1, Dir_UR, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		if (CheckTile(NodeX, NodeY + 1) == false && CheckTile(NodeX - 1, NodeY + 1) == true)
		{
			if (Jump(NodeX - 1, NodeY + 1, Dir_DL, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		return true;
	}
	else if (iX == 0 && iY < 0)		//UU
	{
		if (Jump(NodeX, NodeY - 1, Dir_UU, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (CheckTile(NodeX + 1, NodeY) == false && CheckTile(NodeX + 1, NodeY - 1) == true)
		{
			if (Jump(NodeX + 1, NodeY - 1, Dir_UR, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		if (CheckTile(NodeX - 1, NodeY) == false && CheckTile(NodeX - 1, NodeY - 1) == true)
		{
			if (Jump(NodeX - 1, NodeY - 1, Dir_UL, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		return true;
	}
	else if (iX > 0 && iY < 0)		//RU
	{
		if (Jump(NodeX + 1, NodeY - 1, Dir_UR, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (Jump(NodeX, NodeY - 1, Dir_UU, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (Jump(NodeX + 1, NodeY, Dir_RR, &ResultX, &ResultY))
		{
			if (!MakeNode(pNode, ResultX, ResultY))
				return false;
		}
		if (CheckTile(NodeX - 1, NodeY) == false && CheckTile(NodeX - 1, NodeY - 1) == true)
		{
			if (Jump(NodeX - 1, NodeY - 1, Dir_UL, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		if (CheckTile(NodeX, NodeY + 1) == false && CheckTile(NodeX + 1, NodeY + 1) == true)
		{
			if (Jump(NodeX + 1, NodeY + 1, Dir_DR, &ResultX, &ResultY))
			{
				if (!MakeNode(pNode, ResultX, ResultY))
					return false;
			}
		}
		return true;
	}

	else if (iX == 0 && iX == 0)
	{
		if (!MakeNode(pNode, ResultX, ResultY))
			return false;
		return true;
	}
	return false;
}

bool JPS::MakeNode(stNODE *pParent, int iX, int iY)
{
	stNODE *NewNode;
	stNODE *Search;
	int NextG = pParent->G + (abs(iX - pParent->X) + abs(iY - pParent->Y));
	if (iX < 0 || iX > dfMAP_WIDTH - 1 || iY < 0 || iY > dfMAP_HEIGHT)
		return true;
	Search = SearchNode(OpenList, iX, iY);
	if (Search != nullptr)
	{
		if (NextG < Search->G)
		{
			Search->Parent = pParent;
			Search->G = NextG;
			Search->F = NextG + Search->H;
			SortList(OpenList);
		}
		return true;
	}
	Search = SearchNode(CloseList, iX, iY);
	if (Search != nullptr)
	{
		if (NextG < Search->G)
		{
			Search->Parent = pParent;
			Search->G = NextG;
			Search->F = NextG + Search->H;
		}
		return true;
	}

	NewNode = new stNODE;
	if (NewNode == nullptr)
		return false;
	NewNode->X = iX;
	NewNode->Parent = pParent;
	NewNode->Y = iY;
	NewNode->G = NextG;
	NewNode->H = (abs(NewNode->X - EX) + abs(NewNode->Y - EY));
	NewNode->F = NewNode->G + NewNode->H;
	InsertNode(OpenList, NewNode);
	SortList(OpenList);
	return true;
}

JPS::stNODE* JPS::SearchNode(stList *ListType, int iX, int iY)
{
	stList *Point;
	Point = ListType->Next;
	while (1)
	{
		if (Point == nullptr || Point == OpenList_End)
		{
			return nullptr;
		}
		if (Point->Data->X == iX && Point->Data->Y == iY)
		{
			return Point->Data;
		}
		Point = Point->Next;
	}
	
}

bool JPS::JumpPointSearch(int IsFirst)
{
	if (IsFirst == 1)
	{
		StartNode = new stNODE;
		StartNode->X = Start->X;
		StartNode->Y = Start->Y;
		StartNode->H = Start->H;
		StartNode->G = Start->G;
		StartNode->F = Start->F;
		StartNode->Parent = nullptr;
	}
	if (!CreateNode(StartNode))
		return false;
	InsertNode(CloseList, StartNode);
	StartNode = GetList(OpenList);
	if (StartNode == nullptr)
		return false;
	if (StartNode->X == EX && StartNode->Y == EY)
		return true;
	return false;
}

int JPS::FindPath(Path **path, int MaxSize)
{
	int Count = 0;
	stNODE *temp_node;
	StartNode = new stNODE;
	StartNode->X = Start->X;
	StartNode->Y = Start->Y;
	StartNode->H = Start->H;
	StartNode->G = Start->G;
	StartNode->F = Start->F;
	StartNode->Parent = nullptr;

	while (StartNode->X != EX || StartNode->Y != EY)
	{
		if (!CreateNode(StartNode))
			return 0;
		InsertNode(CloseList, StartNode);
		StartNode = GetList(OpenList);
		if (StartNode == nullptr)
			return 0;
	}
	temp_node = StartNode;
	Count++;
	while (StartNode->Parent != nullptr)
	{
		StartNode->Parent->Next = StartNode;
		StartNode = StartNode->Parent;
		Count++;
	}
	if (Count > MaxSize)
		Count = MaxSize;
	if (Find != nullptr)
	{
		delete[] Find;
	}
	Find = new Path[Count];
	for (int i = 0; i < Count; i++)
	{
		Find[i].X = StartNode->X;
		Find[i].Y = StartNode->Y;
		StartNode = StartNode->Next;
	}
	StartNode = temp_node;
	delete(StartNode);
	*path = Find;
	return Count;

}

bool JPS::Jump(int iX, int iY, int iDir, int *pJumpX, int *pJumpY)
{
	if (!CheckTile(iX, iY))
		return false;
	if (iX == EX && iY == EY)
	{
		*pJumpX = iX;
		*pJumpY = iY;
		return true;
	}
	switch (iDir)
	{
	case Dir_UU:
		return Jump_UU(iX, iY, pJumpX, pJumpY);
	case Dir_UR:
		return Jump_UR(iX, iY, pJumpX, pJumpY);
	case Dir_RR:
		return Jump_RR(iX, iY, pJumpX, pJumpY);
	case Dir_DR:
		return Jump_DR(iX, iY, pJumpX, pJumpY);
	case Dir_DD:
		return Jump_DD(iX, iY, pJumpX, pJumpY);
	case Dir_DL:
		return Jump_DL(iX, iY, pJumpX, pJumpY);
	case Dir_LL:
		return Jump_LL(iX, iY, pJumpX, pJumpY);
	case Dir_UL:
		return Jump_UL(iX, iY, pJumpX, pJumpY);
	}
	
	return false;
}

bool JPS::Jump_UU(int iX, int iY, int *pJumpX, int *pJumpY)
{
	while (CheckTile(iX, iY) != false)
	{
		if (iX == EX && iY == EY)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX + 1, iY) == false && CheckTile(iX + 1, iY - 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		else if (CheckTile(iX - 1, iY) == false && CheckTile(iX - 1, iY - 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		iY--;
	}
	return false;
}

bool JPS::Jump_UR(int iX, int iY, int *pJumpX, int *pJumpY)
{
	int Temp_X;
	int Temp_Y;
	while (CheckTile(iX, iY) != false)
	{
		if (iX == EX && iY == EY)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX - 1, iY) == false && CheckTile(iX - 1, iY - 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		else if (CheckTile(iX, iY + 1) == false && CheckTile(iX + 1, iY + 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (Jump_UU(iX, iY - 1, &Temp_X, &Temp_Y))
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (Jump_RR(iX + 1, iY, &Temp_X, &Temp_Y))
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		iX++;
		iY--;
	}
	return false;
}

bool JPS::Jump_RR(int iX, int iY, int *pJumpX, int *pJumpY)
{
	while (CheckTile(iX, iY) != false)
	{
		if (iX == EX && iY == EY)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX, iY - 1) == false && CheckTile(iX + 1, iY - 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX, iY + 1) == false && CheckTile(iX + 1, iY + 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		iX++;
	}
	return false;
}

bool JPS::Jump_DR(int iX, int iY, int *pJumpX, int *pJumpY)
{
	int Temp_X;
	int Temp_Y;
	while (CheckTile(iX, iY) != false)
	{
		if (iX == EX && iY == EY)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX - 1, iY) == false && CheckTile(iX - 1, iY + 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX, iY - 1) == false && CheckTile(iX + 1, iY - 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (Jump_DD(iX, iY + 1, &Temp_X, &Temp_Y))
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (Jump_RR(iX + 1, iY, &Temp_X, &Temp_Y))
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		iX++;
		iY++;
	}
	return false;
}

bool JPS::Jump_DD(int iX, int iY, int *pJumpX, int *pJumpY)
{
	while (CheckTile(iX, iY) != false)
	{
		if (iX == EX && iY == EY)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX - 1, iY) == false && CheckTile(iX - 1, iY + 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX + 1, iY) == false && CheckTile(iX + 1, iY + 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}	
		iY++;
	}
	return false;
}

bool JPS::Jump_DL(int iX, int iY, int *pJumpX, int *pJumpY)
{
	int Temp_X;
	int Temp_Y;
	while (CheckTile(iX, iY) != false)
	{
		if (iX == EX && iY == EY)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX + 1, iY) == false && CheckTile(iX + 1, iY + 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX, iY - 1) == false && CheckTile(iX - 1, iY - 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (Jump_DD(iX, iY + 1, &Temp_X, &Temp_Y))
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (Jump_LL(iX - 1, iY, &Temp_X, &Temp_Y))
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		iX--;
		iY++;
	}
	return false;
}

bool JPS::Jump_LL(int iX, int iY, int *pJumpX, int *pJumpY)
{
	while (CheckTile(iX, iY) != false)
	{
		if (iX == EX && iY == EY)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX, iY - 1) == false && CheckTile(iX - 1, iY - 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX, iY + 1) == false && CheckTile(iX - 1, iY + 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		iX--;
	}
	return false;
}

bool JPS::Jump_UL(int iX, int iY, int *pJumpX, int *pJumpY)
{
	int Temp_X;
	int Temp_Y;
	while (CheckTile(iX, iY) != false)
	{
		if (iX == EX && iY == EY)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX + 1, iY) == false && CheckTile(iX + 1, iY - 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (CheckTile(iX, iY + 1) == false && CheckTile(iX - 1, iY + 1) == true)
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (Jump_UU(iX, iY - 1, &Temp_X, &Temp_Y))
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		if (Jump_LL(iX - 1, iY, &Temp_X, &Temp_Y))
		{
			*pJumpX = iX;
			*pJumpY = iY;
			return true;
		}
		iX--;
		iY--;
	}
	return false;
}

void JPS::SetStart(int X, int Y)
{
	if (CheckTile(X, Y) == false)
		return;
	Start->X = X;
	Start->Y = Y;
	Start->H = ((abs(Start->X - EX) + (abs(Start->Y - EY))));
	Start->F = Start->G + Start->H;
}

void JPS::GetStart(int *X, int *Y)
{
	*X = Start->X;
	*Y = Start->Y;
}

void JPS::SetEnd(int X, int Y)
{
	if (CheckTile(X, Y) == false)
		return;
	EX = X;
	EY = Y;
	Start->H = ((abs(Start->X - EX) + (abs(Start->Y - EY))));
	Start->F = Start->G + Start->H;
}

void JPS::GetEnd(int *X, int *Y)
{
	*X = EX;
	*Y = EY;
}

void JPS::SetBlock(int X, int Y, int State)
{
	if (X < 0 || X > dfMAP_WIDTH || Y < 0 || Y > dfMAP_HEIGHT)
		return;
	if (State == 1)
	{
		if (TileMap[Y][X] == 0)
			TileMap[Y][X] = 1;
	}
	else if (State == 0)
	{
		if (TileMap[Y][X] == 1)
			TileMap[Y][X] = 0;
	}
}
char JPS::GetBlock(int X, int Y)
{
	return TileMap[Y][X];
}

void JPS::EraseBlock()
{
	memset(TileMap, 0, sizeof(char[dfMAP_HEIGHT][dfMAP_WIDTH]));
}

bool JPS::EraseAll()
{
	stList *ListNode;
	stList *NextNode;
	ListNode = OpenList->Next;
	while (1)
	{
		if (ListNode == OpenList_End || ListNode == nullptr)
			break;
		NextNode = ListNode->Next;
		delete ListNode->Data;
		delete ListNode;
		ListNode = NextNode;
	}
	ListNode = CloseList->Next;
	while (1)
	{
		if (ListNode == nullptr)
			break;
		NextNode = ListNode->Next;
		delete ListNode->Data;
		delete ListNode;
		ListNode = NextNode;
	}
	OpenList->Next = OpenList_End;
	OpenList_End->Priv = OpenList;

	CloseList->Next = nullptr;
	return true;
}