Package: ffmpeg[avcodec,avdevice,avfilter,avformat,core,gpl,swresample,swscale,x264]:x64-windows@7.1.1#5

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.44.35217.0
- CMake Version: 3.30.1
-    vcpkg-tool version: 2025-09-03-4580816534ed8fd9634ac83d46471440edd82dfe
    vcpkg-scripts version: 29ff5b8131 2025-09-19 (7 weeks ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Using cached ffmpeg-ffmpeg-n7.1.1.tar.gz
-- Cleaning sources at D:/vcpkg/buildtrees/ffmpeg/src/n7.1.1-92d93b4c20.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source D:/vcpkg/downloads/ffmpeg-ffmpeg-n7.1.1.tar.gz
-- Applying patch 0001-create-lib-libraries.patch
-- Applying patch 0002-fix-msvc-link.patch
-- Applying patch 0003-fix-windowsinclude.patch
-- Applying patch 0004-dependencies.patch
-- Applying patch 0005-fix-nasm.patch
-- Applying patch 0007-fix-lib-naming.patch
-- Applying patch 0013-define-WINVER.patch
-- Applying patch 0020-fix-aarch64-libswscale.patch
-- Applying patch 0024-fix-osx-host-c11.patch
-- Applying patch 0040-ffmpeg-add-av_stream_get_first_dts-for-chromium.patch
-- Applying patch 0041-add-const-for-opengl-definition.patch
-- Applying patch 0043-fix-miss-head.patch
-- Applying patch 0044-fix-vulkan-debug-callback-abi.patch
CMake Error at scripts/cmake/vcpkg_extract_source_archive.cmake:153 (file):
  file RENAME failed to rename

    D:/vcpkg/buildtrees/ffmpeg/src/n7.1.1-92d93b4c20.clean.tmp/FFmpeg-n7.1.1

  to

    D:/vcpkg/buildtrees/ffmpeg/src/n7.1.1-92d93b4c20.clean

  because: �������� � �������.



Call Stack (most recent call first):
  scripts/cmake/vcpkg_extract_source_archive_ex.cmake:8 (vcpkg_extract_source_archive)
  scripts/cmake/vcpkg_from_github.cmake:127 (vcpkg_extract_source_archive_ex)
  C:/Users/Алексей/AppData/Local/vcpkg/registries/git-trees/0988005f333aa87d797fb08535d33995f6ec302f/portfile.cmake:1 (vcpkg_from_github)
  scripts/ports.cmake:206 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    "fmt",
    "opencv4",
    {
      "name": "ffmpeg",
      "features": [
        "swresample",
        "swscale",
        "avdevice",
        "x264"
      ]
    }
  ]
}

```
</details>
