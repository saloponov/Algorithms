/*
 * qtree.h
 *
 *  Created on: 08.05.2012
 *      Author: Saloponov Ivan ,MIFI,group K2-122, year:  2012
 *
 */

#ifndef QTREE_H_
#define QTREE_H_

/*
 * N - КОЛИЧЕСТВО ЭЛЕМЕНТОВ В УЗЛЕ
 */
#define N 2
/*
 * НАЧАЛЬНЫЕ ГРАНИЦЫ
 */
#define StartXMIN -1000000000000
#define StartXMAX 1000000000000//9999999999
#define StartYMIN -1000000000000
#define StartYMAX 1000000000000///9999999999


typedef struct Info{//структура с информацией находящаяся непосредственно в элементах q-дерева
	int x;
	int y;
	char *str;
}Info;

typedef struct Data{//структура ввода
	int x;
	int y;
	char *string;
	struct Data* next;
}Data;

typedef struct Node{// структура элемента q-дерева
	int xmax;//
	int xmin;
	int ymax;
	int ymin;
	float ymid,xmid;//середина по У и по Х
	int occupancy,depth[4];//заполненность узла
	Info *info[N];//указатели на информацию
	struct Node *branch[4],*p;//указатели на потомков
}Node;

typedef struct Tree{//структура хранящая корень дерева
	Node *root;
}Tree;

typedef Info *nw;

int 	AddNode(Tree*,Info *);//Добавление в дерево Tree, элемента Info
int 	PointBelongsToSquare(int xmax,int xmin,int ymax,int ymin,int xpnt,int ypnt);//проверяет принадлежность точки к определённому квадрату
Tree 	*CreateParent(Tree *rt,Info *t);//Создаёт родительский элемент к текущему дереву, если точка не входит в границы текущего корня
int 	Expansion(Node * cur, Node *new, int branch,int xmax,int xmin,int ymax,int ymin);
Node 	*Find(Node *,Info *,Node **);//поиск в дереве элемента Info
Node 	*ChooseBranch(Node *x,Info *t);//Выбор ветки в которую идём имея определённые ключи
Info 	*Search(Node *,Info *);//поиск элемента Info в дереве , возвращает указатель на нуж
int		DeepestElement(Node *cur,int i,Node **,int *);//поиск листа высота до которого от элемента cur максимальная (не использует значения глубины в cur)
int		DelEmptyLeaves(Node *parent);//если потомки узла parent пусты , удаляем их
int 	delelement(Info *t,Tree * rt);//удаление элемента Info *t в Дереве
void 	DelAllTree(Node*);//очистка всего дерева
int 	createNewChildren(Node *cur);//создание 4x новых потомков
int 	max(int *);//поиск максимального в массиве int
int 	CompareBranches(Node *cur,Node *parent);//возвращает номер ветки в parent в которой находится cur какой
int		DeepestElement(Node *cur,int i,Node **,int *);
void 	depthDown(Node *cur);//изменяет значения высоты(глубины) дерева при изменинии высоты (глубины)
Node 	*WhereMaxDepth(Node *cur);//возвращает номер потомка в котором наибольшая высота(Глубина)
int 	DepthMod(Node *cur);//увеличивает значения высоты дерева (при добавлении)
int 	OrderMin(Node* cur,Data* data,int min);
char 	*strdup(const char *str);
//int	PrintInOrder(Tree *);
//nw	*SortOrder(Node*,Info *Order[],unsigned int*);
//char	*enterString(char*);
/*Node 	*Find(Node *,Info *);
Node 	*ChooseBranch(Node *x,Info *t);
Info 	*Search(Node *,Info *);*/
//int 	BelongsPointToSqare(int xmax,int xmin,int ymax,int ymin,int xpnt,int ypnt);
//Tree 	*CreateParent(Tree *rt,Info *t);
//int 	Expansion(Node * cur, Node *new, int branch,int xmax,int xmin,int ymax,int ymin);
//Tree 	*AddNode(Tree*,Info *);



#endif /* QTREE_H_ */
