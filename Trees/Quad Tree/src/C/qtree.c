#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "qtree.h"


char *strdup(const char *str)
{
	int n = strlen(str) + 1;
	char *dup = malloc(n);
	if (dup)
	{
		strcpy(dup, str);
	}
	return dup;
}

 Tree *CreateParent(Tree *rt, Info *t)//функция создающая необходимое количество родительских элементов при выходе за границы текущего корня
 {
	 if (rt->root != NULL)
	 {
		 Node *new;
		 Node *cur = rt->root;// указатели на новый и текущий элементы

		 int i;
		 int xabs = abs(cur->xmax - cur->xmin),
			 yabs = abs(cur->ymax - cur->ymin);//длина границ

		 cur->p = (Node*)calloc(1, sizeof(Node));//новый родительский элемент

		 new = cur->p;
		 new->occupancy = 0;
		 new->p = NULL;

		 for (i = 0; i < 4; ++i)
		 {
			 new->branch[i] = NULL;
			 new->depth[i] = 0;
		 }

		 for (i = 0; i < N; ++i)
			 new->info[i] = NULL;

		 rt->root = new;//корнем становится новый элемент

		 /*проверяем в какой области лежит добавляемый элемент, присоединяем существующее дерево к нужному листу new и задаются нужные границы у эл-та new*/
		 /***********************************************/
		 if (PointBelongsToSquare(cur->xmax + xabs, cur->xmin, cur->ymax + yabs, cur->ymin, t->x, t->y)) //проверка в какой области лежит новая точка
		 {
			 Expansion(cur, new, 0, cur->xmax + xabs, cur->xmin, cur->ymax + yabs, cur->ymin);//задаёт необходимые параметры новому узлу дерева
			 //		|		|	vert = y
			 //		| 0|	|	hor = x
			 return rt;
		 }
		 else if (PointBelongsToSquare(cur->xmax + xabs, cur->xmin, cur->ymax, cur->ymin - yabs, t->x, t->y)) //проверка в какой области лежит новая точка
		 {
			 //|  1| 	|
			 //|			|
			 Expansion(cur, new, 1, cur->xmax + xabs, cur->xmin, cur->ymax, cur->ymin - yabs);//задаёт необходимые параметры новому узлу дерева
			 return rt;
		 }
		 else if (PointBelongsToSquare(cur->xmax, cur->xmin - xabs, cur->ymax + yabs, cur->ymin, t->x, t->y))//проверка в какой области лежит новая точка
		 {
			 //	|		|
			 //	|	|2	|
			 Expansion(cur, new, 2, cur->xmax, cur->xmin - xabs, cur->ymax + yabs, cur->ymin);//задаёт необходимые параметры новому узлу дерева
			 return rt;
		 }
		 else if (PointBelongsToSquare(cur->xmax, cur->xmin - xabs, cur->ymin, cur->ymin - yabs, t->x, t->y))//проверка в какой области лежит новая точка
		 {
			 //	|	|4	|
			 //	|		|
			 Expansion(cur, new, 3, cur->xmax, cur->xmin - xabs, cur->ymax, cur->ymin - yabs);//задаёт необходимые параметры новому узлу дерева
			 return rt;

		 }

		 /**************************************************/
		 /* если пространства недостаточно расширятся дальше*/
		 else if ((t->x > cur->xmin) && (t->y > cur->ymin))
		 {
			 Expansion(cur, new, 0, cur->xmax + xabs, cur->xmin, cur->ymax + yabs, cur->ymin);//задаёт необходимые параметры новому узлу дерева и переносит с самого низа в новый узел всю информацию
			 rt = CreateParent(rt, t);//расширяем дальше начиная с нового корня
			 return rt;
			 //		|		|	vertical = y
			 //		| 0|	|	horisontal = x
		 }
		 else if ((t->x > cur->xmin) && (t->y < cur->ymin))
		 {
			 Expansion(cur, new, 1, cur->xmax + xabs, cur->xmin, cur->ymax, cur->ymin - yabs);//задаёт необходимые параметры новому узлу дерева и переносит с самого низа в новый узел всю информацию
			 rt = CreateParent(rt, t);//расширяем дальше начиная с нового корня
			 return rt;
		 }
		 else if ((t->x < cur->xmin) && (t->y > cur->ymin))
		 {
			 Expansion(cur, new, 2, cur->xmax, cur->xmin - xabs, cur->ymax + yabs, cur->ymin);//задаёт необходимые параметры новому узлу дерева и переносит с самого низа в новый узел всю информацию
			 rt = CreateParent(rt, t);//расширяем дальше начиная с нового корня
			 return rt;
		 }
		 else if ((t->x < cur->xmin) && (t->y < cur->ymin))
		 {
			 Expansion(cur, new, 3, cur->xmax, cur->xmin - xabs, cur->ymax, cur->ymin - yabs);//задаёт необходимые параметры новому узлу дерева и переносит с самого низа в новый узел всю информацию
			 rt = CreateParent(rt, t);//расширяем дальше начиная с нового корня
		 }
	 }

	 return rt;
 }

