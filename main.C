#include<iostream>
#include<cmath>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<ctime>

using namespace std;

class action{
public:
  int x,y;
  action():x(0),y(0){}
  action(int xx, int yy):x(xx),y(yy){}
};

class status{
public:
  vector<int> s;
  status(){s=vector<int>(9,0);}
  bool j(status& in){
    for(int i=0;i<9;i++){
      if(s[i]!=in.s[i])return false;
    }
    return true;
  }
  int get(int i,int j){return s[i*3+j];}
};


//=============================================================
vector<vector<double>> Q(19683, vector<double>(9,0));
vector<vector<double>> Q1(19683, vector<double>(9,0));
vector<status> S;
vector<action> A;
double learning_ratio = 0.15;
double gama = 0.9;
double ei = 1;
//1 is black, -1 is white, 0 is empty
status chess;
//===============================================================
//Q[i][temm] += learning_ratio*(R()+gama*Qmax(chess)-Q[i][temm]);
void move(action a){
  chess.s[a.x*3+a.y]=1;
}
void move1(action a){
  chess.s[a.x*3+a.y]=-1;
}

double R(action a) {
  if(chess.get(a.x, a.y) !=0){chess.s = {0,0,0,0,0,0,0,0,0};return -1; }
  move(a);
  for(int i = 0; i < 3; i++) {
    if((chess.get(i,0)==1 && chess.get(i,1)==1 && chess.get(i,2)==1) ||(chess.get(0,i)==1 && chess.get(1,i)==1 && chess.get(2,i)==1)) {
      chess.s = {0,0,0,0,0,0,0,0,0};return 1;
    }
    if((chess.get(i,0)==-1 && chess.get(i,1)==-1 && chess.get(i,2)==-1) ||(chess.get(0,i)==-1 && chess.get(1,i)==-1 && chess.get(2,i)==-1)) {
      chess.s = {0,0,0,0,0,0,0,0,0};return -1;
    }
  }
  if((chess.get(0,0)==1 && chess.get(1,1)==1 && chess.get(2,2)==1) || (chess.get(0,2)==1 && chess.get(1,1)==1 && chess.get(2,0)==1)) {
    chess.s = {0,0,0,0,0,0,0,0,0};return 1;
  }
  if((chess.get(0,0)==-1 && chess.get(1,1)==-1 && chess.get(2,2)==-1) || (chess.get(0,2)==-1 && chess.get(1,1)==-1 && chess.get(2,0)==-1)) {
    chess.s = {0,0,0,0,0,0,0,0,0};return -1;
  }
  int y=0;
  for(int i=0;i<9;i++){
    if(chess.s[i]==0)y++;
  }
  if(y==0){chess.s = {0,0,0,0,0,0,0,0,0};return 0;}
  return -0.1; 
}
double R1(action a) {
  if(chess.get(a.x, a.y) !=0){chess.s = {0,0,0,0,0,0,0,0,0};return -1; }
  move1(a);
  for(int i = 0; i < 3; i++) {
    if((chess.get(i,0)==1 && chess.get(i,1)==1 && chess.get(i,2)==1) ||(chess.get(0,i)==1 && chess.get(1,i)==1 && chess.get(2,i)==1)) {
      chess.s = {0,0,0,0,0,0,0,0,0};return -1;
    }
    if((chess.get(i,0)==-1 && chess.get(i,1)==-1 && chess.get(i,2)==-1) ||(chess.get(0,i)==-1 && chess.get(1,i)==-1 && chess.get(2,i)==-1)) {
      chess.s = {0,0,0,0,0,0,0,0,0};return 1;
    }
  }
  if((chess.get(0,0)==1 && chess.get(1,1)==1 && chess.get(2,2)==1) || (chess.get(0,2)==1 && chess.get(1,1)==1 && chess.get(2,0)==1)) {
    chess.s = {0,0,0,0,0,0,0,0,0};return -1;
  }
  if((chess.get(0,0)==-1 && chess.get(1,1)==-1 && chess.get(2,2)==-1) || (chess.get(0,2)==-1 && chess.get(1,1)==-1 && chess.get(2,0)==-1)) {
    chess.s = {0,0,0,0,0,0,0,0,0};return 1;
  }
  int y=0;
  for(int i=0;i<9;i++){
    if(chess.s[i]==0)y++;
  }
  if(y==0){chess.s = {0,0,0,0,0,0,0,0,0};return 0;}
  return -0.1; 
}


