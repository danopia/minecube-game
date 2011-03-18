#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "client/renderer.h"
#include "common/player.h"
#include "common/vector3.h"

GLuint Texture = 0;

GLdouble vertices[96] = {
    -1,  1, -1,
     1,  1, -1,
     1, -1, -1,
    -1, -1, -1,

     1, -1, -1,
     1,  1, -1,
     1,  1,  1,
     1, -1,  1,

    -1, -1, -1,
     1, -1, -1,
     1, -1,  1,
    -1, -1,  1,

     1, -1,  1,
     1,  1,  1,
    -1,  1,  1,
    -1, -1,  1,

    -1,  1,  1,
    -1,  1, -1,
    -1, -1, -1,
    -1, -1,  1,

     1,  1, -1,
    -1,  1, -1,
    -1,  1,  1,
     1,  1,  1,
};

// For grass
GLdouble texcoords[64] = {
    // Bottom
    0.125, 0.375,
    0.0,   0.375,
    0.0,   0.25,
    0.125, 0.25,

    // Back
    0.125, 0.25,
    0.0,   0.25,
    0.0,   0.125,
    0.125, 0.125,

    // Right
    0.125, 0.25,
    0.0,   0.25,
    0.0,   0.125,
    0.125, 0.125,

    // Top
    0.0,   0.0,
    0.125, 0.0,
    0.125, 0.125,
    0.0,   0.125,

    // Front
    0.125, 0.125,
    0.125, 0.25,
    0.0,   0.25,
    0.0,   0.125,

    // Left
    0.125, 0.25,
    0.0,   0.25,
    0.0,   0.125,
    0.125, 0.125,
};


// For stone
GLdouble texcoords2[64] = {
    // Bottom
    0.125, 0.5,
    0.0,   0.5,
    0.0,   0.375,
    0.125, 0.375,

    // Back
    0.125, 0.375,
    0.0,   0.375,
    0.0,   0.5,
    0.125, 0.5,

    // Right
    0.125, 0.375,
    0.0,   0.375,
    0.0,   0.5,
    0.125, 0.5,

    // Top
    0.0,   0.375,
    0.125, 0.375,
    0.125, 0.5,
    0.0,   0.5,

    // Front
    0.125, 0.375,
    0.125, 0.5,
    0.0,   0.5,
    0.0,   0.375,

    // Left
    0.125, 0.5,
    0.0,   0.5,
    0.0,   0.375,
    0.125, 0.375,
};

Renderer::Renderer(Context *context): context(context) {
    InitGraphics();
}

void Renderer::InitGraphics() {
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 1.f); // black

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Face culling to render half the polys
    //glEnable(GL_CULL_FACE);

    // Lighting
    GLfloat LightAmbient[]  = { 0.5f, 0.5f, 0.5f, 1.0f }; // Ambient Light Values
    GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };	// Diffuse Light Values
    GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };	// Light Position
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
    glEnable(GL_LIGHT1);							// Enable Light One
    
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // Let glcolor work with lighting
    //glEnable(GL_COLOR_MATERIAL); // Enable lighting colors
    //glEnable(GL_LIGHTING); // Enable lighting
    glEnable(GL_TEXTURE_2D); // Enable textures
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Color textures
    
    // Uncomment for freaky graphics
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    //glEnable(GL_BLEND);
    
    glShadeModel(GL_SMOOTH); // Enable Smooth Shading
    
    // Load texture
    sf::Image Image;
    if (!Image.LoadFromFile("data/tiles.png"))
        return;
    
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Image.GetWidth(), Image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, Image.GetPixelsPtr());
    
    Font.LoadFromFile("data/Inconsolata.ttf", 20);
}

// Don't forget to destroy our texture 
//glDeleteTextures(1, &Texture);