int Expansion(Node * cur, Node *new, int branch, int xmax, int xmin, int ymax, int ymin)//задаёт необходимые параметры новому корню
{
	int i = 0, j = 0, occupancy;
	Node *res, *dee;

	/*модификация границ*/
	new->xmax = xmax;
	new->xmin = xmin;
	new->ymax = ymax;
	new->ymin = ymin;

	new->xmid = ((float)(xmax + xmin)) / 2;
	new->ymid = ((float)(ymax + ymin)) / 2;

	/*создание новых потомков и присваивание в нужный(branch) квадрат старого корня*/
	createNewChildren(new);
	free(new->branch[branch]);
	new->branch[branch] = cur;

	/*заполнение нового элемента путём подъёма из листьев информации*/
	//DeepestElement(cur,0,&dee,&i);

	while (new->occupancy < N)
	{
		res = dee = cur;//dee - искомый элемент, res- промежуточный
		while (res != NULL && res->occupancy > 0)//поиск самого нижнего элемента
		{
			res = WhereMaxDepth(dee);
			if (res != NULL && res->occupancy > 0)
				dee = res;
		}
		occupancy = new->occupancy;//заполненность найденного элемента
		for (i = dee->occupancy - 1, j = occupancy; i >= new->occupancy && dee != NULL; --i, ++j)//перенос всех элементов
		{
			new->info[j] = dee->info[i];
			dee->info[i] = NULL;

			++new->occupancy;
			--dee->occupancy;
		}

		if (dee != NULL)
		{
			DelEmptyLeaves(dee->p);//проверяем возможно ли удаление и удаляем их при жтом меняя высоту
		}
	}
	return 0;
}

/*int max(int *d)//поиск максимального в массиве из 4x элементов
{
	if (d != null)
		return -INT_MIN;

	for (int i = 0, int c = d[i]; i < 4; ++i)
	{
		if (c < d[i])
			c = d[i];
	}

	return c;
}*/

int PointBelongsToSquare(int xmax, int xmin, int ymax, int ymin, int xpnt, int ypnt)//проверка принадлежности точки xpnt ypnt площади заключённой в точках xmax ymax xmin ymin
{
	int i;

	if (xmax < xmin)//проверка на правильность исходных данных
	{
		i = xmin;
		xmin = xmax;
		xmax = i;
	}

	if (ymax < ymin)//проверка на правильность исходных данных
	{
		i = ymin;
		ymin = ymax;
		ymax = i;
	}
	if ((xpnt <= xmax) && (xpnt >= xmin) && (ypnt <= ymax) && (ypnt >= ymin))//сравнение и результат
		return 1;//yes
	else
		return 0;//no
}


