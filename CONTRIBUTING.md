# Contributing to Mathematics Engine

Thank you for your interest in contributing to the Mathematics Engine! This document provides guidelines and instructions for contributing to this project.

## 🌟 How Can I Contribute?

### 1. Reporting Bugs

If you find a bug, please create an issue with:
- **Clear title** describing the problem
- **Steps to reproduce** the bug
- **Expected behavior** vs **actual behavior**
- **Screenshots** if applicable
- **System information** (OS, compiler version, SDL2 version)

**Example:**
```
Title: Division by zero crashes application in limit mode

Steps to reproduce:
1. Select Limits mode (option 4)
2. Enter expression: 1/x
3. Set limit point to 0
4. Press ENTER

Expected: Should show limit does not exist
Actual: Application crashes
```

### 2. Suggesting Features

Feature requests are welcome! Please:
- Check if the feature already exists or is in progress
- Describe the feature and its use case
- Explain why it would be valuable

**Feature request template:**
```markdown
## Feature Description
[Brief description]

## Use Case
[Why this feature is needed]

## Proposed Implementation
[Optional: How you think it could be implemented]
```

### 3. Contributing Code

We welcome pull requests! Please follow these guidelines:

#### Before You Start
1. **Fork the repository**
2. **Create a branch** for your feature/fix: `git checkout -b feature/your-feature-name`
3. **Check existing issues** to avoid duplicate work
4. **Discuss major changes** by creating an issue first

#### Code Standards

**C++ Style Guide:**
- Use **C++17** features
- Follow **camelCase** for variables and functions
- Follow **PascalCase** for classes and structs
- Use **meaningful names** (e.g., `calculateDerivative` not `calcDeriv`)
- Add **comments** for complex logic
- Keep functions **focused** and **single-purpose**

**Example:**
```cpp
// Good
std::unique_ptr<ASTNode> PartialDerivative::differentiateNode(const ASTNode* node) {
    // Clear logic with comments
    switch (node->type) {
        case NodeType::NUMBER:
            return std::make_unique<NumberNode>(0);
        // ...
    }
}

// Avoid
auto d(const ASTNode* n) { /* unclear implementation */ }
```

**File Organization:**
- **Headers (.h)** in appropriate subdirectories
- **Implementation (.cpp)** matching header names
- **Include guards** in all headers
- **Forward declarations** where possible

**Memory Management:**
- Use **std::unique_ptr** for ownership
- Use **const references** for read-only access
- Avoid raw `new`/`delete`

#### Project Structure

```
src/
├── engine/          # Mathematical engine components
│   ├── ast.h/.cpp              # AST definitions
│   ├── parser.h/.cpp           # Expression parser
│   ├── differentiator.h/.cpp   # Differentiation
│   ├── integrator.h/.cpp       # Integration
│   ├── partial_derivative.h/.cpp    # Partial derivatives
│   └── multivariate_integrator.h/.cpp # Double integration
└── ui/              # User interface components
    ├── renderer.h/.cpp         # SDL2/OpenGL rendering
    ├── text_renderer.h/.cpp    # Text rendering
    └── plotter.h/.cpp          # Function plotting
```

#### Building and Testing

**Build the project:**
```bash
cd Mathh/build
g++ -o math_engine \
    ../src/main.cpp \
    ../src/engine/*.cpp \
    ../src/ui/*.cpp \
    -I../src \
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lopengl32 -lglu32 \
    -std=c++17 -O2 -Wall
```

**Test your changes:**
- Run the application and test all modes
- Test edge cases (division by zero, limits at infinity, etc.)
- Verify no memory leaks (use valgrind on Linux or similar tools)
- Check for warnings (`-Wall` flag)

#### Commit Messages

Write clear, descriptive commit messages:

**Format:**
```
<type>: <short description>

<detailed description if needed>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `refactor`: Code refactoring
- `test`: Adding tests
- `style`: Code style changes

**Examples:**
```
feat: Add gradient calculation for multivariate functions

Implemented gradient vector computation for functions f(x,y).
Includes step-by-step solution display and LaTeX export support.

