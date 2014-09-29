#include<iostream>
#include <stdlib.h>
#include<time.h>
#include<stdio.h>
using namespace std;

#define MAX_DEPTH 6
#define ALPHA 1
#define BETA 0

struct Result 
{
    int x;
    int y;
    float score;
};


int checkr(int x,int y,char (*a)[8],char ch);
int check(int x,int y,char (*a)[8],char ch);
int input(char (*a)[8]);
int draw(char (*b)[8]);
int count_score(char (*a)[8]);

int find_score ( char a[8][8] );
int evaluate ( char a[8][8] ); 
bool board_full ( char a[8][8]);

static int nodes_generated;
static int nodes_explored;
static int nodes_pruned;
static int total_nodes_generated;
static int total_nodes_explored;
static int total_nodes_pruned;

int valid_moves( char a[8][8]){
 int pos[60][2];
      int count=0;
      for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {
            if(a[i][j]==' ')
            {
                if(checkr(i,j,a,'*'))
			     {
						pos[count][0]=i;
						pos[count][1]=j;
						count++;
						//cout<<i<<"  "<<j<<"  AND  ";
				 }	
			}	
	   }
	
	for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {
            if(a[i][j]==' ')
            {
                if(checkr(i,j,a,'O'))
			     {
						pos[count][0]=i;
						pos[count][1]=j;
						count--;
						//cout<<i<<"  "<<j<<"  AND  ";
				 }	
			}	
	   }
	 
	 return count;
}

Result minimax (char a[8][8], int alpha , int beta , bool pruned, int depth=0, char turn='*', int x=9, int y=9 )
{  
                  
      //draw(a);
      //cout<<"THIS IS DEPTH   "<<depth<<"    AND TURN IS OF   "<<turn<<endl;
      nodes_generated++;
      Result res;
      res.x = x;
      res.y = y;

      turn == '*' ? res.score = -999 : res.score = 999;
      if(pruned)   
      	nodes_pruned++;
      	
      	
      if (board_full(a))
      {
            res.score = find_score(a);
            //cout<< "BOARD FULL"<<endl;
            return res;
      }
      
      if (depth == MAX_DEPTH)
       {
            int temp1 = evaluate (a);
            int temp2 = find_score(a);
            int temp3= valid_moves(a);
            res.score = ALPHA*temp1 + temp2*(1-ALPHA-BETA)+ temp3*(BETA);
            //cout<< "MAX DEPTH REACHED"<<endl;
            return res;
      }
      if(!pruned)
      	nodes_explored++;
      	
      int pos[60][2];
      int count=0;
      for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {
            if(a[i][j]==' ')
            {
                if(checkr(i,j,a,turn))
			     {
						pos[count][0]=i;
						pos[count][1]=j;
						count++;
						//cout<<i<<"  "<<j<<"  AND  ";
				 }	
			}	
	   }
	   
	   //cout<<endl<<"TOTAL VALID MOVES ARE  "<<count<<endl;

      if (count == 0)
      {
            //cout<<"No Valid Move Found"<<endl;
            char new_board[8][8];
            for (int j=0 ; j<8; j++)
                for(int k=0; k<8; k++)
                    new_board[j][k] = a[j][k];
            Result temp = minimax (new_board,alpha,beta,pruned, depth+1, turn == '*' ? 'O' : '*', 9, 9);
            return temp;
    }
      
      for (int i = 0 ; i<count ; i++)
      {
            char new_board[8][8];
            for (int j=0 ; j<8; j++)
                for(int k=0; k<8; k++)
                    new_board[j][k] = a[j][k];
            check (pos[i][0], pos[i][1], new_board, turn);
            new_board [pos[i][0]] [pos[i][1]] = turn;
            
            //cout<<"CHECKING FOR MOVE NO.  "<<i<<'\t'<<pos[i][0]<<'\t'<<pos[i][1]<<endl;
            Result temp = minimax (new_board,alpha,beta,pruned, depth+1, turn == '*' ? 'O' : '*' , pos[i][0], pos[i][1]);
            //cout<<"FOR i = "<<i<<"  RESULT FOUND IS  "<<temp.x<<'\t'<<temp.y<<'\t'<<temp.score<<endl;
            //if (depth == 0)
            	//cout<<temp.score<<" at ("<<pos[i][0]<<","<<pos[i][1]<<")"<<'\t';
            	
            	
            if (turn == '*' && temp.score > res.score && !pruned)
            {
            	
                    res.score = temp.score;
                    res.x = pos[i][0];
                    res.y = pos[i][1];
                    alpha = temp.score;
            }
            
            else if (turn == 'O' && temp.score < res.score && !pruned)
            {
                    res.score = temp.score;
                    res.x = pos[i][0];
                    res.y = pos[i][1];
                    beta= temp.score;
            }
           if(alpha>beta)
           	pruned=true;
        }
        //cout<<"HERE ARE THE RETURNED VALUES at DEPTH "<<depth<< '\t'<< res.x<< '\t'<< res.y<< '\t'<< res.score<<endl<<endl<<endl;
        return res;
        
}

     

