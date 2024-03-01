package proj4;

import java.io.IOException;

public class main {

	public static void main(String[] args) throws IOException {
		   
		VendingMc vendingMachine = new VendingMc("Goods.data");
		new VendingMcFrame(vendingMachine);
	  }

}
