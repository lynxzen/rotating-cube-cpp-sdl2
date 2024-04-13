#include "screen.h" 
#include <numeric>

struct vec3 {
    float x, y, z;
};

// represents a connection between two points, useful for creating lines
struct connection {
    int a,b;
};

// rotation function based off rotation matrix https://en.wikipedia.org/wiki/Rotation_matrix
void rotate(vec3& point, float x = 1, float y = 1, float z = 1) {
    float rad = 0;

    rad = x;
    point.y = std::cos(rad) * point.y - std::sin(rad) * point.z;
    point.z = std::sin(rad) * point.y + std::cos(rad) * point.z;

    rad = y;
    point.x = std::cos(rad) * point.x + std::sin(rad) * point.z;
    point.z = std::cos(rad) * point.z - std::sin(rad) * point.x;

    rad = z;
    point.x = std::cos(rad) * point.x - std::sin(rad) * point.y;
    point.y = std::sin(rad) * point.x + std::cos(rad) * point.y;
}

// line draw function, sends it to renderer
void line(Screen& screen, float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;

    float length = std::sqrt(dx * dx + dy * dy);
    float angle = std::atan2(dy, dx);

    for (float i = 0; i < length; i++) {
        screen.pixel(x1 + std::cos(angle) * i, y1 + std::sin(angle) * i);
    }
}

int main() {
    Screen screen;

    // the 8 corners of a cube
    std::vector<vec3> points {
        {100, 100, 100},
        {200, 100, 100},
        {200, 200, 100},
        {100, 200, 100},

        {100, 100, 200},
        {200, 100, 200},
        {200, 200, 200},
        {100, 200, 200}
    };

    // tells what points should be connected together, used for making the line later
    std::vector<connection> connections {
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},

        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},

        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4}

    };

    // centroid 
    vec3 c;

    // finding the centroid
    for (auto& p : points) {
        c.x += p.x;
        c.y += p.y;
        c.z += p.z;
    }
    c.x /= points.size();
    c.y /= points.size();
    c.z /= points.size();

    while(true) {
        // applying centroid to ensure correct rotation on cube's center
        for (auto& p : points) {
            p.x -= c.x;
            p.y -= c.y;
            p.z -= c.z;
            rotate(p, 0.02, 0.01, 0.04);
            p.x += c.x;
            p.y += c.y;
            p.z += c.z;
            screen.pixel(p.x, p.y);
        }
        
        // draw the lines between connections
        for (auto& conn: connections) {
            line(screen, points[conn.a].x, points[conn.a].y, points[conn.b].x, points[conn.b].y);
        }
        
        screen.show();
        screen.clear();
        screen.input();
        SDL_Delay(30);
    }
    return 0;
}
