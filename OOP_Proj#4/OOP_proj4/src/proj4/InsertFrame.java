package proj4;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class InsertFrame extends JFrame implements ActionListener {
	
	private VendingMcFrame vmFrame;
	private JLabel lbMoney;
	
	//Basic Panels Declaration
	private JPanel topP = new JPanel();
	private JPanel botP = new JPanel();
	
	//top Panel components
	private JButton bt100;
	private JButton bt500;
	private JButton bt1000;
	private JButton bt5000;
	private JButton bt10000;
	private JButton bt50000;

	//bottom Panel components
	private JButton btOK;
	
	//Insert버튼 눌렸을 때
	InsertFrame(VendingMcFrame vmFrame, JLabel lbMoney){
		this.vmFrame = vmFrame;
		this.lbMoney = lbMoney;
			
		setLayout(new BorderLayout());
		
		set_topP();
		set_botP();
		
		add(topP,BorderLayout.NORTH);
		add(botP, BorderLayout.SOUTH);
		
		pack();
		setLocationRelativeTo(null);
		setVisible(true);
	}
	
	
	public void actionPerformed(ActionEvent e) {
		
		if(e.getSource() == bt100) {
			Coin.add100();
			lbMoney.setText(Coin.sum + "원");
			vmFrame.refresh();
		}
		
		else if(e.getSource() == bt500) {
			Coin.add500();
			lbMoney.setText(Coin.sum + "원");
			vmFrame.refresh();
		}
		
		else if(e.getSource() == bt1000) {
			Coin.add1000();
			lbMoney.setText(Coin.sum + "원");
			vmFrame.refresh();
		}
		
		else if(e.getSource() == bt5000) {
			Coin.add5000();
			lbMoney.setText(Coin.sum + "원");
			vmFrame.refresh();
		}
		
		else if(e.getSource() == bt10000) {
			Coin.add10000();
			lbMoney.setText(Coin.sum + "원");
			vmFrame.refresh();
		}
		
		else if(e.getSource() == bt50000) {
			Coin.add50000();
			lbMoney.setText(Coin.sum + "원");
			vmFrame.refresh();
		}
		
		else if(e.getSource() == btOK) dispose();
	}
	
	private void set_topP() {
		topP.setLayout(new GridLayout(3,2, 5, 5));
		
		bt100 = new JButton("100");
		bt500 = new JButton("500");
		bt1000 = new JButton("1000");
		bt5000 = new JButton("5000");
		bt10000 = new JButton("10000");
		bt50000 = new JButton("50000");
		
		bt100.addActionListener(this);
		bt500.addActionListener(this);
		bt1000.addActionListener(this);
		bt5000.addActionListener(this);
		bt10000.addActionListener(this);
		bt50000.addActionListener(this);
		
		topP.add(bt100);
		topP.add(bt500);
		topP.add(bt1000);
		topP.add(bt5000);
		topP.add(bt10000);
		topP.add(bt50000);
	}
	
	private void set_botP() {
		botP.setLayout(new FlowLayout());
		
		btOK = new JButton("OK");
		btOK.addActionListener(this);
		botP.add(btOK);
	}

}
