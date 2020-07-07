/*
DB001_Project2
��ǻ�Ͱ��а�_12161579_������
*/

#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
using namespace std;

int N;//�� ���(���)�� �� ��Ʈ������ ����(��������)
int BLOCK_SIZE;
int currentBlock;//���ο� ��� ���� �� ���number ����

//Physical offset of a block in the 
//B+Tree binary file is calculated in the following way :
//12 + ((BID - 1) * BlockSize)

class BPtree;
BPtree* newBPtree;//global declaration of new B+ tree

class Node {//A node is corresponding to a block. 
			//Thus, the size of each node exactly one block.
public:
	friend BPtree;
	vector<pair<int, int>> entry;//key, value���� ���� ����
	Node** pointer;
	Node* parentNode;
	Node* nextLeaf;//���� leaf���� ���� leaf��带 ����ų ������
	Node* prevLeaf;//�ڱ� ������ leaf��带 ����Ŵ

	int* bid;//non-leaf��忡���� bid�� ���� �迭
	int BID;//�ش� ���(���)�� ���ID�� ��� ����
	int size;//�ش� ����� entry ����
	bool isLeaf;//�ش� ��尡 leaf���� �ƴ��� ����
	bool isRoot;//�ش� ��尡 root������� �ƴ��� ����
	int nextBID;
	int nextLevelBID;
	Node() {//�⺻������
		this->bid = new int[N + 1];//non-leaf����� ��� ���� level�� ���鿡 ���� block ID�� ���� �־�� ��
		for (int i = 0;i <= N;i++)
			this->bid[i] = 0;
		BID = 0;
		size = 0;
		isLeaf = true;
		isRoot = true;
		nextBID = 0;
		nextLevelBID = 0;
		nextLeaf = NULL;
		prevLeaf = NULL;
		parentNode = NULL;
		pointer = NULL;
	}
	Node(int BID) {
		this->bid = new int[N + 1];//non-leaf����� ��� ���� level�� ���鿡 ���� block ID�� ���� �־�� ��
		for (int i = 0;i <= N;i++)
			this->bid[i] = 0;
		this->BID = BID;
		this->pointer = new Node * [N + 1];
		for (int i = 0;i <= N;i++)
			this->pointer[i] = NULL;
		this->parentNode = NULL;
		this->nextLeaf = NULL;
		this->prevLeaf = NULL;
		this->size = 0;
		this->isLeaf = true;
		this->isRoot = true;
		this->nextBID = 0;
		this->nextLevelBID = 0;
	}
	bool isFull();
};

bool Node::isFull() {
	if (this->entry.size() >= N)
		return true;
	else
		return false;
}

class BPtree {
public:
	Node* rootNode;
	int size;//number of keys in the tree
	int depth;
	BPtree() {//�⺻������
		this->rootNode = NULL;
		this->depth = 0;
		this->size = 0;
	}
	//constructor with params
	BPtree(const string bptree_bin_file) {
		this->rootNode = NULL;
		this->depth = 0;
		this->size = 0;
		//this->createHeader(bptree_bin_file, 1);//�������(12bytes)
	}
	void createHeader(const string, int);
	void insertSample(const string, const string);
	void writeFile(const string);
	void insert(pair<int, int>);
	void insertNode(Node*, pair<int, int>);
	void insertParent(Node*, Node*);
	void printTree(Node*, const string, const string);
	void BFS(Node*);
	Node* splitNode(Node*, pair<int, int>);
	int inMemoryPointSearch(int);//search-key�� key�� �Է����� �޴´�
	//int blockPointSearch(const string, int);
	void pointSearchSample(const string, const string, const string);
	Node* findNode(int key);//pointSearch �Լ��� ����. value�� �����ϴ� ��� �ش� �����ü�� �����Ѵ�
	vector<pair<int, int>> inMemoryRangeSearch(const string, int, int);
	void rangeSearchSample(const string, const string, const string);
	void setLeafBID();
	void setNonLeafBID();
	//int* search(int key);
	//int* search(int startRange, int endRange);
};
void BPtree::setLeafBID() {
	Node* tempNode = newBPtree->rootNode;
	int bid = 1;//rootNode�� 1������̹Ƿ� 2������ ����
	while (!tempNode->isLeaf) {//leaf������ ������
		tempNode = tempNode->pointer[0];
	}
	//��� leaf����
	while (tempNode != NULL) {
		tempNode->BID = bid;
		if (tempNode->nextLeaf != NULL) {
			tempNode->nextBID = bid + 1;
		}
		tempNode = tempNode->nextLeaf;
		bid++;
	}
	currentBlock = bid;
	cout << '\n';
}
void BPtree::setNonLeafBID() {
	//non-leaf��忡 ���� ���ID ��巹��
	Node* currentLevel = newBPtree->rootNode, *tempNode = NULL;
	while (!currentLevel->isLeaf) {//leaf������ ������
		currentLevel = currentLevel->pointer[0];
	}
	currentLevel = currentLevel->parentNode;//�ٷ� �� ������ �ö�
	int i = 0;
//	this->BFS(this->rootNode);
	while (currentLevel != NULL) {
		tempNode = currentLevel;
		while (tempNode != NULL) {
			for (i = 0;i < tempNode->entry.size();i++) {
				//���� non-leaf��忡�� ����Ű�� ��� ����� ��� id���� ����ų bid�迭
				tempNode->bid[i] = tempNode->pointer[i]->BID;
			}
			tempNode->bid[i] = tempNode->pointer[i]->BID;//����� ������ ������
			tempNode->BID = currentBlock;//non-leaf�� ���� BID
			currentBlock++;
			tempNode = tempNode->nextLeaf;//���� nonleaf�� �Ѿ��
		}
		currentLevel = currentLevel->parentNode;//���� �ö󰡾� �Ѵ�
	}
}

