# Contributing to databento-cpp

We welcome contributions to databento-cpp! This document provides guidelines for contributing to the project.

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/YOUR-USERNAME/databento-cpp.git`
3. Create a new branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Build and test: `mkdir build && cd build && cmake .. && make && make test`
6. Commit your changes: `git commit -am 'Add some feature'`
7. Push to your fork: `git push origin feature/your-feature-name`
8. Create a Pull Request

## Development Guidelines

### Code Style

- Follow the existing code style
- Use C++17 features where appropriate
- Keep functions focused and single-purpose
- Add comments for complex logic
- Use meaningful variable and function names

### Building

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Testing

```bash
cd build
make test
# or
ctest -V
```

### Adding New Features

1. Update the appropriate header file in `include/databento/`
2. Implement the feature in the corresponding source file in `src/`
3. Add tests in `tests/`
4. Update documentation in `README.md`
5. Add examples if appropriate

### Commit Messages

- Use clear, descriptive commit messages
- Start with a verb in present tense (e.g., "Add feature", "Fix bug")
- Keep the first line under 72 characters
- Add detailed description if needed

### Pull Request Process

1. Ensure all tests pass
2. Update the README.md with details of changes if needed
3. Update the version numbers following [Semantic Versioning](https://semver.org/)
4. The PR will be merged once approved by maintainers

## Bug Reports

When reporting bugs, please include:

- Operating system and version
- Compiler and version
- Steps to reproduce the issue
- Expected behavior
- Actual behavior
- Any relevant logs or error messages

## Feature Requests

We welcome feature requests! Please:

- Check if the feature has already been requested
- Provide a clear description of the feature
- Explain why it would be useful
- Include examples if applicable

## Code of Conduct

- Be respectful and inclusive
- Welcome newcomers
- Focus on what is best for the community
- Show empathy towards other community members

## License

By contributing to databento-cpp, you agree that your contributions will be licensed under the MIT License.

## Questions?

Feel free to open an issue for any questions or concerns.

Thank you for contributing to databento-cpp!
