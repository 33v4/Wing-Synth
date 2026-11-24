# Wing Synth

Wing Synth is a Windows x64 VST3 MIDI instrument made with C++ and JUCE 8. It creates a polyphonic, vowel-shaped singing tone in real time. Load it as an instrument in FL Studio and play it from a MIDI keyboard, piano roll, or MIDI clip.

## Get the Windows plugin without installing developer tools

1. Create a **private** repository on GitHub.
2. Upload all files in this project, including the hidden `.github` folder. The easiest method is to upload the ZIP's extracted contents.
3. Open the repository's **Actions** tab.
4. Push the files to the repository default branch (`main` or `master`) to start **Build Wing Synth VST3** automatically. You can also open **Actions**, select the workflow, choose **Run workflow**, and run it on that branch. The first build downloads JUCE automatically.
5. Open the completed workflow run and download **Wing-Synth-VST3-Windows-x64** under Artifacts.
6. Extract the downloaded ZIP.
7. Copy the `Wing Synth.vst3` folder into `C:\Program Files\Common Files\VST3`.
8. Rescan plugins in FL Studio, then load **Wing Synth** as an instrument.

You do not need Visual Studio 2022 on your PC. You also do not need to install CMake, JUCE, a compiler, or other development tools. GitHub Actions supplies the build environment on its hosted Windows runner. GitHub keeps the artifact for the retention period configured for the repository; download it from the successful run before it expires.

## Controls

- **Vowel:** A, E, I, O, U formant shapes.
- **Formant:** shifts the vowel resonances for a brighter or darker vocal color.
- **Breath:** adds a soft noise component.
- **Warmth:** changes harmonic rolloff and resonance spread.
- **Vibrato:** adds pitch modulation.
- **Attack / Release:** shape the note envelope.
- **Output:** controls instrument level.

The instrument is polyphonic (12 voices), receives MIDI, and saves/restores its parameter state in the DAW. It is a synthesized vowel instrument, not a text-to-speech engine: it does not accept lyrics, phonemes, voicebanks, or recorded vocals. The sound is an original synthesis approximation inspired by a theatrical singing voice.

## Build details

CMake fetches JUCE 8.0.3 from its public GitHub repository using FetchContent. GitHub Actions builds the `WingSynth_VST3` target on a GitHub-hosted Windows 2022 runner using the Clang compiler and Ninja through CMake, then uploads a ZIP containing the compiled `.vst3` bundle and installation notes. The hosted runner supplies the Windows SDK and MSVC-compatible runtime needed by JUCE; no Visual Studio IDE or developer tools are installed on your PC.

If a GitHub Actions build fails, open the failed run, expand the first red step, and share the error text or screenshot in the Codex chat. The source project is ready to upload without editing code.

## Test report

- **Windows VST3 build:** Not run in this workspace. A Windows C++ compiler is not available here; the GitHub Actions workflow is the intended build and packages the downloadable VST3 if successful.
- **FL Studio load/host test:** Not run. This workspace cannot launch FL Studio.
- **DSP offline test:** A small independent PowerShell numerical smoke test is included at `tests/offline_dsp_smoke.ps1`. It checks finite output, non-silence, and level bounds for a reference implementation of the additive/formant math; it does not compile or execute the C++ plugin.
- The final confirmation of the actual plugin binary is the successful Actions run and an FL Studio scan/load test on Windows.
