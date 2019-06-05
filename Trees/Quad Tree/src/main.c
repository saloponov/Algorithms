#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "qtree.h"
#include <asm/msr.h> //(заголовок для x86, означающий machine-specific registers )
#include <sys/time.h>

#define DEBUG 0

typedef char *String;

String Menu[]={
		"1. Ввод",
		"2. Упорядоченный вывод",
		"3. Поиск элемента по заданной паре ключей",
		"4. Вывод на экран в виде дерева",
		"5. Вывод в диапазоне ключей",
		"6. Открыть файл",
		"7. Удаление дерева",
		"8. Удаление элемента.",
		"9. Timing"
};

//Диалоговые сообщения(Глобальные)
String DlgMsg[] =
{
		"No elements in table",
		"Please enter key of element:",
		"What do you want to do?:",
		"Write new info field(string):"
};

String Errors[]={"OK",
		"Key not found!",
		"element was added in table early!",
		"Not found element in table!",
};

const int num=sizeof(Menu)/sizeof(Menu[0]);
FILE * time_file,*Tadd,*Tsearch,*Tdel;
String file="time100.txt";
unsigned long long t_time[4];


int 	DAdd(Tree * );
int 	Answer(char *Menu[],int);
char	*enterString(FILE *stream);
int 	DSearch(Tree *);
int		DebugPrint(Tree *);
void 	PrintTree(Node* cur,int l);
int 	addFromFile(Tree *rt, char *fileName);
int 	OpenFile(Tree *rt);
FILE *OpenFileProf(char s[100],FILE * f);
int		RangePrintTree(Tree *);
int 	PrintRange(Node*,Info*,Info*);
int		Del(Tree*);
int 	outs(Tree*);
int 	OrderMin(Node* cur,Data* data,int min);
int 	DelElem(Tree *rt);
int		TimingON(Tree *rt);
long RandomLong(long vMIn, long vMax);
unsigned long long rdtsc( void );
int outsWithoutPrint(Tree *rt);

int (*Func[])(Tree *)={0,DAdd,outs,DSearch,DebugPrint,RangePrintTree,OpenFile,Del,DelElem,TimingON};

int Answer(char* alt[],int n )
{
	int answer=0,i;
	printf("%s\n",DlgMsg[2]);
	for(i=0;i<n;++i)
		printf("%s \n",alt[i]);
	char *prompt = "Choice";
	do
	{
		printf("%s\n",prompt);
		prompt="You are wrong. Please repeat:";
		scanf("%d",&answer);
	}while ((answer<0) ||(answer>=n+1));
	return answer;
}
long RandomLong(long vMIn, long vMax)
{
	srand(time(NULL));
    return (rand() % (vMax - vMIn)) + vMIn;
}

int main()
{
	Tree rt;//элемент с указателем на корень
	rt.root=NULL;
	int ind,err;
	while((ind=Answer(Menu,num)))
	{
		err=Func[ind](&rt);
		printf("%s\n",Errors[err]);
	}
	printf("Good bye!\n");
	return 0;
}

void EnableTiming()
{
	if(!(time_file=fopen(file,"r+")))
	{
		fprintf(stderr,"can't open file %s\n",file);
		if(!(time_file=fopen(file,"w+")))
		{
			perror("Can't create file!");
			exit(1);
		}
		fprintf(stderr,"can't open file %s\n",file);
	}
}

unsigned long long rdtsc( void )
{

	   unsigned long long int x;
	   x=clock();
	   //asm volatile ( "rdtsc" : "=A" (x) );
	   return x;
}


