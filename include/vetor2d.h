#pragma once
#include <cmath>

struct Vetor2D {
    float x = 0.0f;
    float y = 0.0f;

    Vetor2D() = default;
    Vetor2D(float x, float y) : x(x), y(y) {}

    Vetor2D operator+(const Vetor2D& outro) const {
        return Vetor2D(x + outro.x, y + outro.y);
    }

    Vetor2D operator-(const Vetor2D& outro) const {
        return Vetor2D(x - outro.x, y - outro.y);
    }

    Vetor2D operator*(float escalar) const {
        return Vetor2D(x * escalar, y * escalar);
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    Vetor2D normalizar() {
        float mag = magnitude();
        if (mag > 0) {
            x /= mag;
            y /= mag;
        }
        return *this;
    }
};

inline float distancia(const Vetor2D& a, const Vetor2D& b) {
    return (a - b).magnitude();
}