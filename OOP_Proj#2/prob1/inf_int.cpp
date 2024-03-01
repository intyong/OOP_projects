#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <string>
/*
Originally written by
��ǻ�Ͱ��к�
������
*/

inf_int::inf_int()
{
	this->digits = new char[2];	// �����Ҵ�

	this->digits[0] = '0';		// default �� 0 ����
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100];

	if (n < 0) {		// ���� ó��
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// ���ڸ� ���ڿ��� ��ȯ�ϴ� ����
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// ������ ������ 0�� ���
		new (this) inf_int();	// ������ ��ȣ��...gcc���� �����Ͽ����� �ִٰ� ��. inf_int()�� ��� ������ �ν��Ͻ��� ������. 
	}
	else {
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const char* str)
{
	char buf[100];
	
	if (str[0] == '-')
	{
		int count = 0;
		this->thesign = false;
		for (int i = strlen(str) - 1; i > 0; i--)
		{
			buf[count] = str[i];
			count++;
		}
		buf[count] = '\0';

		this->length = count;
		this->digits = new char[count + 1];
		strcpy(this->digits, buf);
	}
	else
	{
		this->thesign = true;
		int count = 0;
		for (int i = strlen(str) - 1; i >= 0; i--)
		{
			buf[count] = str[i];
			count++;
		}
		buf[count] = '\0';

		this->length = count;
		this->digits = new char[count + 1];
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const inf_int& a) {
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}

inf_int::~inf_int() {
	delete digits;		// �޸� �Ҵ� ����
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// �̹� ���ڿ��� ���� ��� ����.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	// we assume 0 is always positive.
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// ��ȣ�� ����, ������ ��ġ�ؾ���.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	if (strcmp(a.digits, b.digits) == 1)
	{
		if (a.thesign == true) return true;
		else if (a.thesign == false) return false;
	}
	else
	{
		if (b.thesign == true) return false;
		else if (b.thesign == false) return true;
	}
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {	// ������ ��ȣ�� ���� ��� + �����ڷ� ����
		for (i = 0; i < a.length; i++) {
			c.Add(1, a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(1, b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// ������ ��ȣ�� �ٸ� ��� - �����ڷ� ����
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;
	
	if (a.thesign == b.thesign)
	{
		if (a.length > b.length || a.length == b.length)
		{
			for (i = 0; i < a.length; i++) {
				c.Add(1, a.digits[i], i + 1);
			}
			for (i = 0; i < b.length; i++) {
				c.Add(0, b.digits[i], i + 1);
			}
			if (a.thesign == false) c.thesign = false;
			
			return c;
		}
		else if (b.length > a.length)
		{
			for (i = 0; i < b.length; i++) {
				c.Add(1, b.digits[i], i + 1);
			}
			for (i = 0; i < a.length; i++) {
				c.Add(0, a.digits[i], i + 1);
			}			
			if (b.thesign == true) c.thesign = false;

			return c;
		}
	}
	else
	{
		c = b;
		c.thesign = a.thesign;

		return a + c;
	}
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	string temps;

	inf_int result;
	inf_int temp;

	inf_int lng;
	inf_int sht;

	if (a.length >= b.length)
	{
		lng = a;
		sht = b;
	}
	else
	{
		lng = b;
		sht = a;
	}

	for (int i = 0; i < lng.length; i++)
	{
		inf_int c;
		for (int j = 0; j < sht.length; j++)
		{
			temps = to_string((lng.digits[i] - '0') * (sht.digits[j] - '0'));
			for (int k = 0; k < j; k++)
			{
				temps.push_back('0');
			}
			for (int k = 0; k < i; k++)
			{
				temps.push_back('0');
			}
			temp = inf_int(temps.c_str());
			c = c + temp;
		}
		result = result + c;
	}
	if (a.thesign == b.thesign) result.thesign = true;
	else if (a.thesign != b.thesign) result.thesign = false;

	return result;
}

ostream& operator<<(ostream& out, const inf_int& a)
{
	int i = 0;
	int sum = 0;
	for (i = a.length - 1; i >= 0; i--) {

		sum += (a.digits[i] - '0');
	}
	if (a.thesign == false && sum != 0) {
		out << '-';
	}
	sum = 0;
	for (i = a.length - 1; i >= 0; i--) {

		sum += (a.digits[i] - '0');
		if (sum != 0) out << a.digits[i];
	}
	if (sum == 0 && a.digits[0] == '0') out << '0';
	
	return out;
}

void inf_int::Add(int option, const char num, const unsigned int index)
{
	if (option == 1)
	{
		if (this->length < index) {
			this->digits = (char*)realloc(this->digits, index + 1);

			if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
				cout << "Memory reallocation failed, the program will terminate." << endl;

				exit(0);
			}

			this->length = index;					// ���� ����
			this->digits[this->length] = '\0';	// �ι��� ����
		}

		if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
			this->digits[index - 1] = '0';
		}

		this->digits[index - 1] += num - '0';	// �� ����


		if (this->digits[index - 1] > '9') {	// �ڸ��ø��� �߻��� ���
			this->digits[index - 1] -= 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
			Add(1, '1', index + 1);			// ���ڸ��� 1�� ���Ѵ�
		}
	}
	else if (option == 0)
	{
		if (this->length < index) {
			this->digits = (char*)realloc(this->digits, index + 1);

			if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
				cout << "Memory reallocation failed, the program will terminate." << endl;

				exit(0);
			}

			this->length = index;					// ���� ����
			this->digits[this->length] = '\0';	// �ι��� ����
		}

		if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
			this->digits[index - 1] = '0';
		}

		this->digits[index - 1] -= num - '0';	// �� ����


		if (this->digits[index - 1] < '0') {
			this->digits[index - 1] += 10;
			Add(0, '1', index + 1);
		}
	}
}