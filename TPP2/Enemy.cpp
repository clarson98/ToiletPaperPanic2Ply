
#include "Enemy.h"
#include "GameEngine.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>

Enemy::Enemy(){
    objType = ObjType::Enemy;
    enemyState = EnemyState::IDLE;
    moveSpeed = 5;
    damageGiven = 5;
};

GameEngine* game4 = GameEngine::GetInstance();

EnemyState Enemy::GetEnemyState(){return enemyState;}
int Enemy::GetMovementSpeed(){return moveSpeed;}
int Enemy::GetDamageGiven(){return damageGiven;}


/*
    state = one of the valid states that Enemy can have. Valid states (defined in header file) are IDLE, MOVE_LEFT, MOVE_RIGHT, JUMP, FALL
    (jump state can be set but it doesn't do anything yet in this build)
*/
void Enemy::SetEnemyState(EnemyState state){enemyState = state;}
void Enemy::SetMovementSpeed(int speed){moveSpeed = speed;}
void Enemy::SetDamageGiven(int d){damageGiven = d;}



//Updates Enemy's position and sprite animation frame depending on Enemy's state
void Enemy::Update(){
    int moveAnimYOffset = this->GetSprite()->GetSrcH(); //used to set sprite's src_rect to the next row of sprites in the sprite sheet
    int moveAnimTotalFrames = 4;
    int moveAnimStartFrame = 0; //which animation frame you want to start at

    int animSpeed = 200;

    int idleAnimTotalFrames = 4;
    int idleAnimStartFrame = 0;

    //link collider to Enemy pos
    this->SetBoxColliderPos(this->GetSprite()->GetX(), this->GetSprite()->GetY());

    switch(enemyState){
        case EnemyState::IDLE: {
            this->GetSprite()->SetSrcY(0);
            this->GetSprite()->UpdateFrame(animSpeed, idleAnimStartFrame, idleAnimTotalFrames);
            break;
        }

        case EnemyState::MOVE_LEFT: {
            this->GetSprite()->SetSrcY(moveAnimYOffset);
            this->GetSprite()->UpdateFrame(animSpeed, moveAnimStartFrame, moveAnimTotalFrames);

            this->MoveX(moveSpeed * -1);
	    
            this->GetSprite()->SetFlip(SDL_FLIP_HORIZONTAL);
            break;
        }

        case EnemyState::MOVE_RIGHT: {
            this->GetSprite()->SetSrcY(moveAnimYOffset);
            this->GetSprite()->UpdateFrame(animSpeed, moveAnimStartFrame, moveAnimTotalFrames);

            this->MoveX(moveSpeed);
	   
            this->GetSprite()->SetFlip(SDL_FLIP_NONE);
            break;
        }

    }

    //adhere to screen bounds
    if(this->GetSprite()->GetX() < 0){this->GetSprite()->SetX(0);}

    if(this->GetSprite()->GetX() > game4->getBgWidth()- this->GetSprite()->GetW()){//Background change
        this->GetSprite()->SetX(game4->getBgWidth() - this->GetSprite()->GetW());//Background change
    }

    return;
}

void Enemy::DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith){ 
    switch(objCollidedWith->GetType()){
        case ObjType::Player: {
            //cast to Player
            auto player = std::dynamic_pointer_cast<Player>(objCollidedWith);
            player->ChangeHealth(damageGiven * -1);
            //add some sort of buffer here since health drains too fast
            break;
        }

        default: {
            break;
        }
    }
}

void Enemy::SetIdle(){
    SetEnemyState(EnemyState::IDLE);
}

std::string Enemy::PrintObjType(){
    return "Enemy";
}