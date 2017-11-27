#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;
//颜色代号 
const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
const int YELLOW = 3;

const int StartNum[4] = {0,39,26,13};//主干道上离该玩家起飞区域最近的一点 
const int WIN = -3;
const int AIRPORT = -2;
const int READY = -1;
//其他的为主干道 

const int AIRPORTW = 3000;//停机坪的权重 
const int READYW = 700;//起飞区的权重 
 
const int OTHERSW= 500;//其他的权重 

int AIColor;//当前移动棋子颜色 
int DiceResult;//骰子结果 
int pos[4][4]; //16个棋子的位置参数 
int factor;//当前棋盘局势对棋子的权重影响因子
 
int	GetWeightFromAllInfo(bool flag);
int Cal();
int CalW(int i);
 
 int main(int argc,char *argv[]){
 	//第一个参数，程序默认为ai的路径名 
 	//前面十六个是棋子位置，后面两个分别是当前移动棋子的颜色和骰子数		
	
	int cmd[4][4];//收集前16个命令参数 
	FILE*file;
	file=fopen(argv[1],"ab+"); 
	for(int i=0;i<4;i++)//将前16个参数都转化成整数 
		for(int j=0;j<4;j++)
			fscanf(file,"%d",&pos[i][j]);
		
	fscanf(file,"%d",&AIColor);//把字符型的数字所代表的颜色转化成整型数字代表的颜色 
	fscanf(file,"%d",&DiceResult); //把骰子结果存入DiceResult 
	fclose(file); 
	return Cal();//计算出应该移动棋子 和 对棋子的移动方式 
} 

const int CrashFactor = 30;
const int MoveStepFactor = 10;
const int DangerInOtherplacesFactor = -30;
int PosLast;//移动后的位置 
int CrashNum;//撞到敌机的数量 
int Jumped;
void  CalCrashNum() 
{
	int sum=0;
	for(int i=0;i<4;i++)
		if(i!=AIColor)
			for(int j=0;j<4;j++)
				if(pos[i][j]==PosLast)
					sum++;
	CrashNum+=sum;
} 
 
bool  GoBack(int MoveStep){
	if(PosLast== READY)
	{
		for(int j=0;j<4;j++)
			if(j!=AIColor)
				{
					int sum;
					for(int k=0;k<4;k++)
					{	sum=0;
						if(pos[j][k]>=StartNum[AIColor]&&pos[j][k]<StartNum[AIColor]+MoveStep-1)
							sum++;
							
					}
					if(sum>=2)
						return true;
				}
		return false; 

	} 
}
unsigned INF=0xffffffff;
int SmallestPos;
void GoBackPos1(int MoveStep)
{
	
	//前面堵的人的位置
	//自己的步数， 
	int ColorOfEnemy=-1,NumOfEnemy=-1;
	 if(PosLast == READY)
	{
		SmallestPos=INF;
		for(int j=0;j<4;j++)
			if(j!=AIColor)
				{
					int sum;
					for(int k=0;k<4;k++)
					{	sum=0;
						if(pos[j][k]>=StartNum[AIColor]&&pos[j][k]<StartNum[AIColor]+MoveStep-1)
							sum++;
					}
					if(sum>=2)//有可能反弹
					{
						
						for(int k=0;k<4;k++)
						{
							int SameNum=1;
							for(int m=k+1;m<4;m++)
								if(pos[j][k]==pos[j][m]) 
									SameNum++;	
							if(SameNum==1){}
							if(SameNum==2)//一定会反弹 
								if(SmallestPos>pos[j][k]) 
									SmallestPos=pos[j][k];
							if(SameNum>=3)
							{ 
								if(SmallestPos>pos[j][k]) 
									SmallestPos=pos[j][k];
								break;
							} 
						}	
					}
				}
		if(SmallestPos<INF)//反弹
		{
			PosLast=StartNum[AIColor]+MoveStep-1;		
			PosLast=2*SmallestPos-(PosLast+1);
			if(PosLast<0)
				PosLast=52+PosLast;
		}
	}	
}
bool NeedJump()
{
	if(Jumped == true)
		return false;
	switch(AIColor)
	{
		case GREEN:
			if((PosLast-0)%4==0&&PosLast!=36)
				return true;
			else return false;
			break;
		case RED:
			if((PosLast-1)%4==0&&PosLast!=49)
				return true;
			else return false;
			break;
		case YELLOW:
			if((PosLast-2)%4==0&&PosLast!=10)
				return true;
			else return false;
			break;
		case BLUE:
			if((PosLast-3)%4==0&&PosLast!=23)
				return true ;
			else return false;
			break;
	}
}

