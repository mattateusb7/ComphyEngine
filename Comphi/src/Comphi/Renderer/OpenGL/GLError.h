
#pragma once
#include <iostream>
#include <glad/glad.h>

bool _check_gl_error(const char *file, int line);
void _toggle_gl_debug(bool mode);
#define check_gl_error() _check_gl_error(__FILE__,__LINE__);
#define check_gl_error_off() _toggle_gl_debug(false);
#define check_gl_error_on() _toggle_gl_debug(true);
