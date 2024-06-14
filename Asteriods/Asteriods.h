#ifndef GAME_ASTERIODS_H
#define GAME_ASTERIODS_H

#include <vector>
using namespace std;

class Asteriods {
public:
    explicit Asteriods(int count = 700);
    vector<float> points;
    vector<float> randX, randY, randZ;
    void draw();
    int num;
private:
    void init();

};

#endif //GAME_ASTERIODS_H