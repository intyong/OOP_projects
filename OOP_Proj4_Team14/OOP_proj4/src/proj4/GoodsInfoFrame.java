package proj4;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class GoodsInfoFrame extends JFrame implements ActionListener {
	
	private final static int ADD = 0;
	private final static int EDIT = 1;
	private int TYPE;
	
	private VendingMc vendingMc;
	private VendingMcFrame vendingMcFrame;
	private Goods goods;
	
	//Basic Panels Declaration
	private JPanel topP = new JPanel();
	private JPanel botP = new JPanel();
	
	//top Panel components
	private JLabel nameL;
	private JLabel priceL;
	private JLabel countL;
	private JLabel imageFileL;
	private JLabel selectL;
	private JLabel typeL;
	
	private JTextField nameTF = new JTextField();
	private JTextField priceTF = new JTextField();
	private JTextField countTF = new JTextField();
	private JTextField imageFileTF = new JTextField();
	private JTextField typeTF = new JTextField();
	private JButton btFile;
	private JFileChooser fileChooser = new JFileChooser("C:");
	
	private String tempName;
	private int tempPrice;
	private int tempCount;
	private String tempPath;
	private String tempType;
	
	
	//bottom Panel components
	private JButton btCancel;
	private JButton btOK;
	
	//Add버튼 눌렸을 때
	GoodsInfoFrame(VendingMc vm, VendingMcFrame vmFrame){
		TYPE = ADD;
		this.vendingMc = vm;
		this.vendingMcFrame = vmFrame;
			
		setLayout(new BorderLayout());
		
		set_topP();
		set_botP();
		
		add(topP,BorderLayout.NORTH);
		add(botP, BorderLayout.SOUTH);
		
		pack();
		setLocationRelativeTo(null);
		setVisible(true);
	}
	
	//Edit버튼 눌렸을 때
	GoodsInfoFrame(VendingMc album, VendingMcFrame albumFrame, Goods goods){
		this(album, albumFrame);
		TYPE = EDIT;
		this.goods = goods;
		
		tempName = goods.getName();
		tempPrice = goods.getPrice();
		tempCount = goods.getCount();
		tempPath = goods.getPath();
		tempType = goods.getType();
		
		if(!tempName.isBlank()) nameTF.setText(tempName);
		priceTF.setText(Integer.toString(tempPrice));
		countTF.setText(Integer.toString(tempCount));
		if(!tempType.isBlank()) {
			typeTF.setText(tempType);
			typeTF.setEnabled(false);
		}
		if(!tempPath.isBlank()) imageFileTF.setText(tempPath);
		
		pack();
		setLocationRelativeTo(null);
	}

	public void actionPerformed(ActionEvent e) {
		
		if(e.getSource() == btFile) {
			int check = fileChooser.showOpenDialog(null);
			if(check != JFileChooser.APPROVE_OPTION) return;	//파일을 선택하지 않았다면
			tempPath = fileChooser.getSelectedFile().getPath();		//파일을 선택했다면 -> textField에 넣어줌
			imageFileTF.setText(tempPath);
		}
		
		else if(e.getSource() == btOK) {
			switch(TYPE) {
			case ADD:
				tempPath = imageFileTF.getText();
				
				if(tempPath.isBlank()) {		//사진파일 정보가 없다면 (오류)
					JOptionPane.showMessageDialog(null, "사진 파일 정보를 입력해주세요.");
				}
				else {
					tempName = nameTF.getText();
					tempPrice = Integer.parseInt(priceTF.getText());
					tempCount = Integer.parseInt(countTF.getText());
					tempType = typeTF.getText();
					
					vendingMc.addGoods(tempName, tempPrice, tempCount, tempType, tempPath);
					
					vendingMcFrame.refresh();
					dispose();
				}
				break;
			
			case EDIT:		// 물건의 타입을 변경할 수는 없음!!!
				tempPath = imageFileTF.getText();
				tempName = nameTF.getText();
				tempPrice = Integer.parseInt(priceTF.getText());
				tempCount = Integer.parseInt(countTF.getText());
				
				goods.setData(tempName, tempPrice, tempCount, tempPath);
				
				vendingMcFrame.refresh();
				dispose();
				break;
			}
		}
		
		else if(e.getSource() == btCancel) dispose();
	}
	
	private void set_topP() {
		topP.setLayout(new GridLayout(2,6, 10, 0));
		
		nameL = new JLabel("Name");
		priceL = new JLabel("Price");
		countL = new JLabel("Count");
		imageFileL = new JLabel("Image File");
		selectL = new JLabel("Select");
		typeL = new JLabel("Type");
		
		btFile = new JButton("File");
		btFile.addActionListener(this);
		
		topP.add(nameL);
		topP.add(priceL);
		topP.add(countL);
		topP.add(typeL);
		topP.add(imageFileL);
		topP.add(selectL);
		
		topP.add(nameTF);
		topP.add(priceTF);
		topP.add(countTF);
		topP.add(typeTF);
		topP.add(imageFileTF);
		topP.add(btFile);
	}
	
	private void set_botP() {
		botP.setLayout(new FlowLayout());
		
		btCancel = new JButton("Cancel");
		btOK = new JButton("OK");
		
		btOK.addActionListener(this);
		btCancel.addActionListener(this);
		
		botP.add(btCancel);
		botP.add(btOK);
	}

}
