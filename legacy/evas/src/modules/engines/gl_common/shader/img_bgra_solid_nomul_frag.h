"#ifdef GL_ES\n"
"precision mediump float;\n"
"#endif\n"
"uniform sampler2D tex;\n"
"varying vec2 tex_c;\n"
"varying vec2 tex_c2;\n"
"void main()\n"
"{\n"
"   vec4 t = texture2D(tex, tex_c.xy);\n"
"   gl_FragColor = vec4(t.r, t.g, t.b, t.a + tex_c2.x);\n"
"}\n"
