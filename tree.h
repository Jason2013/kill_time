#ifndef __TREE_H
#define __TREE_H

#ifndef NULL
#define NULL 0L
#endif

/*
	
	TREE.H

	The CNode class

	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/29/2001
	Description: The CNode class represents a single node in the
			   cyclic linked list tree that holds all objects
			   in the game.

*/

// tree class
class CNode
{
public:
     // data
     CNode *parentNode;  // parent node
     CNode *childNode;   // child node
     CNode *prevNode;    // previous node
     CNode *nextNode;    // next node

	 CNode * prev, * next;

	 static CNode * header, * tail;


     bool HasParent() { return (parentNode != NULL); } // does node have a parent?
     bool HasChild() { return (childNode != NULL); }   // does node have a child?

     // is this node the first child?
     bool IsFirstChild()
     {
          if (parentNode)
               return (parentNode->childNode == this);
          else
               return false;
     }

     // is this node the last child?
     bool IsLastChild()
     {
          if (parentNode)
               return (parentNode->childNode->prevNode == this);
          else
               return false;
     }

     // attach this node to a parent node
     void AttachTo(CNode *newParent)
     {
          // if this node is already attached to another node, then detach
          if (parentNode)
               Detach();

          parentNode = newParent;

          if (parentNode->childNode)
          {
               prevNode = parentNode->childNode->prevNode;
               nextNode = parentNode->childNode;
               parentNode->childNode->prevNode->nextNode = this;
               parentNode->childNode->prevNode = this;
          }
          else
          {
               parentNode->childNode = this;      // this is the first child
          }
     }

     // attach a child to this node
     void Attach(CNode *newChild)
     {
          // if the child node is already attached, then detach it
          if (newChild->HasParent())
               newChild->Detach();

          newChild->parentNode = this;

          if (childNode)
          {
               newChild->prevNode = childNode->prevNode;
               newChild->nextNode = childNode;
               childNode->prevNode->nextNode = newChild;
               childNode->prevNode = newChild;
          }
          else
               childNode = newChild;
     }

     // detach node from parent
     void Detach()
     {
          // if this node is the first child of the parent (first in list)
          // then the parent points to the next child in the list
          if (parentNode && parentNode->childNode == this)
          {
               if (nextNode != this)
                    parentNode->childNode = nextNode;
               else
                    parentNode->childNode = NULL;      // no next child
          }

          // get rid of links
          prevNode->nextNode = nextNode;
          nextNode->prevNode = prevNode;

          // now this node is not in the list
          prevNode = this;
          nextNode = this;
     }

     // count the number of nodes
     int CountNodes()
     {
          if (childNode)
               return childNode->CountNodes() + 1;
          else
               return 1;
     }

     // constructor
	 CNode() : prev(NULL), next(NULL)                                 // setup node
     {
          parentNode = childNode = NULL;
          prevNode = nextNode = this;

		  link_add(this);
		  
     }

     // constructor
     CNode(CNode *node) : prev(NULL), next(NULL)
     {
          parentNode = childNode = NULL;     // setup and attach this node to node
          prevNode = nextNode = this;
          AttachTo(node);

		  link_add(this);
     }

     // destructor
     virtual ~CNode()
     {
		 link_remove(this);
          Detach();                // detach from hierarchy

          //while (childNode)        // delete all children
          //{
          //     //delete childNode;
          //}
     }

	 void link_add(CNode * p)
	 {
		 //VTRACE("add type=%s,(%u)\n", typeid(*this).name(), this);
		 //VTRACE("count=%d\n", link_count());
		 //
		 if (header == NULL && tail == NULL)
		 {
			 header = tail = p;
			 return;
		 }

		 tail->next = p;
		 p->prev = tail;
		 tail = p;
	 }
	 void link_remove(CNode * p)
	 {
		 //VTRACE("del type=%s,(%u)\n", typeid(*this).name(), this);
		 //VTRACE("count=%d\n", link_count());

		 if (header == p && tail == p) // only one
		 {
			 header = tail = NULL;
			 return;
		 }
		 if (header == p)
		 {
			 header = p->next;
			 return;
		 }
		 if (tail == p)
		 {
			 tail = p->prev;
			 tail->next = NULL;
			 return;
		 }
		 p->prev->next = p->next;
		 p->next->prev = p->prev;
	 }
	 int link_count()
	 {
		 int c = 0;
		 for (CNode * t = header; t != NULL; t=t->next)
		 {
			 c++;
		 }
		 return c;
	 }
};


#endif