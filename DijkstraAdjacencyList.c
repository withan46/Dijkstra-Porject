#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

/*
O struct node perigrafei tous kombois ths listas geitniashs .
To pedio v einai h korufh / geitonas tou u.
To pedio weight einai to baros ths akmhs (u , v) .
To next einai o deikths ston epomeno pinaka .
O struct node xrhsimopoieitai sthn main ws ejhs: struct node *graph[i];
*/
struct node {
	int v , weight ;
	struct node *next;
};

// line 77
// ylopoiei ton algori8mo dijkstra gia ton grafo "graph" pou einai lista geitnhashs
// me arxikh korufh thn "start" 
void dijkstraUsingAdjacencyList(int start, int N, struct node *graph[N]);

// line 223
// dhmiourgia listas geitniashs "graph" apo ta shmeia tou excel
void createAdjacencyListGraph(int N, struct node *graph[N]);

// line 199
// afairei thn koryfh u tou Q (S[u]=0) me thn mikroterh apostash apo to start
// kai thn pros8etei sto S (S=S + {u}) 
int deleteMin(int N, int S[N], int d[N]);

// line 152
// tupwnei to suntomotero monopati apo to start sto end
void printPath(int start , int end , int N, int P[N], int d[N]);

// line 175
// h connect dhmiourgei thn akmh start -> end me baros weight kai thn pros8etei
// sthn lista graph[start]
void connect(int start , int end , int weight , int N , struct node *graph[N]);

// line 265
// gemizei ton pinaka points me ta shmeia apo to excel
void fillPointsArray(int N, float points[N][2]);


int main(int argc, char const *argv[])
{
	//arxikopoiei thn genhtria tvn tuxaiwn ari8mvn
	srand(time(NULL));
	
	int i, start;
	FILE *f = fopen("xronoi.txt", "w");
	
	for(i=10;i<=50;i=i+10)
	{
		
		clock_t  start1 = clock();
		
		struct node *graph[i];
		
		createAdjacencyListGraph(i, graph);

		for(start=0;start<i;start++)
			dijkstraUsingAdjacencyList(start, i, graph);
		
		clock_t end1 = clock();
	
		long milliseconds = (end1 - start1) ;
		
		fprintf(f, "\nadjacency List time for %d vertices = %ld milliseconds\n", i, milliseconds );
		
		printf("\nadjacency List time for %d vertices = %ld milliseconds\n", i, milliseconds );
	}
	fclose(f);
	return 0;
}

// ylopoiei ton algori8mo dijkstra gia ton grafo "graph" pou einai lista geitnhashs
// me arxikh korufh thn "start" 
void dijkstraUsingAdjacencyList(int start, int N, struct node *graph[N])
{
	int  i;

	//to d[i] einai h apostash ths koryfhs i apo thn arxikh (s)
	int d[N];
	
	//O pinakas P xrhsimopoieitai gia na briskoyme ta elaxista monopatia apo to start se ka8e koryfh 
	//  p.x. gia thn koryfh u to syntomotero monopati einai 
	//	start -> … P[ P[u] ] -> P[u] -> u
	int P[N];
	
	// O pinakas S xrhsimopoieitai gia na perigracei ta synola S kai Q tou dijkstra
	// Q = { i , S[i] == 0 }
	// S = { i , S[i] == 1 }
	int S[N];

	for (i = 0; i < N ; i++) {
		d[i] = INT_MAX;
		P[i] = -1;
		S[i] = 0;
	}

	int n = 0;

	// s=start
	d[start] = 0;

	while (n < N) // Q!={}
	{
		// u = min(Q)
		// meta u: Q -> S
		int u = deleteMin(N, S, d);

		if (u < 0)
			break;

		n++;

		struct node * neighbour = graph[u];
		//
		// gia ka8e geitona v tou u
		//
		while (neighbour != NULL) {
			int v = neighbour->v;
			int w = neighbour->weight; // w = w(u, v)

			if (d[v] > d[u] + w) {
				d[v] = d[u] + w;
				P[v] = u;
			}
			neighbour = neighbour->next;
		}
	}

	//emfanizv tis syntomoteres apostaseis twn koryfwn apo to start
	
	printf("\n\nstart node: %d\n", start);
	for (i = 0; i < N ; i++)
	{
		printf("d[%d]=%d\n ", i, d[i]);
	}
	printf("\n");
	
	//4o paradoteo 
	if(N==10)
	{
		for(i=start+1;i<N;i++)
		{
			printPath(start , i , N , P,d);
		}
	}
	
}

