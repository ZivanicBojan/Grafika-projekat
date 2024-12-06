#include "Raskrsnica.h"
#include "Shaders.h"
#include <cstdlib>   
#include <ctime>     
#include <GL/glew.h> 
#include <GLFW/glfw3.h> 


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint loadTexture(const char* texturePath) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data) {
        //std::cout << "Texture loaded: " << width << "x" << height << " with " << nrChannels << " channels." << std::endl;

        // Ako slika ima alpha kanal, koristi GL_RGBA
        if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        // Filtriranje teksture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    return textureID;
}


Raskrsnica::Raskrsnica() {
    // Inicijalizacija stanja semafora
    stanjeSemafora = SemaforStanje::CRVENO;

    srand(static_cast<unsigned int>(time(0)));

    shaderProgram = loadShaders("basic.vert", "basic.frag");
    shaderProgramTrake = loadShaders("trake.vert", "trake.frag");
    shaderProgramTextura = loadShaders("name.vert", "name.frag");
    shaderProgramAuto = loadShaders("auto.vert", "auto.frag");
    autoTexture = loadTexture("car.png");
    textureRed = loadTexture("red.png");
    textureYellow = loadTexture("yellow.png");
    textureGreen = loadTexture("green.png");
    shaderProgramCountCars = loadShaders("countCars.vert", "countCars.frag");


    // Kreiranje VBO i VAO objekata
    glGenVertexArrays(1, &vaoTrake);
    glGenBuffers(1, &vboTrake);

    glGenVertexArrays(1, &vaoAuta);
    glGenBuffers(1, &vboAuta);

    glGenVertexArrays(1, &vaoSemafor);
    glGenBuffers(1, &vboSemafor);

    glGenVertexArrays(1, &vaoPravougaonik);  
    glGenBuffers(1, &vboPravougaonik);  

    glGenVertexArrays(1, &vaoCountCars);  
    glGenBuffers(1, &vboCountCars); 


    GLfloat trakeVertices[] = {
        -0.3f, -1.0f,  
        -0.1f, -1.0f,  
        -0.3f,  1.0f,  
        -0.1f,  1.0f   
    };

    glBindVertexArray(vaoTrake);
    glBindBuffer(GL_ARRAY_BUFFER, vboTrake);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trakeVertices), trakeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    GLfloat semaforVertices[] = {
    // Pozicija           // Koordinate teksture
        0.4f,  0.8f,         0.0f, 0.0f,  
        0.6f,  0.8f,         1.0f, 0.0f,  
        0.4f,  0.7f,         0.0f, 1.0f,  
        0.6f,  0.7f,         1.0f, 1.0f   
    };



    glBindVertexArray(vaoSemafor);
    glBindBuffer(GL_ARRAY_BUFFER, vboSemafor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(semaforVertices), semaforVertices, GL_STATIC_DRAW);

    // Pozicija
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Koordinate teksture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    GLfloat autoVertices[] = {
        // Pozicija x, y       | Koordinate teksture s, t
        -0.05f,  0.05f,       0.0f, 1.0f,  
         0.05f,  0.05f,       1.0f, 1.0f,  
        -0.05f, -0.05f,       0.0f, 0.0f,  
         0.05f, -0.05f,       1.0f, 0.0f   
    };

    glBindVertexArray(vaoAuta);
    glBindBuffer(GL_ARRAY_BUFFER, vboAuta);
    glBufferData(GL_ARRAY_BUFFER, sizeof(autoVertices), autoVertices, GL_STATIC_DRAW);

    // Pozicija verteksa
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Koordinate teksture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    GLfloat pravougaonikVertices[] = {
        // Pozicija x, y     | Koordinate teksture s, t
        -1.0f,  0.8f,       0.0f,  1.0f,  
        -0.5f,  0.8f,       1.0f,  1.0f,  
        -1.0f,  1.0f,       0.0f,  0.0f,  
        -0.5f,  1.0f,       1.0f,  0.0f   
    };


    glBindVertexArray(vaoPravougaonik);
    glBindBuffer(GL_ARRAY_BUFFER, vboPravougaonik);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pravougaonikVertices), pravougaonikVertices, GL_STATIC_DRAW);

    // Postavljanje pozicije verteksa (x, y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Postavljanje koordinata teksture (s, t)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    // VBO i VAO za countCars
    GLfloat countCarsVertices[] = {
        // Pozicija x, y     | Koordinate teksture s, t
        0.65f, 0.9f,        0.0f,  0.0f,  
        0.75f, 0.9f,        1.0f,  0.0f,  
        0.65f, 0.7f,        0.0f,  1.0f,  
        0.75f, 0.7f,        1.0f,  1.0f   
    };

    glBindVertexArray(vaoCountCars);
    glBindBuffer(GL_ARRAY_BUFFER, vboCountCars);
    glBufferData(GL_ARRAY_BUFFER, sizeof(countCarsVertices), countCarsVertices, GL_STATIC_DRAW);

    // Postavljanje pozicije verteksa (x, y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Postavljanje koordinata teksture (s, t)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Raskrsnica::~Raskrsnica() {
    // Očistiti VAO i VBO resurse
    glDeleteVertexArrays(1, &vaoTrake);
    glDeleteBuffers(1, &vboTrake);
    glDeleteVertexArrays(1, &vaoAuta);
    glDeleteBuffers(1, &vboAuta);
    glDeleteVertexArrays(1, &vaoSemafor);
    glDeleteBuffers(1, &vboSemafor);
    glDeleteVertexArrays(1, &vaoPravougaonik);  
    glDeleteBuffers(1, &vboPravougaonik); 
    glDeleteVertexArrays(1, &vaoCountCars);  
    glDeleteBuffers(1, &vboCountCars);  
}

