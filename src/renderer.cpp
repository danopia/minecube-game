#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "renderer.h"
#include "player.h"
#include "coord.h"

GLuint Texture = 0;

GLdouble norm = 1 / sqrtf( 3 );

GLdouble x = 0;
GLdouble y = 0;
GLdouble z = 0;
GLdouble radius = 1;
  GLdouble vertices[24] = { x - radius, y - radius, z + radius,
							x + radius, y - radius, z + radius,
							x + radius, y + radius, z + radius,
							x - radius, y + radius, z + radius,
							x - radius, y - radius, z - radius,
							x + radius, y - radius, z - radius,
							x + radius, y + radius, z - radius,
							x - radius, y + radius, z - radius };

  GLdouble normals[24] = { -norm, -norm,  norm,
						    norm, -norm,  norm,
						    norm,  norm,  norm,
						   -norm,  norm,  norm,
						   -norm, -norm, -norm,
						    norm, -norm, -norm,
						    norm,  norm, -norm,
						   -norm,  norm, -norm };

  GLdouble texcoords[24] = { 0.125, 0.0, 0.0,
						     0.0, 0.125, 0.0,
						     0.0, 0.0, 0.0,
						     0.125, 0.0, 0.0,
						     0.0, 0.125, 0.0,
						     0.0, 0.0, 0.0,
						     0.125, 0.0, 0.0,
						     0.0, 0.125, 0.0 };

  GLubyte indices[24] = { 7, 6, 5, 4,
						  5, 6, 2, 1,
						  4, 5, 1, 0,
						  1, 2, 3, 0,
						  3, 7, 4, 0,
						  6, 7, 3, 2 };

Renderer::Renderer(Terrain initterrain, Player* player) : terrain(initterrain), player(player) {
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 1.f); // black

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Face culling to render half the polys
    glEnable(GL_CULL_FACE);

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
}


// Don't forget to destroy our texture 
//glDeleteTextures(1, &Texture);

void drawCube(float x, float y, float z, float length) {
    float subsize = length / 2;

    // Apply some transformations
    glLoadIdentity();
    glTranslatef(x + subsize, y + subsize, z + subsize);
    glScalef(subsize, subsize, subsize);

    glDrawElements( GL_QUADS, 24, GL_UNSIGNED_BYTE, indices );
}

bool current;

void Renderer::renderNode(Octree<bool> terrain, float x, float y, float z, float size) {
    if (terrain.hasChildren) {
        float subsize = size / 2;
        renderNode(terrain.children[0], x,         y,         z,         subsize);
        renderNode(terrain.children[1], x+subsize, y,         z,         subsize);
        renderNode(terrain.children[2], x,         y+subsize, z,         subsize);
        renderNode(terrain.children[3], x+subsize, y+subsize, z,         subsize);
        renderNode(terrain.children[4], x,         y,         z+subsize, subsize);
        renderNode(terrain.children[5], x+subsize, y,         z+subsize, subsize);
        renderNode(terrain.children[6], x,         y+subsize, z+subsize, subsize);
        renderNode(terrain.children[7], x+subsize, y+subsize, z+subsize, subsize);
    } else if (terrain.value) {
        // Collision check against player
        if (player->X > x && player->Y > y && player->Z - 2.f > z
         && player->X < x + size && player->Y < y + size && player->Z - 2.f < z + size)
            current = true;
        
        drawCube(x, y, z, size);
    }
}

void Renderer::render() {

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Apply some transformations
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);
    glRotatef(-90 + player->Yrot, 1.f, 0.f, 0.f);
    glRotatef(player->Zrot, 0.f, 0.f, 1.f);
    glTranslatef(-player->X, -player->Y, -player->Z);    // Translate The Scene Based On Player Position

    glMatrixMode(GL_MODELVIEW);
    
    glBindTexture(GL_TEXTURE_2D, Texture);

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    glVertexPointer( 3, GL_DOUBLE, 0, &vertices[0] );
    glNormalPointer( GL_DOUBLE, 0, &normals[0] );
    glTexCoordPointer( 3, GL_DOUBLE, 0, &texcoords[0] );
    
    // Store current cube
    current = false;
    
    // Loop through chunks and render them
    int i, j, k;
    for(i = 0; i < terrain.sizeX; i++)
        for(j = 0; j < terrain.sizeY; j++)
            for(k = 0; k < terrain.sizeZ; k++)
                renderNode(terrain.GeneratedTerrain[Coord(i,j,k)], i*terrain.chunkSize, j*terrain.chunkSize, k*terrain.chunkSize, terrain.chunkSize);
    
    if (!current) {
        player->Z -= 0.1f;
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
