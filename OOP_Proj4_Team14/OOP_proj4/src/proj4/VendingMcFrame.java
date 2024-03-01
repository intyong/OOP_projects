package proj4;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.util.ArrayList;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;

public class VendingMcFrame extends JFrame implements ActionListener {
	
	private static final int MAX_PANEL = 30;
	private VendingMc vm;
	private int panelNum;
	
	//Basic Panels Declaration
	private JPanel topButtonP = new JPanel();
	private JPanel[] goodsP;
	private JPanel botButtonP_Cus = new JPanel();
	private JPanel botButtonP_Man = new JPanel();
	private JPanel leftP = new JPanel();
	private JPanel rightP = new JPanel();
	
	//topButton Panel components
	private static final int Customer = 0;
	private static final int Manager = 1;
	private int TYPE = Customer;
	private JButton btCustomer;
	private JButton btManager;
	
	//left Panel components
	private ArrayList<JPanel> goods;
	private ArrayList<JLabel> image;
	private ArrayList<JLabel> goodsName;
	private int numGoods;
	private boolean selected;
	private JPanel selectedPanel;
	
	//right Panel components
	private JTextArea textArea;
	
	//right botButton Panel components
	private JButton btEdit;
	private JButton btAdd;
	private JButton btDelete;
	private JButton btBuy;
	
	private JLabel lbPriceT;
	private JLabel lbPrice;
	private JButton btInsert;
	private JButton btClear;
	private JLabel lbCoinT;
	private JLabel lbCoin;
	
	
	VendingMcFrame(VendingMc vendingMc){
		super("Vending Machine");
		this.vm = vendingMc;
		
		setLayout(new BorderLayout());
		
		set_topButtonP();
		set_rightP();
		set_leftPanel();
		
		add(topButtonP,BorderLayout.NORTH);
		add(leftP, BorderLayout.WEST);
		add(rightP, BorderLayout.EAST);
		rightP.add(botButtonP_Cus, BorderLayout.SOUTH);
				
		pack();
		setLocationRelativeTo(null);		//화면 중앙에 위치
		setVisible(true);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
	}
	
	private void set_topButtonP(){
		topButtonP.setLayout(new BorderLayout());
		
		btCustomer = new JButton("Customer");
		btManager = new JButton("Manager");
	
		topButtonP.add(btCustomer, BorderLayout.WEST);
		topButtonP.add(btManager, BorderLayout.EAST);
		
		btCustomer.addActionListener(this);
		btManager.addActionListener(this);
	}
	
	private void set_rightP() {
		rightP.setLayout(new BorderLayout());
		botButtonP_Cus.setLayout(new FlowLayout());
		botButtonP_Man.setLayout(new GridLayout(1,5));
		
		textArea = new JTextArea(10, 7);
		textArea.setFont(new Font("굴림체", Font.BOLD, 30));
		
		lbPriceT = new JLabel("price: ");
		lbPrice = new JLabel("       0원");
		btInsert = new JButton("Insert Coin");
		btClear = new JButton("Clear");
		lbCoinT = new JLabel("coin: ");
		lbCoin = new JLabel("       0원");
		btEdit = new JButton("EDIT");
		btAdd = new JButton("ADD");
		btDelete = new JButton("Delete");
		btBuy = new JButton("Buy");
		
		rightP.add(textArea, BorderLayout.NORTH);
		
		botButtonP_Cus.add(lbPriceT);
		botButtonP_Cus.add(lbPrice);
		botButtonP_Cus.add(btInsert);
		botButtonP_Cus.add(btBuy);
		botButtonP_Cus.add(lbCoinT);
		botButtonP_Cus.add(lbCoin);
		botButtonP_Cus.add(btClear);
		
		botButtonP_Man.add(btEdit);
		botButtonP_Man.add(btAdd);
		botButtonP_Man.add(btDelete);
		
		btInsert.addActionListener(this);
		btClear.addActionListener(this);
		btEdit.addActionListener(this);
		btAdd.addActionListener(this);
		btDelete.addActionListener(this);
		btBuy.addActionListener(this);
	}
	
