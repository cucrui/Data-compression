#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include<string.h>
#include<queue>
#include"huffman_code.h"
using namespace std;

typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;

struct Node{
	Node *parent, *l_child, *r_child;
	BYTE data;
	int cnt;
	int code_len;
	bool is_leaf;
	char code[20];
	Node(){
		parent = NULL;
		l_child = NULL;
		r_child = NULL;
		data = '0';
		cnt = 0;
		is_leaf = false;
	}
	Node(Node *p, Node *l, Node*r, BYTE ch, int num, bool leaf){ //initial Node
		parent = p;
		l_child = l;
		r_child = r;
		data = ch;
		cnt = num;
		is_leaf = leaf;
	}
};

struct cmp{
	bool operator()(Node *a, Node *b) const{
		return a->cnt > b->cnt;		
	}
};

Node *index[256];
priority_queue<Node*, vector<Node*>, cmp> pq;
char out_stream[5000000006];
BYTE *out;

void gen_code(Node *leaf){
	int code_len = 0, idx = 0;
	Node *tem = leaf;
	while(tem -> parent != NULL){
		Node *p = tem->parent;
		if (tem == p->r_child) leaf->code[idx++] = '1';
		else leaf->code[idx++] = '0';
		tem = p;
		code_len++;
	}
	char *str = leaf->code;
	str[idx] = '\0';
	int len = (int)strlen(str);
	for (int i = 0; i < len / 2 ; i++) {
		swap(str[i], str[len - i - 1]);
	}
	index[leaf->data]->code_len = code_len;
}

void dfs_code(Node *now){
	if(now->is_leaf){
		gen_code(now);
		return ;
	}
	dfs_code(now->l_child);
	dfs_code(now->r_child);
	return ;
}

bool flag = false;
void dfs_decode(char *c, Node *now, int *len) {
	if (now->is_leaf) {
		cout << (char)now->data;
		flag = true;
		*len = now->code_len;
		return;
	}
	if (!now->l_child || !now->r_child) return;
	if (flag) return;
	if(*c == '1') dfs_decode(c + 1, now->r_child, len);
	else dfs_decode(c + 1, now->l_child, len);
}


void huffmanCode(BYTE *input_file, FILE *output_file, int size){
	int total_cnt = 0;
	BYTE c;
	for(int i = 0; i < size; i++){
		c = input_file[i];
		if(!index[c]){
			Node *node = new Node();
			index[c] = node;
			node->data = c;
			node->is_leaf = true;
		}
		(index[c]->cnt)++;
		total_cnt++;
	}

	for(int i = 0; i < 256; i++){
		if(index[i]) pq.push(index[i]);
	}

	while(pq.size() != 1){
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();pq.pop();
		Node *fa = new Node(NULL, right, left, '0', (left->cnt) + (right->cnt), false);
		left->parent = fa; right->parent = fa;
		pq.push(fa);
	}

	Node *root = pq.top();
	
	cout << root->cnt << " "  << total_cnt << endl;

	dfs_code(root);
	
	memset(out_stream, 0, sizeof(out_stream));
	int idx = 0, org_idx;
	for(int i = 0; i < size; i++){
		BYTE c = input_file[i];
		char *str = index[c]->code;
		int len = index[c]->code_len;
		for (int j = 0; j < len; j++) out_stream[idx++] = str[j];
	}
	org_idx = idx;
	out_stream[idx] = '\0';

	if (idx % 8 != 0) {
		for (int i = idx; i < idx + 8 - (idx % 8); i++) {
			out_stream[i] = '0';
		}
		idx += (8 - (idx % 8));
	}
	BYTE *out = (BYTE*)malloc(idx / 8);
	for (int i = idx - 1, j = 0; i >= 0; i -= 8) {
		int k = 8, base = 1, tem = i;
		while (k--) {
			out[j] += (BYTE) (base * (out_stream[tem] - '0'));
			tem--;
			base *= 2;
		}
		j++;
	}
	fwrite(out, 1, idx/8, output_file);
	cout << "debug : " << "compress success" << endl;
}
