#ifndef VEC2_H
#define VEC2_H


class Vec2 {
public:
    float x = 0;
    float y = 0;

    Vec2();

    Vec2(float xin, float yin);

    bool operator==(const Vec2 &rhs) const;

    bool operator!=(const Vec2 &rhs) const;

    Vec2 operator+(const Vec2 &rhs) const;

    Vec2 operator-(const Vec2 &rhs) const;

    Vec2 operator*(float val) const;

    Vec2 operator/(float val) const;

    Vec2 operator+=(const Vec2 &rhs) const;

    Vec2 operator-=(const Vec2 &rhs) const;

    Vec2 operator*=(float val) const;

    Vec2 operator/=(float val) const;

    [[nodiscard]] float dist(const Vec2 &v) const;

    [[nodiscard]] float length() const;
    // void normalize()
};

#endif // VEC2_H