void GoBackPos2()//PosLast不在ready区 ,先走后跳再被挡住 ，计算此时的位置 
{
	SmallestPos=INF;
	for(int i=0;i<4;i++)
		if(i!=AIColor)
		{	
			int sum=0;
			for(int j=0;j<4;j++)
			{
				if(pos[i][j]<PosLast+4&&pos[i][j]>PosLast)
					sum++; 
			}
			if(sum>=2)
			{
				int SameNum;
				for(int k=0;k<4;k++)
				{	
					SameNum=1; 
					for(int m=k+1;m<4;m++)
						if(pos[i][k]==pos[i][m])
							SameNum++;
					if(SameNum==1){}
					if(SameNum==2)//一定会反弹 
						if(SmallestPos>pos[i][k]) 
							SmallestPos=pos[i][k];
					if(SameNum>=3)
					{ 
						if(SmallestPos>pos[i][k]) 
							SmallestPos=pos[i][k];
						break;
					}
				}
			} 
		}
	if(SmallestPos<INF)
	{
		PosLast=2*SmallestPos-PosLast-5;
		if(PosLast<0)
			PosLast=52+PosLast;
	} 
}
void IfMove()
{
	int MoveStep=DiceResult;
	Jumped=false;
	if(GoBack(MoveStep))//先反弹 
	{
		GoBackPos1(MoveStep);//退回时的位置 
		CalCrashNum();
		if(NeedJump)//后跳 
		{
			int Pos1=PosLast;
			Jumped=true;
			if(SmallestPos>PosLast+4)
				PosLast+=4;
			else
				PosLast=2*SmallestPos-5-PosLast;
			if(PosLast!=Pos1)
				CalCrashNum();
		}
	}
	else//未反弹 
	{
		PosLast=MoveStep+StartNum[AIColor]-1;
		
		CalCrashNum();
		
		if(NeedJump)
		{
			Jumped=true;
			MoveStep=4;
			if(GoBack( MoveStep))	
			{
				int Pos2=PosLast;
				GoBackPos2();
				if(PosLast!=Pos2)
					CalCrashNum();
			} 
		}
	}
}

//辅助dis函数 
int right(int num)
{
	if(num>=0)
		return num;
	else 
		return -num;
}//辅助DangerLast函数 
int dis(int pl1,int pl2)//求最终位置到敌方棋子的距离 在主干路上的!!! 
{
	int dis1=pl2-pl1;
	dis1=right(dis1);//取dis1的绝对值
	if(dis1 >  26)
		dis1=52-dis1;
	return dis1;
}

int DangerLast(int PosLast )
{
	int sum = 0;
	for(int j=0;j<4;j++)
		if(j!=AIColor)
			for(int k=0;k<4;k++)
			{
				if(pos[j][k]==WIN||pos[j][k]==AIRPORT||pos[j][k]==READY||pos[j][k]>=100){}//对起飞区来说，非主干道，不可能有威胁的时候 
				else
				{
					int distance=dis(pos[j][k],PosLast);
					if(distance<=10)
					{
						if(distance == 5||distance == 6)
							sum-= DangerInOtherplacesFactor*2;
						else 
							sum-=DangerInOtherplacesFactor; 
					}
				}
			}
	return sum; 
}

