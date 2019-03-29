#pragma once
#include<math.h>
#define XMAXSIZE 100
#define YMAXSIZE 50
#define DEFAULTSX 10
#define DEFAULTSY 20
#define DEFAULTEX 50
#define DEFAULTEY 20
class Astar
{
public:
	Astar();
	~Astar();
	bool FindWay(int IsFirst);
	void SetStart(int X, int Y);
	void GetStart(int *X, int *Y);
	void SetEnd(int X, int Y);
	void GetEnd(int *X, int *Y);
	void SetBlock(int X, int Y, int State);
	char GetBlock(int X, int Y);
	bool EraseAll();
	bool GetOpenNode(int X, int Y);
	bool GetCloseNode(int X, int Y);
	void EraseBlock();
	struct Node
	{
		int X;
		int Y;
		Node* Parent;
		int F;
		int G;
		int H;
	};
	Node* GetLastNode();
private:
	
	struct List
	{
		List *Priv;
		List *Next;
		Node *Data;
		List *Last;
	};
	char Map[YMAXSIZE][XMAXSIZE];
	Node *Start;
	Node *StartNode;
	Node* End;
	List* OpenList;
	List* CloseList;
	bool MakeNode(Node* Parent, int X, int Y, int GRise = 5);
	bool CreateNode(Node* RefNode);
	bool CheckMap(int X, int Y);
	Node* SerchNode(List *ListType, int X, int Y);
	bool InsertNode(List *ListType, Node* NewNode);
	bool SortList(List *ListType);
	Node* GetList(List *ListType);
	
};