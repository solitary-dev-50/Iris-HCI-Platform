# MotionCurve Test Documentation

## Test Overview

This test file verifies the correctness of the `MotionCurve` class, including unit tests and property-based tests.

## Test Tasks

- **Task 2.2**: MotionCurve Property Tests
- **Task 2.3**: MotionCurve Unit Tests

## Validated Requirements

- **Requirements 12.1**: S-curve controls all motion
- **Requirements 12.2**: Real-time calculation, avoiding pre-generated large arrays
- **Requirements 12.3**: Target can be switched during motion
- **Requirements 12.4**: Includes five phases: start, acceleration, constant speed, deceleration, stop
- **Requirements 15.5**: Immediate response to new commands

## Test Content

### Unit Tests (5 tests)

1. **test_unit_basic_boundaries**: Basic boundary conditions
   - Verify start value (t=0)
   - Verify end value (t=duration)
   - Verify value after exceeding endpoint
   - Verify midpoint value is within reasonable range

2. **test_unit_negative_motion**: Negative motion
   - Verify motion from large to small values
   - Ensure negative motion also follows boundary conditions

3. **test_unit_target_switch**: Immediate target switch response
   - Verify ability to switch targets during motion
   - Ensure new target takes effect immediately

4. **test_unit_is_complete**: Completion detection
   - Verify correctness of `isComplete()` method
   - Test completion status at different moments

5. **test_unit_reset**: Reset functionality
   - Verify correctness of `reset()` method

### Property Tests (3 tests, 100 iterations each)

1. **test_property_boundaries**: Boundary condition correctness
   - **Property 1**: Real-time motion curve calculation correctness
   - For any start point, end point, and duration, verify boundary values are accurate

2. **test_property_intermediate_range**: Intermediate value range constraints
   - **Property 1**: Real-time motion curve calculation correctness
   - For any parameters, verify intermediate values are between start and end points

3. **test_property_monotonic**: Monotonicity
   - **Property 1**: Real-time motion curve calculation correctness
   - For any parameters, verify curve is monotonic (no backtracking)

## Running Tests

### Method 1: PlatformIO Command Line

```bash
# Run all tests
pio test

# Run specific test
pio test -f test_motion_curve

# View detailed output
pio test -v
```

### Method 2: VSCode Interface

1. Open PlatformIO sidebar
2. Click "Test" icon
3. Select "test_motion_curve"
4. Click run button

## Expected Output

```
========================================
MotionCurve Unit Tests
========================================
test_unit_basic_boundaries:PASS
test_unit_negative_motion:PASS
test_unit_target_switch:PASS
test_unit_is_complete:PASS
test_unit_reset:PASS

========================================
MotionCurve Property Tests
Feature: moss-education-module
Property 1: Real-time motion curve calculation correctness
========================================

[Property Test] Boundary conditions - 100 iterations
  Completed 10/100 iterations
  Completed 20/100 iterations
  ...
  Completed 100/100 iterations
test_property_boundaries:PASS

[Property Test] Intermediate range - 100 iterations
  Completed 10/100 iterations
  ...
  Completed 100/100 iterations
test_property_intermediate_range:PASS

[Property Test] Monotonicity - 100 iterations
  Completed 10/100 iterations
  ...
  Completed 100/100 iterations
test_property_monotonic:PASS

-----------------------
8 Tests 0 Failures 0 Ignored
OK
```

## Notes

1. Property tests use pseudo-random number generator to ensure test repeatability
2. Each property test runs 100 iterations, covering a wide input space
3. Tests use actual `millis()` time, so they need to run on hardware
4. Floating-point comparisons use tolerance (0.1) to avoid precision issues

## Next Steps

After tests pass, continue with:
- **Task 2.5**: IdleMotion Property Tests
- **Task 3**: Checkpoint - Verify algorithm library
