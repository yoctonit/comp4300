#ifndef VEC2_H
#define VEC2_H


class vec2 {
public:
    float x = 0;
    float y = 0;

    static float epsilon;
    static const vec2 kZero;

    vec2();

    vec2(float xin, float yin);


    vec2 operator*(float val) const;

    vec2 operator/(float val) const;

    vec2 operator+=(vec2 rhs);

    vec2 operator-=(vec2 rhs);

    vec2 operator*=(float val);

    vec2 operator/=(float val);


    bool operator==(const vec2 &rhs) const;

    bool operator!=(const vec2 &rhs) const;

    [[nodiscard]] float length() const;

    [[nodiscard]] float length_squared() const;

    vec2 normalize();
};

class point2 {
public:
    float x = 0;
    float y = 0;

    static float epsilon;
    static const point2 kOrigin;

    point2();

    point2(float xin, float yin);

    bool operator==(const point2 &rhs) const;

    bool operator!=(const point2 &rhs) const;

    [[nodiscard]] float distance(point2 p) const;

    [[nodiscard]] float distance_squared(point2 p) const;
};

vec2 operator+(vec2 lhs, vec2 rhs);

point2 operator+(point2 lhs, vec2 rhs);

vec2 operator-(vec2 lhs, vec2 rhs);

vec2 operator-(point2 lhs, point2 rhs);

vec2 operator*(float val, vec2 lhs);

bool equals(vec2 lhs, vec2 rhs);

bool equals(point2 lhs, point2 rhs);

float length(vec2 a);

float length_squared(vec2 a);

vec2 normalize(vec2 a);

float dot(vec2 lhs, vec2 rhs);

float distance(point2 lhs, point2 rhs);

float distance_squared(point2 lhs, point2 rhs);

float angle_radians(vec2 lhs, vec2 rhs);

float angle_degrees(vec2 lhs, vec2 rhs);

bool clockwise(vec2 lhs, vec2 rhs);

bool counterclockwise(vec2 lhs, vec2 rhs);

#endif // VEC2_H
