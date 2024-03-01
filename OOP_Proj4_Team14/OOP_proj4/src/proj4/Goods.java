package proj4;

public class Goods {
	
	private String name;
	private int price = 0;
	private int count;
	private String imagePath;
	private String category;

	Goods(String name, int price, int count, String category, String imagePath){
		this.name = name;
		this.price = price;
		this.count = count;
		this.imagePath = imagePath;
		this.category = category;
	}
	
	public void print() {
		//사진에 대한 정보를 콘솔로 출력한다
		System.out.println(getGoodsInfo());
	}
	
	public String getGoodsInfo() {
		return name+";"+price+";"+count+";"+category+";"+imagePath;
	}

	public String getName() {return name;}
	public int getPrice() {return price;}
	public int getCount() {return count;}
	public String getCategory() {return category;}
	public String getPath() {return imagePath;}
	public String getType() {return category;}
	
	public void setCount(int i) {count = i;}
	
	public void setData(String name, int price, int count, String path) {
		this.name = name;
		this.price = price;
		this.count = count;
		this.imagePath = path;
	}
	
	public boolean buyGoods() {
		if(Coin.sum >= getPrice() && getCount() > 0) {
			Coin.sum -= getPrice();
			setCount(getCount() - 1);
			return true;
		}
		else {
			return false;
		}
	}
		
	
	public String description() {
		String str = "";
		
		return str;
	}
}
