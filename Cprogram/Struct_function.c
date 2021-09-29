#include <stdio.h>
#include <stdio.h>
#include <stdint.h>

typedef struct Point {
    double x, y;
}Point;

Point getMiddlePoint( Point a, Point b){
    Point m;
    m.x = ( a.x + b.x);
    m.y = ( a.y + b.y);
    return m;
}

int main( void ){
    Point p1 = {
        .x = 1, 
        .y = 1
    };

    Point p2 = {
        .x = 2,
        .y = 5
    };

    Point middle;
    middle = getMiddlePoint(p1, p2);
    printf("The resulf: %f, %f\n", middle.x, middle.y);

    return 0;
}