int CalFactor1(int i)// 当处于ready起飞区的时候 
{	
	PosLast = pos[AIColor][i];
	IfMove(); 
	int MoveStep = PosLast+1-StartNum[AIColor];
	return CrashNum*CrashFactor +MoveStep*MoveStepFactor+DangerLast(i); 
}






int DangerLastOfMain=0;
int PosLastOfMain;
int MoveStepOfMain;
int SmallestPosOfMain;
int CrashNumOfMain=0;
int DangerNum=0; 
//辅助CalFactor2（） 
//int DangerNum=0;
int CalFactorOfDanger(){//计算在superfly周围与之颜色对应的敌机 
	int PosSuperFly;
	switch(AIColor)
	{
		case GREEN:
			PosSuperFly=30;
			break;
		case RED:
			PosSuperFly=43;
			break;
		case BLUE:
			PosSuperFly=17;
			break;
		case YELLOW:
			PosSuperFly=4;
			break;
	} 
	int OtherColor;
	switch(PosSuperFly)
	{
		case 4://绿色		
			OtherColor=GREEN;
			break; 
		case 17://红色
			OtherColor=RED;
			break; 
		case 30://黄色
			OtherColor=YELLOW;
			break; 
		case 43://蓝色
			OtherColor=BLUE;
			break; 
	}
	for(int j = 0; j < 4; j++)
	{ 
		if(pos[OtherColor][j]==WIN||pos[OtherColor][j]==READY||pos[OtherColor][j]==AIRPORT){}
		else 
		{
			int distance = dis(pos[OtherColor][j],PosSuperFly);
			
			if(distance<=4&&distance>0)
				DangerNum++;
			if(distance>4&&distance<=6)
				DangerNum+=2;
			if(distance>6&&distance<=10)
				DangerNum+=2;
			
		}
	} 
}

void CalCrashNumOfMain()//撞机数 
{
	int sum=0;
	for(int j=0;j<4;j++)
		if(j!=AIColor)
			for(int k=0;k<4;k++)
			{
				if(pos[j][k]==PosLastOfMain)
					sum++;	
			}
	CrashNumOfMain+=sum;
}


