#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include  <ctime>

#define MAX_INS 10000
int I,TR;
using namespace std;
char input[MAX_INS][23];
static char rep1;
static char rep2;
static int values[]={2,6,4,10,2,9,4,3,2,12,2,6,4,4,9,9,2,4,3,8,9,6,7};
static char symbol[23][12];
class node{
	public:
		int attribute;
		int N;
		int leaf_result;
		node * children[12];
		
};


double IGain(vector<int> examples,int at,double parent_entropy){
	double r,p,t;
	int j,k,N;
	N=examples.size();
	
	/*N=0;
	for(j=0;j<examples.size();j++){
		if(input[examples[j]][at]!='?'){
			N++;
		}
	} */
		r=0;
		for(j=0;j<values[at];j++){
			//cout<<i<<endl;	
			p=0;
			t=0;
			for(k=0;k<examples.size();k++){
			
				if(input[examples[k]][at]==symbol[at][j] || (input[examples[k]][at]=='?' && input[examples[k]][0]=='e' && rep1==symbol[at][j]) || (input[examples[k]][at]=='?' && input[examples[k]][0]=='p' && rep2==symbol[at][j]) ){
					t++;
				if(input[examples[k]][0]=='e')
					p++;
				}
				
			}
			p=p/t;
			
			
			if(t!=0 && p!=0 && p!=1){
			
			r+=(t/N)* (p*(-log2(p))- ((1-p)*(log2(1-p))));
			
			//cout<<r<<endl;
			}
		}

		return parent_entropy-r;
		
}

void ID3(node *parent,vector<int> examples,bool attribute[]){

	int i,j,k,min_index,positive_ex,negative_ex,leaf_result;
	double p=0,entropy,r,t,mn;
		
	for(i=0;i<parent->N;i++){
	
		if(input[examples[i]][0]=='e')
			p++;
	}
	positive_ex=p;
	negative_ex=parent->N-p;

	if(positive_ex>negative_ex){
		leaf_result=0;
	}
	else{
		leaf_result=1;
	}
	for(i=1;i<23;i++){
		if(!attribute[i])
			break;
	}
	if(i==23 || positive_ex==0||negative_ex==0){
		parent->N=-1;
		parent->leaf_result=leaf_result;
		return;
	}
	
	p=p/parent->N;
	if(p!=0 && p!=1)
	entropy=p*(-log2(p))-((1-p)*(log2(1-p)));
	else
		entropy=0;
		
	
	
	
	
	
	
	
	
	
	
	
	
	int freq[2][6],mis_max[2];
	char mis_char[2];
	for(j=0;j<6;j++){
		freq[0][j]=0;
		freq[1][j]=0;
		}
	for(i=0;i<parent->N;i++){
		if(input[examples[i]][11]!='?' && input[examples[i]][0]!='e'){
			if(input[examples[i]][11]=='b')
				freq[0][0]++;
			else if(input[examples[i]][11]=='c')
				freq[0][1]++;
			else if(input[examples[i]][11]=='u')
				freq[0][2]++;
			else if(input[examples[i]][11]=='e')
				freq[0][3]++;
			else if(input[examples[i]][11]=='z')
				freq[0][4]++;
			else if(input[examples[i]][11]=='r')
				freq[0][5]++;
		}
		if(input[examples[i]][11]!='?' && input[examples[i]][0]!='p'){
			if(input[examples[i]][11]=='b')
				freq[1][0]++;
			else if(input[examples[i]][11]=='c')
				freq[1][1]++;
			else if(input[examples[i]][11]=='u')
				freq[1][2]++;
			else if(input[examples[i]][11]=='e')
				freq[1][3]++;
			else if(input[examples[i]][11]=='z')
				freq[1][4]++;
			else if(input[examples[i]][11]=='r')
				freq[1][5]++;
		}
	}
	mis_max[0]=max(freq[0][0],max(freq[0][1],max(freq[0][2],max(freq[0][3],max(freq[0][4],freq[0][5])))));
	mis_max[1]=max(freq[1][0],max(freq[1][1],max(freq[1][2],max(freq[1][3],max(freq[1][4],freq[1][5])))));
	if(mis_max[0]==freq[0][0])
		mis_char[0]='b';
	else if(mis_max[0]==freq[0][1])
		mis_char[0]='c';
	else if(mis_max[0]==freq[0][2])
		mis_char[0]='u';
	else if(mis_max[0]==freq[0][3])
		mis_char[0]='e';
	else if(mis_max[0]==freq[0][4])
		mis_char[0]='z';
	else if(mis_max[0]==freq[0][5])
		mis_char[0]='r';
	
	if(mis_max[1]==freq[1][0])
		mis_char[1]='b';
	else if(mis_max[1]==freq[1][1])
		mis_char[1]='c';
	else if(mis_max[1]==freq[1][2])
		mis_char[1]='u';
	else if(mis_max[1]==freq[1][3])
		mis_char[1]='e';
	else if(mis_max[1]==freq[1][4])
		mis_char[1]='z';
	else if(mis_max[1]==freq[1][5])
		mis_char[1]='r';
	
	
	rep1=mis_char[0];
	rep2=mis_char[1];
	
	
	
	
	
	
	
	
	
	mn=-1;
	for(i=1;i<23;i++){
		if(attribute[i])
			continue;
		r=IGain(examples,i,entropy);
		
		if(r>mn ){
		mn=r;
		min_index=i;
		}
	}
	
	bool *att2= new bool[23];
	
	for(i=0;i<23;i++)
		att2[i]=attribute[i];
	

	att2[min_index]=true;
	parent->attribute=min_index;
	//cout<<min_index<<endl;
	if(mn<0){
	parent->N=-1;
	parent->leaf_result=leaf_result;
	return;
	}
	
	for(i=0;i<values[min_index];i++){
		
		vector<int> v;
		v.clear();
		k=0;
		for(j=0;j<parent->N;j++){
			if(input[examples[j]][min_index]==symbol[min_index][i]){
			v.push_back(examples[j]);
			k++;
			}
		
		}
		
		parent->children[i]=new node;
		if(k==0){
			parent->children[i]->N=-1;
			parent->children[i]->leaf_result=leaf_result;
			continue;
		}
		parent->children[i]->N=k;
		ID3(parent->children[i],v,att2);
	}
	
	
}

