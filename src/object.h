#ifndef __OBJECT_H
#define __OBJECT_H

#include "tree.h"
#include "vector.h"
#include "camera.h"

/*
	OBJECT.H

  	The CObject class

	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/29/2001
	Description: The CObject class is derived from CNode and
			   represents an object in the game world.

*/

class CObject : public CNode
{
public:

     // perform basic physics on the object
     virtual void OnAnimate(scalar_t deltaTime) 
     {
          //position += velocity * deltaTime;
          //velocity += acceleration * deltaTime;
		 if (isDead)
			 delete this;
     }
     // collide with other objects
     virtual void OnCollision(CObject *collisionObject) {}
protected:
     // draw the object given the camera position
     virtual void OnDraw(CCamera *camera) {} 



     // perform collision and other preparations for this object
     virtual void OnPrepare() 
     {
          ProcessCollisions(FindRoot());     // perform collisions starting with
                                             // root world object
     }

public:
     CVector position;        // object position
     CVector velocity;        // velocity of object
     CVector acceleration;    // acceleration
     scalar_t size;           // size of bounding sphere (radius)

     bool isDead;

     CObject() { isDead = false; }
     ~CObject() {}

     virtual void Load() {}
     virtual void Unload() {}

     // draw object
     void Draw(CCamera *camera) 
	 {
		 CObject * firstChild = this;
		 CObject * currChild  = this;
		 CObject * lastChild  = (CObject*)prevNode;

		 while(true)
		 {
			 glPushMatrix();
			 currChild->OnDraw(camera);          // draw this object
			 if (currChild->HasChild())          // draw children
				 ((CObject*)(currChild->childNode))->Draw(camera);
			 glPopMatrix();

			 if (currChild == lastChild)
			 {
				 break;
			 }
			 currChild = (CObject*)currChild->nextNode;
		 }

		 return;
		 

		 //while (currChild != firstChild)
		 //CObject * lastChild = (CObject*)(parentNode->childNode->prevNode);
		 //return;
		 //// push modelview matrix on stack
		 //glPushMatrix();
		 //OnDraw(camera);          // draw this object
		 //if (HasChild())          // draw children
			// ((CObject*)childNode)->Draw(camera);
		 //glPopMatrix();

		 //if (HasParent() && !IsLastChild())
		 //{
			// CObject * currChild = (CObject*)nextNode;
			// CObject * lastChild = (CObject*)(parentNode->childNode->prevNode);
			// do 
			// {
			//	 glPushMatrix();
			//	 currChild->OnDraw(camera);
			//	 if (currChild->HasChild())
			//	 {
			//		 ((CObject*)(currChild->childNode))->Draw(camera);
			//	 }
			//	 glPopMatrix();
			//	 if (currChild == lastChild)
			//	 {
			//		 break;
			//	 }
			//	 currChild = (CObject*)(currChild->nextNode);
			// } while(true);

		 //}
	 }

     // animate object
     void Animate(scalar_t deltaTime) 
     {
		 CNode * pNode = CNode::header;
		 CNode * pNext = NULL;
		 while (pNode != NULL)
		 {
			 pNext = pNode->next;
			 CObject * pObj = dynamic_cast<CObject *>(pNode);
			 if (pObj != NULL)
			 {
				 pObj->OnAnimate(deltaTime);
			 }
			 pNode = pNext;
		 }
		 return;

          //OnAnimate(deltaTime);         // animate this object
     
          //// animate children
          //if (HasChild())
          //     ((CObject*)childNode)->Animate(deltaTime);

          //// animate siblings
          //if (HasParent() && !IsLastChild())
          //     ((CObject*)nextNode)->Animate(deltaTime);

          //if (isDead)
          //     delete this;
     }

     // perform collision detection
     void ProcessCollisions(CObject *obj) 
     {
		 //goto good_lab;
		 CObject * header = dynamic_cast<CObject *>( CNode::header );
		 if (header == NULL)
		 {
			 return;
		 }
		 CObject * first = header;
		 CObject * second = header;

		 for (CObject * a = header; a != NULL; a = (CObject*)(a->next) )//a = dynamic_cast<CObject *>( a->next ))
		 {
			 //if (a == NULL)
				// return;
			 for (CObject * b = header; b != NULL; b = (CObject*)(b->next) )//dynamic_cast<CObject *>( b->next ))
			 {
				 //if (b == NULL)
					// return;
				 if ( ( a != b) && ((a->position - b->position).Length() <= (a->size + b->size)) )
				 {
					 a->OnCollision(b);
				 }
			 }
		 }
		 return;
//good_lab:
//
//          // if this object's bounding sphere collides with obj's sphere
//          // and obj is not this object
//		 //VTRACE("%s(%u) <==> %s(%u)\n", typeid(*this).name(), this, typeid(*obj).name(), obj);
//          if (((obj->position - position).Length() <= (obj->size + size)) &&
//               (obj != ((CObject*)this)))
//          {
//               OnCollision(obj);        // perform this object's collision with obj
//
//               // test child collisions with obj
//               //if (HasChild())
//               //     ((CObject*)childNode)->ProcessCollisions(obj);
//
//               //// test sibling collisions with obj
//               //if (HasParent() && !IsLastChild())
//               //     ((CObject*)nextNode)->ProcessCollisions(obj);
//          }
//		  // test child collisions with obj
//		  if (HasChild())
//			  ((CObject*)childNode)->ProcessCollisions(obj);
//
//		  // test sibling collisions with obj
//		  if (HasParent() && !IsLastChild())
//			  ((CObject*)nextNode)->ProcessCollisions(obj);
//
//          // if obj has children, check collisions with these children
//          if (obj->HasChild())
//               ProcessCollisions((CObject*)(obj->childNode));
//
//          // if obj has siblings, check collisions with these siblings
//          if (obj->HasParent() && !obj->IsLastChild())
//               ProcessCollisions((CObject*)(obj->nextNode));
     }

     // prepare object
     void Prepare()
     {
		 CNode * pNode = CNode::header;
		 while (pNode != NULL)
		 {
			 CObject * pObj = dynamic_cast<CObject *>(pNode);
			 if (pObj != NULL)
			 {
				 pObj->OnPrepare();
			 }
			 pNode = pNode->next;
		 }
		 return;

          OnPrepare();                            // prepare this object

          if (HasChild())                         // prepare children
               ((CObject*)childNode)->Prepare();

          if (HasParent() && !IsLastChild())      // prepare siblings
               ((CObject*)nextNode)->Prepare();
     }

     // find root object of cyclic linked list tree
     CObject *FindRoot()
     {
          // if this object has a parent node, return the root of the parent node
		 /*
          if (parentNode)
               return ((CObject*)parentNode)->FindRoot();
		  return this;
		  */
		  CNode * pTemp = this;

		  while (pTemp->parentNode != NULL)
		  {
			  pTemp = pTemp->parentNode;

		  }
		  return dynamic_cast<CObject*>( pTemp );

         
     }
};


#endif