int TimingON(Tree *rt)
{
	FILE *in,*logAdd,*logFind,*logDel;
	const int SHIFT=10000,MAX=5000000,REPEAT=10;
	int currentAttemptNumber;
	int i,j,res=0;
	unsigned long long TimeAddUp,TimeFind,TimeOrderPrint;
	char buf[200];
	long seconds,TimeAddAllusec,TimeAddAllms,TimeDel;
	//открытие лог файлов
	logAdd=OpenFileProf("timing/time_add.csv",logAdd);
	logFind=OpenFileProf("timing/time_search_and_del.csv",logFind);
	logDel=OpenFileProf("timing/time_del.csv",logDel);
	sprintf(buf,"data.txt");
	printf("%s\n",buf);
	in=fopen(buf,"r");
	fprintf(time_file,"numbers of elements; add time;\n");
//	fprintf(time_file,"%d;",j+1);
	srand(time(NULL));

	struct itimerval value1={{0,0},{1000,0}},value2;
	setitimer(ITIMER_PROF,&value1,NULL);

	for(currentAttemptNumber=1;currentAttemptNumber<REPEAT;++currentAttemptNumber)//Выполнить цикл столько раз, сколь попыток профилирования (таймирования) на сделать
	{
		rewind(in);
		printf("Opened. Adding items:\n"); //Вывести сообщение о том, что все файлы открыты, начинается добавление элементов
		i=SHIFT; //Установить счётчик количества элементов в части, которое ещё надо добавить в начально состояние (т. е. количеству элементов в части)
		 //Обнулить счётчик количества добавленых частей
		//t_time[0]=rdtsc();//Зачесь время начала добавления части
		int t,k;
		Info **RealKeys=NULL;
		RealKeys=(Info**)realloc(RealKeys,MAX*sizeof(Info*));
		for(t=0;t<MAX;++t)//считываем в память базу с ключами и строками
		{
			RealKeys[t]=(Info*)malloc(sizeof(Info));
			fscanf(in, "%d %d", &(RealKeys[t]->x), &(RealKeys[t]->y));
			fscanf(in,"%*[^\n]");
			fscanf(in, "%*[\n]");
			RealKeys[t]->str=enterString(in); //Вводим из файла с данными строку
			//printf("(%d,%d)\n",RealKeys[t]->x,RealKeys[t]->y);
		}
		for(res=10000,k=0;res<MAX;res+=10000,++k)
		{
			int res2=res+SHIFT;
			printf("%d\n",res);
			setitimer(ITIMER_PROF,&value1,NULL);
			for(t=res-10000;t<res;++t)
			{
				AddNode(rt, RealKeys[t]); //Пытаемся добавить элем
			}
			getitimer(ITIMER_PROF,&value2);
			TimeAddAllusec=999999-value2.it_value.tv_usec;//value1.it_value.tv_usec-value2.it_value.tv_usec;
			//TimeAddAll+=rdtsc()-t_time[0];
			if((TimeAddAllms=value1.it_value.tv_sec-value2.it_value.tv_sec)>1)
							TimeAddAllusec=TimeAddAllusec+(TimeAddAllms-1)*1000000;
			//TimeAddAllms=999999-value2.it_value.tv_sec;
			//if(value1.it_value)
			fprintf(logAdd, "%d;%d;%ld;",currentAttemptNumber,res,TimeAddAllusec); //Дописываем в журнал добавления время, за которое добавили эту часть элементов
			fflush(logAdd);
			if(res<=30000)
			{
				setitimer(ITIMER_PROF,&value1,NULL);
				outsWithoutPrint(rt);
				getitimer(ITIMER_PROF,&value2);
				TimeAddAllusec+=999999-value2.it_value.tv_usec;
				TimeAddAllms=value1.it_value.tv_sec-value2.it_value.tv_sec;
				TimeAddAllusec=TimeAddAllusec+(TimeAddAllms-1)*1000000;
				fprintf(logAdd,"%ld;\n",TimeAddAllusec);
				fflush(logAdd);
			}else
				fprintf(logAdd,"\n");
			TimeFind=0;
			setitimer(ITIMER_PROF,&value1,NULL);
			for(j=0;j<(SHIFT);++j) //таймирование неудачного поиска 10000 элементов
			{
				Info *nw;
				nw=(Info*)calloc(1,sizeof(Info));
				nw->x=RandomLong(rt->root->xmin,rt->root->xmax);
				nw->y=RandomLong(rt->root->ymin,rt->root->ymax);
				nw->str=NULL;
				nw = Search(rt->root,nw);
				if(nw!=NULL){//время успешного поиска
					//t_time[2] += t_time[1];
					free(nw->str);
					free(nw);
				}
			}
			getitimer(ITIMER_PROF,&value2);
			TimeAddAllusec=1000000-value2.it_value.tv_usec;
			TimeAddAllms=value1.it_value.tv_sec-value2.it_value.tv_sec;
			if((TimeAddAllms=value1.it_value.tv_sec-value2.it_value.tv_sec)>1)
						TimeAddAllusec=TimeAddAllusec+(TimeAddAllms-1)*1000000;
			fprintf(logFind,"%d;%ld;\n",res,TimeAddAllusec);//Дописываем в журнал поиска время, за которое заданное количество раз искали элементы
			fflush(logFind); //Для быстрой отладки: сразу же сбрасываем буфер в файл
		}
//завершили добавлени
	//таймирование удаления
		int notsuccdel=0;
		int *RandKeys;
		RandKeys=(int*)calloc(MAX,sizeof(int));
		for(j=0;j<(MAX);++j)//генерируем случайные ключи
		{
			RandKeys[j]=j;
		}
		for(j=0;j<MAX;++j)
		{
			int p,randome;
			randome=RandomLong(0,MAX);
			p=RandKeys[j];
			RandKeys[j]=RandKeys[randome];
			RandKeys[randome]=p;
		}
		i=SHIFT;//
		Data *p,*unsearch=NULL;
		setitimer(ITIMER_PROF,&value1,NULL);		
		for(j=0;j<MAX;++j,--i)//таймирование удачного удаления для всех элементов
		{
			int result=0;
			Info *D;
			D=(Info*)calloc(1,sizeof(Info));
			D->x=RealKeys[RandKeys[j]]->x;
			D->y=RealKeys[RandKeys[j]]->y;
			result=delelement(D,rt);
			if(result!=0)//если неуспешно
			{
				++notsuccdel;
#if DEBUG
				unsearch=(Data*)calloc(1,sizeof(Data));
				unsearch->next=p;
				unsearch->x=D->x;
				unsearch->y=D->y;
				p=unsearch;
#endif
			}
			if (i<=0)
			{
				getitimer(ITIMER_PROF,&value2);
				TimeDel=999999-value2.it_value.tv_usec;
				TimeAddAllms=value1.it_value.tv_sec-value2.it_value.tv_sec;
				TimeDel=TimeDel+(TimeAddAllms-1)*1000000;
				printf("%d\n",notsuccdel);
				fprintf(logDel,"%d;%ld;\n",MAX-j,TimeDel);//Дописываем в журнал поиска время, за которое заданное количество раз искали элементы
				fflush(logDel); //Для быстрой отладки: сразу же сбрасываем буфер в файл
				i=SHIFT;				
				setitimer(ITIMER_PROF,&value1,NULL);
			}
			free(D);
		}
		getitimer(ITIMER_PROF,&value2);
		TimeDel=999999-value2.it_value.tv_usec;
		TimeAddAllms=value1.it_value.tv_sec-value2.it_value.tv_sec;
		TimeDel=TimeDel+(TimeAddAllms-1)*1000000;
		printf("%d\n",notsuccdel);
#if DEBUG
		while(unsearch!=NULL)
		{
			printf("(%d,%d)\n",unsearch->x,unsearch->y);
			p=unsearch;
			unsearch=unsearch->next;
			free(p);
		}
		printf("\n");
#endif
		fprintf(logDel,"%d;%ld;\n",MAX-j,TimeDel);//Дописываем в журнал поиска время, за которое заданное количество раз искали элементы
		fflush(logDel); //Для быстрой отладки: сразу же сбрасываем буфер в файл
	}
	Del(rt);
	fclose(in);
	fclose(time_file);
	fclose(Tsearch);
	fclose(Tadd);
	fclose(Tdel);
	return 0;
}

