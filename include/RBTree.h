#ifndef __RBTREE_H_PROTECTED__
#define __RBTREE_H_PROTECTED__
#include "Comparable.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif // _DEBUG

enum color_t{RED=0, BLACK};
enum dir_t{ LEFT, RIGHT};

template <typename KeyType>
class RBNode
{
public:
	RBNode(Comparable<KeyType>* item);
	~RBNode();
	cmp_t compare(KeyType key) { return _data->Compare(key); }

	static bool is_red(RBNode<KeyType>* &root);
	static void flip_color(RBNode<KeyType>* &root);
	static void rotate(RBNode<KeyType>* &root, dir_t dir);

	static void fix_up(RBNode<KeyType>* &root);
	static void delete_min(RBNode<KeyType>* &root);
	static void move_red_left(RBNode<KeyType>* &root);
	static void move_red_right(RBNode<KeyType>* &root);

	static RBNode<KeyType>* get_inorder_node(RBNode<KeyType>* &root);
	
	static Comparable<KeyType>* search(RBNode<KeyType>* &root, KeyType key);
	static void insert(RBNode<KeyType>* &root, Comparable<KeyType>* &item) {
		_insert(root, item);
		root->_color = BLACK;
	}

	static bool _remove(RBNode<KeyType>* &root, KeyType key);
	static bool remove(RBNode<KeyType>* &root, KeyType key) {
		bool ret = _remove(root, key);
		root->_color = BLACK;
		return ret;
	}

#ifdef _DEBUG
	enum TraversalOrder { LTREE, KEY, RTREE };
	static void Dump(TraversalOrder order, const RBNode<KeyType> * node, int level = 0);
	static void Dump(RBNode<KeyType>* &root, int level = 0);
#endif // _DEBUG
	
protected:
	static void _insert(RBNode<KeyType>* &root, Comparable<KeyType>* &item);
private:
	color_t _color;
	Comparable<KeyType>* _data;
	RBNode<KeyType>* _left, *_right;
};


template <typename KeyType>
class RBTree
{
public:
	RBTree();
	//~RBTree();

	void insert(Comparable<KeyType>* item) {
		RBNode<KeyType>::insert(_root, item);
	}
	Comparable<KeyType>* search(KeyType key) {
		return RBNode<KeyType>::search(_root, key);
	}
	bool remove(KeyType key) {
		return RBNode<KeyType>::remove(_root, key);
	}

#ifdef _DEBUG
	void DumpTree() {
		RBNode<KeyType>::Dump(_root, 0);
	}
#endif // _DEBUG
private:
	RBNode<KeyType>* _root;
};


template <typename KeyType>
RBNode<KeyType>::RBNode(Comparable<KeyType>* item)
	:_data(item->Clone()), _color(RED)
{
	_left = _right = NULL;
}

template <typename KeyType>
RBNode<KeyType>::~RBNode() {
	delete _data;
	_data = NULL;
	_left = _right = NULL;
}

template <typename KeyType>
bool RBNode<KeyType>::is_red(RBNode<KeyType>* &root)
{
	if (root == NULL){
		return false;
	}
	return root->_color == RED;
}

template <typename KeyType>
void RBNode<KeyType>::flip_color(RBNode<KeyType>* &root)
{
	root->_color = color_t(1 - int(root->_color));
	if (root->_left != NULL){
		root->_left->_color = color_t(1 - int(root->_left->_color));
	}
	if (root->_right != NULL) {
		root->_right->_color = color_t(1 - int(root->_right->_color));
	}
}

template <typename KeyType>
void RBNode<KeyType>::rotate(RBNode<KeyType>* &root, dir_t dir)
{
	RBNode<KeyType>* old_root = root;
	if (dir == LEFT) {
		root = old_root->_right;
		old_root->_right = root->_left;
		root->_left = old_root;
	}
	else {
		root = old_root->_left;
		old_root->_left = root->_right;
		root->_right = old_root;
	}

	root->_color = old_root->_color;
	old_root->_color = RED;
}

template <typename KeyType>
void RBNode<KeyType>::fix_up(RBNode<KeyType>* &root)
{	
	if (is_red(root->_right) && is_red(root->_left)) {
		flip_color(root);
	}
	if (is_red(root->_right)) {
		rotate(root, LEFT);
	}
	if (is_red(root->_left) && (root->_left != NULL && is_red(root->_left->_left))) {
		rotate(root, RIGHT);
	}
}

template <typename KeyType>
void RBNode<KeyType>::delete_min(RBNode<KeyType>* &root)
{
	if (root->_left == NULL) {
		delete root;
		root = NULL;
		return;
	}
	if (!is_red(root->_left) && !is_red(root->_left->_left)) {
		move_red_left(root);
	}
	delete_min(root->_left);
	return fix_up(root);
}

