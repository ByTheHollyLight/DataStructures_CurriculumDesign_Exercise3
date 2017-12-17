#include<fstream>
#include<cstring>
#include<map>
#include<string>
#include"heap.h"

map<char, string> code;		//������ѹ��ʱ�洢ÿ���ַ���Ӧ�Ĺ���������
map<string, char> decode;	//�����ڽ�ѹʱ�洢ÿ�������������Ӧ���ַ�
node *root;					//���������ĸ��ڵ�

char toAsc(string s) {	//���ڽ�һ��8λ����01��ת���ɶ�Ӧ��ASCII�ַ�
	char tmp = 0;
	int n = 0;
	while (n < 8) {
		if (s[n] == '1')tmp += 1 << (7 - n);
		n++;
	}
	return tmp;
}

string toString(char c) {	//���ڽ�һ���ַ�ת��Ϊ��8λ�������������ַ�������
	string tmp;
	int n = 8;
	while (n--) {
		tmp += (c & 1) ? '1' : '0';
		c >>= 1;
	}
	reverse(tmp.begin(), tmp.end());
	return tmp;
}

string fullBin(string s, int t) {	//���ڲ���һ���ַ���Ӧ��01�����룬ʹ���ܳ���Ϊ8�ı���������ܳ�������ı�������������Ѹñ���תΪ��Ӧ��ASCII�ַ���
	int l = (t / 8) + ((t % 8) ? 1 : 0);	//������Ҫ����8λ
	int m = 8 * l - s.length();		//������Ҫ�����λ��
	string tmp, ans;
	ans += (char)m;	//���油���λ��
	while (m--)		//����ǰ��0
		tmp += '0';
	tmp += s;
	int i = 7;
	while (i < l * 8) {		//�Ѳ����ı���תΪ��Ӧ��ASCII�ַ���
		string cur;
		for (int k = i - 7; k <= i; k++)
			cur += tmp[k];
		ans += toAsc(cur);
		i += 8;
	}
	return ans;
}

string to32bit(int x) {		//��һ������תΪ���Ӧ��32λ�����������ַ��������������ⳤ��Ϊ32���ַ�����Ӧ�ĳ���Ϊ4���ַ���
	string tmp;
	int n = 32;
	while (n--) {	//��ȡ32λ�ַ���
		tmp += (x & 1) ? '1' : '0';
		x >>= 1;
	}
	reverse(tmp.begin(), tmp.end());
	string ans;
	for (int i = 7; i < 32; i += 8) {	//ת��Ϊ4����Ӧ��ASCII�ַ�
		string t;
		for (int j = i - 7; j <= i; j++)
			t += tmp[j];
		ans += toAsc(t);
	}
	return ans;
}

int toint(string s) {	//��һ������Ϊ32λ��01�ַ���ת�������Ӧ������
	int tmp = 0;
	int n = 0;
	while (n < 32) {
		if (s[n] == '1')tmp += 1 << (31 - n);
		n++;
	}
	return tmp;
}

void pretraverse(node *rt, string cur) {	//ǰ���������������������ÿ���ַ���Ӧ�Ĺ���������
	if (rt->lc == NULL) {
		code[rt->c] = cur;
		return;
	}
	pretraverse(rt->lc, cur + '0');
	pretraverse(rt->rc, cur + '1');
}

