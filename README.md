# RasterTek

<p align="center"><img src="res/Tutorial 49.png"></p>

## About

The source code of [RasterTek](http://www.rastertek.com/tutdx11.html)'s DirectX 11 tutorials (made ready to use in Visual Studio 2017). Documentation and further explanation can be found at [RasterTek](http://www.rastertek.com/tutdx11.html).

I am not backing all the (software) design decisions nor the used conventions, but the tutorials get the things done, obtain very pleasing results and are inspiring for people wanting to grasp the rendering pipeline (which is for most special effects less intuitive as opposed to ray tracing).

These tutorials require the obsolete [DirectX 11 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812) which is very similar to the DirectX 11 component contained in the Windows SDK. With the exception of the use of `D3DXMath` (for which [`DirectXMath`](https://github.com/Microsoft/DirectXMath) is a superior and widely used alternative), nearly all code would remain the same and is still relevant. For more info, I refer to [Chuck Walbourn](https://github.com/walbourn)'s [Living without D3DX](https://walbourn.github.io/living-without-d3dx/).

## Use
1. Install the [DirectX 11 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812). *(It is possible that the installer ends with an error code. Nothing to worry about, since I think this is Microsoft's way of notifying the user of installing obsolete Microsoft Software. Roughly stated: if `C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)` is present and contains data the installation succeeded.)*
2. Make sure the `DXSDK_DIR` environment variable is set. *(If you also use the obsolete DirectX 9 SDK, you need to decide whether `DXSDK_DIR` points to the DirectX 9 SDK or DirectX 11 SDK.)*
3. You can now open all `.sln` files and compile the projects.
4. Have fun!
