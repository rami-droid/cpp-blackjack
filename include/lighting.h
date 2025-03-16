#include <raylib.h>

typedef struct lighting
{
    bool active;
    bool dirty;

    Vector2 position;
    float size;
    Color color;

    void draw() {
        DrawCircleGradient(position.x, position.y, size, color, {0, 0, 0, 0});
    }
} Light;


void lightMap() {

}