void BPtree::createHeader(const string fileName, int bid) {
	//binary file �̸� ����
	ofstream writeFile(fileName, ios::out | ios::binary);
	if (this->rootNode == NULL) {
		//this->rootNode = new Node(1);//��Ʈ����� ��� id�� 1�̰�, ��Ʈ���� N����ŭ ����
		this->depth++;//��Ʈ��� �ϳ��ۿ� ������ depth���� 1
		//�ʱ���� 12byte (block size, root block, depth) write
		this->size++;

		writeFile.write((char*)&BLOCK_SIZE, sizeof(int));
		writeFile.write((char*)&bid, sizeof(int));
		writeFile.write((char*)&this->depth, sizeof(int));
	}
	else {//���� �� ��ƮBID ������Ʈ �ϴ°��
		writeFile.write((char*)&BLOCK_SIZE, sizeof(int));
		writeFile.write((char*)&bid, sizeof(int));
		writeFile.write((char*)&this->depth, sizeof(int));
	}
	writeFile.close();
	return;
}

void BPtree::printTree(Node* currentNode, const string bptree_bin_file, const string printFile) {
	ofstream outputFile(printFile, ios::out | ios::binary);
	if (outputFile.is_open()) {

		Node* tempNode = NULL;
		int i = 0;
		int currentDepth = 0;
		while (currentNode != NULL) {
			tempNode = currentNode;
			outputFile <<"\n<" << currentDepth << ">\n";
				while (tempNode != NULL) {
					for (i = 0;i < tempNode->entry.size();i++) {
						outputFile << tempNode->entry[i].first << ", ";
					}
					tempNode = tempNode->nextLeaf;
				}
			currentNode = currentNode->pointer[0];//�� �� �����ͷ� ������ �� ���ʿ��� �������� key���� ���
			currentDepth++;
		}
		outputFile << '\n';
	}
	outputFile.close();
}

void BPtree::writeFile(const string bptree_bin_file) {
	//Ʈ���� ��ȸ�ϸ� ���Ͽ� �Է�
	ofstream outputFile(bptree_bin_file, ios::app | ios::binary);//��� ����� ������ ���� �߰���

	Node* tempNode = newBPtree->rootNode;
	while (!tempNode->isLeaf) {//leaf������ ������
		tempNode = tempNode->pointer[0];
	}
	//��� leaf���� �Է�
	int i, zero = 0;
	while (tempNode != NULL) {
		//���� ����� key-value�� �Է�
		i = 0;
		if (tempNode->isFull()) {//���� ���� �� ����� �ڿ� ���� 0�� �������� �ʾƵ� ��
			for (i = 0;i < N;i++) {
				outputFile.write((char*)&tempNode->entry[i].first, sizeof(int));//key
				outputFile.write((char*)&tempNode->entry[i].second, sizeof(int));//value
			}
		}
		else {//���� ���� ���� ����� ��� �ڿ� 0�� �ٿ���� ��
			for (i = 0;i < tempNode->entry.size();i++) {
				outputFile.write((char*)&tempNode->entry[i].first, sizeof(int));//key
				outputFile.write((char*)&tempNode->entry[i].second, sizeof(int));//value
			}
			for (int j = i; j < N;j++) {
				outputFile.write((char*)&zero, sizeof(int));
				outputFile.write((char*)&zero, sizeof(int));
			}
		}

		tempNode = tempNode->nextLeaf;
		//���� ����� BId�Է�
		if(tempNode!=NULL)
			outputFile.write((char*)&tempNode->BID, sizeof(int));
		else//���� ��尡 ���� ���(���� level���� ���� �������� ��ġ�� ���)
			outputFile.write((char*)&zero, sizeof(int));
	}

	//non-leaf���� �Է�
	Node* currentLevel = newBPtree->rootNode;
	while (!currentLevel->isLeaf) {//leaf������ ������
		currentLevel = currentLevel->pointer[0];
	}
	currentLevel = currentLevel->parentNode;//�ٷ� �� ������ �ö�

	i = 0;
	while (currentLevel != NULL) {
		tempNode = currentLevel;
		while (tempNode != NULL) {
			if (tempNode->isFull()) {//���� ���� �� ����� �ڿ� ���� 0�� �������� �ʾƵ� ��
				for (i = 0;i < N;i++) {
					outputFile.write((char*)&tempNode->bid[i], sizeof(int));//BID
					outputFile.write((char*)&tempNode->entry[i].first, sizeof(int));//key
				}
			}
			else {//���� ���� ���� ����� ��� �ڿ� 0�� �ٿ���� ��(��� ũ�⸸ŭ)
				for (i = 0;i < tempNode->entry.size();i++) {
					outputFile.write((char*)&tempNode->bid[i], sizeof(int));//BID
					outputFile.write((char*)&tempNode->entry[i].first, sizeof(int));//key
				}
				for (int j = i; j < N;j++) {
					outputFile.write((char*)&zero, sizeof(int));
					outputFile.write((char*)&zero, sizeof(int));
				}
			}
			//���� ����� BID�Է�
			if (tempNode->nextLeaf != NULL)
				outputFile.write((char*)&tempNode->bid[i], sizeof(int));
			else {//���� ��尡 ���� ���(���� level���� ���� �������� ��ġ�� ���)
				if (tempNode->parentNode != NULL) {
					if (tempNode->parentNode->isRoot)
						outputFile.write((char*)&this->rootNode->BID, sizeof(int));
					else
						outputFile.write((char*)&zero, sizeof(int));
				}
				else if (tempNode->isRoot) {
					outputFile.write((char*)&tempNode->bid[i], sizeof(int));
				}
			}

			tempNode = tempNode->nextLeaf;//���� nonleaf�� �Ѿ��
		}
		currentLevel = currentLevel->parentNode;//���� �ö󰡾� �Ѵ�
	}
	outputFile.close();
}