FILE *OpenFileProf(char s[100],FILE * f)
{

	if(!(f=fopen(s,"r+")))
	{
		if(!(f=fopen(s,"w+")))
		{
			perror("Can't create file!");
			exit(1);
		}
	}
	fseek(f,0,SEEK_END);
	return f;
}

int DAdd(Tree *rt)//dialog
{
	Info *nw;
	int result;
	nw=(Info*)malloc(sizeof(Info));if(!nw){perror("Malloc Error!");exit(1);}
	printf("%s\n",DlgMsg[1]);
	scanf("%d%d",&(nw->x),&(nw->y));
	getchar();
	printf("%s",DlgMsg[3]);
	//enter string
	nw->str=enterString(stdin);
	result=AddNode(rt,nw);
	//fprintf(time_file,"%ld\n",t_time[2]);
	return result;
}


int DSearch(Tree *rt)
{
	Info *nw;
	nw=(Info*)malloc(sizeof(Info));
	printf("%s\n",DlgMsg[1]);
	scanf("%d%d",&(nw->x),&(nw->y));
	getchar();
	nw->str=NULL;
	nw=Search(rt->root,nw);
	fprintf(stdout,"%llu\n",t_time[2]);
	if(nw!=NULL)
	{
		printf("key x : %d ; key y: %d ; string : %s\n",nw->x,nw->y,nw->str);\
		free(nw->str);
		free(nw);
		return  1;
	}else
	{
		return 0;
	}
}