int AddNode(Tree *rt, Info *t)//Добавление информации t в q-дерево
{
	Node *cur, *f, *leaf;
	Info *buf;
	int i;//BranchCh;

	if (rt->root == NULL)//if root is NULL
	{
		cur = (Node*)calloc(1, sizeof(Node));//create new root

		/*root's boorders (defined in qtree.h)*/
		cur->xmax = StartXMAX;
		cur->xmin = StartXMIN;
		cur->ymin = StartYMIN;
		cur->ymax = StartYMAX;

		/*midles*/
		cur->xmid = ((float)cur->xmax + (float)cur->xmin) / 2;
		cur->ymid = ((float)cur->ymax + (float)cur->ymin) / 2;
		rt->root = cur;
	}
	if (rt && rt->root && (t->x > rt->root->xmax || t->x < rt->root->xmin || t->y < rt->root->ymin || t->y > rt->root->ymax))//point don't belong to current square
	{
		rt = CreateParent(rt, t);//create new root
		cur = rt->root;// contin adding
	}

	if ((f = Find(rt->root, t, &leaf)) == NULL)//no elements with dublicate keys in table
	{
		cur = leaf;
		while (cur->occupancy >= N)//cur is full
		{
			if (!cur->branch[0] && !cur->branch[1] && !cur->branch[2] && !cur->branch[3])//cur is leaf
			{
				createNewChildren(cur);//create new child
			}
			cur = ChooseBranch(cur, t);//choose branch
		}
		if (cur->occupancy < N)//cur isn't full   inserted following the procedure
		{
			cur->info[cur->occupancy] = t;

			if (cur->occupancy == 0)//t has added in empty Node
				DepthMod(cur);//change value depth(Глубины)

			/**/
			/*добавляем в пустой элемент или добавляемый элемент больше всех остальных , то добавляем в конец..... P.S добавление в узел в упорядоченном виде*/
			/*if((cur->occupancy==0)||((t->x)>(cur->info[cur->occupancy-1]->x))||(((t->x)==(cur->info[cur->occupancy-1]->x))&&((t->y)>(cur->info[cur->occupancy-1]->y))))
			{
				cur->info[cur->occupancy]=t;
				if(cur->occupancy==0)//меняем значения высоты(Глубины)
					DepthMod(cur);
			}else//иначе вставляем в соблюдая упорядоченность
			{
			for(i=0;i<=(cur->occupancy);++i)//поиск подходящего места
			{
				if((t->x)<(cur->info[i]->x))//если найден нужная позиция , перемещаем уже имеющиеся и вставляем
				{
					buf=cur->info[i];
					cur->info[i]=t;
					for(++i;i<=(cur->occupancy);++i)
					{
						buf2=cur->info[i];
						cur->info[i]=buf;
						buf=buf2;
					}
				}else if ((t->x)==(cur->info[i]->x))// compare y
				{
					if((t->y)<(cur->info[i]->y))
					{
						buf=cur->info[i];
						cur->info[i]=t;
						for(++i;i<=(cur->occupancy);++i)
						{
							buf2=cur->info[i];
							cur->info[i]=buf;
							buf=buf2;
						}
					}
				}
			}}*/
			++(cur->occupancy);//occupancy +1
		}
	}
	else
		return 2;// Element add early!!!

	return 0;
}

Node *ChooseBranch(Node *x, Info *t)//выбор ветки в которую необходимо направляться
{
	if ((float)t->x <= x->xmid)
	{
		if ((float)t->y <= x->ymid)
			return x->branch[0];
		else
			return x->branch[1];
	}
	else
	{
		if ((float)t->y < x->ymid)
			return x->branch[2];
		else
			return x->branch[3];
	}
	return NULL;
}

int createNewChildren(Node *cur)// создаёт 4 потомка у элемента cur
{
	Node *new = cur->branch[0] = (Node*)calloc(1, sizeof(Node));//выделяем чистую память

	if (!new)
		perror("Calloc error!(createNewChildren)");

	/*присваивание границ*/
	new->ymax = cur->ymid;
	new->ymin = cur->ymin;
	new->xmax = cur->xmid;
	new->xmin = cur->xmin;

	new->p = cur;//задаём указатель на родительский

	new->xmid = ((float)new->xmax + (float)new->xmin) / 2;//вычисление границ
	new->ymid = ((float)new->ymax + (float)new->ymin) / 2;//вычисление границ

	/* те же самые операции для всех остальных потомков , учитывая границы*/
	new = cur->branch[1] = (Node*)calloc(1, sizeof(Node));

	if (!new)
		perror("Calloc error!(createNewChildren)");

	new->ymax = cur->ymax;
	new->ymin = (int)cur->ymid;
	new->xmax = cur->branch[0]->xmax;
	new->xmin = cur->branch[0]->xmin;

	new->p = cur;

	new->xmid = ((float)cur->branch[1]->xmax + (float)cur->branch[1]->xmin) / 2;
	new->ymid = ((float)cur->branch[1]->ymax + (float)cur->branch[1]->ymin) / 2;
	new = cur->branch[2] = (Node*)calloc(1, sizeof(Node));

	if (!new)
		perror("Calloc error!(createNewChildren)");

	new->ymax = (int)cur->ymid;
	new->ymin = cur->ymin;
	new->xmax = cur->xmax;
	new->xmin = cur->branch[0]->xmax;

	new->p = cur;

	new->xmid = ((float)cur->branch[2]->xmax + (float)cur->branch[2]->xmin) / 2;
	new->ymid = ((float)cur->branch[2]->ymax + (float)cur->branch[2]->ymin) / 2;

	new = cur->branch[3] = (Node*)calloc(1, sizeof(Node));

	if (!new)
		perror("Calloc error!(createNewChildren)");

	new->ymax = cur->ymax;
	new->ymin = (int)cur->ymid;
	new->xmax = cur->xmax;
	new->xmin = cur->branch[0]->xmax;

	new->p = cur;

	new->xmid = ((float)cur->branch[3]->xmax + (float)cur->branch[3]->xmin) / 2;
	new->ymid = ((float)cur->branch[3]->ymax + (float)cur->branch[3]->ymin) / 2;

	return 0;
}