void BPtree::insertSample(const string bptree_bin_file, const string fileName) {//key, rootid, value
	
	ifstream inputFile(fileName);//�Է°����� �����̸��� ������ B plus tree�� �����Ѵ�.
	string line, firstInt, secondInt;
	int key = 0, value = 0, bid = 0;
	if(inputFile.is_open()) {
		//�̹� ����� ������ rootnode�� �ִٰ� ����, sample input file�� ������ �о�� ����
		while (getline(inputFile, line)) {//read in entire line(row)
			stringstream ss(line);//
			getline(ss, firstInt, ',');//key�� ���� ����
			key = stoi(firstInt);//change to int
			getline(ss, secondInt);//�״��� value(������)����
			value = stoi(secondInt);//change to int

			//cout << '\n'<<"Insert Key: " << key << " Value: " << value << '\n';
			pair<int, int> newPair = { key, value };
			//cout << "made pair <" << newPair.first << ", " << newPair.second << ">";
			this->insert(newPair);
		}
		//this->BFS(this->rootNode);
		this->setLeafBID();
		this->setNonLeafBID();
		this->BFS(this->rootNode);
	}
	else {//input���� ���µ� ������ �ִ� ���
		cout << "Failed to open file.\n";//��� �޽��� ���
		return;
	}
	//cout << this->rootNode->BID << '\n';
	this->createHeader(bptree_bin_file, this->rootNode->BID);//������Ʈ�� ��� ����
	//bin���Ͽ� �ش� Ʈ���� ���� �Է�
	this->writeFile(bptree_bin_file);

	//apply inserted bptree to the bin file
	//updateTree(bptree_bin_file);
	//printTree(newBPtree->rootNode, "print.txt");
	inputFile.close();//sample input���� �ݱ�
}

//int BPtree::blockPointSearch(const string bptree_bin_file, int key) {
//	ifstream inputFile(bptree_bin_file, ios::in | ios::binary);//bin������ �о key���� ã�´�
//	int value = 0, rootBID = 0;
//
//	if (inputFile.is_open()) {
//		inputFile.read((char*)&rootBID, sizeof(int));
//		inputFile.read((char*)&rootBID, sizeof(int));
//
//		while (!inputFile.eof()) {
//			for (int i = 0;i < 4;i++) {
//				inputFile.read((char*)&key, sizeof(int));//���Ͽ��� ���� �о�ͼ� key�� ���� �� 
//				inputFile.read((char*)&value, sizeof(int));
//				cout << key << "," << value << "\n";//���
//			}
//			inputFile.read((char*)&n, sizeof(int));//����BID
//			cout << "NEXT BID: " << n << '\n';
//		}
//		inputFile.clear();
//		inputFile.seekg(ios::cur);
//		cout << "Pointer seek:" << (char)inputFile.get() << '\n';
//	}
//
//	return value;
//}

int BPtree::inMemoryPointSearch(int key) {
	Node* tempNode = newBPtree->rootNode;//Point Query�� �������� �׻� ��Ʈ��忡�� �����Ѥ���.
	//�������� ��Ʈ������ �����Ͽ�, �Է°����� ���� key���� ���ϴ� ������ ã�´�.
	//�� non-leaf������ routing������ �� ���̸�, 
	//tempNode�� leaf��尡 �� ������ �ش� �ݺ����� ����Ѵ�.
	while (!tempNode->isLeaf) {
		int i = 0;
		for (i = 0;i < tempNode->entry.size() - 1;i++) {
			if (tempNode->isRoot && tempNode->entry.size() == 1) {
				if (key >= tempNode->entry.begin()->first) {
					i++;
					break;
				}
				else
					break;
			}
			if (key > tempNode->entry[i].first && key < tempNode->entry[i + 1].first) {//������ ��ġ�� ã�� ���
				i++;//i��° ���� ������(i+1)�� ���� ������
				break;
			}
			else if (key < tempNode->entry[i].first) {//Ki���� ������ Ż��
				break;
			}
			else if (key >= tempNode->entry[i].first) {//key���� ���� ��忡���� i��° key������ ū ���
				continue;
			}
		}
		if (key >= tempNode->entry[i].first) {//������ key������ ū ��� �� ������ �����ͷ� ������
			i++;
		}
		tempNode = tempNode->pointer[i];//�� �ݺ����� ���� �ش�Ǵ� �ڽĳ��� ���� ġȯ�ȴ�
	}

	//������� �Դٸ� tempNode�� �ݵ�� leaf��忩�� �Ѵ�
	for (int i = 0;i < tempNode->entry.size();i++) {
		if (tempNode->entry[i].first == key)
			return tempNode->entry[i].second;//second�� �ش� i��°�� ���ϴ� value���� ���� �ִ�
	}
	return -1;//����� ��ã�� ���, -1�� �����Ѵ�.
}
void BPtree::pointSearchSample(const string bptree_bin_file, const string input_file, const string output_file) {
	ifstream inputFile(input_file);//�Է°����� �����̸��� ������ B plus tree�� �����Ѵ�.
	ofstream outputFile(output_file, ios::out);
	string line, firstInt, secondInt;
	int key = 0, value = 0, bid = 0;
	if (inputFile.is_open()) {
		//�̹� ����� ������ rootnode�� �ִٰ� ����, sample input file�� ������ �о�� ����
		while (getline(inputFile, line)) {//read in entire line(row)
			if (line == "")
				break;
			key = stoi(line);
			cout << '\n' << "search Key: " << key << '\n';
			value = this->inMemoryPointSearch(key);
			//value = this->blockPointSearch(bptree_bin_file, key);
			outputFile << key << ", " << value << '\n';
		}
	}
	else {//input���� ���µ� ������ �ִ� ���
		cout << "Failed to open file. :(\n";//��� �޽��� ���
		return;
	}
	//apply inserted bptree to the bin file
	//updateTree(bptree_bin_file);
	//printTree(newBPtree->rootNode, "print.txt");
	inputFile.close();//sample input���� �ݱ�
	outputFile.close();
}


