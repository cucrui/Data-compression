#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include<string.h>
#include<queue>
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
	int len = strlen(str);
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

int main(){
	FILE *input_file;
	int total_cnt = 0;
	unsigned int c; //声明称无符号整型 负数会导致越界
	cout << "输入文件名 ：" << endl;
	char in_name[256], out_name[256], out_txt[256];
	cin >> in_name;

	int len = strlen(in_name), pos = -1;
	for (int i = 0; i < len; i++) {
		out_name[i] = in_name[i];
		out_txt[i] = in_name[i];
		if (in_name[i] == '.') {
			pos = i;
			break;
		}
	}
	out_name[pos + 1] = 'h'; out_name[pos + 2] = 'u'; out_name[pos + 3] = 'f'; out_name[pos + 4] = '\0';
	out_txt[pos] = '_'; out_txt[pos+1] = 'o'; out_txt[pos+2] = 'u';out_txt[pos+3] = 't';out_txt[pos+4] = '.';
	out_txt[pos+5] = 't';out_txt[pos+6] = 'x';out_txt[pos+7] = 't';out_txt[pos+8] = '\0';

	cout << in_name << " " << out_name << " " << out_txt << endl;
	
	input_file = fopen(in_name, "rb"); //rb模式 而不是r模式！！！

	if(!input_file){
		cout << "can not open file" << endl;
		system("pause");
		return 0;
	}
	while((c = fgetc(input_file)) != EOF){
		if(!index[c]){
			Node *node = new Node();
			index[c] = node;
			node->data = c;
			node->is_leaf = true;
		}
		(index[c]->cnt)++;
		total_cnt++;
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
	fclose(input_file);
	input_file = fopen(in_name, "rb");
	memset(out_stream, 0, sizeof(out_stream));
	int idx = 0, org_idx;
	while ((c = fgetc(input_file)) != EOF) {
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

	FILE *output_file;
	
	output_file = fopen(out_name, "wb");
	out = (BYTE*)malloc(idx / 8);
	for (int i = idx - 1, j = 0; i >= 0; i -= 8) {
		int k = 8, base = 1, tem = i;
		while (k--) {
			out[j] += (BYTE) (base * (out_stream[tem] - '0'));
			tem--;
			base *= 2;
		}
		j++;
	}

	fwrite(out, 1, idx / 8, output_file);
	cout << "debug : " << "compress success" << endl;
	return 0;
}

