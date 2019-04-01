#include <iostream>
#include<bits/stdc++.h>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
#define MAX_LINE_LEN 500
#define MAX_CAR 80000
#define MAX_CANNAL 50
#define MAX_LENGTH 50
#define MAX_Dir 2
#define PRINT   printf
#define MAX_D 4
#define INF 10000000
//车辆id，始发地、目的地、最高速度、计划出发时间
struct Car{
 int id;
 int org;
 int dst;
 int speed;
 int stime;
 bool operator < (const Car & s2) const {//符号重载 
            return id<s2.id;
    }

};

struct Pcar{
  int id;
  int org;
  int dst;
  int speed;
  int stime;
  int length;
  friend bool operator <(Pcar a,Pcar b){
  	
  	if(a.stime==b.stime&&a.speed==b.speed&&a.length==b.length) return a.id>b.id;

	else if(a.stime==b.stime&&a.speed==b.speed) return a.length>b.length;
        else if(a.stime==b.stime) return a.speed<b.speed;
	else  return a.stime>b.stime;
	  	
  }	
};

priority_queue<Pcar> Scar[MAX_LINE_LEN];
priority_queue<Pcar> Dcar;
//道路id，道路长度，最高限速，车道数目，起始点id，终点id，是否双向
//"#(id,length,speed,channel,form,to,isDuplex)
struct Road{
 int id;
 int length;
 int speed;
 int channel;
 int from;
 int to;
 int isDuplex;
 bool operator < (const Road & s2) const {//符号重载 
            return id<s2.id;
    }
  // queue<int> stl[MAX_LINE_LEN];  
  // int roadmodel[MAX_LINE_LEN][MAX_LINE_LEN];
};

//路口上右下左顺时针连接 
struct Cross{
 int id;
 int link[MAX_D];
 bool operator < (const Cross & s2) const {//符号重载 
            return id<s2.id;
    }
};


//地图简化 
//限速，车道数，车道长度； 
struct Graph{
 int  node;	
 int speed;	
 int   id;
 int length;
//int carInroad[MAX_LINE_LEN][MAX_LINE_LEN]; 
}; 
	
Car car[MAX_CAR];
Road road[MAX_LINE_LEN];
Cross cross[MAX_LINE_LEN];
int graph[MAX_LINE_LEN][MAX_LINE_LEN];
//读取文件 
int Link[MAX_LINE_LEN][MAX_D];
int read_file(char ** const buff, const unsigned int spec, const char * const filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
    	PRINT("Fail to open file %s, %s.\n", filename, strerror(errno));
        return 0;
    }
    PRINT("Open file %s OK.\n", filename);

    char line[MAX_LINE_LEN + 2];
    unsigned int cnt = 0;
    while ((cnt < spec) && !feof(fp))
    {
        line[0] = 0;
        if (fgets(line, MAX_LINE_LEN + 2, fp) == NULL)  continue;
        if (line[0] == 0)   continue;
        buff[cnt] = (char *)malloc(MAX_LINE_LEN + 2);
        strncpy(buff[cnt], line, MAX_LINE_LEN + 2 - 1);
        buff[cnt][MAX_LINE_LEN + 1] = 0;
        cnt++;
    }
    fclose(fp);
    PRINT("There are %d lines in file %s.\n", cnt, filename);

    return --cnt;
}
//字符串处理 
char * charToNum(char * str, int& target)
{
	int sum = 0;int ad=1;
	while((((*str)<48)||((*str) >57))&& ((*str) != 0)){
	  if(*str==45) ad=0;
	str++;}
	
//	cout<<*str<<" "<<endl;
	while(((*str)>=48)&& ((*str) != 0)&& ((*str) <=57))
	{	
		sum = sum * 10 + ((*str) - '0');
		str++;
	}
	
	target = sum;
	if(ad==0) target=-target;
	return ++str;
}

