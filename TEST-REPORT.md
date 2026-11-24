# Wing Synth test report

Date: 2026-11-24

## Results

- **Offline DSP reference smoke test:** PASS. `tests/offline_dsp_smoke.ps1` evaluated 256 samples for each of the five vowel formant sets at 48 kHz. The result was finite, non-silent, and below the configured peak and RMS ceilings for every vowel.
- **C++/JUCE compile:** NOT RUN. This environment does not have a C++ compiler installed, and this package intentionally avoids requiring local development tools.
- **Windows x64 VST3 build:** NOT RUN. The provided GitHub Actions workflow is set up to perform this build with Clang and Ninja on a GitHub-hosted Windows runner; it must be run after the project is uploaded to GitHub.
- **FL Studio scan/playback:** NOT RUN. A successful CI build will confirm packaging, but actual DAW compatibility still needs a scan and MIDI playback check in FL Studio.

The smoke test runs a small independent numerical version of the additive formant calculation. It is useful for catching obvious numeric/level errors, but it is not a substitute for rendering the C++ plugin or listening to it.
