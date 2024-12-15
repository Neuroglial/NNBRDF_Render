# 基于PBR模型的实时渲染引擎

## 项目简介

本项目旨在实现一个基于**物理渲染基础（PBR）模型**的实时渲染引擎。  
通过采用现代计算机图形学技术，实现真实感渲染效果，并支持复杂场景的交互与动态光照。

## 功能特性

- **PBR 渲染模型**：
  - 支持 Cook-Torrance 光照模型。
  - 实现金属度、粗糙度、环境光遮蔽（AO）等纹理映射。
  - 基于 IBL（图像基照明）的环境反射技术。
  
- **高效渲染**：
  - 支持延迟渲染技术，优化多光源处理性能。
  - 实现 HDR 渲染和后期处理效果（如 Bloom、景深、抗锯齿等）。

- **引擎模块化设计**：
  - 提供统一的底层 API 接口，兼容 OpenGL、Vulkan 和 DirectX。
  - 资源管理模块，支持 Mesh 导入、纹理加载和场景管理。

- **交互与优化**：
  - 支持射线检测、包围盒和八叉树技术，用于场景交互和性能优化。
  - 提供直观友好的用户界面（UI）。

## 技术实现

1. **底层图形 API 封装**：  
   屏蔽底层图形接口差异，为用户提供一致的渲染调用方式。
   
2. **PBR 光照模型**：  
   实现基于物理的光照计算，支持辐射度和能量守恒。
   
3. **延迟渲染**：  
   在多光源场景中提高渲染效率，适用于复杂的动态光照计算。
   
4. **后期处理**：  
   - HDR 渲染和曝光调整。
   - Bloom 和景深效果。
   - 抗锯齿（MSAA/FXAA）支持。

5. **资源管理与加载**：  
   支持模型（OBJ、FBX）和纹理（PNG、HDR）的高效加载。

6. **场景交互与优化**：  
   实现交互事件处理，使用八叉树提升场景性能。

## 使用说明

### 环境要求

- C++ 17 或以上版本。
- 支持 OpenGL 4.5 的显卡。
- 必须安装 CMake 3.15 或以上。


### 开发进度

2024-12-10 OpenGL渲染平台搭建完毕，黑洞渲染测试：

![BlackHole](https://github.com/Neuroglial/NNBRDF_Render/blob/main/resource/image/NNBRDF_Render%202024-12-10%2023-29-06.gif)



## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.