//h printPath emfanizei to syntomotero monopati apo to start sto end
void printPath(int start , int end , int N, int P[N], int d[N])
{
	int i , j;
	int f[N];
	f[0]=end;
	
	printf("\nmonopati %d->%d (mhkos: %d): ", start, end, d[end]);
	for(i=end, j=1 ; i!=start ; i=P[i], j++)
	{
		f[j]=P[i];
	}
	for(i=j-1; i>=0; i--)
	{
		printf("%d",f[i]);
		if(i > 0)
			printf(" -> ");
	}
	
}
/*
	h connect dhmiourgei thn akmh start -> end me baros weight kai thn pros8etei
	sth lista graph[start]
*/
void connect(int start , int end , int weight , int N , struct node *graph[N])
{
	struct node *edge = malloc(sizeof(struct node));

	edge->v = end;
	edge->weight = weight;
	edge->next = NULL;

	if (graph[start] == NULL)
		graph[start] = edge;
	else
	{
		edge->next = graph[start];
		graph[start] = edge;
	}
}

/*
	Briskei th mikroterh timh d[minPos]
	gia ta stoixeia pou isxyei S[i] == 0 (Q)
    kai epistrefei to minPos
	An gia ola ta stoixeia S[i] == 1 tote epistrefei -1
	An to minpos >= 0 tote metaferei thn koryfh minpos apo to Q -> S
*/
int deleteMin(int size, int S[size], int d[size])
{
	int i , min , minpos;

	minpos = -1;
	for (i = 0; i < size; i++)
	{
		if (S[i] == 0)
		{
			if (d[i] < min || minpos == -1)
			{
				min = d[i];
				minpos = i;
			}
		}
	}
	if (minpos >= 0)
		S[minpos] = 1; // minPos: Q -> S

	return minpos;

}

// dhmiourgia listas geitniashs "graph" apo ta shmeia tou excel
void createAdjacencyListGraph(int N, struct node *graph[N]) {
	float nodeArray[N][2];
	//gemizei to nodeArray me ta shmeia apo to excel plh8ous N
	fillPointsArray(N, nodeArray);
	int radius;
	if (N == 10)
		radius = 25;
	else if (N == 20)
		radius = 20;
	else if (N == 30)
		radius = 30;
	else if (N == 40)
		radius = 25;
	else if (N == 50)
		radius = 20;
		
	int i , j;
	
	// Arxikopoihsh tou grafou
	for (i = 0; i < N; i++)
		graph[i] = NULL;
		
	//dhmiourgia tou grafou(adjacency list ) me bash ta shmeia tou excel kai  thn aktina radius
	for (j = 0; j < N; j++)
	{
		for (i = j + 1; i < N; i++)
		{
			if (( (nodeArray[i][0] - nodeArray[j][0]) * (nodeArray[i][0] - nodeArray[j][0]) ) + 
				( (nodeArray[i][1] - nodeArray[j][1]) * (nodeArray[i][1] - nodeArray[j][1])) < 
				radius * radius)
			{
				int weight = rand() % 100 + 1;
				// i -> (weight) -> j
				connect(i , j , weight , N , graph);
				// j -> (weight) -> i
				connect(j , i , weight , N , graph);
			}
		}
	}
}