vector<pair<int, int>> BPtree::inMemoryRangeSearch(const string bptree_bin_file, int key1, int key2) {
	ifstream inputFile(bptree_bin_file);//bin������ �о key1�� key2���� ������ ��� ��ȯ�Ѵ�

	vector<pair<int, int>> pairs;
	Node* tempNode = findNode(key1);
	//�ش� ����� key���� �� key1�� key2������ ������ ������ ���� ������ �ݺ��Ѵ�
	//key1�� �ش� ����� ù��° key���� ���Ͽ� key1�� �� ũ�ų�,
	//key2�� �ش� ����� ������ key���� ���Ͽ� key2�� �� ������ �ȴ�
	while (tempNode != NULL) {
		//if (key1 >= tempNode->entry[0].first || key2 <= tempNode->entry[N - 1].first)
		for (int i = 0;i < tempNode->entry.size();i++) {
			if (tempNode->entry[i].first >= key1 &&
				tempNode->entry[i].first <= key2) {//key1�� key2���� ������ ���� �ֵ��� ��� ���Ϳ� ����
				pairs.push_back(tempNode->entry[i]);
			}
		}
		//���� leaf���� �̵�
		tempNode = tempNode->nextLeaf;
	}
	return pairs;
}

void BPtree::rangeSearchSample(const string bptree_bin_file, const string input_file, const string output_file) {
	ifstream inputFile(input_file);//�Է°����� �����̸��� ������ B plus tree�� �����Ѵ�.
	ofstream outputFile(output_file, ios::out);
	string line, firstInt, secondInt;
	vector<pair<int, int>> pair;
	int key1 = 0, key2 = 0, value = 0, bid = 0;
	if (inputFile.is_open()) {
		//�̹� ����� ������ rootnode�� �ִٰ� ����, sample input file�� ������ �о�� ����
		while (getline(inputFile, line)) {//read in entire line(row)
			stringstream ss(line);//
			getline(ss, firstInt, ',');//key�� ���� ����
			key1 = stoi(firstInt);//change to int
			getline(ss, secondInt);//�״��� value(������)����
			key2 = stoi(secondInt);//change to int

			cout << '\n' << "Find all pairs among Key1: " << key1 << " Key2: " << key2 << '\n';
			pair = this->inMemoryRangeSearch(bptree_bin_file, key1, key2);
			for (auto& i : pair) {
				outputFile << i.first << "," << i.second << " ";
			}
			outputFile << '\n';
		}
	}
	else {//input���� ���µ� ������ �ִ� ���
		cout << "Failed to open file. :(\n";//��� �޽��� ���
		return;
	}
	inputFile.close();//sample input���� �ݱ�
	outputFile.close();
}

bool comp(pair<int, int> a, pair<int, int> b) {
	return a.first < b.first;//sort on keys
}

void BPtree::insertNode(Node* node, pair<int, int> newPair) {//key���� �� key���� ����Ű�� ������
	//�� key���� ������ ��带 ã�� ����

	if (!node->isFull()) {//�ش� ��尡 �� �� ���°� �ƴ϶��
		//cout << "���Ⱑ ����\n";
		node->entry.push_back(newPair);//���� ����
	}
	else {//�ش� ��尡 �� �� ������ ���
		//��带 2���� ���� �ɰ��� �Ѵ�
		if (!node->isLeaf) {
			node->entry.push_back(newPair);
			sort(node->entry.begin(), node->entry.end(), comp);//sort by key values

			Node* primeNode = splitNode(node, newPair);//���� ���� �θ���(Ȥ�� ������ �θ���)
			/*cout << "primeNode: ";
			for (auto& a : primeNode->entry)
				cout << a.first << " ";
			cout << '\n';*/
		}
		else {
			Node* primeNode = splitNode(node, newPair);//���� ���� �θ���(Ȥ�� ������ �θ���)

			//�� ù��° key���� ������ ù��° �����͵��󳻷����� ����
			if (newPair.first < primeNode->parentNode->entry[0].first)
				insertNode(primeNode->parentNode->pointer[0], newPair);
			else if (newPair.first >= primeNode->parentNode->entry[primeNode->parentNode->entry.size() - 1].first)//������ �����ͷ� �������� ���
				insertNode(primeNode->parentNode->pointer[primeNode->parentNode->entry.size()], newPair);
			else {
				//������ ���� ��ġ�� Ž��
				int i = 0;
				for (i = 0;i < primeNode->parentNode->entry.size() - 1;i++) {
					if (newPair.first >= primeNode->parentNode->entry[i].first &&
						newPair.first < primeNode->parentNode->entry[i + 1].first) {
						insertNode(primeNode->parentNode->pointer[i + 1], newPair);
						break;
					}
				}
			}
		}
	}
	sort(node->entry.begin(), node->entry.end(), comp);//sort by key values
	node->size++;
}

