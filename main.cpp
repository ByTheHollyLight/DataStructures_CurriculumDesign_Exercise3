#include"huffman.h"

void menu() {	//打印功能菜单
	cout << "Please select a function:" << endl;
	cout << "1: Huffman Compress" << endl;
	cout << "2: Huffman Decompress" << endl;
	cout << "3: Exit" << endl;
}

int main() {
	menu();
	int f;
	while (cin >> f) {	//根据输入的指令执行对应功能
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