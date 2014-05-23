# The KiWi Roadmap

This is a list of planned features for KiWi and their development status.

Label: **done (bold)**, *in progress (italics)*, to do (no style).
 
### Core
- *System/general Events*
  - **Mouseover**
  - **Focus Gain/Lose**
  - *Text editing*
  - **Press/release**
  - **Drag (Start/Move/Stop)**
- *Widget events*
  - Geometry changed
  - Parent changed
  - Children added
- *Tile renderer*
  - **Fill an area with a tile**
  - **Render a frame based on a set of tiles**
  - Stretch tiles instead of multiple tile rendering if user decides so
- *Text renderer*
  - **Render UTF-8 text**
  - Render UTF-8 wrapped text
 

### Widgets
- *Frames*
  - **Daw a frame with a set of tiles**
  - Support for labeled frames (?)
- *Labels*
  - **Vertical alignment**
  - **Horizontal alignment**
  - Support for icons in labels
  - Label changed event
  - Multi-line (wrapped) labels
- *Buttons*
  - **Handle mouse events**
  - Handle touch events properly
  - ButtonPressed (according to core press event) and ButtonReleased (if core release or core focus lose while pressed?)
- *Editboxes*
  - **States for mouse over and clicked (active)**
  - Support for text replacing (insert key), replacing the cursor.
  - Text changed event
  - *Proper support for UTF-8*
  - *Support for TEXTEDITING events (i.e, Android swype, IME, etc)*
  - Multi-line editboxes. 
- Progress bars
  - Frame progress bar with filled/non-filled bg
  - Percent changed events.
- Sliders
- Tables
  - Check if really needed.
- Tabs/tabbed views 
  - KW_AddTabPage() API (decide the API)
  - Events for tab page changed, etc.
  - Decide on which tiles are needed.
- Checkboxes
  - Checked changed events
  - Tile for not checked, hover, checked, hover-checked, clicked, clicked-checked.
  - Display labels for each item
- Radio group and buttons
  - CurrentSelected changed events for the group
  - Checked changed events for the items
  - Tile for not checked, hover, checked, hover-checked, clicked, clicked-checked.
  - Display labels for each item
- *Scrollboxes*
  - **Clip children widgets to parent area** (done in the renderer)
  - **Clip events for clipped children**
  - *Implement the scrollbar*
  - *Implement scroll buttons*
  - Have scrollbars change width

### Layouts

- Core
  - Decide if they are widgets or separate things
  - Make layouts control their widgets geometries
  - Pros of being widgets:
    - Can have layouts as children widgets of other widgets
    - Can have layouts as children of layouts for nestedness greatness
  - Cons:
    - They aren't really widgets... or are they?
- Form layout
  - A two column by N lines grid 
- Grid layout
  - Can put widgets in any place in the grid
- Horizontal and vertical layouts
- Nested layouts
