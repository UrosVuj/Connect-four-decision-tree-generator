#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
using namespace std;


class Tree {

	struct node {

		//array that holds minimax values for its children
		int arr[5]{ -2,-2,-2,-2,-2 };
		
		int who_plays;
		int result;
		int table[5][5];
		node* father;
		node** children;

		/*
		Node Constructor

		*****

		rand() determines who plays first if there is no father
		else it just switches the player

		if a node has a father, then it inherits
		its fathers table, if not, that means that
		node is the root node
		*/

		node(int arr[5][5], node* _father = nullptr) :father(_father)
		{
			if (father == nullptr) who_plays = rand() % 2 + 1;
			else who_plays = (father->who_plays) % 2 + 1;
			children = nullptr;
			if (father != nullptr) {
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++)table[i][j] = father->table[i][j];
				}
			}
			else for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)table[i][j] = arr[i][j];
		}

	};

	struct Stack {
		struct Elem {
			node* _node;
			Elem* next;
			Elem(node* neww = nullptr, Elem* nextt = nullptr) : _node(neww), next(nextt) {}
		};

		Elem* top = nullptr;

		void push(node* curr) {
			if (top == nullptr) {
				top = new Elem(curr);
			}
			else {
				Elem* temp = new Elem(curr);
				temp->next = top;
				top = temp;
			}
		}

		node* pop() {
			if (top == nullptr) { cout << "Error"; exit(1); }
			else {
				Elem* temp = top;
				node* res = top->_node;
				top = top->next;
				delete temp;
				return res;
			}
		}

		bool stack_empty() {
			if (top == nullptr) return true;
			else return false;
		}

	};

	node* root;

	int root_depth(node* unknown) {
		int res = 0;
		while (unknown->father != nullptr) {
			res++;
			unknown = unknown->father;
		}
		return res;
	}

	bool empty_pointer(node* x) {
		for (int i = 0; i < 3; i++)
		{
			if (x->children[i] == nullptr) return true;
		}
		return false;
	}


	//returns true if the table is full
	bool is_full(int arr[5][5]) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
				if (arr[i][j] == 0) return false;
		}
		return true;
	}

	//returns 1 if there is a winner
	int is_done(node* x) {

		//checks if there are 3 of the same kind lined up vertically
		if (x->father == nullptr) return false;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 5; j++) {
				if (x->table[i][j] == x->father->who_plays
					&& x->table[i + 1][j] == x->table[i + 2][j]
					&& x->father->who_plays == x->table[i + 2][j]) return true;
			}
		}

		// same as above, but horizontally

		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 5; i++) {
				if (x->table[i][j] == x->father->who_plays
					&& x->table[i][j + 1] == x->table[i][j + 2]
					&& x->father->who_plays == x->table[i][j + 2]) return true;
			}
		}

		// diagonally 

		for (int i = 2; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				if (x->table[i][j] == x->father->who_plays
					&& x->table[i - 1][j + 1] == x->father->who_plays
					&& x->table[i - 2][j + 2] == x->father->who_plays)
					return true;
			}
		}

		// other diagonal

		for (int i = 2; i < 5; i++) {
			for (int j = 2; j < 5; j++) {
				if (x->table[i][j] == x->father->who_plays &&
					x->table[i - 1][j - 1] == x->father->who_plays &&
					x->table[i - 2][j - 2] == x->father->who_plays)
					return true;
			}
		}
		return false;

	}


	//returns who won the game (the result)
	int is_done_res(node* x) {

		//vertical
		if (x->father == nullptr) return false;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 5; j++) {
				if (x->table[i][j] == x->father->who_plays
					&& x->table[i + 1][j] == x->table[i + 2][j]
					&& x->father->who_plays == x->table[i + 2][j]) {

					if (x->father->who_plays == root->who_plays) return 1;
					else return -1;
				}
			}
		}

		//horizontal

		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 5; i++) {
				if (x->table[i][j] == x->father->who_plays
					&& x->table[i][j + 1] == x->table[i][j + 2]
					&& x->father->who_plays == x->table[i][j + 2]) {
					if (x->father->who_plays == root->who_plays) return 1;
					else return -1;
				};
			}
		}

		// diagonal #1

		for (int i = 2; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				if (x->table[i][j] == x->father->who_plays
					&& x->table[i - 1][j + 1] == x->father->who_plays
					&& x->table[i - 2][j + 2] == x->father->who_plays)
				{
					if (x->father->who_plays == root->who_plays) return 1;
					else return -1;
				}
			}
		}

		// diagonal #2

		for (int i = 2; i < 5; i++) {
			for (int j = 2; j < 5; j++) {
				if (x->table[i][j] == x->father->who_plays
					&& x->table[i - 1][j - 1] == x->father->who_plays
					&& x->table[i - 2][j - 2] == x->father->who_plays)
				{
					if (x->father->who_plays == root->who_plays) return 1;
					else return -1;
				}
			}
		}

	}


	//checks if a column is full
	bool is_col_full(node* x, int j) {
		for (int i = 0; i < 5; i++) {
			if (x->table[i][j] == 0) return false;
		}
		return true;
	}


	//Puts a circle on the board depending on what move the father made.
	void add_new(node* x, int j) {
		int k = 4;
		while (x->table[k][j] != 0)k--;
		x->table[k][j] = x->father->who_plays;
	}


	void print_matrix(node* x) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
				cout << x->table[i][j] << " ";
			cout << endl;
		}
		if (x->result == 0 || x->result == 1 || x->result == -1) cout << endl << x->result;
		cout << endl << endl;
	}


	int index_of_first(node* x) {
		int i = 0;
		while (x->children[i] == nullptr) i++;
		return i;
	}

	int identify_son(node* x) {
		int i = 0;
		while (x->father->children[i] != x)i++;
		return i;
	}

	//returns either the min value of the result
	//or the max value depending on who is playing
	int find_minimax(node* x) {
		if (x->who_plays == root->who_plays) {
			int max = -1;
			for (int i = 0; i < 5; i++) {
				if (x->arr[i] > max && x->arr[i] != -2)max = x->arr[i];
			}
			return max;
		}
		else if (x->who_plays != root->who_plays) {
			int min = 1;
			for (int i = 0; i < 5; i++) {
				if (x->arr[i] < min && x->arr[i] != -2)min = x->arr[i];
			}
			return min;
		}
	}