char* enterString(FILE *stream)
{
	char *str;
	char buf[64];
	int size=1, n;
	buf[63]='\0';
	str=(char*)malloc(size);
	*str='\0';
	if(!str) { perror("ERROR! Malloc error!\n"); exit(1); }
	fscanf(stream, "%*1[\n]");
	while((n=fscanf(stream, "%63[^\n]", buf))==1)
	{
		size+=strlen(buf);
		str=(char*)realloc(str,size);
		if(!str) { perror("ERROR! Realloc error!\n"); exit(1); }
		strcat(str, buf);
	}
	if(n==EOF && size==1) return NULL;
	fscanf(stream, "%*1[\n]");
	return str;
}


int DebugPrint(Tree *rt)//вывод виде дерева
{
	PrintTree(rt->root,0);
	return 0;
}


void PrintTree(Node* cur,int l)//выводв виде дерева
{
	int i=l;
	if(cur)
	{
		while(--i>=0)
			printf("...");
		for(i=0;i<cur->occupancy;++i)
		{
			printf("(%d,%d)",cur->info[i]->x,cur->info[i]->y);
		}
#if DEBUG
		printf("depth");
		for(i=0;i<4;++i)
			printf("%d",cur->depth[i]);
		printf("xmax %d, xmin %d,ymax %d ymin %d",cur->xmax,cur->xmin,cur->ymax,cur->ymin);
#endif
		printf("\n");
		++l;
		for(i=0;i<4;++i)
		{
			PrintTree(cur->branch[i],l);
		}
	}
}

//вывод дерева в диапазоне ключей
int RangePrintTree(Tree *rt)
{
	Info *n1,*n2,*t;

	n1=(Info*)malloc(sizeof(Info));
	n2=(Info*)malloc(sizeof(Info));
	printf("Enter first border:\nx:");
	scanf("%d",&(n1->x));
	printf("\ny:");
	scanf("%d",&(n1->y));
	printf("\nEnter second border:\nx:");
	scanf("%d",&(n2->x));
	printf("\ny:\n");
	scanf("%d",&(n2->y));
	n1->str=n2->str=NULL;
	if(((n1->x)>(n2->x))||((n1->x)==(n2->x)&&((n1->y)>(n2->y))))
	{
		t=n1;
		n1=n2;
		n2=t;
	}
	PrintRange(rt->root,n1,n2);
	return 1;
}