int DepthMod(Node *cur)//увеличивает глубину дерева , cur - указатель на элемент с которого необходимо стартовать
{
	int rescomp;//результат сравнения веток родителя

	while (cur != NULL && cur->p != NULL)//пока не дошли до корня
	{
		rescomp = CompareBranches(cur, cur->p);//номер ветки(квадрата) в которой(м) находится cur
		cur = cur->p;//переходим к родительскому элементу
		cur->depth[rescomp] += 1;//увеличиваем значение глубины
	}

	return 0;
}

int CompareBranches(Node *cur, Node *parent)//выводит номер ветки parent в которой находится элемент cur
{
	for (int i = 0; i < 4; ++i)//сравниваем все ветки
	{
		if (parent->branch[i] == cur)//если совпадение => выводим результат
			return i;
	}

	return 0;
}


Info *Search(Node *root, Info *nw)//search nw in tree , if search is success - copy in nw string with data, else free nw
{
	Node *x, *leaf;//x указатель на найденный узел , leaf  -> указатель на родительский элемент
	int i;

	if ((x = Find(root, nw, &leaf)) != NULL)// результат поиска
	{
		for (i = 0; i < N; ++i)//находим в узле указатель на информацию
		{
			if (x->info[i] != NULL && x->info[i]->x == nw->x && x->info[i]->y == nw->y)
				nw->str = strdup(x->info[i]->str);//копируем строку
		}
		return nw;//возвращаем нказатель на искомый элемент
	}
	else
	{
		free(nw);//если не найден , то очищаем память и возвращаем NULL
		return NULL;
	}
}


Node *Find(Node *x, Info *t, Node **leaf)//поиск Node с элементом Info t(ключами) в узле
{
	while (x != NULL)
	{
		for (int i = 0; i < N; ++i)
		{
			if (x->info[i] != NULL && x->info[i]->x == t->x && x->info[i]->y == t->y)
				return x;
		}

		*leaf = x;//сохраняем указатель на радительский элемент(необходимо для вставки)
		x = ChooseBranch(x, t);//выбираем ветку
	}
	return x;
}


int PrintRange(Node *x, Info * n1, Info * n2)//рекурсивный вывод элементов лежащих в диапазоне ключей от n1 до n2
{
	if (x)
	{
		Info *now;
		int i = 0;
		for (i = 0; i < x->occupancy; ++i)//
		{
			now = x->info[i];
			if (n1->x <= now->x && n2->x >= now->x && n1->y <= now->y && n2->y >= now->y)//если значение лежит в заданных границах n1 и n2
				printf("x: %d;y: %d;info:%s\n", now->x, now->y, now->str);
		}

		PrintRange(x->branch[0], n1, n2);// просматриваем всё дерево
		PrintRange(x->branch[1], n1, n2);
		PrintRange(x->branch[2], n1, n2);
		PrintRange(x->branch[3], n1, n2);
		return 1;
	}

	return 0;
}