bool  GoBackOfMain(int MoveStep)
{
	SmallestPosOfMain = INF;
	for(int j=0;j<4;j++)
			if(j!=AIColor)
				{
					int sum;
					for(int k=0;k<4;k++)
					{	sum=0;
						if(pos[j][k]>PosLastOfMain&&pos[j][k]<PosLastOfMain+MoveStep)
							sum++;
					}
					if(sum>=2)//有可能反弹
					{
						
						for(int k=0;k<4;k++)
						{
							int SameNum=1;
							for(int m=k+1;m<4;m++)
								if(pos[j][k]==pos[j][m]) 
									SameNum++;	
							if(SameNum==1){}
							if(SameNum==2)//一定会反弹 
								if(SmallestPosOfMain>pos[j][k]) 
									SmallestPosOfMain=pos[j][k];
							if(SameNum>=3)
							{ 
								if(SmallestPosOfMain>pos[j][k]) 
									SmallestPosOfMain=pos[j][k];
								break;
							} 
						}	
					}
				}
	if(SmallestPosOfMain<INF)
		return true;
	else
		return false;
} 
bool NeedJumpOfMain()
{
	if(Jumped == true)
		return false;
	switch(AIColor)
	{
		case GREEN:
			if((PosLastOfMain-0)%4==0&&PosLastOfMain!=36)
				return true;
			else return false;
			break;
		case RED:
			if((PosLastOfMain-1)%4==0&&PosLastOfMain!=49)
				return true;
			else return false;
			break;
		case YELLOW:
			if((PosLastOfMain-2)%4==0&&PosLastOfMain!=10)
				return true;
			else return false;
			break;
		case BLUE:
			if((PosLastOfMain-3)%4==0&&PosLastOfMain!=23)
				return true ;
			else return false;
			break;
	}
}
void CalMoveStepOfMain(int i)
{
	if(PosLastOfMain>=100)
		MoveStepOfMain=DiceResult;
	else 
	{
		int ans=PosLastOfMain-pos[AIColor][i];
		int distance= dis(pos[AIColor][i],PosLastOfMain);
		if(PosLastOfMain<26&&pos[AIColor][i]>26)
			MoveStepOfMain=distance;
		else if(ans<0)	
			MoveStepOfMain=-distance;
		else MoveStepOfMain = distance; 
	}
}
void IfMoveOfMain(int i)//考虑到的因素有1.支路，2.反弹，3.跳，4，撞， 
{
	int MoveStep=DiceResult;
	PosLastOfMain = pos[AIColor][i];
	PosLastOfMain+DiceResult;
	
	int ColorBrch ; 
	switch(AIColor) 
	{
		case GREEN:
			ColorBrch = 36;
			break;
		case RED:
			ColorBrch = 49;
			break;
		case BLUE:
			ColorBrch = 23;
			break;	
		case YELLOW:
			ColorBrch = 10;
			break;	
	}
	
	if(!GoBackOfMain( MoveStep ))//第一步走，没有反弹
	{	           
		
		if( PosLastOfMain<=ColorBrch&&PosLastOfMain+DiceResult>ColorBrch)//进入支路 
		{
			DangerLastOfMain=0;
			PosLastOfMain=(PosLastOfMain+DiceResult-ColorBrch)*100+ColorBrch; 
			if(PosLastOfMain/100==3)
			    DangerLastOfMain -= 100;//********************
			if(PosLastOfMain/100==6)
				DangerLastOfMain += 100; 
		} 
		else//在主干道上 
		{
			int PosPrev=PosLastOfMain; 
			CalCrashNumOfMain();
			if(NeedJumpOfMain())//跳 
			{
				if(GoBackOfMain(4))//反弹 
				{
					PosLastOfMain=SmallestPosOfMain-(4+PosLastOfMain-SmallestPosOfMain+1);
					if(PosLastOfMain<0)
						PosLastOfMain+=52;
					if(PosLastOfMain!=PosPrev)
						CalCrashNumOfMain();
				}
				else 
				{
					PosLastOfMain=(PosLastOfMain+4)%52;
					CalCrashNumOfMain();
				}
			}		
		}
	} 
	else//第一步有反弹， 
	{ 
		Jumped=false;
		PosLastOfMain=SmallestPosOfMain-(MoveStep+PosLastOfMain-SmallestPosOfMain+1);
		if(PosLastOfMain<0)
			PosLastOfMain=52+PosLastOfMain;
		CalCrashNumOfMain();
		int PosPrev=PosLastOfMain; 
		if(NeedJumpOfMain())//跳了一次 
		{
			if(GoBackOfMain(4))//反弹
			{
				PosLastOfMain=SmallestPosOfMain-(4+PosLastOfMain-SmallestPosOfMain+1);
				if(PosLastOfMain<0)
					PosLastOfMain=52+PosLastOfMain;
				if(PosLastOfMain!=PosPrev)
					CalCrashNumOfMain();
			} 
			else 
				CalCrashNumOfMain();
		}
	}
	CalMoveStepOfMain(i);
}

