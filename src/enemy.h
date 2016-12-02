#ifndef __ENEMY_H
#define __ENEMY_H

/*
	ENEMY.H

	The CEnemy class

	OpenGL Game Programming
	Author: Kevin Hawkins
	Date: 3/30/2001
	Description: The CEnemy class

*/

#include "entity.h"
#include "audiosystem.h"
#include "vector.h"

class CPlayer;
class CTerrain;

enum AIState_t
{
	AI_UNCARING,	// enemy is not scared and does not care
	AI_SCARED,	// enemy is scared and running away
	AI_DEAD
};

enum modelState_t
{
	MODEL_IDLE,		// idle animation
	MODEL_CROUCH,		// crouch animation
	MODEL_RUN,		// running animation
	MODEL_JUMP,		// jumping animation
	MODEL_FIRE,		// model shooting gun
	MODEL_DIE			// model dying
};

class CEnemy : public CEntity
{
private:

protected:
     int hitPoints;           // hit points the enemy has left
     float distFromPlayer;    // distance this enemy is from player
     float runSpeed;          // speed of enemy when running
     AIState_t aiState;       // state of enemy thought

	 modelState_t modelState;

     virtual void OnProcessAI() {}

public:
     CPlayer *player;

     CEnemy(CMD2Model * pmd2 = NULL) : CEntity(pmd2)
     {
          hitPoints = 100;    // start with 100 hit points
          isDead = false;     // enemy starts off alive
          velocity = CVector(0.0, 0.0, 0.0); // speed of enemy
          runSpeed = velocity.z;
          SetState(MODEL_IDLE);    // idle state
          direction = 0.0f;        // point north
          player = NULL;

		  modelState = MODEL_IDLE;
     }

     ~CEnemy() {}

     void ProcessAI() { OnProcessAI(); }
     void SetPlayer(CPlayer *p) { player = p; }
	 void OnPrepare()
	 {
		 /*
		 Frame#  Action
		 ----------------
		 0-39    idle
		 40-46   running
		 47-60   getting shot but not falling (back bending)
		 61-66   getting shot in shoulder
		 67-73   jumping
		 74-95   idle
		 96-112  getting shot and falling down
		 113-122 idle
		 123-135 idle
		 136-154 crouch
		 155-161 crouch crawl
		 162-169 crouch adjust weapon (idle)
		 170-177 kneeling dying
		 178-185 falling back dying
		 186-190 falling forward dying
		 191-198 falling back slow dying
		 */

		 switch (modelState)
		 {
		 case MODEL_IDLE:
			 stateStart = 0;
			 stateEnd = 39;
			 break;
		 case MODEL_CROUCH:
			 break;
		 case MODEL_RUN:
			 stateStart = 40;
			 stateEnd = 46;
			 velocity = CVector(0.0, 0.0, 15.0);
			 break;
		 case MODEL_JUMP:
			 stateStart = 67;
			 stateEnd = 73;
			 break;
		 case MODEL_DIE:
			 stateStart = 178;
			 stateEnd = 184;
			 //currentFrame = 178;
			 //interpol = 0.0f;
			 //isDead = true;
			 break;
		 default:
			 stateStart = 0;
			 stateEnd = 1;
			 break;
		 }
		 // TODO ¹Ç÷À¶¯»­¶ÔÏóÛs×²¼ì²â
		 // perform collision detection from this entity with all other objects in world

		 /*ProcessCollisions(FindRoot())*/;
	 }
	 // set animation state of model
	 void SetState(modelState_t state){modelState = state;}	

	 // retrieve animation state of model
	 modelState_t GetState(){return modelState;}
};

#endif