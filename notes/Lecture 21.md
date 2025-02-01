[COMP4300 - Game Programming - Lecture 21 - Intro to Shaders](https://youtu.be/C7J3C0iUzhM?si=7Xz6XMS8ifa9Vm6V)

Shader Resources
* [The Book of Shaders](https://thebookofshaders.com/)
* [Shader Toy](https://www.shadertoy.com/)

CPU vs GPU
* https://www.youtube.com/watch?v=-P28LKWTzrI

Shaders
* Shaders are programs run on the GPU
* Shaders are programmed as a series of instructions in GLSL (GL Shader Language)
* Shader programs are compiled by the GPU
* Shaders are executed in parallel on the GPU
* Two types: Vertex and Fragment Shaders
* shader/program creation illustration at 4:05

Vertex Shader
* Executed once for every vertex
* Not commonly used in 2D shaders
* Less impressive effects for 2D
* Let's skip them for now

Fragment Shader
* Executed once per pixel (frag) being drawn
* Executed in parallel by GPU, every pixel of the screen is calculated at the same time
* Given an input pixel position and color, output a color you want to replace it
* Inputs: fragCoord
* Outputs: fragColor

GLSL
* [Overview of GLSL](https://www.youtube.com/watch?v=uOErsQljpHs)
```
vec4 rgba = {r, g, b, a}
vec3 color = {r, g, b}
vec2 pos = {x, y}
vec3 gray(0.5)
vec4 rgba(color, a)
```

[Using Shaders in SFML](https://www.sfml-dev.org/tutorials/2.5/graphics-shader.php)
```
sf::Shader shader;
shader.loadFromFile("fragment_shader.frag", sf::Shader::Fragment);
shader.setUniform("myvar", 5.0f);
window.draw(whatever, &shader);

// Shader will be applied to whatever being drawn
```

[2d Shader Examples](https://www.youtube.com/watch?v=4zqEnhxBSCc)