int  CalDangerLastOfMain(int i)
{
	//在主干道
	if(PosLastOfMain<100&&PosLastOfMain>=0)
	{
		
		for(int j=0;j<4;j++)
			if(j!=AIColor)
				for(int k=0;k<4;k++)
				{
					if(pos[j][k]==WIN||pos[j][k]==READY||pos[j][k]==AIRPORT){} 
					else
					{
						int ans = PosLastOfMain-pos[j][k];
						if(ans<0)
							ans=ans+52;
						switch(ans)
						{
							case 1:
								DangerLastOfMain+=DangerInOtherplacesFactor; 
								break;
							case 2:
								DangerLastOfMain+=DangerInOtherplacesFactor;
								break;	
							case 3:
								DangerLastOfMain+=DangerInOtherplacesFactor;
								break;
							case 4:
								DangerLastOfMain+=DangerInOtherplacesFactor;
								break;
							case 5:
								DangerLastOfMain+=DangerInOtherplacesFactor*2;
								break;
							case 6:
								DangerLastOfMain+=DangerInOtherplacesFactor*2;
								break;
							case 7:
								DangerLastOfMain+=DangerInOtherplacesFactor;
								break;
							case 8:
								DangerLastOfMain+=DangerInOtherplacesFactor;
								break;
							case 9:
								DangerLastOfMain+=DangerInOtherplacesFactor;
								break;
							case 10:
								DangerLastOfMain+=DangerInOtherplacesFactor;
								break;
							default:
								DangerLastOfMain+=0;
						}
					}	
				}
	}
	//在支路上
	if(PosLastOfMain>=100)
	{
		if(PosLastOfMain/100==3)
			return DangerLastOfMain-=CalFactorOfDanger();
		else return DangerLastOfMain;
	}		
}
int CalFactor2(int i)//当处于主干道或者支路时 
{
	if(pos[AIColor][i]>=100)//在支路时,加大到WIN的权重，200以上 ，加大danger的权重（尤其是当敌机在周围的superfly的可能比较大时 
	{
		int WeightInBranch=0; 
		int AddWeight = 0;
		if(pos[AIColor][i]/100==3)//当前位置在danger上时，权重增加AddWeight 
			AddWeight = 20;
		int result=pos[AIColor][i]/100+DiceResult;
		
		if(result>6)
			result=6-(result-6);
		switch(result)
		{
			case 1:
				WeightInBranch = 45;
				break;
			case 2:
				WeightInBranch = 50;
				break;
			case 3:
				//附近的危险因素:求在周围的对应颜色的棋子数和相应的概率， 
				 
				CalFactorOfDanger();
				
				WeightInBranch = 50 - DangerNum*10;
				break;
			case 4:
				WeightInBranch = 60;
				break;
			case 5:
				
				WeightInBranch = 65;
				break;
			case 6:
				WeightInBranch = 300;
				break;
		}
		return WeightInBranch + AddWeight;
	}
	else//在主干道上时，加大superfly的权重，加大撞飞敌机的权重，， 加大危险的权重， 
	{
		IfMoveOfMain(i);
		CalDangerLastOfMain(i);
		return CrashNumOfMain*CrashFactor +MoveStepOfMain*MoveStepFactor+DangerLastOfMain; 	
	}
}
int	GetWeightFromAllInfo(bool flag,int i)
{ 
	if(flag == true)
	{
		factor = CalFactor1(i); 
		return factor + READYW; 
	}
	else 
	{	
		factor = CalFactor2(i);
		return factor + OTHERSW;
	} 
}
 
int CalW(int i){//求该棋子的权重 
	switch(pos[AIColor][i])
	{
		case WIN:
			return 0;
			break;
		case AIRPORT:
			
			factor = 0;//简单版时设为0 
			
			if(DiceResult == 6)
				return AIRPORTW + factor;
			else 
				return 0;
			break;
		default:
			if(pos[AIColor][i] == READY)
				return GetWeightFromAllInfo(1,i);
			else
				return GetWeightFromAllInfo(0,i); 
	}
} 

int Cal()//求移动棋子的代号（个位）以及移动的方式（十位） 
{
	int w[4];
	
	for(int i = 0; i < 4; i ++ )//计算该颜色每个棋子应该移动的权重 
		w[i]=CalW(i);
	int maxw=w[0],k=0;//k为要移动的棋子代号 
	for(int i= 1; i < 4; i ++ )
		if(w[i]>maxw)
			k=i;
	int ten;//返回值命令的十位数 
	if( DiceResult==6&&pos[AIColor][k]==AIRPORT)
		ten = 2;
	else 
		ten = 1;
	return ten*10+k;	 
}
