#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

#define BLACK 0
#define WHITE 1
#define EMPTY 2

struct Point{ //锟斤拷峁?
	int x,y;
};
struct Step{ //锟斤拷锟结构
	Point first,second;
	int value;
};
int Board[19][19];//锟芥储锟斤拷锟斤拷锟斤拷息锟斤拷锟斤拷元锟斤拷值为 BLACK, WHITE, EMPTY 之一

int myRoadValue(int n) { //宸辨柟璺殑浠峰€?
    switch(n) {
    case 1:
        return 1; 
    case 2:
        return 20;
    case 3:
        return 40;
    case 4:
        return 200;
    case 5:
        return 200;
    case 6:
        return 1000000;
    default:
        return 0;
   }
}

int opRoadValue(int n) { //瀵规柟璺殑濞佽儊鍊?
    switch(n) {
    case 1:
        return 1; 
    case 2:
        return 25;
    case 3:
        return 50;
    case 4:
        return 6000;
    case 5:
        return 6000;
    case 6:
        return 1000000;
    default:
        return 0;
   }
}

int roadValue(int x, int y, int side) {
    int node[3], value = 0;
    for(int i=0;i<6;++i) {
        if(x-i>=0&&x+5-i<19) {
            for(int j=0;j<3;++j) node[j]=0;
            for(int j=0;j<6;++j) node[Board[x-i+j][y]]++;
            if(node[BLACK]!=0&&node[WHITE]!=0) value += 0;
            else if(node[1-side]!=0) value += opRoadValue(node[1-side]);
            else value += myRoadValue(node[side]+1) - myRoadValue(node[side]);
        }
        if(y-i>=0&&y+5-i<19) {
            for(int j=0;j<3;++j) node[j]=0;
            for(int j=0;j<6;++j) node[Board[x][y-i+j]]++;
            if(node[BLACK]!=0&&node[WHITE]!=0) value += 0;
            else if(node[1-side]!=0) value += opRoadValue(node[1-side]);
            else value += myRoadValue(node[side]+1) - myRoadValue(node[side]);
        }
        if(x-i>=0&&x+5-i<19&&y-i>=0&&y+5-i<19) {
            for(int j=0;j<3;++j) node[j]=0;
            for(int j=0;j<6;++j) node[Board[x-i+j][y-i+j]]++;
            if(node[BLACK]!=0&&node[WHITE]!=0) value += 0;
            else if(node[1-side]!=0) value += opRoadValue(node[1-side]);
            else value += myRoadValue(node[side]+1) - myRoadValue(node[side]);
        }
        if(x-i>=0&&x+5-i<19&&y+i<19&&y-5+i>=0) {
            for(int j=0;j<3;++j) node[j]=0;
            for(int j=0;j<6;++j) node[Board[x-i+j][y+i-j]]++;
            if(node[BLACK]!=0&&node[WHITE]!=0) value += 0;
            else if(node[1-side]!=0) value += opRoadValue(node[1-side]);
            else value += myRoadValue(node[side]+1) - myRoadValue(node[side]);
        }
    }
    return value;
}

int stepValue(int x1, int y1, int x2, int y2, int side) {
    if(x1==x2&&y1==y2) return -1;
    int value;
    value = roadValue(x1,y1,side);
    Board[x1][y1] = side;
    value += roadValue(x2,y2,side);
    Board[x1][y1] = EMPTY;
    return value;
}

bool dis(Step x1,Step x2) {
    return sqrt(pow(x1.first.x-9,2)+pow(x1.first.y-9,2))+sqrt(pow(x1.second.x-9,2)+pow(x1.second.y-9,2))<sqrt(pow(x2.first.x-9,2)+pow(x2.first.y-9,2))+sqrt(pow(x2.second.x-9,2)+pow(x2.second.y-9,2));
}

