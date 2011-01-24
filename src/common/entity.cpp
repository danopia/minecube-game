#include "common/entity.h"

void Entity::DoStep(float amount) {
    if (LocatedOn) {
        GravitySpeed = 0;
    } else {
        GravitySpeed -= 20.f * amount;
        if (GravitySpeed > 25.f) GravitySpeed = 25.f;
        
        if (Pos.Z < 0) GravitySpeed = -GravitySpeed;
        Pos.Z += GravitySpeed * amount;
    }
}

void Entity::Collide(PositionedBlock& block) {
    if (!(LastPos.Z + Hitbox.Z > block.pos.Z && LastPos.Z < block.pos.Z + block.sideLength)) {
        if (Pos.Z + (Hitbox.Z / 2) < block.pos.Z + (block.sideLength / 2)) {
            Pos.Z = block.pos.Z - Hitbox.Z;
            
            if (GravitySpeed > 0.f)
                GravitySpeed = 0.f;
        } else {
            Pos.Z = block.pos.Z + block.sideLength;
            
            LocatedOn = &block;
            return; // or else you can't step between blocks
        }
    }
    
    if (!(LastPos.X + Hitbox.X > block.pos.X && LastPos.X < block.pos.X + block.sideLength)) {
        if (Pos.X + (Hitbox.X / 2) < block.pos.X + (block.sideLength / 2))
            Pos.X = block.pos.X - Hitbox.X;
        else
            Pos.X = block.pos.X + block.sideLength;
    }
    
    if (!(LastPos.Y + Hitbox.Y > block.pos.Y && LastPos.Y < block.pos.Y + block.sideLength)) {
        if (Pos.Y + (Hitbox.Y / 2) < block.pos.Y + (block.sideLength / 2))
            Pos.Y = block.pos.Y - Hitbox.Y;
        else
            Pos.Y = block.pos.Y + block.sideLength;
    }
}

