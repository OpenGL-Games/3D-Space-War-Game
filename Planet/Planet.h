#ifndef GAME_PLANET_H
#define GAME_PLANET_H

#include <string>
#include <map>

using namespace std;

class Planet {
public:

    float *vertices = nullptr;
    float *textureCoordinates = nullptr;
    unsigned int textureID{};
    string planetName;
    int p = 200; // Number of grid columns.
    int q = 200; // Number of grid rows
    int R = 10;
    float angle = 0.0;

    map<string, float> planetScales = {
            {"sun", 1392700.0 / 12742.0},
            {"mercury", 4880.0 / 12742.0},
            {"venus", 12104.0 / 12742.0},
            {"earth", 1.0f},
            {"mars", 6779.0 / 12742.0},
            {"jupiter", 139820.0 / 12742.0},
            {"saturn", 116460.0 / 12742.0},
            {"uranus", 50724.0 / 12742.0},
            {"neptune", 49244.0 / 12742.0},
            {"moon", 3474.0 / 12742.0}
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

    void static drawPlanets(Planet *planets, float angle);

private:
    void fillVertexArray(void);

    void fillTextureCoordArray(void);

    float f(int i, int j);

    float g(int i, int j);

    float h(int i, int j);

    void initTexture();
};

#endif //GAME_PLANET_H
