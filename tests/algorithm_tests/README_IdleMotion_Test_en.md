# IdleMotion Test Documentation

## Test Overview

This test file verifies the correctness of the `IdleMotion` class, including unit tests and property-based tests.

## Test Tasks

- **Task 2.5**: IdleMotion Property Tests

## Validated Requirements

- **Requirements 13.1**: Idle state continuously generates small random fluctuations
- **Requirements 13.2**: Uses Perlin noise algorithm to generate natural fluctuations
- **Requirements 13.3**: Superimposed on control signal, producing slight floating and swaying
- **Requirements 13.4**: Fluctuation amplitude does not exceed 2 degrees, frequency does not exceed 0.1Hz

## Test Content

### Unit Tests (5 tests)

1. **test_unit_basic_functionality**: Basic functionality
   - Verify getNoise() returns valid floating-point numbers
   - Ensure no NaN or Inf values

2. **test_unit_amplitude_constraint**: Amplitude constraints
   - Verify noise values are within [-amplitude, +amplitude] range
   - Sample 50 points for verification

3. **test_unit_set_parameters**: Parameter setting
   - Verify setAmplitude() and setFrequency() functionality
   - Ensure normal operation after parameter modification

4. **test_unit_continuity_adjacent**: Continuity (adjacent moments)
   - Verify value changes at adjacent moments (10ms interval) are small
   - Difference should be less than 20% of amplitude

5. **test_unit_frequency_effect**: Frequency effect
   - Verify higher frequency produces faster changes
   - Compare change rates between low and high frequencies

### Property Tests (4 tests, 100 iterations each)

1. **test_property_continuity**: Continuity
   - **Property 4**: Idle motion continuity
   - For any parameters, difference between consecutive moments < amplitude * 0.1

2. **test_property_amplitude_bounds**: Amplitude constraints
   - **Property 4**: Idle motion continuity
   - For any parameters, all noise values within [-amplitude, +amplitude] range

3. **test_property_smoothness**: Smoothness
   - **Property 4**: Idle motion continuity
   - For any parameters, continuous sampling without abrupt changes (change < amplitude * 0.2)

4. **test_property_parameter_change_stability**: Parameter modification stability
   - **Property 4**: Idle motion continuity
   - After parameter modification, still satisfies continuity and amplitude constraints

## Running Tests

### Method 1: PlatformIO Command Line

```bash
# Run IdleMotion test
pio test -f test_idle_motion

# View detailed output
pio test -f test_idle_motion -v
```

### Method 2: VSCode Interface

1. Open PlatformIO sidebar
2. Click "Test" icon
3. Select "test_idle_motion"
4. Click run button

## Expected Output

```
========================================
IdleMotion Unit Tests
========================================
test_unit_basic_functionality:PASS
test_unit_amplitude_constraint:PASS
test_unit_set_parameters:PASS
test_unit_continuity_adjacent:PASS
test_unit_frequency_effect:PASS

========================================
IdleMotion Property Tests
Feature: moss-education-module
Property 4: Idle motion continuity
========================================

[Property Test] Continuity - 100 iterations
  Completed 10/100 iterations
  Completed 20/100 iterations
  ...
  Completed 100/100 iterations
test_property_continuity:PASS

[Property Test] Amplitude constraints - 100 iterations
  Completed 10/100 iterations
  ...
  Completed 100/100 iterations
test_property_amplitude_bounds:PASS

[Property Test] Smoothness - 100 iterations
  Completed 10/100 iterations
  ...
  Completed 100/100 iterations
test_property_smoothness:PASS

[Property Test] Parameter modification stability - 100 iterations
  Completed 10/100 iterations
  ...
  Completed 100/100 iterations
test_property_parameter_change_stability:PASS

-----------------------
9 Tests 0 Failures 0 Ignored
OK
```

## Test Principles

### Perlin Noise Characteristics

IdleMotion uses 1D Perlin noise algorithm with the following characteristics:
- **Continuity**: Smooth transitions between adjacent moments
- **Naturalness**: Similar to natural random fluctuations
- **Controllability**: Controlled through amplitude and frequency parameters

### Continuity Verification

Continuity is the core characteristic of idle motion, ensuring natural-looking movement:
- Difference between adjacent moments (10ms) < 10% amplitude
- Continuous sampling (20ms interval) change < 20% amplitude
- No abrupt changes or jumps

### Amplitude Constraints

Amplitude constraints ensure micro-movements are not excessive:
- All noise values within [-amplitude, +amplitude] range
- Complies with Requirements 13.4 (not exceeding 2 degrees)

## Notes

1. Property tests use pseudo-random number generator to ensure test repeatability
2. Each property test runs 100 iterations, covering a wide parameter space
3. Continuity tests use 10ms and 20ms intervals, simulating actual usage scenarios
4. Floating-point comparisons use tolerance to avoid precision issues

## Next Steps

After tests pass, continue with:
- **Task 2.7**: SoundLocalization Property Tests (waiting for I2S microphone arrival)
- **Task 3**: Checkpoint - Verify algorithm library
