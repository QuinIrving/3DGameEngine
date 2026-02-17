# 3D Software Rendering Engine - C++

## Overview

**3DGameEngine** is a lightweight, 3D software renderer written entirely in C++ from scratch. It implements a programmable graphics pipeline purely on the CPU without utilizing hardware acceleration APIs like OpenGL, DirectX, or Vulkan.

The goal of this project was to demystify the graphics pipeline by implementing every stage manually, from vertex processing and clipping to rasterization and fragment shading. It relies solely on the **C++ STL** and **stb_image** (for texture decoding), ensuring that all math, linear algebra, and rendering logic are custom implementations.

### Video Demo

https://raw.githubusercontent.com/QuinIrving/GithubVideos/blob/master/CPU_Renderer.mp4

----------

## Key Features

### The Pipeline

-   **Custom Math Library:** Implementation of vectors (Vec2, Vec3, Vec4) and matrices (Mat4) for handling transformations (Model, View, Projection).
    
-   **Rasterization:** Triangle rasterization using Barycentric coordinates with perspective-correct interpolation.
    
-   **Z-Buffering:** Depth testing for correct visibility and occlusion handling.
    
-   **Back-face Culling:** Optimization to discard non-visible geometry.
    
-   **Clipping:** Sutherland-Hodgman clipping against the view frustum.
    

### Shading & Visuals

-   **Phong Shading Model:** Per-pixel lighting including Ambient, Diffuse, and Specular components.
    
-   **Texture Mapping:** UV coordinate mapping with perspective correction.
    
-   **Normal Mapping:** Tangent space calculations for detailed surface relief.
    
-   **OBJ Model Loading:** Custom parser for standard `.obj` 3D model files.
    

### Interactivity

-   **FPS Camera:** Free-look camera system using Quaternions, and handles Euler angles (Pitch/Yaw).
-  **Key Remapper:** Able to dynamically remap function bindings to any key.
    

----------

## Technical Implementation

### Barycentric Rasterization

The core of the renderer uses a bounding-box approach. For every triangle, the bounding box is calculated, and pixels inside are tested using barycentric coordinates.

λ0​+λ1​+λ2​=1

This allows for easy interpolation of vertex attributes (colors, UVs, normals) across the triangle face.

### The Math

To avoid external dependencies like GLM, I implemented a custom linear algebra struct. This handles:

-   Homogeneous coordinates (w-divide).
    
-   Cross/Dot products for lighting calculations.
    
-   Tangents and Bitangents for normal mapping
