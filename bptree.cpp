/*
DB001_Project2
컴퓨터공학과_12161579_박준현
*/

#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
using namespace std;

int N;//한 블록(노드)에 들어갈 엔트리들의 개수(전역변수)
int BLOCK_SIZE;
int currentBlock;//새로운 노드 생성 시 블록number 지정

//Physical offset of a block in the 
//B+Tree binary file is calculated in the following way :
//12 + ((BID - 1) * BlockSize)

class BPtree;
BPtree* newBPtree;//global declaration of new B+ tree

class Node {//A node is corresponding to a block. 
			//Thus, the size of each node exactly one block.
public:
	friend BPtree;
	vector<pair<int, int>> entry;//key, value쌍을 넣을 벡터
	Node** pointer;
	Node* parentNode;
	Node* nextLeaf;//만약 leaf노드면 다음 leaf노드를 가리킬 포인터
	Node* prevLeaf;//자기 이전의 leaf노드를 가리킴

	int* bid;//non-leaf노드에서의 bid를 담을 배열
	int BID;//해당 노드(블록)의 블록ID를 담는 변수
	int size;//해당 블록의 entry 개수
	bool isLeaf;//해당 노드가 leaf인지 아닌지 구분
	bool isRoot;//해당 노드가 root노드인지 아닌지 구분
	int nextBID;
	int nextLevelBID;
	Node() {//기본생성자
		this->bid = new int[N + 1];//non-leaf노드의 경우 다음 level의 노드들에 대한 block ID를 갖고 있어야 함
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
		this->bid = new int[N + 1];//non-leaf노드의 경우 다음 level의 노드들에 대한 block ID를 갖고 있어야 함
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
	BPtree() {//기본생성자
		this->rootNode = NULL;
		this->depth = 0;
		this->size = 0;
	}
	//constructor with params
	BPtree(const string bptree_bin_file) {
		this->rootNode = NULL;
		this->depth = 0;
		this->size = 0;
		//this->createHeader(bptree_bin_file, 1);//헤더생성(12bytes)
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
	int inMemoryPointSearch(int);//search-key값 key를 입력으로 받는다
	//int blockPointSearch(const string, int);
	void pointSearchSample(const string, const string, const string);
	Node* findNode(int key);//pointSearch 함수의 변형. value를 리턴하는 대신 해당 노드자체를 리턴한다
	vector<pair<int, int>> inMemoryRangeSearch(const string, int, int);
	void rangeSearchSample(const string, const string, const string);
	void setLeafBID();
	void setNonLeafBID();
	//int* search(int key);
	//int* search(int startRange, int endRange);
};
void BPtree::setLeafBID() {
	Node* tempNode = newBPtree->rootNode;
	int bid = 1;//rootNode가 1번블록이므로 2번부터 시작
	while (!tempNode->isLeaf) {//leaf노드까지 내려감
		tempNode = tempNode->pointer[0];
	}
	//모든 leaf노드들
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
	//non-leaf노드에 대한 블록ID 어드레싱
	Node* currentLevel = newBPtree->rootNode, *tempNode = NULL;
	while (!currentLevel->isLeaf) {//leaf노드까지 내려감
		currentLevel = currentLevel->pointer[0];
	}
	currentLevel = currentLevel->parentNode;//바로 윗 레벨로 올라감
	int i = 0;
//	this->BFS(this->rootNode);
	while (currentLevel != NULL) {
		tempNode = currentLevel;
		while (tempNode != NULL) {
			for (i = 0;i < tempNode->entry.size();i++) {
				//현재 non-leaf노드에서 가리키는 모든 노드의 블록 id들을 가리킬 bid배열
				tempNode->bid[i] = tempNode->pointer[i]->BID;
			}
			tempNode->bid[i] = tempNode->pointer[i]->BID;//노드의 마지막 포인터
			tempNode->BID = currentBlock;//non-leaf에 대한 BID
			currentBlock++;
			tempNode = tempNode->nextLeaf;//다음 nonleaf로 넘어간다
		}
		currentLevel = currentLevel->parentNode;//위로 올라가야 한다
	}
}

void BPtree::createHeader(const string fileName, int bid) {
	//binary file 이름 지정
	ofstream writeFile(fileName, ios::out | ios::binary);
	if (this->rootNode == NULL) {
		//this->rootNode = new Node(1);//루트노드의 블록 id는 1이고, 엔트리는 N개만큼 가짐
		this->depth++;//루트노드 하나밖에 없으니 depth역시 1
		//초기헤더 12byte (block size, root block, depth) write
		this->size++;

		writeFile.write((char*)&BLOCK_SIZE, sizeof(int));
		writeFile.write((char*)&bid, sizeof(int));
		writeFile.write((char*)&this->depth, sizeof(int));
	}
	else {//삽입 후 루트BID 업데이트 하는경우
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
			currentNode = currentNode->pointer[0];//맨 앞 포인터로 내려간 후 왼쪽에서 오른쪽을 key값들 출력
			currentDepth++;
		}
		outputFile << '\n';
	}
	outputFile.close();
}

void BPtree::writeFile(const string bptree_bin_file) {
	//트리를 순회하며 파일에 입력
	ofstream outputFile(bptree_bin_file, ios::app | ios::binary);//모든 출력은 파일의 끝에 추가됨

	Node* tempNode = newBPtree->rootNode;
	while (!tempNode->isLeaf) {//leaf노드까지 내려감
		tempNode = tempNode->pointer[0];
	}
	//모든 leaf노드들 입력
	int i, zero = 0;
	while (tempNode != NULL) {
		//현재 노드의 key-value쌍 입력
		i = 0;
		if (tempNode->isFull()) {//만약 가득 찬 노드라면 뒤에 따로 0을 붙혀주지 않아도 됨
			for (i = 0;i < N;i++) {
				outputFile.write((char*)&tempNode->entry[i].first, sizeof(int));//key
				outputFile.write((char*)&tempNode->entry[i].second, sizeof(int));//value
			}
		}
		else {//가득 차지 않은 노드의 경우 뒤에 0을 붙여줘야 함
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
		//다음 노드의 BId입력
		if(tempNode!=NULL)
			outputFile.write((char*)&tempNode->BID, sizeof(int));
		else//다음 노드가 없는 경우(현재 level에서 가장 마지막에 위치한 노드)
			outputFile.write((char*)&zero, sizeof(int));
	}

	//non-leaf노드들 입력
	Node* currentLevel = newBPtree->rootNode;
	while (!currentLevel->isLeaf) {//leaf노드까지 내려감
		currentLevel = currentLevel->pointer[0];
	}
	currentLevel = currentLevel->parentNode;//바로 윗 레벨로 올라감

	i = 0;
	while (currentLevel != NULL) {
		tempNode = currentLevel;
		while (tempNode != NULL) {
			if (tempNode->isFull()) {//만약 가득 찬 노드라면 뒤에 따로 0을 붙혀주지 않아도 됨
				for (i = 0;i < N;i++) {
					outputFile.write((char*)&tempNode->bid[i], sizeof(int));//BID
					outputFile.write((char*)&tempNode->entry[i].first, sizeof(int));//key
				}
			}
			else {//가득 차지 않은 노드의 경우 뒤에 0을 붙여줘야 함(블록 크기만큼)
				for (i = 0;i < tempNode->entry.size();i++) {
					outputFile.write((char*)&tempNode->bid[i], sizeof(int));//BID
					outputFile.write((char*)&tempNode->entry[i].first, sizeof(int));//key
				}
				for (int j = i; j < N;j++) {
					outputFile.write((char*)&zero, sizeof(int));
					outputFile.write((char*)&zero, sizeof(int));
				}
			}
			//다음 노드의 BID입력
			if (tempNode->nextLeaf != NULL)
				outputFile.write((char*)&tempNode->bid[i], sizeof(int));
			else {//다음 노드가 없는 경우(현재 level에서 가장 마지막에 위치한 노드)
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

			tempNode = tempNode->nextLeaf;//다음 nonleaf로 넘어간다
		}
		currentLevel = currentLevel->parentNode;//위로 올라가야 한다
	}
	outputFile.close();
}

void BPtree::insertSample(const string bptree_bin_file, const string fileName) {//key, rootid, value
	
	ifstream inputFile(fileName);//입력값으로 파일이름을 받으면 B plus tree에 삽입한다.
	string line, firstInt, secondInt;
	int key = 0, value = 0, bid = 0;
	if(inputFile.is_open()) {
		//이미 헤더를 생성해 rootnode가 있다고 가정, sample input file의 값들을 읽어와 삽입
		while (getline(inputFile, line)) {//read in entire line(row)
			stringstream ss(line);//
			getline(ss, firstInt, ',');//key값 먼저 읽음
			key = stoi(firstInt);//change to int
			getline(ss, secondInt);//그다음 value(데이터)읽음
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
	else {//input파일 여는데 문제가 있는 경우
		cout << "Failed to open file.\n";//경고 메시지 출력
		return;
	}
	//cout << this->rootNode->BID << '\n';
	this->createHeader(bptree_bin_file, this->rootNode->BID);//업데이트된 헤더 갱신
	//bin파일에 해당 트리의 노드들 입력
	this->writeFile(bptree_bin_file);

	//apply inserted bptree to the bin file
	//updateTree(bptree_bin_file);
	//printTree(newBPtree->rootNode, "print.txt");
	inputFile.close();//sample input파일 닫기
}

//int BPtree::blockPointSearch(const string bptree_bin_file, int key) {
//	ifstream inputFile(bptree_bin_file, ios::in | ios::binary);//bin파일을 읽어서 key값을 찾는다
//	int value = 0, rootBID = 0;
//
//	if (inputFile.is_open()) {
//		inputFile.read((char*)&rootBID, sizeof(int));
//		inputFile.read((char*)&rootBID, sizeof(int));
//
//		while (!inputFile.eof()) {
//			for (int i = 0;i < 4;i++) {
//				inputFile.read((char*)&key, sizeof(int));//파일에서 값을 읽어와서 key에 저장 후 
//				inputFile.read((char*)&value, sizeof(int));
//				cout << key << "," << value << "\n";//출력
//			}
//			inputFile.read((char*)&n, sizeof(int));//다음BID
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
	Node* tempNode = newBPtree->rootNode;//Point Query의 시작점은 항상 루트노드에서 시작한ㄷㅏ.
	//이제부터 루트노드부터 시작하여, 입력값으로 받은 key값이 속하는 범위를 찾는다.
	//각 non-leaf노드들은 routing역할을 할 것이며, 
	//tempNode가 leaf노드가 될 때가지 해당 반복문을 계속한다.
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
			if (key > tempNode->entry[i].first && key < tempNode->entry[i + 1].first) {//적절한 위치를 찾은 경우
				i++;//i번째 다음 포인터(i+1)를 따라 내려감
				break;
			}
			else if (key < tempNode->entry[i].first) {//Ki보다 작으면 탈출
				break;
			}
			else if (key >= tempNode->entry[i].first) {//key값이 현재 노드에서의 i번째 key값보다 큰 경우
				continue;
			}
		}
		if (key >= tempNode->entry[i].first) {//마지막 key값보다 큰 경우 맨 오른쪽 포인터로 내려감
			i++;
		}
		tempNode = tempNode->pointer[i];//위 반복문에 따라 해당되는 자식노드로 새로 치환된다
	}

	//여기까지 왔다면 tempNode는 반드시 leaf노드여야 한다
	for (int i = 0;i < tempNode->entry.size();i++) {
		if (tempNode->entry[i].first == key)
			return tempNode->entry[i].second;//second는 해당 i번째에 속하는 value값을 갖고 있다
	}
	return -1;//대상을 못찾은 경우, -1을 리턴한다.
}
void BPtree::pointSearchSample(const string bptree_bin_file, const string input_file, const string output_file) {
	ifstream inputFile(input_file);//입력값으로 파일이름을 받으면 B plus tree에 삽입한다.
	ofstream outputFile(output_file, ios::out);
	string line, firstInt, secondInt;
	int key = 0, value = 0, bid = 0;
	if (inputFile.is_open()) {
		//이미 헤더를 생성해 rootnode가 있다고 가정, sample input file의 값들을 읽어와 삽입
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
	else {//input파일 여는데 문제가 있는 경우
		cout << "Failed to open file. :(\n";//경고 메시지 출력
		return;
	}
	//apply inserted bptree to the bin file
	//updateTree(bptree_bin_file);
	//printTree(newBPtree->rootNode, "print.txt");
	inputFile.close();//sample input파일 닫기
	outputFile.close();
}


vector<pair<int, int>> BPtree::inMemoryRangeSearch(const string bptree_bin_file, int key1, int key2) {
	ifstream inputFile(bptree_bin_file);//bin파일을 읽어서 key1과 key2사이 값들을 모두 반환한다

	vector<pair<int, int>> pairs;
	Node* tempNode = findNode(key1);
	//해당 노드의 key값들 중 key1과 key2사이의 범위에 속하지 않을 때까지 반복한다
	//key1과 해당 노드의 첫번째 key값을 비교하여 key1이 더 크거나,
	//key2와 해당 노드의 마지막 key값을 비교하여 key2가 더 작으면 된다
	while (tempNode != NULL) {
		//if (key1 >= tempNode->entry[0].first || key2 <= tempNode->entry[N - 1].first)
		for (int i = 0;i < tempNode->entry.size();i++) {
			if (tempNode->entry[i].first >= key1 &&
				tempNode->entry[i].first <= key2) {//key1과 key2사이 범위에 속한 쌍들은 모두 벡터에 저장
				pairs.push_back(tempNode->entry[i]);
			}
		}
		//다음 leaf노드로 이동
		tempNode = tempNode->nextLeaf;
	}
	return pairs;
}

void BPtree::rangeSearchSample(const string bptree_bin_file, const string input_file, const string output_file) {
	ifstream inputFile(input_file);//입력값으로 파일이름을 받으면 B plus tree에 삽입한다.
	ofstream outputFile(output_file, ios::out);
	string line, firstInt, secondInt;
	vector<pair<int, int>> pair;
	int key1 = 0, key2 = 0, value = 0, bid = 0;
	if (inputFile.is_open()) {
		//이미 헤더를 생성해 rootnode가 있다고 가정, sample input file의 값들을 읽어와 삽입
		while (getline(inputFile, line)) {//read in entire line(row)
			stringstream ss(line);//
			getline(ss, firstInt, ',');//key값 먼저 읽음
			key1 = stoi(firstInt);//change to int
			getline(ss, secondInt);//그다음 value(데이터)읽음
			key2 = stoi(secondInt);//change to int

			cout << '\n' << "Find all pairs among Key1: " << key1 << " Key2: " << key2 << '\n';
			pair = this->inMemoryRangeSearch(bptree_bin_file, key1, key2);
			for (auto& i : pair) {
				outputFile << i.first << "," << i.second << " ";
			}
			outputFile << '\n';
		}
	}
	else {//input파일 여는데 문제가 있는 경우
		cout << "Failed to open file. :(\n";//경고 메시지 출력
		return;
	}
	inputFile.close();//sample input파일 닫기
	outputFile.close();
}

bool comp(pair<int, int> a, pair<int, int> b) {
	return a.first < b.first;//sort on keys
}

void BPtree::insertNode(Node* node, pair<int, int> newPair) {//key값과 그 key값을 가리키는 포인터
	//새 key값을 삽입할 노드를 찾은 상태

	if (!node->isFull()) {//해당 노드가 꽉 찬 상태가 아니라면
		//cout << "여기가 문제\n";
		node->entry.push_back(newPair);//삽입 진행
	}
	else {//해당 노드가 꽉 찬 상태인 경우
		//노드를 2개의 노드로 쪼개야 한다
		if (!node->isLeaf) {
			node->entry.push_back(newPair);
			sort(node->entry.begin(), node->entry.end(), comp);//sort by key values

			Node* primeNode = splitNode(node, newPair);//새로 생긴 부모노드(혹은 기존의 부모노드)
			/*cout << "primeNode: ";
			for (auto& a : primeNode->entry)
				cout << a.first << " ";
			cout << '\n';*/
		}
		else {
			Node* primeNode = splitNode(node, newPair);//새로 생긴 부모노드(혹은 기존의 부모노드)

			//맨 첫번째 key보다 작으면 첫번째 포인터따라내려가서 삽입
			if (newPair.first < primeNode->parentNode->entry[0].first)
				insertNode(primeNode->parentNode->pointer[0], newPair);
			else if (newPair.first >= primeNode->parentNode->entry[primeNode->parentNode->entry.size() - 1].first)//마지막 포인터로 내려가는 경우
				insertNode(primeNode->parentNode->pointer[primeNode->parentNode->entry.size()], newPair);
			else {
				//적절한 삽입 위치를 탐색
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
	//parent노드에 new노드와 pointer를 삽입시킨다
	//우선 적절한 위치를 찾는다
	Node* temp = NULL;
	int i, key = newNode->entry[0].first;
	//rightnode의 제일 작은 key값이 부모노드로 올라간다(K prime)
	//
	
	//parent에 하나의 값만 있는 경우
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
	else {//parent에 여러개 있는 경우
		//첫 key값보다 작은 경우
		int i = 0;
		if (key < parent->entry[0].first) {
			i = 0;
		}
		//마지막 key보다 큰 경우
		else if (key >= parent->entry[parent->entry.size() - 1].first) {
			i = parent->entry.size();
		}
		else {
			for (i = 0;i < parent->entry.size() - 1;i++) {
				//적절한 위치 확인
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
			if (key >= parent->entry[i].first) {//마지막 key값보다 큰 경우
				i++;
			}
		}

		//포인터 정리
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
	//1.현재 트리에 아무것도 없는 상태(root노드가 NULL)
	
	if (this->rootNode == NULL) {
		//루트노드를 생성하고 newNode를 사용하여 연산을 진행한다
		this->rootNode = new Node(1);
		newNode = this->rootNode;
	}
	else {
		//삽입할 key값이 해당되는 범위의 노드를 찾는다
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
			//entry의 각 값들의 first가 key값, second가 value를 나타낸다.
			//현재노드에서 각 key값들과 입력받은 key값을 비교해가며, 
			//적절한 범위를 찾아 leaf노드를 향해 내려간다.
			if (key > tempNode->entry[i].first && key < tempNode->entry[i + 1].first) {//적절한 위치를 찾은 경우
				i++;//i번째 다음 포인터(i+1)를 따라 내려감
				break;
			}
			else if (key < tempNode->entry[i].first) {//Ki보다 작으면 탈출
				break;
			}

			else if (key >= tempNode->entry[i].first) {//key값이 현재 노드에서의 i번째 key값보다 큰 경우
				continue;
			}
		}

		if (key >= tempNode->entry[i].first) {//마지막 key값보다 큰 경우 맨 오른쪽 포인터로 내려감
			i++;
		}
		////만약 내려가려는 블록이 가득 찼고, internal node인 경우 split
		//if (tempNode->pointer[i]->isFull() && !tempNode->pointer[i]->isLeaf) {
		//	splitNode(tempNode, i);//internal node split
		//}
		//else
		tempNode = tempNode->pointer[i];//위 반복문에 따라 해당되는 자식노드로 새로 치환된다
	}

	//여기까지 왔다면 tempNode는 반드시 leaf노드여야 한다
	return tempNode;
}

Node* BPtree::splitNode(Node* node, pair<int, int> newPair) {
	//현재노드 node가 가득 찬 상태라고 가정
	Node* rightNode = new Node(0);//위로 올라가는 새 부모노드 생성
	Node* newParent = new Node(0);
	int midIndex = N / 2;
	//leaf노드인 경우와 non-leaf인 경우로 나누어 처리
	//leaf노드인 경우의 split
	if (node->isLeaf) {
		//중간을 포함한 값들을 rightNode에 할당
		for (int i = midIndex;i < N;i++) {
			rightNode->entry.push_back(node->entry[i]);
			rightNode->pointer[i - midIndex] = node->pointer[i];
		}

		//node가 루트노드인 경우(트리에 루트 하나만 있는 경우)
		if (node->parentNode == NULL) {//split하는 노드가 부모노드가 없는 경우
			newParent->entry.push_back(node->entry[midIndex]);//부모노드에 중간 인덱스 key값 삽입
			//부모자식관계를 이어줌
			newParent->isRoot = true;//새로 생긴 부모노드가 root노드가 되고
			newParent->isLeaf = false;//더이상 leaf노드가 아니게 된다

			//부모노드의 각 포인터들을 자식과 연결
			newParent->pointer[newParent->entry.size() - 1] = node;
			newParent->pointer[newParent->entry.size()] = rightNode;

			/*for (int i = 0;i <= newParent->entry.size();i++) {
				cout << "newparent pointer: ";
				for (auto& a : newParent->pointer[i]->entry)
					cout << a.first << " ";
				cout << '\n';
			}*/

			//양쪽으로 갈라진 두 노드의 부모노드를 newParent로 지정
			node->parentNode = newParent;
			rightNode->parentNode = newParent;


			//leaf노드끼리 이어준다
			node->nextLeaf = rightNode;
			rightNode->prevLeaf = node;

			//split한 노드들과 새로 생긴 부모노드 연결
			newBPtree->rootNode = newParent;//트리의 루트를 newParent노드로 갱신

			//현재 노드의 중간부터 끝까지 삭제(왼쪽 노드이기때문에 중간 기준 오른쪽은 모두 삭제)
			node->entry.erase(node->entry.begin() + midIndex, node->entry.end());
		}
		else {//현재노드가 이미 부모노드가 있는 노드인 경우
			//먼저 부모노드가 꽉 찼는지 확인한다
			if (!node->parentNode->isFull()) {
				//부모노드가 꽉 차지 않은 경우

				rightNode->nextLeaf = node->nextLeaf;
				node->nextLeaf = rightNode;
				rightNode->prevLeaf = node;
				if (rightNode->nextLeaf != NULL)//다음 leaf노드가 없다면(현재 마지막 leaf노드인 경우) 지정할 필요없다
					rightNode->nextLeaf->prevLeaf = rightNode;

				/*for (int i = 0;i <= node->parentNode->entry.size();i++) {
					cout << "node->parentNode pointer: ";
					for (auto& a : node->parentNode->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}*/
				
				//부모노드로 들어갈 새로운 노드는 오른쪽노드에서 가장 작았던 key값과
				//포인터를 갖고 올라간다
				newParent->entry.push_back(rightNode->entry[0]);
				newParent->pointer[newParent->entry.size() - 1] = node;//0번째 포인터
				newParent->size++;
				newParent->pointer[newParent->entry.size()] = rightNode;//새 노드의 오른쪽 포인터는 rightnode
				//새로 업데이트된 부모노드와 포인터들의 연결을 수정한다
				insertParent(node->parentNode, newParent);

				/*for (int i = 0;i <= newParent->entry.size();i++) {
					cout << "newparent pointer: ";
					for (auto& a : newParent->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}*/

				//node의 부모노드 업데이트
				newParent = node->parentNode;
				node->parentNode = newParent;
				rightNode->parentNode = newParent;
			}
			else {
				//만약 부모노드마저 꽉 찬 경우
				//일단 부모노드에 넣는다
				newParent->entry.push_back(rightNode->entry[0]);
				newParent->pointer[newParent->size] = node;
				newParent->size++;
				newParent->pointer[newParent->size] = rightNode;//새 노드의 오른쪽 포인터는 rightnode
				//새로 업데이트된 부모노드와 포인터들의 연결을 수정한다
				insertParent(node->parentNode, newParent);
				//넣고 나서 부모노드가 쪼개진 후
				//기존 leaf노드 역시 쪼갰으니 연결해야함

				newParent = node->parentNode;
				node->parentNode = newParent;
				rightNode->parentNode = newParent;


				newParent->isLeaf = false;//새로 생긴 부모노드는 더이상 leaf가 아님

				rightNode->nextLeaf = node->nextLeaf;
				node->nextLeaf = rightNode;
				rightNode->prevLeaf = node;
				if (rightNode->nextLeaf != NULL)//다음 leaf노드가 없다면(현재 마지막 leaf노드인 경우) 지정할 필요없다
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
			//split한 노드 중간기준 오른쪽절반 삭제
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
			////출력을 통해 노드의 값들 확인
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
	//non-leaf노드인 경우의 split(internal nodes)
	else {

		int i = 0;
		for (i = midIndex + 1;i < node->entry.size();i++) {
			rightNode->entry.push_back(node->entry[i]);
			rightNode->pointer[i - midIndex - 1] = node->pointer[i];
			rightNode->pointer[i - midIndex - 1]->parentNode = rightNode;
		}
		rightNode->pointer[i - midIndex - 1] = node->pointer[i];
		//오른쪽으로 생긴 노드의 자식들도 부모노드설정 필요
		rightNode->pointer[i - midIndex - 1]->parentNode = rightNode;


		//현재  쪼갠 노드가 부모노드가 없는 경우
		if (node->parentNode == NULL) {
			newParent->entry.push_back(node->entry[midIndex]);//가운데 있던 key값 새 부모노드로 올려보냄
			newParent->pointer[newParent->entry.size() - 1] = node;
			newParent->pointer[newParent->entry.size()] = rightNode;

			//찢어진 노드들의 부모노드 지정
			node->parentNode = newParent;
			rightNode->parentNode = newParent;


			rightNode->nextLeaf = node->nextLeaf;
			node->nextLeaf = rightNode;
			rightNode->prevLeaf = node;
			
			if (rightNode->nextLeaf != NULL)//다음 leaf노드가 없다면(현재 마지막 leaf노드인 경우) 지정할 필요없다
				rightNode->nextLeaf->prevLeaf = rightNode;


			newParent->isRoot = true;
			newParent->isLeaf = false;
			node->isLeaf = false;
			rightNode->isLeaf = false;

			newBPtree->rootNode = newParent;//root노드 갱신

			//split한 노드 중간기준 오른쪽절반 삭제
			node->entry.erase(node->entry.begin() + midIndex, node->entry.end());
		}
		else {//split한 노드가 부모노드가 있는 경우, 즉 leaf와 root사이의 non-leaf노드
			//먼저 부모노드가 꽉 찼는지 확인한다
			//부모노드는 non-leaf노드이거나 root노드인 두가지 경우로 나뉜다
			if (!node->parentNode->isFull()) {
				//부모노드가 꽉 차지 않은 경우

				

				newParent->isLeaf = false;
				node->isLeaf = false;
				rightNode->isLeaf = false;

				//부모노드로 들어갈 새로운 노드는 오른쪽노드에서 가장 작았던 key값과
				//포인터를 갖고 올라간다
				newParent->entry.push_back(node->entry[midIndex]);
				newParent->pointer[newParent->size] = node;
				newParent->size++;
				newParent->pointer[newParent->size] = rightNode;//새 노드의 오른쪽 포인터는 rightnode
				//새로 업데이트된 부모노드와 포인터들의 연결을 수정한다
				insertParent(node->parentNode, newParent);

				/*for (int i = 0;i <= node->parentNode->entry.size();i++) {
					cout << "node->parentNode pointer: ";
					for (auto& a : node->parentNode->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}*/

				//node의 부모노드 업데이트
				newParent = node->parentNode;
				node->parentNode = newParent;
				rightNode->parentNode = newParent;

				rightNode->nextLeaf = node->nextLeaf;
				node->nextLeaf = rightNode;
				rightNode->prevLeaf = node;
				if (rightNode->nextLeaf != NULL)//다음 leaf노드가 없다면(현재 마지막 leaf노드인 경우) 지정할 필요없다
					rightNode->nextLeaf->prevLeaf = rightNode;

				//현재 노드의 중간부터 끝까지 삭제(왼쪽 노드이기때문에 중간 기준 오른쪽은 모두 삭제)
				node->entry.erase(node->entry.begin() + midIndex, node->entry.end());
			}
			else {
				//만약 부모노드마저 꽉 찬 경우
				//일단 부모노드에 넣는다
				newParent->entry.push_back(rightNode->entry[0]);
				newParent->pointer[newParent->size] = node;
				newParent->size++;
				newParent->pointer[newParent->size] = rightNode;//새 노드의 오른쪽 포인터는 rightnode
				//새로 업데이트된 부모노드와 포인터들의 연결을 수정한다
				insertParent(node->parentNode, newParent);
				//넣고 나서 부모노드가 쪼개진 후
				//기존 leaf노드 역시 쪼갰으니 연결해야함

				/*for (int i = 0;i <= node->parentNode->entry.size();i++) {
					cout << "node->parentNode pointer: ";
					for (auto& a : node->parentNode->pointer[i]->entry)
						cout << a.first << " ";
					cout << '\n';
				}*/

				newParent->isLeaf = false;//새로 생긴 부모노드는 더이상 leaf가 아님
				//포인터연결(마지막 포인터만 수정하면 됨
				newParent->pointer[newParent->entry.size()] = rightNode;

				newParent = node->parentNode;
				node->parentNode = newParent;
				rightNode->parentNode = newParent;

				rightNode->nextLeaf = node->nextLeaf;
				node->nextLeaf = rightNode;
				rightNode->prevLeaf = node;
				if (rightNode->nextLeaf != NULL)//다음 leaf노드가 없다면(현재 마지막 leaf노드인 경우) 지정할 필요없다
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
		currentNode = currentNode->pointer[0];//맨 앞 포인터로 내려간 후 왼쪽에서 오른쪽을 key값들 출력
		currentDepth++;
	}
	this->depth = currentDepth - 1;
	cout << '\n';
}

int main(int argc, char* argv[])
{
	//command prompt에서 받은 인자들
	char cmd;
	string fileName = "", bptree_bin_file = "", outputName = "";

	//*****************************************************************
	//커맨드 창에서 insertion('i')부분이 실행이 되지 않을 경우 
	//이 부분을 uncomment해주세요.
	BLOCK_SIZE = 36;//여기에 블록크기를 지정해주세요
	N = (BLOCK_SIZE - 4) / 8;
	newBPtree = new BPtree;
	newBPtree->insertSample("bptree.bin", "sample_insertion_input.txt");//생성할 bin파일과 샘플 입력파일 이름을 지정해주세요
	//newBPtree = new BPtree();//각 노드들이 N개의 엔트리를 갖는 B+트리 //이부분은 comment처리해주세요
	//*****************************************************************

	cmd = argv[1][0];//커맨드 창에서 하나의 알파벳을 옵션으로 입력받는다
	

	switch (cmd)//입력받은 커맨드 switch로 구분...
	{
	case 'c':
		bptree_bin_file = argv[2];//파일 이름을 string으로 받음
		BLOCK_SIZE = stoi(argv[3]);//stoi사용하여 정수형으로 바이트 수 읽음

		//한 노드에 들어갈 수 있는 entry pair 개수 N(The number of entries per node)
		N = (BLOCK_SIZE - 4) / 8;//36byte 입력받았다면 key는 4개 (4*4 = 16)

		//sum = 16 + 20 = 36bytes
		cout << bptree_bin_file << " " << BLOCK_SIZE << '\n';
		//binary파일 생성
		//새로운 B+ 트리 생성(생성할 binary 파일 이름과 노드(block)의 크기
		//한 블록이 36byte라고 선언했으면:
		//non-leaf에는 포인터가 정수형이고, key값도 정수형 => 합쳐서 8byte
		
		newBPtree->createHeader(bptree_bin_file, 1);//create header with bid 1
		break;
	case 'i':
		// insert records from [records data file], ex) records.txt
		bptree_bin_file = argv[2];//헤더만 생성한 binary file(bptree.bin)
		fileName = argv[3];//샘플 입력파일 이름을 string으로 받음
		//newBPtree = new BPtree(bptree_bin_file);
		cout << bptree_bin_file << " " << fileName << '\n';
		newBPtree->insertSample(bptree_bin_file, fileName);
		break;
	case 's':
		// point-search key in [input file] and print results to [output file]
		bptree_bin_file = argv[2];
		fileName = argv[3];//샘플 입력파일 이름을 string으로 받음
		outputName = argv[4];//출력할 파일 이름
		newBPtree->pointSearchSample(bptree_bin_file, fileName, outputName);
		break;
	case 'r':
		// range-search keys in [input file] and print results to [output file]
		bptree_bin_file = argv[2];
		fileName = argv[3];
		outputName = argv[4];//출력할 파일 이름
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