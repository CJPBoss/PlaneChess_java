package planeChess;

import java.io.*;

public class Player{
	public boolean isHuman;
	public PlaneChess game;
	public boolean inGame;
	public String[] COLOR = {"    Red    ", "Yellow", "  Blue  ", "Green  "};
	public String AiName;
	public String AiAddress;
	public int color;
	int CMD = 5;
	int kill = 0;
	int dead = 0;

	public void updataCMD(int c) {
		CMD = c;
	}
	
	public void init() {/*
		int option = playerType.getSelectedIndex();
		if (option == 0) {
			isHuman = true;
			inGame = false;
		}
		else if (option == 1) {
			isHuman = true;
			inGame = true;
		}
		else {
			isHuman = false;
			inGame = true;
		}*/
	}
	
	public Player(int col) {
		
		color = col % 4;
		
		init();
	}
	
	public String toString() {
		String info = "";
		String col;
		col = COLOR[color];
		if (isHuman) {
			info += "<" + col + ">Human ";
		}
		else {
			info += "<" + col + ">AI ";
			info += AiName;
		}
		return info;
		
	}
	
	int c;
	public int getCMD(int rand) {
		c = 5;/*
		Thread a = new Thread() {
			public void run() {
					
			}
		};
		a.start();*/
		if (isHuman) {
			c =  getHumanCMD();
		}
		else {
			c =  getAICMD(rand);
		}	
		return c;
	}
	public void delay(int time) {
		try {
			Thread.currentThread();
			Thread.sleep(time);
		}
		catch (Exception e) {
			//who TM care
		}
	}
	private int getHumanCMD() {
		CMD = 5;
		while (CMD > 3) {
			delay(100);
		}
		int cmd = CMD;
		System.out.println("\n" + CMD);
		CMD = 5;
		/*
		int cmd = CMD;
		CMD = 5;
	
		Scanner input = new Scanner(System.in);
		int cmd = input.nextInt();
		*/
		return cmd;
	}
	
	private int getAICMD(int randStep) {
		String fAddress = "MAPINFO.txt";
		File file = new File(fAddress);
		if (!file.exists()) {
			try {
				file.createNewFile();
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		}
		String cmdString = "";
		String gameInfo = "";
		cmdString += AiAddress + " " + "MAPINFO.txt" + " ";
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				gameInfo += game.planePos[i][j] + " ";
			}
		}
		gameInfo += color + " " + randStep + " ";
		OutputStream out = null;
		try {
			out = new FileOutputStream(file);
			out.write(gameInfo.getBytes());
			out.flush();
			out.close();
		}
		catch (Exception e) {
			
		}
		cmdString += gameInfo;
		//System.out.println("AI " + cmdString);
		Runtime run = Runtime.getRuntime();
		int cmd = 5;
		try {
			Process process = run.exec(cmdString);
			process.waitFor();
			cmd = process.exitValue();
		}
		catch (Exception e) {
			
		}
		return cmd;
	}
}
