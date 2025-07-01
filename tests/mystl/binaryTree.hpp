#ifndef TEST_BINARY_TREE_HPP
#define TEST_BINARY_TREE_HPP
#include "template/algo/binaryTreeNode.hpp"
#include "template/algo/binaryTree.hpp"
#include <gtest/gtest.h>
#include<sstream>

using namespace algo;

class BinaryTreeNodeTest:public ::testing::Test
{
public:
    typedef BinaryTreeNode<int> Node;
protected:
    void SetUp()
    {
        default_node=std::shared_ptr<Node>(new Node());
        root_node=std::make_shared<Node>(0);
        root_node->lchild=std::make_shared<Node>(1);
        root_node->rchild=std::make_shared<Node>(2);
        root_node->lchild->parent=root_node;
        root_node->rchild->parent=root_node;
    }
    void TearDown()
    {}

    std::shared_ptr<Node> default_node; /*!< 指向一个默认构造的结点*/
    std::shared_ptr<Node> root_node;    /*!< 指向一个非默认构造的结点*/
};

//!binary_tree_node_test：测试二叉树节点
/*!
*
* 当创建默认节点时，节点应该满足的特征
*/
TEST_F(BinaryTreeNodeTest, test_default_node)
{
    EXPECT_EQ(default_node->key,0);
    EXPECT_FALSE(default_node->lchild); //shared_ptr<T> 直接代表真假
    EXPECT_EQ(default_node->rchild.get(),nullptr); //也可以抽取其指针
    EXPECT_FALSE(default_node->parent.get());  //weak_ptr<T> 必须通过lock()获取一个shared_ptr<T>
}



#endif