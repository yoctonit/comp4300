#include "vec2.h"
#include <cmath>
#include <iostream>
#include <cassert>

void print(const std::string &s, vec2 v) {
    std::cout << s << "(" << v.x << ", " << v.y << ")\n";
}

void print(const std::string &s, point2 v) {
    std::cout << s << "(" << v.x << ", " << v.y << ")\n";
}

bool epsilon_equals(float a, float b, float epsilon = 0.001) {
    return std::abs(a - b) < epsilon;
}

int main() {
    print("Zero vector:  ", vec2::kZero);
    print("Origin point: ", point2::kOrigin);
    std::cout << "size(vec2):   " << sizeof(vec2) << "\n";
    std::cout << "size(point2): " << sizeof(point2) << "\n";
    assert(sizeof(vec2) == 2 * sizeof(float));
    assert(sizeof(point2) == 2 * sizeof(float));

    // test default constructor
    vec2 v1;
    assert(v1.x == 0.0f);
    assert(v1.y == 0.0f);

    // test initialization constructor
    vec2 v2(100, 100);
    assert(v2.x == 100.0f);
    assert(v2.y == 100.0f);

    // test member operator*
    vec2 v3 = v2 * 2.0f;
    assert(v3.x == 200.0f);
    assert(v3.y == 200.0f);

    // test non-member operator*
    v3 = 1.5f * v2;
    assert(v3.x == 150.0f);
    assert(v3.y == 150.0f);

    // test member operator/
    // non-member operator/ is not defined because
    // division of float with vec2 doesn't make sense
    v3 = v2 / 2.0f;
    assert(v3.x == 50.0f);
    assert(v3.y == 50.0f);

    // test member operator+=
    v2 += vec2{1.0f, 1.0f};
    assert(v2.x == 101.0f);
    assert(v2.y == 101.0f);

    // test member operator-=
    v2 -= vec2{2.0f, 2.0f};
    assert(v2.x == 99.0f);
    assert(v2.y == 99.0f);

    // test member operator*=
    v2 *= 2.0f;
    assert(v2.x == 198.0f);
    assert(v2.y == 198.0f);

    // test member operator/=
    v2 /= 3.0f;
    assert(v2.x == 66.0f);
    assert(v2.y == 66.0f);

    // test operator==
    v1 = v2;
    assert(v1 == v2);

    // test operator!=
    v1.x = 5.0f;
    assert(v1 != v2);

    // test length member functions
    v1.x = 1.0;
    v1.y = 2.0;
    assert(v1.length() == std::sqrt(5.0f));
    assert(v1.length_squared() == 5.0f);
    // test length non-member functions
    assert(length(v1) == std::sqrt(5.0f));
    assert(length_squared(v1) == 5.0f);

    // test normalize member function
    v1.normalize();
    assert(std::abs(v1.length() - 1.0f) < vec2::epsilon);
    print("v1 after normalization:  ", v1);
    std::cout << "v1.length(): " << v1.length() << "\n";

    // test default constructor
    point2 p1;
    assert(p1.x == 0.0f);
    assert(p1.y == 0.0f);

    // test initialization constructor
    point2 p2(100, 100);
    assert(p2.x == 100.0f);
    assert(p2.y == 100.0f);

    // test point2 operator-
    v2 = p2 - point2{10.0f, 10.0f};
    assert(v2.x == 90.0f);
    assert(v2.y == 90.0f);

    // test operator==
    p1 = p2;
    assert(p1 == p2);

    // test operator!=
    p1.x = 5.0f;
    assert(p1 != p2);

    // test distance member function
    p1.x = 1.0;
    p1.y = 1.0;
    p2.x = 2.0;
    p2.y = 3.0;
    assert(p1.distance(p2) == std::sqrt(5.0f));
    assert(p1.distance_squared(p2) == 5.0f);
    assert(p2.distance(p1) == std::sqrt(5.0f));
    assert(p2.distance_squared(p1) == 5.0f);
    assert(p2.distance(p2) == 0.0f);
    // test distance non-member function
    assert(distance(p1, p2) == std::sqrt(5.0f));
    assert(distance_squared(p1, p2) == 5.0f);
    assert(distance(p2, p1) == std::sqrt(5.0f));
    assert(distance_squared(p2, p1) == 5.0f);
    assert(distance(p2, p2) == 0.0);

    // test non-member operator+
    v1 = vec2{1.0f, 2.0f};
    v2 = vec2{3.0f, 4.0f};
    v3 = v1 + v2;
    assert(v3.x == 4.0f);
    assert(v3.y == 6.0f);

    // test non-member point2 operator+
    p1 = point2{1.0f, 2.0f};
    p2 = p1 + v3;
    assert(p2.x == 5.0f);
    assert(p2.y == 8.0f);

    // test vec2 non-member operator-
    v3 = v1 - v2;
    assert(v3.x == -2.0f);
    assert(v3.y == -2.0f);

    // test point2 non-member operator-
    v3 = p1 - p2;
    assert(v3.x == -4.0f);
    assert(v3.y == -6.0f);

    // test vec2 equals
    v1 = vec2{1.0f, 100.0f};
    v2 = vec2{1.001f, 100.004f};
    vec2::epsilon = 0.005;
    assert(equals(v1, v2));
    vec2::epsilon = 0.003;
    assert(!equals(v1, v2));

    // test point2 equals
    p1 = point2{1.0f, 100.0f};
    p2 = point2{1.001f, 100.004f};
    point2::epsilon = 0.005;
    assert(equals(p1, p2));
    point2::epsilon = 0.003;
    assert(!equals(p1, p2));

    // test non-member vec2 normalize
    v1 = vec2{3.0f, 4.0f};
    v2 = normalize(v1);
    assert(v1.x == 3.0f);
    assert(v1.y == 4.0f);
    assert(std::abs(v2.length() - 1.0f) < vec2::epsilon);
    print("v1: ", v1);
    print("v2: ", v2);

    // test dot product
    v1 = vec2{3.0f, 4.0f};
    v2 = vec2{5.0f, 6.0f};
    float d = dot(v1, v2);
    assert(d == 39.0f);

    // test angle_radians and angle_degrees function;
    v1 = vec2{3.0f, 0.0f};
    v2 = vec2{-2.0f, 0.0f};
    const float kPI = 3.14159265359f;
    float angle = angle_radians(v1, v2);
    float angle_deg = angle_degrees(v1, v2);
    std::cout << "angle:     " << angle << "\n";
    std::cout << "angle_deg: " << angle_deg << "\n";
    assert(epsilon_equals(angle, kPI));
    assert(epsilon_equals(angle_deg, 180.0f));

    v1 = vec2{0.0f, 3.0f};
    angle = angle_radians(v2, v1);
    angle_deg = angle_degrees(v1, v2);
    std::cout << "angle: " << angle << "\n";
    std::cout << "angle_deg: " << angle_deg << "\n";
    assert(epsilon_equals(angle, kPI / 2.0f));
    assert(epsilon_equals(angle_deg, 90.0f));

    v1 = vec2{-2.0f, -2.0f};
    v2 = vec2{0.0f, -3.0f};
    angle = angle_radians(v2, v1);
    angle_deg = angle_degrees(v1, v2);
    std::cout << "angle: " << angle << "\n";
    std::cout << "angle_deg: " << angle_deg << "\n";
    assert(epsilon_equals(angle, kPI / 4.0f));
    assert(epsilon_equals(angle_deg, 45.0f));

    v1 = vec2{-2.0f, 2.0f};
    v2 = vec2{0.0f, -3.0f};
    angle = angle_radians(v2, v1);
    angle_deg = angle_degrees(v1, v2);
    std::cout << "angle: " << angle << "\n";
    std::cout << "angle_deg: " << angle_deg << "\n";
    assert(epsilon_equals(angle, 3.0f * kPI / 4.0f));
    assert(epsilon_equals(angle_deg, 135.0f));

    angle = angle_radians(v1, v1);
    angle_deg = angle_degrees(v1, v1);
    std::cout << "angle: " << angle << "\n";
    std::cout << "angle_deg: " << angle_deg << "\n";
    assert(epsilon_equals(angle, 0.0f));
    assert(epsilon_equals(angle_deg, 0.0f));

    // test clockwise and counterclockwise function
    v1 = vec2{2.0f, 1.0f};
    v2 = vec2{1.0f, 3.0f};
    assert(!clockwise(v1, v2));
    assert(counterclockwise(v1, v2));
    assert(clockwise(v2, v1));
    assert(!counterclockwise(v2, v1));

    v1 = vec2{1.0f, 0.0f};
    v2 = vec2{-1.0f, 0.0f};
    assert(!clockwise(v1, v2));
    assert(counterclockwise(v1, v2));

    return 0;
}
