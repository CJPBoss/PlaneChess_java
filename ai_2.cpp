#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>

const int diff[4] = {0, 39, 26, 13};
const int startPos[4] = {0, 13, 26, 39};
const int round = 52;
float weight[] = {55, -40, 37, 3, 1};
				//win, port, sta, road, endroad
float eff = 0.9;
int endRoadPos[4] = {49, 10, 23, 36};
int superFly[4] = {17, 30, 43, 4};

void kill(int pos[16], int p, int f)
{
	for (int i = 0; i < 16; i++) 
	{
		if (pos[i] == p) 
		{
			pos[i] = -2;
		}
	}
}

void kill(int pos[16], int id)
{
	for (int i = 4; i < 16; i++)
	{
		if (pos[i] == id)
		{
			pos[i] = -2;
		}
	}
}
int walkable(int pos[], int id)
{
	if (pos[id] > 600) 
	{
		return 0;
	}
	int count = 0;
	for (int j = 4; j < 16; j++) {	//
		if (pos[j] == pos[j]) 
		{
			count++;
		}
	}
	if (count > 1) 
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}
int move(int pos[16], int index, int step)
{
	int color = 0;
	index %= 4;
	if (pos[index] == -3) {
		return 1;
	}
	else if (pos[index] == -2) {
		if (step != 6) { 
			return 1;
		}
		else {
			pos[index] = -1;
			return 0;
		}
	}
	
	int col = color;
	int id = index;
	int direction = 1;
	for (int i = 1; i <= step; i++) {
		if (pos[id] == -1) {
			pos[id] = startPos[col];
			if (!walkable(pos, id)) {
				direction = - direction;
			}
			//repaint();
			continue ;
		}
		pos[id] += direction;
		//repaint();
		if ((pos[id] == endRoadPos[col] + 1&& direction > 0)|| pos[id] > 100) {
			pos[id] += direction * 99;
		}
		else {
			pos[id] += 52;
			pos[id] %= 52;
		}
		//System.out.print(pos[col][id] + " ");
		if (!walkable(pos, id)) {
			direction = - direction;
		}
		//repaint();
	}
	//System.out.println(">");
	
	if (pos[id] > 600) {
		pos[id] = -3;
	}
	
	kill(pos, id);
	if (pos[id] == superFly[col] - 4) {
		kill(pos, superFly[col], 0);
		kill(pos, 300 + endRoadPos[(col + 2) % 4], 0);
		kill(pos, superFly[col] + 12, 0);
		pos[id] += 16;
		pos[id] %= 52;
	}
	else if (pos[id] == superFly[col]) {
		kill(pos, superFly[col] + 12, 0);
		kill(pos, superFly[col] + 17, 0);
		kill(pos, 300 + endRoadPos[(col + 2) % 4], 0);
		pos[id] += 16;
		pos[id] %= 52;
	}
	else if (pos[id] % 4 == (col + 1) % 4&& pos[id] != endRoadPos[col]&& pos[id] < 100) {
		pos[id] += 4;
		pos[id] %= 52;
		kill(pos, id);
	}
	
	return 0;
} 



class Status
{
public :
	int planePos[16];
	int deep;	//search deep
	int floor; 
	int startColor;
	int color;
	int step;
	float valueOfNext[4];
	int flag[4];
	
