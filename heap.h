#include"node.h"

class heap {	//��һ����С������������ڵ㣬����������������
public:
	void swap(int i, int j) {	//�����ڵ�λ��
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
	void loadHeap(int source[]) {	//�����ַ�������С��
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
	void buildHeap() {	//ά����С��
		for (int i = n / 2; i > 0; i--)
			shiftdown(i);
	}
	bool isLeaf(int pos) {	//�ж��Ƿ�ΪҶ�ӽڵ�
		return (pos > n / 2) && (pos <= n);
	}
	int leftchild(int pos) {	//�������ӵ�λ��
		return 2 * pos;
	}
	int rightchild(int pos) {	//�����Һ��ӵ�λ��
		return 2 * pos + 1;
	}
	int parent(int pos) {	//���ظ��׽ڵ��λ��
		return pos / 2;
	}
	void shiftdown(int pos) {	//�ڵ����ƺ���������ά����С��
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
	void insert(node *t) {	//����м���һ���½ڵ㲢ά����С��
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
	node* buildHuffman() {	//���õ�ǰ����С��������һ�Ź������������ظ����ĸ��ڵ�
		while (n > 1) {
			//ÿ�����δӶѶ�����ȡ�������ڵ�
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
			//���������ڵ�ֱ���Ϊ���Һ��ӣ��ϳ�һ���µĽڵ㲢������С��
			node *c = new node;
			c->c = EOF;
			c->fre = a->fre + b->fre;
			c->lc = a;
			c->rc = b;
			if (n == 0)return c;
			insert(c);
		}
	}
	int getN() {	//��ȡ����Ԫ�ظ���
		return n;
	}
private:
	int n;	//����Ԫ�ظ���
	node Heap[128];
};