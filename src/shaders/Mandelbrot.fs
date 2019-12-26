#version 330

uniform int iterations = 100;

uniform float viewWidth;
uniform float viewHeight;

uniform float compWidth;
uniform float compHeight;

uniform float compCenterX;
uniform float compCenterY;

out vec4 fragColor;

// Assumes unit interval, based on cubic hermite splines.
// p0 = point at t = 0
// p1 = point at t = 1
// m0 = slope at t = 0
// m1 = slope at t = 1
float cubicInterp(float i, float p0, float p1, float m0, float m1)
{
    return (pow(i, 3) * (2 * p0 + m0 - 2 * p1 + m1)) +
           (i * i * (-3 * p0 - 2 * m0 + 3 * p1 - m1)) + (i * m0) + p0;
}

// The Mandelbrot set is the set of complex numbers c where
// fn(z) = fn-1(z)^2 + c does not diverge, f0(z) = c (i.e, iterate with z = 0).
// Using 2d image coordinates for c makes cool fractals.
void main()
{
    float x = (gl_FragCoord.x - viewWidth / 2) / viewWidth * compWidth;
    float y = (gl_FragCoord.y - viewHeight / 2) / viewHeight * compHeight;
    vec2 c = vec2(x, y) + vec2(compCenterX, compCenterY);

    // fi we will iterate with. Since initial z = 0, f0(z) = c;
    vec2 fi = c;
    int i;
    for (i = 1; i < iterations; i++) {
        // First test this iteration.
        float x = fi.x * fi.x - fi.y * fi.y + c.x;
        float y = 2 * fi.x * fi.y + c.y;

        // Apparently if the magnitude ever goes above 2 (or mag squard above
        // 4), then it will for sure be not in the set.
        if ((x * x + y * y) > 4.0)
            break;

        fi.x = x;
        fi.y = y;
    }

    float slider = float(i) / float(iterations);

    // Purely based on experimentation.
    fragColor = vec4(cubicInterp(slider, 0, 0, 1, -6),
                     cubicInterp(slider, 0, 0, 4, -3),
                     cubicInterp(slider, 0.1, 0, 6, 0),
                     1.0);
}