// gemizei ton pinaka points me ta shmeia apo to excel
void fillPointsArray(int N, float points[N][2])  {
	int i;
	if (N == 10)
	{
		float nodeArray[10][2] = {
			{23.258710646132,  35.693792554939},
			{15.6919665166512,	5.44971720778571},
			{46.9368664040456,	29.0435454700767},
			{46.2284516570725,	33.011184648756},
			{8.06540313613383,	45.7786701426429},
			{20.6012365234468,	11.4199747200291},
			{11.0331537145085,	20.8673842200159},
			{10.3565762170931,	25.054904381848},
			{15.7948657560649,	8.55448099309915},
			{29.6484291891324,	32.5830126733596}
		};
		for(i=0;i<N;i++)
		{
			points[i][0] = nodeArray[i][0];
			points[i][1] = nodeArray[i][1];
		}
	}
	else if (N == 20)
	{
		float nodeArray[20][2] = {
			{19.4782000291965,	13.0426057596613},
			{31.6883715566965,	20.6529906790755},
			{9.96934687471611,	12.0584107458776},
			{29.0209844491098,	49.1917151049223},
			{14.8317977393665,	15.5903073838503},
			{36.0809918668299,	0.70407879652254},
			{37.8188100098651,	41.6279361049553},
			{2.38796972618831,	8.43766727439773},
			{48.2932875001688,	43.9697992657406},
			{31.7792702155835,	23.2439022801048},
			{8.78260244884041,	27.0509998554966},
			{32.0408026787308,	8.40737553648451},
			{25.709890262236,	46.2937500640225},
			{44.2884789402208,	36.6904985180961},
			{40.2889075301339,	3.88488043872125},
			{40.4813875945209,	0.550916553500227},
			{32.361223281229,	20.9130036488649},
			{39.6051322216627,	45.3857904314089},
			{30.153511612035,	4.64927165504574},
			{31.9875005748663,	32.9073598178429}

		};
		for(i=0;i<N;i++)
		{
			points[i][0] = nodeArray[i][0];
			points[i][1] = nodeArray[i][1];
		}
	}
	else if (N == 30)
	{
		float nodeArray[30][2] = {
			{71.6125547988082,	3.8242032975036},
			{83.1152691261193,	15.9722607806532},
			{82.9322898120652,	34.1779942967061},
			{15.8184326723634,	97.0741828043056},
			{81.2415369467325,	42.717414366535},
			{0.320365905822875,	73.217835747446},
			{46.3471385444976,	18.7367839086252},
			{13.2848253402699,	83.2126084776223},
			{10.3087804770113,	98.431244324488},
			{58.9746209057563,	59.8079194016843},
			{93.2714077451964,	46.5604496683703},
			{9.51674655271673,	13.1103811847668},
			{76.5143383476695,	0.368718136384443},
			{10.3473607902186,	4.82308258541998},
			{37.881934159848,	66.0774544758543},
			{3.97600001637971,	49.6073452124367},
			{22.5317718338319,	45.9556309685642},
			{43.3544926513598,	15.080023750512},
			{28.5478618471117,	4.6286237322507},
			{88.9191901497671,	0.274452260282659},
			{41.0947824364926,	39.5597376702768},
			{6.25408016125357,	51.7361659881501},
			{89.8468517546298,	54.8669699653356},
			{89.8158604094888,	20.3949232090259},
			{50.5524260093231,	69.3849523334775},
			{92.123299033974,	91.6679079377077},
			{5.61407823697861,	46.0820930611327},
			{20.2631026242226,	33.2543138673933},
			{32.2699095183868,	40.2123083263866},
			{90.0681433692914,	80.3961715308591}
		};
		for(i=0;i<N;i++)
		{
			points[i][0] = nodeArray[i][0];
			points[i][1] = nodeArray[i][1];
		}
	}
	else if (N == 40)
	{
		float nodeArray[40][2] = {
			{33.3688509595257,	12.357936929311},
			{55.0607283022388,	52.466782588678},
			{17.3204210738773,	79.5397092791039},
			{2.68375438933118,	31.4575964309491},
			{89.0208171247178,	79.0039333998895},
			{73.8169823896908,	59.6229633783574},
			{96.8924463216691,	43.0900329973109},
			{9.37565654789531,	76.1990260234724},
			{89.358977478835,	40.9217078521009},
			{27.6296413612071,	88.8642131011973},
			{30.3778113838799,	40.8387057676544},
			{51.544532777439,	34.8391389558667},
			{44.4483833891106,	49.0607664021513},
			{78.916943682833,	25.8683058980146},
			{92.4832105313053,	77.7315882480214},
			{95.7503743150434,	55.4193583953723},
			{63.0311079874799,	27.5130782614979},
			{61.941846750006,	18.9375784098825},
			{75.6684082562138,	37.3913158866908},
			{90.4110433176371,	41.8158818747257},
			{95.3256646317101,	10.9161867584256},
			{57.2154326348974,	69.112721397504},
			{30.7872527431084,	84.1543869143316},
			{6.74570412487007,	12.5475169522863},
			{43.7552163764709,	79.1903919933029},
			{71.1604138041817,	3.09483290950172},
			{36.8532344561649,	70.1288345812546},
			{17.0089995637427,	99.9102647553561},
			{57.555226048122,	83.5480188970718},
			{43.7244324416202,	17.6742079868029},
			{42.3272133630867,	1.30129982676569},
			{3.02060553127484,	79.2696950033529},
			{19.7759061813694,	46.2208947041851},
			{17.3425567007229,	2.47126063425567},
			{49.3529703430325,	33.5896379829291},
			{29.163892292462,	50.2399724342612},
			{13.1388778137008,	1.39685082822297},
			{26.4664394684039,	68.5535146788214},
			{48.9935016254676,	89.0294439114744},
			{79.4059269526821,	27.0498062335956}
		};
		for(i=0;i<N;i++)
		{
			points[i][0] = nodeArray[i][0];
			points[i][1] = nodeArray[i][1];
		}
	}
	else if (N == 50)
	{
		float nodeArray[50][2] = {
			{28.2593497279357,	38.4225565823674},
			{37.8297035441908,	78.4836646063071},
			{22.980026489444,	22.2712909839848},
			{38.4752927415898,	6.74473427628444},
			{23.9691097576463,	9.57194120928986},
			{52.007003498521,	35.1304925348981},
			{85.2693146281953,	7.19643416404977},
			{87.3545846619016,	92.3599687899328},
			{64.5392080156558,	76.8090187008438},
			{61.748806513827,	74.567543908218},
			{77.6408088111575,	87.2361518561816},
			{96.2963505402947,	56.1575772013008},
			{80.4153066096243,	94.8137144212303},
			{24.405204101946,	90.0593928574095},
			{28.6115520166409,	0.458265139446656},
			{94.5482559174171,	56.9329288404103},
			{85.06013635694,	26.0367044092159},
			{69.4690410173149,	18.9922542306232},
			{22.3535628168612,	64.7533591949236},
			{52.0839770547702,	83.6519504674966},
			{49.813958263173,	50.5778211216188},
			{49.1069551264503,	62.1023565994245},
			{65.8918030070022,	17.4305281133872},
			{71.3784253648472,	55.018165068832},
			{91.1702220988929,	89.7977211232876},
			{72.9461388399786,	2.22159495398691},
			{15.9066247816031,	29.3520998159372},
			{14.23103090318,	61.1412629720897},
			{36.9957251963107,	80.3393715912183},
			{72.3561341801217,	84.8900639120475},
			{93.7905970539879,	69.318750052336},
			{29.6032861920267,	97.8345128602944},
			{43.319392303585,	82.1492341527806},
			{67.9655505024573,	87.4482839999288},
			{85.3736499509179,	43.8078872266942},
			{34.6831342139894,	97.9619737133895},
			{8.82911700776903,	47.3268744679079},
			{16.8092947940441,	19.597670369682},
			{18.1938336204912,	30.6380201144784},
			{95.174029446805,	94.5827783903861},
			{13.5854134644628,	94.1537446044179},
			{33.7556347616594,	62.1882086709358},
			{32.6909711660397,	63.7933376058331},
			{82.4792684512831,	8.58909401306265},
			{79.8885149498657,	34.4920981905896},
			{91.6602895704264,	83.056122591233},
			{19.1443402893813,	77.5896396478674},
			{1.30230382123108,	68.8605060051405},
			{75.9351676871549,	41.8110391820709},
			{77.0986216098902,	1.64984034961611}
		};
		for(i=0;i<N;i++)
		{
			points[i][0] = nodeArray[i][0];
			points[i][1] = nodeArray[i][1];
		}
	}
}

