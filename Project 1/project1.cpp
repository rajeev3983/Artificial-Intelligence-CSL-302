/**
* This script searches the solution for an 8-puzzle problem through AI search techniques
* The blank tile is represented by the tile with the number 9
* The current search technique is a naive breadth first search
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <map>
#include <ctime>


#define N 3         // NxN puzzle
#define MAXVALIDMOVES 4  // maximum number valid moves (4 for the center tile)
//#define SOLDEPTH 9   // actual depth of the solution
int SOLDEPTH;
using namespace std;
typedef struct      // coordinates of a single tile
{
    int i;
    int j;
} Location;

struct Node             // node of the search space
{
    int **layout;       // tile configuration
    float g_val;          // cost to reach this node
    float h_val;
    float f_val;
    int move;           // move from the parent to reach this node
    int depth;
    struct Node *parent;// pointer to the parent. We will use this for tracing back
};

struct heapNode{
	Node *nodeptr;
	float value;
};

// search queue elements
struct SearchQueueElement
{
    struct Node *nodeptr;               // pointer to the state in the search space
    struct SearchQueueElement *next;    // pointer to the next element in the search queue
};

void SetGoal(int **a);       // Set the goal state of the puzzle
void PrintPuzzle(int **a);      // print the puzzle to the standard output
void MoveTile(int **a, Location x, int direction);     // move tile at location x along the direction
void Scramble(int **a);      // scramble the initial pattern moves number of times
void FindBlankTile(int **a, Location *blank);       // find the location of the blank tile
int IsValidMove(Location blank, int move);      // determine if a move is valid
int HeuristicMisplacedTiles(int **goal, int **a);   // compute the heuristic - number of misplaced tiles
int GoalTest(int **goal, int **a);      // Test if the current state is a goal state
void PrintPath(int **a, int *path);     // print the path to the goal state
int * BFS(int **goal, int **a);      // breadth first search
int * DFS(int **goal, int **a);      //  first search
int * GBEFS(int **goal, int **a);      //  first search
int * AStar(int **goal, int **a);      //  first search
int * IDAStar(int **goal, int **a);      //  first search
int * AStarHeuristicManhattanDistance(int **goal, int **a); 
int * AStar_with_EvaluationFunction(int **goal, int **a,float w);
int maxdepth=SOLDEPTH;
int top=0;

struct heapNode heap[100000];

// search traversal functions
struct Node * CreateNode(int **a);         // create a node with the reuired information
struct SearchQueueElement *CreateSearchQueueElement(struct Node *curnode);        // Create hte search queue element
void AppendSearchQueueElementToEnd(struct SearchQueueElement* cursqelement);   // append a search queue elment to the end of the queue
void AppendSearchQueueElementToFront(struct SearchQueueElement* cursqelement); 
void FreeSearchMemory();


// search variables
struct SearchQueueElement *head = NULL;
struct SearchQueueElement *tail = NULL;

void addToHeap(Node *ptr, int v){
	int i,j,k;
	Node *tempptr;
	int tempv;
	heap[top].nodeptr=ptr;
	heap[top].value=v;
	i=top;
	while(i!=0){
		if(heap[(i-1)/2].value>= heap[i].value){
			tempptr=heap[(i-1)/2].nodeptr;
			tempv=heap[(i-1)/2].value;
			heap[(i-1)/2].nodeptr=heap[i].nodeptr;
			heap[(i-1)/2].value=heap[i].value;
			heap[i].nodeptr=tempptr;
			heap[i].value=tempv;
			i=(i-1)/2;
			
		}
		else
			break;
		}
		top++;
}

void removeTop(){
	top--;
	int i,j,k;
	Node *tempptr;
	int tempv;
	heap[0].nodeptr=heap[top].nodeptr;
	heap[0].value=heap[top].value;
	i=0;
	while(2*i+2<=top){
		if(heap[2*i+1].value< heap[i].value || heap[2*i+2].value< heap[i].value){
			if(heap[2*i+1].value< heap[2*i+2].value){
				tempptr=heap[i].nodeptr;
				tempv=heap[i].value;
				heap[i].nodeptr=heap[2*i+1].nodeptr;
				heap[i].value=heap[2*i+1].value;
				heap[2*i+1].nodeptr=tempptr;
				heap[2*i+1].value=tempv;
				i=2*i+1;
			}
			else{
				tempptr=heap[i].nodeptr;
				tempv=heap[i].value;
				heap[i].nodeptr=heap[2*i+2].nodeptr;
				heap[i].value=heap[2*i+2].value;
				heap[2*i+2].nodeptr=tempptr;
				heap[2*i+2].value=tempv;
				i=2*i+2;
			}
		}
		else
			break;
	}

}
int main(int argc, char *argv[])
{
    int i, j,choice;
    int **puzzle, **goal;       // puzzle variable
    int *path;

    // allocate memory to the variable that stores the puzzle.
    puzzle = (int **)malloc(sizeof(int *)*N);
    goal = (int **)malloc(sizeof(int *)*N);
    for (i=0; i<N; i++)
    {
        puzzle[i] = (int *)calloc(N, sizeof(int));
        goal[i] = (int *)calloc(N, sizeof(int));
    }

    // set the goal state the puzzle
    SetGoal(puzzle);

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            goal[i][j] = puzzle[i][j];
            
     cout<<"Enter the actual solution depth  :";
     cin>>SOLDEPTH;
     for(i=0;i<N;i++)
    			for(j=0;j<N;j++)
    				puzzle[i][j]=goal[i][j];
    	
    		Scramble(puzzle);
     cout<<"Choose the Algorithm and Press appropriate key"<<endl;
     cout<<"1 for BFS"<<endl;
     cout<<"2 for DFS"<<endl;	
     cout<<"3 for GBEFS"<<endl;	
     cout<<"4 for AStar"<<endl;	
     cout<<"5 for IDAStar"<<endl;
     cout<<"6 for AStar with Manhatten Distance"<<endl;
     cout<<"7 for AStar with Evaluation Function"<<endl;
     cin>>choice;
     if(choice==1){
     	 printf("Goal state tile configuration:\n");
    		PrintPuzzle(goal);
    		
    		printf("Start state tile configuration:\n");
    		PrintPuzzle(puzzle);
     		struct timeval begin,end;
    		gettimeofday(&begin, NULL);
    		path = BFS(goal, puzzle);
    		gettimeofday(&end, NULL);
    		cout<<"Time in microseconds  = "<<end.tv_usec-begin.tv_usec<<endl;
    		PrintPath(puzzle, path);
    		FreeSearchMemory();
     }
     else if( choice==2){
     	 printf("Goal state tile configuration:\n");
    		PrintPuzzle(goal);
    		
    		printf("Start state tile configuration:\n");
    		PrintPuzzle(puzzle);
    		
    		struct timeval begin,end;
    		cout<<"Enter the depth limit ";
    		cin>>maxdepth;
    		gettimeofday(&begin, NULL);
    		
    		path = DFS(goal, puzzle);
    		gettimeofday(&end, NULL);
    		cout<<"Time in microseconds = "<<end.tv_usec-begin.tv_usec<<endl;
    		PrintPath(puzzle, path);
    		FreeSearchMemory();
     }
     
     else if( choice ==3){
     	 printf("Goal state tile configuration:\n");
    		PrintPuzzle(goal);
    		
    		printf("Start state tile configuration:\n");
    		PrintPuzzle(puzzle);
     		struct timeval begin,end;
    		gettimeofday(&begin, NULL);
    		path = GBEFS(goal, puzzle);
    		gettimeofday(&end, NULL);
    		cout<<"Time in microseconds = "<<end.tv_usec-begin.tv_usec<<endl;
    		PrintPath(puzzle, path);
    		FreeSearchMemory();
     }
     
     else if(choice ==4){
     		 printf("Goal state tile configuration:\n");
    		PrintPuzzle(goal);
    		
    		printf("Start state tile configuration:\n");
    		PrintPuzzle(puzzle);
     		struct timeval begin,end;
    		gettimeofday(&begin, NULL);
    		path = AStar(goal, puzzle);
    		gettimeofday(&end, NULL);
    		cout<<"Time in microseconds = "<<end.tv_usec-begin.tv_usec<<endl;
    		PrintPath(puzzle, path);
    		FreeSearchMemory();
     }
     
     else if(choice ==5){
     		 printf("Goal state tile configuration:\n");
    		PrintPuzzle(goal);
    		
    		printf("Start state tile configuration:\n");
    		PrintPuzzle(puzzle);
     		struct timeval begin,end;
    		gettimeofday(&begin, NULL);
    		path = IDAStar(goal, puzzle);
    		gettimeofday(&end, NULL);
    		cout<<"Time in microseconds = "<<end.tv_usec-begin.tv_usec<<endl;
    		PrintPath(puzzle, path);
    		FreeSearchMemory();
     }
     
     else if(choice==6){
     		 printf("Goal state tile configuration:\n");
    		PrintPuzzle(goal);
    		
    		printf("Start state tile configuration:\n");
    		PrintPuzzle(puzzle);
     		struct timeval begin,end;
    		gettimeofday(&begin, NULL);
    		path = AStarHeuristicManhattanDistance(goal, puzzle);
    		gettimeofday(&end, NULL);
    		cout<<"Time in microseconds = "<<end.tv_usec-begin.tv_usec<<endl;
    		PrintPath(puzzle, path);
    		FreeSearchMemory();
     }
     
     else if(choice ==7){
     		cout<<"Enter w( 0<= w <=1 )"<<endl;
     		float er;
     		cin>>er;
     		 printf("Goal state tile configuration:\n");
    		PrintPuzzle(goal);
    		
    		printf("Start state tile configuration:\n");
    		PrintPuzzle(puzzle);
     		struct timeval begin,end;
    		gettimeofday(&begin, NULL);
    		path = AStar_with_EvaluationFunction(goal, puzzle,er);
    		gettimeofday(&end, NULL);
    		cout<<"Time in microseconds = "<<end.tv_usec-begin.tv_usec<<endl;
    		PrintPath(puzzle, path);
    		FreeSearchMemory();
     
     }
     else{
     	cout<<"Please enter a valid choice"<<endl;
     }		
    
    for (i=0; i<N; i++)
    {
        free(puzzle[i]);
        free(goal[i]);
    }
    free(puzzle);
    free(goal);
    free(path);

    return(1);
}

// This function sets the goal configuration of the tiles in the puzzle. Feel free to replace
// the code within the function to any create an goal layout of your liking.
// This will the goal state of the puzzle
void SetGoal(int **a)
{
    int i, j;
    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            a[i][j] = (i*N)+j+1;
    return;
}

// This function prints the 8 puzzle problem to the standard output. The blank tile
// is represented by the tile with the number 9
void PrintPuzzle(int **a)
{
    int i, j;

    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
            if (a[i][j] != 9)
                printf("%d ", a[i][j]);
            else
                printf("  ");

        printf("\n");
    }
    return;
}


// This function moves the tile at location x, along the 4 possible directions
// 0 - left
// 1 - right
// 2 - top
// 3 - bottom
void MoveTile(int **a, Location x, int direction)
{
    int temp;

    // value of the tile at the source location
    temp = a[x.i][x.j];

    switch (direction)
    {
        // move tile right
    case 0:
        a[x.i][x.j] = a[x.i][x.j-1];
        a[x.i][x.j-1] = temp;
        break;
        // move tile left
    case 1:
        a[x.i][x.j] = a[x.i][x.j+1];
        a[x.i][x.j+1] = temp;
        break;
        // move tile top
    case 2:
        a[x.i][x.j] = a[x.i-1][x.j];
        a[x.i-1][x.j] = temp;
        break;
        // move tile bottom
    case 3:
        a[x.i][x.j] = a[x.i+1][x.j];
        a[x.i+1][x.j] = temp;
        break;
    }
    return;
}

// This function scrambles the initial tile configuration using valid moves.
// It keeps track of the location of the blank tile
// It moves the blank tile along any of the valid moves a fixed number of times
// specified by moves.
void Scramble(int **a)
{
    int i, j;
    Location blank;
    int nvalidmoves=0, *validmoves;
    int move=0;     // stores the move in the previous iteration


    for (i=0; i<SOLDEPTH; i++)
    {
        // determine the initial location of the blank tile
        FindBlankTile(a, &blank);

        // list all valid moves
        // ignore the moves that cancel the previous move
        validmoves = (int *)calloc(MAXVALIDMOVES, sizeof(int));
        for (j=0; j<MAXVALIDMOVES; j++)
        {
            // ignore the moves that cancel previous move
            if ((j == 0 && move != 1) || (j == 1 && move != 0) ||
                    (j == 2 && move != 3) || (j == 3 && move != 2))
            {
                // consider only valid mvoes
                if (IsValidMove(blank, j)==1)
                {
                    validmoves[nvalidmoves] = j;
                    nvalidmoves++;
                }
            }
        }

        // select a move at random
        //srand((unsigned)time(NULL));
        move = validmoves[rand() % nvalidmoves];
        // perform the move
        MoveTile(a, blank, move);

        nvalidmoves = 0;
        free(validmoves);
    }
}

// This function determines the location of the blank tile in the puzzle
void FindBlankTile(int **a, Location *blank)
{
    int i, j;

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            if (a[i][j] == 9)
            {
                blank->i = i;
                blank->j = j;
                return;
            }
}

// This function determines if a move is valid
// returns 1 if move is valid else 0
int IsValidMove(Location blank, int move)
{
    switch (move)
    {
        // left
    case 0:
        if (blank.j-1 >= 0)
            return(1);
        break;
        // right
    case 1:
        if (blank.j+1 <= N-1)
            return(1);
        break;
        // top
    case 2:
        if (blank.i-1 >= 0)
            return(1);
        break;
        // bottom
    case 3:
        if (blank.i+1 <= N-1)
            return(1);
        break;
    }
    return(0);
}

// This function computes the number of misplaced tiles
int HeuristicMisplacedTiles(int **goal, int **a)
{
    int i, j;
    int h=0;

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            if (goal[i][j] != a[i][j])
                h++;
    return(h);
}

// This function checks if the current state is the goal state
int GoalTest(int **goal, int **a)
{
    int i, j;

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            if (goal[i][j] != a[i][j])
                return(0);
    return(1);
}

unsigned long stateNumber(int **a){
	unsigned long k=0;
	int i,j;
	
	for(i=0;i<N; i++){
		for(j=0; j<N; j++){
			if(a[i][j]==9){
				k=10*k;
				}
			else{
				k=10*k+ a[i][j];
			}
		}
	}
	return k;
}

// This function computes sum of Manhattan distance heuristic
int HeuristicManhattanDistance(int **goal, int **a)
{
    // Fill in the code
    int i,j,k,l,sum=0;
    for(i=0;i<N;i++){
    	for(j=0;j<N;j++){
    		for(k=0;k<N;k++){
    			for(l=0;l<N;l++){
    				if(goal[i][j]==a[k][l]){
    					sum+=abs(k-i)+abs(j-l);
    				}
    			}
    		}
    	}
    }
    return sum;
}

void PrintPath(int **a, int *path)
{
    int i;
    Location blank;

    printf("path to the goal node: \n");
    PrintPuzzle(a);
    for (i=path[0]; i>0; i--)
    {
        FindBlankTile(a, &blank);
        MoveTile(a, blank, path[i]);
        printf("move blank tile: ");
        switch (path[i])
        {
            case 0: printf("left");
                    break;
            case 1: printf("right");
                    break;
            case 2: printf("top");
                    break;
            case 3: printf("bottom");
                break;
        }
        printf("\n");
        PrintPuzzle(a);
    }
}

// This function performs breadth first search
int *BFS(int **goal, int **start)
{
    int i,j,before=0,after=0,nodesExpanded,nodesGenerated,depthReached;
    struct Node *curnode;
    struct SearchQueueElement *cursqelement, *temphead;
    Location blank;
    int *path = NULL;
    
    map<unsigned long, bool> exploredStates;

    // create the root node of the search tree
    curnode = CreateNode(start);
	curnode->depth=0;
    // create the first element of the search queue
    if (head == NULL)
    {
        cursqelement = CreateSearchQueueElement(curnode);
        head = cursqelement;
        tail=cursqelement;
    }

    temphead = head;
	nodesExpanded=0;
	nodesGenerated=1;
	depthReached=0;
    while(temphead != NULL)
    {
        // check for goal
        if(temphead->nodeptr->depth> depthReached)
                		depthReached=temphead->nodeptr->depth;
        nodesExpanded++;
        if (GoalTest(goal, temphead->nodeptr->layout) == 1)
        {
            // we have found a goal state!
            printf("goal state found at depth: %d\n", (int)temphead->nodeptr->g_val);
            // path[0] - length of the path
            // path[1:path[0]] - the moves in the path
            path = (int *)malloc(sizeof(int));
            path[0] = 0;
            // traverse the path in the reverse order - from goal to root
            // while traversing, store the blank tile moves for each step
            curnode = temphead->nodeptr;
            while (curnode->parent != NULL)
            {
                path = (int *)realloc(path, sizeof(int)*(path[0]+2));
                path[path[0]+1] = curnode->move;
                path[0]++;
                curnode = curnode->parent;
            }
            cout<<"Nodes Expanded = "<<nodesExpanded<<endl;
            cout<<"Noded Generated=  "<<nodesGenerated<<endl;
            cout<<"Maximum Depth Explored =  "<<depthReached<<endl;
          cout<<" Memory used in bytes =  "<<(sizeof(Node)+sizeof(SearchQueueElement)+ 9*sizeof(int))*nodesGenerated<<endl;
            return(path);
        }
        FindBlankTile(temphead->nodeptr->layout, &blank);
        // compute the children of the current node
        for (i=0; i<MAXVALIDMOVES; i++)
        {
            if (IsValidMove(blank, i) == 1)
            {
                curnode = CreateNode(temphead->nodeptr->layout);
                MoveTile(curnode->layout, blank, i);
                curnode->depth=temphead->nodeptr->depth+1;
                before++;
                if(!exploredStates[stateNumber(curnode->layout)]){
                	after++;
                	nodesGenerated++;
                	exploredStates[stateNumber(curnode->layout)]=true;
                	curnode->move = i;
                	curnode->g_val = temphead->nodeptr->g_val+1;
                	curnode->parent = temphead->nodeptr;
                	cursqelement = CreateSearchQueueElement(curnode);
                	AppendSearchQueueElementToEnd(cursqelement);
                	
		}
		else{
			for (j=0; j<N; j++)
            			free(curnode->layout[j]);
        		free(curnode->layout);
        		free(curnode);
		}
            }
        }

        temphead = temphead->next;
    }
    return(path);
}


int *DFS(int **goal, int **start)
{
    int i,j,before=0,after=0,nodesExpanded,nodesGenerated,depthReached;
    struct Node *curnode;
    struct SearchQueueElement *cursqelement, *temphead;
    Location blank;
    int *path = NULL;
    
    map<unsigned long, int> exploredStates;
    exploredStates.clear();

    // create the root node of the search tree
    curnode = CreateNode(start);
    curnode->depth=1;
    exploredStates[stateNumber(curnode->layout)]=curnode->depth;
	head=NULL;
	tail=NULL;
    // create the first element of the search queue
    if (head == NULL)
    {
        cursqelement = CreateSearchQueueElement(curnode);
        head = cursqelement;
        head->next=NULL;
    }

    temphead = head;
    head=head->next;
	nodesExpanded=0;
	nodesGenerated=1;
	depthReached=0;
	
    while(temphead != NULL)
    {
        // check for goal
    //   cout<<"1"<<endl;
    if(temphead->nodeptr->depth> depthReached)
                		depthReached=temphead->nodeptr->depth;
        nodesExpanded++;
     //    cout<<"2"<<endl;
        if (GoalTest(goal, temphead->nodeptr->layout) == 1)
        {
            // we have found a goal state!
            printf("goal state found at depth: %d\n",(int)temphead->nodeptr->g_val);
            // path[0] - length of the path
            // path[1:path[0]] - the moves in the path
            path = (int *)malloc(sizeof(int));
            path[0] = 0;
            // traverse the path in the reverse order - from goal to root
            // while traversing, store the blank tile moves for each step
            curnode = temphead->nodeptr;
            while (curnode->parent != NULL)
            {
                path = (int *)realloc(path, sizeof(int)*(path[0]+2));
                path[path[0]+1] = curnode->move;
                path[0]++;
                curnode = curnode->parent;
            }
     		cout<<"Nodes Expanded = "<<nodesExpanded<<endl;
            cout<<"Noded Generated=  "<<nodesGenerated<<endl;
            cout<<"Maximum Depth Explored =  "<<depthReached-1<<endl;
             cout<<" Memory used in bytes  "<<(sizeof(Node)+sizeof(SearchQueueElement)+ 9*sizeof(int))*nodesGenerated<<endl;
            return(path);
        }
        FindBlankTile(temphead->nodeptr->layout, &blank);
        // compute the children of the current node
     //  cout<<"3"<<endl;
        for (i=0; i<MAXVALIDMOVES; i++)
        {
        //	  cout<<"4 "<<i<<endl;
            if (IsValidMove(blank, i) == 1)
            {
          //  	 cout<<"4 "<<i<<endl;
         //   	 cout<<"5"<<endl;
                curnode = CreateNode(temphead->nodeptr->layout);
         //        cout<<"6"<<endl;
                MoveTile(curnode->layout, blank, i);
          //       cout<<"7"<<endl;
                curnode->depth=temphead->nodeptr->depth+1;
         //        cout<<"8"<<endl;
                before++;
                // cout<<"Hello"<<maxdepth<<endl;
                if((exploredStates[stateNumber(curnode->layout)]==0 && curnode->depth<=maxdepth+1) || (exploredStates[stateNumber(curnode->layout)] > curnode->depth)){
                	
                	after++;
              //  	 cout<<"9"<<endl;
                	exploredStates[stateNumber(curnode->layout)]=curnode->depth;
                	curnode->move = i;
                	curnode->g_val = temphead->nodeptr->g_val+1;
                	curnode->parent = temphead->nodeptr;
                	nodesGenerated++;
                	// cout<<"10"<<endl;
                	cursqelement = CreateSearchQueueElement(curnode);
                	AppendSearchQueueElementToFront(cursqelement);
                	
		}
		else{
		//	cout<<"9"<<endl;
			for (j=0; j<N; j++)
            			free(curnode->layout[j]);
            		
        		free(curnode->layout);
        		free(curnode);
        	//	cout<<"10"<<endl;
        		
		}
            }
        }

        temphead = head;
        head=head->next;
    }
    return(path);
}

int *GBEFS(int **goal, int **start)
{
    int i,j,before=0,after=0,nodesExpanded,nodesGenerated,depthReached;
    struct Node *curnode;
    struct SearchQueueElement *cursqelement, *temphead;
    Location blank;
    int *path = NULL;
    
    map<unsigned long, bool> exploredStates;

    // create the root node of the search tree
    curnode = CreateNode(start);
    curnode->depth=0;
    curnode->h_val=HeuristicMisplacedTiles(goal,curnode->layout);
	head=NULL;
    // create the first element of the search queue
    top=0;
    if (top == 0)
    {
        temphead = CreateSearchQueueElement(curnode);
       // head = cursqelement;
       // head->next=NULL;
        addToHeap(curnode,curnode->h_val);
    }
	nodesExpanded=0;
	nodesGenerated=1;
	depthReached=0;
    while(top!=0)
    {
    	
    	nodesExpanded++;
    	temphead->nodeptr=heap[0].nodeptr;
   	if(temphead->nodeptr->depth> depthReached)
                		depthReached=temphead->nodeptr->depth;
    	removeTop();
    	
        // check for goal
        if (GoalTest(goal, temphead->nodeptr->layout) == 1)
        {
            // we have found a goal state!
            printf("goal state found at depth: %d\n", (int)temphead->nodeptr->g_val);
            // path[0] - length of the path
            // path[1:path[0]] - the moves in the path
            path = (int *)malloc(sizeof(int));
            path[0] = 0;
            // traverse the path in the reverse order - from goal to root
            // while traversing, store the blank tile moves for each step
            curnode = temphead->nodeptr;
            while (curnode->parent != NULL)
            {
                path = (int *)realloc(path, sizeof(int)*(path[0]+2));
                path[path[0]+1] = curnode->move;
                path[0]++;
                curnode = curnode->parent;
            }
          cout<<"Nodes Expanded = "<<nodesExpanded<<endl;
            cout<<"Noded Generated=  "<<nodesGenerated<<endl;
            cout<<"Maximum Depth Explored =  "<<depthReached<<endl;
             cout<<"Memory used in bytes = "<<(sizeof(Node)+sizeof(heapNode)+9*sizeof(int))*nodesGenerated<<endl;
            return(path);
        }
        FindBlankTile(temphead->nodeptr->layout, &blank);
        // compute the children of the current node
        for (i=0; i<MAXVALIDMOVES; i++)
        {
            if (IsValidMove(blank, i) == 1)
            {
                curnode = CreateNode(temphead->nodeptr->layout);
                MoveTile(curnode->layout, blank, i);
                curnode->depth=temphead->nodeptr->depth+1;
                before++;
                if(!exploredStates[stateNumber(curnode->layout)]){
                	after++;
                	exploredStates[stateNumber(curnode->layout)]=true;
                	curnode->move = i;
                	curnode->g_val = temphead->nodeptr->g_val+1;
                	curnode->parent = temphead->nodeptr;
                	nodesGenerated++;
                	curnode->h_val=HeuristicMisplacedTiles(goal,curnode->layout);
                	addToHeap(curnode,curnode->h_val);
                	
		}
		else{
			for (j=0; j<N; j++)
            			free(curnode->layout[j]);
        		free(curnode->layout);
        		free(curnode);
		}
            }
        }

       
    }
    return(path);
}

int *AStar(int **goal, int **start)
{
    int i,j,before=0,after=0,nodesExpanded,nodesGenerated,depthReached;
    struct Node *curnode;
    struct SearchQueueElement *cursqelement, *temphead;
    Location blank;
    int *path = NULL;
    
    map<unsigned long, bool> exploredStates;

    // create the root node of the search tree
    curnode = CreateNode(start);
    curnode->depth=0;
    curnode->h_val=HeuristicMisplacedTiles(goal,curnode->layout);
    curnode->f_val=curnode->g_val+curnode->h_val;
	head=NULL;
    // create the first element of the search queue
    top=0;
    if (top == 0)
    {
        temphead = CreateSearchQueueElement(curnode);
       // head = cursqelement;
       // head->next=NULL;
        addToHeap(curnode,curnode->f_val);
    }

	nodesExpanded=0;
	nodesGenerated=1;
	depthReached=0;
    while(top!=0)
    {	
    	
    	nodesExpanded++;
    	temphead->nodeptr=heap[0].nodeptr;
   	if(temphead->nodeptr->depth > depthReached)
                		depthReached=temphead->nodeptr->depth;
    	removeTop();
    	
        // check for goal
        if (GoalTest(goal, temphead->nodeptr->layout) == 1)
        {
            // we have found a goal state!
             printf("goal state found at depth: %d\n", (int)temphead->nodeptr->g_val);
            // path[0] - length of the path
            // path[1:path[0]] - the moves in the path
            path = (int *)malloc(sizeof(int));
            path[0] = 0;
            // traverse the path in the reverse order - from goal to root
            // while traversing, store the blank tile moves for each step
            curnode = temphead->nodeptr;
            while (curnode->parent != NULL)
            {
                path = (int *)realloc(path, sizeof(int)*(path[0]+2));
                path[path[0]+1] = curnode->move;
                path[0]++;
                curnode = curnode->parent;
            }
           cout<<"Nodes Expanded = "<<nodesExpanded<<endl;
            cout<<"Noded Generated=  "<<nodesGenerated<<endl;
            cout<<"Maximum Depth Explored =  "<<depthReached<<endl;
            cout<<"Memory used in bytes = "<<(sizeof(Node)+sizeof(heapNode)+9*sizeof(int))*nodesGenerated<<endl;
            return(path);
        }
        FindBlankTile(temphead->nodeptr->layout, &blank);
        // compute the children of the current node
        for (i=0; i<MAXVALIDMOVES; i++)
        {
            if (IsValidMove(blank, i) == 1)
            {
                curnode = CreateNode(temphead->nodeptr->layout);
                MoveTile(curnode->layout, blank, i);
                curnode->depth=temphead->nodeptr->depth+1;
                before++;
                if(!exploredStates[stateNumber(curnode->layout)]){
                	after++;
                	exploredStates[stateNumber(curnode->layout)]=true;
                	curnode->move = i;
                	curnode->g_val = temphead->nodeptr->g_val+1;
                	curnode->parent = temphead->nodeptr;
                	nodesGenerated++;
                	curnode->h_val=HeuristicMisplacedTiles(goal,curnode->layout);
                	curnode->f_val=curnode->g_val+curnode->h_val;
                	addToHeap(curnode,curnode->f_val);
                	
		}
		else{
			for (j=0; j<N; j++)
            			free(curnode->layout[j]);
        		free(curnode->layout);
        		free(curnode);
		}
            }
        }

       
    }
    return(path);
}



int *IDAStar(int **goal, int **start)
{
    int i,j,before=0,after=0,depthlimit=0,nodesExpanded,nodesGenerated,depthReached;
    struct Node *curnode;
    struct SearchQueueElement *cursqelement, *temphead;
    Location blank;
    int *path = NULL;
    temphead = CreateSearchQueueElement(curnode);
    
    while(1){
    map<unsigned long, bool> exploredStates;
    curnode = CreateNode(start);
   
    curnode->h_val=HeuristicMisplacedTiles(goal,curnode->layout);
    curnode->f_val=curnode->g_val+curnode->h_val;
     curnode->depth=curnode->f_val;
	head=NULL;
    // create the first element of the search queue
    top=0;
    if (top == 0)
    {
        
       // head = cursqelement;
       // head->next=NULL;
        addToHeap(curnode,curnode->f_val);
    }
	nodesExpanded=0;
	nodesGenerated=1;
	depthReached=0;
    while(top!=0)
    {
    	nodesExpanded++;
    	temphead->nodeptr=heap[0].nodeptr;
    	
   	if(temphead->nodeptr->depth> depthReached)
                		depthReached=temphead->nodeptr->depth;
    	removeTop();
    	
        // check for goal
        if (GoalTest(goal, temphead->nodeptr->layout) == 1)
        {
            // we have found a goal state!
            printf("goal state found at depth: %d\n", (int)temphead->nodeptr->g_val);
            // path[0] - length of the path
            // path[1:path[0]] - the moves in the path
            path = (int *)malloc(sizeof(int));
            path[0] = 0;
            // traverse the path in the reverse order - from goal to root
            // while traversing, store the blank tile moves for each step
            curnode = temphead->nodeptr;
            while (curnode->parent != NULL)
            {
                path = (int *)realloc(path, sizeof(int)*(path[0]+2));
                path[path[0]+1] = curnode->move;
                path[0]++;
                curnode = curnode->parent;
            }
            cout<<"Nodes Expanded = "<<nodesExpanded<<endl;
            cout<<"Noded Generated=  "<<nodesGenerated<<endl;
            cout<<"Maximum Depth Explored =  "<<depthReached<<endl;
             cout<<"Memory used in bytes = "<<(sizeof(Node)+sizeof(heapNode)+9*sizeof(int))*nodesGenerated<<endl;
            return(path);
        }
        FindBlankTile(temphead->nodeptr->layout, &blank);
        // compute the children of the current node
        for (i=0; i<MAXVALIDMOVES; i++)
        {
            if (IsValidMove(blank, i) == 1)
            {
                curnode = CreateNode(temphead->nodeptr->layout);
                MoveTile(curnode->layout, blank, i);
                curnode->depth=temphead->nodeptr->depth+1;
                curnode->g_val = temphead->nodeptr->g_val+1;
                curnode->h_val=HeuristicMisplacedTiles(goal,curnode->layout);
                curnode->f_val=curnode->g_val+curnode->h_val;
                before++;
                if(!exploredStates[stateNumber(curnode->layout)] && curnode->f_val <= depthlimit ){
                	after++;
                	exploredStates[stateNumber(curnode->layout)]=true;
                	curnode->move = i;
                	
                	curnode->parent = temphead->nodeptr;
                	nodesGenerated++;
                	addToHeap(curnode,curnode->f_val);
                	
		}
		else{
			for (j=0; j<N; j++)
            			free(curnode->layout[j]);
        		free(curnode->layout);
        		free(curnode);
		}
            }
        }

       
    }
    depthlimit++;
    }
    return(path);
}

int *AStarHeuristicManhattanDistance(int **goal, int **start)
{
    int i,j,before=0,after=0,nodesExpanded,nodesGenerated,depthReached;
    struct Node *curnode;
    struct SearchQueueElement *cursqelement, *temphead;
    Location blank;
    int *path = NULL;
    
    map<unsigned long, bool> exploredStates;

    // create the root node of the search tree
    curnode = CreateNode(start);
    curnode->depth=0;
    curnode->h_val=HeuristicManhattanDistance(goal,curnode->layout);
    curnode->f_val=curnode->g_val+curnode->h_val;
	head=NULL;
    // create the first element of the search queue
    top=0;
    if (top == 0)
    {
        temphead = CreateSearchQueueElement(curnode);
       // head = cursqelement;
       // head->next=NULL;
        addToHeap(curnode,curnode->f_val);
    }

	nodesExpanded=0;
	nodesGenerated=1;
	depthReached=0;
    while(top!=0)
    {
    	nodesExpanded++;
    	temphead->nodeptr=heap[0].nodeptr;
   	if(temphead->nodeptr->depth> depthReached)
                		depthReached=temphead->nodeptr->depth;
    	removeTop();
    	
        // check for goal
        if (GoalTest(goal, temphead->nodeptr->layout) == 1)
        {
            // we have found a goal state!
            printf("goal state found at depth: %d\n", (int)temphead->nodeptr->g_val);
            // path[0] - length of the path
            // path[1:path[0]] - the moves in the path
            path = (int *)malloc(sizeof(int));
            path[0] = 0;
            // traverse the path in the reverse order - from goal to root
            // while traversing, store the blank tile moves for each step
            curnode = temphead->nodeptr;
            while (curnode->parent != NULL)
            {
                path = (int *)realloc(path, sizeof(int)*(path[0]+2));
                path[path[0]+1] = curnode->move;
                path[0]++;
                curnode = curnode->parent;
            }
           cout<<"Nodes Expanded = "<<nodesExpanded<<endl;
            cout<<"Noded Generated=  "<<nodesGenerated<<endl;
            cout<<"Maximum Depth Explored =  "<<depthReached<<endl;
            cout<<"Memory used in bytes = "<<(sizeof(Node)+sizeof(heapNode)+9*sizeof(int))*nodesGenerated<<endl;
            return(path);
        }
        FindBlankTile(temphead->nodeptr->layout, &blank);
        // compute the children of the current node
        for (i=0; i<MAXVALIDMOVES; i++)
        {
            if (IsValidMove(blank, i) == 1)
            {
                curnode = CreateNode(temphead->nodeptr->layout);
                MoveTile(curnode->layout, blank, i);
                curnode->depth=temphead->nodeptr->depth+1;
                before++;
                if(!exploredStates[stateNumber(curnode->layout)]){
                	after++;
                	exploredStates[stateNumber(curnode->layout)]=true;
                	curnode->move = i;
                	curnode->g_val = temphead->nodeptr->g_val+1;
                	curnode->parent = temphead->nodeptr;
                	nodesGenerated++;
                	curnode->h_val=HeuristicManhattanDistance(goal,curnode->layout);
                	curnode->f_val=curnode->g_val+curnode->h_val;
                	addToHeap(curnode,curnode->f_val);
                	
		}
		else{
			for (j=0; j<N; j++)
            			free(curnode->layout[j]);
        		free(curnode->layout);
        		free(curnode);
		}
            }
        }

       
    }
    return(path);
}

int *AStar_with_EvaluationFunction(int **goal, int **start,float w)
{
    int i,j,before=0,after=0,nodesExpanded,nodesGenerated,depthReached;
    struct Node *curnode;
    struct SearchQueueElement *cursqelement, *temphead;
    Location blank;
    int *path = NULL;
    
    map<unsigned long, bool> exploredStates;

    // create the root node of the search tree
    curnode = CreateNode(start);
    curnode->depth=0;
    curnode->h_val=HeuristicManhattanDistance(goal,curnode->layout);
    curnode->f_val=w*curnode->g_val+(1-w)*curnode->h_val;
	head=NULL;
    // create the first element of the search queue
    top=0;
    if (top == 0)
    {
        temphead = CreateSearchQueueElement(curnode);
       // head = cursqelement;
       // head->next=NULL;
        addToHeap(curnode,curnode->f_val);
    }

	nodesExpanded=0;
	nodesGenerated=1;
	depthReached=0;
    while(top!=0)
    {
    	nodesExpanded++;
    	temphead->nodeptr=heap[0].nodeptr;
    	
   	if(temphead->nodeptr->depth> depthReached)
                		depthReached=temphead->nodeptr->depth;
    	removeTop();
    	
        // check for goal
        if (GoalTest(goal, temphead->nodeptr->layout) == 1)
        {
            // we have found a goal state!
            printf("goal state found at depth: %d\n", (int)temphead->nodeptr->g_val);
            // path[0] - length of the path
            // path[1:path[0]] - the moves in the path
            path = (int *)malloc(sizeof(int));
            path[0] = 0;
            // traverse the path in the reverse order - from goal to root
            // while traversing, store the blank tile moves for each step
            curnode = temphead->nodeptr;
            while (curnode->parent != NULL)
            {
                path = (int *)realloc(path, sizeof(int)*(path[0]+2));
                path[path[0]+1] = curnode->move;
                path[0]++;
                curnode = curnode->parent;
            }
           cout<<"Nodes Expanded = "<<nodesExpanded<<endl;
            cout<<"Noded Generated=  "<<nodesGenerated<<endl;
            cout<<"Maximum Depth Explored =  "<<depthReached<<endl;
             cout<<"Memory used in bytes =  "<<(sizeof(Node)+sizeof(heapNode)+9*sizeof(int))*nodesGenerated<<endl;
            return(path);
        }
        FindBlankTile(temphead->nodeptr->layout, &blank);
        // compute the children of the current node
        for (i=0; i<MAXVALIDMOVES; i++)
        {
            if (IsValidMove(blank, i) == 1)
            {
                curnode = CreateNode(temphead->nodeptr->layout);
                MoveTile(curnode->layout, blank, i);
                curnode->depth=temphead->nodeptr->depth+1;
                before++;
                if(!exploredStates[stateNumber(curnode->layout)]){
                	after++;
                	exploredStates[stateNumber(curnode->layout)]=true;
                	curnode->move = i;
                	curnode->g_val = temphead->nodeptr->g_val+1;
                	curnode->parent = temphead->nodeptr;
                	nodesGenerated++;
                	curnode->h_val=HeuristicManhattanDistance(goal,curnode->layout);
                	curnode->f_val=w*curnode->g_val+(1-w)*curnode->h_val;
                	addToHeap(curnode,curnode->f_val);
                	
		}
		else{
			for (j=0; j<N; j++)
            			free(curnode->layout[j]);
        		free(curnode->layout);
        		free(curnode);
		}
            }
        }

       
    }
    return(path);
}


// This function creates a node variable. Copies the contents of the layout of the node,
// computes the heuristic values
struct Node *CreateNode(int **a)
{
    int i, j;
    struct Node *curnode;

    curnode = (struct Node *)malloc(sizeof(struct Node));
    curnode->layout = (int **)malloc(sizeof(int*)*N);
    for (i=0; i<N; i++)
    {
        curnode->layout[i] = (int *)malloc(sizeof(int)*N);
        for (j=0; j<N; j++)
            curnode->layout[i][j] = a[i][j];
    }
    curnode->parent = NULL;
    curnode->g_val = 0;
    curnode->move = -1;

    return(curnode);
}

// This function creates a search queue element
struct SearchQueueElement *CreateSearchQueueElement(struct Node *curnode)
{
    struct SearchQueueElement *cursqelement;

    cursqelement = (struct SearchQueueElement*)malloc(sizeof(struct SearchQueueElement));
    cursqelement->nodeptr = curnode;
    cursqelement->next = NULL;

    return(cursqelement);

}

// This function appends a search queue element to the end of the queue - for breadth first search
void AppendSearchQueueElementToEnd(struct SearchQueueElement* cursqelement)
{
    struct SearchQueueElement *tempsqelement;
    if (head != NULL)
    {
       /* tempsqelement = head;
        while (tempsqelement->next != NULL)
            tempsqelement = tempsqelement->next;
        tempsqelement->next = cursqelement;*/
        tail->next=cursqelement;
        tail=tail->next;
        tail->next=NULL;
    }
    else{
        head = cursqelement;
        tail=cursqelement;
        tail->next=NULL;
        }
    return;
}

void AppendSearchQueueElementToFront(struct SearchQueueElement* cursqelement)
{
    struct SearchQueueElement *tempsqelement;
    if (head != NULL)
    {
    	cursqelement->next=head;
    	head=cursqelement;
    }
    else{
        head = cursqelement;
        tail=cursqelement;
        
        }
    return;
}

void FreeSearchMemory()
{
    int i;
    struct SearchQueueElement *cursqelement, *tempsqelement;
    struct Node *curnode;

    cursqelement = head;
    while (cursqelement != NULL)
    {
        tempsqelement = cursqelement;
        cursqelement = cursqelement->next;
        curnode = tempsqelement->nodeptr;
        free(tempsqelement);
        for (i=0; i<N; i++)
            free(curnode->layout[i]);
        free(curnode->layout);
        free(curnode);
    }
    head = NULL;
}
