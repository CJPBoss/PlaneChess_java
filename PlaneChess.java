package planeChess;

import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.util.Random;

public class PlaneChess extends JLayeredPane{
	int[][] roadPos = new int[][]{
		{38, 150},{69, 140},{97, 140},{126, 150},
		{150, 128},{138, 98},{138, 72},{148, 41},
		{180, 31},{207, 32},{236, 30},{263, 30},
		{292, 30},{324, 40},{333, 72},{332, 102},
		{324, 130},{345, 152},{375, 141},{401, 141},
		{432, 151},{444, 183},{442, 211},{443, 235},
		{444, 264},{444, 293},{432, 325},{402, 335},
		{373, 335},{345, 321},{321, 344},{333, 375},
		{333, 402},{325, 432},{291, 445},{264, 443},
		{234, 443},{210, 444},{180, 444},{148, 433},
		{137, 401},{137, 372},{150, 344},{128, 322},
		{97, 333},{70, 332},{39, 324},{27, 290},
		{29, 262},{27, 234},{28, 210},{27, 181}
	};
	
	int[][][] portPos = new int[][][] {
		{{33, 32},{83, 32},{83, 76},{33, 79}}, 
		{{397, 33},{443, 33},{399, 83},{443, 83}}, 
		{{391, 392},{443, 392},{393, 436},{445, 437}}, 
		{{34, 391},{81, 389},{32, 438},{81, 437}}
	};
	
	int[][] takeOffPos = new int[][] {
		{17, 130}, {348, 22}, {459, 348}, {125, 456} 
	};
	
	int[][][] endPos = new int[][][] {
		{{73, 239}, {102, 238}, {130, 239}, {159, 237}, {186, 238}, {215, 238}}, 
		{{240, 76}, {239, 104}, {239, 133}, {239, 159}, {239, 189}, {240, 215}},
		{{402, 237}, {376, 238}, {347, 237}, {318, 237}, {289, 237}, {261, 239}},
		{{237, 402}, {239, 376}, {237, 344}, {238, 319}, {236, 287}, {237, 259}}
	};
	
	private ImageIcon map;
	private JLabel bg = new JLabel();
	public GameController gc;
	public Player[] players = new Player[4];
	int mouseX;
	int mouseY;
	
	public int[] startPos = {0, 13, 26, 39};
	public int[] endRoadPos = {49, 10, 23, 36};
	public int[] superFly = {17, 30, 43, 4};
	int planeSize = 25;
	boolean[][] flagForAnima = {
			{false, false, false, false}, 
			{false, false, false, false}, 
			{false, false, false, false}, 
			{false, false, false, false}
		};
	public Plane[][] planes = new Plane[4][4];
	public int[][] planePos = new int[4][4];
	
	Mouseclicked mouseclicked = new Mouseclicked();
	MouseMoved mousemoved = new MouseMoved();
	
	public void paintMap(Graphics g) {
	  	map.paintIcon(this, g, 10, 10);
	}
	
	public int[] getPosition(int col, int id) {
		int[] position = new int[2];
		int pos = planePos[col][id];
		Plane p = planes[col][id];
		int o = p.offset;
		if (pos >= 0&& pos < 52) {
			position[0] = roadPos[pos][0] + o;
			position[1] = roadPos[pos][1] + o;
		}
		else if (pos > 100) {
			int h = pos / 100 - 1;
			position[0] = endPos[col][h][0] + o;
			position[1] = endPos[col][h][1] + o;
		}
		else if (pos == -1) {
			position[0] = takeOffPos[col][0] + o;
			position[1] = takeOffPos[col][1] + o;
		}
		else {
			position[0] = portPos[col][id][0];
			position[1] = portPos[col][id][1];
		}
		return position;
	}
	
	public void paintChess(Graphics g) {
		
		for (int i = 0; i < 4; i++) {
			if (!players[i].inGame) {
				continue ;
			}
			for (int j = 0; j < 4; j++) {
				if (flagForAnima[i][j]) {
					continue ;
				}
				int[] pos = getPosition(i, j);
				planes[i][j].setLocation(pos[0], pos[1]);
				if (planePos[i][j] == -3) {
					planes[i][j].setBackground(Color.pink);
				}
			}
		}
		//repaint();
	}
	
	public void paint(Graphics g) {
		super.paint(g);
		//paintMap(g);
		if (planes[0][0] != null&& players[0] != null) {
			paintChess(g);
		}
	}
	