void Renderer::drawCube(Block *block, float x, float y, float z, float length) {
    float subsize = length / 2;

    // Apply some transformations
    glLoadIdentity();
    glScalef(10, 10, 10);
    glTranslatef(x + subsize, y + subsize, z + subsize);
    glScalef(subsize, subsize, subsize);
    
    if (block->Type == 1) // Stone
        glTexCoordPointer(2, GL_DOUBLE, 0, &texcoords2[0]);
    else if (block->Type == 3) // Grass
        glTexCoordPointer(2, GL_DOUBLE, 0, &texcoords[0]);
    else
        printf("Type %i\n", block->Type);

    /*
    if (block->faces & 0x01 > 0 && player->Pos.Z < z) glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, &indices[0] );
    if (block->faces & 0x02 > 0 && player->Pos.X > x) glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, &indices[4] );
    if (block->faces & 0x04 > 0 && player->Pos.Y < y) glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, &indices[8] );
    if (block->faces & 0x08 > 0 && player->Pos.Z > z) glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, &indices[12] );
    if (block->faces & 0x10 > 0 && player->Pos.X < x) glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, &indices[16] );
    if (block->faces & 0x20 > 0 && player->Pos.Y > y) glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, &indices[20] );
    */
    if (context->player->Pos.Z + 1.5f < z) glDrawArrays(GL_QUADS, 0, 4);
    if (context->player->Pos.X + 0.5f > x) glDrawArrays(GL_QUADS, 4, 4);
    if (context->player->Pos.Y - 0.5f < y) glDrawArrays(GL_QUADS, 8, 4);
    if (context->player->Pos.Z + 1.5f > z) glDrawArrays(GL_QUADS, 12, 4);
    if (context->player->Pos.X - 0.5f < x) glDrawArrays(GL_QUADS, 16, 4);
    if (context->player->Pos.Y + 0.5f > y) glDrawArrays(GL_QUADS, 20, 4);
}

void Renderer::renderBlock(PositionedBlock *block) {
    drawCube(block->block, block->pos.X, block->pos.Y, block->pos.Z, block->sideLength);
    
    if (block->marked) {
      glColor3f(0, 0, 0);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      
      drawCube(block->block, block->pos.X, block->pos.Y, block->pos.Z, block->sideLength);
      
      glColor3f(255, 255, 255);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      block->marked = false;
    }
}

void Renderer::render() {
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Apply some transformations
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);
    glRotatef(-90 + context->player->Rotation.Y, 1.f, 0.f, 0.f);
    glRotatef(context->player->Rotation.Z, 0.f, 0.f, 1.f);
    glTranslatef((-context->player->Pos.X - 0.5f) * 10, (-context->player->Pos.Y - 0.5f) * 10, (-context->player->Pos.Z - 1.5f) * 10);    // Translate The Scene Based On Player Position

    glMatrixMode(GL_MODELVIEW);
    
    glBindTexture(GL_TEXTURE_2D, Texture);

    glEnableClientState( GL_VERTEX_ARRAY );
    //glEnableClientState( GL_NORMAL_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    glVertexPointer( 3, GL_DOUBLE, 0, &vertices[0] );
    //glNormalPointer( GL_DOUBLE, 0, &vertices[0] );
    glTexCoordPointer(2, GL_DOUBLE, 0, &texcoords[0]);
    
    // Loop through blocks and render them
    //for (int i = 0; i < context->world->Blocks.size(); i++)
    //    renderBlock(context->world->Blocks[i]);
    for (std::list<PositionedBlock*>::iterator it = context->world->VisibleBlocks.begin(); it != context->world->VisibleBlocks.end(); ++it)
        renderBlock(*it);
    
    Block *block = new StoneBlock();
    for (std::map<int, Player*>::iterator player = context->socket->Players.begin(); player != context->socket->Players.end(); player++) {
        if (player->first != context->socket->Number) {
            drawCube(block, player->second->Pos.X, player->second->Pos.Y, player->second->Pos.Z, 2.f);
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
