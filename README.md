# RasterTek

## About

The source code of [RasterTek](http://www.rastertek.com/tutdx11.html)'s DirectX 11 tutorials (made ready to use in Visual Studio 2017). Documentation and further explanation can be found at [RasterTek](http://www.rastertek.com/tutdx11.html).

These tutorials require the obsolete [DirectX 11 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812) which is very similar to the DirectX 11 component contained in the Windows SDK. With the exception of the use of D3DXMath (for which DIrectXMath is a superior and widely used alternative), nearly all code would remain the same. For more info, I refer to [Chuck Walbourn](https://github.com/walbourn)'s [Living without D3DX](https://blogs.msdn.microsoft.com/chuckw/2013/08/20/living-without-d3dx/).

## Use
1. Install the [DirectX 11 SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812). (*It is possible that the installer ends with an error code. Nothing to worry about, since I think this is Microsoft's way of notifying the user of installing obsolete Microsoft Software. Roughly stated: if `C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)` is present and contains data the installation succeeded.*)
2. Make sure the `DXSDK_DIR` environment variable is set.
