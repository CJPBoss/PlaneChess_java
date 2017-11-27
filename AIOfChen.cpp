#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;
//��ɫ���� 
const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
const int YELLOW = 3;

const int StartNum[4] = {0,39,26,13};//���ɵ���������������������һ�� 
const int WIN = -3;
const int AIRPORT = -2;
const int READY = -1;
//������Ϊ���ɵ� 

const int AIRPORTW = 3000;//ͣ��ƺ��Ȩ�� 
const int READYW = 700;//�������Ȩ�� 
 
const int OTHERSW= 500;//������Ȩ�� 

int AIColor;//��ǰ�ƶ�������ɫ 
int DiceResult;//���ӽ�� 
int pos[4][4]; //16�����ӵ�λ�ò��� 
int factor;//��ǰ���̾��ƶ����ӵ�Ȩ��Ӱ������
 
int	GetWeightFromAllInfo(bool flag);
int Cal();
int CalW(int i);
 
 int main(int argc,char *argv[]){
 	//��һ������������Ĭ��Ϊai��·���� 
 	//ǰ��ʮ����������λ�ã����������ֱ��ǵ�ǰ�ƶ����ӵ���ɫ��������		
	
	int cmd[4][4];//�ռ�ǰ16��������� 
	FILE*file;
	file=fopen(argv[1],"ab+"); 
	for(int i=0;i<4;i++)//��ǰ16��������ת�������� 
		for(int j=0;j<4;j++)
			fscanf(file,"%d",&pos[i][j]);
		
	fscanf(file,"%d",&AIColor);//���ַ��͵��������������ɫת�����������ִ������ɫ 
	fscanf(file,"%d",&DiceResult); //�����ӽ������DiceResult 
	fclose(file); 
	return Cal();//�����Ӧ���ƶ����� �� �����ӵ��ƶ���ʽ 
} 