public:
	Tree(int arr[5][5]) {
		root = new node(arr);
	}


	void generate_game_tree() {
		Stack stack;
		node* next;
		stack.push(root);
		while (!stack.stack_empty())
		{
			next = stack.pop();
			while (next != 0) {

				//creates new children if the table isnt full
				//or the winner hasn't been found
				if (!is_full(next->table) && !is_done(next)) {		

					for (int i = 0; i < 5; i++) {
						if (!is_col_full(next, i))
						{
							if (next->children == nullptr || next->children[i] == nullptr) {
								if (next->children == nullptr) {
									next->children = new node * [5]; for (int i = 0; i < 5; i++) next->children[i] = nullptr;
								}

								next->children[i] = new node(next->table, next);
								add_new(next->children[i], i);

								// Determines the result of the table
								// full and no winner -> 0
								// a winner exists? 1 if the one who played first won
								//                -1 if the second player won
								if (is_full(next->children[i]->table)) next->children[i]->result = 0;
								if (is_done(next->children[i])) next->children[i]->result = is_done_res(next->children[i]);
							}
						}
					}
				}

				if (next->children != nullptr)
					for (int i = 4; i > index_of_first(next); i--) {
						if (next->children[i] != nullptr) stack.push(next->children[i]);
					}
				if (next->children != nullptr)next = next->children[index_of_first(next)];
				else next = nullptr;

			}
		}

	}



	//prints the whole tree
	void print_everything() {
		Stack stack;
		node* next;
		stack.push(root);
		while (!stack.stack_empty())
		{
			next = stack.pop();
			while (next != 0) {

				print_matrix(next);

				if (next->children != nullptr)
					for (int i = 4; i > index_of_first(next); i--) {
						if (next->children[i] != nullptr) stack.push(next->children[i]);
					}
				if (next->children != nullptr)next = next->children[index_of_first(next)];
				else next = nullptr;

			}
		}

	}


	//function that uses propagation to determine
	//the results of the whole tree
	void minimax() {
		Stack stack;
		node* next;
		stack.push(root);
		while (!stack.stack_empty())
		{
			next = stack.pop();
			while (next != 0) {

				if (next->children == nullptr && next->father != nullptr && next->father != root) {
					node* temp = next;
					int id = identify_son(next);
					while (temp->father != nullptr && temp->father != root) {
						id = identify_son(temp);
						temp->father->arr[id] = temp->result;
						temp->father->result = find_minimax(temp->father);
						temp = temp->father;
					}

				}

				if (next->children != nullptr)
					for (int i = 4; i > index_of_first(next); i--) {
						if (next->children[i] != nullptr) stack.push(next->children[i]);
					}
				if (next->children != nullptr)next = next->children[index_of_first(next)];
				else next = nullptr;

			}
		}

	}


};

int main() {
	srand(time(NULL));
	int n;
	int arr[5][5];


	cout << "Greetings! Choose what you want to do:" << endl << endl << endl;



	while (1) {
		cout << "1 - Matrix input for the 'starting state' of the game" << endl;
		cout << "2 - Output" << endl;
		cout << "0 - End" << endl;
		cin >> n;
		switch (n) {
		case 1: {
			cout << endl << endl << "Please put in the values of the matrix:" << endl;
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					cin >> arr[i][j];

			break;
		}
		case 2: {
			cout << endl << endl;
			Tree x(arr);
			x.generate_game_tree();
			x.minimax();
			x.print_everything();
			break;
		}
		case 0: exit(1);

		}
	}
	system("pause");

}