void BPtree::insertParent(Node* parent, Node* newNode) {
	//parent��忡 new���� pointer�� ���Խ�Ų��
	//�켱 ������ ��ġ�� ã�´�
	Node* temp = NULL;
	int i, key = newNode->entry[0].first;
	//rightnode�� ���� ���� key���� �θ���� �ö󰣴�(K prime)
	//
	
	//parent�� �ϳ��� ���� �ִ� ���
	if (parent->entry.size() == 1) {
		
		if (newNode->entry[0].first < parent->entry[0].first) {
			parent->pointer[parent->entry.size() + 1] = parent->pointer[parent->entry.size()];
			for (int i = 0;i <= parent->entry.size();i++)
				parent->pointer[i] = newNode->pointer[i];
		}
		else {
			parent->pointer[1] = newNode->pointer[0];
			parent->pointer[2] = newNode->pointer[1];
		}
		insertNode(parent, newNode->entry[0]);
	}
	else {//parent�� ������ �ִ� ���
		//ù key������ ���� ���
		int i = 0;
		if (key < parent->entry[0].first) {
			i = 0;
		}
		//������ key���� ū ���
		else if (key >= parent->entry[parent->entry.size() - 1].first) {
			i = parent->entry.size();
		}
		else {
			for (i = 0;i < parent->entry.size() - 1;i++) {
				//������ ��ġ Ȯ��
				if (key > parent->entry[i].first && key < parent->entry[i + 1].first) {
					i++;
					break;
				}
				else if (key < parent->entry[i].first)
					break;
				else {
					continue;
				}
			}
			if (key >= parent->entry[i].first) {//������ key������ ū ���
				i++;
			}
		}

		//������ ����
		for (int j = parent->entry.size();j > i;j--) {
			parent->pointer[j + 1] = parent->pointer[j];
		}
		parent->pointer[i] = newNode->pointer[0];
		parent->pointer[i + 1] = newNode->pointer[1];
		

		insertNode(parent, newNode->entry[0]);
	}
	return;
}

void BPtree::insert(pair<int, int> newPair) {
	Node* newNode;
	//1.���� Ʈ���� �ƹ��͵� ���� ����(root��尡 NULL)
	
	if (this->rootNode == NULL) {
		//��Ʈ��带 �����ϰ� newNode�� ����Ͽ� ������ �����Ѵ�
		this->rootNode = new Node(1);
		newNode = this->rootNode;
	}
	else {
		//������ key���� �ش�Ǵ� ������ ��带 ã�´�
		newNode = this->findNode(newPair.first);
	}
	this->insertNode(newNode, newPair);

	//this->BFS(this->rootNode);
}

Node* BPtree::findNode(int key) {
	Node* tempNode = this->rootNode;

	while (!tempNode->isLeaf) {
		int i = 0;
		
		for (i = 0;i < tempNode->entry.size() - 1;i++) {
			if (tempNode->isRoot && tempNode->entry.size() == 1) {
				if (key >= tempNode->entry.begin()->first) {
					i++;
					break;
				}
				else
					break;
			}
			//entry�� �� ������ first�� key��, second�� value�� ��Ÿ����.
			//�����忡�� �� key����� �Է¹��� key���� ���ذ���, 
			//������ ������ ã�� leaf��带 ���� ��������.
			if (key > tempNode->entry[i].first && key < tempNode->entry[i + 1].first) {//������ ��ġ�� ã�� ���
				i++;//i��° ���� ������(i+1)�� ���� ������
				break;
			}
			else if (key < tempNode->entry[i].first) {//Ki���� ������ Ż��
				break;
			}

			else if (key >= tempNode->entry[i].first) {//key���� ���� ��忡���� i��° key������ ū ���
				continue;
			}
		}

		if (key >= tempNode->entry[i].first) {//������ key������ ū ��� �� ������ �����ͷ� ������
			i++;
		}
		////���� ���������� ����� ���� á��, internal node�� ��� split
		//if (tempNode->pointer[i]->isFull() && !tempNode->pointer[i]->isLeaf) {
		//	splitNode(tempNode, i);//internal node split
		//}
		//else
		tempNode = tempNode->pointer[i];//�� �ݺ����� ���� �ش�Ǵ� �ڽĳ��� ���� ġȯ�ȴ�
	}

	//������� �Դٸ� tempNode�� �ݵ�� leaf��忩�� �Ѵ�
	return tempNode;
}

