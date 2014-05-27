#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H
#include <vector>
#include <map>
#include <GLGraphics/GLHeader.h>
#include <CGLA/Vec3f.h>
#include <CGLA/Vec2f.h>
#include <CGLA/Mat3x3f.h>
#include <CGLA/Mat4x4f.h>


CGLA::Vec3f refract(const CGLA::Vec3f & in, const CGLA::Vec3f & n, float n1, float n2);

// These functions return the fresnel terms based on polarizations.
CGLA::Vec2f fresnelAmplitudeReflectance(const CGLA::Vec3f & in, const CGLA::Vec3f & n, float n1, float n2);
CGLA::Vec2f fresnelAmplitudeTransmittance(const CGLA::Vec3f & in, const CGLA::Vec3f & n, float n1, float n2);
CGLA::Vec2f fresnelPowerReflectance(const CGLA::Vec3f & in, const CGLA::Vec3f & n, float n1, float n2);
CGLA::Vec2f fresnelPowerTransmittance(const CGLA::Vec3f & in, const CGLA::Vec3f & n, float n1, float n2);

inline float fresnel_R(const CGLA::Vec3f & in, const CGLA::Vec3f & n, float n1, float n2)
{
    CGLA::Vec2f R = fresnelPowerReflectance(in,n,n1,n2);
    float RR = 0.5f * (R[0] + R[1]);
    return RR;
}

inline float fresnel_T(const CGLA::Vec3f & in, const CGLA::Vec3f & n, float n1, float n2)
{
    CGLA::Vec2f T = fresnelPowerTransmittance(in,n,n1,n2);
    float TT = 0.5f * (T[0] + T[1]);
    return TT;
}

void debugFresnelTerms();
float C_Sigma(float ni);
float C_e(float ni);
float C_1(float ni);
float C_2(float ni);

float clamp01(float x);
float clamp(float x, float min, float max);

void planeHammersley(std::vector<CGLA::Vec2f> & result, int n);
void planeHammersleyCircle(std::vector<CGLA::Vec2f> & result, int n);
void planeHammersleyCircleAlternative(std::vector<CGLA::Vec2f> & result, int n);
CGLA::Vec2f haltonPoint(int k, int p1, int p2);
CGLA::Vec2f haltonPointCircle(int k, int p1, int p2);
void planeHalton(std::vector<CGLA::Vec2f> & result, int n);
void planeHaltonCircle(std::vector<CGLA::Vec2f> & result, int n);
void planeHaltonCircleRejectionExponential(std::vector<CGLA::Vec2f> &result, int n, float sigma_tr);
void planeHaltonCircleRejectionExponentialMulti(std::vector<std::vector<CGLA::Vec2f> > & result, int n, int cols, float sigma_tr);


void planeHammersleyCircleMulti(std::vector<std::vector<CGLA::Vec2f> > & result, int n, int cols);
void planeHammersleyCircleMultiExp(std::vector<std::vector<CGLA::Vec2f> > & result, int n, int cols, float sigma);
void circleUniformPoints(std::vector<std::vector<CGLA::Vec2f> > &result, int n, int cols, int m);
#endif // MISCELLANEOUS_H