int main()
{
	char a[8][8];
	bool flag;
	for(int i=0;i<8;i++)
    	for(int j=0;j<8;j++)
    		a[i][j]=' ';
	a[3][3]='O';
	a[3][4]='*';
	a[4][3]='*';
	a[4][4]='O';
	//int turn=0;
	draw(a);
	int f=1,grid_count=0,winner;
	flag=false;
	total_nodes_generated=0;
	total_nodes_explored=0;
	total_nodes_pruned=0;
	while(grid_count<60)
	{
		
		
		f=input(a);
		if(f==0){
			if(flag){
			if(winner)
				cout<<"You Won\n";
			else
				cout<<"Sorry You lost\n";		
			cout<<"Game over"<<endl;
			cout<<"Total Nodes generated = "<<total_nodes_generated<<endl;
			cout<<"Total Nodes explored = "<<total_nodes_explored<<endl;
			cout<<"Total Nodes pruned = "<<total_nodes_pruned<<endl; 
			
			return 0;
			}
			flag=true;
			}
		else
			{
			flag=false;
			grid_count++;
			}
		draw(a);
		winner=count_score(a);
	}
	if(winner)
				cout<<"You Won\n";
			else
				cout<<"Sorry You lost\n";	
	cout<<"Game over"<<endl;
	cout<<"Total Nodes generated = "<<total_nodes_generated<<endl;
	cout<<"Total Nodes explored = "<<total_nodes_explored<<endl;
	cout<<"Total Nodes pruned = "<<total_nodes_pruned<<endl; 
	return 0;
}


bool board_full( char a[8][8])
{
      for (int i=0; i<8; i++)
      { 
          for (int j=0; j<8; j++)
          {
              if (a[i][j] == 'O' || a[i][j] == '*' )
                 continue;
              
              else
                return false;
          }
      }
      return true;
}

int find_score(char a[8][8])
{
	int score = 0;
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
		{
			if(a[i][j]=='O')
				score--;
			else if(a[i][j]=='*')
				score++;				
		}	
	return score;
}

int evaluate(char a[8][8])
{
	int score = 0;
	int wt;
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
		{
			if ( i==0 && j==0 || i==7 && j==7 || i==0 && j==7 || i==7 && j==0)
			     wt = 4;
			     
			else if (i==0 || i==7 || j==0 || j==7)
			     wt = 2;
			
			else
			     wt = 1;
			
            if(a[i][j]=='O')
				score -= wt;
                
            if (a[i][j] == '*')
                score += wt;				
		}	
	return score;
}