Step alphabeta(Step s, int depth, int width, int alpha, int beta, int side, int color) {
    int val;
    Step sort[width],step,step2,step3,tmp,v;
    if(depth == 0) {
        s.value=0;
        return s;
    }
    for(int i=0;i<width;++i) sort[i].value=0;
    for(int i=0;i<19*19-1;++i) for(int j=i+1;j<19*19;++j) {
            if(Board[i/19][i%19]!=EMPTY||Board[j/19][j%19]!=EMPTY) continue;
            val = stepValue(i/19,i%19,j/19,j%19,side);
            step3.value=val; step3.first.x=i/19; step3.first.y=i%19; step3.second.x=j/19; step3.second.y=j%19;
            if(val>sort[0].value||(val==sort[0].value&&dis(step3,sort[0]))) {
                step=sort[0];
                sort[0]=step3;
                for(int k=1;k<width;++k) {step2=sort[k]; sort[k]=step; step=step2;}
            } else if(val<sort[width-1].value||(val==sort[width-1].value&&dis(sort[width-1],step3))) continue;
            else {
                for(int k=1;k<width;++k) {
                    if((sort[k-1].value>=val&&sort[k].value<val)||(sort[k-1].value>=val&&sort[k].value==val&&dis(step3,sort[k]))) {
                        step=sort[k];
                        sort[k]=step3;
                        for(int m=k+1;m<width;++m) {step2=sort[m]; sort[m]=step; step=step2;}
                        break;
                    }
                }
            }
        }
    if(side==color) {
        v.value=-1000000;
        for(int i=0;i<width;++i) {
            Board[sort[i].first.x][sort[i].first.y]=side;
            Board[sort[i].second.x][sort[i].second.y]=side;
            tmp = sort[i];
            tmp.value=tmp.value+alphabeta(sort[i],depth-1,width,alpha,beta,1-side,color).value;
            Board[sort[i].first.x][sort[i].first.y]=EMPTY;
            Board[sort[i].second.x][sort[i].second.y]=EMPTY;
            v = v.value>=tmp.value?v:tmp;
            alpha = alpha>v.value?alpha:v.value;
            if(beta<=alpha) break;
        }
        return v;
    } else {
        v.value=1000000;
        for(int i=0;i<width;++i) {
            Board[sort[i].first.x][sort[i].first.y]=side;
            Board[sort[i].second.x][sort[i].second.y]=side;
            tmp = sort[i];
            tmp.value=-tmp.value+alphabeta(sort[i],depth-1,width,alpha,beta,1-side,color).value;
            Board[sort[i].first.x][sort[i].first.y]=EMPTY;
            Board[sort[i].second.x][sort[i].second.y]=EMPTY;
            v = v.value<=tmp.value?v:tmp;
            beta = beta<v.value?alpha:v.value;
            if(beta<=alpha) break;
        }
        return v;
    }
}