void compress() {	//ѹ������
	//�ļ�����
	char input[100], output[100];
	cout << "Please input source file name:" << endl;
	cin >> input;
	cout << "Please input code file name:" << endl;
	cin >> output;
	int count[128];
	memset(count, 0, sizeof(count));
	fstream in;
	in.open(input, ios::in);
	if (!in) {		//����ļ��Ϸ���
		cout << "File not exist or File is empty" << endl;
		return;
	}
	while (1) {		//ͳ���ļ���ÿ���ַ����ֵĴ���
		char tmp = in.get();
		if (in.eof())break;
		count[tmp]++;
	}
	in.close();
	heap temp;	//����һ����С�ѱ�������ַ���Ӧ�Ľڵ�
	temp.loadHeap(count);
	temp.buildHeap();
	int totalnum = temp.getN();
	root = temp.buildHuffman();		//������С�ѽ�����������
	string tmp;
	pretraverse(root, tmp);		//ǰ���������������ȡÿ���ַ���Ӧ�Ĺ���������
	fstream out;
	in.open(input, ios::in);
	out.open(output, ios::out);
	out << (char)totalnum;		//д���ܵ��ַ�����
	map<char, string>::iterator it;		//����map������ȡÿ���ַ���Ӧ�Ĺ���������
	for (it = code.begin(); it != code.end(); it++)		//д������ַ������Ӧ�Ĺ���������
		out << it->first << fullBin(it->second, totalnum);
	string s;	//�ַ���s���ڱ���Դ�ļ���Ӧ��ѹ����ı��루01����
	while (1) {
		char tmp = in.get();
		if (in.eof())break;
		s += code[tmp];
	}
	int m = (8 - s.length() % 8) % 8;	//����ѹ���������0�ĸ���
	out << (char)m;		//д�����0�ĸ���
	string ss;	//�ַ���ss�ǳ�������λ8�ı���֮���ѹ������
	while (m--)
		ss += '0';
	ss += s;
	int len = ss.length();
	out << to32bit(len);	//д��ѹ��������ܳ���
	for (int i = 7; i < len; i += 8) {		//��8λλ��λд��ѹ�������Ӧ��ASCII�ַ�
		string tmp;
		for (int j = i - 7; j <= i; j++)
			tmp += ss[j];
		out << toAsc(tmp);
	}
	in.close();
	out.close();
	cout << "Compression succeed" << endl;
}

void decompress() {		//��ѹ����
	//�ļ�����
	char input[100], output[100];
	cout << "Please input code file name:" << endl;
	cin >> input;
	cout << "Please input target file name:" << endl;
	cin >> output;
	fstream in;
	in.open(input, ios::in);
	if (!in) {		//�ļ��Ϸ��Լ��
		cout << "File not exist or File is empty" << endl;
		return;
	}
	char tmp = in.get();	//��ȡ�ܵ��ַ�����
	int totalnum = (int)tmp;
	int codebyte = (totalnum / 8) + ((totalnum % 8) ? 1 : 0);	//�������������ĳ��ȣ�ռ���ٸ��ֽڣ�
	while (totalnum--) {	//��ȡÿ�������Ӧ���ַ�
		tmp = in.get();		//��ȡ�ַ�
		char t = in.get();		//��ȡ�������������0�ĸ���
		int m = (int)t;
		string s;	//�ַ���s�洢��ǰ�ַ���Ӧ�Ĺ���������
		int rem = codebyte;
		while (rem--) {
			char temp = in.get();
			s += toString(temp);
		}
		int i = m;
		string ss;	//�ַ���ss�洢ȥ�����0��ǰ�ַ��Ĺ���������
		while (i < 8 * codebyte)
			ss += s[i++];
		decode[ss] = tmp;	//��map�������浱ǰ�����Ӧ���ַ�
	}
	tmp = in.get();		//��ȡѹ���������0�ĸ���
	int m = (int)tmp;
	string ml;		//�ַ���ml��Ӧѹ��������ܳ���
	for (int i = 0; i < 4; i++) {
		tmp = in.get();
		ml += toString(tmp);
	}
	int maxlen = toint(ml);
	fstream out;
	out.open(output, ios::out);
	string code;	//��ȡѹ�����뱣�浽code�ַ���
	while (code.length() < maxlen) {
		tmp = in.get();
		code += toString(tmp);
	}
	int i = m, len = code.length();	//�������0����ʼ����
	map<string, char>::iterator it;
	string temp;
	while (i < len) {
		temp += code[i];
		it = decode.find(temp);	//��map������Ѱ�ҵ�ǰ���룬���ж�Ӧ�ַ�������ַ�����û�ж�Ӧ�ַ����������һλ
		if (it != decode.end()) {
			out << decode[temp];
			temp.clear();
		}
		i++;
	}
	in.close();
	out.close();
	cout << "Decompression succeed" << endl;
}