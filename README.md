## Overview

TL;DR: The bumpy ride of Vulkan Learning.

The *classic* tutorial, named Vulkan Tutorial, is a semi-great place to code in Vulkan, but it comes with a horrific code structure: *everything in one file*.

Hence, the repo tries to mitigate this:

- Vulkan objects will be encapsulated in custom wrappers, and usually they have implicit conversion toward the raw Vulkan object. The wrappers follow RAII.
- Long methods are chopped into smaller chunks
- Better interdependencies between objects

We also use Vulkan Guide as a supplementary reference.

A homebrew OOP version (more specifically, custom RAII wrappers from scratch), is potentially not optimal compared to the official RAII but should allow us to understand better the inner workings of the API.

The repo goes slightly beyond the tutorials by also implementing:

- Player loop updates driven by flecs ECS
- Shader reflection
- Multi-window support
- Deferred lighting supporting PBR rendering


## Installation

The primary IDE used during the development is *CLion*, but *Visual Studio 2022* should work as well. We have only tested building with **MSVC**.

We use *CMake* as our build system.

The repo has changed to use **vcpkg** for package management. Follow [Tutorial: Install and use packages with CMake](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell) to configure vcpkg on your system.

## Architecture

Some guides and explanations on consolidated architectural choices. (Some architectural choices are under revision, and are not yet presented).

### Table of Contents

0.  [Style Guide](documentations/engine/Style-Guide.md) : How are classes, enums, functions named, as well as common macro utilities.
1.  [Descriptor Management](documentations/engine/Descriptor-Management.md) : The engine's policy for descriptor allocation.

## Topics

Here, we discuss some less obvious concepts encountered during the learning process. These are usually quite lengthy and have been moved to dedicated files for clarity.

## Table of Contents

1.  [Vulkan Initialization and Surfaces](documentations/articles/Vulkan-Surfaces-and-Init.md)
2.  [Descriptor Set Layout, Descriptor Pool, Descriptor Set](documentations/articles/Descriptor-Management.md)
3.  [Pipeline Layout, Pipeline, Descriptor Sets  (WIP)](documentations/articles/Pipeline-Management.md)
4.  [Render Pass and Associated Resources](documentations/articles/RenderPass-Resource-Management.md)
5.  [Attachment Referencing](documentations/articles/Attachment-Referencing.md)
6.  [Render Graph Overview (WIP)](documentations/articles/Granite-Render-Graph.md)
7.  [Unreal Engine's Render Dependency Graph (WIP)](documentations/articles/UnrealEngine-RDG.md)


