#include "renderer.h"
#include "player.h"
#include <SFML/Graphics.hpp>

GLuint Texture = 0;

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
    
    sf::Image Image;
    if (!Image.LoadFromFile("data/tiles.png"))
        return;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Image.GetWidth(), Image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, Image.GetPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

// Don't forget to destroy our texture 
//glDeleteTextures(1, &Texture); 

void drawCube(float x, float y, float z, float length) {
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

    renderNode(terrain.GeneratedTerrain, 0.f, 0.f, 0.f, 50.f);
}
