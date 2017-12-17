#include<fstream>
#include<cstring>
#include<map>
#include<string>
#include"heap.h"

map<char, string> code;		//用于在压缩时存储每个字符对应的哈夫曼编码
map<string, char> decode;	//用于在解压时存储每个哈夫曼编码对应的字符
node *root;					//哈夫曼树的根节点

char toAsc(string s) {	//用于将一个8位长的01串转化成对应的ASCII字符
	char tmp = 0;
	int n = 0;
	while (n < 8) {
		if (s[n] == '1')tmp += 1 << (7 - n);
		n++;
	}
	return tmp;
}

string toString(char c) {	//用于将一个字符转化为其8位二进制数并以字符串保存
	string tmp;
	int n = 8;
	while (n--) {
		tmp += (c & 1) ? '1' : '0';
		c >>= 1;
	}
	reverse(tmp.begin(), tmp.end());
	return tmp;
}

string fullBin(string s, int t) {	//用于补足一个字符对应的01串编码，使其总长度为8的倍数（这个总长度由最长的编码决定），并把该编码转为对应的ASCII字符串
	int l = (t / 8) + ((t % 8) ? 1 : 0);	//计算需要几个8位
	int m = 8 * l - s.length();		//计算需要补足的位数
	string tmp, ans;
	ans += (char)m;	//保存补足的位数
	while (m--)		//补足前导0
		tmp += '0';
	tmp += s;
	int i = 7;
	while (i < l * 8) {		//把补足后的编码转为对应的ASCII字符串
		string cur;
		for (int k = i - 7; k <= i; k++)
			cur += tmp[k];
		ans += toAsc(cur);
		i += 8;
	}
	return ans;
}

string to32bit(int x) {		//把一个整数转为其对应的32位二进制数（字符串），并返回这长度为32的字符串对应的长度为4的字符串
	string tmp;
	int n = 32;
	while (n--) {	//获取32位字符串
		tmp += (x & 1) ? '1' : '0';
		x >>= 1;
	}
	reverse(tmp.begin(), tmp.end());
	string ans;
	for (int i = 7; i < 32; i += 8) {	//转化为4个对应的ASCII字符
		string t;
		for (int j = i - 7; j <= i; j++)
			t += tmp[j];
		ans += toAsc(t);
	}
	return ans;
}

int toint(string s) {	//把一个长度为32位的01字符串转化成其对应的整数
	int tmp = 0;
	int n = 0;
	while (n < 32) {
		if (s[n] == '1')tmp += 1 << (31 - n);
		n++;
	}
	return tmp;
}

void pretraverse(node *rt, string cur) {	//前序遍历哈夫曼树，并生成每个字符对应的哈夫曼编码
	if (rt->lc == NULL) {
		code[rt->c] = cur;
		return;
	}
	pretraverse(rt->lc, cur + '0');
	pretraverse(rt->rc, cur + '1');
}

void compress() {	//压缩程序
	//文件定向
	char input[100], output[100];
	cout << "Please input source file name:" << endl;
	cin >> input;
	cout << "Please input code file name:" << endl;
	cin >> output;
	int count[128];
	memset(count, 0, sizeof(count));
	fstream in;
	in.open(input, ios::in);
	if (!in) {		//检查文件合法性
		cout << "File not exist or File is empty" << endl;
		return;
	}
	while (1) {		//统计文件中每个字符出现的次数
		char tmp = in.get();
		if (in.eof())break;
		count[tmp]++;
	}
	in.close();
	heap temp;	//建立一个最小堆保存各个字符对应的节点
	temp.loadHeap(count);
	temp.buildHeap();
	int totalnum = temp.getN();
	root = temp.buildHuffman();		//根据最小堆建立哈夫曼树
	string tmp;
	pretraverse(root, tmp);		//前序遍历哈夫曼树获取每个字符对应的哈夫曼编码
	fstream out;
	in.open(input, ios::in);
	out.open(output, ios::out);
	out << (char)totalnum;		//写入总的字符个数
	map<char, string>::iterator it;		//遍历map容器获取每个字符对应的哈夫曼编码
	for (it = code.begin(); it != code.end(); it++)		//写入各个字符和其对应的哈夫曼编码
		out << it->first << fullBin(it->second, totalnum);
	string s;	//字符串s用于保存源文件对应的压缩后的编码（01串）
	while (1) {
		char tmp = in.get();
		if (in.eof())break;
		s += code[tmp];
	}
	int m = (8 - s.length() % 8) % 8;	//计算压缩编码填充0的个数
	out << (char)m;		//写入填充0的个数
	string ss;	//字符串ss是长度修正位8的倍数之后的压缩编码
	while (m--)
		ss += '0';
	ss += s;
	int len = ss.length();
	out << to32bit(len);	//写入压缩编码的总长度
	for (int i = 7; i < len; i += 8) {		//以8位位单位写入压缩编码对应的ASCII字符
		string tmp;
		for (int j = i - 7; j <= i; j++)
			tmp += ss[j];
		out << toAsc(tmp);
	}
	in.close();
	out.close();
	cout << "Compression succeed" << endl;
}

void decompress() {		//解压程序
	//文件定向
	char input[100], output[100];
	cout << "Please input code file name:" << endl;
	cin >> input;
	cout << "Please input target file name:" << endl;
	cin >> output;
	fstream in;
	in.open(input, ios::in);
	if (!in) {		//文件合法性检查
		cout << "File not exist or File is empty" << endl;
		return;
	}
	char tmp = in.get();	//读取总的字符个数
	int totalnum = (int)tmp;
	int codebyte = (totalnum / 8) + ((totalnum % 8) ? 1 : 0);	//计算哈夫曼编码的长度（占多少个字节）
	while (totalnum--) {	//获取每个编码对应的字符
		tmp = in.get();		//读取字符
		char t = in.get();		//读取哈夫曼编码填充0的个数
		int m = (int)t;
		string s;	//字符串s存储当前字符对应的哈夫曼编码
		int rem = codebyte;
		while (rem--) {
			char temp = in.get();
			s += toString(temp);
		}
		int i = m;
		string ss;	//字符串ss存储去除填充0后当前字符的哈夫曼编码
		while (i < 8 * codebyte)
			ss += s[i++];
		decode[ss] = tmp;	//用map容器保存当前编码对应的字符
	}
	tmp = in.get();		//获取压缩编码填充0的个数
	int m = (int)tmp;
	string ml;		//字符串ml对应压缩编码的总长度
	for (int i = 0; i < 4; i++) {
		tmp = in.get();
		ml += toString(tmp);
	}
	int maxlen = toint(ml);
	fstream out;
	out.open(output, ios::out);
	string code;	//获取压缩编码保存到code字符串
	while (code.length() < maxlen) {
		tmp = in.get();
		code += toString(tmp);
	}
	int i = m, len = code.length();	//忽略填充0，开始解码
	map<string, char>::iterator it;
	string temp;
	while (i < len) {
		temp += code[i];
		it = decode.find(temp);	//在map容器中寻找当前编码，若有对应字符则输出字符，若没有对应字符则继续读下一位
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