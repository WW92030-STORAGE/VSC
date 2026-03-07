#ifndef BOUNDINGVOLUMECREATION_VSC_PHYC
#define BOUNDINGVOLUMECREATION_VSC_PHYC

#include <string>
#include <vector>

#include "../../utils/Math.h"
#include "BoundingVolume.h"
#include "BoundingVolumeUtils.h"

// Actual creation methods

static BoundingAABB createBoundingAABB(std::vector<Vector3> points) {
    Vector3 inferior(points[0]);
    Vector3 superior(points[1]);

    Vector3 MARGIN(0.01, 0.01, 0.01);
    
    for (auto p : points) {
        inferior = min(p, inferior);
        superior = max(p, superior);
    }

    Vector3 center = (inferior + superior) * 0.5;

    return BoundingAABB(center, MARGIN + (superior - center));
}

static BoundingSphere createBoundingSphere(std::vector<Vector3> points) {
    if (points.size() <= 0) return BoundingSphere();
    if (points.size() == 1) return BoundingSphere(points[0], 1);
    Vector3 inferior[3] = {points[0], points[0], points[0]};
    Vector3 superior[3] = {points[0], points[1], points[2]};

    for (auto p : points) {
        for (int i = 0; i < 3; i++) {
            if (inferior[i].get(i) > p.get(i)) inferior[i] = p;
            if (superior[i].get(i) < p.get(i)) superior[i] = p;
        }
    }

    int index = 0;
    int maxd = 0;
    for (int i = 0; i < 3; i++) {
        float rsq = (superior[i] - inferior[i]).normsquared();
        if (rsq > maxd) {
            maxd = rsq;
            index = i;
        }
    }

    BoundingSphere res = fromDiameter(inferior[index], superior[index]);

    for (auto p : points) res = growSphere(res, p);

    return res;
}


static BoundingSphere mergeBoundingSphere(BoundingSphere a, BoundingSphere b) {
    Vector3 disp = (b.position - a.position);
    Vector3 disp_n = disp.normalized();

    Vector3 pa = a.position - disp_n * a.radius;
    Vector3 pb = b.position + disp_n * b.radius;

    Vector3 center = (pa + pb) * 0.5;
    float rrrrr = (pb - center).length();

    return BoundingSphere(center, rrrrr);
}

static BoundingAABB mergeBoundingAABB(BoundingAABB a, BoundingAABB b) {
    Vector3 inferior = min(a.position - a.halfrad, b.position - b.halfrad);
    Vector3 superior = min(a.position + a.halfrad, b.position + b.halfrad);

    Vector3 center = (inferior + superior) * 0.5;
    Vector3 hr = superior - center;
    return BoundingAABB(center, hr);
}

#endif