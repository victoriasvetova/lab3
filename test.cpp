#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "pch.h"
#include "binary_tree.h"

TEST(test_insert, test1) {//тест insert
	binary_tree<int>* obj = new binary_tree<int>();
	obj->insert(10);
	EXPECT_EQ(obj->get_size(), 1);
	EXPECT_TRUE(true);
}
TEST(test_reduce, test2) {//тест reduce
	binary_tree<int>* obj = new binary_tree<int>();
    obj->insert(16);
    obj->insert(8);
    obj->insert(4);
    obj->insert(12);
    obj->insert(1);
    obj->insert(5);
    obj->insert(10);
    obj->insert(14);
    obj->insert(24);
    obj->insert(20);
    obj->insert(18);
    obj->insert(22);
    obj->insert(32);
    obj->insert(30);
    obj->insert(64);
	EXPECT_EQ(obj->Reduce(sum), 280);
	EXPECT_TRUE(true);
}
TEST(test_map, test3) {//тест map
    binary_tree<int>* obj = new binary_tree<int>();
    obj->insert(16);
    obj->insert(8);
    obj->insert(4);
    obj->insert(12);
    obj->insert(1);
    obj->insert(5);
    obj->insert(10);
    obj->insert(14);
    obj->insert(24);
    obj->insert(20);
    obj->insert(18);
    obj->insert(22);
    obj->insert(32);
    obj->insert(30);
    obj->insert(64);
    obj->Map(sqr);
    EXPECT_EQ(obj->tree_to_string(), "1 16 25 64 100 144 196 256 324 400 484 576 900 1024 4096 ");
    EXPECT_TRUE(true);
}
TEST(test_find, test4) {//тест find
    binary_tree<int>* obj = new binary_tree<int>();
    obj->insert(16);
    obj->insert(8);
    obj->insert(4);
    obj->insert(12);
    obj->insert(1);
    obj->insert(5);
    obj->insert(10);
    obj->insert(14);
    obj->insert(24);
    obj->insert(20);
    obj->insert(18);
    obj->insert(22);
    obj->insert(32);
    obj->insert(30);
    obj->insert(64);
    EXPECT_EQ(obj->find(4),1);
    EXPECT_TRUE(true);
}
TEST(test_delete, test5) {//тест delete
    binary_tree<int>* obj = new binary_tree<int>();
    obj->insert(16);
    obj->insert(8);
    obj->insert(4);
    obj->insert(12);
    obj->insert(1);
    obj->insert(5);
    obj->insert(10);
    obj->insert(14);
    obj->insert(24);
    obj->insert(20);
    obj->insert(18);
    obj->insert(22);
    obj->insert(32);
    obj->insert(30);
    obj->insert(64);
    obj->delete_elem(4);
    EXPECT_EQ(obj->find(4), 0);
    EXPECT_TRUE(true);
}
TEST(test_get_sub_tree, test6) {//тест get_sub_tree
    binary_tree<int>* first = new binary_tree<int>();
    int val[8] = { 5, 3, 9, 4, 1, 0, 8, 2 };
    for (int i = 0; i < 8; i++) {
        first->insert(val[i]);
    }
    binary_tree<int>* sub = first->get_sub_tree(3);
    EXPECT_EQ(sub->tree_to_string(), "0 1 2 3 4 5 8 9 ");
    EXPECT_TRUE(true);
}
TEST(test_is_balanced_str, test7) {//тест is_balanced_str
    binary_tree<int>* first = new binary_tree<int>();
    int val[8] = { 5, 3, 9, 4, 1, 0, 8, 2 };
    for (int i = 0; i < 8; i++) {
        first->insert(val[i]);
    }
    
    EXPECT_EQ(first->is_balanced_str(), "0 0 0 -1 0 -1 0 1 ");
    EXPECT_TRUE(true);
}
//where?
/*
TEST(test_where, test8) {//тест where
    binary_tree<int>first;
    int val[5] = { 9, 6, 8, 3, 2 };
    for (int i = 0; i < 5; i++) {
        first.insert(val[i]);
    }
    first.Where(is_more_than_25);
    EXPECT_EQ(first.tree_to_string(), "2 3 6 8 9 ");
    EXPECT_TRUE(true);
}*/