int main()
{
	Step step;//锟斤拷时锟斤拷锟结构
	char message[256];//通锟斤拷锟斤拷息锟斤拷锟斤拷
    int computerSide;//锟斤拷锟斤拷执锟斤拷锟斤拷色
    int start=0;//锟皆局匡拷始锟斤拷锟?
	srand(int(time(0)));
	//锟剿达拷锟斤拷锟矫筹拷始锟斤拷锟斤拷锟斤拷
    //...
    
	while (1)	//锟斤拷锟斤拷锟斤拷循锟斤拷
	{
		fflush(stdout);//锟斤拷要删锟斤拷锟斤拷锟斤拷洌拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
		scanf("%s", message);//锟斤拷取平台锟斤拷锟斤拷锟斤拷息
        //锟斤拷锟斤拷锟斤拷锟斤拷
		if (strcmp(message, "name?") == 0)//锟斤拷锟秸狡教拷锟斤拷投锟斤拷锟?
		{
			fflush(stdin);
			/***********锟斤拷"锟斤拷锟斤拷锟?锟斤拷为锟斤拷亩锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?锟斤拷锟斤拷锟街伙拷12锟斤拷英锟斤拷锟斤拷母锟斤拷锟斤拷锟斤拷锟睫成硷拷************/
			/*******/		printf("name CBB\n");		/**只锟睫革拷锟斤拷锟斤拷澹拷锟揭撅拷锟絥ame锟秸革拷****/
			/***********锟斤拷"锟斤拷锟斤拷锟?锟斤拷为锟斤拷亩锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?锟斤拷锟斤拷锟街伙拷12锟斤拷英锟斤拷锟斤拷母锟斤拷锟斤拷锟斤拷锟睫成硷拷************/
		}
		else if (strcmp(message, "new") == 0)//锟斤拷锟斤拷锟斤拷锟斤拷锟?
		{
			int i, j;
			scanf("%s", message);//锟斤拷取锟斤拷锟斤拷执锟斤拷锟斤拷色
			fflush(stdin);
			if (strcmp(message, "black") == 0)	computerSide = BLACK;  //执锟斤拷
			else  computerSide = WHITE;   //执锟斤拷

			for (i = 0; i<19; ++i)   //锟斤拷始锟斤拷锟斤拷锟?
				for (j = 0; j<19; ++j)
					Board[i][j] = EMPTY;
			start = 1;

			if (computerSide == BLACK)
			{
				/**********锟斤拷锟缴碉拷一锟斤拷锟脚凤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷step锟结构锟叫ｏ拷锟斤拷锟斤拷锟斤拷锟斤拷为(step.first.x,step.first.y)**********/
				/****************************锟斤拷锟铰凤拷锟斤拷锟斤拷锟诫，锟斤拷锟芥换锟揭碉拷示锟斤拷锟斤拷锟斤拷******************************/


				step.first.x = 9;
				step.first.y = 9;


				/******************************锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷锟?******************************************/

				Board[step.first.x][step.first.y] = computerSide;//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
				printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//锟斤拷锟斤拷欧锟?
			}
		}
		else if(strcmp(message,"move")==0)//锟斤拷锟斤拷,锟斤拷锟斤拷锟斤拷锟斤拷锟?
		{
			scanf("%s", message);//锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷锟脚凤拷
            fflush(stdin); 
			step.first.x=message[0]-'A';		step.first.y=message[1]-'A';
			step.second.x=message[2]-'A';		step.second.y=message[3]-'A';
            //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
			Board[step.first.x][step.first.y] = 1 - computerSide;
			if(!(step.second.x==-1 && step.second.y==-1)) Board[step.second.x][step.second.y] = 1 - computerSide;

            /**********************************************************************************************************/
			/***锟斤拷锟斤拷锟斤拷锟接碉拷锟斤拷锟疥，锟斤拷锟斤拷锟斤拷step锟结构锟叫ｏ拷锟斤拷1锟斤拷锟斤拷锟斤拷(step.first.x,step.first.y)锟斤拷锟斤拷2锟斤拷锟斤拷锟斤拷(step.first.x,step.first.y)*****/
			/**************************************锟斤拷锟铰凤拷锟斤拷锟斤拷锟诫，锟斤拷锟芥换锟揭碉拷示锟斤拷锟斤拷锟斤拷*****************************************/
			
			//锟斤拷锟缴碉拷1锟斤拷锟斤拷锟斤拷位锟斤拷step.first.x锟斤拷step.first.y
			int height=1,width=1;
    		step=alphabeta(step,height,width,-1000000,1000000,computerSide,computerSide);
			if(step.value==0) {
				for(int i=0;i<19*19-1;++i) for(int j=i+1;j<19*19;++j) {
					if(Board[i/19][i%19]!=EMPTY||Board[j/19][j%19]!=EMPTY) continue;
					step.first.x=i/19; step.first.y=i%19; step.second.x=j/19; step.second.y=j%19;
				}
			}
			Board[step.first.x][step.first.y] = computerSide;
			Board[step.second.x][step.second.y] = computerSide;
			
			/*****************************************锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷******************************************************/
			/**********************************************************************************************************/

			printf("move %c%c%c%c\n",step.first.x+'A',step.first.y+'A',step.second.x+'A',step.second.y+'A');//锟斤拷锟斤拷欧锟?
		}
        else if (strcmp(message, "error") == 0)//锟脚凤拷锟斤拷锟斤拷
        {
            fflush(stdin);
        }
		else if (strcmp(message, "end") == 0)//锟皆局斤拷锟斤拷
		{
            fflush(stdin);
			start = 0;
		}
		else if (strcmp(message, "quit") == 0)//锟剿筹拷锟斤拷锟斤拷
		{
            fflush(stdin);
			printf("Quit!\n");
			break;
		}
	}
	return 0;
}
