package planeChess;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;

public class GameController extends JPanel{
	public PlaneChess game;
	public PlayerInfo[] playerInfo = new PlayerInfo[4];
	public JPanel Info = new JPanel();
	public JTextArea Record = new JTextArea(1, 15);
	public int[] winner;
	public String[] COLOR = {"    Red    ", "Yellow", "  Blue  ", "Green  "};
	boolean isGaming;
	boolean inited;
	boolean pause;
	public JButton Start = new JButton("Start");
	public JButton Pause = new JButton("Pause");
	public JPanel ButtonSet = new JPanel(new GridLayout(1, 2));
	public JLabel randNumber = new JLabel("Random", JLabel.CENTER);
	public JScrollPane jsp = new JScrollPane();
	public JScrollBar jsb = null;
	boolean flag = false;
	public boolean again = false;
	
	public GameController() {
		isGaming = false;
		inited = false;
		init();
		setLayout(new BorderLayout());
		Info.setLayout(new GridLayout(7, 1));
		randNumber.setFont(new java.awt.Font("Dialog", 1, 20));
		Start.addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent arg0) {
					gamingInit();
					isGaming = isGaming|| flag ? false : true;
					if (isGaming) {
						pause = false;
						gamingInit();
						Record.setText("Record\n");
						Start.setText("Reset");
					}
					else {
						pause = true;
						Start.setText("Start");
						if (!flag) {
							gamingInit();
						}
						else {
							flag = false;
						}
					}
				}
			}
		);
		Pause.addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent arg0) {
					pause = pause ? false : true;
					if (pause) {
						Pause.setText("Continue");
					}
					else {
						Pause.setText("Pause");
					}
				}
			}
		);
		ButtonSet.add(Start);
		ButtonSet.add(Pause);
		//Info.add(player);
		randNumber.setOpaque(true);
		randNumber.setFont(new Font("»ªÎÄÐÐ¿¬", Font.BOLD, 40));
		Record.setEditable(false);
		Record.setLineWrap(true);
		Record.setWrapStyleWord(true);
		jsp.setViewportView(Record);
		jsp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        jsb = jsp.getVerticalScrollBar();
		Info.add(randNumber);
		Info.add(jsp);
		Info.add(ButtonSet);
		updataInfo(0, 0);
		for (int i = 0; i < 4; i++) {
			playerInfo[i] = new PlayerInfo(i);
			Info.add(playerInfo[i]);
		}
		add(game, BorderLayout.CENTER);
		add(Info, BorderLayout.EAST);
		add(new JPanel(), BorderLayout.NORTH);
		add(new JPanel(), BorderLayout.WEST);
		add(new JPanel(), BorderLayout.SOUTH);
	}
	public void init() {
		game = new PlaneChess();
		game.gc = this;
		//game.init();
		winner = new int[]{-1, -1, -1, -1};
		//inited = true;
		for (int i = 0; i < 4; i++) {
			game.players[i] = new Player(i);
		}
	}
	
	public void gamingInit() {
		
		for (int i = 0; i < 4; i++) {
			game.players[i].kill = 0;
			game.players[i].dead = 0;
			game.players[i].AiName = playerInfo[i].AiName;
			game.players[i].AiAddress = playerInfo[i].AiAddress;
			game.players[i].isHuman = playerInfo[i].isHuman;
			game.players[i].inGame = playerInfo[i].inGame;
			game.players[i].game = game;
		}
		//pause = false;
		game.init();
		//game.initPlane();
		winner = new int[]{-1, -1, -1, -1};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				game.planes[i][j].setPlayer(game.players[i]);
			}
		}
		inited = true;
	}
	
	public void updataInfo(int nowPlayer, int randStep) {

		randNumber.setText("" + randStep);
		if (nowPlayer == 0) {
			randNumber.setBackground(Color.red);
		}
		else if (nowPlayer == 1) {
			randNumber.setBackground(Color.yellow);
		}
		else if (nowPlayer == 2) {
			randNumber.setBackground(Color.blue);
		}
		else {
			randNumber.setBackground(Color.green);
		}
		//repaint();
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
	
	public void start() {
		//isGaming = true;
		if (!isGaming|| pause) {
			return ;
		}
		Random random = new Random();
		if (!inited) {
			gamingInit();
		}
		int nowPlayer = 0;
		int randStep = 0;
		int winnerNum = 0;
		//System.out.println("game over ? " + game.gameOver());
		int stepCount = 0;
		pause = false;
		System.out.println(game.gameOver() + ", " + isGaming);

		//Scanner input = new Scanner(System.in);
		while (!game.gameOver()&& isGaming) {
			while (pause) {
				delay(100);
			}
			//randNumber.setText(nowPlayer + "");
			if (!game.players[nowPlayer].inGame|| game.isWin(nowPlayer)) {
				nowPlayer++;
				nowPlayer %= 4;
				continue;
			}
			
			//System.out.println("Now " + nowPlayer);
			randStep = random.nextInt(6) + 1;
			System.out.println(randStep + "");
			updataInfo(nowPlayer, randStep);
			delay(100);
			//randStep = input.nextInt();
			//randStep = 1;
			//System.out.println("\tGet : " + randStep);
			System.out.println("<" + nowPlayer + "> " + randStep);
			if (!game.movable(nowPlayer, randStep)) {
				nowPlayer++;
				nowPlayer %= 4;
				//System.out.println("Can't not move");
				continue;
			}
			stepCount++;
			String id = "%4d.  ";
			id = String.format(id, stepCount);
			Record.append(id + COLOR[nowPlayer] + "  [" + randStep + "]  ");
			jsb.setValue(jsb.getMaximum());
			boolean cmdFlag = true;
			while (cmdFlag) {
				
				int cmd = game.players[nowPlayer].getCMD(randStep);
				Record.append("ID : " + (cmd % 10) + "\n");
				//System.out.println("CMD :" + cmd);
				int planeID = cmd % 10;
				cmdFlag= game.move(nowPlayer, planeID, randStep);
			}
			if (game.isWin(nowPlayer)) {
				winner[winnerNum] = nowPlayer;
				winnerNum++;
			}
			if (randStep != 6) {
				nowPlayer++;
				nowPlayer %= 4;
			}
			else if (again) {
				again = false;
				nowPlayer++;
				nowPlayer %= 4;
			}
		}	//game is over

		if (!isGaming) {
			gamingInit();
			return ;
		}
		
		int total = 0;
		int c = 0;
		for (int i = 0; i < 4; i++) {
			if (game.players[i].inGame) {
				total += i;
				c++;
			}
		}
		if (c == 0) {
			JOptionPane.showMessageDialog(null, "No Player !", "Rank", JOptionPane.INFORMATION_MESSAGE);
			inited = false;
			isGaming = false;
			return ;
		}
		
		int temp = 0;
		for (int i = 0; i < c - 1; i++) {
			temp += winner[i];
		}
		winner[c - 1] = total - temp;
		String[] finalScore = new String[4];
		String finals = "";
		for (int i = 0; i < c; i++) {
			finalScore[i] = "No." + (i + 1) + " : " + game.players[winner[i]];
			finals += finalScore[i] + "\n";
			System.out.println(finalScore[i]);
		}
		Record.append("\n-----------------------------------------\n");
		for (int i = 0; i < c; i++) {
			Record.append("." + finalScore[i] + "\n");
		}
		Record.append("-----------------------------------------\n");
		JOptionPane.showMessageDialog(null, finals.substring(0, finals.length() - 1), "Rank", JOptionPane.INFORMATION_MESSAGE);
		inited = false;
		isGaming = false;
		flag = true;
		jsb.setValue(jsb.getMaximum());
	}
}
