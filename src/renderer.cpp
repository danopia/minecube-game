	#include <GL/glew.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "renderer.h"
#include "player.h"
#include "coord.h"

GLuint Texture = 0;
    GLuint texture = 1;

  GLdouble norm = 1 / sqrt( 3 );

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

  GLdouble normals[24] = { -norm, -norm, norm,
						   norm, -norm, norm,
						   norm, norm, norm,
						   -norm, norm, norm,
						   -norm, -norm, -norm,
						   norm, -norm, -norm,
						   norm, norm, -norm,
						   -norm, norm, -norm };

  GLdouble texcoords[24] = { 1.0, 0.0, 0.0,
						     0.0, 1.0, 0.0,
						     0.0, 0.0, 0.0,
						     1.0, 0.0, 0.0,
						     0.0, 1.0, 0.0,
						     0.0, 0.0, 0.0,
						     1.0, 0.0, 0.0,
						     0.0, 1.0, 0.0 };

  /*GLubyte indices[24] = { 4, 5, 6, 7,
						  1, 2, 6, 5,
						  0, 1, 5, 4,
						  0, 3, 2, 1,
						  0, 4, 7, 3,
						  2, 3, 7, 6 };*/
  GLubyte indices[24] = { 7, 6, 5, 4,
						  5, 6, 2, 1,
						  4, 5, 1, 0,
						  1, 2, 3, 0,
						  3, 7, 4, 0,
						  6, 7, 3, 2 };

GLuint shader1 = 0;
GLuint shader2 = 0;
GLuint program = 0;

GLint getUniLoc(GLuint program, const GLchar *name)
{
    GLint loc;

    loc = glGetUniformLocation(program, name);

    if (loc == -1)
        printf("No such uniform named \"%s\"\n", name);

    return loc;
}