string to_String(int n)
{
    int m=n;
	int i=0,j=0;
	char s[MAX_LINE_LEN ];
	char ss[MAX_LINE_LEN];
	while(m>0)
	{
		s[i++]= m%10 + '0';
		m/=10;
	}
	s[i]='\0';
 
	i=i-1;
	while(i>=0)
	{
		ss[j++]=s[i--];
	}
	ss[j]='\0';
 
	return ss;
}
//
int Road_num,Car_num,Cross_num;
char *RoadIO[MAX_LINE_LEN], *CarIO[MAX_CAR],*CrossIO[MAX_LINE_LEN];

map<int,int> Mroad;
map<int,int> Mcar;
map<int,int> Mcross;
map<int,int> Morg;
map<int,int> Mdst;
int gn=0;
int Clock=1;

int Cnd=336;
int Tnt=7;
int nmd=1;
void Io_mg() {

    //道路初始化 
    for(int i=1;i<=Road_num;i++)
    {
    //	cout<<RoadIO[i]<<endl;
    	char *charIndex = NULL;
        charIndex = charToNum(RoadIO[i], road[i].id);
		charIndex = charToNum(charIndex, road[i].length);
		charIndex = charToNum(charIndex, road[i].speed);
		charIndex = charToNum(charIndex, road[i].channel);
		charIndex = charToNum(charIndex, road[i].from);
		charIndex = charToNum(charIndex, road[i].to);
		charIndex = charToNum(charIndex, road[i].isDuplex);
	  //	Qroad.push(road[i]);
	}	
	//车辆初始化

	for(int i=1;i<=Car_num;i++)
    {
    //	cout<<CarIO[i]<<endl;
    	char *charIndex = NULL;
        charIndex = charToNum(CarIO[i],  car[i].id);
		charIndex = charToNum(charIndex, car[i].org);
		charIndex = charToNum(charIndex, car[i].dst);
		charIndex = charToNum(charIndex, car[i].speed);
		charIndex = charToNum(charIndex, car[i].stime);
	//	Qcar.push(car[i]);

	}	
	
	for(int i=1;i<=Cross_num;i++)
    {
    //	cout<<CrossIO[i]<<endl;
    	char *charIndex = NULL;
    	charIndex = charToNum(CrossIO[i],cross[i].id);
        charIndex = charToNum(charIndex, cross[i].link[0]);
		charIndex = charToNum(charIndex, cross[i].link[1]);
		charIndex = charToNum(charIndex, cross[i].link[2]);
		charIndex = charToNum(charIndex, cross[i].link[3]);
 }

	sort(road,road+Road_num);
	sort(car,car+Car_num);
	sort(cross,cross+Cross_num);
	for(int i=1;i<=Road_num;i++)
    {
       Mroad.insert(pair<int,int>(road[i].id,i));
	}
	
	for(int i=1;i<=Car_num;i++)
    {
       Mcar.insert(pair<int,int>(car[i].id,i));
	}
	for(int i=1;i<=Cross_num;i++)
    {
       Mcross.insert(pair<int,int>(cross[i].id,i));
	}
  


}
//构图
int n;
int Vroad[MAX_LINE_LEN][MAX_LINE_LEN];
int Lroad[MAX_LINE_LEN][MAX_LINE_LEN];
int V[MAX_LINE_LEN][MAX_LINE_LEN];

  void BGraph(){
 	for(int i=1;i<=Cross_num;i++)
  	  for(int j=1;j<=Cross_num;j++)
  	    Lroad[i][j]=INF;
  	
	 memset(graph,0,sizeof(graph));
	for(int i=1;i<=Road_num;i++){
	 //int f=road[i].from,t=road[i].to;
	 int f =Mcross[road[i].from],t = Mcross[road[i].to];
	    if(road[i].isDuplex){
	      graph[f][t]=i;
	      graph[t][f]=i;
	      Vroad[t][f]=road[i].length*road[i].channel*2/3;
	      Vroad[f][t]=road[i].length*road[i].channel*2/3;
             //  Vroad[t][f]=road[i].length;
             // Vroad[f][t]=road[i].length;
	      Lroad[t][f]=road[i].length;
	      Lroad[f][t]=road[i].length;
	  }
	  else{
       graph[f][t]=i;
       Vroad[f][t]=road[i].length*road[i].channel*2/3;
       // Vroad[f][t]=road[i].length;
       Lroad[f][t]=road[i].length;
	  	 }
}                

}
	  

