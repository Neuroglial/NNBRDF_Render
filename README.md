# Neural Network BRDF Renderer (基于OpenGL的神经网络BRDF渲染器)

## 项目简介
这是一个基于OpenGL的神经网络BRDF（双向反射分布函数）渲染器，用于在计算机图形学中实现更加真实的光照模型。项目利用神经网络学习和逼近复杂的BRDF模型，以生成高质量的渲染效果。

2024-12-10 OpenGL渲染平台搭建完毕，黑洞渲染测试：

![BlackHole](https://github.com/Neuroglial/NNBRDF_Render/blob/main/source/image/NNBRDF_Render%202024-12-10%2023-29-06.gif)


### 特点
- **神经网络驱动的BRDF**：通过训练神经网络来预测光照和材质反射行为，从而提升渲染效果的真实感。
- **基于OpenGL的渲染**：使用OpenGL进行实时图形渲染，支持现代GPU硬件加速。
- **高效的光照计算**：通过优化的BRDF计算方法，减少渲染时间并提高渲染效率。
- **支持多种材质和光源类型**：可以渲染不同材质的表面效果，并且支持点光源、平行光源等多种光源。
