#include "Renderer.h"
#include "Texture.h"
#include <GL/freeglut.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

static const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPosition;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 vTexCoord;

    uniform mat4 uModel;
    uniform mat4 uProjection;

    void main() {
        vTexCoord = aTexCoord;
        gl_Position = uProjection * uModel * vec4(aPosition, 0.0, 1.0);
    }
)";

static const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 vTexCoord;
    out vec4 fragColor;

    uniform sampler2D uTexture;

    void main() {
        fragColor = texture(uTexture, vTexCoord);
    }
)";

// ovo je sve GLSL boilerplate kod koji sam doslovno kopirao i malo doterao sa sajta learnopengl.com
// preciznije, ovo je statican konstantan niz karaktera koji je zapravo GLSL kod koji cu
// malo nize koristiti za shaderProgram tj za zapravo renderovanje stvari na ekranu

Renderer::Renderer(int sirinaProz, int visinaProz) {
    this->sirinaProz = sirinaProz;
    this->visinaProz = visinaProz;
    VAO = 0;
    VBO = 0;
    shaderProgram = 0;
    initShaders();
    initBuffers();
}

// konstruktor, podosta uobicajeno, trazi sirinu i visinu prozora koji ce se napraviti
// takodje dodeljuje vrednost nula VAO i VBO , tako ih definisemo i dajemo placeholder vrednost
// sve dok im ne ubacimo pravi output GLSL koda kasnije, takodje automatski pozivamo initShaders
// i initBuffers metode pri kreiranju Renderer klase i tako inicijalizujemo oba shadera

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

// uobicajen destruktor,brisemo VAO i VBO kao i shader program, ne bi valjalo da to ostane u memoriji

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

// ova funkcija (clear) ce se pozivati na pocetku svakog frame-a kako se frame-ovi ne bi crtali jedni
// preko drugih i naravno kako bi mogli novi upravo izracunati frame-ovi da se iscrtaju na cistom ekranu
// GL COLOR BUFFER BIT je specifican GLbitfield tip koji je samo flag kakvu vrstu ciscenja ekrana trazimo
