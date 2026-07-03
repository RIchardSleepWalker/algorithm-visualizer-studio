# Algorithm Visualizer Studio

Algorithm Visualizer Studio is a modern C++ desktop application for interactive step-by-step algorithm visualization.

The project focuses on clean architecture, algorithmic correctness, testability, and graphical representation of algorithm states.

## Planned Features

- Graph algorithm visualization
  - Dijkstra
  - A*
  - Traveling Salesman Problem
  - Hamiltonian Path

- Sorting algorithm visualization
  - Bubble Sort
  - Quick Sort
  - Heap Sort
  - Merge Sort

- Array and statistics algorithms
  - Kadane's Algorithm
  - Linear Search
  - Binary Search
  - Interpolation Search

- Interactive execution control
  - Start
  - Pause
  - Stop
  - Step forward
  - Step backward
  - Speed control

## Technology Stack

- C++20 / C++23
- CMake
- Qt 6
- GoogleTest
- GitHub Actions

## Architecture Goals

- Separation of algorithm logic from GUI
- Testable algorithm core
- Step-based algorithm execution
- Timeline-based visualization
- Clean module boundaries
- SOLID-oriented design

## Repository Structure

```text
app/             Application entry point
core/            Algorithms and domain models
visualization/   Visualization state and step mapping
ui/              GUI layer
tests/           Unit tests
docs/            Documentation
tools/           Helper scripts
assets/          Icons, examples and sample data
