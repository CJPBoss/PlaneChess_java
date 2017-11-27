package planeChess;

import javax.swing.*;
import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.*;
import java.io.File;

public class PlayerInfo extends JPanel{

	public boolean isHuman;
	public boolean inGame;
	public String[] COLOR = {"Red", "Yellow", "Blue", "Green"};
	public JPanel panel = new JPanel();
	public JComboBox<String> playerType = new JComboBox<String>();
	public JLabel Info = new JLabel("", JLabel.CENTER);
	public String[] typeOption = {
			"None",
			"Human",
			"AI"
	};
	//public JComboBox 
	public String AiAddress;
	public String AiName;
	public int color;
	int CMD = 5;

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
	
	public PlayerInfo(int col) {
		
		setLayout(new GridLayout(2, 1));
		color = col % 4;
		if (color == 0) {
			panel.setBackground(Color.red);
			setBackground(Color.red);
		}
		else if (color == 1) {
			panel.setBackground(Color.yellow);
			setBackground(Color.yellow);
		}
		else if (color == 2) {
			panel.setBackground(Color.blue);
			setBackground(Color.blue);
		}
		else {
			panel.setBackground(Color.green);
			setBackground(Color.green);
		}
		for (int i = 0; i < typeOption.length; i++) {
			playerType.addItem(typeOption[i]);
		}
		playerType.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(ActionEvent arg0) {
					// TODO Auto-generated method stub
					int option = playerType.getSelectedIndex();
					if (option == 0) {
						inGame = false;
						isHuman = true;
						Info.setText("X");
					}
					else if (option == 1) {
						inGame = true;
						isHuman = true;
						Info.setText("");
					}
					else if (option == 2) {
						inGame = true;
						isHuman = false;
						if (setAI()) {
							playerType.setSelectedItem(typeOption[0]);
							isHuman = true;
							inGame = false;
						}
					}
					//Info.setText("<" + COLOR[color] + ">");
					if (!isHuman) {
						Info.setText("AI: " + AiName);
						int len = Info.getText().length();
						if (len >= 15) {
							Info.setText(Info.getText().substring(0, 12) + "...");
						}
					}
				}
			}
		);
		panel.add(playerType);
		add(panel);
		add(Info);
		color = col;
		init();
	}
	
	public String toString() {
		String info = "";
		String col;
		if (color == 0) {
			col = "Red";
		}
		else if (color == 1) {
			col = "Yellow";
		}
		else if (color == 2) {
			col = "Blue";
		}
		else {
			col = "Green";
		}
		if (isHuman) {
			info += "<" + col + ">Human ";
		}
		else {
			info += "<" + col + ">AI ";
			info += AiName;
		}
		return info;
		
	}
	
	public boolean setAI() {
		JFileChooser fd = new JFileChooser(new File(System.getProperty("user.dir")));
		fd.showOpenDialog(null);
		File f = fd.getSelectedFile();
		try {
			isHuman = false;
			AiAddress = f.getPath();
			System.out.println("AI address : " + AiAddress);
			String[] l = AiAddress.split("\\.|\\/|\\\\|\\:");
			AiName = l[l.length - 2];
			System.out.println("AI Name : " + AiName);
			return false;
		}
		catch (Exception e) {
			return true;
		}
		
		/*
		Scanner input = new Scanner(System.in);
		AiAddress = input.nextLine().toString();
		System.out.println(AiAddress);
		String[] l = AiAddress.split("\\.|\\/|\\\\");
		AiName = l[l.length - 2];
		System.out.println(AiName);
		//input.close();
		 */
	}
	
}