int getindex(status s){
  for(int i=0;i<19683;i++){
    if(S[i].j(s))return i;
  }
  return -1;
}


double getbest_Q(){
  vector<double> tem = Q[getindex(chess)];
  return *max_element(tem.begin(), tem.end());
}
double getbest_Q1(){
  vector<double> tem = Q1[getindex(chess)];
  return *max_element(tem.begin(), tem.end());
}

int getbest_A(){
    int best_action = -1;
    double best_value = -1e9; // 很小的初始值
    for(int i=0; i<9; i++){
        if(chess.get(A[i].x, A[i].y) != 0) continue; // 非法动作跳过
        if(Q[getindex(chess)][i] > best_value){
            best_value = Q[getindex(chess)][i];
            best_action = i;
        }
    }
    return best_action;
}
int getbest_A1(){
    int best_action = -1;
    double best_value = -1e9; // 很小的初始值
    for(int i=0; i<9; i++){
        if(chess.get(A[i].x, A[i].y) != 0) continue; // 非法动作跳过
        if(Q1[getindex(chess)][i] > best_value){
            best_value = Q1[getindex(chess)][i];
            best_action = i;
        }
    }
    return best_action;
}
void print(){
  for(int i=0;i<3;i++){
    for(int ii=0;ii<3;ii++){
      cout<<(int)chess.get(i,ii)<<" ";
    }
    cout<<endl;
  }
}
//================================================================

int main(){
  for (int a = -1; a < 2; ++a)
  for (int b = -1; b < 2; ++b)
  for (int c = -1; c < 2; ++c)
  for (int d = -1; d < 2; ++d)
  for (int e = -1; e < 2; ++e)
  for (int f = -1; f < 2; ++f)
  for (int g = -1; g < 2; ++g)
  for (int h = -1; h < 2; ++h)
  for (int i = -1; i < 2; ++i) {
    status ss;
    ss.s = {
      a, b, c,
      d, e, f,
      g, h, i
    };
    S.push_back(ss);
  }

  for(int i=0;i<3;i++){
    for(int ii=0;ii<3;ii++){
      action t(i,ii);
      A.push_back(t);
    }
  }
  int epoch = 300000;
  srand(time(nullptr));
  while(epoch>1){
    //===============
    if(rand()%100<ei*100){
      int tem=rand()%9;
      int index = getindex(chess);
      double r = R(A[tem]);
      Q[index][tem] += learning_ratio*(r+gama*getbest_Q()-Q[index][tem]);
      //cout<<index<<" "<<tem<<endl;
    }
    //===============
    else
    {
      int tem=getbest_A();
      int index = getindex(chess);
      double r = R(A[tem]);
      Q[index][tem] += learning_ratio*(r+gama*getbest_Q()-Q[index][tem]);//cout<<index<<" "<<tem<<endl;
    }
    //===============
    if(rand()%100<ei*100){
      int tem=rand()%9;
      int index = getindex(chess);
      double r = R1(A[tem]);
      Q1[index][tem] += learning_ratio*(r+gama*getbest_Q()-Q1[index][tem]);
      //cout<<index<<" "<<tem<<endl;
    }
    //===============
    else
    {
      int tem=getbest_A();
      int index = getindex(chess);
      double r = R1(A[tem]);
      Q1[index][tem] += learning_ratio*(r+gama*getbest_Q()-Q1[index][tem]);//cout<<index<<" "<<tem<<endl;
    }
    epoch--;
    ei-=ei<0.2?0:0.0001;
  }
  
  for(int i=0;i<19683;i++){
    for(int ii=0;ii<9;ii++){
      cout<<Q[i][ii];
    }
    //cout<<endl;
  }chess.s = {0,0,0,0,0,0,0,0,0};
  
  
  
  while(true){
  int xx,yy;
  cout<<"You: ";
  cin>>xx>>yy;
  action t(xx,yy);
  move1(t);

  print();
  
  cout<<"AI: "<<getbest_A()<<endl;
  R(A[getbest_A()]);
  print();
  }
}
