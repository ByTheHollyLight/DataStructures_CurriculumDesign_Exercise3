#include<iostream>

using namespace std;

struct  node {
	int c;		//字符
	int fre;	//出现频率
	node *lc;	//左孩子
	node *rc;	//右孩子
};