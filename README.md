## Deferred Rendering Demo
For a video demo of this project, check out https://youtu.be/dLGexoLvb0w

More lights

  ![image](https://github.com/MiaZhengLS/Deferred-Rendering/assets/121979781/e2b585fa-69db-4d9f-8cac-01ad5e75e037)

  
Fewer lights

  ![image](https://github.com/MiaZhengLS/Deferred-Rendering/assets/121979781/48473f4f-96a9-4ff0-8f3f-2b8337c977c8)


Position map

  ![image](https://github.com/MiaZhengLS/Deferred-Rendering/assets/121979781/58a6b8c7-352f-4c58-a055-0047d5cae070)


Normal map

  ![image](https://github.com/MiaZhengLS/Deferred-Rendering/assets/121979781/df928e01-962e-4607-bc00-015502a8df4a)


Albedo map

  ![image](https://github.com/MiaZhengLS/Deferred-Rendering/assets/121979781/06f60c95-fb77-41fc-93da-9ed1d9d45e46)


Specular map

 ![image](https://github.com/MiaZhengLS/Deferred-Rendering/assets/121979781/b1fe5165-74e9-484b-9809-e5689b39200f)


I made this project in CS5310: Computer Graphics. I learned the idea from https://learnopengl.com/Advanced-Lighting/Deferred-Shading.

Deferred shading aims to solve the issue that when there are many lights, the rendering performance drops with traditional forward shading. 

Forward shading calculates the fragment color for every light, which means the calculation burden keeps growing with the number of lights. Deferred shading divides the shading into two passes. 

In the first pass, which we refer to as "Geometry Pass", we calculate position, normal, albedo, specular and other information for each fragment and write them into textures separately. These textures are also called "G-buffer". They are essentially "screenshots" that store different information for each pixel. In the second pass, which we refer to as "Lighting Pass", we use the information from the buffer along with information from all the lights to calculate the final color for each fragment. This way, we make sure the pixel's color won't be overwritten by following light calculation. And the number of lights won't impact the performance since we only calculate once for all lights in the lighting pass.