Node* BPtree::splitNode(Node* node, pair<int, int> newPair) {
	//������ node�� ���� �� ���¶�� ����
	Node* rightNode = new Node(0);//���� �ö󰡴� �� �θ��� ����
	Node* newParent = new Node(0);
	int midIndex = N / 2;
	//leaf����� ���� non-leaf�� ���� ������ ó��
	//leaf����� ����� split
	if (node->isLeaf) {
		//�߰��� ������ ������ rightNode�� �Ҵ�
		for (int i = midIndex;i < N;i++) {
			rightNode->entry.push_back(node->entry[i]);
			rightNode->pointer[i - midIndex] = node->pointer[i];
		}

		//node�� ��Ʈ����� ���(Ʈ���� ��Ʈ �ϳ��� �ִ� ���)
		if (node->parentNode == NULL) {//split�ϴ� ��尡 �θ��尡 ���� ���
			newParent->entry.push_back(node->entry[midIndex]);//�θ��忡 �߰� �ε��� key�� ����
			//�θ��ڽİ��踦 �̾���
			newParent->isRoot = true;//���� ���� �θ��尡 root��尡 �ǰ�
			newParent->isLeaf = false;//���̻� leaf��尡 �ƴϰ� �ȴ�

			//�θ����� �� �����͵��� �ڽİ� ����
			newParent->pointer[newParent->entry.size() - 1] = node;
			newParent->pointer[newParent->entry.size()] = rightNode;

			/*for (int i = 0;i <= newParent->entry.size();i++) {
				cout << "newparent pointer: ";
				for (auto& a : newParent->pointer[i]->entry)
					cout << a.first << " ";
				cout << '\n';
			}*/

			//�������� ������ �� ����� �θ��带 newParent�� ����
			node->parentNode = newParent;
			rightNode->parentNode = newParent;


			//leaf��峢�� �̾��ش�
			node->nextLeaf = rightNode;
			rightNode->prevLeaf = node;

			//split�� ����� ���� ���� �θ��� ����
			newBPtree->rootNode = newParent;//Ʈ���� ��Ʈ�� newParent���� ����

			//���� ����� �߰����� ������ ����(���� ����̱⶧���� �߰� ���� �������� ��� ����)
			node->entry.erase(node->entry.begin() + midIndex, node->entry.end());
		}
		else {//�����尡 �̹� �θ��尡 �ִ� ����� ���
			//���� �θ��尡 �� á���� Ȯ���Ѵ�
			if (!node->parentNode->isFull()) {
				//�θ��尡 �� ���� ���� ���

				rightNode->nextLeaf = node->nextLeaf;
				node->nextLeaf = rightNode;
				rightNode->prevLeaf = node;
				if (rightNode->nextLeaf != NULL)//���� leaf��尡 ���ٸ�(���� ������ leaf����� ���) ������ �ʿ����
					rightNode->nextLeaf->prevLeaf = rightNode;

				/*for (int i = 0;i <= node->parentNode->entry.size();i++) {
					cout << "node->parentNode pointer: ";
					for (auto& a : node->parentNode->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}*/
				
				//�θ���� �� ���ο� ���� �����ʳ�忡�� ���� �۾Ҵ� key����
				//�����͸� ���� �ö󰣴�
				newParent->entry.push_back(rightNode->entry[0]);
				newParent->pointer[newParent->entry.size() - 1] = node;//0��° ������
				newParent->size++;
				newParent->pointer[newParent->entry.size()] = rightNode;//�� ����� ������ �����ʹ� rightnode
				//���� ������Ʈ�� �θ���� �����͵��� ������ �����Ѵ�
				insertParent(node->parentNode, newParent);

				/*for (int i = 0;i <= newParent->entry.size();i++) {
					cout << "newparent pointer: ";
					for (auto& a : newParent->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}*/

				//node�� �θ��� ������Ʈ
				newParent = node->parentNode;
				node->parentNode = newParent;
				rightNode->parentNode = newParent;
			}
			else {
				//���� �θ��帶�� �� �� ���
				//�ϴ� �θ��忡 �ִ´�
				newParent->entry.push_back(rightNode->entry[0]);
				newParent->pointer[newParent->size] = node;
				newParent->size++;
				newParent->pointer[newParent->size] = rightNode;//�� ����� ������ �����ʹ� rightnode
				//���� ������Ʈ�� �θ���� �����͵��� ������ �����Ѵ�
				insertParent(node->parentNode, newParent);
				//�ְ� ���� �θ��尡 �ɰ��� ��
				//���� leaf��� ���� �ɰ����� �����ؾ���

				newParent = node->parentNode;
				node->parentNode = newParent;
				rightNode->parentNode = newParent;


				newParent->isLeaf = false;//���� ���� �θ���� ���̻� leaf�� �ƴ�

				rightNode->nextLeaf = node->nextLeaf;
				node->nextLeaf = rightNode;
				rightNode->prevLeaf = node;
				if (rightNode->nextLeaf != NULL)//���� leaf��尡 ���ٸ�(���� ������ leaf����� ���) ������ �ʿ����
					rightNode->nextLeaf->prevLeaf = rightNode;

				/*for (int i = 0;i <= newParent->entry.size();i++) {
					cout << "newParent pointer: ";
					for (auto& a : newParent->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}
				Node* tempNode = newParent->nextLeaf;
				for (int i = 0;i <= tempNode->parentNode->entry.size();i++) {
					cout << "tempNode->parentNode pointer: ";
					for (auto& a : tempNode->parentNode->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}*/
				
			}
			//split�� ��� �߰����� ���������� ����
			node->entry.erase(node->entry.begin() + midIndex, node->entry.end());
		}

		/*cout << "left child: ";
		for (auto& a : newParent->pointer[newParent->size]->entry)
			cout << a.first << " ";
		cout << '\n';
		cout << "right child: ";
		for (auto& a : newParent->pointer[newParent->size + 1]->entry)
			cout << a.first << " ";
		cout << '\n';
		newParent->size++;


		cout << "Root.isroot: " << newBPtree->rootNode->isRoot <<
			", Root.isleaf: " << newBPtree->rootNode->isLeaf << '\n';*/
			////����� ���� ����� ���� Ȯ��
			//cout << "Current Node: ";
			//for (auto& a : node->entry)
			//	cout << a.first << " ";
			//cout << '\n';
			//cout << "RightNode of current Node: ";
			//for (auto& a : rightNode->entry)
			//	cout << a.first << " ";
			//cout << '\n';
			//cout << "ParentNode of current Node: ";
			//for (auto& a : node->parentNode->entry)
			//	cout << a.first << " ";
			//cout << '\n';
			//cout << "Root Node of newBPtree: ";
			//for (auto& a : newBPtree->rootNode->entry)
			//	cout << a.first << " ";
			//cout << '\n';
	}
	//non-leaf����� ����� split(internal nodes)
	else {

		int i = 0;
		for (i = midIndex + 1;i < node->entry.size();i++) {
			rightNode->entry.push_back(node->entry[i]);
			rightNode->pointer[i - midIndex - 1] = node->pointer[i];
			rightNode->pointer[i - midIndex - 1]->parentNode = rightNode;
		}
		rightNode->pointer[i - midIndex - 1] = node->pointer[i];
		//���������� ���� ����� �ڽĵ鵵 �θ��弳�� �ʿ�
		rightNode->pointer[i - midIndex - 1]->parentNode = rightNode;


		//����  �ɰ� ��尡 �θ��尡 ���� ���
		if (node->parentNode == NULL) {
			newParent->entry.push_back(node->entry[midIndex]);//��� �ִ� key�� �� �θ���� �÷�����
			newParent->pointer[newParent->entry.size() - 1] = node;
			newParent->pointer[newParent->entry.size()] = rightNode;

			//������ ������ �θ��� ����
			node->parentNode = newParent;
			rightNode->parentNode = newParent;


			rightNode->nextLeaf = node->nextLeaf;
			node->nextLeaf = rightNode;
			rightNode->prevLeaf = node;
			
			if (rightNode->nextLeaf != NULL)//���� leaf��尡 ���ٸ�(���� ������ leaf����� ���) ������ �ʿ����
				rightNode->nextLeaf->prevLeaf = rightNode;


			newParent->isRoot = true;
			newParent->isLeaf = false;
			node->isLeaf = false;
			rightNode->isLeaf = false;

			newBPtree->rootNode = newParent;//root��� ����

			//split�� ��� �߰����� ���������� ����
			node->entry.erase(node->entry.begin() + midIndex, node->entry.end());
		}
		else {//split�� ��尡 �θ��尡 �ִ� ���, �� leaf�� root������ non-leaf���
			//���� �θ��尡 �� á���� Ȯ���Ѵ�
			//�θ���� non-leaf����̰ų� root����� �ΰ��� ���� ������
			if (!node->parentNode->isFull()) {
				//�θ��尡 �� ���� ���� ���

				

				newParent->isLeaf = false;
				node->isLeaf = false;
				rightNode->isLeaf = false;

				//�θ���� �� ���ο� ���� �����ʳ�忡�� ���� �۾Ҵ� key����
				//�����͸� ���� �ö󰣴�
				newParent->entry.push_back(node->entry[midIndex]);
				newParent->pointer[newParent->size] = node;
				newParent->size++;
				newParent->pointer[newParent->size] = rightNode;//�� ����� ������ �����ʹ� rightnode
				//���� ������Ʈ�� �θ���� �����͵��� ������ �����Ѵ�
				insertParent(node->parentNode, newParent);

				/*for (int i = 0;i <= node->parentNode->entry.size();i++) {
					cout << "node->parentNode pointer: ";
					for (auto& a : node->parentNode->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}*/

				//node�� �θ��� ������Ʈ
				newParent = node->parentNode;
				node->parentNode = newParent;
				rightNode->parentNode = newParent;

				rightNode->nextLeaf = node->nextLeaf;
				node->nextLeaf = rightNode;
				rightNode->prevLeaf = node;
				if (rightNode->nextLeaf != NULL)//���� leaf��尡 ���ٸ�(���� ������ leaf����� ���) ������ �ʿ����
					rightNode->nextLeaf->prevLeaf = rightNode;

				//���� ����� �߰����� ������ ����(���� ����̱⶧���� �߰� ���� �������� ��� ����)
				node->entry.erase(node->entry.begin() + midIndex, node->entry.end());
			}
			else {
				//���� �θ��帶�� �� �� ���
				//�ϴ� �θ��忡 �ִ´�
				newParent->entry.push_back(rightNode->entry[0]);
				newParent->pointer[newParent->size] = node;
				newParent->size++;
				newParent->pointer[newParent->size] = rightNode;//�� ����� ������ �����ʹ� rightnode
				//���� ������Ʈ�� �θ���� �����͵��� ������ �����Ѵ�
				insertParent(node->parentNode, newParent);
				//�ְ� ���� �θ��尡 �ɰ��� ��
				//���� leaf��� ���� �ɰ����� �����ؾ���

				/*for (int i = 0;i <= node->parentNode->entry.size();i++) {
					cout << "node->parentNode pointer: ";
					for (auto& a : node->parentNode->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}*/

				newParent->isLeaf = false;//���� ���� �θ���� ���̻� leaf�� �ƴ�
				//�����Ϳ���(������ �����͸� �����ϸ� ��
				newParent->pointer[newParent->entry.size()] = rightNode;

				newParent = node->parentNode;
				node->parentNode = newParent;
				rightNode->parentNode = newParent;

				rightNode->nextLeaf = node->nextLeaf;
				node->nextLeaf = rightNode;
				rightNode->prevLeaf = node;
				if (rightNode->nextLeaf != NULL)//���� leaf��尡 ���ٸ�(���� ������ leaf����� ���) ������ �ʿ����
					rightNode->nextLeaf->prevLeaf = rightNode;
			}
		}

	}
	return rightNode;
}