	public void actionPerformed(ActionEvent e) {
		
		if(e.getSource() == btCustomer) {
			TYPE = Customer;
			rightP.remove(botButtonP_Man);
			rightP.add(botButtonP_Cus, BorderLayout.SOUTH);
			refresh();
		}
		
		else if(e.getSource() == btManager) {
			TYPE = Manager;
			rightP.remove(botButtonP_Cus);
			rightP.add(botButtonP_Man, BorderLayout.SOUTH);
			refresh();
		}
		
		else if(e.getSource() == btEdit) {
			if(selectedPanel == null) {
				JOptionPane.showMessageDialog(null, "상품을 선택해주세요."); 
			}
			else {
				Goods selectedDrink = vm.getGoods(goods.indexOf(selectedPanel));
				new GoodsInfoFrame(vm, this, selectedDrink);
			}
		}
		
		else if(e.getSource() == btAdd) {
			new GoodsInfoFrame(vm, this);
		}
		
		else if(e.getSource() == btDelete) {
			if(selectedPanel == null) {
				JOptionPane.showMessageDialog(null, "상품을 선택해주세요.");
			}
			else {
				vm.deleteGoods(goods.indexOf(selectedPanel));
				refresh();
			}
		}
		
		else if(e.getSource() == btInsert) {
			new InsertFrame(this, lbCoin);
			lbCoin.setText(Coin.sum + "원");
		}
		
		else if(e.getSource() == btClear) {
			Coin.clearCoin();
			lbCoin.setText(Coin.sum + "원");
		}
		
		else if(e.getSource() == btBuy) {
			if(selected == true) {
				if(!vm.getGoods(goods.indexOf(selectedPanel)).buyGoods()) {	//구매 실패했으면
					JOptionPane.showMessageDialog(null, "투입된 돈과 재고를 확인해주세요.");
				}
				else {
					lbCoin.setText(Coin.sum + "원");
					textArea.setText(vm.getGoods(goods.indexOf(selectedPanel)).description());
					refresh();
				}
			}
			else {
				JOptionPane.showMessageDialog(null, "상품을 선택해주세요.");
			}
		}
		
		
		/*else if(e.getSource() == btLoad) {
			vm.readFile();
			TYPE = Customer;
			refresh();
		}
		
		else if(e.getSource() == btSave) {
			try {
				vm.saveVMInFile();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}*/
	}
	
	
	private class MyMouseListener extends MouseAdapter {
	      public void mouseClicked(MouseEvent e) {
	    	  if(selected == false) {
		    	  selectedPanel = (JPanel) e.getSource();
		    	  selectedPanel.setBorder(new LineBorder(Color.black, 3));;
		    	  lbPrice.setText(Integer.toString(vm.getGoods(goods.indexOf(selectedPanel)).getPrice()) + "원");
		    	  textArea.setText(vm.getGoods(goods.indexOf(selectedPanel)).description());
		    	  selected = true;
		         }
		         else {
		        	 if(selectedPanel == e.getSource()) {	//클릭했던거 또 클릭
		        		 selectedPanel.setBorder(null);
		        		 selectedPanel = null;
		        		 lbPrice.setText("       0원");
		        		 textArea.setText("");
		        		 selected = false;
		        	 }
		        	 else {							//이미 하나 클릭된 상태에서 다른거 클릭
		        		 JOptionPane.showMessageDialog(null, "이미 음료가 선택되어있습니다. 해제하고 싶다면 이 음료를 다시 클릭해주세요.");
		        	 }
		         }	       
	      }
	   }
	
	
	private void set_goodsP(int index, String type) {
		goodsP[index] = new JPanel();
		goodsP[index].setLayout(new FlowLayout(3));
		//goodsP[index].setPreferredSize(new Dimension(340, 400));
		goodsP[index].setBorder(new TitledBorder(type));
	}
	
	private void addGoods(int index, JPanel goods) {
		goodsP[index].add(goods);
		numGoods++;		
	}

	private void set_leftPanel() {
		leftP.removeAll();
		goodsP = new JPanel[MAX_PANEL];
		goods = new ArrayList<JPanel>();
		image = new ArrayList<JLabel>();
		goodsName = new ArrayList<JLabel>();	
		numGoods = 0;
		selected = false;
		selectedPanel = null;
		
		ArrayList<String> arrangeList = null;
		Goods g;
		JPanel newGoods;
		
		switch(TYPE) {
		case Customer:
			//사용자 모드로 바뀌었을 때
			btCustomer.setEnabled(false);
			btManager.setEnabled(true);
			break;
		case Manager:
			//매니저 모드로 바뀌었을 때
			btCustomer.setEnabled(true);
			btManager.setEnabled(false);
			break;
		}
		
		arrangeList = vm.getCategoryList(); 
		panelNum = arrangeList.size();
		
		for(int i=0;i<panelNum;i++) set_goodsP(i, arrangeList.get(i));
			
		for(int i=0;i<vm.numGoods();i++) {
			g = vm.getGoods(i);
			newGoods = makeGoods(g.getPath(), g.getName());
			
			addGoods(arrangeList.indexOf(g.getCategory()), newGoods);
		}

		leftP.setLayout(new GridLayout(panelNum,1));
		
		for(int i=0;i<panelNum;i++) leftP.add(goodsP[i]);
		
		pack();
	}
	
	private JPanel makeGoods(String path, String name) {
		ImageIcon icon = new ImageIcon(path);
		icon.setImage(icon.getImage().getScaledInstance(100, 100, Image.SCALE_SMOOTH));
		image.add(new JLabel(icon));
		
		goodsName.add(new JLabel(name));
		goodsName.get(numGoods).setHorizontalAlignment(JLabel.CENTER);
		
		goods.add(new JPanel());
		
		JPanel newP = goods.get(numGoods);
		newP.setLayout(new BorderLayout());
		
		newP.add(image.get(numGoods), BorderLayout.NORTH);	//image가 drink패널의 0번 컴포넌트
		newP.add(goodsName.get(numGoods), BorderLayout.CENTER);
		
		newP.addMouseListener(new MyMouseListener());
		
		return newP;
	}
	
	public void refresh() {
		try {
			vm.saveVMInFile();
		} catch (IOException e) {
			e.printStackTrace();
		}
		set_leftPanel();
		revalidate();
		repaint();
	}
}