void Raskrsnica::promeniStanjeSemafora() {
    switch (stanjeSemafora) {
    case SemaforStanje::CRVENO:
        stanjeSemafora = SemaforStanje::ZELENO;
        break;
    case SemaforStanje::ZELENO:
        stanjeSemafora = SemaforStanje::ZUTO;
        break;
    case SemaforStanje::ZUTO:
        stanjeSemafora = SemaforStanje::CRVENO;
        break;
    }
}

void Raskrsnica::promeniStanjeSemafora(int svetloIndex) {
    if (svetloIndex == 0) {
        stanjeSemafora = SemaforStanje::CRVENO;
    }
    else if (svetloIndex == 1) {
        stanjeSemafora = SemaforStanje::ZUTO;
    }
    else if (svetloIndex == 2) {
        stanjeSemafora = SemaforStanje::ZELENO;
    }
}


void Raskrsnica::generisiAuto() {
    Auto noviAuto;

    // Nasumično odaberi traku (0 za lijevu, 1 za desnu)
    int odabranaTraka = rand() % 2;

    // Postavi horizontalnu poziciju na osnovu odabrane trake
    if (odabranaTraka == 0) {
        noviAuto.pozicijaX = -0.2f; // Lijeva traka
    }
    else {
        noviAuto.pozicijaX = 0.2f;  // Desna traka
    }

    // Postavi početnu vertikalnu poziciju ispod raskrsnice
    noviAuto.pozicijaY = -1.0f;

    // Nasumična brzina kretanja automobila
    noviAuto.brzina = static_cast<float>(rand() % 5 + 1) / 10.0f;

    // Dodaj novo vozilo u listu automobila
    automobili.push_back(noviAuto);
}


void Raskrsnica::draw() {
    drawTrake();
    drawSemafor();
    drawAute();
    drawPravougaonik();
    drawBrojAutomobila();
}

void Raskrsnica::drawSemafor() {
    float offsetY = 0.0f; // Početni vertikalni pomak za semafore

    glUseProgram(shaderProgram);

    // Teksture za crveno, žuto i zeleno svetlo
    GLuint textures[3];

    textures[0] = textureRed;    // Crvena tekstura
    textures[1] = textureYellow; // Žuta tekstura
    textures[2] = textureGreen;  // Zelena tekstura

    // Postavite offsetY za svaki semafor
    GLint offsetLocation = glGetUniformLocation(shaderProgram, "offsetY");

    for (int i = 0; i < 3; i++) {
        if (stanjeSemafora == SemaforStanje::CRVENO && i == 0) {
            glBindTexture(GL_TEXTURE_2D, textures[0]);  // Crvena tekstura
        }
        else if (stanjeSemafora == SemaforStanje::ZUTO && i == 1) {
            glBindTexture(GL_TEXTURE_2D, textures[1]);  // Žuta tekstura
        }
        else if (stanjeSemafora == SemaforStanje::ZELENO && i == 2) {
            glBindTexture(GL_TEXTURE_2D, textures[2]);  // Zelena tekstura
        }
        else {
            glBindTexture(GL_TEXTURE_2D, 0); // Ne primjenjuj teksturu
        }

        // Postavite vertikalni pomak za svaki semafor
        glUniform1f(offsetLocation, offsetY);

        // Nacrtajte semafor
        glBindVertexArray(vaoSemafor);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Crtanje pravougaonika sa teksturom
        offsetY -= 0.15f; // Pomak za sljedeći semafor
    }

    glUseProgram(0); // Isključivanje programa
}




void Raskrsnica::drawTrake() {
    float offsetX = 0.0f; // Početni pomak za prvu traku

    glUseProgram(shaderProgramTrake);

    for (int i = 0; i < 2; i++) {
        GLint offsetLocation = glGetUniformLocation(shaderProgramTrake, "offsetX");
        glUniform1f(offsetLocation, offsetX);

        glBindVertexArray(vaoTrake);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        offsetX += 0.4f; // Pomjeraj za sledeću traku
    }

    glUseProgram(0);
}


