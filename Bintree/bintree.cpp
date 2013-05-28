#include<iostream>

#include<cstdlib>

#define NULL 0

using namespace std;

struct tree {
	int data;
	tree *Left, *Right;
};

tree *root;

class bin_tree{

public:
	bin_tree();
	void insert_data(int);
	tree *insert_node(tree *, tree *);
	void Delete(int);
	void pretrav(tree *);
	void intrav(tree *);
	void print_tree(tree *);
};

bin_tree::bin_tree() {

	root = NULL;
}

tree* bin_tree::insert_node(tree *temp, tree *newnode){

	if(temp == NULL)
		temp=newnode;

	else if(temp->data < newnode->data) {

		insert_node(temp->Right,newnode);

		if(temp->Right == NULL)
			temp->Right = newnode;

	} else {

		insert_node(temp->Left, newnode);

		if(temp->Left == NULL)
			temp->Left = newnode;
	}

	return temp;
}

void bin_tree::insert_data(int n) {

	tree *temp=root,*newnode;

	newnode=new tree;
	newnode->Left=NULL;
	newnode->Right=NULL;
	newnode->data=n;

	root=insert_node(temp,newnode);
}

void bin_tree::print_tree(tree *t = root){

	if(root==NULL){

		cout<<"Nothing to display";

	} else
		if(t != NULL) {

			print_tree(t->Left);
			print_tree(t->Right);
			cout<< t->data << " ";
		}
}

void bin_tree::Delete(int key)
{

	tree *temp = root, *parent = root, *marker;

	if (temp == NULL) {
		cout<<"The tree is empty"<<endl;

	} else {

		while(temp != NULL && temp->data != key) {

			parent=temp;

			if(temp->data < key)
				temp = temp->Right;
			else
				temp = temp->Left;
		}
	}

	marker = temp;

	if(temp==NULL)
		cout<<"No node present";

	else if(temp == root){

		if(temp->Right==NULL && temp->Left==NULL)
			root = NULL;

		else if(temp->Left==NULL)
			root = temp->Right;

		else if(temp->Right==NULL)
			root = temp->Left;

		else {

			tree *temp1;
			temp1 = temp->Right;

			while(temp1->Left!=NULL) {

				temp = temp1;
				temp1 = temp1->Left;
			}

			if(temp1!=temp->Right) {

				temp->Left = temp1->Right;
				temp1->Right = root->Right;
			}

			temp1->Left = root->Left;
			root = temp1;
		}

	} else {

		if(temp->Right==NULL && temp->Left==NULL){

			if (parent->Right == temp)
				parent->Right=NULL;
			else
				parent->Left=NULL;

		} else if (temp->Left == NULL) {

			if (parent->Right == temp)
				parent->Right = temp->Right;
			else
				parent->Left = temp->Right;
		}

		else if (temp->Right == NULL){

			if (parent->Right == temp)
				parent->Right = temp->Left;
			else
				parent->Left = temp->Left;

		} else {

			tree *temp1;
			parent = temp;
			temp1 = temp->Right;

			while(temp1->Left != NULL){

				parent = temp1;
				temp1 = temp1->Left;

			}

			if(temp1 != temp->Right){

				temp->Left = temp1->Right;
				temp1->Right = parent->Right;

			}

			temp1->Left=parent->Left;
			parent=temp1;
		}
	}

	delete marker;
}

int main(){

	bin_tree bt;
	int choice, n, key;

	while(1) {

		cout<<"\n1. Insert\n2. Delete\n3. Print\n4. Exit"<<endl;
		cout<<"Enter your choice: ";

		cin>>choice;

		switch(choice) {

		case 1:

			cout<<"Enter item: ";
			cin>>n;
			bt.insert_data(n);
			break;

		case 2:

			cout<<"Enter element to delete: ";
			cin>>key;
			bt.Delete(key);
			break;

		case 3:

			cout<<endl;
			bt.print_tree();
			break;

		case 4:
			exit(0);

		}
	}

	return 0;
}