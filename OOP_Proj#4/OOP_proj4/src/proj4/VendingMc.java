package proj4;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.Scanner;
import java.util.ArrayList;

public class VendingMc {	

	private ArrayList<Goods> goodsList;
	private ArrayList<String> categoryList;
	
	private String goodsFilePath;
	private File file;
	private FileWriter fw;
	
	VendingMc(String goodsFilePath) throws IOException{
		this.goodsFilePath = goodsFilePath;
		readFile();
	}
	
	public void readFile() {
		file = new File(goodsFilePath);
		
		Scanner input = null;
		goodsList = new ArrayList<Goods>();
		
		try {
			input = new Scanner(file);
		}
		catch(Exception e) {
			System.out.println("파일 입력이 올바르지 않습니다.");
		}
		while(input.hasNext()) {
			String line = input.nextLine();
			
			if(line.isEmpty()) {
			}
			else {
				String[] info = line.split(";",-1);
				addGoods(info[0].trim(), Integer.parseInt(info[1]), Integer.parseInt(info[2]), info[3].trim(), info[4].trim());
			}
		}
		setArrangeList();
	}
	
	public void addGoods (String name, int price, int count, String className, String imagePath) {
		Goods temp = null;
		try {
			temp = (Goods) Class.forName("proj4." + className).getDeclaredConstructor(String.class, int.class, 
					int.class, String.class, String.class).newInstance(name, price, count, className, imagePath);
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
        goodsList.add(temp);
	}
	
	public void deleteGoods(int index) {
		goodsList.remove(index);
	}
	
	public void setArrangeList() {
		categoryList = new ArrayList<String>();
		Goods g;
		
		for(int i=0;i<numGoods();i++) {
			g = goodsList.get(i);
			if(!categoryList.contains(g.getCategory())) categoryList.add(g.getCategory());
		}
	}
	
	public void saveVMInFile() throws IOException {
		//VendingMC 객체를 파일에 저장한다 (안에 Goods들을 줄줄이 저장하는)
		fw = new FileWriter(file);
		
		for(int i=0;i<goodsList.size();i++) {
			fw.write(goodsList.get(i).getGoodsInfo() + "\n");	
		}
		fw.close();
	}
	
	public int numGoods() {return goodsList.size();}
	public Goods getGoods(int i) {return goodsList.get(i);}
	public ArrayList<String> getCategoryList() {return categoryList;}
	
}