int checkr(int x,int y,char (*a)[8],char ch)
{
	int i=0,flag=0,j=0;
	if( x>7 || y>7 ||a[x][y]!=' ')
		return 0;
	// check left
	i=y-1;
	if(a[x][i]==ch)
		i=-10;	
	while(i>=0)
	{
		if(a[x][i]==' ')
			break;
		
		if(a[x][i]==ch)
		{	
			flag=1;
			break;
	    }
	    i--;
	}	
	
	// check right
	i=y+1;	
	if(a[x][i]==ch)
		i=10;
	while(i<8)
	{
		if(a[x][i]==' ')
			break;
		if(a[x][i]==ch)
		{	
			flag=1;
			break;
	    }
	    i++;
	}	
	
	// check down
	i=x+1;
	if(a[i][y]==ch)
		i=10;	
	while(i<8)
	{
		if(a[i][y]==' ')
			break;
		if(a[i][y]==ch)
		{	
			flag=1;
			break;
	    }
	    i++;
	}	

	// check up
	i=x-1;	
	if(a[i][y]==ch)
		i=-10;
	while(i>=0)
	{
		if(a[i][y]==' ')
			break;
		
		if(a[i][y]==ch)
		{	
			flag=1;
			break;
	    }
	    i--;
	}	

   // check NW
   i=x-1;j=y-1;
   if(a[i][j]==ch)
		i=-10;
   while(i>=0&&j>=0)
   {
   		if(a[i][j]==' ')
			break;
		
		if(a[i][j]==ch)
		{	
			flag=1;
			break;
	    }
	    i--;
	    j--;
   }
   
    // check SE
   i=x+1;j=y+1;
   if(a[i][j]==ch)
		i=10;
   while(i<=7&&j<=7)
   {
   		if(a[i][j]==' ')
			break;
		
		if(a[i][j]==ch)
		{	
			flag=1;
			break;
	    }
	    i++;
	    j++;
   }
   
   // check NE
   i=x-1;j=y+1;
   if(a[i][j]==ch)
		i=-10;
   while(i>=0&&j<=7)
   {
   		if(a[i][j]==' ')
			break;
		
		if(a[i][j]==ch)
		{	
			flag=1;
			break;
	    }
	    i--;
	    j++;
   }
   
   
   // check SW
   i=x+1;j=y-1;
   if(a[i][j]==ch)
		i=10;
   while(i<=7&&j>=0)
   {
   		if(a[i][j]==' ')
			break;
		
		if(a[i][j]==ch)
		{	
			flag=1;
			break;
	    }
	    i++;
	    j--;
   }
   
   
	if(flag==1)
		return 1;
	return 0;	
}



int check(int x,int y,char (*a)[8],char ch)
{
	int i=0,flag=0,j=0;
	if( x>7 || y>7 ||a[x][y]!=' ')
		return 0;

	// check left
	i=y-1;
	if(a[x][i]==ch)
		i=-10;	
	while(i>=0)
	{
		if(a[x][i]==' ')
			break;
		
		if(a[x][i]==ch)
		{	
			flag=1;
			while(i<y)
			{
				a[x][i]=ch;
				i++;
			}
			break;
	    }
	    i--;
	}	
	
	// check right
	i=y+1;	
	if(a[x][i]==ch)
		i=10;
	while(i<8)
	{
		if(a[x][i]==' ')
			break;
		if(a[x][i]==ch)
		{	
			flag=1;
			while(i>y)
			{
				a[x][i]=ch;
				i--;
			}
			break;
	    }
	    i++;
	}	
	
	// check down
	i=x+1;
	if(a[i][y]==ch)
		i=10;	
	while(i<8)
	{
		if(a[i][y]==' ')
			break;
		if(a[i][y]==ch)
		{	
			flag=1;
			while(i>x)
			{
				a[i][y]=ch;
				i--;
			}
			break;
	    }
	    i++;
	}	

	// check up
	i=x-1;	
	if(a[i][y]==ch)
		i=-10;
	while(i>=0)
	{
		if(a[i][y]==' ')
			break;
		
		if(a[i][y]==ch)
		{	
			flag=1;
			while(i<x)
			{
				a[i][y]=ch;
				i++;
			}
			break;
	    }
	    i--;
	}	

   // check NW
   i=x-1;j=y-1;
   if(a[i][j]==ch)
		i=-10;
   while(i>=0&&j>=0)
   {
   		if(a[i][j]==' ')
			break;
		
		if(a[i][j]==ch)
		{	
			flag=1;
			while(i<x&&j<y)
			{
				a[i][j]=ch;
				i++;
				j++;
			}
			break;
	    }
	    i--;
	    j--;
   }
   
    // check SE
   i=x+1;j=y+1;
   if(a[i][j]==ch)
		i=10;
   while(i<=7&&j<=7)
   {
   		if(a[i][j]==' ')
			break;
		
		if(a[i][j]==ch)
		{	
			flag=1;
			while(i>x&&j>y)
			{
				a[i][j]=ch;
				i--;
				j--;
			}
			break;
	    }
	    i++;
	    j++;
   }
   
   // check NE
   i=x-1;j=y+1;
   if(a[i][j]==ch)
		i=-10;
   while(i>=0&&j<=7)
   {
   		if(a[i][j]==' ')
			break;
		
		if(a[i][j]==ch)
		{	
			flag=1;
			while(i<x&&j>y)
			{
				a[i][j]=ch;
				i++;
				j--;
			}
			break;
	    }
	    i--;
	    j++;
   }
   
   
   // check SW
   i=x+1;j=y-1;
   if(a[i][j]==ch)
		i=10;
   while(i<=7&&j>=0)
   {
   		if(a[i][j]==' ')
			break;
		
		if(a[i][j]==ch)
		{	
			flag=1;
			while(i>x&&j<y)
			{
				a[i][j]=ch;
				i--;
				j++;
			}
			break;
	    }
	    i++;
	    j--;
   }
   
   
	if(flag==1)
		return 1;
	return 0;	
}