int OrderMin(Node* cur, Data* data, int min)//создание списка с элементами больше чем min но в даном дереве имеющие самые маленькие значения ключей
{
	int i = 0;
	Data* p = data, *p1, *end = data;
	{
		while (end->next != NULL)
			end = end->next;
		for (i = 0; i < N; ++i)
		{
			if (cur->info[i] && cur->info[i]->x == data->x)//если нашли элемент с таким же ключем, увеличиваем список data и записываем его туда
			{
				end->next = (Data*)malloc(sizeof(Data));
				end = end->next;
				end->x = cur->info[i]->x;
				end->y = cur->info[i]->y;
				end->string = cur->info[i]->str;
				end->next = NULL;
			}

			if (cur->info[i] && cur->info[i]->x < data->x && cur->info[i]->x > min)//если текущий элемент меньше наименьшего в данный момент и больше предыдущего самого маленького элемента(который был выведен)
			{
				data = p->next;//очищаем весь список, и формируем его заново. с новым наименьшим ключем.
				while (data != NULL)
				{
					p1 = data;
					data = data->next;
					free(p1);
				}
				p->x = cur->info[i]->x;
				p->y = cur->info[i]->y;
				p->string = cur->info[i]->str;
				p->next = NULL;
				data = p;
			}
		}
		//ищем меньшие иксы только в тех квадратах манимальные границы которых меньше текущего наименьшего икса.
		// т.к.в узлах правее искать нет смысла-там все иксы больше.
		/*
			   -25
				|
				|
	100 ----------------------
		  |       |    |                 |          например нашли наименьший икс в главном квадрате =-25 и меньшие иксы ищем только в квадратах 0 и 1.
		  |       |  1 |             2 |
	0	  -----------------------
		  |       |    |                 |
		  |       | 0 |              3 |
	-100 ----------------------
		-100    |    0          100
				   |

				выбираем в какой квардат идти и идем рекурсивно.

		*/
		if (cur->branch[0] != NULL && cur->branch[0]->xmin < data->x)
			OrderMin(cur->branch[0], p, min);
		if (cur->branch[1] != NULL && cur->branch[1]->xmin < data->x)
			OrderMin(cur->branch[1], p, min);
		if (cur->branch[2] != NULL && cur->branch[2]->xmin < data->x)
			OrderMin(cur->branch[2], p, min);
		if (cur->branch[3] != NULL && cur->branch[3]->xmin < data->x)
			OrderMin(cur->branch[3], p, min);
	}
	return 0;
}

void DelAllTree(Node *x)//удаление всего дерева
{
	if (x)
	{
		int i;
		DelAllTree(x->branch[0]);
		DelAllTree(x->branch[1]);
		DelAllTree(x->branch[2]);
		DelAllTree(x->branch[3]);

		for (i = 0; i < x->occupancy; ++i)
		{
			if (x->info[i]->str)
				free(x->info[i]->str);
			if (x->info[i])
				free(x->info[i]);
		}

		free(x);
	}
}

int delelement(Info *t, Tree * rt)//удаление элемента с ключами t и перенос на место удаляемого элемента , элемента с самого низа дерева
{
	Node *x, *cur, *res;//
	int i, j, n;

	if ((x = Find(rt->root, t, &cur)) != NULL)//поиск узла с удаляемым элементом
	{
		for (i = 0; i < N; ++i)//поиск удоляемого эл-та в узле
		{
			if (x->info[i] != NULL && x->info[i]->x == t->x && x->info[i]->y == t->y) //удаление информационного поля
			{
				free(x->info[i]->str);
				free(x->info[i]);
				n = i;//сохраняем позицию удалённого элемента
				break;
			}
		}
		res = cur = x;//res - результат выбора пути, cur - сохраняет указатель на предыдущий, x - с него начинаем поиск
		//поиск элемента находящегося в самом низу
		while (res != NULL)//
		{
			res = WhereMaxDepth(cur);//выбираем ветвь в которой глубина максимальная
			if (res != NULL)//если не вышли за пределы дерева
				cur = res;//сохроняем указатель на последний
		}

		if (cur == x)//удаляемая информация находилась в листе (x- лист)
		{
			for (j = n + 1, i = n; j < x->occupancy; ++j, ++i)
			{
				cur->info[i] = cur->info[j];//смещаем все элементы
			}
		}
		else
		{
			while (cur->occupancy == 0)
			{
				cur = cur->p;
			}
			x->info[n] = cur->info[cur->occupancy - 1];//перемещааем информацию с найденного элемента в x
		}

		cur->info[cur->occupancy - 1] = NULL;
		--cur->occupancy;// уменьшаем заполненность в глубоком элементе

		if (cur->occupancy == 0 && cur != rt->root)//если заполненность =0 и cur не равен корню
		{
			depthDown(cur);//уменьшаем глубину в дереве
			DelEmptyLeaves(cur->p);//и смотрим возможно ли удаление cur вместе с братьями
		}
		else if (cur == rt->root && cur->occupancy == 0)//если текущий элемент - корень , то удаляем его
		{
			free(cur);
			rt->root = NULL;
		}
		return 0;
	}
	return 1;
}


