package planeChess;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Main extends JFrame{

	public GameController game;
	public JMenuBar menuBar = new JMenuBar();
	public JMenu[] menu = {new JMenu("Help")};
	public JMenuItem[] helpMenu = {new JMenuItem("Manual"), new JMenuItem("Developer")};
	
	public static void delay(int time) {
		try {
			Thread.currentThread();
			Thread.sleep(time);
		}
		catch (Exception e) {
			
		}
	}
	
	public void setMenuBar() {

		helpMenu[0].setActionCommand("Manual");
		helpMenu[1].setActionCommand("About");
		for (int i = 0; i < helpMenu.length; i++) {
			menu[0].add(helpMenu[i]);
		}
		helpMenu[0].addActionListener(new ActionListener(){
				public void actionPerformed(ActionEvent e) {
					JMenuItem s = (JMenuItem) e.getSource();
					if (s.getActionCommand().equals("Manual")) {
						String manual = "右边设置玩家 [None : 无 Human: 人类 AI: 电脑] " +
										"\n点击[start]开始，[restart]重置，[pause]暂停，[continue]继续" +
										"\n点击方形棋子来走一步（当玩家为Human时）" +
										"\n选择AI时弹出文件选择框，应选择可用的AI可执行文件" +
										"\n右上方为当前玩家的颜色及获得的骰子点数" +
										"\n右方文本框为游戏流程的记录";
						JOptionPane.showConfirmDialog(null, manual, "说明", JOptionPane.CLOSED_OPTION,JOptionPane.INFORMATION_MESSAGE);
					}
				}
			}
		);
		helpMenu[1].addActionListener(new ActionListener(){
				public void actionPerformed(ActionEvent e) {
					JMenuItem s = (JMenuItem) e.getSource();
					if (s.getActionCommand().equals("About")) {
						JOptionPane.showConfirmDialog(null,"团队成员：\n" +
	                            "刘志涛   53160127\n" +
	                            "陈铮       531603..\n" +
	                            "李舞桂   53160120\n" +
	                            "黄昕睿   ........\n","关于",JOptionPane.CLOSED_OPTION,JOptionPane.INFORMATION_MESSAGE);
					}
				}
			}
		);
		menuBar.add(menu[0]);
		setJMenuBar(menuBar);
	}
	
	Main() {
		//helpMenu[0].addActionListener(menuclicked);
		setTitle("飞行棋");
		game = new GameController();
		setBounds(50, 50, 700, 570);
		setResizable(false);
		setVisible(true);
		setLayout(new BorderLayout());
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		add(game, BorderLayout.CENTER);
		setMenuBar();
		//game.start();
	}
	
	public static void main(String[] args) {
		Main m = new Main();
		m.game.isGaming = false;
		while (true) {
			try {
				m.game.start();
			}
			catch (Exception e) {
				//who tm care
			}
			delay(100);
		}
	}
}