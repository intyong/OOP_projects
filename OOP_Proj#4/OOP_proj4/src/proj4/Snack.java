package proj4;

public class Snack extends Goods{
	
	Snack(String name, int price, int count, String category, String imagePath){
		
		super(name, price, count, category, imagePath);
	}
	
	public String description() {
		String str;
		str = "제품명 : " + this.getName() + 
			  "\n가격 : " + this.getPrice() +
			  "\n개수 : " + this.getCount() +
			  "\n\n과자 제품";
		
		return str;
	}

}