Node *WhereMaxDepth(Node *cur)//выбирает ветку - значение глубины в которой максимально , !!! если сur - лист  или глубина одинаковая(что в принципе одинаково) выдаст указатель на ВЕТКУ[0]
{
	int i = 0, c = 0, *d = cur->depth;//d - массив depth
	Node* res = NULL;//искомый узел

	for (i = 0; i < 4 && cur->branch[i]; ++i)
	{
		if (c < d[i])//если глубина в ветке i больше чем во всех предыдущих
		{
			c = d[i];//сохраняем максимум
			res = cur->branch[i];//сохран указатель на узел в котором глубина максимальна
		}
	}

	return res;
}

int DelEmptyLeaves(Node *parent)//проверяет пустые ли потомки элемента parent , усли да то удаляет их
{
	/*заполненность информацией каждого листа должна быть равной нулю*/
	if (parent->branch[0] != NULL && parent->branch[0]->occupancy == 0 && parent->branch[1]->occupancy == 0 && parent->branch[2]->occupancy == 0 && parent->branch[3]->occupancy == 0)
	{
		free(parent->branch[0]);
		free(parent->branch[1]);
		free(parent->branch[2]);
		free(parent->branch[3]);
		parent->branch[0] = NULL;
		parent->branch[1] = NULL;
		parent->branch[2] = NULL;
		parent->branch[3] = NULL;
		return 1;
	}
	else
		return -1;
}

int DeepestElement(Node *c, int i, Node **deepmost, int *maxd)//функция поиска самого глубокого элемента , относительно *с (не использует значения высоты)
{

	/* 	с - просматриваемый узел
		i-текущая глубина просматриваемого элемента
		deepmost -хранит указатель на самый глубокий
		maxd - значение максимальной глубины
	*/

	//текущий существует  и в нем есть элементы
	if (c != NULL && c->occupancy != 0)
	{
		int j;
		i += c->occupancy;//текущее значение глубины суммируется из заполненности
		if (*maxd < i)//если текущая глубина превышает значение глубины найденной ранее
		{
			*deepmost = c;//сохраняем указатель на текущий
			*maxd = i;//и значение высоты
		}
		for (j = 0; j < 4; ++j)//заходим во все ветки узла *c
		{
			DeepestElement(c->branch[j], i, deepmost, maxd);//вызываем для всех веток узла *c
		}
		return i;
	}
	else
	{
		return i;
	}
}

void depthDown(Node *cur)//уменьшает значение высоты дерева при уделении
{
	int rescomp;

	while (cur != NULL && cur->p != NULL)
	{
		rescomp = CompareBranches(cur, cur->p);
		cur = cur->p;
		--cur->depth[rescomp];
	}
}