const int CrashFactor = 30;
const int MoveStepFactor = 10;
const int DangerInOtherplacesFactor = -30;
int PosLast;//�ƶ����λ�� 
int CrashNum;//ײ���л������� 
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
	
	//ǰ��µ��˵�λ��
	//�Լ��Ĳ����� 
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
					if(sum>=2)//�п��ܷ���
					{
						
						for(int k=0;k<4;k++)
						{
							int SameNum=1;
							for(int m=k+1;m<4;m++)
								if(pos[j][k]==pos[j][m]) 
									SameNum++;	
							if(SameNum==1){}
							if(SameNum==2)//һ���ᷴ�� 
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
		if(SmallestPos<INF)//����
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

void GoBackPos2()//PosLast����ready�� ,���ߺ����ٱ���ס �������ʱ��λ�� 
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
					if(SameNum==2)//һ���ᷴ�� 
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
	if(GoBack(MoveStep))//�ȷ��� 
	{
		GoBackPos1(MoveStep);//�˻�ʱ��λ�� 
		CalCrashNum();
		if(NeedJump)//���� 
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
	else//δ���� 
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

//����dis���� 
int right(int num)
{
	if(num>=0)
		return num;
	else 
		return -num;
}//����DangerLast���� 
int dis(int pl1,int pl2)//������λ�õ��з����ӵľ��� ������·�ϵ�!!! 
{
	int dis1=pl2-pl1;
	dis1=right(dis1);//ȡdis1�ľ���ֵ
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
				if(pos[j][k]==WIN||pos[j][k]==AIRPORT||pos[j][k]==READY||pos[j][k]>=100){}//���������˵�������ɵ�������������в��ʱ�� 
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

int CalFactor1(int i)// ������ready�������ʱ�� 
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
//����CalFactor2���� 
//int DangerNum=0;
int CalFactorOfDanger(){//������superfly��Χ��֮��ɫ��Ӧ�ĵл� 
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
		case 4://��ɫ		
			OtherColor=GREEN;
			break; 
		case 17://��ɫ
			OtherColor=RED;
			break; 
		case 30://��ɫ
			OtherColor=YELLOW;
			break; 
		case 43://��ɫ
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

void CalCrashNumOfMain()//ײ���� 
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
					if(sum>=2)//�п��ܷ���
					{
						
						for(int k=0;k<4;k++)
						{
							int SameNum=1;
							for(int m=k+1;m<4;m++)
								if(pos[j][k]==pos[j][m]) 
									SameNum++;	
							if(SameNum==1){}
							if(SameNum==2)//һ���ᷴ�� 
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
void IfMoveOfMain(int i)//���ǵ���������1.֧·��2.������3.����4��ײ�� 
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
	
	if(!GoBackOfMain( MoveStep ))//��һ���ߣ�û�з���
	{	           
		
		if( PosLastOfMain<=ColorBrch&&PosLastOfMain+DiceResult>ColorBrch)//����֧· 
		{
			DangerLastOfMain=0;
			PosLastOfMain=(PosLastOfMain+DiceResult-ColorBrch)*100+ColorBrch; 
			if(PosLastOfMain/100==3)
			    DangerLastOfMain -= 100;//********************
			if(PosLastOfMain/100==6)
				DangerLastOfMain += 100; 
		} 
		else//�����ɵ��� 
		{
			int PosPrev=PosLastOfMain; 
			CalCrashNumOfMain();
			if(NeedJumpOfMain())//�� 
			{
				if(GoBackOfMain(4))//���� 
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
	else//��һ���з����� 
	{ 
		Jumped=false;
		PosLastOfMain=SmallestPosOfMain-(MoveStep+PosLastOfMain-SmallestPosOfMain+1);
		if(PosLastOfMain<0)
			PosLastOfMain=52+PosLastOfMain;
		CalCrashNumOfMain();
		int PosPrev=PosLastOfMain; 
		if(NeedJumpOfMain())//����һ�� 
		{
			if(GoBackOfMain(4))//����
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
	//�����ɵ�
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
	//��֧·��
	if(PosLastOfMain>=100)
	{
		if(PosLastOfMain/100==3)
			return DangerLastOfMain-=CalFactorOfDanger();
		else return DangerLastOfMain;
	}		
}
int CalFactor2(int i)//���������ɵ�����֧·ʱ 
{
	if(pos[AIColor][i]>=100)//��֧·ʱ,�Ӵ�WIN��Ȩ�أ�200���� ���Ӵ�danger��Ȩ�أ������ǵ��л�����Χ��superfly�Ŀ��ܱȽϴ�ʱ 
	{
		int WeightInBranch=0; 
		int AddWeight = 0;
		if(pos[AIColor][i]/100==3)//��ǰλ����danger��ʱ��Ȩ������AddWeight 
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
				//������Σ������:������Χ�Ķ�Ӧ��ɫ������������Ӧ�ĸ��ʣ� 
				 
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
	else//�����ɵ���ʱ���Ӵ�superfly��Ȩ�أ��Ӵ�ײ�ɵл���Ȩ�أ��� �Ӵ�Σ�յ�Ȩ�أ� 
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
 
int CalW(int i){//������ӵ�Ȩ�� 
	switch(pos[AIColor][i])
	{
		case WIN:
			return 0;
			break;
		case AIRPORT:
			
			factor = 0;//�򵥰�ʱ��Ϊ0 
			
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

int Cal()//���ƶ����ӵĴ��ţ���λ���Լ��ƶ��ķ�ʽ��ʮλ�� 
{
	int w[4];
	
	for(int i = 0; i < 4; i ++ )//�������ɫÿ������Ӧ���ƶ���Ȩ�� 
		w[i]=CalW(i);
	int maxw=w[0],k=0;//kΪҪ�ƶ������Ӵ��� 
	for(int i= 1; i < 4; i ++ )
		if(w[i]>maxw)
			k=i;
	int ten;//����ֵ�����ʮλ�� 
	if( DiceResult==6&&pos[AIColor][k]==AIRPORT)
		ten = 2;
	else 
		ten = 1;
	return ten*10+k;	 
}