void BPtree::BFS(Node* currentNode) {
	//breath first traverse
	Node* tempNode = NULL;
	int i = 0;
	int currentDepth = 0;
	while (currentNode != NULL) {
		tempNode = currentNode;
		cout <<'\n'<<"<"<< currentDepth<<">" << '\n';
		while (tempNode != NULL) {
			for (i = 0;i < tempNode->entry.size();i++) {
				if(tempNode->isLeaf)
					cout <<"("<<tempNode->BID<<")"<< tempNode->entry[i].first << ", ";
				else
					cout << "(" << tempNode->bid[i]<< ")" << tempNode->entry[i].first << ", ";
			}
			if(!tempNode->isLeaf)
				cout << "(" << tempNode->bid[i] << ")"  << ", ";
			tempNode = tempNode->nextLeaf;
		}
		currentNode = currentNode->pointer[0];//�� �� �����ͷ� ������ �� ���ʿ��� �������� key���� ���
		currentDepth++;
	}
	this->depth = currentDepth - 1;
	cout << '\n';
}

int main(int argc, char* argv[])
{
	//command prompt���� ���� ���ڵ�
	char cmd;
	string fileName = "", bptree_bin_file = "", outputName = "";

	//*****************************************************************
	//Ŀ�ǵ� â���� insertion('i')�κ��� ������ ���� ���� ��� 
	//�� �κ��� uncomment���ּ���.
	BLOCK_SIZE = 36;//���⿡ ���ũ�⸦ �������ּ���
	N = (BLOCK_SIZE - 4) / 8;
	newBPtree = new BPtree;
	newBPtree->insertSample("bptree.bin", "sample_insertion_input.txt");//������ bin���ϰ� ���� �Է����� �̸��� �������ּ���
	//newBPtree = new BPtree();//�� ������ N���� ��Ʈ���� ���� B+Ʈ�� //�̺κ��� commentó�����ּ���
	//*****************************************************************

	cmd = argv[1][0];//Ŀ�ǵ� â���� �ϳ��� ���ĺ��� �ɼ����� �Է¹޴´�
	

	switch (cmd)//�Է¹��� Ŀ�ǵ� switch�� ����...
	{
	case 'c':
		bptree_bin_file = argv[2];//���� �̸��� string���� ����
		BLOCK_SIZE = stoi(argv[3]);//stoi����Ͽ� ���������� ����Ʈ �� ����

		//�� ��忡 �� �� �ִ� entry pair ���� N(The number of entries per node)
		N = (BLOCK_SIZE - 4) / 8;//36byte �Է¹޾Ҵٸ� key�� 4�� (4*4 = 16)

		//sum = 16 + 20 = 36bytes
		cout << bptree_bin_file << " " << BLOCK_SIZE << '\n';
		//binary���� ����
		//���ο� B+ Ʈ�� ����(������ binary ���� �̸��� ���(block)�� ũ��
		//�� ����� 36byte��� ����������:
		//non-leaf���� �����Ͱ� �������̰�, key���� ������ => ���ļ� 8byte
		
		newBPtree->createHeader(bptree_bin_file, 1);//create header with bid 1
		break;
	case 'i':
		// insert records from [records data file], ex) records.txt
		bptree_bin_file = argv[2];//����� ������ binary file(bptree.bin)
		fileName = argv[3];//���� �Է����� �̸��� string���� ����
		//newBPtree = new BPtree(bptree_bin_file);
		cout << bptree_bin_file << " " << fileName << '\n';
		newBPtree->insertSample(bptree_bin_file, fileName);
		break;
	case 's':
		// point-search key in [input file] and print results to [output file]
		bptree_bin_file = argv[2];
		fileName = argv[3];//���� �Է����� �̸��� string���� ����
		outputName = argv[4];//����� ���� �̸�
		newBPtree->pointSearchSample(bptree_bin_file, fileName, outputName);
		break;
	case 'r':
		// range-search keys in [input file] and print results to [output file]
		bptree_bin_file = argv[2];
		fileName = argv[3];
		outputName = argv[4];//����� ���� �̸�
		newBPtree->rangeSearchSample(bptree_bin_file, fileName, outputName);
		break;
	case 'p':
		bptree_bin_file = argv[2];
		outputName = argv[3];
		// print B+-Tree structure to [output file]
		newBPtree->printTree(newBPtree->rootNode, bptree_bin_file, outputName);
		break;
	default:
		cout << "\nAvailable commands: \nc: create index file" <<
			"\ni: insert records from [records data file]" <<
			"\ns: search keys in [input file] and print results to [output file]" <<
			"\nr: range-search keys in [input file] and print results to [output file]" <<
			"\np: print B+-Tree structure to [output file]\n";
		break;
	}
	return 0;
}