/*
int PrintInOrder(Tree *rt)
{
	Info min,max;
	Node *cur=rt->root;
	int i,LenOrd=0;
	Info **Order=NULL;
	max.x=cur->xmax;
	max.y=cur->ymax;
	max.str=NULL;
	min.x=cur->xmin;
	min.y=cur->ymin;
	min.str=NULL;
	while((Order=SearchMin(Order,rt->root,&min,&max,&LenOrd))!=NULL)
	{
		for(i=0;i<LenOrd;++i)
			printf();
		min.x=Order[0]->x;
		min.y=cur->ymin;

	}
	PrintR(rt->root);
	while(x!=NULL)
	{
		for(i=0;i<x->F;++i)
		{
			min=x->info[i];

		}
	}
}


SearchMin(Node *cur,int *min,int *max)
{

}
*/
/*

Info **SearchMin(Info** Order,Node *cur,Info *min,Info *max,int *LenOrd)
{
	int i=0;
	//PointBelongsToSquare(max->x,min->x,max->y,min->y,xpnt,int ypnt);
	for(i=0;i<N;++i)
	{
		if(cur->info[i]->x==max->x)
		{
			*LenOrd+=1;
			Order=(Info*)malloc(sizeof(Info*)*LenOrd);if(!Order){perror("Malloc Error");exit(1);}
			Order[*LenOrd-1]=cur->info[i];
		}
		if((cur->info[i]->x<max->x)&&(cur->info[i]->x>min->x))
		{

		}
		if(cur->info[i]->x>max->x)
		{
			break;
		}
	}
	Node *next;
	next=ChooseBranch(cur,);

	Order=SearchMin()

	if(cur->xmid>order[0]->x)
	{
		Order=SearchMin(cur->branch[0])
	}
}

*/


/*
	nw *Order=NULL;
	unsigned int len;
	int i,j;
	len=0;
	Order=SortOrder(rt->root,Order,&len);
	j=len;///sizeof(Info*);
	for(i=0;i<j;++i)
	{
		printf("x: %d ; y: %d ; info : %s \n",Order[i]->x,Order[i]->y,Order[i]->str);
	}
	free(Order);
	return 1;
}

nw *SortOrder(Node *cur,nw *Order,unsigned int *len )
{
	if((cur)&&(cur->F!=0))
	{
		nw *NewOrd;
		int i=0,j=0,k=0,oldlen=*len;
		*len=(cur->F)+*len;
		NewOrd=(nw*)malloc((*len)*sizeof(nw));if(!NewOrd){perror("Malloc Error");exit(1);}
		for(i=0,j=0,k=0;i<(*len);++i)
		{
			if((Order!=NULL)&&(k<cur->F)&&(j<oldlen))
			{
				if((Order[j])&&((Order[j]->x)<(cur->info[k]->x)))
				{
					NewOrd[i]=Order[j];
					++j;
				}else if(((Order[j]->x)==(cur->info[k]->x)))
				{
					if((Order[j]->y)<(cur->info[k]->y))
					{
						NewOrd[i]=Order[j];
						++j;
					}else
					{
						NewOrd[i]=cur->info[k];
						++k;
					}
				}else if((Order[j]->x)>(cur->info[k]->x))
				{
					NewOrd[i]=cur->info[k];
					++k;
				}
			}else if(!(k<(cur->F)))
			{
				NewOrd[i]=Order[j];
				++j;
			}
			else if((!Order)||(j>=oldlen))
			{
				NewOrd[i]=cur->info[k];
				++k;
			}
		}
		NewOrd=SortOrder(cur->branch[0],NewOrd,len);
		NewOrd=SortOrder(cur->branch[1],NewOrd,len);
		NewOrd=SortOrder(cur->branch[2],NewOrd,len);
		NewOrd=SortOrder(cur->branch[3],NewOrd,len);
		free(Order);
		return NewOrd;
	}
	return Order;
}

/*
int Divide(Node *x)
{
	int i;
	Node *u;
	for (i = 0; i < N; ++i)
	{
		if (((float)(x->info[i]->x)) <= (x->xmid))//!!!!
		{
			if (((float)(x->info[i]->y)) <= (x->ymid))
			{
				u = x->branch[0];
				u->info[u->occupancy] = x->info[i];
				++(u->occupancy);
				x->info[i] = NULL;
				--(x->occupancy);
			}
			else
			{
				u = x->branch[1];
				u->info[u->occupancy] = x->info[i];
				++(u->occupancy);
				x->info[i] = NULL;
				--(x->occupancy);
			}
		}
		else if (((float)(x->info[i]->y)) < (x->ymid))
		{
			u = x->branch[2];
			u->info[u->occupancy] = x->info[i];
			++(u->occupancy);
			x->info[i] = NULL;
			--(x->occupancy);
		}
		else
			x = x->branch[3];
		u = x->branch[3];
		u->info[u->occupancy] = x->info[i];
		++(u->occupancy);
		x->info[i] = NULL;
		--(x->occupancy);
	}
	return 0;
}

*/