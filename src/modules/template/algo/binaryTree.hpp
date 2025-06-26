#ifndef __BINARYTREE_H__
#define __BINARYTREE_H__
#include <memory>
#include <functional>

namespace algo
{

template <typename NodeT>
struct BinaryTree
{
    public:
        typedef NodeT NodeType;
        typedef typename NodeT::keyType keyType;

        BinaryTree() :root(std::shared_ptr<NodeT>(nullptr)) {}

        /**
         * @brief Show the tree xml format.
         */
        std::string to_xml() const
        {
            if(root)
            {
                return root->to_xml();
            }
            else{
                return "root is null";
            }

        }

        std::shared_ptr<NodeT> root;
};

/**
 * @brief A binary tree inorder traverseal.
 */
template <typename NodeT, typename ActionType = std::function<void (typename NodeT::keyType T)>>
void inorder_traversal(std::shared_ptr<NodeT> root, ActionType action = [](typename NodeT::keyType T){})
{
    if(root)
    {
        inorder_traversal(root->lchild, action);
        action(root->key);
        inorder_traversal(root->rchild, action);
    }
}
/**
 * @brief A binary tree preorder traverseal.
 */
template <typename NodeT, typename ActionType = std::function<void (typename NodeT::keyType T)>>
void preorder_traversal(std::shared_ptr<NodeT> root, ActionType action = [](typename NodeT::keyType T){})
{
    if(root)
    {
        action(root->key);
        oreoder_traversal(root->lchild, action);
        oreoder_traversal(root->rchild, action);
    }
}

/**
 * @brief A binary tree postorder traverseal.
 */
template <typename NodeT, typename ActionType = std::function<void (typename NodeT::keyType T)>>
void postorder_traversal(std::shared_ptr<NodeT> root, ActionType action = [](typename NodeT::keyType T){})
{

    if(root)
    {
        postorder_traversal(root->lchild, action);
        postorder_traversal(root->rchild, action);
        action(root->key);
    }
}

/**
 * A binary tree left rotate.
 */
template <typename NodeT>
void left_rotate(std::shared_ptr<NodeT>& node, std::shared_ptr<NodeT>& root)
{
    if(node)
    {
        auto right = node->rchild;
        if(right)
        {
            auto l_right = right->child;
            right->parent = node->parent;
            auto shared_p = node->parent.lock();
            if(shared_p)
            {
                if(node->is_left_child())
                {
                    shared_p->lchild = right;
                }else if(node->is_right_child())
                {
                    shared_p->rchild = right;
                }
            }else{
                root = right;
            }
            node->right = l_right;
            if(l_right) 
            {
                l_right->parent = node;
            }
            node->parent = right;
            right->lchild = node;
            
        }
    }
}
/**
 * A binary tree right rotate.
 */
template <typename NodeT>
void right_rotate(std::shared_ptr<NodeT>& node, std::shared_ptr<NodeT>& root)
{
    if(node)
    {
        auto left  = node->lchild;
        if(left)
        {
            auto r_left = left->child;
            left->parent = node->parent;
            auto shared_p = node->parent.lock();
            if(shared_p)
            {
                if(node->is_left_child())
                {
                    shared_p->lchild = left;
                }
                else if(node->is_right_child())
                {
                    shared_p->rchild = left;
                }
                
            }
            else{
                root = left;
            }
            node->lchild = r_left;
            if(r_left)
            {
                r_left->parent = node;
            }
            node->parent = left;
            left->rchild = node;
        }
    }
}


}
#endif