---

fix: Prevent division by zero in limit evaluation

Added check in evaluateLimit to return NaN when denominator is zero.
Fixes issue #42.
```

#### Pull Request Process

1. **Update documentation** if needed (README.md, code comments)
2. **Ensure clean compilation** with no warnings
3. **Test thoroughly** on Windows (MinGW)
4. **Create pull request** with:
   - Clear title and description
   - Reference to related issues
   - Screenshots/examples if applicable

**PR Template:**
```markdown
## Description
[What does this PR do?]

## Related Issues
Fixes #[issue number]

## Changes Made
- [Change 1]
- [Change 2]

## Testing Done
- [Test 1]
- [Test 2]

## Screenshots
[If applicable]
```

## 🎯 Areas for Contribution

### High Priority
- [ ] **Unit tests** for mathematical operations
- [ ] **Documentation** improvements
- [ ] **Bug fixes** in existing features
- [ ] **Performance optimizations**

### Medium Priority
- [ ] **New mathematical operations** (gradients, curl, divergence)
- [ ] **Improved UI/UX**
- [ ] **LaTeX rendering improvements**
- [ ] **Additional export formats** (PDF, MathML)

### Advanced Features
- [ ] **Triple integration**
- [ ] **Vector calculus operations**
- [ ] **Differential equations solver**
- [ ] **3D function plotting**
- [ ] **Symbolic equation solver**

## 🏗️ Architecture Overview

### AST (Abstract Syntax Tree)
The core data structure representing mathematical expressions:
- `NumberNode`: Constants
- `VariableNode`: Variables (x, y)
- `BinaryOpNode`: Operations (+, -, *, /, ^)
- `UnaryFuncNode`: Functions (sin, cos, ln, exp, sqrt)

### Parser
Converts string expressions to AST:
1. Tokenization
2. Shunting-yard algorithm
3. AST construction

### Calculus Engines
- **Differentiator**: Symbolic differentiation using calculus rules
- **Integrator**: Symbolic integration
- **PartialDerivative**: Partial derivatives ∂f/∂x, ∂f/∂y
- **MultivariateIntegrator**: Double integration ∫∫ f(x,y) dy dx
- **LimitCalculator**: Limits with L'Hôpital's rule

### UI Layer
- **Renderer**: SDL2/OpenGL initialization
- **TextRenderer**: UTF-8 text rendering with SDL2_ttf
- **Plotter**: Function graphing with OpenGL

## 📚 Resources

### Learning Materials
- [Abstract Syntax Trees](https://en.wikipedia.org/wiki/Abstract_syntax_tree)
- [Symbolic Differentiation](https://en.wikipedia.org/wiki/Symbolic_computation)
- [SDL2 Documentation](https://wiki.libsdl.org/)
- [OpenGL Tutorial](https://learnopengl.com/)

### Development Tools
- **Compiler**: MinGW-w64 GCC 15.2+
- **Libraries**: SDL2, SDL2_ttf, OpenGL
- **Build system**: GCC or CMake
- **Version control**: Git

## 🐛 Common Issues

### Compilation Errors
**Issue**: Cannot find SDL2 headers
**Solution**: 
```bash
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf
```

**Issue**: Undefined reference to OpenGL functions
**Solution**: Use `-lopengl32 -lglu32` (not `-lGL -lGLU`) on Windows

### Runtime Errors
**Issue**: Font not found
**Solution**: Ensure `assets/fonts/NotoSans-Regular.ttf` exists in working directory

**Issue**: DLL not found
**Solution**: Copy required DLLs from MinGW64/bin to build directory

## 💬 Communication

- **Issues**: For bug reports and feature requests
- **Pull Requests**: For code contributions
- **Discussions**: For general questions and ideas

## 📜 License

By contributing to this project, you agree that your contributions will be licensed under the MIT License.

## 🙏 Recognition

All contributors will be acknowledged in the project. Thank you for helping make Mathematics Engine better!

---

**Questions?** Feel free to open an issue or reach out to the maintainers.

Happy coding! 🚀
