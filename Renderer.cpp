#include "Renderer.h"
#include <GL/freeglut.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
using namespace glm;

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
void Renderer::drawSprite(Texture& textura,vec2 poz, vec2 vel) {
    glUseProgram(shaderProgram);

    mat4 model = mat4(1.0f);
    model = translate(model, vec3(poz, 0.0f));
    model = scale(model, vec3(vel, 0.0f));
    mat4 projekcija = ortho(0.0f,(float)visinaProz,(float)sirinaProz,0.0f,-1.0f,1.0f);
    //mogu ovo da optimizujem, ne mora da se svaki frejm pravi nova projection matrica, ODRADITI KASNIJE

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModel"), 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"), 1, GL_FALSE, value_ptr(projekcija));

    glActiveTexture(GL_TEXTURE0);
    textura.bind();
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

GLuint Renderer::compileShader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int radi;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &radi);
    if (!radi) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader se nije kompajlirao kako treba, nesto ne radi.. :(" << infoLog << std::endl;
    }
    return shader;
}

void Renderer::initShaders(){
    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int radi;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &radi);
    if (!radi) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Shader se nije linkovao kako treba, nesto ne radi.. :(" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer :: initBuffers() {
    float vertices[] = {
        0.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 0.0f,   1.0f, 0.0f,
        1.0f, 1.0f,   1.0f, 1.0f,

        0.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 1.0f,   1.0f, 1.0f,
        0.0f, 1.0f,   0.0f, 1.0f,
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Texture& Renderer::getUmetnuTeksturu() {
    if (umetnaTekstura.id == 0) {
        unsigned char white[] = { 255, 255, 255, 255 };
        glGenTextures(1, &umetnaTekstura.id);
        glBindTexture(GL_TEXTURE_2D, umetnaTekstura.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        umetnaTekstura.sirina = 1;
        umetnaTekstura.visina = 1;
        //OpenGL boilerplate za placeholder teksturu za probu, beli piksel, ako ne postoji napravi je
        //ako postoji ssamo je returnuje
    }
    return umetnaTekstura;
}
