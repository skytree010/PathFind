#include "stdafx.h"
#include "AStar.h"

Astar::Astar()
{
	End = new Node;
	End->X = DEFAULTEX;
	End->Y = DEFAULTEY;
	End->Parent = nullptr;
	End->G = 0;
	End->H = 0;
	End->F = 0;

	Start = new Node;
	Start->X = DEFAULTSX;
	Start->Y = DEFAULTSY;
	Start->Parent = nullptr;
	Start->G = 0;
	Start->H = 5 * (abs(Start->X - End->X) + abs(Start->Y - End->Y));
	Start->F = Start->G + Start->H;
	StartNode = Start;

	memset(Map, 0, sizeof(char[YMAXSIZE][XMAXSIZE]));

	OpenList = new List;
	OpenList->Data = nullptr;
	OpenList->Priv = nullptr;
	OpenList->Next = nullptr;
	OpenList->Last = nullptr;

	CloseList = new List;
	CloseList->Data = nullptr;
	CloseList->Priv = nullptr;
	CloseList->Next = nullptr;
	CloseList->Last = nullptr;
}

Astar::~Astar()
{
	delete(Start);
	delete(End);
	//Not perfect will find other node, which is under the start node.
}

bool Astar::FindWay(int IsFirst)
{
	if (IsFirst == 1)
	{
		StartNode = new Node;
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
	if (StartNode->X == End->X && StartNode->Y == End->Y)
		return true;
	return false;
}

void Astar::SetStart(int X, int Y)
{
	if (X < 0 || X > XMAXSIZE - 1 || Y < 0 || Y > YMAXSIZE - 1)
		return;
	Start->X = X;
	Start->Y = Y;
}

void Astar::GetStart(int *X, int *Y)
{
	*X = Start->X;
	*Y = Start->Y;
}

void Astar::SetEnd(int X, int Y)
{
	if (X < 0 || X > XMAXSIZE - 1 || Y < 0 || Y > YMAXSIZE - 1)
		return;
	End->X = X;
	End->Y = Y;
}

void Astar::GetEnd(int *X, int *Y)
{
	*X = End->X;
	*Y = End->Y;
}

void Astar::SetBlock(int X, int Y, int State)
{
	if (State == 1)
	{
		if (Map[Y][X] == 0)
			Map[Y][X] = 1;
	}
	else if (State == 0)
	{
		if (Map[Y][X] == 1)
			Map[Y][X] = 0;
	}
}

char Astar::GetBlock(int X, int Y)
{
	return Map[Y][X];
}

bool Astar::EraseAll()
{
	List* ListNode;
	List* NextList;
	ListNode = OpenList->Next;
	while (1)
	{
		if (ListNode == nullptr)
			break;
		NextList = ListNode->Next;
		delete(ListNode->Data);
		delete(ListNode);
		ListNode = NextList;
	}
	ListNode = CloseList->Next;
	while (1)
	{
		if (ListNode == nullptr)
			break;
		NextList = ListNode->Next;
		delete(ListNode->Data);
		delete(ListNode);
		ListNode = NextList;
	}

	OpenList->Next = nullptr;
	OpenList->Last = nullptr;

	CloseList->Next = nullptr;
	CloseList->Last = nullptr;
	return true;
}

bool Astar::GetOpenNode(int X, int Y)
{
	List* Serch;
	Serch = OpenList->Next;
	while (1)
	{
		if (Serch == nullptr)
			return false;
		if (Serch->Data->X == X && Serch->Data->Y == Y)
			return true;
		Serch = Serch->Next;
	}
}

bool Astar::GetCloseNode(int X, int Y)
{
	List* Serch;
	Serch = CloseList->Next;
	while (1)
	{
		if (Serch == nullptr)
			return false;
		if (Serch->Data->X == X && Serch->Data->Y == Y)
			return true;
		Serch = Serch->Next;
	}
}

void Astar::EraseBlock()
{
	memset(Map, 0, sizeof(char[YMAXSIZE][XMAXSIZE]));
}

Astar::Node* Astar::GetLastNode()
{
	return StartNode;
}

bool Astar::MakeNode(Node* Parent, int X, int Y, int GRise)
{
	Node *NewNode;
	Node *Serch;
	int NextG = Parent->G + GRise;
	if (X < 0 || X > XMAXSIZE - 1 || Y < 0 || Y > YMAXSIZE - 1)
		return true;
	Serch = SerchNode(OpenList, X, Y);
	if (Serch != nullptr)
	{
		if (NextG < Serch->G)
		{
			Serch->Parent = Parent;
			Serch->G = NextG;
			Serch->F = NextG + Serch->H;
			SortList(OpenList);	//sorting
		}
		return true;
	}
	Serch = SerchNode(CloseList, X, Y);
	if (Serch != nullptr)
	{
		if (NextG < Serch->G)
		{
			Serch->Parent = Parent;
			Serch->G = NextG;
			Serch->F = NextG + Serch->H;	//not serting
		}
		return true;
	}
		
	NewNode = new Node;
	if (NewNode == nullptr)
		return false;
	NewNode->Parent = Parent;
	NewNode->X = X;
	NewNode->Y = Y;
	NewNode->G = Parent->G + GRise;
	NewNode->H = 5 * (abs(NewNode->X - End->X) + abs(NewNode->Y - End->Y));
	NewNode->F = NewNode->G + NewNode->H;
	InsertNode(OpenList, NewNode);
	SortList(OpenList);	//sorting
	return true;
}

bool Astar::CreateNode(Node* RefNode)
{
	if (RefNode == nullptr)
		return false;
	int X = RefNode->X;
	int Y = RefNode->Y;
	if (CheckMap(X + 1, Y))
	{
		if (!MakeNode(RefNode, X + 1, Y, 5))
			return false;
	}
	if (CheckMap(X + 1, Y + 1))
	{
		if (!MakeNode(RefNode, X + 1, Y + 1, 7))
			return false;
	}
	if (CheckMap(X, Y + 1))
	{
		if (!MakeNode(RefNode, X, Y + 1, 5))
			return false;
	}
	if (CheckMap(X - 1, Y + 1))
	{
		if (!MakeNode(RefNode, X - 1, Y + 1, 7))
			return false;
	}
	if (CheckMap(X - 1, Y))
	{
		if (!MakeNode(RefNode, X - 1, Y, 5))
			return false;
	}
	if (CheckMap(X - 1, Y - 1))
	{
		if (!MakeNode(RefNode, X - 1, Y - 1, 7))
			return false;
	}
	if (CheckMap(X, Y - 1))
	{
		if (!MakeNode(RefNode, X, Y - 1, 5))
			return false;
	}
	if (CheckMap(X + 1, Y - 1))
	{
		if (!MakeNode(RefNode, X + 1, Y - 1, 7))
			return false;
	}
	return true;
}

bool Astar::CheckMap(int X, int Y)
{
	if (Map[Y][X] == 1)
		return false;
	else
		return true;
}

Astar::Node* Astar::SerchNode(List *ListType, int X, int Y)
{
	List *Point;
	Point = ListType->Next;
	while (1)
	{
		if (Point == nullptr)
			return nullptr;
		if (Point->Data->X == X && Point->Data->Y == Y)
		{
			return Point->Data;
		}
		Point = Point->Next;
	}
}

bool Astar::InsertNode(List *ListType, Node* NewNode)
{
	List* NewList;
	NewList = new List;
	if (NewList == nullptr)
		return false;

	NewList->Data = NewNode;
	if (ListType->Last == nullptr)
	{
		NewList->Next = nullptr;
		NewList->Priv = ListType;
		ListType->Next = NewList;
		ListType->Last = NewList;
		return true;
	}
	NewList->Next = nullptr;
	NewList->Priv = ListType->Last;
	ListType->Last->Next = NewList;
	ListType->Last = NewList;
	return true;
}

bool Astar::SortList(List *ListType)//Using Insert sort because this list is almost sorted so insert sort is fastest way to sort.
{
	List *Point;
	List *NextPoint;
	List *Serch;
	int PointF;
	Serch = ListType->Next;
	if (Serch == nullptr)
		return false;
	Point = ListType->Next->Next;
	while (1)
	{
		if (Point == nullptr)
			break;
		NextPoint = Point->Next;
		PointF = Point->Data->F;
		Serch = ListType->Next;
		while (Serch != Point)
		{
			if (PointF < Serch->Data->F)
			{
				Point->Priv->Next = Point->Next;
				if (Point->Next == nullptr)
					ListType->Last = Point->Priv;
				else
					Point->Next->Priv = Point->Priv;
				Point->Next = Serch;
				Point->Priv = Serch->Priv;
				Serch->Priv->Next = Point;
				Serch->Priv = Point;
				break;
			}
			Serch = Serch->Next;
		}
		Point = NextPoint;
	}
	return true;
}

Astar::Node* Astar::GetList(List* ListType)
{
	Node* node;
	List* DelList;
	if (ListType->Next == nullptr)
		return nullptr;
	node = ListType->Next->Data;
	DelList = ListType->Next;
	ListType->Next = DelList->Next;
	if(DelList->Next != nullptr)
		DelList->Next->Priv = ListType;
	delete(DelList);
	return node;
}