int ParseID3(node *parent,int test_index){
	if(parent->N==-1){
		return parent->leaf_result;
	}
	int k;
	k=parent->attribute;
	for(int i=0;i<values[k];i++){
		if(input[test_index][k]==symbol[k][i]){
			return ParseID3(parent->children[i],test_index);
	}
	}
	int ed=0,pd=0;
	for(int i=0;i<values[k];i++){
		
		if(ParseID3(parent->children[i],test_index)==0)
			ed++;
		else
			pd++;
	
	}
	if(ed>=pd)
		return 0;
	return 1;
}

int main(){
	int i,j,k,r;
	float average_accuracy;
	srand(time(NULL));
	symbol[0][0]='e';
	symbol[0][1]='p';
	
	symbol[1][0]='b';
	symbol[1][1]='c';
	symbol[1][2]='x';
	symbol[1][3]='f';
	symbol[1][4]='k';
	symbol[1][5]='s';
	
	symbol[2][0]='f';
	symbol[2][1]='g';
	symbol[2][2]='y';
	symbol[2][3]='s';
	
	symbol[3][0]='n';
	symbol[3][1]='b';
	symbol[3][2]='c';
	symbol[3][3]='g';
	symbol[3][4]='r';
	symbol[3][5]='p';
	symbol[3][6]='u';
	symbol[3][7]='e';
	symbol[3][8]='w';
	symbol[3][9]='y';
	
	symbol[4][0]='t';
	symbol[4][1]='f';
	
	symbol[5][0]='a';
	symbol[5][1]='l';
	symbol[5][2]='c';
	symbol[5][3]='y';
	symbol[5][4]='f';
	symbol[5][5]='m';
	symbol[5][6]='n';
	symbol[5][7]='p';
	symbol[5][8]='s';
	
	symbol[6][0]='a';
	symbol[6][1]='d';
	symbol[6][2]='f';
	symbol[6][3]='n';
	
	symbol[7][1]='c';
	symbol[7][2]='w';
	symbol[7][3]='d';
	
	symbol[8][0]='b';
	symbol[8][1]='n';
	
	symbol[9][0]='k';
	symbol[9][1]='n';
	symbol[9][2]='b';
	symbol[9][3]='h';
	symbol[9][4]='g';
	symbol[9][5]='r';
	symbol[9][6]='o';
	symbol[9][7]='p';
	symbol[9][8]='u';
	symbol[9][9]='e';
	symbol[9][10]='w';
	symbol[9][11]='y';
	
	symbol[10][0]='e';
	symbol[10][1]='t';
	
	symbol[11][0]='b';
	symbol[11][1]='c';
	symbol[11][2]='u';
	symbol[11][3]='e';
	symbol[11][4]='z';
	symbol[11][5]='r';
	
	symbol[12][0]='f';
	symbol[12][1]='y';
	symbol[12][2]='k';
	symbol[12][3]='s';
	
	symbol[13][0]='f';
	symbol[13][1]='y';
	symbol[13][2]='k';
	symbol[13][3]='s';
	
	symbol[14][0]='n';
	symbol[14][1]='b';
	symbol[14][2]='c';
	symbol[14][3]='g';
	symbol[14][4]='o';
	symbol[14][5]='p';
	symbol[14][6]='e';
	symbol[14][7]='w';
	symbol[14][8]='y';
	
	symbol[15][0]='n';
	symbol[15][1]='b';
	symbol[15][2]='c';
	symbol[15][3]='g';
	symbol[15][4]='o';
	symbol[15][5]='p';
	symbol[15][6]='e';
	symbol[15][7]='w';
	symbol[15][8]='y';
	
	symbol[16][0]='p';
	symbol[16][1]='u';
	
	symbol[17][0]='n';
	symbol[17][1]='o';
	symbol[17][2]='w';
	symbol[17][3]='y';
	
	symbol[18][0]='n';
	symbol[18][1]='o';
	symbol[18][2]='t';
	
	symbol[19][0]='c';
	symbol[19][1]='e';
	symbol[19][2]='f';
	symbol[19][3]='l';
	symbol[19][4]='n';
	symbol[19][5]='p';
	symbol[19][6]='s';
	symbol[19][7]='z';
	
	symbol[20][0]='k';
	symbol[20][1]='n';
	symbol[20][2]='b';
	symbol[20][3]='h';
	symbol[20][4]='r';
	symbol[20][5]='o';
	symbol[20][6]='u';
	symbol[20][7]='w';
	symbol[20][8]='y';
	
	symbol[21][0]='a';
	symbol[21][1]='c';
	symbol[21][2]='n';
	symbol[21][3]='s';
	symbol[21][4]='v';
	symbol[21][5]='y';
	
	symbol[22][0]='g';
	symbol[22][1]='l';
	symbol[22][2]='m';
	symbol[22][3]='p';
	symbol[22][4]='u';
	symbol[22][5]='w';
	symbol[22][6]='d';
	
	ifstream f;
	string s;
	f.open("data.txt");
	i=0;
	vector<string> imp;
	imp.clear();
	I=0;
	while(!f.eof()){
		f>>s;
		imp.push_back(s);
		I++;
	}
	f.close();
	i=0;
	while(i<I){
		s=imp[i];
		for(j=0;j<45;j+=2)
			input[i][j/2]=s[j];
		i++;
	}
	vector<int> fold[5];
	for(i=0;i<I;i++){
		float ms=(1.0*rand())/RAND_MAX;
		if(ms<=0.2)
			fold[0].push_back(i);
		else if(ms<=0.4)
			fold[1].push_back(i);
		else if(ms<=0.6)
			fold[2].push_back(i);
		else if(ms<=0.8)
			fold[3].push_back(i);
		else
			fold[4].push_back(i);
	} 

	vector<int> examples,test_set;	
	average_accuracy=0;
	cout<<"Results of 5 fold varification (without noise) : "<<endl;
	for(int num_fold=0;num_fold<5;num_fold++){
	node *root;
	root=new node;
	
	bool attribute[23];
	for(i=0;i<23;i++)
		attribute[i]=false;
		
	examples.clear();
	test_set.clear();
	

	if(num_fold==0){
		for(j=0;j<fold[1].size();j++)
			examples.push_back(fold[1][j]);
		for(j=0;j<fold[2].size();j++)
			examples.push_back(fold[2][j]);
		for(j=0;j<fold[3].size();j++)
			examples.push_back(fold[3][j]); 
		for(j=0;j<fold[4].size();j++)
			examples.push_back(fold[4][j]);  
		for(j=0;j<fold[0].size();j++)
			test_set.push_back(fold[0][j]);
		TR=examples.size();
	}
	else if(num_fold==1){
		for(j=0;j<fold[0].size();j++)
			examples.push_back(fold[0][j]);
		for(j=0;j<fold[2].size();j++)
			examples.push_back(fold[2][j]);
		for(j=0;j<fold[3].size();j++)
			examples.push_back(fold[3][j]); 
		for(j=0;j<fold[4].size();j++)
			examples.push_back(fold[4][j]);
		for(j=0;j<fold[1].size();j++)
			test_set.push_back(fold[1][j]);
		TR=examples.size();
		//cout<<TR<<endl;
	}
	else if(num_fold==2){
		for(j=0;j<fold[1].size();j++)
			examples.push_back(fold[1][j]);
		for(j=0;j<fold[0].size();j++)
			examples.push_back(fold[0][j]);
		for(j=0;j<fold[3].size();j++)
			examples.push_back(fold[3][j]);
		for(j=0;j<fold[4].size();j++)
			examples.push_back(fold[4][j]); 
		for(j=0;j<fold[2].size();j++)
			test_set.push_back(fold[2][j]);
		TR=examples.size();
		//cout<<TR<<endl;
	}
	else if(num_fold==3){
		for(j=0;j<fold[1].size();j++)
			examples.push_back(fold[1][j]);
		for(j=0;j<fold[2].size();j++)
			examples.push_back(fold[2][j]);
		for(j=0;j<fold[0].size();j++)
			examples.push_back(fold[0][j]);
		for(j=0;j<fold[4].size();j++)
			examples.push_back(fold[4][j]);
		for(j=0;j<fold[3].size();j++)
			test_set.push_back(fold[3][j]);
		TR=examples.size();
		//cout<<TR<<endl;
	}
	
	else if(num_fold==4){
		for(j=0;j<fold[1].size();j++)
			examples.push_back(fold[1][j]);
		for(j=0;j<fold[2].size();j++)
			examples.push_back(fold[2][j]);
		for(j=0;j<fold[3].size();j++)
			examples.push_back(fold[3][j]);
		for(j=0;j<fold[0].size();j++)
			examples.push_back(fold[0][j]);
		for(j=0;j<fold[4].size();j++)
			test_set.push_back(fold[4][j]);
		TR=examples.size();
		//cout<<TR<<endl;
	}
	
	root->N=TR;

	
	ID3(root,examples,attribute);
	float correct=0,total=0;
	for(i=0;i< test_set.size();i++){
		total++;
		r=ParseID3(root,test_set[i]);
		if((input[test_set[i]][0]=='e'&& r==0) || (input[test_set[i]][0]=='p' && r==1))
			correct++;
	}
	cout<<"Accuracy of  varification "<<num_fold +1 <<" = "<<(correct/total)*100<<"%"<<endl;
	average_accuracy+=correct/total;
	}
	cout<<"Average Accuracy over 5 fold varification = "<<(average_accuracy/5)*100<<"%"<<endl;
	float noise;
	
	
	
	cout<<"Enter the percentage of noise you want to add  ";
	cin>>noise;
	noise=noise/100;

		
	node *root;
	root=new node;
	examples.clear();
	test_set.clear();
	
	bool attribute[23];
	for(i=0;i<23;i++)
		attribute[i]=false;   
		
		for(j=0;j<fold[0].size();j++)
			examples.push_back(fold[0][j]);
		for(j=0;j<fold[2].size();j++)
			examples.push_back(fold[2][j]);
		for(j=0;j<fold[3].size();j++)
			examples.push_back(fold[3][j]); 
		for(j=0;j<fold[4].size();j++)
			examples.push_back(fold[4][j]);
		for(j=0;j<fold[1].size();j++)
			test_set.push_back(fold[1][j]);
		TR=examples.size();
		
	for(j=0;j<TR;j++){
			float rt=(1.0*rand())/RAND_MAX;
			if(rt<noise){
				if(input[examples[j]][0]=='p')
					input[examples[j]][0]='e';
				else
					input[examples[j]][0]='p';
			}
		} 
	root->N=examples.size();	
	ID3(root,examples,attribute);
	float correct=0,total=0;
	for(i=0;i< test_set.size();i++){
		total++;
		r=ParseID3(root,test_set[i]);
		if((input[test_set[i]][0]=='e'&& r==0) || (input[test_set[i]][0]=='p' && r==1))
			correct++;
	}
	cout<<"Accuracy of  varification(with noise) = "<<(correct/total)*100<<"%"<<endl;
	
}
