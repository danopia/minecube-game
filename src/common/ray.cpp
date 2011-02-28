#include "common/ray.h"

Ray::Ray(Entity *entity) {
    Origin = entity->Pos + Vector3(0.5, 0.5, 1.5);
    
    Direction = Vector3(
        sin((PI * entity->Rotation.Z) / 180),
        cos((PI * entity->Rotation.Z) / 180),
       -tan((PI * entity->Rotation.Y) / 180));
    
    if (Direction.X == 0.f) Direction.X = 0.0001f;
    if (Direction.Y == 0.f) Direction.Y = 0.0001f;
    if (Direction.Z == 0.f) Direction.Z = 0.0001f;
}

bool Ray::CheckCollision(PositionedBlock *block) {
    static float x, y, z;
    
    Vector3 planes;
    planes = (block->pos - Origin) / Direction;
    
    // x is fixed at block.x
    y = Origin.Y + Direction.Y * planes.X;
    z = Origin.Z + Direction.Z * planes.X;
    if (y >= block->pos.Y && y <= block->pos.Y + block->sideLength
     && z >= block->pos.Z && z <= block->pos.Z + block->sideLength)
        return true;
    
    // y is fixed at block.y
    x = Origin.X + Direction.X * planes.Y;
    z = Origin.Z + Direction.Z * planes.Y;
    if (x >= block->pos.X && x <= block->pos.X + block->sideLength
     && z >= block->pos.Z && z <= block->pos.Z + block->sideLength)
        return true;
    
    // z is fixed at block.z
    x = Origin.X + Direction.X * planes.Z;
    y = Origin.Y + Direction.Y * planes.Z;
    if (x >= block->pos.X && x <= block->pos.X + block->sideLength
     && y >= block->pos.Y && y <= block->pos.Y + block->sideLength)
        return true;
    
    
    planes = (block->pos + block->sideLength - Origin) / Direction;
    
    // x is fixed at block.x + block.size
    y = Origin.Y + Direction.Y * planes.X;
    z = Origin.Z + Direction.Z * planes.X;
    if (y >= block->pos.Y && y <= block->pos.Y + block->sideLength
     && z >= block->pos.Z && z <= block->pos.Z + block->sideLength)
        return true;
    
    // y is fixed at block.y + block.size
    x = Origin.X + Direction.X * planes.Y;
    z = Origin.Z + Direction.Z * planes.Y;
    if (x >= block->pos.X && x <= block->pos.X + block->sideLength
     && z >= block->pos.Z && z <= block->pos.Z + block->sideLength)
        return true;
    
    // z is fixed at block.z + block.size
    x = Origin.X + Direction.X * planes.Z;
    y = Origin.Y + Direction.Y * planes.Z;
    if (x >= block->pos.X && x <= block->pos.X + block->sideLength
     && y >= block->pos.Y && y <= block->pos.Y + block->sideLength)
        return true;
    
    return false;
}