template <typename KeyType>
void RBNode<KeyType>::move_red_left(RBNode<KeyType>* &root)
{
	// assume both root->_left and root->_left->_left are black
	flip_color(root);
	if (root->_right != NULL && is_red(root->_right->_left)) {
		rotate(root->_right, RIGHT);
		rotate(root, LEFT);
		flip_color(root);
	}
}

template <typename KeyType>
void RBNode<KeyType>::move_red_right(RBNode<KeyType>* &root)
{
	// assume root->_right and root->_right's children are black
	flip_color(root);
	if (is_red(root->_left) && (root->_left != NULL && is_red(root->_left->_left))) {
		rotate(root, RIGHT);
		flip_color(root);
	}
}

template <typename KeyType>
Comparable<KeyType>* RBNode<KeyType>::search(RBNode<KeyType>* &root, KeyType key)
{
	RBNode<KeyType>* tmp = root;
	while (tmp) {
		cmp_t cmp = tmp->_data->Compare(key);
		if (cmp == CMP_EQ) {
			return tmp->_data;
		}else {
			tmp = cmp == CMP_MIN ? tmp->_right : tmp->_left;
		}
	}
	return NULL;
}

template <typename KeyType>
RBNode<KeyType>* RBNode<KeyType>::get_inorder_node(RBNode<KeyType>* &root)
{
	if (root->_right == NULL) {
		return NULL;
	}
	RBNode<KeyType>* node = root->_right;
	while (node->_left != NULL) {
		node = node->_left;
	}
	return node;
}

template <typename KeyType>
bool RBNode<KeyType>::_remove(RBNode<KeyType>* &root, KeyType key)
{
	cmp_t ret = root->compare(key);
	if (ret == CMP_MAX) {
		// left
		if (root->_left == NULL) {
			// not found
			return false;
		}

		if (!is_red(root->_left) && !is_red(root->_left->_left)) {
			move_red_left(root);
		}
		_remove(root->_left, key);		
	}else {
		if (is_red(root->_left)) {
			rotate(root, RIGHT);
		}

		if (root->compare(key) == CMP_EQ) {
			RBNode<KeyType>* node = get_inorder_node(root);
			if (node != NULL) {
				delete root->_data;
				root->_data = node->_data->Clone();
				delete_min(root->_right);
			}else {
				delete root;
				root = NULL;
				return true;
			}
		}else {
			if (!is_red(root->_right) && !is_red(root->_right->_left)) {
				// root->_right and it's children are black
				move_red_right(root);
			}
			_remove(root->_right, key);
		}
	}
 	fix_up(root);
	return true;
}

template <typename KeyType>
void RBNode<KeyType>::_insert(RBNode<KeyType>* &root, Comparable<KeyType>* &item)
{
	if (root == NULL){
		root = new RBNode<KeyType>(item);
		return;
	}

	if (is_red(root->_left) && is_red(root->_right)) {
		flip_color(root);
	}
	
	cmp_t result = root->compare(item->Key());
	if (result == CMP_EQ) {
		delete root->_data;
		root->_data = item;
	}else {
		if (result == CMP_MIN) {
			_insert(root->_right, item);
		}else {
			_insert(root->_left, item);
		}	
	}
	if (is_red(root->_right)) {
		// left rotate
		rotate(root, LEFT);
	}

	if (is_red(root->_left) && 
		(root->_left != NULL && is_red(root->_left->_left))) {
		// right rotate
		rotate(root, RIGHT);
	}

	 
	if (is_red(root->_left) && is_red(root->_right)) {
		flip_color(root);
	}
}


template <typename KeyType>
RBTree<KeyType>::RBTree()
{

}


#ifdef _DEBUG

static inline void Indent(int len) {
	for (int i = 0; i < len; i++) {
		std::cout << ' ';
	}
}

template <typename KeyType>
void RBNode<KeyType>::Dump(TraversalOrder order, const RBNode<KeyType> * node, int level)
{
	unsigned  len = (level * 5) + 1;
	if ((order == LTREE) && (node->_left == NULL)) {
		Indent(len);
		std::cout << "     **NULL**" << endl;
	}
	if (order == KEY) {
		Indent(len);
		std::cout << node->_data->Key() << ":" << node->_color << endl;
	}
	if ((order == RTREE) && (node->_right == NULL)) {
		Indent(len);
		std::cout << "     **NULL**" << endl;
	}
}


template <typename KeyType>
void RBNode<KeyType>::Dump(RBNode<KeyType>* &root, int level)
{
	if (root != NULL) {
		Dump(RTREE, root, level);
		if (root->_right != NULL) {
			Dump(root->_right, level + 1);
		}
		Dump(KEY, root, level);
		if (root->_left != NULL) {
			Dump(root->_left, level + 1);
		}
		Dump(LTREE, root, level);
	}
}
#endif // _DEBUG
#endif
