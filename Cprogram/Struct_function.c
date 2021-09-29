#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Point {
    double x, y;
}Point;


Point  getMiddlePoint( Point a, Point b);
Point  getMiddlePoint_1(Point *a, Point *b);
Point* getMiddlePoint_2(Point *a, Point *b);
Point* getMiddlePoint_3(Point *a, Point *b);
void getMiddlePoint_4(Point *a, Point *b, Point *out);


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
    printf("The resulf 1: %f, %f\n", middle.x, middle.y);
    middle = getMiddlePoint_1(&p1, &p2);
    printf("The resulf 2: %f, %f\n", middle.x, middle.y);
    middle = *getMiddlePoint_2(&p1, &p2);
    printf("The resulf 3: %f, %f\n", middle.x, middle.y);
    middle = *getMiddlePoint_3(&p1, &p2);
    printf("The resulf 4: %f, %f\n", middle.x, middle.y);
    Point* middle_1 = getMiddlePoint_3(&p1, &p2);
    printf("The resulf 5: %f, %f\n", middle_1->x, middle_1->y);
    free(middle_1);
    getMiddlePoint_4(&p1, &p2, &middle);
    printf("The resulf 6: %f, %f\n", middle.x, middle.y);

    return 0;
}

Point getMiddlePoint( Point a, Point b){
    Point m;
    m.x = ( a.x + b.x);
    m.y = ( a.y + b.y);
    return m;
}

Point getMiddlePoint_1(Point *a, Point *b){
    Point m;
    m.x = a->x + b->x;
    m.y = a->y + b->y;
    return m;
}

Point* getMiddlePoint_2(Point *a, Point *b){
    static Point m;
    m.x = a->x + b->x;
    m.y = a->y + b->y;
    return &m;
}

Point* getMiddlePoint_3(Point *a, Point *b){
    Point* m = malloc(sizeof(Point));
    m->x = a->x + b->x;
    m->y = a->x + b->y;
    return m;
}

void getMiddlePoint_4(Point *a, Point *b, Point *out){
    out->x = a->x + b->x;
    out->y = a->x + b->y;
}




