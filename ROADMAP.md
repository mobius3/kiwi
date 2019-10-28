# KiWi Roadmap

This is a roadmap for KiWi and its future releases.

## 1.0.0

This list is ordered by most-needed and sometimes by easier to do. All of them
are needed to reach version 1.0.0. Things might get added periodically.

- [ ] Core (by priority)
  - [ ] Headers should'nt be prefixed by `KW_` or anything else (specially
        widgets)
  - [ ] `include` and `src` folders must be separated
  - [ ] Support for allocators at `KW_GUI` construction
  - [ ] Function names and placement (which header file) should be consistent
  - [ ] Get/Set functions should only get and set values
    - Exception is calling *changed handlers*
  - [ ] Users should be capable of changing the position of their tiles in the
        tileset
  - [ ] Users should be capable of using tilesets with other tile width and
        height
  - [ ] Stop using custom KIWI_EXPORTS macro and use CMake's generated one
  - [ ] SDL2 render driver should be decoupled and replaceable
  - [ ] Current render-driver API is too low level, it must be easier to
        implement
  - [ ] KiWi must not allocate large chunks of data (specially render data)
  - [ ] KiWi must not allocate memory frequently
  - [ ] All include files should be lowercase
  - [ ] Add `c-rez` instead of older resource generator
- [ ] Widgets (by value and how easy to write)
  - [ ] Checkbox
  - [ ] Radio (with radio group/automatic check control)
  - [ ] Progress bar
  - [ ] Slider
  - [ ] Single-selection Dropdown (with filter/auto-complete) (reuse radio?)
  - [ ] Editbox/Multi-line text box
  - [ ] Multi-selection dropdown (reuse checkbox?)
  - [ ] Table
  - [ ] (what else?)
- [ ] Documentation (by most needed)
  - [ ] All documentation should be consistent
  - [ ] All public functions and types should have documentation
  - [ ] Users should have an install guide for every platform
  - [ ] Users should know how to *include* and *link* against pre-built binaries
        on every platform
    - [ ] Using CMake
      - [ ] On Linux
      - [ ] On Windows
      - [ ] On MacOS
    - [ ] Using plain Makefiles
    - [ ] Using VisualStudio
    - [ ] Using XCode
      - [ ] For MacOS
      - [ ] For iOS
    - [ ] Using MinGW
    - [ ] Using NDK
  - [ ] All complex functions should have examples
  - [ ] Documentation must be available publicly and should be automatically
        generated
  - [ ] The library needs a website or homepage
     - [ ] Needs logo
- [ ] Pacakging
  - [ ] Generated library should be called `kiwi.so/dll` and not `KiWi.so/dll`
  - [ ] Core and Widgets should be separated in two libraries (and include
        files)
      - [ ] `#include "kiwi/core.h"`
         - Everything non-widget related
         - Must have the "core" subfolder (eg, `#include "kiwi/core/gui.h"`)
      - [ ] `#include "kiwi/widgets.h"`
         - Everything widget related
      -    Must have the "widgets" subfolder (eg, `#include
           "kiwi/widgets/frame.h"`)
     - [ ] Aliases `kiwi::core` and `kiwi::widgets`
  - [ ] `kiwi-config.cmake` needs to work and be up to date with modern CMake
        practices
  - [ ] `kiwi-config.cmake` needs to have its dependencies properly expressed
  - [ ] Examples should go in their own repository
  - [ ] Generated library should have versioning in its name, except for Windows
  - [ ] Every release needs source and binary packages publicly available
     - `kiwi-source-${version}` packages for Linux, MacOS and Windows
    -  `kiwi-${version}-${platform-target}-${compiler-target}` with prebuilt
       binaries for Linux, MacOS, iOS, Android and Windows (eg,
       `kiwi-1.0.0-win64-vc2019.zip`)
        - The used SDL version when linking needs to be clear in release notes
  - [ ] autoconf's `.pc` files must be generated
- [ ] Bindings
  - [ ] Check chicken bindings and fix them
    - Might be necessary asking help from author
  - [ ] Check D bindings and fix them
    - Might be necessary asking help from author

## Wishlist

Things here might become part of a roadmap of a future release. They are in no
specific order.

- Create a script to automatically generate new releases
- Provide a pure Vulkan render driver
- Replace SDL_ttf with custom text renderer
- Lua bindings