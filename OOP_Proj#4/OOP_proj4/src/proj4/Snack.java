package proj4;

public class Snack extends Goods{
	
	Snack(String name, int price, int count, String category, String imagePath){
		
		super(name, price, count, category, imagePath);
	}
	
	public String description() {
		String str;
		str = "��ǰ�� : " + this.getName() + 
			  "\n���� : " + this.getPrice() +
			  "\n���� : " + this.getCount() +
			  "\n\n���� ��ǰ";
		
		return str;
	}

}