	public void initPlane() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				//planes[i][j] = new Plane(i, j/*, players[i]*/);
				planes[i][j].resetColor();
				planes[i][j].setLocation(portPos[i][j][0], portPos[i][j][1]);
				if (!players[i].inGame) {
					planes[i][j].setVisible(false);
				}
				else {
					planes[i][j].setVisible(true);
				}
				add(planes[i][j], 0);
			}
		}
	}
	
	public void init() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				planePos[i][j] = -2;	//set to the stop port 
			}
		}
		initPlane();
	}
	
	public PlaneChess() {

		setLayout(null);
		map=new ImageIcon(getClass().getResource("bg.png"));
		bg.setIcon(map);
		bg.setSize(500, 500);
		bg.setVisible(true);
		setLayer(bg, -1, -1);
		//place.setSize(500, 500);
		//place.setLayout(null);
		//add(place);
		add(bg, -1);
		//init();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				planePos[i][j] = -2;
				planes[i][j] = new Plane(i, j);
			}
		}
		addMouseListener(mouseclicked);
		addMouseMotionListener(mousemoved);
		repaint();
	}
	
	public boolean isWin(int color) {
		
		color %= 4;
		for (int id = 0; id < 4; id++) {
			if (planePos[color][id] != -3) {
				return false;
			}
		}
		return true;
	}
	
	public boolean movable(int color, int step) {
		
		color %= 4;
		for (int id = 0; id < 4; id++) {

			if (planePos[color][id] > -2) {
				return true;
			}
			else if (planePos[color][id] == -2&& step == 6) {
				return true;
			}
		}
		return false;
	}
	
	
	public boolean gameOver(int limited) {
		int count = 0;
		int c = 0;
		boolean flag;
		boolean f = false;
		for (int i = 0; i < 4; i++) {
			flag = true;	//all plane in i is at win port, true
			for (int j = 0; j < 4;j++) {
				if (planePos[i][j] != -3) {
					flag = false;
					break;
				}
			}
			if (players[i].inGame) {
				c++;
				f = flag;
			}
			if (flag|| !players[i].inGame) {
				count++;
			}
		}
		if (c == 1) {
			return f;
		}
		return count >= limited;
	}
	
	public boolean gameOver() {
		return gameOver(3);
	}
	
	
	private int kill(int color, int index) {
		//kill the plane which on pp[color][index]'s pos
		int killCount = 0;
		int pos = planePos[color][index];
		if (pos > 100|| pos < 0) {	//not on the road
			return 0;
		}
		for (int i = 0; i < 4; i++) {
			if (i == color) {	//ignore same color
				continue ;
			}
			for (int j = 0; j < 4; j++) {
				if (planePos[i][j] == pos) {
					int[] orgPos = getPosition(i, j);
					planePos[i][j] = -2;	//kill
					final int ii = i;
					final int jj = j;
					Thread m = new Thread() {
						public void run() {
							moveAnima(ii, jj, orgPos[0], orgPos[1], 200);
						}
					};
					m.start();
					killCount++;
					players[i].dead++;
				}
			}
		}
		return killCount;
	}

	private int kill(int pos) {
		//kill the plane which on pos
		int killCount = 0;
		if (pos < 0) {
			return 0;
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (planePos[i][j] == pos) {
					int[] orgPos = getPosition(i, j);
					planePos[i][j] = -2;
					final int ii = i;
					final int jj = j;
					Thread m = new Thread() {
						public void run() {
							moveAnima(ii, jj, orgPos[0], orgPos[1], 200);
						}
					};
					m.start();
					killCount++;
					players[i].dead++;
				}
			}
		}
		return killCount;
	}
	
	private boolean walkable(int col, int id) {

		if (planePos[col][id] > 600) {
			return false;
		}
		int count = 0;
		for (int j = 0; j < 4; j++) {	//
			if (j == col) {
				continue;
			}
			for (int k = 0; k < 4; k++) {
				if (planePos[j][k] == planePos[col][id]) {
					count++;
				}
			}
		}
		if (count > 1) {
			return false;
		}
		else {
			return true;
		}
	}
	
	public int[][] getProcess(int x1, int y1, int x2, int y2, int step) {
		if (step < 1) {
			return new int[][]{{x2, y2}};
		}
		int[][] process = new int[step][2];
		float dx = (float)(x2 - x1) / step;
		float dy = (float)(y2 - y1) / step;
		for (int i = 0; i < step - 1; i++) {
			process[i][0] = (int)(x1 + dx * (i + 1));
			process[i][1] = (int)(y1 + dy * (i + 1));
		}
		process[step - 1][0] = x2;
		process[step - 1][1] = y2;
		return process;
	}
	int framePerSecond = 25;
	public void moveAnima(int color, int id, int x, int y, int time) {
		flagForAnima[color][id] = true;
		int invert = 1000 / framePerSecond;
		int[] pos = getPosition(color, id);
		int[][] process = getProcess(x, y, pos[0], pos[1], time / framePerSecond);
		for (int i = 0; i < process.length; i++) {
			planes[color][id].setLocation(process[i][0], process[i][1]);
			repaint();
			delay(invert);
		}
		flagForAnima[color][id] = false;
	}
	
	public boolean move(int color, int index, int step) {
		color %= 4;
		index %= 4;
		if (planePos[color][index] == -3) {
			return true;
		}
		else if (planePos[color][index] == -2) {
			if (step != 6) { 
				return true;
			}
			else {
				int[] orgPos = getPosition(color, index);
				planePos[color][index] = -1;
				moveAnima(color, index, orgPos[0], orgPos[1], 200);
				//repaint();
				return false;
			}
		}
		
		int col = color;
		int id = index;
		int direction = 1;
		//System.out.print("<");
		for (int i = 1; i <= step; i++) {
			int[] orgPos = getPosition(col, id);
			if (planePos[col][id] == -1) {
				planePos[col][id] = startPos[col];
				moveAnima(col, id, orgPos[0], orgPos[1], 100);
				if (!walkable(col, id)) {
					if (step == 6) {
						Random rand = new Random();
						int s = rand.nextInt(6) + 1;
						delay(250);
						gc.updataInfo(color, s);
						gc.jsb.setValue(gc.jsb.getMaximum());
						gc.Record.append(" -----same " + s + "\n");
						move(color, index, s);
						gc.again = true;
						return false;
					}
					direction = - direction;
				}
				//repaint();
				continue ;
			}
			planePos[col][id] += direction;
			//repaint();
			if ((planePos[col][id] == endRoadPos[col] + 1&& direction > 0)|| planePos[col][id] > 100) {
				planePos[col][id] += direction * 99;
			}
			else {
				planePos[col][id] += 52;
				planePos[col][id] %= 52;
			}
			//System.out.print(planePos[col][id] + " ");
			if (!walkable(col, id)) {
				if (planePos[col][id] < 100&& planePos[col][id] > -1&& step == 6) {
					Random rand = new Random();
					int s = rand.nextInt(6) + 1;
					delay(250);
					gc.updataInfo(color, s);
					gc.Record.append("  same " + s + "\n");
					move(color, index, s);
					gc.again = true;
					return false;
				}
				direction = - direction;
			}
			moveAnima(col, id, orgPos[0], orgPos[1], 100);
			delay(50);
			//repaint();
		}
		//System.out.println(">");
		
		if (planePos[col][id] > 600) {
			int[] orgPos = getPosition(color, index);
			planes[col][id].setBackground(Color.pink);
			planePos[col][id] = -3;
			moveAnima(color, index, orgPos[0], orgPos[1], 200);
		}
		
		players[color].kill += kill(col, id);
		int[] orgPos = getPosition(col, id);
		System.out.println(orgPos[0] + ", " + orgPos[1]);
		if (planePos[col][id] == superFly[col] - 4) {
			System.out.println("SPFly1");
			players[color].kill += kill(superFly[col]);
			players[color].kill += kill(300 + endRoadPos[(col + 2) % 4]);
			players[color].kill += kill(superFly[col] + 12);
			planePos[col][id] += 16;
			planePos[col][id] %= 52;
			moveAnima(col, id, orgPos[0], orgPos[1], 500);
		}
		else if (planePos[col][id] == superFly[col]) {
			System.out.println("SPFly2");
			players[color].kill += kill(superFly[col] + 12);
			players[color].kill += kill(superFly[col] + 17);
			players[color].kill += kill(300 + endRoadPos[(col + 2) % 4]);
			planePos[col][id] += 16;
			planePos[col][id] %= 52;
			moveAnima(col, id, orgPos[0], orgPos[1], 500);
		}
		else if (planePos[col][id] % 4 == (col + 1) % 4&& planePos[col][id] != endRoadPos[col]&& planePos[col][id] < 100) {
			System.out.println("Jump");
			planePos[col][id] += 4;
			planePos[col][id] %= 52;
			players[color].kill += kill(col, id);
			moveAnima(col, id, orgPos[0], orgPos[1], 200);
		}
		
		repaint();
		return false;
	}
	
	class MouseMoved implements MouseMotionListener	{
		
		public void mouseMoved(MouseEvent e){
			//System.out.println(e.getX() + ", " + e.getY());
			mouseX = e.getX();
			mouseY = e.getY();
		}
		
		@Override
		public void mouseDragged(MouseEvent arg0) {
			// TODO Auto-generated method stub
			
		}
	}
	class Mouseclicked extends MouseAdapter		//判断鼠标左击并通知棋盘和电脑
	{
		public void mouseClicked(MouseEvent e) {
			System.out.println("{" + e.getX() + ", " + e.getY() + "}, ");
		}
	}
	public void delay(int time) {
		try {
			Thread.currentThread();
			Thread.sleep(time);
		}
		catch (Exception e) {
			
		}
	}
}
