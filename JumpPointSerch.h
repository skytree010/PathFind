#pragma once
#define dfMAP_HEIGHT 100
#define dfMAP_WIDTH 150
#define dfDEFAULTSX 5
#define dfDEFAULTSY 5
#define dfDEFAULTEX 20
#define dfDEFAULTEY 20
#define CheckTile(X, Y) ((TileMap[Y][X]!=1)&&(X>=0)&&(X<=(dfMAP_WIDTH-1))&&(Y>=0)&&(Y<=(dfMAP_HEIGHT-1)))
class JPS
{
public:
	struct Path
	{
		int X;
		int Y;
	};
	struct stNODE
	{
		int X;
		int Y;
		stNODE* Parent;
		stNODE* Next;
		int F;
		int G;
		int H;
	};
private:
	char TileMap[dfMAP_HEIGHT][dfMAP_WIDTH];
	enum eDirection
	{
		Dir_UU,
		Dir_UR,
		Dir_RR,
		Dir_DR,
		Dir_DD,
		Dir_DL,
		Dir_LL,
		Dir_UL
	};
	
	struct stList
	{
		stList *Priv;
		stList *Next;
		stNODE *Data;
	};
	stList *OpenList;
	stList *OpenList_End;
	stList *CloseList;
	stNODE *Start;
	int EX;
	int EY;
	stNODE *StartNode;
	Path *Find;

	stNODE* GetList(stList* ListType);
	bool InsertNode(stList* ListType, stNODE* Node);
	bool SortList(stList* ListType);
	stNODE* SearchNode(stList *ListType, int iX, int iY);
	bool Jump(int iX, int iY, int iDir, int *pJumpX, int *pJumpY);
	bool Jump_UU(int iX, int iY, int *pJumpX, int *pJumpY);
	bool Jump_UR(int iX, int iY, int *pJumpX, int *pJumpY);
	bool Jump_RR(int iX, int iY, int *pJumpX, int *pJumpY);
	bool Jump_DR(int iX, int iY, int *pJumpX, int *pJumpY);
	bool Jump_DD(int iX, int iY, int *pJumpX, int *pJumpY);
	bool Jump_DL(int iX, int iY, int *pJumpX, int *pJumpY);
	bool Jump_LL(int iX, int iY, int *pJumpX, int *pJumpY);
	bool Jump_UL(int iX, int iY, int *pJumpX, int *pJumpY);

	bool MakeNode(stNODE *pParent, int iX, int iY);
	bool CreateNode(stNODE *pNode);
public:
	
	JPS();
	~JPS();
	bool JumpPointSearch(int IsFirst);
	int FindPath(Path **path, int MaxSize);
	void SetStart(int X, int Y);
	void GetStart(int *X, int *Y);
	void SetEnd(int X, int Y);
	void GetEnd(int *X, int *Y);
	void SetBlock(int X, int Y, int State);
	char GetBlock(int X, int Y);
	void EraseBlock();
	bool EraseAll();
};