int input(char (*a)[8])
{	
	static int turn=-1;
	int x,y;
    srand (time(NULL));
  	turn++;

	if(turn%2==0)
	{
		int count=0;
		for(int i=0;i<8;i++)
		{	for(int j=0;j<8;j++)
			{
				if(a[i][j]==' ')
				{
					if(checkr(i,j,a,'O'))
					{
						count++;
						break;
					}
					
				}	
			}
		 if(count>=1)
		 	break;
		}	
		if(count==0)
		{
			cout<<"You have no valid move Hence giving turn to computer\n\n";
			return 0;
					
		}
		cout<<"Your Turn  ";
		
        
        
        char inx[10], iny[10];
		cin>>inx>>iny;
		x = inx[0] - '0';
		y = iny[0] - '0';
		//cout<<a[x][y]<<"here\n";
		while((!check(x,y,a,'O')))
		{
			cout<<"Invalid move try again\n";
			fflush(stdin);
			cin>>inx>>iny;
			x = inx[0] - '0';
			y = iny[0] - '0';
			if (inx[1]	!= '\0' || iny[1] != '\0')
				x=9;		
		}		
		a[x][y]='O';
		
		
	}
	else 
	{
		
		int pos[60][2];
		int count=0;
		for(int i=0;i<8;i++)
			for(int j=0;j<8;j++)
			{
				if(a[i][j]==' ')
				{
					if(checkr(i,j,a,'*'))
					{
						pos[count][0]=i;
						pos[count][1]=j;
						count++;
					}
					
				}	
			}
		if(count==0)
		{
			cout<<"Computer has no valid move \n\n";
			return 0;		
		}
		
		//cout<<"Possible moves for computer are "<<endl;
	nodes_generated=0;
	nodes_explored=0;
	nodes_pruned=0;
        Result r = minimax (a,-1000,1000,false);
        total_nodes_generated+=nodes_generated;
        total_nodes_explored+=nodes_explored;
        total_nodes_pruned+=nodes_pruned;
        
        cout<<endl<<endl<<"Computer moves at ("<<r.x<<","<<r.y<<")"<<endl;// for "<< r.score<<endl<<endl;
		if (!check(r.x ,r.y, a,'*'))
            cout<<"ERROR - INVALID MOVE FOUND"<<endl;
		else
            a[r.x][r.y] ='*';
	cout<<"Nodes Generated = "<<nodes_generated<<endl;
	cout<<"Nodes Explored = "<<nodes_explored<<endl;
	cout<<"Nodes Pruned = "<<nodes_pruned<<endl;
		/*
        int num = rand() % count;
		cout<<"Computer' turn at "<<pos[num][0]<<" "<<pos[num][1]<<"\n";
		check(pos[num][0],pos[num][1],a,'*');
		a[pos[num][0]][pos[num][1]]='*';
		*/
        //return 1;
	}	
	
	return 1;
}


int draw(char (*b)[8])
{
    cout<<"      0  1  2  3  4  5  6  7";	
    for(int i=0;i<8;i++)
    {   cout<<"\n\n";
        cout<<i<<"   "; 
    	for(int j=0;j<8;j++)
    	{	
    	       cout<<"  "<<b[i][j];
    	}
    }
    cout<<"\n\n";   
}

int count_score(char (*a)[8])
{
	int comp=0,human=0;
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
		{
			if(a[i][j]=='O')
				human++;
			else if(a[i][j]=='*')
				comp++;					
		}		
	cout<<"Your Score "<<human<<"\n";
	cout<<"Computer Score "<<comp<<"\n"<<"-----------------------------------------------\n";
	if(human>comp)
		return 1;
	else
		return 0;	
}

