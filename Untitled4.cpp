#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

int row,col,sx,sy,ex,ey; //長寬、起點座標、終點座標
bool a[51][51][4]={0},visit[51][51]={0}; //迷宮圖、檢測是否造訪過
int p1[6][4]={{1,-1,0,0},{1,0,-1,0},{1,0,0,-1},{1,-1,0,0},{1,0,-1,0},{1,0,0,-1}};
int p2[6][4]={{0,0,1,-1},{0,1,0,-1},{0,1,-1,0},{0,0,-1,1},{0,-1,0,1},{0,-1,1,0}}; //增加製造迷宮隨機性

void SetColor(int color=7) //改變字體顏色
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,color);
}

void color(int k)
{
    SetColor(k);
    cout << "  ";
    SetColor();
}

void print() //輸出迷宮樣貌
{

    for(int i=0;i<2*col+1;i++) color(119);
    cout << "\n";
    for(int i=0;i<row;i++)
    {
        color(119);
        for(int j=0;j<col;j++)
        {
            if(i==sx&&j==sy) color(170); //起點(綠色)
            else if(i==ex&&j==ey) color(204); //終點(紅色)
            else if(a[i][j][2]==1||a[i][j][3]==1||a[i-1][j][3]==1||a[i][j-1][2]==1) color(238);
            else cout << "  ";                                  //是否為最短路徑經過的地方(橘色)
            if(a[i][j][0]==1&&a[i][j][2]==0) cout << "  ";
            else if(a[i][j][0]==1&&a[i][j][2]==1) color(238);
            else color(119);
        }
        cout << "\n";
        for(int j=0;j<col;j++)
        {
            color(119);
            if(a[i][j][1]==1&&a[i][j][3]==0) cout << "  ";
            else if(a[i][j][1]==1&&a[i][j][3]==1) color(238);
            else color(119);
        }
        color(119);
        cout << "\n";
    }
}

void make(int x,int y) //製造迷宮
{
    int r=rand()%6,k=rand()%4;
    visit[x][y]=1;

    for(int i=k;i<k+4;i++)
    {
        int x1=x+p1[r][i%4],y1=y+p2[r][i%4];
        if(x1<row&&x1>=0&&y1<col&&y1>=0)
        {
            if(visit[x1][y1]==0)
            {
                if(x1-x==1) a[x][y][1]=1;
                else if(x1-x==-1) a[x1][y1][1]=1;
                else if(y1-y==1) a[x][y][0]=1;
                else a[x1][y1][0]=1;
                make(x1,y1);
            }
        }
    }
}

int d[4][2]={{-1,0},{1,0},{0,1},{0,-1}};
int step[51][51];

void bestroute(int x,int y) //紀錄最短路徑
{
    int nx,ny;
    stack <pair<int,int>> s;
    s.push(make_pair(x,y));
    while(!(s.empty()))
    {
        pair<int,int> cur= s.top();
        s.pop();
        int cx=cur.first;
        int cy=cur.second;
        if(step[cx][cy]==0) break;
        for(int i=0;i<4;i++)
        {
            nx=cx+d[i][0];
            ny=cy+d[i][1];
            if(nx>=row||nx<0||ny>=col||ny<0)continue;
            if(step[nx][ny]!=step[cx][cy]-1)continue;
            if((d[i][0]==-1&&a[nx][ny][1]==0)||(d[i][0]==1&&a[cx][cy][1]==0)||(d[i][1]==-1&&a[nx][ny][0]==0)||(d[i][1]==1&&a[cx][cy][0]==0)) continue;
            if(d[i][0]==-1||d[i][1]==-1) a[nx][ny][3-(d[i][0]+2)/2]=1;
            else a[cx][cy][2+(d[i][0]+1)/2]=1;
            s.push(make_pair(nx,ny));
        }
    }
}

void bfs(int x,int y) //尋找最短路徑
{
    queue<pair<int,int>> q;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++) step[i][j]=-1;
    }
    step[x][y]=0;
    pair<int,int> st =make_pair(x,y);
    q.push(st);
    while(!q.empty())
    {
        pair<int,int> cur= q.front();
        q.pop();
        int cx=cur.first;
        int cy=cur.second;
        if(cx==ex&&cy==ey)
        {
            bestroute(cx,cy);
            break;
        }
        for(int i=0;i<4;i++)
        {
            int cnx=cx+d[i][0],cny=cy+d[i][1];
            if(cnx>=row||cnx<0||cny>=col||cny<0)continue;
            if(step[cnx][cny]!=-1)continue;
            if((d[i][0]==-1&&a[cnx][cny][1]==0)||(d[i][0]==1&&a[cx][cy][1]==0)||(d[i][1]==-1&&a[cnx][cny][0]==0)||(d[i][1]==1&&a[cx][cy][0]==0)) continue;
            step[cnx][cny]=step[cx][cy]+1;
            q.push(make_pair(cnx,cny));
        }
    }
}


int main()
{
    srand(time(NULL));
    do
    {
        cin >> row >> col;
    } while(row%2==0||col%2==0||row>51||row<5||col>51||col<5);
    make(0,0);
    sx=rand()%row;
    sy=rand()%col;
    do
    {
        ex=rand()%row;
        ey=rand()%col;
    }while(ex==sx&&ey==sy);
    print();
    bfs(sx,sy);
    cout << "\n";
    print();
}
