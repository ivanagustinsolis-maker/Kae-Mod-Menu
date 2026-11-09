# Input Adjuster

Starter project for a Geometry Dash 2.2081 / Geode Android64 mod.

## Current status

This is a build-verification scaffold only. It does not change gameplay input,
automate clicks, or claim to improve timing yet.

## Build

GitHub Actions builds the project for Android64 when you push to `main`,
or when you manually run the workflow from the Actions tab.

Download the `Input-Adjuster-Android64` artifact from the completed workflow.

## Next development step

Verify the exact Geode 5.11.2 bindings for GD 2.2081 before implementing
touch-event measurement and calibration. Do not add timestamp modification
until the relevant hook signatures and behavior have been confirmed.
