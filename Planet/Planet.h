#ifndef GAME_PLANET_H
#define GAME_PLANET_H

#include <string>
#include <map>

using namespace std;

class Planet {
public:

//    float *vertices = nullptr;
//    float *textureCoordinates = nullptr;
//    float *normals = nullptr; // Array for normals
    unsigned int textureID{};
    string planetName;
    int p = 200; // Number of grid columns.
    int q = 200; // Number of grid rows
    int R = 10;
    float angle = 0.0;

    float *vertices = new float[3 * (p + 1) * (q + 1)];
    float *textureCoordinates = new float[2 * (p + 1) * (q + 1)];
    float *normals = new float[3 * (p + 1) * (q + 1)]; // Allocate memory for normals

    map<string, float> planetScales = {
            {"sun", 1.0f},
            {"mercury", 4880.0 / 1392700.0},
            {"venus", 12104.0 / 1392700.0},
            {"earth", 12742.0 / 1392700.0},
            {"mars", 6779.0 / 1392700.0},
            {"jupiter", 139820.0 / 1392700.0},
            {"saturn", 116460.0 / 1392700.0},
            {"uranus", 50724.0 / 1392700.0},
            {"neptune", 49244.0 / 1392700.0},
            {"moon", 3474.0 / 1392700.0}
    };

    map<string, float> planetDistances = {
            {"sun", 0.0f},
            {"mercury", 57.9 * 5 / 100.0},
            {"venus", 108.2 * 5 / 100.0},
            {"earth", 149.6 * 5 / 100.0},
            {"mars", 227.9 * 5 / 100.0},
            {"jupiter", 778.5 * 5 / 100.0},
            {"saturn", 1427.0 * 5 / 100.0},
            {"uranus", 2871.0 * 5 / 100.0},
            {"neptune", 4497.1 * 5 / 100.0},
            {"moon", (149.6 + 0.384) * 5 / 100.0} // Distance from the Sun via Earth
    };

    Planet();

    void draw();

    void setup(void);

    void init();

    void static drawPlanets(Planet *planets, float angle);


private:
    void fillVertexArray(void);

    void fillTextureCoordArray(void);

    float f(int i, int j);

    float g(int i, int j);

    float h(int i, int j);

    void initTexture();

//    void loadTextures(std::string file, unsigned int t);


    void setMaterialProperties(float r, float g, float b);
};

#endif //GAME_PLANET_H
