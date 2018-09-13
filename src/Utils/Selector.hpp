/*
 * Copyright 2008  Johns Hopkins University
 * All Rights Reserved
 *
 *       Created by Erion Plaku
 *       Laboratory for Computational Sensing and Robotics
 *       Johns Hopkins University
 */

/**
 *@file Selector.H
 *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
 *@brief Given items <em>p1, p2, ..., pn</em> with weights
 *       <em>w1, w2, ..., wn</em>, select item <em>pi</em>
 *       with probability <em>wi / (w1 + ... + wn)</em>
 */

#ifndef UTILS__SELECTOR_H_
#define UTILS__SELECTOR_H_

#include "Utils/PseudoRandom.hpp"

namespace Antipatrea
{
    /**
     *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
     *@brief Given items <em>p1, p2, ..., pn</em> with weights
     *       <em>w1, w2, ..., wn</em>, select item <em>pi</em>
     *       with probability <em>wi / (w1 + ... + wn)</em>
     *
     *@par Description:
     *  Items are inserted into a balanced binary tree (according to the weights).
     *  This makes it possible to select/update weights in logarithmic time.
     */
    template <typename Key>
    class Selector
    {
    public:
                /**
                 *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
                 *@brief Initially the binary tree is empty
                 */
                Selector(void) : m_root(NULL),
                                 m_nNodes(0)
                {
                }

                /**
                 *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
                 *@brief Destructor deletes the binary tree
                 */
                virtual ~Selector(void)
                {
                	DeleteSubTree(m_root);
                }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Contains item key and weight as well as
         *       left, right, and parent pointers
         */
        class Node
        {
            friend class Selector;

        public:
            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Initially the node is empty
             */
            Node(void) : m_weight(0.0),
                         m_left(NULL),
                         m_right(NULL),
                         m_parent(NULL)
            {
            }

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Destructor does nothing
             */
            virtual ~Node(void)
            {
            }

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Get node key
             */
            virtual Key GetKey(void) const
            {
				return m_key;
            }

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Get node weight
             */
            virtual double GetWeight(void) const
            {
				return m_weight;
            }

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Set node key
             *
             *@param key node key
             */
            virtual void SetKey(Key key)
            {
                    m_key = key;
            }

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Set node weight
             *
             *@param w node weight
             */
            virtual void SetWeight(const double w)
            {
				m_weight = w;
            }

        protected:
            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Update node weight, i.e., set to sum of its children' weights
             */
            virtual void Update(void)
            {
				m_weight = (m_left  ? m_left->m_weight : 0.0) +
						(m_right ? m_right->m_weight: 0.0);
            }

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Node key
             */
            Key    m_key;

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Node weight
             */
            double m_weight;

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Pointer to left child
             */
            Node  *m_left;

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Pointer to right child
             */
            Node  *m_right;

            /**
             *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
             *@brief Pointer to parent
             */
            Node  *m_parent;
        };

