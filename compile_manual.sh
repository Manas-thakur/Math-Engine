#!/bin/bash
# Manual compilation script for MathEngine
# Use this in MSYS2 MinGW64 terminal

cd "$(dirname "$0")/build"

echo "Compiling MathEngine with g++..."

g++ -std=c++17 \
    -I/mingw64/include \
    -I/mingw64/include/SDL2 \
    -I../src \
    ../src/main.cpp \
    ../src/engine/parser.cpp \
    ../src/engine/differentiator.cpp \
    ../src/engine/simplifier.cpp \
    ../src/engine/integrator.cpp \
    ../src/engine/limit_calculator.cpp \
    ../src/engine/matrix_operations.cpp \
    ../src/engine/latex_exporter.cpp \
    ../src/engine/partial_derivative.cpp \
    ../src/engine/multivariate_integrator.cpp \
    ../src/engine/implicit_differentiation.cpp \
    ../src/engine/linear_transformation.cpp \
    ../src/engine/taylor_series.cpp \
    ../src/engine/parametric_curve.cpp \
    ../src/engine/laplace_transform.cpp \
    ../src/engine/fourier_series.cpp \
    ../src/engine/differential_equations.cpp \
    ../src/engine/vector_calculus.cpp \
    ../src/engine/complex_numbers.cpp \
    ../src/engine/sequences_series.cpp \
    ../src/engine/numerical_methods.cpp \
    ../src/engine/eigenvalues.cpp \
    ../src/engine/statistics.cpp \
    ../src/ui/renderer.cpp \
    ../src/ui/text_renderer.cpp \
    ../src/ui/plotter.cpp \
    -L/mingw64/lib \
    -lmingw32 \
    -lSDL2main \
    -lSDL2 \
    -lSDL2_ttf \
    -lglew32 \
    -lopengl32 \
    -lfreetype \
    -lharfbuzz \
    -lgraphite2 \
    -lbz2 \
    -lz \
    -lpng \
    -lrpcrt4 \
    -mwindows \
    -o MathEngineUTF8.exe

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Compilation successful!"
    echo "Run with: ./MathEngineUTF8.exe"
else
    echo ""
    echo "✗ Compilation failed!"
    exit 1
fi
