package proj4;

public class Coin {
	
	static int sum;

	public static int add100() {
		sum += 100;
		return sum;
	}
	public static int add500() {
		sum += 500;
		return sum;
	}
	public static int add1000() {
		sum += 1000;
		return sum;
	}
	public static int add5000() {
		sum += 5000;
		return sum;
	}
	public static int add10000() {
		sum += 10000;
		return sum;
	}
	public static int add50000() {
		sum += 50000;
		return sum;
	}
	public static int clearCoin() {
		sum = 0;
		return sum;
	}
}