int visited[MAX_LINE_LEN];
int link[MAX_LINE_LEN][MAX_LINE_LEN][MAX_LINE_LEN];
int f[MAX_LINE_LEN];

void Rest(){
 for(int i=1;i<=Cross_num;i++)
  for(int j=1;j<=Cross_num;j++)
    V[i][j]=Vroad[i][j];	  	    
}

int flag=0; 
void   BFS(int s,int d){	
  memset(visited,0,sizeof(visited));	
  memset(f,0,sizeof(f));
   visited[s]=1; 
   queue<int> q;
    flag=0; 
   q.push(s); 
while(!q.empty()) 
{ 
   int top = q.front(); 
   q.pop(); 
   int i,k=0;  
   for(i=1;i<=Cross_num;++i) 
   { 
   if(visited[i] == 0 && graph[top][i]!=0&&i==d)
   {
   	 flag=1;
   	f[i]=top;
   	break;
   }
    if(visited[i] == 0 && graph[top][i]!=0) 
    { 
     visited[i] = 1; 
     link[s][top][i]=1;
     f[i]=top;
     q.push(i); 
    } 
   } 
} 		
}
struct Vcar{
 int node;
 int vol;
 int dis;
 int value;
 int level;
 friend bool operator < (Vcar a, Vcar b){
   if(a.vol==b.vol&&a.level==b.level) return a.dis>b.dis;
   else if(a.level==b.level) return a.vol<b.vol;  
   else  return a.level>b.level;
 }
};

void Astar(int s,int d){
  memset(visited,0,sizeof(visited));	
  memset(f,0,sizeof(f));	
   visited[s]=1; 
   priority_queue<Vcar> q;
   Vcar tmp;
   tmp.node=s;
   tmp.dis=Lroad[s][d];
   tmp.vol=INF;
   tmp.level=0;
    flag=0; 
   q.push(tmp); 
   int cnt=0;
while(!q.empty()) 
{ 
   Vcar top = q.top(); 
   //cout<<top.node<<" "<<top.vol<<"|";
   q.pop(); 
 //  int d=q.size();

   int i,k=0;  
   for(i=1;i<=Cross_num;++i) 
   { 
   if(visited[i] == 0 && graph[top.node][i]!=0&&i==d&&V[top.node][i]>=1)
   {
   	 flag=1;
   	f[i]=top.node;
   	  return;
   }
    if(visited[i] == 0 && graph[top.node][i]!=0&&V[top.node][i]>=1) 
    { 
     visited[i] = 1; 
     f[i]=top.node;
     Vcar tmp;
     tmp.vol=V[top.node][i];
     tmp.node=i;
     tmp.dis=Lroad[i][d];
     tmp.level=top.level+1;
   //  tmp.value=tmp.vol*20+tmp.dis;
     q.push(tmp); 
   //  if(V[top.node][i]-1>0) 
    // V[top.node][i]--;
     //else{
	  //flag=0;
	 //return ;
//}
    } 
   } 
} 			
	
	
}

void Floyd(){

   for(int i=1;i<=n;i++)
    for(int j=1;j<=n;j++)
     for(int k=1;k<=n;k++){ 
       if(Lroad[i][j]>(Lroad[i][k]+Lroad[k][j])){
         Lroad[i][j]=Lroad[i][k]+Lroad[k][j];
       } 
    } 	
}