        int GetNrNodes(void) const
        {
            return m_nNodes;
        }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Select item
         *
         *@param wselect selection weight between <em>0</em> and <em>w1 + ... + wn</em>
         */
        virtual Node* Select(void) const
        {
            return Select(RandomUniformReal(0, m_root->m_weight));
        }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Select item
         *
         *@param wselect selection weight between <em>0</em> and <em>w1 + ... + wn</em>
         */
        virtual Node* Select(const double wselect) const
        {
			Node   *node = m_root;
			double  w    = wselect;

			while(node->m_left != NULL || node->m_right != NULL)
			{
				if(node->m_left != NULL && (node->m_right == NULL || node->m_left->m_weight > w))
					node = node->m_left;
				else if(node->m_right != NULL)
				{
					w   -= node->m_left ? node->m_left->m_weight : 0.0;
					node = node->m_right;
				}
				else //selection failed : should not happen
					return NULL;
			}
			return node;
        }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Create new node
         */
        virtual Node* Create(void) const
        {
            return new Node();
        }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Insert node into selector
         *
         *@param node node to be inserted
         *
         *@remarks
         *  Key and weight should be set prior to inserting <em>node</em>
         *  into the selector
         */
        virtual void Insert(Node * const node)
        {
			++m_nNodes;

			node->m_left = node->m_right = node->m_parent = NULL;

			if(m_root == NULL)
			{
				m_root = node;
				return;
			}

			//find position
			Node *parent = m_root;
			while(parent->m_left != NULL && parent->m_right != NULL)
					parent = parent->m_left->m_weight < parent->m_right->m_weight ? parent->m_left : parent->m_right;

			if(parent->m_left == NULL && parent->m_right != NULL)
			{
				node->m_parent = parent;
				parent->m_left = node;
			}
			else if(parent->m_left != NULL && parent->m_right == NULL)
			{
				node->m_parent = parent;
				parent->m_right= node;
			}
			else
			{
				Node *gparent = parent->m_parent;
				Node *newnode = Create();

				newnode->m_parent = gparent;
				if(gparent)
				{
					if(gparent->m_left == parent) gparent->m_left = newnode;
					else	gparent->m_right= newnode;
				}

				newnode->m_left = parent;
				parent->m_parent= newnode;

				newnode->m_right = node;
				node->m_parent   = newnode;

				if(parent == m_root)
						m_root = newnode;
			}

			Update(node);
        }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Update selector
         *
         *@param node node to be updated
         *@param  w new node weight
         */
        virtual void Update(Node * const node, const double w)
        {
            node->m_weight = w;
            Update(node);
        }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Update selector
         *
         *@param node node to be updated
         */
        virtual void Update(Node * const node)
        {
            Node *parent = node->m_parent;
            while(parent != NULL)
            {
                    parent->Update();
                    parent = parent->m_parent;
            }
        }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Remove node from selection
         *
         *@param node node to be removed
         *
         *@remarks
         * Items are stored in leaf nodes, so only leaf nodes can be removed.
         * That is, the left and right children of <em>node</em> should be <em>NULL</em>.
         */
        virtual void Remove(Node * const node)
        {
            --m_nNodes;

            Node *parent = node->m_parent;

            if(parent)
            {
                if(parent->m_left == node)
                    parent->m_left = NULL;
                else
                    parent->m_right = NULL;

                parent->Update();
                Update(parent);

                if(parent->m_left == NULL && parent->m_right == NULL)
                    Remove(parent);
            }
            else //root
                m_root = NULL;

            node->m_parent = node->m_left = node->m_right = NULL;
        }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Clear selector by removing all items
         */
        virtual void Clear(void)
        {
            m_nNodes = 0;

            DeleteSubTree(m_root);
            m_root = NULL;
        }

    protected:
        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Delete subtree rooted at <em>node</em>
         *
         *@param node root of subtree
         */
        virtual void DeleteSubTree(Node * const node)
        {
            if(node)
            {
                DeleteSubTree(node->m_left);
                DeleteSubTree(node->m_right);
                delete node;
            }
        }

        /**
         *@author <a href="http:/www.cs.jhu.edu/~erion">Erion Plaku</a>
         *@brief Root of binary tree
         */
        Node *m_root;

        int m_nNodes;
        //--------------------------------------------------------------------------
                public:
        virtual Node* Find(Key key)
        {
                return Find(key, m_root);

        }
        //--------------------------------------------------------------------------
        virtual Node* Find(Key key, Node* root)
        {
			if (root->GetKey() == key) return root;
			else
			{
				Node* n = NULL;
				if (root->m_left)
				{
					n = Find(key,root->m_left);
					if (n) return n;
				}
				if (root->m_right)
				{
					n =  Find(key,root->m_right);
					if (n) return n;
				}
				return NULL;
		}
        }

};
}

#endif