void Raskrsnica::drawAute() {
    float velicinaAuta = 0.1f;         // Visina automobila
    float minimalnaDistanca = 0.1f;    // Minimalna distanca između automobila

    // Ukloni automobile koji su izašli iz ekrana
    automobili.erase(
        std::remove_if(automobili.begin(), automobili.end(), [](const Auto& a) {
            return a.pozicijaY > 1.0f; // Granica ekrana
            }),
        automobili.end()
    );

    glUseProgram(shaderProgramAuto); // Koristi shader program za automobile

    // Postavljamo teksturu automobila
    glBindTexture(GL_TEXTURE_2D, autoTexture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (size_t i = 0; i < automobili.size(); ++i) {
        auto& a = automobili[i];

        bool blokiran = false; // Pretpostavka da automobil nije blokiran

        // Provjeri distancu do prethodnih automobila u istoj traci
        for (size_t j = 0; j < i; ++j) {
            auto& prethodni = automobili[j];

            // Da li su u istoj traci?
            if (std::fabs(prethodni.pozicijaX - a.pozicijaX) < 0.05f) {
                // Proveri razdaljinu
                if (prethodni.pozicijaY > a.pozicijaY &&
                    (prethodni.pozicijaY - a.pozicijaY) < (velicinaAuta + minimalnaDistanca)) {
                    // Postavi automobil iza prethodnog
                    a.pozicijaY = prethodni.pozicijaY - (velicinaAuta + minimalnaDistanca);
                    a.brzina = std::min(a.brzina, prethodni.brzina); // Postavi brzinu na brzinu prethodnog ako je manja
                    blokiran = true;
                    break;
                }
            }
        }

        // Ako nije blokiran, omogući mu da se kreće
        if (!blokiran) {
            if (stanjeSemafora != SemaforStanje::CRVENO || a.pozicijaY < 0.6f) {
                a.brzina = std::max(a.brzina, 0.15f); // Postavi početnu brzinu ako je bila 0
                a.pozicijaY += a.brzina * 0.01f;
            }
            if (stanjeSemafora == SemaforStanje::CRVENO && a.pozicijaY > 0.61f) {
                a.brzina = 0.15f;
                a.pozicijaY += a.brzina * 0.01f;
            }
        }

        // Postavi offset-e za X i Y poziciju u shaderu
        GLint offsetXLoc = glGetUniformLocation(shaderProgramAuto, "offsetX");
        GLint offsetYLoc = glGetUniformLocation(shaderProgramAuto, "offsetY");

        glUniform1f(offsetXLoc, a.pozicijaX);
        glUniform1f(offsetYLoc, a.pozicijaY);

        // Crtanje automobila
        glBindVertexArray(vaoAuta);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glBindVertexArray(0);
    glUseProgram(0); // Resetuj shader program

    glDisable(GL_BLEND);
}



bool Raskrsnica::klikNaAuto(float xpos, float ypos) {
    
    for (const Auto& a : automobili) {
        // Proveriti da li su koordinate kursora unutar opsega automobila
        if (xpos >= a.pozicijaX - 0.05f && xpos <= a.pozicijaX + 0.05f &&
            ypos >= a.pozicijaY - 0.05f && ypos <= a.pozicijaY + 0.05f) {
            // Kliknuli smo na automobil
            std::cout << "Kliknuto na auto! Pozicija: (" << a.pozicijaX << ", " << a.pozicijaY << "), Brzina: " << a.brzina << std::endl;
            return true;
        }
    }
    return false;
}

void Raskrsnica::drawPravougaonik() {
    glUseProgram(shaderProgramTextura); // Koristi shader za teksture

    GLuint texture = loadTexture("name.png"); // Putanja do teksture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Omogućavanje mešanja za prozirne teksture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(vaoPravougaonik);

    // Crtanje pravougaonika sa teksturom
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);

    // Isključivanje mešanja nakon crtanja
    glDisable(GL_BLEND);
}

GLuint loadDigitTexture(int digit) {
    // Učitaj teksture za brojeve 0-9
    // Svaki broj se nalazi u folderu 'digits', sa imenom broja (0.png, 1.png, ...)
    std::string texturePath = std::to_string(digit) + ".png";
    return loadTexture(texturePath.c_str());
}

void Raskrsnica::drawBrojAutomobila() {
    glUseProgram(shaderProgramCountCars);  // Koristite shader program za brojanje

    GLint offsetLocation = glGetUniformLocation(shaderProgramCountCars, "offsetX");
    GLuint textureLocation = glGetUniformLocation(shaderProgramCountCars, "textureSampler");

    float offsetX = 0.0f;  // Početni offset za X osu
    int carCount = automobili.size();  // Prikaz broja automobila

    std::string carCountStr = std::to_string(carCount);  // Pretvorite broj u string
    

    for (char digitChar : carCountStr) {
        int digit = digitChar - '0';  // Pretvorite karakter u broj
        GLuint digitTexture = loadDigitTexture(digit);
        if (digitTexture == 0) {
            std::cerr << "Error loading texture for digit." << std::endl;
            return;
        }
        

        glActiveTexture(GL_TEXTURE0);  // Aktivirajte teksturu na slotu 0
        glBindTexture(GL_TEXTURE_2D, digitTexture);  // Vežite teksturu

        glUniform1f(offsetLocation, offsetX);  // Pošaljite offset u shader
        glUniform1i(textureLocation, 0);  // Pošaljite teksturu u fragment shader

        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(vaoCountCars);  

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  // Crtanje
        glDisable(GL_BLEND);

        offsetX += 0.1f;  // Povećajte offset za sledeći broj
    }
}

