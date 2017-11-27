package planeChess;

import java.awt.Color;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;

public class Plane extends JButton{

	public Player player;
	public int color;
	public int index;
	public int offset;
	
	public void setPlayer(Player p) {
		player = p;
	}
	
	public void resetColor() {
		if (color == 0) {
			setBackground(Color.red);
		}
		else if (color == 1) {
			setBackground(Color.yellow);
		}
		else if (color == 2) {
			setBackground(Color.blue);
		}
		else {
			setBackground(Color.green);
		}
	}
	
	public Plane(int col, int id) {
		setMargin(new Insets(0, 0, 0, 0));
		setFont(new Font("»ªÎÄÐÐ¿¬", Font.BOLD, 12));
		setText("" + id);
		color = col % 4;
		index = id % 4;
		offset = (id - 1) * 2;
		setSize(25, 25);
		//System.out.println(player.isHuman);
		resetColor();
		addActionListener(
			new ActionListener() {

				@Override
				public void actionPerformed(ActionEvent arg0) {
					// TODO Auto-generated method stub
					System.out.println(player.CMD + ", " + color + ", " + index);
					player.updataCMD(index);
					System.out.println(player.CMD + ", " + color + ", " + index);
					
				}
			}
		);
	}
}
