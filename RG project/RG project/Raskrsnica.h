#ifndef RASKRSNICA_H
#define RASKRSNICA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

enum class SemaforStanje { CRVENO, ZUTO, ZELENO };

struct Auto {
    float pozicijaX;
    float pozicijaY;
    float brzina;
};

class Raskrsnica {
public:
    Raskrsnica();
    ~Raskrsnica();

    void draw();
    void drawSemafor();
    void drawAute();
    void drawTrake();
    void drawPravougaonik();
    void drawBrojAutomobila();

    SemaforStanje getStanjeSemafora() const { return stanjeSemafora; }
    void setStanjeSemafora(SemaforStanje novoStanje) { stanjeSemafora = novoStanje; }

    void promeniStanjeSemafora();
    void promeniStanjeSemafora(int index);
    void generisiAuto();
    bool klikNaAuto(float x, float y);

private:
    GLuint vaoTrake, vboTrake;
    GLuint vaoAuta, vboAuta;
    GLuint vaoSemafor, vboSemafor;
    GLuint vaoPravougaonik, vboPravougaonik;
    GLuint shaderProgram, shaderProgramTrake, shaderProgramTextura, shaderProgramAuto;
    GLuint autoTexture;
    GLuint textureRed, textureYellow, textureGreen;
    GLuint shaderProgramCountCars;
    GLuint vaoCountCars, vboCountCars;

    SemaforStanje stanjeSemafora;
    std::vector<Auto> automobili;
};

#endif // RASKRSNICA_H
