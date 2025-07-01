#ifndef BINARYTREENODE_HPP
#define BINARYTREENODE_HPP
#include <memory>
#include <string>
#include <sstream>
#include "tools/DebugLog.h"

namespace algo
{
    template <typename KType>
    struct BinaryTreeNode
    {
    public:
        typedef KType KeyType;    /*节点保存数据类*/
        typedef std::shared_ptr<BinaryTreeNode<KType>> NodePtr; /*节点指针类型*/

        BinaryTreeNode()
        :key(KeyType()), lchild(NodePtr()), rchild(NodePtr()), parent(NodePtr())
        {}

        BinaryTreeNode(const KeyType& k)
        :key(k), lchild(NodePtr()), rchild(NodePtr()), parent(NodePtr())
        {}

        virtual std::string to_string()
        {
            std::ostringstream oss;
            oss << "key: " << key;
            if(auto shared_p = parent.get())
            {
                oss << ", parent: " << shared_p->key;
            }else{
                oss << ", parent: null";
            }

            oss << ", lchild: ";
            if(lchild.get() == nullptr) 
            oss << std::string("null");
            else 
            oss << lchild->key;
            
            oss << ", rchild: " ;
            if(rchild.get() == nullptr )
            oss << std::string("null");
            else 
            oss << rchild->key;
            return oss.str();
        }

        virtual std::string to_xml()
        {
            std::ostringstream oss;
            oss << "\n <node>" << key << "\n";
            if(auto shared_p = parent.get())
            {
                oss << "\t <parent>" << shared_p->key << "</parent>";
            }else{
                oss << "\t <parent>null</parent>";
            }
            oss << "\n\t <lchild>" << (lchild.get() == nullptr ? std::string("null") : lchild->to_xml()) << "</lchild>"\
                << "\n\t <rchild>" << (rchild.get() == nullptr ? std::string("null") : rchild->to_xml()) << "</rchild>"\
                << "\n </node>";
                return oss.str();
        }

        bool is_left_child()
        {
            if(auto shared_p = parent.get())
            {
                return shared_p->lchild.get() == this;
            }
            return false;
            
        }

        bool is_right_child()
        {
            if(auto shared_p = parent.get())
            {
                return shared_p->rchild.get() == this;
            }
            return false;
            
        }

        KeyType key; /*节点保存数据*/
        NodePtr lchild;
        NodePtr rchild;
        NodePtr parent; /*父节点指针*/
    };




}


#endif