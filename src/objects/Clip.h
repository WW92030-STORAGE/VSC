#ifndef CLIP_EXT
#define CLIP_EXT

// #include <iostream>



#include <utility>
#include <vector>
#include "../utils/Math.h"
#include "../utils/geometry/Primitives.h"
#include "Triangle.h"

// Split triangle only
std::pair<std::vector<Triangle3>, std::vector<Triangle3>> TriSplit(Triangle3 t, Plane splane) {
    std::vector<Triangle3> positive;
    std::vector<Triangle3> negative;

    Vector3 p[3];
    for (int i = 0; i < 3; i++) p[i] = (t.p[i]);
    bool vpos[3];
    for (int i = 0; i < 3; i++) vpos[i] = splane.distanceFrom(p[i]) > 0;
    int sum = 0;
    for (int i = 0; i < 3; i++) sum += vpos[i];

    if (sum == 3) {
        positive.push_back(Triangle3(t));
    } else if (sum == 0) {
        negative.push_back(Triangle3(t));
    } else if (sum == 2) {
        // Rotate the points so that points[0] is the odd one out
        for (int yourmom = 0; yourmom < 6; yourmom++) {
            if (!vpos[0]) break;
            bool temp = vpos[0];
            Vector3 tev(p[0]);
            p[0] = p[1];
            p[1] = p[2];
            p[2] = tev;
            vpos[0] = vpos[1];
            vpos[1] = vpos[2];
            vpos[2] = temp;
        }

        Line L1(p[0], p[1] - p[0]);
        Line L2(p[0], p[2] - p[0]);

        Vector3 i1 = splane.intersection(L1);
        Vector3 i2 = splane.intersection(L2);
        if (i1 == NILVEC3 || i2 == NILVEC3) positive.push_back(Triangle3(t));
        else {
            // p[0] and the two intersections form a triangle.
            negative.push_back(Triangle3(p[0], i1, i2));
            // i1, p[1], p[2], i2 form a quad in the same orientation as the original triangle
            // split along the shortest axis
            float i1p2 = (i1 - p[2]).normsquared();
            float p1i2 = (p[1] - i2).normsquared();

            if (i1p2 < p1i2) {
                positive.push_back(Triangle3(i1, p[1], p[2]));
                positive.push_back(Triangle3(p[2], i2, i1));
            } else {
                positive.push_back(Triangle3(p[1], p[2], i2));
                positive.push_back(Triangle3(i2, i1, p[1]));
            }
        }

        // for (int i = 0; i < 3; i++) std::cout << p[i].to_string() << " " << vpos[i] << "\n";
    } else if (sum == 1) {
        // Rotate the points so that points[0] is the odd one out
        for (int yourmom = 0; yourmom < 6; yourmom++) {
            if (vpos[0]) break;
            bool temp = vpos[0];
            Vector3 tev(p[0]);
            p[0] = p[1];
            p[1] = p[2];
            p[2] = tev;
            vpos[0] = vpos[1];
            vpos[1] = vpos[2];
            vpos[2] = temp;
        }

        Line L1(p[0], p[1] - p[0]);
        Line L2(p[0], p[2] - p[0]);

        Vector3 i1 = splane.intersection(L1);
        Vector3 i2 = splane.intersection(L2);
        if (i1 == NILVEC3 || i2 == NILVEC3) negative.push_back(Triangle3(t));
        else {
            // p[0] and the two intersections form a triangle.
            positive.push_back(Triangle3(p[0], i1, i2));
            // i1, p[1], p[2], i2 form a quad in the same orientation as the original triangle
            // split along the shortest axis
            float i1p2 = (i1 - p[2]).normsquared();
            float p1i2 = (p[1] - i2).normsquared();

            if (i1p2 < p1i2) {
                negative.push_back(Triangle3(i1, p[1], p[2]));
                negative.push_back(Triangle3(p[2], i2, i1));
            } else {
                negative.push_back(Triangle3(p[1], p[2], i2));
                negative.push_back(Triangle3(i2, i1, p[1]));
            }
        }
    }


    return {positive, negative};
}

