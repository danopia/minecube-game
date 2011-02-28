#include "common/ray.h"

Ray::Ray(Entity *entity) {
    Origin = entity->Pos + Vector3(0.5f, 0.5f, 1.5f);
    
    Direction = Vector3(
        sin((PI * entity->Rotation.Z) / 180),
        cos((PI * entity->Rotation.Z) / 180),
       -tan((PI * entity->Rotation.Y) / 180));
    
    if (Direction.X == 0.f) Direction.X = 0.0001f;
    if (Direction.Y == 0.f) Direction.Y = 0.0001f;
    if (Direction.Z == 0.f) Direction.Z = 0.0001f;
}

bool CheckBlockContact(PositionedBlock *block, Vector3 &collision) {
    return (collision.X >= block->pos.X && collision.X <= block->pos.X + block->sideLength
         && collision.Y >= block->pos.Y && collision.Y <= block->pos.Y + block->sideLength
         && collision.Z >= block->pos.Z && collision.Z <= block->pos.Z + block->sideLength);
}

float Ray::CheckCollision(PositionedBlock *block) {
    float x = 0.f, y = 0.f, z = 0.f;
    
    Vector3 planes1, planes2, collision;
    planes1 = (block->pos - Origin) / Direction;
    planes2 = (block->pos + block->sideLength - Origin) / Direction;
    
    if (planes1.X != 0.f && Origin.X < block->pos.X)
        collision = (Direction * planes1.X) + Origin;
    else if (planes2.X != 0.f)
        collision = (Direction * planes2.X) + Origin;
    else
        collision = Vector3();
        
    if (CheckBlockContact(block, collision))
        x = Origin.distance(collision);
    else
        x = 0;
        
    
    if (planes1.Y != 0.f && Origin.Y < block->pos.Y)
        collision = (Direction * planes1.Y) + Origin;
    else if (planes2.Y != 0.f)
        collision = (Direction * planes2.Y) + Origin;
    else
        collision = Vector3();
        
    if (CheckBlockContact(block, collision))
        y = Origin.distance(collision);
    else
        y = 0;
        
    
    if (planes1.Z != 0.f && Origin.Z < block->pos.Z)
        collision = (Direction * planes1.Z) + Origin;
    else if (planes2.Z != 0.f)
        collision = (Direction * planes2.Z) + Origin;
    else
        collision = Vector3();
        
    if (CheckBlockContact(block, collision))
        z = Origin.distance(collision);
    else
        z = 0;
    
    
    float dist = x;
    if ((dist == 0.f || y < dist) && y > 0.f) dist = y;
    if ((dist == 0.f || z < dist) && z > 0.f) dist = z;
    
    return dist;
}
