# Contributing to Android Debloater

Thank you for your interest in contributing to [Android Debloater](https://github.com/KushagraKarira/Debloat)! Whether you are submitting package lists for new devices, fixing bugs, or improving documentation, your help is appreciated.

Please read our [Code of Conduct](https://github.com/KushagraKarira/Debloat/blob/master/CODE_OF_CONDUCT.md) before participating.

## Ways to Contribute

### 1. Share Device & Bloatware Package Lists

Expanding coverage across Android manufacturers and carriers is one of the fastest ways to help:

1. Connect your Android device via USB with **USB Debugging** enabled.
2. Run the following command to list installed packages:
   ```bash
   adb shell pm list packages
   ```
3. Open a new [Issue](https://github.com/KushagraKarira/Debloat/issues) with your **device model**, **Android/UI version**, and the **package list** (noting any known bloatware or adware packages), or submit a Pull Request updating [packages.ini](https://www.google.com/search?q=https://github.com/KushagraKarira/Debloat/blob/master/packages.ini) with appropriate risk levels (`Safe` or `Caution`).

### 2. Report Bugs or Request Features

- Check existing [Issues](https://github.com/KushagraKarira/Debloat/issues) first to avoid duplicates.
- Include your **Linux distribution**, **ADB version**, **Android device model**, and clear steps to reproduce the issue (or screenshots/output from the **Debug** tab).

### 3. Submit Code Changes (Pull Requests)

1. Fork the repository and create a new branch from `master`:
   ```bash
   git checkout -b fix/your-branch-name
   ```
2. Make your changes (for example, in [`app.c`](https://github.com/KushagraKarira/Debloat/blob/master/app.c) or [packages.ini](https://www.google.com/search?q=https://github.com/KushagraKarira/Debloat/blob/master/packages.ini)) and test building and running the app locally.
3. Commit your changes with a clear, descriptive message and open a Pull Request against `master`.

### 4. Documentation & Community Support

- Improve the user guide and setup instructions in [`README.md`](https://github.com/KushagraKarira/Debloat/blob/master/README.md) or help with the project website.
- Answer questions and help other users troubleshoot in [Issues](https://github.com/KushagraKarira/Debloat/issues) and [Discussions](https://github.com/KushagraKarira/Debloat/discussions).

## License

By contributing to this repository, you agree that your contributions will be licensed under the [GNU General Public License v3.0](https://github.com/KushagraKarira/Debloat/blob/master/LICENSE).