int OpenFile(Tree *rt)//открытие файла с ключами и строками
{
	char *fileName;
	printf("enterFileName\n");
	fileName=enterString(stdin);
	while(addFromFile(rt, fileName)==1)
	{
		free(fileName);
		printf("enterCorrectFileName");
		fileName=enterString(stdin);
	}
	free(fileName);
	return 0;
}

int addFromFile(Tree *rt, char *fileName)//добавление данных из файла
{
	FILE *file;//файл
	Info *new;//добавляемый элемент
	file=fopen(fileName, "r");
	if(!file)
		return 1;
	//считывыем ключи
	while(new=(Info*)malloc(sizeof(Info)),fscanf(file, "%d %d", &(new->x), &(new->y))==2)
	{
		fscanf(file, "%*[^\n]");//считывыем строку
		fscanf(file, "%*1[\n]");
		new->str=enterString(file);
		AddNode(rt, new);	//добавляем
	}
	if(!fclose(file))
		return 2;
	return 0;
}

int outsWithoutPrint(Tree *rt)//функция вывода дерева с первого ключа в порядке возрастания без вывода на экран
{
	Node* cur=rt->root;
	Data *p0,*p1;
	Data* data;
	int min;
	min=cur->xmin-1;
	data=(Data*)malloc(sizeof(Data));
	data->next=NULL;
	data->x=cur->xmax;
	data->y=0;
	data->string=NULL;t_time[0]=rdtsc();
	while(data->x<cur->xmax+1)//устанавливаем текущий икс в значение, которое не может быть в дереве- все элементы в дереве меньше.
		 //функция поиска следующего элемента.
	{
		data->x=cur->xmax+1;
		OrderMin(cur,data,min);
		if(data->x<=cur->xmax)
		{
			p0=data;
			p0=data->next;
			data->y=0;
			data->string=NULL;
			while(p0!=NULL)//jxbcnrf cgbcrf
			{
				p1=p0;
				p0=p0->next;
				free(p1);
			}
			data->next=NULL;
			min=data->x;//pfgjvbyftv vnybvev
			//запоминаем элемент который вывели ( для поиска элемента меньшего текущего наименьшего и больше предыдущего наименьшего в функции orderly output
			//sav и есть это предыдущее значение.
		}
	}
	return 0;
}


int outs(Tree *rt)
{
		Node* cur=rt->root;
		Data *p0,*p1;
		Data* data;
		int min;
		if(cur!=NULL)
		{
		min=cur->xmin-1;
		data=(Data*)malloc(sizeof(Data));
		data->next=NULL;
		data->x=cur->xmax;
		data->y=0;
		data->string=NULL;
		while(data->x<cur->xmax+1)//устанавливаем текущий икс в значение, которое не может быть в дереве- все элементы в дереве меньше.
			 //функция поиска следующего элемента.
		{
			data->x=cur->xmax+1;
			OrderMin(cur,data,min);
			if(data->x<=cur->xmax)//выводим все элементы
			{
				p0=data;
				while(p0!=NULL)
				{
					printf("\n\n (%10d;%10d) string: %s",p0->x,p0->y,p0->string);
					p0=p0->next;
				}
				p0=data->next;
				data->y=0;
				data->string=NULL;
				while(p0!=NULL)//jxbcnrf cgbcrf
				{
					p1=p0;
					p0=p0->next;
					free(p1);
				}
				data->next=NULL;
				min=data->x;//pfgjvbyftv vnybvev
				//запоминаем элемент который вывели ( для поиска элемента меньшего текущего наименьшего и больше предыдущего наименьшего в функции orderly output
												//sav и есть это предыдущее значение.
			}
		}
		return 0;
		}
	return 1;
}


int Del(Tree *rt)//удаление всего дерева
{
	DelAllTree(rt->root);
	rt->root=NULL;
	return 0;
}

int DelElem(Tree *rt)//удаление одного элемента
{
	Info D;
	int i;
	printf("%s\n",DlgMsg[1]);
	scanf("%d%d",&(D.x),&(D.y));
	i=delelement(&D,rt);
	return i;
}