// Split triangle attributes using barycentric interpolation
// The input is the attribute values at the triangle vertices.
template <typename T>
std::pair<std::vector<std::vector<T>>, std::vector<std::vector<T>>> TriSplitAttr(Triangle3 t, Plane splane, std::vector<T> attr) {
    std::vector<std::vector<T>> positive;
    std::vector<std::vector<T>> negative;


    Vector3 p[3];
    for (int i = 0; i < 3; i++) p[i] = (t.p[i]);
    bool vpos[3];
    for (int i = 0; i < 3; i++) vpos[i] = splane.distanceFrom(p[i]) > 0;
    int sum = 0;
    for (int i = 0; i < 3; i++) sum += vpos[i];

    if (sum == 3) {
        positive.push_back(attr);
    } else if (sum == 0) {
        negative.push_back(attr);
    } else if (sum == 2) {
        // Rotate the points so that points[0] is the odd one out
        for (int yourmom = 0; yourmom < 6; yourmom++) {
            if (!vpos[0]) break;
            bool temp = vpos[0];
            Vector3 tev(p[0]);
            p[0] = p[1];
            p[1] = p[2];
            p[2] = tev;
            vpos[0] = vpos[1];
            vpos[1] = vpos[2];
            vpos[2] = temp;

            T temt = attr[0];
            attr[0] = attr[1];
            attr[1] = attr[2];
            attr[2] = temt;
        }

        Line L1(p[0], p[1] - p[0]);
        Line L2(p[0], p[2] - p[0]);

        Vector3 i1 = splane.intersection(L1);
        Vector3 i2 = splane.intersection(L2);

        if (i1 == NILVEC3 || i2 == NILVEC3) positive.push_back(attr);
        else if (BASE::fzero((p[1] - p[0]).length()) || BASE::fzero((p[2] - p[0]).length())) positive.push_back(attr);
        else {
            // Calculate the interpolated values
            float rel1 = (i1 - p[0]).length() / (p[1] - p[0]).length();
            float rel2 = (i2 - p[0]).length() / (p[2] - p[0]).length();

            T t1 = lerp<T>(attr[0], attr[1], rel1);
            T t2 = lerp<T>(attr[0], attr[2], rel2);

            // p[0] and the two intersections form a triangle.
            // negative.push_back(Triangle3(p[0], i1, i2));
            negative.push_back(std::vector<T>({attr[0], t1, t2}));

            // i1, p[1], p[2], i2 form a quad in the same orientation as the original triangle
            // split along the shortest axis
            float i1p2 = (i1 - p[2]).normsquared();
            float p1i2 = (p[1] - i2).normsquared();

            /*
            if (i1p2 < p1i2) {
                positive.push_back(Triangle3(i1, p[1], p[2]));
                positive.push_back(Triangle3(p[2], i2, i1));
            } else {
                positive.push_back(Triangle3(p[1], p[2], i2));
                positive.push_back(Triangle3(i2, i1, p[1]));
            }
            */
            if (i1p2 < p1i2) {
                positive.push_back(std::vector<T>({t1, attr[1], attr[2]}));
                positive.push_back(std::vector<T>({attr[2], t2, t1}));
            } else {
                positive.push_back(std::vector<T>({attr[1], attr[2], t2}));
                positive.push_back(std::vector<T>({t2, t1, attr[1]}));
            }
        }

        // for (int i = 0; i < 3; i++) std::cout << p[i].to_string() << " " << vpos[i] << "\n";
    } else if (sum == 1) {
        // Rotate the points so that points[0] is the odd one out
        for (int yourmom = 0; yourmom < 6; yourmom++) {
            if (vpos[0]) break;
            bool temp = vpos[0];
            Vector3 tev(p[0]);
            p[0] = p[1];
            p[1] = p[2];
            p[2] = tev;
            vpos[0] = vpos[1];
            vpos[1] = vpos[2];
            vpos[2] = temp;

            T temt = attr[0];
            attr[0] = attr[1];
            attr[1] = attr[2];
            attr[2] = temt;
        }

        Line L1(p[0], p[1] - p[0]);
        Line L2(p[0], p[2] - p[0]);

        Vector3 i1 = splane.intersection(L1);
        Vector3 i2 = splane.intersection(L2);
        if (i1 == NILVEC3 || i2 == NILVEC3) negative.push_back(attr);
        else if (BASE::fzero((p[1] - p[0]).length()) || BASE::fzero((p[2] - p[0]).length())) negative.push_back(attr);
        else {
            // Calculate the interpolated values
            float rel1 = (i1 - p[0]).length() / (p[1] - p[0]).length();
            float rel2 = (i2 - p[0]).length() / (p[2] - p[0]).length();

            T t1 = lerp<T>(attr[0], attr[1], rel1);
            T t2 = lerp<T>(attr[0], attr[2], rel2);

            // p[0] and the two intersections form a triangle.
            // positive.push_back(Triangle3(p[0], i1, i2));
            positive.push_back(std::vector<T>({attr[0], t1, t2}));

            // i1, p[1], p[2], i2 form a quad in the same orientation as the original triangle
            // split along the shortest axis
            float i1p2 = (i1 - p[2]).normsquared();
            float p1i2 = (p[1] - i2).normsquared();

            /*
            if (i1p2 < p1i2) {
                negative.push_back(Triangle3(i1, p[1], p[2]));
                negative.push_back(Triangle3(p[2], i2, i1));
            } else {
                negative.push_back(Triangle3(p[1], p[2], i2));
                negative.push_back(Triangle3(i2, i1, p[1]));
            }
            */
            if (i1p2 < p1i2) {
                negative.push_back(std::vector<T>({t1, attr[1], attr[2]}));
                negative.push_back(std::vector<T>({attr[2], t2, t1}));
            } else {
                negative.push_back(std::vector<T>({attr[1], attr[2], t2}));
                negative.push_back(std::vector<T>({t2, t1, attr[1]}));
            }
        }
    }


    return {positive, negative};
}

#endif