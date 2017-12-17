#include"huffman.h"

void menu() {	//��ӡ���ܲ˵�
	cout << "Please select a function:" << endl;
	cout << "1: Huffman Compress" << endl;
	cout << "2: Huffman Decompress" << endl;
	cout << "3: Exit" << endl;
}

int main() {
	menu();
	int f;
	while (cin >> f) {	//���������ָ��ִ�ж�Ӧ����
		switch (f) {
		case 1:
			compress();
			break;
		case 2:
			decompress();
			break;
		case 3:
			return 0;
		default:
			cout << "Illegal input" << endl;
			break;
		}
		cout << endl;
		menu();
	}
	return 0;
}