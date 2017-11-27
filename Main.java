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
						String manual = "�ұ�������� [None : �� Human: ���� AI: ����] " +
										"\n���[start]��ʼ��[restart]���ã�[pause]��ͣ��[continue]����" +
										"\n���������������һ���������ΪHumanʱ��" +
										"\nѡ��AIʱ�����ļ�ѡ���Ӧѡ����õ�AI��ִ���ļ�" +
										"\n���Ϸ�Ϊ��ǰ��ҵ���ɫ����õ����ӵ���" +
										"\n�ҷ��ı���Ϊ��Ϸ���̵ļ�¼";
						JOptionPane.showConfirmDialog(null, manual, "˵��", JOptionPane.CLOSED_OPTION,JOptionPane.INFORMATION_MESSAGE);
					}
				}
			}
		);
		helpMenu[1].addActionListener(new ActionListener(){
				public void actionPerformed(ActionEvent e) {
					JMenuItem s = (JMenuItem) e.getSource();
					if (s.getActionCommand().equals("About")) {
						JOptionPane.showConfirmDialog(null,"�Ŷӳ�Ա��\n" +
	                            "��־��   53160127\n" +
	                            "���       531603..\n" +
	                            "�����   53160120\n" +
	                            "����   ........\n","����",JOptionPane.CLOSED_OPTION,JOptionPane.INFORMATION_MESSAGE);
					}
				}
			}
		);
		menuBar.add(menu[0]);
		setJMenuBar(menuBar);
	}
	
	Main() {
		//helpMenu[0].addActionListener(menuclicked);
		setTitle("������");
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