//int Cnd=336;
//int Tnt=7;
//int nmd=1;
int main(int argc, char *argv[]) { 
	char *Road_file = argv[2];
    char *Car_file = argv[1];
    char *Cross_file = argv[3];
    char *Answer_file = argv[4];
    ofstream fout(Answer_file); 
    Road_num = read_file(RoadIO, MAX_LINE_LEN, Road_file);
    Car_num = read_file(CarIO,MAX_CAR, Car_file);
    Cross_num = read_file(CrossIO, MAX_LINE_LEN, Cross_file); 
    
    Io_mg();
    
    BGraph();
    
    n=Cross_num;
      Floyd();
      
      //cout<<"before bg"<<endl;
     // cout<<Lroad[1][16]<<endl;
     int countzhx=0;
	for(int i=1;i<=Car_num;i++)
      {
        countzhx++;//cout<<countzhx<<endl;
          Pcar tmpc;
		 tmpc.id=car[i].id;
		 tmpc.org=Mcross[car[i].org];
		 tmpc.dst=Mcross[car[i].dst];
		 tmpc.stime=car[i].stime;
		 tmpc.speed=car[i].speed;
		 tmpc.length=Lroad[tmpc.org][tmpc.dst];
		 Scar[tmpc.org].push(tmpc);
		 Dcar.push(tmpc);
	}
	//cout<<"here"<<endl;
	Rest();
	
	int cnt=1;
	 while(!Dcar.empty()){
	 	cnt++;
	    int s,d;Pcar tmpc;
	     tmpc=Dcar.top();
	    s=tmpc.org;d=tmpc.dst;
		//BFS(s,d);
		Astar(s,d);
		 //if(flag==0)cout<<"warning"<<endl;
	//     if(flag==0&&gn==1&&nmd==1){Rest();Astar(s,d);Cnd+=49;Tnt--;nmd=0;}
	      if(flag==0){ Rest(); Astar(s,d);}
	      //if(cnt%331==0){Clock+=10;}//1-2152 + 2-2028
	      //if(cnt%60==0) Clock+=2;//1-2346
	      //if(cnt%330==0) Clock+=9;//1-1964 2-1857
	      //if(cnt%330==0) Clock+=8;//1-1782 2-1678
	     // if(cnt%336==0) Clock+=7;//1-1576
              // if(cnt%385==0) Clock+=6;//1-1597 2-1500

	     // else
	     if (cnt%Cnd==0)Clock+=Tnt;       //380 // 1-1210
	    //  if(cnt%280==0) Clock+=6;//new
	     // if(cnt%470==0)Clock+=10;
	      
             /* while(!Dcar.empty()){
	 	cnt++;
	    int s,d;Pcar tmpc;int j;
            for(j=1;j<=Cross_num;j++)
              if(!Scar[j].empty()){
	      tmpc=Scar[j].top();
	    s=tmpc.org;d=tmpc.dst;
		//BFS(s,d);
		Astar(s,d);
         if(flag==0)cout<<"warning"<<endl;
          if(flag==0){Rest();Astar(s,d);}
	   if(cnt%275==0) {
	     //Rest();
	     //Astar(s,d);
	     Clock+=10;
	    }*/
           

	   
	  vector<int> prin;
	   prin.push_back(d);
	   while(f[d]!=s){
	   //	cout<<f[d]<<" ";
       prin.push_back(f[d]);
       V[f[d]][d]--;
       d= f[d];		
	   }
	   V[s][d]--;
	   // cout<<endl; 
	   prin.push_back(s);
	 /* for(int i=prin.size()-1;i>=0;i--)
	     cout<<prin[i]<<" ";
	     cout<<endl;*/
  	 
	  	if(tmpc.stime>Clock)Clock=tmpc.stime;
  	    string ptout;
       string tmp;
       ptout+="(";
       ptout+=to_String(tmpc.id);
       ptout+=",";
       if(tmpc.stime<Clock)tmpc.stime=Clock;
        ptout+=to_String(tmpc.stime);
       // cout<<printout[i].size()<<endl;
       for(int i=prin.size()-1;i>0;i--)
         {
          ptout+=",";	
          ptout+=to_String(road[graph[prin[i]][prin[i-1]]].id);
		 }
        ptout+=")";
         ptout+="\n";
       	
	   fout<<ptout;
   	   Dcar.pop(); 
         //  Scar[j].pop();

}
	//Potime(Answer_file);	   

}
