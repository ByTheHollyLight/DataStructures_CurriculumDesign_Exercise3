#include"node.h"

class heap {	//用一个最小堆来保存各个节点，辅助建立哈夫曼树
public:
	void swap(int i, int j) {	//交换节点位置
		char tmpc = Heap[i].c;
		Heap[i].c = Heap[j].c;
		Heap[j].c = tmpc;

		int tmpf = Heap[i].fre;
		Heap[i].fre = Heap[j].fre;
		Heap[j].fre = tmpf;

		node *tmp = Heap[i].lc;
		Heap[i].lc = Heap[j].lc;
		Heap[j].lc = tmp;

		tmp = Heap[i].rc;
		Heap[i].rc = Heap[j].rc;
		Heap[j].rc = tmp;
	}
	void loadHeap(int source[]) {	//将各字符加入最小堆
		int k = 0;
		for (int i = 0; i < 128; i++)
			if (source[i]) {
				k++;
				Heap[k].c = i;
				Heap[k].fre = source[i];
				Heap[k].lc = Heap[k].rc = NULL;
			}
		n = k;
	}
	void buildHeap() {	//维护最小堆
		for (int i = n / 2; i > 0; i--)
			shiftdown(i);
	}
	bool isLeaf(int pos) {	//判断是否为叶子节点
		return (pos > n / 2) && (pos <= n);
	}
	int leftchild(int pos) {	//返回左孩子的位置
		return 2 * pos;
	}
	int rightchild(int pos) {	//返回右孩子的位置
		return 2 * pos + 1;
	}
	int parent(int pos) {	//返回父亲节点的位置
		return pos / 2;
	}
	void shiftdown(int pos) {	//节点下移函数，用于维护最小堆
		while (!isLeaf(pos) && n > 1) {
			int j = leftchild(pos), rc = rightchild(pos);
			if ((rc <= n) && (Heap[rc].fre < Heap[j].fre))
				j = rc;
			if (Heap[pos].fre <= Heap[j].fre)
				return;
			swap(pos, j);
			pos = j;
		}
	}
	void insert(node *t) {	//向堆中加入一个新节点并维护最小堆
		n++;
		Heap[n].c = t->c;
		Heap[n].fre = t->fre;
		Heap[n].lc = t->lc;
		Heap[n].rc = t->rc;
		int cur = n;
		while ((cur > 1) && Heap[cur].fre < Heap[parent(cur)].fre) {
			swap(cur, parent(cur));
			cur = parent(cur);
		}
	}
	node* buildHuffman() {	//利用当前的最小堆来生成一颗哈夫曼树，返回该树的根节点
		while (n > 1) {
			//每次依次从堆顶依次取出两个节点
			swap(1, n);
			node *a = new node;
			a->c = Heap[n].c;
			a->fre = Heap[n].fre;
			a->lc = Heap[n].lc;
			a->rc = Heap[n].rc;
			n--;
			shiftdown(1);
			swap(1, n);
			node *b = new node;
			b->c = Heap[n].c;
			b->fre = Heap[n].fre;
			b->lc = Heap[n].lc;
			b->rc = Heap[n].rc;
			n--;
			shiftdown(1);
			//把这两个节点分别作为左右孩子，合成一个新的节点并加入最小堆
			node *c = new node;
			c->c = EOF;
			c->fre = a->fre + b->fre;
			c->lc = a;
			c->rc = b;
			if (n == 0)return c;
			insert(c);
		}
	}
	int getN() {	//获取堆中元素个数
		return n;
	}
private:
	int n;	//堆中元素个数
	node Heap[128];
};