Renderer::Renderer(Terrain initterrain) : terrain(initterrain) {
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 1.f); // black

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Face culling to render half the polys
    glEnable(GL_CULL_FACE);
    
    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);

    // Lighting
    GLfloat LightAmbient[]  = { 0.5f, 0.5f, 0.5f, 1.0f }; // Ambient Light Values
    GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };	// Diffuse Light Values
    GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };	// Light Position
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
    glEnable(GL_LIGHT1);							// Enable Light One
    
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // Let glcolor work with lighting
    //glEnable(GL_COLOR_MATERIAL); // Enable lighting
    glEnable(GL_TEXTURE_2D); // Enable textures
    //glEnable(GL_LIGHTING); // Enable lighting
    
    glShadeModel(GL_SMOOTH); // Enable Smooth Shading
    
    SDL_Surface *image;
    if(!(image = IMG_Load("data/tiles.png")))
        return;
    /*glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Image.GetWidth(), Image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, Image.GetPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/
    
    

    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY,texture);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGBA,image->w/2, image->h/2, 4,0,GL_RGBA,GL_UNSIGNED_BYTE,image->pixels);
    SDL_FreeSurface(image);
//    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Image.GetWidth(), Image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, Image.GetPixelsPtr());


		glewInit();

		if (glewIsSupported("GL_VERSION_2_0"))
			printf("Ready for OpenGL 2.0\n");
		else {
			printf("OpenGL 2.0 not supported\n");
            exit(1);
		}
		
    std::ifstream in("data/brick.vert");
    std::string all;
    while(in.good()) {
        std::string line;
        getline(in, line);
        all += line + "\n";
    };

    shader1 = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* cstr = all.c_str();
    int len = all.length();
    glShaderSource(shader1, 1, &cstr, &len);
    glCompileShader(shader1);
		
    std::ifstream in2("data/brick.frag");
    std::string all2;
    while(in2.good()) {
        std::string line2;
        getline(in2, line2);
        all2 += line2 + "\n";
    };

    shader2 = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* cstr2 = all2.c_str();
    int len2 = all2.length();
    glShaderSource(shader2, 1, &cstr2, &len2);
    glCompileShader(shader2);
    
    program = glCreateProgram();
    glAttachShader(program, shader1);
    glAttachShader(program, shader2);
    glLinkProgram(program);
    glUseProgram(program);
}


// Don't forget to destroy our texture 
//glDeleteTextures(1, &Texture); 

void drawCube(float x, float y, float z, float length) {
    float sublength = length / 2;

	glPushMatrix(); // Preserve world matrix
	  
    // Apply some transformations
    glTranslatef(x + sublength, y + sublength, z + sublength);
    glScalef(sublength, sublength, sublength);

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnableClientState( GL_NORMAL_ARRAY );
  glEnableClientState( GL_TEXTURE_COORD_ARRAY );

  glVertexPointer( 3, GL_DOUBLE, 0, &vertices[0] );
  glNormalPointer( GL_DOUBLE, 0, &normals[0] );
  glTexCoordPointer( 3, GL_DOUBLE, 0, &texcoords[0] );

  glDrawElements( GL_QUADS, 24, GL_UNSIGNED_BYTE, indices );

  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    
    glPopMatrix(); // Undo the translations
}

void drawCube2(float x, float y, float z, float length) {
    float sublength = length / 2;

	glPushMatrix(); // Preserve world matrix
	  
    // Apply some transformations
    glTranslatef(x + sublength, y + sublength, z + sublength);
    glScalef(sublength, sublength, sublength);

    glBindTexture(GL_TEXTURE_2D, Texture);
    glBegin(GL_QUADS);
    
    // Top Face: Grass Top
    glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
    glTexCoord2f(0.0f,   0.0f  ); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 1 (Front)
    glTexCoord2f(0.125f, 0.0f  ); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 2 (Front)
    glTexCoord2f(0.125f, 0.125f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Front)
    glTexCoord2f(0.0f,   0.125f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 4 (Front)

    // Bottom Face: Dirt
    glNormal3f( 0.0f, 0.0f,-1.0f);					// Normal Pointing Away From Viewer
    glTexCoord2f(0.125f, 0.25f ); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Back)
    glTexCoord2f(0.125f, 0.375f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 2 (Back)
    glTexCoord2f(0.0f,   0.25f ); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 3 (Back)
    glTexCoord2f(0.0f,   0.375f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 4 (Back)

    // Front Face: Grass Side
    glNormal3f( 0.0f, 1.0f, 0.0f);					// Normal Pointing Up
    glTexCoord2f(0.0f,   0.25f ); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 1 (Top)
    glTexCoord2f(0.0f,   0.125f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 2 (Top)
    glTexCoord2f(0.125f, 0.125f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Top)
    glTexCoord2f(0.125f, 0.25f ); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 4 (Top)

    // Back Face: Grass Side
    glNormal3f( 0.0f,-1.0f, 0.0f);					// Normal Pointing Down
    glTexCoord2f(0.125f, 0.25f ); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Bottom)
    glTexCoord2f(0.0f,   0.25f ); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 2 (Bottom)
    glTexCoord2f(0.0f,   0.125f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 3 (Bottom)
    glTexCoord2f(0.125f, 0.125f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 4 (Bottom)

    // Left face: Grass Side
    glNormal3f( 1.0f, 0.0f, 0.0f);					// Normal Pointing Right
    glTexCoord2f(0.125f, 0.25f ); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 1 (Right)
    glTexCoord2f(0.0f,   0.25f ); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 2 (Right)
    glTexCoord2f(0.0f,   0.125f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Right)
    glTexCoord2f(0.125f, 0.125f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 4 (Right)

    // Right Face: Grass Side
    glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing Left
    glTexCoord2f(0.0f,   0.25f ); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Left)
    glTexCoord2f(0.0f,   0.125f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 2 (Left)
    glTexCoord2f(0.125f, 0.125f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 3 (Left)
    glTexCoord2f(0.125f, 0.25f ); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 4 (Left)
		
    glEnd(); // Done Drawing Quads
    glPopMatrix(); // Undo the translations
}

void renderNode(Octree<bool> terrain, float x, float y, float z, float size) {
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
        drawCube(x, y, z, size);
    }
}

void Renderer::render(Player player) {

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    
    // Apply some transformations
    glLoadIdentity();
    glRotatef(-90 + player.Yrot, 1.f, 0.f, 0.f); 
    glRotatef(player.Zrot, 0.f, 0.f, 1.f); 
    glTranslatef(-player.X, -player.Y, -player.Z);    // Translate The Scene Based On Player Position
    for(int i = 0; i < terrain.sizeX; i++)
    {
        for(int j = 0; j < terrain.sizeY; j++)
        {
            for(int k = 0; k < terrain.sizeZ; k++)
            {

                renderNode(terrain.GeneratedTerrain[Coord(i,j,k)], i*terrain.chunkSize, j*terrain.chunkSize, k*terrain.chunkSize, terrain.chunkSize);
            }
        }
    }
}