	int setToStart(int id)
	{
		for (int i = 0; i < 16; i++)
		{
			if (planePos[i] > -1&& planePos[i] < 50)
			{
				planePos[i] = (planePos[i] + diff[id]) % round;
			}
		}
		int temp[4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				//printf("%d ", planePos[i * 4 + j]);
				temp[j] = planePos[j * 4 + i];
			}
			for (int j = 0; j < 4; j++)
			{
				planePos[j * 4 + i] = temp[(id + j) % 4];
			}
			//printf("\n");
		}
	}
	
	float getNext()
	{
		if (deep <= floor)
		{
			return 0;
		}
		int four[4][16];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				four[i][j] = planePos[j];
			}
		}
		int enable = 0;
		float thisValue[4];
		for (int i = 0; i < 4; i++)
		{
			flag[i] = 1 - move(four[i], i, step);
			enable += flag[i];
			valueOfNext[i] = getValue(four[i], (4 - floor) % 4);
			thisValue[i] = getValue(four[i], 0);
		}
		int mi = 0;
		for (; mi < 4; mi++)
		{
			if (flag[mi])
			{
				break;
			}
		}
		float mv = thisValue[mi];
		for (int i = mi; i < 4; i++)
		{
			if (flag[i])
			{
				if (mv < thisValue[i])
				{
					mv = thisValue[i];
					mi = i;
				}
			}
		}
		if (flag[mi])
		{
			for (int j = 1; j < 6; j++)
			{
				Status s(four[mi]);
				s.color = (color + 1) % 4;
				s.startColor = startColor;
				s.deep = deep;
				s.floor = floor + 1;
				s.setToStart(1);
				s.step = j;
				valueOfNext[mi] += eff * s.getNext();//s.getValue((4 - floor) % 4)
			}
			float a = 0;
			for (int i = 0; i < 4; i++)
			{
				a += valueOfNext[i];
			}
			return a / enable / 6;
		}
		return 0;
		
		/*
		for (int i = 0; i < 4; i++)
		{
			if (flag[i])
			{
				for (int j = 1; j < 6; j++)
				{
					Status s(four[i]);
					s.color = (color + 1) % 4;
					s.startColor = startColor;
					s.deep = deep;
					s.floor = floor + 1;
					s.setToStart(1);
					s.step = j;
					valueOfNext[i] += eff * s.getNext();//s.getValue((4 - floor) % 4)
				}
			}
		}
		if (enable == 0)
		{
			return 0;
		}
		else
		{
			float a = 0;
			for (int i = 0; i < 4; i++)
			{
				a += valueOfNext[i];
			}
			return a / enable;
		}
		*/
	}
	
	float getValue(int id = 0)
	{
		return getValue(planePos, id);
	} 

	Status()
	{
		for (int i = 0; i < 16; i++)
		{
			planePos[i] = 0;
		}
		for (int i = 0; i < 4; i++)
		{
			valueOfNext[i] = 0;
		}
	}
	
	float getValue(int p[16], int id = 0)
	{
		float value = 0; 
		for (int i = 0; i < 16; i++)
		{
			if (i > id * 4 && i < (id + 1) * 4) {
				if (p[i] == -3)
				{
					value += weight[0];
				}
				else if (p[i] == -2)
				{
					value += weight[1];
				}
				else if (p[i] == -1)
				{
					value += weight[2];
				}
				else if (p[i] > -1 && p[i] < 100)
				{
					value += weight[3] * p[i];
				}
				else if (p[i] > 100)
				{
					value += (p[i] / 100 + p[i] % 100) * weight[4];
				}
			}
			else {
				if (p[i] == -3)
				{
					value -= weight[0];
				}
				else if (p[i] == -2)
				{
					value -= weight[1];
				}
				else if (p[i] == -1)
				{
					value -= weight[2];
				}
				else if (p[i] > -1 && p[i] < 100)
				{
					value -= weight[3] * p[i];
				}
				else if (p[i] > 100)
				{
					value -= (p[i] / 100 + p[i] % 100) * weight[4];
				}
			}
		}
		return value;
	}
	
	Status(int pos[16])
	{
		for (int i = 0; i < 16; i++)
		{
			planePos[i] = pos[i];
		}
		for (int i = 0; i < 4; i++)
		{
			valueOfNext[i] = 0;
		}
	}
	int onRoad(int id)
	{
		if (planePos[id] > -2 && planePos[id] < 100)
		{
			return 1;
		}
		return 0;
	}
	
	int onEndRoad(int id)
	{
		if (planePos[id] > 100)
		{
			return 1;
		}
		return 0;
	}
}; 

int main(int argc, char *argv[])
{
	FILE *f;
	FILE *ff;
	ff = fopen("record.txt", "a");
	f = fopen(argv[1], "r");
	int posi[16];
	for (int i = 0; i < 16; i++)
	{
		fscanf(f, "%d", &posi[i]);
	}
	int index, randStep;
	fscanf(f, "%d", &index);
	fscanf(f, "%d", &randStep);
	for (int i = 0; i < 16; i++)
	{
		fprintf(ff, "%d ", posi[i]);
	}
	fprintf(ff, "\nindex : %d rand : %d \n", index, randStep);
	fclose(f);
	Status s(posi);
	s.color = s.startColor = index;
	s.floor = 0;
	s.deep = 6;
	s.setToStart(index);
	for (int i = 0; i < 16; i++)
	{
		fprintf(ff, "%d ", s.planePos[i]);
	}
	
	s.step = randStep;
	s.getNext();
	int i;
	for (i = 0; i < 4; i++)
	{
		if (s.flag[i] == 1)
		{
			break;
		}
	}
	float mv = s.valueOfNext[i];
	int mi = i;
	for (; i < 4; i++)
	{
		printf("%f ", s.valueOfNext[i]);
		if (s.valueOfNext[i] > mv&& s.flag[i])
		{
			mv = s.valueOfNext[i];
			mi = i;
		}
	}
	printf("\n");
	if (posi[mi + index * 4] == -2&& randStep == 6)
	{
		fprintf(ff, "\nCMD : %d\n\n", 20 + mi);
		fclose(ff);
		return 20 + mi;
	}
	else
	{
		fprintf(ff, "\nCMD : %d\n\n", 10 + mi);
		fclose(ff);
		return 10 + mi;
	}
}



