# What is Fusion?

Fusion is a clean, modern Vulkan rendering framework written in C++.

It was originally developed as the early graphics foundation for FranchukOS, where it powered experimental desktop rendering, GPU acceleration tests, and early compositor prototypes. Fusion began as an internal graphics layer designed to give FranchukOS direct, explicit control over the GPU without relying on large external engines.

Instead of being a full game engine, Fusion focuses on:

- High-level Vulkan orchestration

- Frame management and synchronization

- Render graph execution

- Pipeline and GPU resource control

- Minimal abstraction without hiding Vulkan

Fusion exists to provide powerful, explicit GPU control while remaining clean, modular, and engine-agnostic.
