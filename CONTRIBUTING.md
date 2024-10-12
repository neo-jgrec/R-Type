# Contributing to R-Type

Thank you for considering contributing to the R-Type project! We welcome all contributions that help improve the game, whether it's through bug fixes, new features, or documentation improvements.

Please take a moment to review this guide before submitting your contributions.

## Table of Contents

- [Getting Started](#getting-started)
- [Code of Conduct](#code-of-conduct)
- [How to Contribute](#how-to-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Feature Requests](#feature-requests)
  - [Submitting Code Changes](#submitting-code-changes)
- [Code Style Guidelines](#code-style-guidelines)
- [Development Workflow](#development-workflow)
- [License](#license)

---

## Getting Started

1. Fork the repository to your own GitHub account.
2. Clone your forked repository to your local machine:

   ```bash
   git clone https://github.com/neo-jgrec/R-Type.git
   ```

3. Set up the necessary build tools (CMake, package managers, etc.).
4. Create a new branch for your contribution:

   ```bash
   git checkout -b feature/my-new-feature
   ```

## Code of Conduct

We adhere to the [Contributor Covenant](https://www.contributor-covenant.org/) to maintain a welcoming and respectful environment. By participating in this project, you agree to follow our [Code of Conduct](CODE_OF_CONDUCT.md).

## How to Contribute

### Reporting Bugs

If you find a bug, please open an issue and include:

- A clear description of the bug and how it affects the game.
- Steps to reproduce the bug.
- If possible, provide screenshots or error logs.

### Feature Requests

If you have ideas for new features, submit an issue with:

- A detailed description of the feature and its expected behavior.
- How this feature will improve the game.
- Any potential impact on existing code.

### Submitting Code Changes

1. Make sure your changes adhere to the project's [Code Style Guidelines](#code-style-guidelines).
2. Ensure that all existing tests pass before submitting your pull request.
3. Write tests for any new functionality.
4. Commit your changes with clear and descriptive commit messages:

   ```bash
   git commit -m "Add feature XYZ: description of change"
   ```

5. Push your branch to your forked repository:

   ```bash
   git push origin feature/my-new-feature
   ```

6. Submit a pull request to the main repository's `develop` branch.

## Code Style Guidelines

- Use **C++17** standards.
- Follow standard object-oriented practices (SOLID principles, encapsulation, etc.).
- Use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to ensure consistent formatting across the project.
- Use meaningful variable and function names.
- Comment your code where necessary, explaining complex logic or algorithms.

## Development Workflow

We use Git for version control and follow a **feature-branch** workflow. Here’s a typical development workflow:

1. **Feature Branches**: For each new feature or bug fix, create a new branch from `develop`.
2. **Pull Requests**: Open a pull request (PR) from your feature branch into the `develop` branch.
3. **Reviews**: All code contributions will be reviewed by a maintainer before being merged.
4. **Continuous Integration**: Ensure the CI pipeline passes all tests before final review.

We use [GitHub Actions](https://github.com/features/actions) for automated testing and linting. Make sure your branch passes all checks before submitting your PR.

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
